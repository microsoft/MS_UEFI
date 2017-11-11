##
# Copyright (c) 2017, Microsoft Corporation
#
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Python script allowing parsing raw FPDT XML and creating an HTML performance
# report.
##

import os, sys
import datetime
import argparse
import logging
import re
import xml.etree.ElementTree as ET
import hashlib
from operator import itemgetter

# global errors and warnings array since they're used within helper method, too
messages = []
ScriptPath = os.path.dirname(sys.argv[0])
reportGenVersion = '1.00'

#------------------------------------------------------------------------------
# Start PROGRESS CODE temp change to insert progress IDs that match current code here.
# Really want to keep the tool to a single python and single html template file
# for now to ease in distribution.  Once Perf changes have been made this
# progress code stuff should go away.
#
#------------------------------------------------------------------------------
# Code using these data structures assumes that progress IDs are hex numbers of 0xXXXX format:
# "0x" in front, then 4 characters, numbers and UPPERCASE characters

eventProgressId = '0x0000'

dynamicStringProgressIds = ['0x000F', \
                            '0x0011', \
                            '0x0013']

guidQwordProgressIds = ['0x0003', \
                        '0x0005', \
                        '0x0007', \
                        '0x0009']

guidProgressId = '0x0001'

dualGuidStringProgressId = ['0x000B', \
                           '0x000D']

# set of "start" progress ID codes
startProgressIds = ['0x0001', \
                    '0x0003', \
                    '0x0005', \
                    '0x0007', \
                    '0x0009', \
                    '0x000B', \
                    '0x000D', \
                    '0x000F', \
                    '0x0011', \
                    '0x0013']

# dict of all progress ID codes -> their labels
progressIdLabelDict = {'0x0001' : 'Entrypoint', \
                       '0x0003' : 'LoadImage', \
                       '0x0005' : 'BindingSupport', \
                       '0x0007' : 'BindingStart', \
                       '0x0009' : 'BindingStop', \
                       '0x000B' : 'EventSignal', \
                       '0x000D' : 'Callback', \
                       '0x000F' : 'Function', \
                       '0x0011' : 'Inmodule', \
                       '0x0013' : 'Crossmodule'}

##
# Regular Expressions for extracting Guids from UEFI file types
#
# DEC files
decRegex = r'\s*([a-zA-Z]\w*)\s*\=\s*' + \
           r'\{\s*0x([0-9a-fA-F]{1,8})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,4})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,4})\s*,\s*' + \
           r'\s*\{\s*0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*,\s*' + \
           r'0x([0-9a-fA-F]{1,2})\s*\}\s*\}'

# INF files
infFileRegex = r'\s*BASE_NAME\s*\=\s*([a-zA-Z]\w*)\s*'
infGuidRegex = r'\s*FILE_GUID\s*\=\s*([0-9a-fA-F]{8,8}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{12,12})\s*'

#FDF files
fdfGuidRegex = r'\s*FILE\s*DRIVER\s*\=\s*([0-9a-fA-F]{8,8}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{4,4}-[0-9a-fA-F]{12,12})\s*\{+\s*'
fdfFileRegex = r"\s*SECTION\s*UI\s*\=\s*\"([\w\s]+)\"\s*"

def AddGuid(guid, name, GuidDict):
    logging.debug("Found: '%s' : '%s'" % (name, guid))
    if(guid.upper() in GuidDict.keys()):
        current = GuidDict[guid.upper()]
        if(current.upper() != name.upper()):
            #no conflict - same name
            logging.error("Guid Conflict: for guid: %s.  Existing Name: %s Conflicting Name: %s" % (guid, current , name))
    else:
        GuidDict[guid.upper()] = name

