/*****************************************************************************
 *
 * Copyright 2008 - 2021 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 * AMD is granting You permission to use this software and documentation (if
 * any) (collectively, the "Software") pursuant to the terms and conditions of
 * the Software License Agreement included with the Software. If You do not have
 * a copy of the Software License Agreement, contact Your AMD representative for
 * a copy.
 *
 * You agree that You will not reverse engineer or decompile the Software, in
 * whole or in part, except as allowed by applicable law.
 *
 * WARRANTY DISCLAIMER: THE SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND. AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, QUALITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT AND WARRANTIES
 * ARISING FROM CUSTOM OF TRADE OR COURSE OF USAGE WITH RESPECT TO THE SOFTWARE,
 * INCLUDING WITHOUT LIMITATION, THAT THE SOFTWARE WILL RUN UNINTERRUPTED OR
 * ERROR-FREE. THE ENTIRE RISK ASSOCIATED WITH THE USE OF THE SOFTWARE IS
 * ASSUMED BY YOU. Some jurisdictions do not allow the exclusion of implied
 * warranties, so the above exclusion may not apply to You, but only to the
 * extent required by law.
 *
 * LIMITATION OF LIABILITY AND INDEMNIFICATION: TO THE EXTENT NOT PROHIBITED BY
 * APPLICABLE LAW, AMD AND ITS LICENSORS WILL NOT, UNDER ANY CIRCUMSTANCES BE
 * LIABLE TO YOU FOR ANY PUNITIVE, DIRECT, INCIDENTAL, INDIRECT, SPECIAL OR
 * CONSEQUENTIAL DAMAGES ARISING FROM POSSESSION OR USE OF THE SOFTWARE OR
 * OTHERWISE IN CONNECTION WITH ANY PROVISION OF THIS AGREEMENT EVEN IF AMD AND
 * ITS LICENSORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. THIS
 * INCLUDES, WITHOUT LIMITATION, DAMAGES DUE TO LOST OR MISAPPROPRIATED DATA,
 * LOST PROFITS OR CONFIDENTIAL OR OTHER INFORMATION, FOR BUSINESS INTERRUPTION,
 * FOR PERSONAL INJURY, FOR LOSS OF PRIVACY, FOR FAILURE TO MEET ANY DUTY
 * INCLUDING OF GOOD FAITH OR REASONABLE CARE, FOR NEGLIGENCE AND FOR ANY OTHER
 * PECUNIARY OR OTHER LOSS WHTSOEVER. In no event shall AMD's total liability to
 * You for all damages, losses, and causes of action (whether in contract, tort
 * (including negligence) or otherwise) exceed the amount of $50 USD. You agree
 * to defend, indemnify and hold harmless AMD, its subsidiaries and affiliates
 * and their respective licensors, directors, officers, employees, affiliates or
 * agents from and against any and all loss, damage, liability and other
 * expenses (including reasonable attorneys' fees), resulting from Your
 * possession or use of the Software or violation of the terms and conditions of
 * this Agreement.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS: Notice to U.S. Government End Users. The
 * Software and related documentation are "commercial items", as that term is
 * defined at 48 C.F.R. Section 2.101, consisting of "commercial computer
 * software" and "commercial computer software documentation", as such terms are
 * used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section 227.7202,
 * respectively. Consistent with 48 C.F.R. Section 12.212 or 48 C.F.R. Sections
 * 227.7202-1 through 227.7202-4, as applicable, the commercial computer
 * software and commercial computer software documentation are being licensed to
 * U.S. Government end users: (a) only as commercial items, and (b) with only
 * those rights as are granted to all other end users pursuant to the terms and
 * conditions set forth in this Agreement. Unpublished rights are reserved under
 * the copyright laws of the United States.
 *
 * EXPORT RESTRICTIONS:  You shall adhere to all applicable U.S. import/export
 * laws and regulations, as well as the import/export control laws and
 * regulations of other countries as applicable. You further agree You will not
 * export, re-export, or transfer, directly or indirectly, any product,
 * technical data, software or source code received from AMD under this license,
 * or the direct product of such technical data or software to any country for
 * which the United States or any other applicable government requires an export
 * license or other governmental approval without first obtaining such licenses
 * or approvals, or in violation of any applicable laws or regulations of the
 * United States or the country where the technical data or software was
 * obtained. You acknowledges the technical data and software received will not,
 * in the absence of authorization from U.S. or local law and regulations as
 * applicable, be used by or exported, re-exported or transferred to: (i) any
 * sanctioned or embargoed country, or to nationals or residents of such
 * countries; (ii) any restricted end-user as identified on any applicable
 * government end-user list; or (iii) any party where the end-use involves
 * nuclear, chemical/biological weapons, rocket systems, or unmanned air
 * vehicles.  For the most current Country Group listings, or for additional
 * information about the EAR or Your obligations under those regulations, please
 * refer to the website of the U.S. Bureau of Industry and Security at
 * http://www.bis.doc.gov/.
 ******************************************************************************/

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MpmFuncLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciEnumerationComplete.h>
#include <IndustryStandard/Pci22.h>

