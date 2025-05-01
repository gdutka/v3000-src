/** @file

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/SimpleTextOut.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>               // MdeModulePkg

/**
**/
STATIC
UINTN
GetConversionSpecificationLength (
  IN CONST CHAR16  *ConversionSpecification
  )
{
  UINTN  Index;

  if (ConversionSpecification == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The ConversionSpecification is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  Index = 0;
  if (ConversionSpecification[Index] != L'%') {
    return 0;
  }
  Index++;

  if (ConversionSpecification[Index] == L'%') {
    Index++;
    return Index;
  }

  //
  // Flags.
  // Apostrophe and '#' are not supported in EDK II PrintLib.
  // The comma flag is specific to EDK II, which prints digit (thousand)
  // separators. The equivalent bahavior is available in POSIX as apostrophe.
  //
  while (TRUE) {
    switch (ConversionSpecification[Index]) {

    case L',':
    case L'-':
    case L'+':
    case L' ':
    case L'0':
      Index++;
      continue;
    }
    break;
  }

  //
  // Field width
  //
  if (ConversionSpecification[Index] == L'*') {
    Index++;
  } else {
    while (ConversionSpecification[Index] >= L'0' &&
        ConversionSpecification[Index] <= L'9')
    {
      Index++;
    }
  }

  //
  // Precision
  //
  if (ConversionSpecification[Index] == L'.') {
    if (ConversionSpecification[Index] == L'*') {
      Index++;
    } else {
      while (ConversionSpecification[Index] >= L'0' &&
          ConversionSpecification[Index] <= L'9')
      {
        Index++;
      }
    }
  }

  //
  // Length modifier.
  // EDK II PrintLib only supports 'L' and 'l'.
  // ISO C 'hh', 'h', 'll', 'j', 'z' and 't' are not supported.
  // Note that 't' in EDK II PrintLib is a conversion specifier, not a length
  // modifier.
  //
  switch (ConversionSpecification[Index]) {

  case L'l':
  case L'L':
    Index++;
    break;
  }

  //
  // Conversion specifier.
  // Note: 'i' and 'o' are unsupported in EDK II PrintLib.
  //
  switch (ConversionSpecification[Index]) {

  case 'c':
  case 's': case 'S':
  case 'd':
  case 'x': case 'X':
  case 'u':
  case 'p':
  //
  // Conversion specifiers specific to EDK II. Note that 'a', 'g' and 't' have
  // different semantics from ISO C standard printf().
  //
  case 'a':
  case 'g':
  case 't':
  case 'r':
    break;

  default:
    //
    // Invalid format specifier yields undefined bahavior. PrintLib prints the
    // conversion specifier character literally; we still increment one
    // character length for it.
    //
    ASSERT (FALSE);
  }

  if (ConversionSpecification[Index] != L'\0') {
    Index++;
  }
  return Index;
}

/**
  Writes a string to the console until the end of the console row is reached or
  a line break is found.

  @implements H2O_CONSOLE_BOOT_MESSAGE_OUTPUT_LINE

  @param[in,out] Console       An output console (a Simple Text Output Protocol
                               instance).
  @param[in]     MaxColumn     Maximum column count for string output. This
                               value must not be greater than the number of
                               columns of the current mode of Console.
  @param[in,out] StringBuffer  The Null-terminated string to be displayed on
                               Console. The string buffer must be mutable. (The
                               functions modifies the string internally during
                               the process and restores to the original string
                               on return.)

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsoleOutputLine (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN OUT CHAR16                           *StringBuffer
  )
{
  BOOLEAN     IsEndOfLine;
  BOOLEAN     PadSpace;
  UINTN       Index;
  UINTN       CharCount;
  INT32       Row;
  INT32       Column;
  INT32       DisplayLength;
  CHAR16      Char;
  INT32       Width;
  EFI_STATUS  Status;

  if (MaxColumn >= MAX_INT32) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The MaxColumn exceeds the limit, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  if ((StringBuffer == NULL) || (Console == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The StringBuffer/Console is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  if (Console->Mode == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Error!! The Console->Mode is NULL\n",
      __FUNCTION__
      ));
    return 0;
  }

  IsEndOfLine = FALSE;
  PadSpace = FALSE;
  Index = 0;
  CharCount = 0;
  Row = Console->Mode->CursorRow;

  while ((!IsEndOfLine) && StringBuffer[Index] != L'\0' &&
      Console->Mode->CursorRow == Row &&
      Console->Mode->CursorColumn < (INT32) MaxColumn)
  {
    //
    // If we can predict the display length of the characters (according to the
    // width table in UefiLib), print them together in one OutputString()
    // routine. Otherwise, print them character by character.
    //
    Column = Console->Mode->CursorColumn;
    DisplayLength = 0;
    while (StringBuffer[Index] != L'\0') {
      ASSERT (CharCount <= Index);
      Char = StringBuffer[Index];

      if (Char == L'\r' && StringBuffer[Index + 1] == L'\n') {
        IsEndOfLine = TRUE;
        Index += 2;
        break;
      }
      if ((Char >= L'\n' && Char <= L'\r') || // LF, VT, FF and CR
          (UINT16) Char == 0x0085 ||          // Next Line (NEL)
          (UINT16) Char == 0x2028 ||          // Line Separator
          (UINT16) Char == 0x2029)            // Paragraph Separator
      {
        IsEndOfLine = TRUE;
        Index++;
        break;
      }

      //
      // C0 and C1 control codes. GetGlyphWidth() from UefiLib would return
      // wrong width for these.
      //
      if (((UINT16) Char & (~0x0080)) < 0x001F || (UINT16) Char == 0x007F) {
        Index++;
        break;
      }

      //
      // UTF-16 surrogate pairs
      //
      if ((UINT16) Char >= 0xD800 && (UINT16) Char <= 0xDBFF) {
        if (CharCount == Index) {
          Char = StringBuffer[++Index];
          if ((UINT16) Char >= 0xDC00 && (UINT16) Char <= 0xDFFF) {
            Index++;
          }
        }
        break;
      }

      Width = (INT32) GetGlyphWidth (Char);
      if (Width <= 0) {
        if (CharCount == Index) {
          Index++;
        }
        break;
      }
      DisplayLength += Width;
      if (Column + DisplayLength >= (INT32) MaxColumn) {
        IsEndOfLine = TRUE;
        if (Column + DisplayLength == (INT32) MaxColumn) {
          Index++;
        } else {
          PadSpace = TRUE;
        }
        break;
      }
      Index++;
    }

    //
    // Output the substring
    //
    Char = StringBuffer[Index];
    StringBuffer[Index] = L'\0';
    Status = Console->OutputString (Console, &StringBuffer[CharCount]);
    StringBuffer[Index] = Char;
    if (Status != EFI_SUCCESS) {
      DEBUG ((
        ((EFI_ERROR (Status)) ? EFI_D_ERROR : EFI_D_WARN),
        "OutputString (%p) status: %r\n",
        Console,
        Status
        ));
      if (EFI_ERROR (Status)) {
        return CharCount;
      }
    }
    CharCount = Index;
  }
  if (PadSpace) {
    Status = Console->OutputString (Console, L" ");
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "OutputString (%p) status: %r\n", Console, Status));
      return CharCount;
    }
  }
  if (Console->Mode->CursorColumn >= (INT32) MaxColumn) {
    Status = Console->OutputString (Console, L"\r\n");
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "OutputString (%p) status: %r\n", Console, Status));
    }
  }
  return CharCount;
}

/**
  Writes a string to the console within a set limit of columns and rows.

  If the string reaches the column limit when displayed, the string wraps to
  the next row, and then, if the cursor exceeds the row limit, it moves to the
  first row. The string continues from new cursor position.

  This function does not interfere with the automatic scrolling feature of EFI
  Simple Text Output protocol.

  @param[in,out] Console       An output console (a Simple Text Output Protocol
                               instance).
  @param[in]     MaxColumn     Maximum column count for string output. This
                               value must not be greater than the number of
                               columns of the current mode of Console.
  @param[in]     MaxRow        Maximum row count for string output. This value
                               must not be greater than the number of rows of
                               the current mode of Console.
  @param[in,out] StringBuffer  The Null-terminated string to be displayed on
                               Console. The string buffer must be mutable. (The
                               functions modifies the string internally during
                               the process and restores to the original string
                               on return.)

  @return The number of Unicode characters printed to Console.

**/
STATIC
UINTN
OutputStringWithPageWrapping (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN OUT CHAR16                           *StringBuffer
  )
{
  UINTN       TotalChars;
  INT32       Row;
  EFI_STATUS  Status;
  UINTN       CharCount;

  if ((MaxColumn >= MAX_INT32) || (MaxRow >= MAX_INT32)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The MaxColumn/MaxRow exceeds the limit, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  if ((StringBuffer == NULL) || (Console == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The StringBuffer/Console is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  if (Console->Mode == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Error!! The Console->Mode is NULL\n", __FUNCTION__));
    return 0;
  }

  TotalChars = 0;

  while (StringBuffer[TotalChars] != L'\0') {
    Row = Console->Mode->CursorRow;
    if (Console->Mode->CursorColumn >= (INT32) MaxColumn ||
        Row >= (INT32) MaxRow)
    {
      if (Console->Mode->CursorColumn >= (INT32) MaxColumn) {
        Row++;
      }
      if (Row >= (INT32) MaxRow) {
        Row = 0;
      }
      Status = Console->SetCursorPosition (
                          Console,
                          0, // Column
                          (UINTN) Row
                          );
      if (EFI_ERROR (Status)) {
        DEBUG ((
          EFI_D_ERROR,
          "SetCursorPosition (%p) status: %r\n",
          Console,
          Status
          ));
        break;
      }
    }
    CharCount = ConsoleOutputLine (
                  Console,
                  MaxColumn,
                  &StringBuffer[TotalChars]
                  );
    if (CharCount <= 0) {
      break;
    }
    TotalChars += CharCount;
  }
  return TotalChars;
}

/**
  Prints a formatted Unicode string to a console, with line wrapping and
  optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in,out] FormatBuffer
  @param[in]     Marker

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsoleVaPrint (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN OUT CHAR16                           *FormatBuffer,
  IN VA_LIST                              Marker
  )
{
  UINTN         TotalChars;
  VA_LIST       ExtraMarker;
  CHAR16        *OutputBuffer;
  UINTN         FormatCharIndex;
  UINTN         AttributeIndex;
  UINTN         Length;
  EFI_STATUS    Status;
  CHAR16        Char;
  UINTN         CharCount;

  if ((FormatBuffer == NULL) || (Console == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The FormatBuffer/Console is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  TotalChars = 0;

  VA_COPY (ExtraMarker, Marker);
  OutputBuffer = CatVSPrint (NULL, FormatBuffer, ExtraMarker);
  VA_END (ExtraMarker);
  if (OutputBuffer == NULL) {
    DEBUG ((EFI_D_ERROR, "%a(): Out of memory\n", __FUNCTION__));
    return TotalChars;
  }

  FormatCharIndex = 0;
  AttributeIndex = 0;
  while (FormatBuffer[FormatCharIndex] != L'\0') {
    if (FormatBuffer[FormatCharIndex] == L'%' &&
        FormatBuffer[FormatCharIndex + 1] != L'%')
    {
      //
      // L'%' Begins a conversion specification, but we don't set attribute for
      // the L"%%" specification that outputs a single L'%'.
      //
      Length = GetConversionSpecificationLength (
                 &FormatBuffer[FormatCharIndex]
                 );
      ASSERT (Length > 0);
      FormatCharIndex += Length;
    } else {
      while (FormatBuffer[FormatCharIndex] != L'\0') {
        if (FormatBuffer[FormatCharIndex] == L'%') {
          if (FormatBuffer[FormatCharIndex + 1] != L'%') {
            break;
          }
          FormatCharIndex++;
        }
        FormatCharIndex++;
      }
    }

    Char = FormatBuffer[FormatCharIndex];

    if (AttributeIndex < AttributeCount) {
      if (Attributes == NULL) {
        DEBUG ((
          EFI_D_ERROR,
          "%a() - The Attributes is NULL, please check input Param\n",
          __FUNCTION__
          ));
        break;
      }
      Status = Console->SetAttribute (Console, Attributes[AttributeIndex]);
      if (EFI_ERROR (Status)) {
        DEBUG ((
          EFI_D_ERROR,
          "SetAttribute (%p) status: %r\n",
          Console,
          Status
          ));
        break;
      }
      if ((++AttributeIndex) < AttributeCount) {
        FormatBuffer[FormatCharIndex] = L'\0';
      }
    }

    VA_COPY (ExtraMarker, Marker);
    Length = SPrintLength (FormatBuffer, ExtraMarker);
    VA_END (ExtraMarker);

    FormatBuffer[FormatCharIndex] = Char;

    if (TotalChars < Length) {
      Char = OutputBuffer[Length];
      OutputBuffer[Length] = L'\0';
      CharCount = OutputStringWithPageWrapping (
                    Console,
                    MaxColumn,
                    MaxRow,
                    &OutputBuffer[TotalChars]
                    );
      OutputBuffer[Length] = Char;
      TotalChars += CharCount;
      if (TotalChars < Length) {
        break;
      }
    }
    if (AttributeIndex >= AttributeCount) {
      break;
    }
  }

  FreePool (OutputBuffer);

  return TotalChars;
}

/**
  Prints a formatted Unicode string to a console, with line wrapping and
  optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_PRINT

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in]     Format
  @param[in]     ...

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsolePrint (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN CONST CHAR16                         *Format,
  ...
  )
{
  VA_LIST  Marker;
  UINTN    CharCount;
  CHAR16   *FormatBuffer;

  if (Format == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Format is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  VA_START (Marker, Format);

  CharCount = 0;
  FormatBuffer = AllocateCopyPool (StrSize (Format), Format);
  if (FormatBuffer == NULL) {
    DEBUG ((EFI_D_ERROR, "%a(): Out of memory\n", __FUNCTION__));
    return CharCount;
  }

  CharCount = ConsoleVaPrint (
                Console,
                MaxColumn,
                MaxRow,
                AttributeCount,
                Attributes,
                FormatBuffer,
                Marker
                );

  FreePool (FormatBuffer);

  VA_END (Marker);

  return CharCount;
}

/**
  Prints a formatted string as specified in a HII string package to a console,
  with line wrapping and optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_VA_PRINT_HII_STRING

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in]     HiiHandle
  @param[in]     FormatStringId
  @param[in]     Language
  @param[in]     Marker

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsoleVaPrintHiiString (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN EFI_STRING_ID                        FormatStringId,
  IN CONST CHAR8                          *Language OPTIONAL,
  IN VA_LIST                              Marker
  )
{
  EFI_STRING  FormatBuffer;
  UINTN       CharCount;

  FormatBuffer = HiiGetString (HiiHandle, FormatStringId, Language);
  if (FormatBuffer == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "HiiGetString (Handle=%p, Id=0x%x, Lang=%a) failed.\n",
      HiiHandle,
      (unsigned int) FormatStringId,
      ((Language != NULL) ? Language : "(null)")
      ));
    return 0;
  }

  CharCount = ConsoleVaPrint (
                Console,
                MaxColumn,
                MaxRow,
                AttributeCount,
                Attributes,
                (CHAR16 *) FormatBuffer,
                Marker
                );

  FreePool (FormatBuffer);

  return CharCount;
}

/**
  Prints a formatted string as specified in a HII string package to a console,
  with line wrapping and optional color attributes.

  @implements H2O_CONSOLE_BOOT_MESSAGE_PRINT_HII_STRING

  @param[in,out] Console         An output console (a Simple Text Output
                                 Protocol instance).
  @param[in]     MaxColumn       Maximum column count for string output. This
                                 value must not be greater than the number of
                                 columns of the current mode of Console.
  @param[in]     MaxRow          Maximum row count for string output. This
                                 value must not be greater than the number of
                                 rows of the current mode of Console.
  @param[in]     AttributeCount
  @param[in]     Attributes
  @param[in]     HiiHandle
  @param[in]     FormatStringId
  @param[in]     Language
  @param[in]     ...

  @return The number of Unicode characters printed to Console.

**/
UINTN
EFIAPI
ConsolePrintHiiString (
  IN OUT EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                                MaxColumn,
  IN UINTN                                MaxRow,
  IN UINTN                                AttributeCount,
  IN CONST INT32                          *Attributes OPTIONAL,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN EFI_STRING_ID                        FormatStringId,
  IN CONST CHAR8                          *Language OPTIONAL,
  ...
  )
{
  VA_LIST  Marker;
  UINTN    CharCount;

  VA_START (Marker, Language);
  CharCount = ConsoleVaPrintHiiString (
                Console,
                MaxColumn,
                MaxRow,
                AttributeCount,
                Attributes,
                HiiHandle,
                FormatStringId,
                Language,
                Marker
                );
  VA_END (Marker);

  return CharCount;
}
