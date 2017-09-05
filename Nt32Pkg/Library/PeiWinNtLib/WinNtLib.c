/**@file

Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  WinNtLib.c

Abstract:

  WinNt Library 

**/

#include <PiDxe.h>
#include <WinNtDxe.h>
#include <Library/WinNtLib.h>
#include <Ppi/NtThunk.h>
#include <Library/DebugLib.h>


EFI_WIN_NT_THUNK_PROTOCOL *gWinNt;

EFI_STATUS
WinNtLibConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
/*++

Routine Description:

Arguments:

Returns:

--*/
{

  EFI_STATUS                Status;
  PEI_NT_THUNK_PPI          *PeiNtService;
  
  Status = (**PeiServices).LocatePpi (
                            (const EFI_PEI_SERVICES **)PeiServices,
                            &gPeiNtThunkPpiGuid,
                            0,                  
                            NULL,
                            (VOID**)&PeiNtService
                            );
  ASSERT_EFI_ERROR (Status);
    
  //
  // Calculate the time to sleep.  Win API smallest unit to sleep is 1 millisec
  // so micro second units need be divided by 1000 to convert to ms
  //
  gWinNt = (EFI_WIN_NT_THUNK_PROTOCOL*) PeiNtService->NtThunk();
  ASSERT (gWinNt != NULL);
  return EFI_SUCCESS;
}