#
# Parse Guids within the Filesystem
#
def ParseGuidsInDirectory(inputPath, guidDict):
    logging.debug('Parsing path %s:' % inputPath)
    # Walk the Filesystem looking for guids
    for root, dirs, files in os.walk(inputPath):
        for name in files:
            #
            # Parse DEC file
            #
            if name.lower().endswith('.dec'):
                logging.debug('Parsing DEC file %s:' % os.path.join(root, name))
                decFile = open(os.path.join(root, name), 'r')
                p = re.compile(decRegex)
                for line in decFile:
                    m = p.match(line)
                    if (m is not None):
                        guidKey = '%s-%s-%s-%s%s-%s%s%s%s%s%s' % (m.group(2).upper().zfill(8), \
                                                                  m.group(3).upper().zfill(4), \
                                                                  m.group(4).upper().zfill(4), \
                                                                  m.group(5).upper().zfill(2), m.group(6).upper().zfill(2), \
                                                                  m.group(7).upper().zfill(2), m.group(8).upper().zfill(2), m.group(9).upper().zfill(2), \
                                                                  m.group(10).upper().zfill(2), m.group(11).upper().zfill(2), m.group(12).upper().zfill(2))
                        AddGuid(guidKey, m.group(1), guidDict)
                decFile.close()
            #
            # Parse INF file
            #
            elif name.lower().endswith('.inf'):
                logging.debug('Parsing INF file %s:' % os.path.join(root, name))
                infFile = open(os.path.join(root, name), 'r')
                pFile = re.compile(infFileRegex)
                pGuid = re.compile(infGuidRegex)
                guidValue = None
                guidKey = None
                for line in infFile:
                    mFile = pFile.match(line)
                    mGuid = pGuid.match(line)
                    if (mFile is not None):
                        guidValue = mFile.group(1)
                    elif (mGuid is not None):
                        guidKey = mGuid.group(1).upper()
                    if ((guidValue is not None) and (guidKey is not None)):
                        AddGuid(guidKey, guidValue, guidDict)
                        guidValue = None
                        guidKey = None
                infFile.close()
            #
            # Parse FDF file
            #
            elif name.lower().endswith('.fdf'):
                logging.debug('Parsing FDF file %s:' % os.path.join(root, name))
                fdfFile = open(os.path.join(root, name), 'r')
                pFile = re.compile(fdfFileRegex)
                pGuid = re.compile(fdfGuidRegex)
                guidValue = None
                guidKey = None
                for line in fdfFile:
                    mFile = pFile.match(line)
                    mGuid = pGuid.match(line)
                    # here GUID must be followed by FILE promptly
                    if (mGuid is not None):
                        guidKey = mGuid.group(1).upper()
                        guidValue = None
                    elif (mFile is not None):
                        guidValue = mFile.group(1)
                    if ((guidValue is not None) and (guidKey is not None)):
                        AddGuid(guidKey, guidValue, guidDict)
                        guidValue = None
                        guidKey = None
                fdfFile.close()

def StaticGuidListFixUp(Guids):
    # If we need to manually cleanup some GUIDs, do it here.
    logging.info("NOT performing GUID fixup at this point")

