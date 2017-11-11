# Performance2 Library and Report Creation Tools

## Copyright

Copyright (c) 2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

Performance2Lib provides macros and functions to create performance records during UEFI boot.
The tools that come with this library allow collecting the FPDT and FBPT performance data from a target system into an XML file and then converting the XML into an HTML report.

**Architecture and Use Model**
MdePkg\Include\Library\Performance2Lib.h is a public interface for Performance2Lib.
The header file lists a number of macros to be used in UEFI code to create performance records.
The following records are to be used by the core code, and in this branch the core is already instrumented:
  - PERF_ENTRYPOINT_BEGIN/END: used for logging time of entrypoint execution
  - PERF_LOADIMAGE_BEGIN/END: used for logging time it takes to load driver image
  - PERF_BINDING_SUPPORT_BEGIN/END: used for logging time it takes to run a BindingSupport function
  - PERF_BINDING_START_BEGIN/END: used for logging time it takes to run a BindingStart function
  - PERF_BINDING_STOP_BEGIN/END: used for logging time it takes to run a BindingStop function

The remaining macros are used for instrumenting non-core UEFI modules:
  - PERF_EVENT: to measure time from power-on to this macro execution
  - PERF_EVENTSIGNAL_BEGIN/END: to surround a SignalEvent call
  - PERF_CALLBACK_BEGIN/END: to instrument a callback function
  - PERF_FUNCTION_BEGIN/END: to instrument any function
  - PERF_INMODULE_BEGIN/END: to measure time between any two locations in one module’s code
  - PERF_CROSSMODULE_BEGIN/END: to measure time across locations in two separate modules

Verbosity parameter is always the first one when calling the above macros. Verbosity values are defined in the same header file: PERF_VERBOSITY_LOW, PERF_VERBOSITY_STANDARD, PERF_VERBOSITY_HIGH. See PCD descriptions below for details on setting verbosity settings per build and per module.

__Performance2Lib does not support measuring timings inside of SMI handlers.__

BasePerformance2LibNull in MdePkg implements the Performance2Lib interface with empty functions.

Libraries in PerformancePkg have a full implementation of the Performance2Lib interface:
  - PeiCorePerformance2Lib: this library is linked to the PEI Core and thus stays in memory until end of PEI. The library is used directly for creating perf records in a HOB when called from the PEI Core. On the first perf call (which must come from the core) it installs a PPI which allows calling into the perf measurement routine in the core library. Thus, all PEI perf calls land in this library, in the one instance linked to the PEI core, and all PEI perf records are stored in the same HOB.
  - PeiPerformance2Lib: this "thin" library locates the performance PPI and passes the parameters to it. This library is linked to PEI modules that need to be instrumented internally (by default all PEI entrypoints are logged anyway).
  - DxeCorePerformance2Lib: similarly to the PEI core lib, this one is used directly by DXE core and via a protocol called by a thin DXE library. Up until EndOfDxe this library stores the performance records in a local buffer. At EndOfDxe the HOB from the PEI core library as well as the local buffer get reported via status codes, and from there on records are reported one by one as status codes. The listener in MdeModulePkg\Universal\Acpi\FirmwarePerformanceDataTableDxe stores the information from the status codes in the local buffer until ReadyToBoot, at which point FPDT and FBPT are installed, and all the data from the local buffer is copied to FBPT. Between ReadyToBoot and ExitBootServices, the appends the records to the tail end of  FBPT one by one.
  - DxePerformance2Lib: the thin library that locates the perf2 protocol and passes the parameters to the protocol call. This library is linked to DXE modules, SMM core, and SMM driver entrypoints.

PCDs used by this library are under [PcdsFixedAtBuild,PcdsPatchableInModule]:
  - PcdPerformance2LibraryVerbosityLevel: 0 None, 1 Low, 2 Standard, 3 High. Default is 2 (Standard)
  - PcdPerformance2LibraryCoreFunctionalityMask: Bits 0-4: Entrypoint, Loadimage, BindingSupport, BindingStart, BindingStop. By default, Bits 0, 1, 3, and 4 are set. BindingSupport bit (Bit2) is not set because it generates a huge number of records.

**Tools and Reports**
To collect perf data run FpdtParser on the target system.
  - Located at PerformancePkg/Tools/FpdtParser
  - Requires a Windows Insider Preview build (as of November 7 2017) with x64 Python2 and PyWin32
  - Usage:
        ParseFpdt.py –x OUTPUT_XML_FILE

To create an HTML performance report run PerfReportGen on a system with access to UEFI source code (this tool parses the UEFI source to map the GUIDs to their names).
  - Located at PerformancePkg/Tools/PerfReportGen
  - Usage:
        PerfReportGen.py –r OUTPUT_HTML_FILE –i INPUT_XML_FILE –s PATH_TO_SOURCE_TREE_ROOT

In the report, in the  "Data Table" tab, select the records of interest in the table (can use Ctrl and Shift to select more than one record). Then switch to the "Line Chart" tab and see the selected entries on the plot.