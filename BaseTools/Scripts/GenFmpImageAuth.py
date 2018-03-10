##
## Script to Generate a UEFI 2.4B FMP compliant Image Auth Header wrapped
## around the payload file.
##
## For dev purposes this script takes a payload file and signs it and encapsulates it
## in the correct headers.  This file is then ready to be put into a FMP capsule. 
##
## For production use this script has a production flag and a DetachedSignature parameter
## which allows the signing to be done offline. 
##
## General process:
##   Phase 1: Create payload file by combining payload and monotonic count
##   Phase 2: Sign it using signtool
##   Phase 3: Wrap payload in headers to create final FMP Image header/payload
##
##
## Copyright Microsoft Corporation, 2014
##


import os, sys
from optparse import OptionParser
import logging
import datetime
import struct
import subprocess
import uuid
import threading
import shutil


gPhase3PackageOnly = False


####
# Class to support running commands from the shell in a python environment. 
# Don't use directly.  
# 
# PropagatingThread copied from sample here:
# https://stackoverflow.com/questions/2829329/catch-a-threads-exception-in-the-caller-thread-in-python
####
class PropagatingThread(threading.Thread):
    def run(self):
        self.exc = None
        try:
            if hasattr(self, '_Thread__target'):
                # Thread uses name mangling prior to Python 3.
                self.ret = self._Thread__target(*self._Thread__args, **self._Thread__kwargs)
            else:
                self.ret = self._target(*self._args, **self._kwargs)
        except BaseException as e:
            self.exc = e

    def join(self, timeout=None):
        super(PropagatingThread, self).join()
        if self.exc:
            raise self.exc
        return self.ret


####
# Helper functions for running commands from the shell in python environment
# Don't use directly
#
# process output stream and write to log.
# part of the threading pattern.
#
#  http://stackoverflow.com/questions/19423008/logged-subprocess-communicate
####
def reader(filepath, outstream, stream):
    f = None
    #open file if caller provided path
    if(filepath):
        f = open(filepath, "w")

    while True:
        s = stream.readline().decode()
        if not s:
            break
        if(f is not None):
            #write to file if caller provided file
            f.write(s)
        if(outstream is not None):
            #write to stream object if caller provided object
            outstream.write(s)
        logging.info(s.rstrip())
    stream.close()
    if(f is not None):
        f.close()

####
# Run a shell commmand and print the output to the log file
# This is the public function that should be used to run commands from the shell in python environment
# @param cmd - cmd string to run including parameters
# @param capture - boolean to determine if caller wants the output captured in any format.
# @param workingdir - path to set to the working directory before running the command.
# @param outfile - capture output to file of given path.
# @param outstream - capture output to a stream.  
#
# @return returncode of called cmd
####
def RunCmd(cmd, capture=True, workingdir=None, outfile=None, outstream=None):
    starttime = datetime.datetime.now()
    logging.debug("Cmd to run is: " + cmd)
    logging.info("------------------------------------------------")
    logging.info("--------------Cmd Output Starting---------------")
    logging.info("------------------------------------------------")
    c = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, cwd=workingdir)
    if(capture):
        outr = PropagatingThread(target=reader, args=(outfile, outstream, c.stdout,))
        outr.start()
        c.wait()
        outr.join()
    else:
        c.wait()
            
    endtime = datetime.datetime.now()
    delta = endtime - starttime
    logging.info("------------------------------------------------")
    logging.info("--------------Cmd Output Finished---------------")
    logging.info("--------- Running Time (mm:ss): {0[0]:02}:{0[1]:02} ----------".format(divmod(delta.seconds, 60)))
    logging.info("------------------------------------------------")
    return c.returncode