CONST STATIC BOOLEAN mIsCheckModuleDidVidBeforeLoadDriver = TRUE;
#define WIFI_MODULE_VID                          (0x17CB)
#define WIFI_MODULE_DID                          (0x1103)

VOID                            *mNfa765PciIoRegistration;



VOID
Nfa765PciIoCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                    Status;
  UINTN                         BufferSize;
  EFI_HANDLE                    Handle;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT32                        VidDid;

  while (TRUE) {
    BufferSize = sizeof (EFI_HANDLE);
    Status = gBS->LocateHandle (
                    ByRegisterNotify,
                    NULL,
                    mNfa765PciIoRegistration,
                    &BufferSize,
                    &Handle
                    );
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = gBS->HandleProtocol (
                    Handle,
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                      );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          PCI_VENDOR_ID_OFFSET,
                          1,
                          &VidDid
                          );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (((VidDid & 0xFFFF) != WIFI_MODULE_VID) ||
        ((VidDid >> 16 ) != WIFI_MODULE_DID)) {
      continue;
    }

    DEBUG ((DEBUG_INFO,
      "Found WLAN module vid:did (0x%04X:0x%04X), Install gNfa765DepexGuid\n",
      (VidDid & 0xFFFF),
      (VidDid >> 16 )
      ));
    Status = gBS->InstallProtocolInterface (
                    &gImageHandle,
                    &gNfa765DepexGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);

    gBS->CloseEvent (Event);
    break;
  }

}

/**
  Publish Nfa765 dependence upon system setup

  @param  ImageHandle   ImageHandle of the loaded driver.
  @param  SystemTable   Pointer to the EFI System Table.

  @retval EFI_SUCCESS   The handlers were registered successfully.
**/
EFI_STATUS
EFIAPI
Nfa765LauncherEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;
  VOID                          *DummyProtocol;
  EFI_EVENT                     Event;

  Status = gBS->LocateProtocol (
                  &gNfa765DepexGuid,
                  NULL,
                  (VOID **) &DummyProtocol
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "gNfa765DepexGuid is installed prior to Nfa765LauncherEntry().\n"));
    return EFI_ALREADY_STARTED;
  }

  if (EFI_ERROR (Status)) {
    if ((PcdGetBool (PcdMpmEnable) == TRUE) && (PcdGetBool (PcdMpmWirelessManageability) == TRUE)) {
      DEBUG ((DEBUG_INFO, "AIM-T is enabled, skip to publish gNfa765DepexGuid.\n"));
      return EFI_UNSUPPORTED;
    }
  }

  if (!mIsCheckModuleDidVidBeforeLoadDriver) {
    DEBUG ((DEBUG_INFO, "Install gNfa765DepexGuid directly directly\n"));
    Status = gBS->InstallProtocolInterface (
                    &gImageHandle,
                    &gNfa765DepexGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  //
  // BUGBUG:  It should wait gEfiPciEnumerationCompleteProtocolGuid and then check PciIo,
  //          but PciBus doesn't sync EDKII solution to install gEfiPciEnumerationCompleteProtocolGuid
  //          after StartPciDevices() function
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  Nfa765PciIoCallback,
                  NULL,
                  &Event
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Register for protocol notifications on this event
  //
  mNfa765PciIoRegistration = NULL;
  Status = gBS->RegisterProtocolNotify (
                  &gEfiPciIoProtocolGuid,
                  Event,
                  &mNfa765PciIoRegistration
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    gBS->CloseEvent (Event);
  }

  return Status;

}

