
# SMMPagingAudit
SMM is a privileged mode of ia32/x64 cpus.  In this environment nearly all system state can 
be inspected including that of the operating system, kernel, and hypervisor.  Due to it's 
capabilities SMM has become an area of interest for those searching to exploit the system. 
To help minimize the interest and impact of an exploit in SMM the SMI handlers should operate
in a least privileged model.  To do this standard paging can be leveraged to limit the SMI
handlers access.  Tianocore has a feature to enable paging within SMM and this tool helps confirm
the configuration being used.  This tool requires three parts to get a complete view.  
 
## SMM
The SMM driver must be included in your build and dispatched to SMM before the End Of Dxe.  It is
recommended that this driver should only be used on debug builds as it reports the entire
SMM memory environment to the caller.  The shell app will communicate to the SMM driver and 
request critical memory information including IDT, GDT, page tables, and loaded images.

## App
The UEFI shell application collects system information from the DXE environment and then
communicates to the SMM driver/handler to collect necessary info from SMM.  It then 
writes this data to files and then that content is used by the windows scripts.  

## Windows
The Windows script will look at the *.DAT files, make sense of their content, check for errors
and then insert the formatted data into the Html report file.  This report file is then double-clickable
by the end user/developer to review the posture of the SMM environment.  The Results tab applies 
the Microsoft Windows System Guard requirement heuristics to show if the environment passes or fails.  
If it fails the filters on the data tab can be configured to show where the problem exists.  


## Usage / Enabling
First for the SMM driver and app you need to add them to your DSC file for your project.

[Components.X64]
 UefiTestingPkg\AuditTests\SmmPagingAudit\Smm\SmmPagingAuditSmm.inf
 UefiTestingPkg\AuditTests\SmmPagingAudit\App\SmmPagingAuditApp.inf

Next you must add the SMM driver to a FW in your FDF that can dispatch SMM modules.
  INF UefiTestingPkg\AuditTests\SmmPagingAudit\Smm\SmmPagingAuditSmm.inf

Third after building you must:
1. flash that image on the system.  
2. Copy the SmmPagingAuditApp.efi to a USB key

Then, boot your system running the new firmware to the shell and run the app. The
tool will create a set of *.dat files on the same USB command. 

Boot to windows and run the Python script on the data found on your USB key.  

Finally double-click the HTML output file and check your results.   