####
# Locally Sign input file using Windows SDK signtool.  This will use a local Pfx file.  
# WARNING!!! : This should not be used for production signing as that process should follow stronger security practices (HSM / smart cards / etc)
# 
#  Signing is in format specified by UEFI authentacted variables 
####
def DetachedSignWithSignTool(SignToolPath, ToSignFilePath, SignatureOutputFile, PfxFilePath, PfxPass=None, Oid="1.2.840.113549.1.7.2"):

    #check signtool path
    if not os.path.exists(SignToolPath):
        logging.error("Path to signtool invalid.  %s" % SignToolPath)
        return -1

    OutputDir = os.path.dirname(SignatureOutputFile)
    #Signtool docs https://docs.microsoft.com/en-us/dotnet/framework/tools/signtool-exe
    #Signtool parameters from https://docs.microsoft.com/en-us/windows-hardware/manufacture/desktop/secure-boot-key-generation-and-signing-using-hsm--example
    # Search for "Secure Boot Key Generation and Signing Using HSM"
    cmd = SignToolPath + ' sign /fd sha256 /p7ce DetachedSignedData /p7co ' + Oid + ' /p7 "' + OutputDir + '" /f "' + PfxFilePath + '"'
    if PfxPass is not None:
        #add password if set
        cmd = cmd + ' /p ' + PfxPass
    cmd = cmd + ' /debug /v "' + ToSignFilePath + '" '
    ret = RunCmd(cmd)
    if(ret != 0):
        logging.error("Signtool failed %d" % ret)
        return ret
    signedfile = os.path.join(OutputDir, os.path.basename(ToSignFilePath) + ".p7")
    if(not os.path.isfile(signedfile)):
        raise Exception("Output file doesn't eixst %s" % signedfile)

    shutil.move(signedfile, SignatureOutputFile)
    return ret