#
# Main function
#
def main():

    # Set up command line arguments
    parser = argparse.ArgumentParser(description='FPDT XML Parser Tool')
    parser.add_argument('-t', '--OutputText', dest='output_text_file', help='Name of the output text file which will contain the boot timing info', default=None)
    parser.add_argument('-r', '--OutputHtml', dest='output_html_file', help='Name of the output HTML file which will contain the boot timing info', default=None)
    parser.add_argument('-i', '--InputXmlFile', help='Path to the input XML file with raw FPDT data', default=None)
    parser.add_argument('-s', '--SrcTreeRoot', help="Root of UEFI Code tree to parse for guids", default=None)
    parser.add_argument('-d', '--debug', help='enable debug output', action='store_true')
    parser.add_argument("-l", "--OutputDebugLog", dest='OutputDebugLog', help="Log all debug and error output to file", default=None)
    options = parser.parse_args()

    #setup file based logging if OutputDebugLog specified
    if(options.OutputDebugLog):
        if(len(options.OutputDebugLog) < 2):
            logging.critical("the output debug log file parameter is invalid")
            return -2
        else:
            #setup file based logging
            filelogger = logging.FileHandler(filename=options.OutputDebugLog, mode='w')
            if(options.debug):
                filelogger.setLevel(logging.DEBUG)
            else:
                filelogger.setLevel(logging.INFO)

            filelogger.setFormatter(formatter)
            logging.getLogger('').addHandler(filelogger)

    logging.info("Log Started: " + datetime.datetime.strftime(datetime.datetime.now(), "%A, %B %d, %Y %I:%M%p" ))


    #empty guid dictionary
    Guids = {}

    if(options.InputXmlFile is None):
        logging.critical("No Input XML File")
        return -4

    textLog = None
    if (options.output_text_file):
        if(len(options.output_text_file) < 2):
            logging.critical('The output text file parameter is invalid')
            return -3
        else:
            # Create a new text log file
            textLog = open(options.output_text_file, 'w')

    html_report = None
    if (options.output_html_file):
        if(len(options.output_html_file) < 2):
            logging.critical('The output JSON file parameter is invalid')
            return -1
        else:
            # Create a new html report file
            html_report = open(options.output_html_file, 'w')

    if(options.SrcTreeRoot is not None):
        logging.critical("Parsing Code Tree for GUIDs: %s" % options.SrcTreeRoot)
        ParseGuidsInDirectory(options.SrcTreeRoot, Guids)
        StaticGuidListFixUp(Guids)
        #
        # Print out the Guid list if debug is on
        #
        if(options.debug):
            for (k, v) in Guids.items():
                logging.debug("%s = %s" % (k, v))

    logging.critical("Parsing Fpdt XML records")

    # list of tuples, where each tuple is (<ID#>, <Type>, <GUID>, <GuidValue>, <String>, <StartTime>, <Length>)
    # ID is a counter that starts at 1
    timingList = []
    idCounter = 1

    # dictionary label:startTime
    startRecordsDict = {}

    # dictionary for crossmodule start guids and guidStrings
    # label : (guid, guidString)
    crossmoduleStartGuidDict = {}

    fbpt = ET.parse(options.InputXmlFile).find('Fbpt')

    # let's loop through records and find all binding_start_end records which contain the string,
    # we'll create a dict of qword->string mappings
    # dictionary qword:DevicePath
    controllerHandleDict = {}

    for record in fbpt:
        if (record.tag == 'GuidQwordStringEvent'):
            # the type should be BindingStart
            qword = record.find('Qword').attrib['Value']
            stringValue = record.find('String').attrib['Value']
            controllerHandleDict[qword] = stringValue

    for record in fbpt:
        # special case for basic record: create 3 events (ResetEnd, OSLoaderLoadImageStart,
        # OSLoaderStartImageStart), and one process (ExitBootServices)
        if (record.tag == 'FirmwareBasicBootPerformanceEvent'):

            # Length of 0.000001 (1 nanosecond) signifies an event
            timingList.append((1, 'Event', 'N/A', 'N/A', 'ResetEnd', float(record.find('ResetEnd').attrib['ValueInMilliseconds']), 0.000001))
            timingList.append((2, 'Event', 'N/A', 'N/A', 'OSLoaderLoadImageStart', float(record.find('OSLoaderLoadImageStart').attrib['ValueInMilliseconds']), 0.000001))
            timingList.append((3, 'Event', 'N/A', 'N/A', 'OSLoaderStartImageStart', float(record.find('OSLoaderStartImageStart').attrib['ValueInMilliseconds']), 0.000001))
            timingList.append((4, 'Process', 'N/A', 'N/A', 'ExitBootServices', float(record.find('ExitBootServicesEntry').attrib['ValueInMilliseconds']), float(record.find('ExitBootServicesExit').attrib['ValueInMilliseconds']) - float(record.find('ExitBootServicesEntry').attrib['ValueInMilliseconds'])))
            idCounter += 4

        elif (record.tag == 'DynamicStringEvent'):

            progressIdString = '0x%04X' % int(record.find('ProgressID').attrib['Value'], 16)
            progressIdStringMinusOne = '0x%04X' % (int(record.find('ProgressID').attrib['Value'], 16) - 1)

            # Can be an Event:
            if (progressIdString == eventProgressId):
                guid = record.find('GUID').attrib['Value']
                guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                stringValue = record.find('String').attrib['Value']
                startTime = float(record.find('Timestamp').attrib['ValueInMilliseconds'])
                length = 0.000001
                #                    (<ID#>, <Type>, <GUID>, <GuidValue>, <String>, <StartTime>, <Length>)
                timingList.append((idCounter, 'Event', guid, guidString, 'Event measurement from module ' + guidString + ' with label ' + stringValue, startTime, length))
                idCounter += 1

            # Can be Function, Crossmodule, or Inmodule begin
            elif (progressIdString in dynamicStringProgressIds):
                # here we need to create an entry for a label:startTime dict, startRecordsDict

                type = progressIdLabelDict[progressIdString]
                guid = record.find('GUID').attrib['Value']
                guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                stringValue = record.find('String').attrib['Value']

                if (type == 'Crossmodule'):
                    # label is <type>_<string>
                    label = type + '_' + stringValue
                    # preserve the GUID and the guidString of the start record
                    crossmoduleStartGuidDict[label] = (guid, guidString)
                else:
                    # label is <type>_<guidString>_<string>
                    label = type + '_' + guidString + '_' + stringValue

                startTime = float(record.find('Timestamp').attrib['ValueInMilliseconds'])

                # place this into the starts dictionary, but check if one with same label is already there
                if (label in startRecordsDict):
                    logging.error('Error! Start record with label %s already in dict!' % label)
                    messages.append(('Error', 'Start record with label %s already in dict!' % label))
                else:
                    startRecordsDict[label] = startTime

            # Can be Function, Crossmodule, or Inmodule end
            elif (progressIdStringMinusOne in dynamicStringProgressIds):
                # label is <type>_<guidString>_<string>
                type = progressIdLabelDict[progressIdStringMinusOne]
                guid = record.find('GUID').attrib['Value']
                guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                stringValue = record.find('String').attrib['Value']

                if (type == 'Crossmodule'):
                    # label is <type>_<string>
                    label = type + '_' + stringValue
                else:
                    # label is <type>_<guidString>_<string>
                    label = type + '_' + guidString + '_' + stringValue

                if (label in startRecordsDict):
                    startTime = startRecordsDict[label]
                    del startRecordsDict[label]
                    length = float(record.find('Timestamp').attrib['ValueInMilliseconds']) - startTime
                    if (length < 0):
                        logging.error('Error! Cannot have negative time!')

                    if (type == 'Crossmodule'):
                        #                 (<ID#>, <Type>, <GUID>, <GuidValue>, <String>, <StartTime>, <Length>)
                        DescriptionString = type + ' measurement from ' + crossmoduleStartGuidDict[label][1] + ' to ' + guidString + ' with label ' + stringValue
                        timingList.append((idCounter, type, crossmoduleStartGuidDict[label][0] + ', ' + guid, crossmoduleStartGuidDict[label][1] + ', ' + guidString, DescriptionString, startTime, length))
                    else:
                        timingList.append((idCounter, type, guid, guidString, type + ' measurement from module ' + guidString + ' with label ' + stringValue, startTime, length))
                        
                    idCounter += 1
                else:
                    logging.error("Error! Can't find a start event in the dict for label %s!" % label)
                    messages.append(('Error', "Can't find a start event in the dict for label %s!" % label))

        elif (record.tag == 'GuidQwordEvent'):

            progressIdString = '0x%04X' % int(record.find('ProgressID').attrib['Value'], 16)
            progressIdStringMinusOne = '0x%04X' % (int(record.find('ProgressID').attrib['Value'], 16) - 1)

            # Can be LoadImage, BindingSupport, BindingStart, BindingStop begin
            if (progressIdString in guidQwordProgressIds):
                # here we need to create an entry for a label:startTime dict, startRecordsDict

                # label here will depend on type
                type = progressIdLabelDict[progressIdString]

                if (type == "LoadImage"):
                    # label here is <type>_<qword> because guid is only populated in the end record
                    qword = record.find('Qword').attrib['Value']
                    label = type + '_' + qword
                else:
                    # otherwise this is a binding-related record and GUID is of the driver
                    # thus the label is <type>_<guidString>_<qword>
                    guid = record.find('GUID').attrib['Value']
                    guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                    qword = record.find('Qword').attrib['Value']
                    label = type + '_' + guidString + '_' + qword

                startTime = float(record.find('Timestamp').attrib['ValueInMilliseconds'])

                # place this into the starts dictionary, but check if one with same label is already there
                if (label in startRecordsDict):
                    logging.error('Error! Start record with label %s already in dict!' % label)
                    messages.append(('Error', 'Start record with label %s already in dict!' % label))
                else:
                    startRecordsDict[label] = startTime

            # Can be LoadImage, BindingSupport, BindingStop end
            elif (progressIdStringMinusOne in guidQwordProgressIds):
                # label here will depend on type
                type = progressIdLabelDict[progressIdStringMinusOne]

                if (type == "LoadImage"):
                    # label here is <type>_<qword> because guid is only populated in the end record
                    qword = record.find('Qword').attrib['Value']
                    label = type + '_' + qword
                else:
                    # otherwise this is a binding-related record and GUID is of the driver
                    # thus the label is <type>_<guidString>_<qword>
                    guid = record.find('GUID').attrib['Value']
                    guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                    qword = record.find('Qword').attrib['Value']
                    label = type + '_' + guidString + '_' + qword

                if (label in startRecordsDict):
                    startTime = startRecordsDict[label]
                    del startRecordsDict[label]
                    length = float(record.find('Timestamp').attrib['ValueInMilliseconds']) - startTime
                    if (length < 0):
                        logging.error('Error! Cannot have negative time!')

                    if (type == 'LoadImage'):
                        guid = record.find('GUID').attrib['Value']
                        guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                        timingList.append((idCounter, type, guid, guidString, 'LoadImage measurement for module ' + guidString, startTime, length))
                        idCounter += 1
                    else:
                        # then this must be BindingSupport, BindingStop end
                        guid = record.find('GUID').attrib['Value']
                        guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                        qword = record.find('Qword').attrib['Value']
                        stringValue  = controllerHandleDict[qword] if (qword in controllerHandleDict) else 'Unknown Device'

                        #                 (<ID#>, <Type>, <GUID>, <GuidValue>, <String>, <StartTime>, <Length>)
                        timingList.append((idCounter, type, guid, guidString, type + ' measurement for module ' + guidString + ' and device ' + stringValue, startTime, length))
                        idCounter += 1
                else:
                    logging.error("Error! Can't find a start event in the dict for label %s!" % label)
                    messages.append(('Error', "Can't find a start event in the dict for label %s!" % label))

        elif (record.tag == 'GuidQwordStringEvent'):

            progressIdStringMinusOne = '0x%04X' % (int(record.find('ProgressID').attrib['Value'], 16) - 1)

            # Should only be BindingStart end
            type = progressIdLabelDict[progressIdStringMinusOne]

            # the type should be BindingStart

            # the label is <type>_<guidString>_<qword>
            guid = record.find('GUID').attrib['Value']
            guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
            qword = record.find('Qword').attrib['Value']
            label = type + '_' + guidString + '_' + qword

            if (label in startRecordsDict):
                startTime = startRecordsDict[label]
                del startRecordsDict[label]
                length = float(record.find('Timestamp').attrib['ValueInMilliseconds']) - startTime
                if (length < 0):
                    logging.error('Error! Cannot have negative time!')

                stringValue = record.find('String').attrib['Value']
                timingList.append((idCounter, type, guid, guidString, type + ' measurement for module ' + guidString + ' and device ' + stringValue, startTime, length))
                idCounter += 1

            else:
                logging.error("Error! Can't find a start event in the dict for label %s!" % label)
                messages.append(('Error', "Can't find a start event in the dict for label %s!" % label))

        elif (record.tag == 'GuidEvent'):

            # this must be an entrypoint record, start or end
            progressIdString = '0x%04X' % int(record.find('ProgressID').attrib['Value'], 16)
            progressIdStringMinusOne = '0x%04X' % (int(record.find('ProgressID').attrib['Value'], 16) - 1)

            if (progressIdString == guidProgressId):
                # here we need to create an entry for a label:startTime dict, startRecordsDict

                # label is <type>_<guidString>
                type = progressIdLabelDict[progressIdString]
                guid = record.find('GUID').attrib['Value']
                guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                label = type + '_' + guidString

                startTime = float(record.find('Timestamp').attrib['ValueInMilliseconds'])

                # place this into the starts dictionary, but check if one with same label is already there
                if (label in startRecordsDict):
                    logging.error('Error! Start record with label %s already in dict!' % label)
                    messages.append(('Error', 'Start record with label %s already in dict!' % label))
                else:
                    startRecordsDict[label] = startTime

            elif (progressIdStringMinusOne == guidProgressId):
                # label is <type>_<guidString>
                type = progressIdLabelDict[progressIdStringMinusOne]
                guid = record.find('GUID').attrib['Value']
                guidString  = Guids[guid] if (guid in Guids) else ('UNKNOWN-' + hashlib.sha256(guid).hexdigest()[0:4])
                label = type + '_' + guidString

                if (label in startRecordsDict):
                    startTime = startRecordsDict[label]
                    del startRecordsDict[label]
                    length = float(record.find('Timestamp').attrib['ValueInMilliseconds']) - startTime
                    if (length < 0):
                        logging.error('Error! Cannot have negative time!')

                    #                 (<ID#>, <Type>, <GUID>, <GuidValue>, <String>, <StartTime>, <Length>)
                    timingList.append((idCounter, type, guid, guidString, type + ' measurement for module ' + guidString, startTime, length))
                    idCounter += 1
                else:
                    logging.error("Error! Can't find a start event in the dict for label %s!" % label)
                    messages.append(('Error', "Can't find a start event in the dict for label %s!" % label))

        elif (record.tag == 'DualGuidStringEvent'):

            progressIdString = '0x%04X' % int(record.find('ProgressID').attrib['Value'], 16)
            progressIdStringMinusOne = '0x%04X' % (int(record.find('ProgressID').attrib['Value'], 16) - 1)

            # Can be eventsignal or callback begin
            if (progressIdString in dualGuidStringProgressId):

                # here we need to create an entry for a label:startTime dict, startRecordsDict

                type = progressIdLabelDict[progressIdString]
                guid1 = record.find('GUID1').attrib['Value']
                guidString1  = Guids[guid1] if (guid1 in Guids) else ('UNKNOWN-' + hashlib.sha256(guid1).hexdigest()[0:4])
                guid2 = record.find('GUID2').attrib['Value']
                guidString2  = Guids[guid2] if (guid2 in Guids) else ('UNKNOWN-' + hashlib.sha256(guid2).hexdigest()[0:4])
                stringValue = record.find('String').attrib['Value']
                label = type + '_' + guidString1 + '_' + guidString2 + '_' + stringValue

                startTime = float(record.find('Timestamp').attrib['ValueInMilliseconds'])

                # place this into the starts dictionary, but check if one with same label is already there
                if (label in startRecordsDict):
                    logging.error('Error! Start record with label %s already in dict!' % label)
                    messages.append(('Error', 'Start record with label %s already in dict!' % label))
                else:
                    startRecordsDict[label] = startTime

            # Can be eventsignal or callback end
            elif (progressIdStringMinusOne in dualGuidStringProgressId):

                type = progressIdLabelDict[progressIdStringMinusOne]
                guid1 = record.find('GUID1').attrib['Value']
                guidString1  = Guids[guid1] if (guid1 in Guids) else ('UNKNOWN-' + hashlib.sha256(guid1).hexdigest()[0:4])
                guid2 = record.find('GUID2').attrib['Value']
                guidString2  = Guids[guid2] if (guid2 in Guids) else ('UNKNOWN-' + hashlib.sha256(guid2).hexdigest()[0:4])
                stringValue = record.find('String').attrib['Value']
                label = type + '_' + guidString1 + '_' + guidString2 + '_' + stringValue

                if (label in startRecordsDict):
                    startTime = startRecordsDict[label]
                    del startRecordsDict[label]
                    length = float(record.find('Timestamp').attrib['ValueInMilliseconds']) - startTime
                    if (length < 0):
                        logging.error('Error! Cannot have negative time!')

                    #                  (<ID#>,  <Type>, <GUID>, <GuidValue>, <String>, <StartTime>, <Length>)
                    descriptionString = type + ' measurement from module ' + guidString2 + ' and trigger/event ' + guidString1
                    timingList.append((idCounter, type, guid2 + ', ' + guid1, guidString2 + ', ' + guidString1, descriptionString, startTime, length))
                    idCounter += 1
                else:
                    logging.error("Error! Can't find a start event in the dict for label %s!" % label)
                    messages.append(('Error', "Can't find a start event in the dict for label %s!" % label))

    # check that startRecordsDict is empty now
    if (len(startRecordsDict) != 0):
        logging.error('Error! %d start record(s) with no matching end record(s) found:\n   %s' % (len(startRecordsDict), startRecordsDict))
        messages.append(('Error', '%d start record(s) with no matching end record(s) found: %s' % (len(startRecordsDict), startRecordsDict)))

    # print out messages
    logging.debug('Messages:\n    %s' % messages)

    # sort timingList by start times, i.e. 5th element
    # produce requested output
    timingList.sort(key=itemgetter(5))

    if (textLog is not None):
        logging.critical("Writing TXT Output")
        for timing in timingList:
            textLog.write(str(timing) + '\n')
        textLog.close()


    UEFIVersionXml = ET.parse(options.InputXmlFile).find('UEFIVersion')
    ModelXml = ET.parse(options.InputXmlFile).find('Model')
    DateCollectedXml = ET.parse(options.InputXmlFile).find('DateCollected')
    FpdtParserVersionXml = ET.parse(options.InputXmlFile).find('FpdtParserVersion')

    if (html_report is not None):
        logging.critical("Writing HTML Report")

        template = open(os.path.join(ScriptPath, "Perf_Report_template.html"), "r")
        for line in template.readlines():
            if "%TO_BE_FILLED_IN_BY_PYTHON_SCRIPT%" in line:
                #line = line.replace("%TO_BE_FILLED_IN_BY_PYTHON_SCRIPT%", rep.ToJsonString())
                html_report.write("        var JsonData = {\"Data\": {")
                html_report.write("\"Model\": \"%s\"," % ModelXml.attrib['Value'])
                html_report.write("\"UefiVersion\": \"%s\"," % UEFIVersionXml.attrib['Value'])
                html_report.write("\"DateCollected\": \"%s\"," % DateCollectedXml.attrib['Value'])
                html_report.write("\"ReportGenVersion\": \"%s\"," % reportGenVersion)
                html_report.write("\"FpdtParserVersion\": \"%s\"," % FpdtParserVersionXml.attrib['Value'])
                html_report.write("\"TimingData\": [")
                first = True
                for timing in timingList:
                    if (not first):
                        html_report.write(",")
                    first = False
                    html_report.write("{\"ID\": \"%s\",\"Type\": \"%s\",\"GUID\": \"%s\",\"GuidValue\": \"%s\",\"String\": \"%s\",\"StartTime\": \"%.6f\",\"Length\": \"%.6f\"}" % (timing[0], timing[1], timing[2], timing[3], timing[4], timing[5], timing[6]))
                html_report.write("],")
                html_report.write("\"Messages\": [")
                first = True
                for m in messages:
                    if (not first):
                        html_report.write(",")
                    first = False
                    html_report.write("{\"Type\": \"%s\", \"Message\": \"%s\"}" % (m[0], m[1]))
                html_report.write("]")
                html_report.write("}};")
            else:
                html_report.write(line)
        template.close()
        html_report.close()

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
        logging.critical("Test Failed.  Return Code: %i" % retcode)
    #end logging
    logging.shutdown()
    sys.exit(retcode)


