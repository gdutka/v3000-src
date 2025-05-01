/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PlatformInfo.h>
#include <Library/BaseOemSvcChipsetLib.h>
#include <BoardConfigInit.h>

EC_INFO_DISPLAY_TABLE        mDisplayEcFunction[]       = {EC_INFO_DISPLAY_TABLE_LIST};


EFI_STATUS
GetEcVersion (
  IN     VOID                      *OpCodeHandle,
  IN     EFI_HII_HANDLE            MainHiiHandle,
  IN     EFI_HII_HANDLE            AdvanceHiiHandle,
  IN     CHAR16                    *StringBuffer  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINTN                         Index;
  
  for (Index = 0; mDisplayEcFunction[Index].DisplayEcFunction != NULL; Index++) {
    if (mDisplayEcFunction[Index].Option == DISPLAY_ENABLE) {
      ZeroMem(StringBuffer, 0x100);
      Status = mDisplayEcFunction[Index].DisplayEcFunction (OpCodeHandle, MainHiiHandle, AdvanceHiiHandle, StringBuffer);
    }
  }

  return Status;
}


EFI_STATUS
GetEcVerIdFunc (
  IN     VOID                      *OpCodeHandle,
  IN     EFI_HII_HANDLE            MainHiiHandle,
  IN     EFI_HII_HANDLE            AdvanceHiiHandle,
  IN     CHAR16                    *StringBuffer
  )
{
  EFI_STATUS          Status;
  UINT8               MajorNum;
  UINT8               MinorNum;
  EFI_STRING_ID       EcVersionText;
  EFI_STRING_ID       EcVersionString;
  EFI_STATUS          ReadEcVersionStatus;

  Status = EFI_UNSUPPORTED;
  MajorNum = 0;
  MinorNum = 0;
  ReadEcVersionStatus = EFI_UNSUPPORTED;
  
  DEBUG_OEM_SVC ((DEBUG_INFO, "Base OemChipsetServices Call:OemSvcEcVersion \n"));
  //Status = OemSvcEcVersion (&ReadEcVersionStatus, &MajorNum, &MinorNum);
  DEBUG_OEM_SVC ((DEBUG_INFO, "Base OemChipsetServices OemSvcEcVersion  Status: %r\n", Status));

  if (!EFI_ERROR(ReadEcVersionStatus)) {
    //
    // Update EC Version to SCU
    //
    UnicodeSPrint (StringBuffer, 0x100, L"%x.%02x", (UINTN)MajorNum, (UINTN)MinorNum);
    EcVersionString=HiiSetString (MainHiiHandle, 0, StringBuffer, NULL);
  } else {
    //
    // Update Default EC Version to SCU
    //
    NewStringToHandle (
      AdvanceHiiHandle,
      STRING_TOKEN (STR_EC_VERSION_STRING),
      MainHiiHandle,
      &EcVersionString
      );
  }

    NewStringToHandle (
      AdvanceHiiHandle,
      STRING_TOKEN (STR_EC_VERSION_TEXT),
      MainHiiHandle,
      &EcVersionText
      );
  
  HiiCreateTextOpCode (OpCodeHandle,EcVersionText, 0, EcVersionString );

  return ReadEcVersionStatus;
}