#
#main script function
#
def main():
    parser = OptionParser()
    #Output debug log
    parser.add_option("-l", dest="OutputLog", help="Create an output log file: ie -l out.txt", default=None)
    parser.add_option("-o", "--OutputFile", dest="OutputFile", help="Result/Output file", default=None)
    parser.add_option("-p", "--payload", dest="Payload", help="Input unsigned payload file", default=None)
    parser.add_option("--production", dest="ProductionSign", action="store_true", help="Production Sign Process (no dev signing)", default=False)
    parser.add_option("-m", dest="MonotonicCount", help="Monotonic Count Value", default=0)
    parser.add_option("-s", dest="DetachedSignature", help="Detached Signature file (production signed phase 3 step only)", default=None)
    parser.add_option("--pfxfile", dest="PfxPath", help="Path to PFX file for dev signing", default=None)
    parser.add_option("--pfxpass", dest="PfxPass", help="Optional - PFX password for dev signing with PFX cert", default=None)
    parser.add_option("--SignTool", dest="SignToolPath", help="Path to signtool.exe")
    #Turn on dubug level logging
    parser.add_option("--debug", action="store_true", dest="debug", help="turn on debug logging level for file log",  default=False)
    parser.add_option("--dirty", action="store_true", dest="dirty", help="turn on dirty flag to keep intermediate files.  Default is to delete them.",  default=False)

    (options, args) = parser.parse_args()

    #setup file based logging if outputReport specified
    if(options.OutputLog):
        if(len(options.OutputLog) < 2):
            logging.critical("the output log file parameter is invalid")
            return -2
        else:
            #setup file based logging
            filelogger = logging.FileHandler(filename=options.OutputLog, mode='w')
            if(options.debug):
                filelogger.setLevel(logging.DEBUG)
            else:
                filelogger.setLevel(logging.INFO)

            filelogger.setFormatter(formatter)
            logging.getLogger('').addHandler(filelogger)

    logging.info("Log Started: " + datetime.datetime.strftime(datetime.datetime.now(), "%A, %B %d, %Y %I:%M%p" ))

    #check for valid files
    if not options.Payload:
        logging.critical("No Payload file specified")
        return -1

    if not os.path.isfile(options.Payload):
        logging.critical("Invalid Path to payload file")
        return -2

    if not options.DetachedSignature:
        logging.debug("No Detached Signature File.")
    else:
        logging.debug("Parameter for detached signature file specified. " + options.DetachedSignature)
        logging.debug("Entering Phase2-PackageOnly Mode")
        global gPhase3PackageOnly
        gPhase3PackageOnly = True

    if not options.OutputFile:
        logging.debug("No output file specified.  Using default. AuthPayload.FmImageAuth")
        options.OutputFile = "AuthPayload.FmImageAuth"

    if(not gPhase3PackageOnly):
        #must have a pfx file
        if not options.PfxPath:
            logging.critical("No Pfx File given.")
            return -7
        if not os.path.isfile(options.PfxPath):
            logging.critical("Invalid PFX Path.  File doesn't exist.  " + options.PfxPath)
            return -6

        logging.debug("Using PFX file: " + str(options.PfxPath))

    
    logging.debug("Production Mode: " + str(options.ProductionSign))
    logging.debug("Monotonic Count: " + str(options.MonotonicCount))
    logging.debug("Output File: " + str(options.OutputFile))
    logging.debug("Dirty Mode: " + str(options.dirty))

    FileToSign = os.path.join("payload.Temp.ToBeSigned")



    #if not doing phase2 only then we need to do presign stuff
    if not gPhase3PackageOnly:
        #Since we are not in phase3packageonly mode we know no DetachedSignature file speficied.  Set to the default output. 
        OutputDir = os.path.dirname(os.path.abspath(options.OutputFile))
        logging.debug("Temp files will be written to: " + str(OutputDir))

        #change the path to temp location
        FileToSign = os.path.join(OutputDir, FileToSign)
        options.DetachedSignature = FileToSign + ".p7"

        #Create a temp file with payload + monotonic count
        f = open(FileToSign, "wb")
        pf = open(options.Payload, "rb")
        f.write(pf.read())
        mc = struct.pack("Q", int(options.MonotonicCount))
        f.write(mc)
        pf.close()
        f.close()

        
        #if not doing production signing then sign it
        if not options.ProductionSign:
            #check sign tool
            if(os.path.exists(options.SignToolPath)):
                logging.debug("Signtool.exe found at location: " + options.SignToolPath)
            else:
                logging.critical("Can't find signtool at location: " + options.SignToolPath)
                return -5
            
            ret = DetachedSignWithSignTool(
                options.SignToolPath, 
                FileToSign,  
                options.DetachedSignature, 
                options.PfxPath, 
                PfxPass=options.PfxPass
                )

            if ret != 0:
                logging.critical("DetachedSignWithSignTool Failed: " + str(ret))
                return ret

            if not options.dirty:
                logging.debug("Delete temp file: " + str(FileToSign))
                os.remove(FileToSign)
                

        else:
            logging.critical("File To Production Sign Created: " + FileToSign)
            return 0

    #package the final output (phase 3)
    wcugSize = os.path.getsize(options.DetachedSignature)
    logging.debug("PKCS7 Signed Data is size: " + str(wcugSize))
    wcugSize = wcugSize + 4 + 2 + 2 + 16;  # matches the hdr + guid below

    #
    #Header layout and structures defined in UEFI 2.4 Errata B.
    #
    
    #EFI_FIRMWARE_IMAGE_AUTH
        #UINT64 Monotonic Count                  <--count value used when signing it
        #WIN_CERTIFICATE_UEFI_GUID AuthInfo
            #WIN_CERTIFICATE Hdr
                #UINT32 dwLength                 <--Length of cert header 
                #UINT16 wRevision                <--Revision level of win cert current 0x0200
                #UINT16 wCertType                <--WIN_CERT_TYPE_EFI_GUID  0x0EF1
            #EFI_GUID CertType                   <--gEfiCertPkcs7Guid  = { 0x4aafd29d, 0x68df, 0x49ee, {0x8a, 0xa9, 0x34, 0x7d, 0x37, 0x56, 0x65, 0xa7 }}
            #UINT8[] PKCS7 SignedData            <--DetachedSignature from signtool 
    #UINT8[] Payload                             <--Payload file

    #struct format for the header
    header = struct.pack("QLHH", int(options.MonotonicCount), int(wcugSize), int("200", 16), int("0EF1", 16))
    pkcsguid = uuid.UUID('{4aafd29d-68df-49ee-8aa9-347d375665a7}')

    f = open(options.OutputFile, "wb")
    f.write(header)
    f.write(pkcsguid.bytes_le)
    sd = open(options.DetachedSignature, "rb")
    f.write(sd.read())
    sd.close()
    p = open(options.Payload, "rb")
    f.write(p.read())
    p.close()
    f.close()
    logging.critical("Final FMP compliant Authenticated Payload Image File created:\n " + os.path.abspath(str(options.OutputFile)))

    #if user wants temp files deleted and didn't pass in the p7 file....then delete it now
    if not options.dirty:
        if not gPhase3PackageOnly:
            logging.debug("Delete temp file: " + str(options.DetachedSignature))
            os.remove(options.DetachedSignature)

            
    return 0


if __name__ == '__main__':
    #setup main console as logger
    logger = logging.getLogger('')
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter("%(levelname)s - %(message)s")
    console = logging.StreamHandler()
    console.setLevel(logging.CRITICAL)
    console.setFormatter(formatter)
    logger.addHandler(console)

    #call main worker function
    retcode = main()

    if retcode != 0:
        logging.critical("Failed.  Return Code: %i" % retcode)
    #end logging
    logging.shutdown()
    sys.exit(retcode)