EFI_STATUS
GetBoardFunc (
  IN     VOID                      *OpCodeHandle,
  IN     EFI_HII_HANDLE            MainHiiHandle,
  IN     EFI_HII_HANDLE            AdvanceHiiHandle,
  IN     CHAR16                    *StringBuffer
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;
  EFI_STRING_ID  BoardIdText;
  EFI_STRING_ID  BoardIdString;
  BOARD_PRIVATE_DATA               *BoardInfo;
  UINT16                           BoardId;

  BoardInfo = (BOARD_PRIVATE_DATA *) PcdGetPtr (PcdBoardPrivateData);
  BoardId = BoardInfo->BoardId;
  switch (BoardId) {

    case BoardIdCannonlakeUDdr4Rvp:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL U DDR4 RVP");
      break;

    case BoardIdCannonlakeULpddr4Rvp:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL U LPDDR4 RVP");
      break;

    case BoardIdCannonlakeULpdimmDvp:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL U LPDIMM DVP");
      break;

    case BoardIdCannonlakeUSds:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL U SDS");
      break;

    case BoardIdCannonLakeYLpddr3:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL Y LPDDR3");
      break;

    case BoardIdCannonLakeYLpddr4:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL Y LPDDR4");
      break;

    case BoardIdCannonLakeHDdr4:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL H DDR4");
      break;

    case BoardIdCannonLakeHLpddr4:
      UnicodeSPrint (StringBuffer, 0x100, L"CNL H LPDDR4");
      break;
      
    case BoardIdCoffeeLakeULpddr3CpuBrd:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL U LPDDR3 CPU Board");
      break;

    case BoardIdCoffeeLakeULpddr3CompBrd:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL U LPDDR3 COMP Board");
      break;

    case BoardIdCoffeeLakeUDdr4:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL U DDR4");
      break;

    case BoardIdCoffeeLakeULpddr3CpuBrdCrb:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL U LPDDR3 CPU BOARD CRB");
      break;      

    case BoardIdCoffeeLakeHDdr4:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL H DDR4");
      break;

    case BoardIdCoffeeLakeHDdr4mDvpCfgC:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL H DDR4M DVP CFG C");
      break;

    case BoardIdCoffeeLakeHDdr4mDvpCfgD:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL H DDR4M DVP CFG D");
      break;      

    case BoardIdCoffeeLakeSUdimm:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL S UDIMM");
      break;

    case BoardIdCoffeeLakeSSodimm:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL S SODIMM");
      break;

    case BoardIdCoffeeLakeOCUdimm:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL OC UDIMM");
      break;

    case BoardIdCoffeeLakeS82Udimm:
      UnicodeSPrint (StringBuffer, 0x100, L"CFL S UDIMM 82");
      break;      
    case BoardIdWhiskeyLakeErb:
      UnicodeSPrint (StringBuffer, 0x100, L"WHL ERB");
      break;

    default:
      break;
  }
 
  BoardIdString = HiiSetString (MainHiiHandle, 0, StringBuffer, NULL);
  
  NewStringToHandle (
    AdvanceHiiHandle,
    STRING_TOKEN (STR_BOARD_ID_TEXT),
    MainHiiHandle,
    &BoardIdText
    );

  HiiCreateTextOpCode (OpCodeHandle, BoardIdText, 0, BoardIdString );

  return Status;
}


EFI_STATUS
GetFabFunc (
  IN     VOID                      *OpCodeHandle,
  IN     EFI_HII_HANDLE            MainHiiHandle,
  IN     EFI_HII_HANDLE            AdvanceHiiHandle,
  IN     CHAR16                    *StringBuffer
  )
{
  EFI_STATUS     Status = EFI_SUCCESS;
  EFI_STRING_ID  FabIdText;
  EFI_STRING_ID  FabIdString;
  UINT8          BoardFabId;
  UINT8          FabId = 0;

  BoardFabId = PcdGet8 (PcdBoardRev);

  switch (BoardFabId) {
    case CRB_BOARD_ID_FAB_ID_FAB2:
      FabId = FAB2_ID;
      break;

    case CRB_BOARD_ID_FAB_ID_FAB3:
      FabId = FAB3_ID;
      break;

    case CRB_BOARD_ID_FAB_ID_FAB4:
      FabId = FAB4_ID;
      break;

    default:
      break;
  }

  UnicodeSPrint (StringBuffer, 0x100, L"%x", FabId);
      
  FabIdString = HiiSetString (MainHiiHandle, 0, StringBuffer, NULL);
  
  NewStringToHandle (
    AdvanceHiiHandle,
    STRING_TOKEN (STR_FAB_ID_TEXT),
    MainHiiHandle,
    &FabIdText
    );

  HiiCreateTextOpCode (OpCodeHandle, FabIdText, 0, FabIdString);

  return Status;
}
