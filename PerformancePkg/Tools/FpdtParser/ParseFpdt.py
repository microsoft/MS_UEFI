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
# Python script allowing reading FPDT and FBPT from Windows, storing result in XML
# or text form.
##

import os, sys
import argparse
from ctypes import *
from collections import namedtuple
import logging
import pywintypes
import win32api, win32process, win32security, win32file
import winerror
import struct
import subprocess
import datetime
import xml.etree.ElementTree as ET

FpdtParserVersion = '1.00'

FIRMWARE_BASIC_BOOT_PERFORMANCE_DATA_EVENT_TYPE = 0x0002
GUID_EVENT_TYPE                                 = 0x1010
DYNAMIC_STRING_EVENT_TYPE                       = 0x2001
DUAL_GUID_STRING_EVENT_TYPE                     = 0x2002
GUID_QWORD_EVENT_TYPE                           = 0x2003
GUID_QWORD_STRING_EVENT_TYPE                    = 0x2004

known_FBPT_record_types = [FIRMWARE_BASIC_BOOT_PERFORMANCE_DATA_EVENT_TYPE,
                           GUID_EVENT_TYPE, DYNAMIC_STRING_EVENT_TYPE,
                           DUAL_GUID_STRING_EVENT_TYPE, GUID_QWORD_EVENT_TYPE,
                           GUID_QWORD_STRING_EVENT_TYPE]


class ACPI_TABLE_HEADER(object):
    struct_format = '=4sIBB6s8sI4sI'
    size = struct.calcsize(struct_format)

    def __init__(self, header_byte_array):
        (self.Signature,
         self.Length,
         self.Revision,
         self.Checksum,
         self.OEMID,
         self.OEMTableID,
         self.OEMRevision,
         self.CreatorID,
         self.CreatorRevision) = struct.unpack_from(ACPI_TABLE_HEADER.struct_format, header_byte_array)

    def __str__(self):
        return """\n  ACPI Table Header
------------------------------------------------------------------
  Signature        : %s
  Length           : 0x%08X
  Revision         : 0x%02X
  Checksum         : 0x%02X
  OEM ID           : %s
  OEM Table ID     : %s
  OEM Revision     : 0x%08X
  Creator ID       : %s
  Creator Revision : 0x%08X
""" % ( self.Signature, self.Length, self.Revision, self.Checksum, self.OEMID, self.OEMTableID, self.OEMRevision, self.CreatorID, self.CreatorRevision )

    def toXml(self):
        xml_repr = ET.Element('AcpiTableHeader')
        xml_repr.set('Signature',       '%s'   % self.Signature)
        xml_repr.set('Length',          '0x%X' % self.Length)
        xml_repr.set('Revision',        '0x%X' % self.Revision)
        xml_repr.set('Checksum',        '0x%X' % self.Checksum)
        xml_repr.set('OEMID',           '%s'   % self.OEMID)
        xml_repr.set('OEMTableID',      '%s'   % self.OEMTableID)
        xml_repr.set('OEMRevision',     '0x%X' % self.OEMRevision)
        xml_repr.set('CreatorID',       '%s'   % self.CreatorID)
        xml_repr.set('CreatorRevision', '0x%X' % self.CreatorRevision)
        return xml_repr


class FW_BASIC_BOOT_PERFORMANCE_RECORD(object):
    struct_format = '=HBBIQ'
    size = struct.calcsize(struct_format)
    def __init__(self, record_byte_array):
        (self.PerformanceRecordType,
         self.RecordLength,
         self.Revision,
         self.Reserved,
         self.FBPTPointer) = struct.unpack_from(FW_BASIC_BOOT_PERFORMANCE_RECORD.struct_format, record_byte_array)

    def __str__(self):
        return """\n  Firmware Basic Boot Performance Record
------------------------------------------------------------------
  Performance Record Type : 0x%04X
  Record Length           : 0x%02X
  Revision                : 0x%02X
  Reserved                : 0x%08X
  FBPT Pointer            : 0x%016X
""" % (self.PerformanceRecordType, self.RecordLength, self.Revision, self.Reserved, self.FBPTPointer)

    def toXml(self):
        xml_repr = ET.Element('FwBasicBootPerformanceRecord')
        xml_repr.set('PerformanceRecordType', '0x%X' % self.PerformanceRecordType)
        xml_repr.set('RecordLength',          '0x%X' % self.RecordLength)
        xml_repr.set('Revision',              '0x%X' % self.Revision)
        xml_repr.set('Reserved',              '0x%X' % self.Reserved)
        xml_repr.set('FBPTPointer',           '0x%X' % self.FBPTPointer)
        return xml_repr

class FW_BASIC_BOOT_PERFORMANCE_TABLE_HEADER(object):
    struct_format = '=4sI'
    size = struct.calcsize(struct_format)
    def __init__(self, header_byte_array):
        (self.Signature,
         self.Length) = struct.unpack_from(FW_BASIC_BOOT_PERFORMANCE_TABLE_HEADER.struct_format, header_byte_array)

    def __str__(self):
        return """\n  Firmware Basic Boot Performance Table Header
------------------------------------------------------------------
  Signature : %s
  Length    : 0x%08X
""" % (self.Signature, self.Length)

    def toXml(self):
        xml_repr = ET.Element('Fbpt')
        xml_repr.set('Signature', '%s'   % self.Signature)
        xml_repr.set('Length',    '0x%X' % self.Length)
        return xml_repr


class FBPT_RECORD_HEADER(object):
    struct_format = '=HBB'
    size = struct.calcsize(struct_format)
    def __init__(self, header_byte_array):
        (self.PerformanceRecordType,
         self.RecordLength,
         self.Revision) = struct.unpack_from(FBPT_RECORD_HEADER.struct_format, header_byte_array)

    def __str__(self):
        return """\n  FBPT Record Header
------------------------------------------------------------------
  Performance Record Type : 0x%04X
  Record Length           : 0x%02X
  Revision                : 0x%02X
""" % (self.PerformanceRecordType, self.RecordLength, self.Revision)

    def toXml(self):

        if (self.PerformanceRecordType == FIRMWARE_BASIC_BOOT_PERFORMANCE_DATA_EVENT_TYPE):
            xml_repr = ET.Element('FirmwareBasicBootPerformanceEvent')
        elif (self.PerformanceRecordType == GUID_EVENT_TYPE):
            xml_repr = ET.Element('GuidEvent')
        elif (self.PerformanceRecordType == DYNAMIC_STRING_EVENT_TYPE):
            xml_repr = ET.Element('DynamicStringEvent')
        elif (self.PerformanceRecordType == DUAL_GUID_STRING_EVENT_TYPE):
            xml_repr = ET.Element('DualGuidStringEvent')
        elif (self.PerformanceRecordType == GUID_QWORD_EVENT_TYPE):
            xml_repr = ET.Element('GuidQwordEvent')
        elif (self.PerformanceRecordType == GUID_QWORD_STRING_EVENT_TYPE):
            xml_repr = ET.Element('GuidQwordStringEvent')
        else:
            logging.critical('Creating XML out of an uknown type record!')
            xml_repr = ET.Element('UnknownEvent')

        xml_repr.set('PerformanceRecordType', '0x%X' % self.PerformanceRecordType)
        xml_repr.set('RecordLength', '0x%X' % self.RecordLength)
        xml_repr.set('Revision', '0x%X' % self.Revision)

        return xml_repr


class FW_BASIC_BOOT_PERFORMANCE_DATA_RECORD(object):
    struct_format = '=IQQQQQ'
    size = struct.calcsize(struct_format)

    def __init__(self, record_header, contents_byte_array):
        self.header = record_header
        (self.Reserved,
         self.ResetEnd,
         self.OSLoaderLoadImageStart,
         self.OSLoaderStartImageStart,
         self.ExitBootServicesEntry,
         self.ExitBootServicesExit) = struct.unpack_from(FW_BASIC_BOOT_PERFORMANCE_DATA_RECORD.struct_format, contents_byte_array)

    def __str__(self):
        return str(self.header) + \
"""\n  FW Basic Boot Performance Data Record Contents
------------------------------------------------------------------
  Reserved                   : 0x%08X
  Reset End                  : 0x%016X
  OS Loader LoadImage Start  : 0x%016X
  OS Loader StartImage Start : 0x%016X
  ExitBootServices Entry     : 0x%016X
  ExitBootServices Exit      : 0x%016X
""" % (self.Reserved, self.ResetEnd, self.OSLoaderLoadImageStart, self.OSLoaderStartImageStart, self.ExitBootServicesEntry, self.ExitBootServicesExit)

    def toXml(self):
        xml_repr = self.header.toXml()

        resetend_xml_et = ET.SubElement(xml_repr, 'ResetEnd')
        resetend_xml_et.set('RawValue', '0x%X' % self.ResetEnd)
        resetend_xml_et.set('ValueInMilliseconds', '%.6f' % (self.ResetEnd / 1000000.0))

        osloaderloadimagestart_xml_et = ET.SubElement(xml_repr, 'OSLoaderLoadImageStart')
        osloaderloadimagestart_xml_et.set('RawValue', '0x%X' % self.OSLoaderLoadImageStart)
        osloaderloadimagestart_xml_et.set('ValueInMilliseconds', '%.6f' % (self.OSLoaderLoadImageStart / 1000000.0))

        osloaderstartimagestart_xml_et = ET.SubElement(xml_repr, 'OSLoaderStartImageStart')
        osloaderstartimagestart_xml_et.set('RawValue', '0x%X' % self.OSLoaderStartImageStart)
        osloaderstartimagestart_xml_et.set('ValueInMilliseconds', '%.6f' % (self.OSLoaderStartImageStart / 1000000.0))

        exitbootservicesentry_xml_et = ET.SubElement(xml_repr, 'ExitBootServicesEntry')
        exitbootservicesentry_xml_et.set('RawValue', '0x%X' % self.ExitBootServicesEntry)
        exitbootservicesentry_xml_et.set('ValueInMilliseconds', '%.6f' % (self.ExitBootServicesEntry / 1000000.0))

        exitbootservicesexit_xml_et = ET.SubElement(xml_repr, 'ExitBootServicesExit')
        exitbootservicesexit_xml_et.set('RawValue', '0x%X' % self.ExitBootServicesExit)
        exitbootservicesexit_xml_et.set('ValueInMilliseconds', '%.6f' % (self.ExitBootServicesExit / 1000000.0))

        return xml_repr


class GUID_EVENT_RECORD(object):
    struct_format = '=HIQIHHBBBBBBBB'
    size = struct.calcsize(struct_format)

    def __init__(self, record_header, contents_byte_array):
        self.header = record_header
        (self.ProgressID,
         self.ApicID,
         self.Timestamp,
         self.GuidUint32,
         self.GuidUint16_0,
         self.GuidUint16_1,
         self.GuidUint8_0,
         self.GuidUint8_1,
         self.GuidUint8_2,
         self.GuidUint8_3,
         self.GuidUint8_4,
         self.GuidUint8_5,
         self.GuidUint8_6,
         self.GuidUint8_7) = struct.unpack_from(GUID_EVENT_RECORD.struct_format, contents_byte_array)

    def __str__(self):
        return str(self.header) + \
"""\n  GUID Event Record Contents
------------------------------------------------------------------
  Progress ID : 0x%04X
  Apic ID     : 0x%08X
  Timestamp   : 0x%016X
  GUID        : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X
""" % (self.ProgressID, self.ApicID, self.Timestamp, self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7)

    def toXml(self):

        xml_repr = self.header.toXml()

        progressid_xml_et = ET.SubElement(xml_repr, 'ProgressID')
        progressid_xml_et.set('Value', '0x%X' % self.ProgressID)

        apicid_xml_et = ET.SubElement(xml_repr, 'ApicID')
        apicid_xml_et.set('Value', '0x%X' % self.ApicID)

        timestamp_xml_et = ET.SubElement(xml_repr, 'Timestamp')
        timestamp_xml_et.set('RawValue', '0x%X' % self.Timestamp)
        timestamp_xml_et.set('ValueInMilliseconds', '%.6f' % (self.Timestamp / 1000000.0))

        guid_xml_et = ET.SubElement(xml_repr, 'GUID')
        guid_xml_et.set('Value', '%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X' % (self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7))

        return xml_repr


class DYNAMIC_STRING_EVENT_RECORD(object):
    struct_format = '=HIQIHHBBBBBBBB'
    size = struct.calcsize(struct_format)

    def __init__(self, record_header, contents_byte_array, string_byte_array, string_size):
        self.header = record_header
        string_format = '=' + str(string_size) + 's'
        self.String = struct.unpack_from(string_format, string_byte_array)[0]
        (self.ProgressID,
         self.ApicID,
         self.Timestamp,
         self.GuidUint32,
         self.GuidUint16_0,
         self.GuidUint16_1,
         self.GuidUint8_0,
         self.GuidUint8_1,
         self.GuidUint8_2,
         self.GuidUint8_3,
         self.GuidUint8_4,
         self.GuidUint8_5,
         self.GuidUint8_6,
         self.GuidUint8_7) = struct.unpack_from(DYNAMIC_STRING_EVENT_RECORD.struct_format, contents_byte_array)
         # in case String has non-printable chars, let's replace those with '?'
        self.String = ''.join([i if ((ord(i) == 0) or ((ord(i) > 31) and (ord(i) < 127)))  else '?' for i in self.String])

    def __str__(self):
        return str(self.header) + \
"""\n  Dynamic String Event Record Contents
------------------------------------------------------------------
  Progress ID : 0x%04X
  Apic ID     : 0x%08X
  Timestamp   : 0x%016X
  GUID        : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X
  String      : %s
""" % (self.ProgressID, self.ApicID, self.Timestamp, self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7, self.String.split('\x00')[0])

    def toXml(self):

        xml_repr = self.header.toXml()

        progressid_xml_et = ET.SubElement(xml_repr, 'ProgressID')
        progressid_xml_et.set('Value', '0x%X' % self.ProgressID)

        apicid_xml_et = ET.SubElement(xml_repr, 'ApicID')
        apicid_xml_et.set('Value', '0x%X' % self.ApicID)

        timestamp_xml_et = ET.SubElement(xml_repr, 'Timestamp')
        timestamp_xml_et.set('RawValue', '0x%X' % self.Timestamp)
        timestamp_xml_et.set('ValueInMilliseconds', '%.6f' % (self.Timestamp / 1000000.0))

        guid_xml_et = ET.SubElement(xml_repr, 'GUID')
        guid_xml_et.set('Value', '%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X' % (self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7))

        string_xml_et = ET.SubElement(xml_repr, 'String')
        string_xml_et.set('Value', '%s' % self.String.split('\x00')[0])

        return xml_repr


class DUAL_GUID_STRING_EVENT_RECORD(object):
    struct_format = '=HIQIHHBBBBBBBBIHHBBBBBBBB'
    size = struct.calcsize(struct_format)

    def __init__(self, record_header, contents_byte_array, string_byte_array, string_size):
        self.header = record_header
        string_format = '=' + str(string_size) + 's'
        self.String = struct.unpack_from(string_format, string_byte_array)[0]
        (self.ProgressID,
         self.ApicID,
         self.Timestamp,
         self.Guid1_Uint32,
         self.Guid1_Uint16_0,
         self.Guid1_Uint16_1,
         self.Guid1_Uint8_0,
         self.Guid1_Uint8_1,
         self.Guid1_Uint8_2,
         self.Guid1_Uint8_3,
         self.Guid1_Uint8_4,
         self.Guid1_Uint8_5,
         self.Guid1_Uint8_6,
         self.Guid1_Uint8_7,
         self.Guid2_Uint32,
         self.Guid2_Uint16_0,
         self.Guid2_Uint16_1,
         self.Guid2_Uint8_0,
         self.Guid2_Uint8_1,
         self.Guid2_Uint8_2,
         self.Guid2_Uint8_3,
         self.Guid2_Uint8_4,
         self.Guid2_Uint8_5,
         self.Guid2_Uint8_6,
         self.Guid2_Uint8_7) = struct.unpack_from(DUAL_GUID_STRING_EVENT_RECORD.struct_format, contents_byte_array)
         # in case String has non-printable chars, let's replace those with '?'
        self.String = ''.join([i if ((ord(i) == 0) or ((ord(i) > 31) and (ord(i) < 127)))  else '?' for i in self.String])

    def __str__(self):
        return str(self.header) + \
"""\n  Dual GUID String Event Record Contents
------------------------------------------------------------------
  Progress ID : 0x%04X
  Apic ID     : 0x%08X
  Timestamp   : 0x%016X
  GUID1       : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X
  GUID2       : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X
  String      : %s
""" % (self.ProgressID, self.ApicID, self.Timestamp, self.Guid1_Uint32, self.Guid1_Uint16_0, self.Guid1_Uint16_1, self.Guid1_Uint8_0, self.Guid1_Uint8_1, self.Guid1_Uint8_2, self.Guid1_Uint8_3, self.Guid1_Uint8_4, self.Guid1_Uint8_5, self.Guid1_Uint8_6, self.Guid1_Uint8_7, self.Guid2_Uint32, self.Guid2_Uint16_0, self.Guid2_Uint16_1, self.Guid2_Uint8_0, self.Guid2_Uint8_1, self.Guid2_Uint8_2, self.Guid2_Uint8_3, self.Guid2_Uint8_4, self.Guid2_Uint8_5, self.Guid2_Uint8_6, self.Guid2_Uint8_7, self.String.split('\x00')[0])

    def toXml(self):

        xml_repr = self.header.toXml()

        progressid_xml_et = ET.SubElement(xml_repr, 'ProgressID')
        progressid_xml_et.set('Value', '0x%X' % self.ProgressID)

        apicid_xml_et = ET.SubElement(xml_repr, 'ApicID')
        apicid_xml_et.set('Value', '0x%X' % self.ApicID)

        timestamp_xml_et = ET.SubElement(xml_repr, 'Timestamp')
        timestamp_xml_et.set('RawValue', '0x%X' % self.Timestamp)
        timestamp_xml_et.set('ValueInMilliseconds', '%.6f' % (self.Timestamp / 1000000.0))

        guid_xml_et = ET.SubElement(xml_repr, 'GUID1')
        guid_xml_et.set('Value', '%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X' % (self.Guid1_Uint32, self.Guid1_Uint16_0, self.Guid1_Uint16_1, self.Guid1_Uint8_0, self.Guid1_Uint8_1, self.Guid1_Uint8_2, self.Guid1_Uint8_3, self.Guid1_Uint8_4, self.Guid1_Uint8_5, self.Guid1_Uint8_6, self.Guid1_Uint8_7))

        guid_xml_et = ET.SubElement(xml_repr, 'GUID2')
        guid_xml_et.set('Value', '%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X' % (self.Guid2_Uint32, self.Guid2_Uint16_0, self.Guid2_Uint16_1, self.Guid2_Uint8_0, self.Guid2_Uint8_1, self.Guid2_Uint8_2, self.Guid2_Uint8_3, self.Guid2_Uint8_4, self.Guid2_Uint8_5, self.Guid2_Uint8_6, self.Guid2_Uint8_7))

        string_xml_et = ET.SubElement(xml_repr, 'String')
        string_xml_et.set('Value', '%s' % self.String.split('\x00')[0])

        return xml_repr


class GUID_QWORD_EVENT_RECORD(object):
    struct_format = '=HIQIHHBBBBBBBBQ'
    size = struct.calcsize(struct_format)

    def __init__(self, record_header, contents_byte_array):
        self.header = record_header
        (self.ProgressID,
         self.ApicID,
         self.Timestamp,
         self.GuidUint32,
         self.GuidUint16_0,
         self.GuidUint16_1,
         self.GuidUint8_0,
         self.GuidUint8_1,
         self.GuidUint8_2,
         self.GuidUint8_3,
         self.GuidUint8_4,
         self.GuidUint8_5,
         self.GuidUint8_6,
         self.GuidUint8_7,
         self.Qword) = struct.unpack_from(GUID_QWORD_EVENT_RECORD.struct_format, contents_byte_array)

    def __str__(self):
        return str(self.header) + \
"""\n  GUID Qword Event Record Contents
------------------------------------------------------------------
  Progress ID : 0x%04X
  Apic ID     : 0x%08X
  Timestamp   : 0x%016X
  GUID        : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X
  Qword       : 0x%016X
""" % (self.ProgressID, self.ApicID, self.Timestamp, self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7, self.Qword)

    def toXml(self):

        xml_repr = self.header.toXml()

        progressid_xml_et = ET.SubElement(xml_repr, 'ProgressID')
        progressid_xml_et.set('Value', '0x%X' % self.ProgressID)

        apicid_xml_et = ET.SubElement(xml_repr, 'ApicID')
        apicid_xml_et.set('Value', '0x%X' % self.ApicID)

        timestamp_xml_et = ET.SubElement(xml_repr, 'Timestamp')
        timestamp_xml_et.set('RawValue', '0x%X' % self.Timestamp)
        timestamp_xml_et.set('ValueInMilliseconds', '%.6f' % (self.Timestamp / 1000000.0))

        guid_xml_et = ET.SubElement(xml_repr, 'GUID')
        guid_xml_et.set('Value', '%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X' % (self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7))

        qword_xml_et = ET.SubElement(xml_repr, 'Qword')
        qword_xml_et.set('Value', '0x%X' % self.Qword)

        return xml_repr


class GUID_QWORD_STRING_EVENT_RECORD(object):
    struct_format = '=HIQIHHBBBBBBBBQ'
    size = struct.calcsize(struct_format)

    def __init__(self, record_header, contents_byte_array, string_byte_array, string_size):
        self.header = record_header
        string_format = '=' + str(string_size) + 's'
        self.String = struct.unpack_from(string_format, string_byte_array)[0]
        (self.ProgressID,
         self.ApicID,
         self.Timestamp,
         self.GuidUint32,
         self.GuidUint16_0,
         self.GuidUint16_1,
         self.GuidUint8_0,
         self.GuidUint8_1,
         self.GuidUint8_2,
         self.GuidUint8_3,
         self.GuidUint8_4,
         self.GuidUint8_5,
         self.GuidUint8_6,
         self.GuidUint8_7,
         self.Qword) = struct.unpack_from(GUID_QWORD_STRING_EVENT_RECORD.struct_format, contents_byte_array)
         # in case String has non-printable chars, let's replace those with '?'
        self.String = ''.join([i if ((ord(i) == 0) or ((ord(i) > 31) and (ord(i) < 127)))  else '?' for i in self.String])

    def __str__(self):
        return str(self.header) + \
"""\n  GUID Qword String Event Record Contents
------------------------------------------------------------------
  Progress ID : 0x%04X
  Apic ID     : 0x%08X
  Timestamp   : 0x%016X
  GUID        : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X
  Qword       : 0x%016X
  String      : %s
""" % (self.ProgressID, self.ApicID, self.Timestamp, self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7, self.Qword, self.String.split('\x00')[0])

    def toXml(self):

        xml_repr = self.header.toXml()

        progressid_xml_et = ET.SubElement(xml_repr, 'ProgressID')
        progressid_xml_et.set('Value', '0x%X' % self.ProgressID)

        apicid_xml_et = ET.SubElement(xml_repr, 'ApicID')
        apicid_xml_et.set('Value', '0x%X' % self.ApicID)

        timestamp_xml_et = ET.SubElement(xml_repr, 'Timestamp')
        timestamp_xml_et.set('RawValue', '0x%X' % self.Timestamp)
        timestamp_xml_et.set('ValueInMilliseconds', '%.6f' % (self.Timestamp / 1000000.0))

        guid_xml_et = ET.SubElement(xml_repr, 'GUID')
        guid_xml_et.set('Value', '%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X' % (self.GuidUint32, self.GuidUint16_0, self.GuidUint16_1, self.GuidUint8_0, self.GuidUint8_1, self.GuidUint8_2, self.GuidUint8_3, self.GuidUint8_4, self.GuidUint8_5, self.GuidUint8_6, self.GuidUint8_7))

        qword_xml_et = ET.SubElement(xml_repr, 'Qword')
        qword_xml_et.set('Value', '0x%X' % self.Qword)

        string_xml_et = ET.SubElement(xml_repr, 'String')
        string_xml_et.set('Value', '%s' % self.String.split('\x00')[0])

        return xml_repr


class SystemFirmwareTable(object):

    def __init__(self):
        # enable required SeSystemEnvironmentPrivilege privilege
        privilege = win32security.LookupPrivilegeValue( None, 'SeSystemEnvironmentPrivilege' )
        token = win32security.OpenProcessToken( win32process.GetCurrentProcess(), win32security.TOKEN_READ|win32security.TOKEN_ADJUST_PRIVILEGES )
        win32security.AdjustTokenPrivileges( token, False, [(privilege, win32security.SE_PRIVILEGE_ENABLED)] )
        win32api.CloseHandle( token )

        kernel32 = windll.kernel32
        ntdll    = windll.ntdll

        # import firmware variable APIs
        try:
            self._GetSystemFirmwareTable = kernel32.GetSystemFirmwareTable
            self._GetSystemFirmwareTable.restype = c_int
            self._GetSystemFirmwareTable.argtypes = [c_int, c_int, c_void_p, c_int]

            self._NtQuerySystemInformation = ntdll.NtQuerySystemInformation

            # NTSTATUS WINAPI NtQuerySystemInformation(
            #     _In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
            #     _Inout_   PVOID                    SystemInformation,
            #     _In_      ULONG                    SystemInformationLength,
            #     _Out_opt_ PULONG                   ReturnLength
            # );
            self._NtQuerySystemInformation.restype = c_ulong
            self._NtQuerySystemInformation.argtypes = [c_int, c_void_p, c_ulong, POINTER(c_ulong)]

        except AttributeError:
            logging.error( "GetSystemFirmwareTable function doesn't seem to exist" )
            pass

    #
    #Function to get an AcpiTable
    # return a tuple of error code, table data, and errorstring (None if not error)
    #
    def GetAcpiTable(self, TableId):
        err = 0 #success
        TableType = struct.unpack('>i', 'ACPI')[0]  #big endian
        TableIdAsInt = struct.unpack('<i', TableId)[0]  #TableId is Little endian or native
        TableLength = 1000
        Table = create_string_buffer( TableLength )
        if self._GetSystemFirmwareTable is not None:
            logging.info("calling GetSystemFirmwareTable( FwTableProvider=0x%x, FwTableId=0x%X )" % (TableType, TableIdAsInt))
            length = self._GetSystemFirmwareTable(TableType , TableIdAsInt, Table, TableLength)
            if(length > TableLength):
                logging.info("Table Length is: 0x%x" % length)
                Table = create_string_buffer( length )
                length2 = self._GetSystemFirmwareTable( TableType, TableIdAsInt, Table, length)

                if(length2 != length):
                    err = kernel32.GetLastError()
                    logging.error( 'GetSystemFirmwareTable failed (GetLastError = 0x%x)' %  err)
                    logging.error(WinError())
                    return (err, None, WinError(err))
            elif length != 0:
                return (err, Table[:length], None)
            err = kernel32.GetLastError()
            logging.error( 'GetSystemFirmwareTable failed (GetLastError = 0x%x)' %  err)
            logging.error(WinError())
            return (err, None, "Length = 0")
        return (-20, None, "GetSystemFirmwareTable is None")

    def GetFbpt(self):
        param0 = c_int(198) # 198 is SystemFirmwareBootPerformanceInformation
        param1 = c_void_p(0)
        param2 = c_ulong(0)
        param3c = c_ulong(0)
        param3 = pointer(param3c)

        # first call the function with NULL SystemInformation and 0 SystemInformationLength
        # this way we'll get the size of FBPT in ReturnLength
        status = self._NtQuerySystemInformation(param0, param1, param2, param3)

        # 0xc0000004 is STATUS_INFO_LENGTH_MISMATCH
        if (status != 0xc0000004 or param3c == 0):
            return (1, None)

        param1 = create_string_buffer(param3c.value)
        param2 = param3c
        status = self._NtQuerySystemInformation(param0, param1, param2, param3)

        if (status != 0):
            return (1, None)

        return (status, param1)


# Helper function for parsing FBPT contents
def FbptParsingFactory(fbpt_contents_file, fbpt_records_list):
    # We need to read first record's header first (4 bytes), then using the length of this record, read the rest
    # of the record
    fbpt_contents_string = fbpt_contents_file.read(FBPT_RECORD_HEADER.size)

    # If the size of returned string for the header is less than the size of the header that we
    # requested, it means we've reached EOF
    # The check below is more strict, it assumes the file stops after a record and not in the middle of a header
    while(fbpt_contents_string != ''):
        fbpt_record_header = FBPT_RECORD_HEADER(fbpt_contents_string)

        # If unknown record type print it and stop
        if (fbpt_record_header.PerformanceRecordType not in known_FBPT_record_types):
            logging.critical('Uknown record type 0x%04x' % fbpt_record_header.PerformanceRecordType)
            return 1

        # Using the record type from the record header we will fill out a struct with the info from the record and will print it
        # FIRMWARE_BASIC_BOOT_PERFORMANCE_DATA_EVENT_TYPE
        # GUID_EVENT_TYPE
        # DYNAMIC_STRING_EVENT_TYPE
        # DUAL_GUID_STRING_EVENT_TYPE
        # GUID_QWORD_EVENT_TYPE
        # GUID_QWORD_STRING_EVENT_TYPE
        if (fbpt_record_header.PerformanceRecordType == FIRMWARE_BASIC_BOOT_PERFORMANCE_DATA_EVENT_TYPE):
            fw_basic_boot_performance_data_record_string = fbpt_contents_file.read(FW_BASIC_BOOT_PERFORMANCE_DATA_RECORD.size)
            fw_basic_boot_performance_data_record = FW_BASIC_BOOT_PERFORMANCE_DATA_RECORD(fbpt_record_header, fw_basic_boot_performance_data_record_string)
            fbpt_records_list.append(fw_basic_boot_performance_data_record)
        elif (fbpt_record_header.PerformanceRecordType == GUID_EVENT_TYPE):
            guid_event_record_contents_string = fbpt_contents_file.read(GUID_EVENT_RECORD.size)
            guid_event_record = GUID_EVENT_RECORD(fbpt_record_header, guid_event_record_contents_string)
            fbpt_records_list.append(guid_event_record)
        elif (fbpt_record_header.PerformanceRecordType == DYNAMIC_STRING_EVENT_TYPE):
            dynamic_string_event_record_contents_string = fbpt_contents_file.read(DYNAMIC_STRING_EVENT_RECORD.size)
            dynamic_string_string_size = fbpt_record_header.RecordLength - FBPT_RECORD_HEADER.size - DYNAMIC_STRING_EVENT_RECORD.size
            dynamic_string_string_byte_array = fbpt_contents_file.read(dynamic_string_string_size)
            dynamic_string_event_record = DYNAMIC_STRING_EVENT_RECORD(fbpt_record_header, dynamic_string_event_record_contents_string, dynamic_string_string_byte_array, dynamic_string_string_size)
            fbpt_records_list.append(dynamic_string_event_record)
        elif (fbpt_record_header.PerformanceRecordType == DUAL_GUID_STRING_EVENT_TYPE):
            dual_guid_string_event_record_contents_string = fbpt_contents_file.read(DUAL_GUID_STRING_EVENT_RECORD.size)
            dual_guid_string_string_size = fbpt_record_header.RecordLength - FBPT_RECORD_HEADER.size - DUAL_GUID_STRING_EVENT_RECORD.size
            dual_guid_string_string_byte_array = fbpt_contents_file.read(dual_guid_string_string_size)
            dual_guid_string_event_record = DUAL_GUID_STRING_EVENT_RECORD(fbpt_record_header, dual_guid_string_event_record_contents_string, dual_guid_string_string_byte_array, dual_guid_string_string_size)
            fbpt_records_list.append(dual_guid_string_event_record)
        elif (fbpt_record_header.PerformanceRecordType == GUID_QWORD_EVENT_TYPE):
            guid_qword_event_record_contents_string = fbpt_contents_file.read(GUID_QWORD_EVENT_RECORD.size)
            guid_qword_event_record = GUID_QWORD_EVENT_RECORD(fbpt_record_header, guid_qword_event_record_contents_string)
            fbpt_records_list.append(guid_qword_event_record)
        elif (fbpt_record_header.PerformanceRecordType == GUID_QWORD_STRING_EVENT_TYPE):
            guid_qword_string_event_record_contents_string = fbpt_contents_file.read(GUID_QWORD_STRING_EVENT_RECORD.size)
            guid_qword_string_string_size = fbpt_record_header.RecordLength - FBPT_RECORD_HEADER.size - GUID_QWORD_STRING_EVENT_RECORD.size
            guid_qword_string_string_byte_array = fbpt_contents_file.read(guid_qword_string_string_size)
            guid_qword_string_event_record = GUID_QWORD_STRING_EVENT_RECORD(fbpt_record_header, guid_qword_string_event_record_contents_string, guid_qword_string_string_byte_array, guid_qword_string_string_size)
            fbpt_records_list.append(guid_qword_string_event_record)

        # Read next record's header
        fbpt_contents_string = fbpt_contents_file.read(FBPT_RECORD_HEADER.size)

    return 0


def getUefiVersion():
    tempFileName = 'temp.txt'
    wmicBiosOutputFile = open(tempFileName, 'w')
    command = ['wmic', 'bios']
    p = subprocess.Popen(command, stdout=wmicBiosOutputFile)
    p.wait()
    wmicBiosOutputFile.close()

    wmicBiosOutputFile = open(tempFileName, 'rb')
    wmicBiosOutputString = wmicBiosOutputFile.read().decode('utf-16').split('\n')
    wmicBiosOutputFile.close()

    if (os.path.isfile(tempFileName)):
        os.remove(tempFileName)

    # First line is table header, UEFI version is under Caption
    captionIndex = wmicBiosOutputString[0].find('Caption')

    # The BIOS version is in XXXX.XXXX.XXXX format, UINT32 in 10-12-10 bit split, so with dots it's 14 characters at most, then spaces
    uefiVersion =  wmicBiosOutputString[1][captionIndex:(captionIndex + 14)].rstrip()

    return uefiVersion


def getModel():
    tempFileName = 'temp.txt'
    wmicGetModelFile = open(tempFileName, 'w')
    command = ['wmic', 'computersystem', 'get', 'model']
    p = subprocess.Popen(command, stdout=wmicGetModelFile)
    p.wait()
    wmicGetModelFile.close()

    wmicGetModelFile = open(tempFileName, 'rb')
    wmicGetModelString = wmicGetModelFile.read().decode('utf-16').split('\n')
    wmicGetModelFile.close()

    if (os.path.isfile(tempFileName)):
        os.remove(tempFileName)

    # First line says "Model", second line is what we want, but rstrip in case there's whitespace
    model = wmicGetModelString[1].rstrip()

    return model


if __name__ == "__main__":

    #
    # 1. Setup: command line args, logger, cleanup before we start, create the text file log, create XML tree with UEFI version and model
    #

    # Set up command line arguments
    parser = argparse.ArgumentParser(description='FPDT Parser Tool')
    parser.add_argument('-t', '--OutputText', dest='output_text_file', help='Name of the output text file which will contain the FPDT info', default=None)
    parser.add_argument('-x', '--OutputXml', dest='output_xml_file', help='Name of the output XML file which will contain the FPDT info', default=None)
    options = parser.parse_args()

    # Set up logging
    logger = logging.getLogger('')
    logger.setLevel(logging.INFO)
    formatter = logging.Formatter("%(levelname)s - %(message)s")
    console = logging.StreamHandler()
    console.setLevel(logging.CRITICAL)
    console.setFormatter(formatter)
    logger.addHandler(console)

    if(options.output_xml_file):
        if(len(options.output_xml_file) < 2):
            logging.critical('The output XML file parameter is invalid')
            sys.exit(1)

    if(options.output_text_file):
        if(len(options.output_text_file) < 2):
            logging.critical('The output text file parameter is invalid')
            sys.exit(1)
        else:
            # Create a new text log file
            text_log = open(options.output_text_file, 'w')

    # Get UEFI version and model, print and/or store in XML
    uefi_version = getUefiVersion()
    model = getModel()
    if(options.output_text_file):
        text_log.write("  Platform Information\n------------------------------------------------------------------\n  UEFI Version : %s\n  Model        : %s\n" % (uefi_version, model))
    if(options.output_xml_file):
        xml_tree = ET.Element('FpdtParserData')
        xml_repr = ET.Element('UEFIVersion')
        xml_repr.set('Value', uefi_version)
        xml_tree.append(xml_repr)

        xml_repr = ET.Element('Model')
        xml_repr.set('Value', model)
        xml_tree.append(xml_repr)

        date_collected = str(datetime.datetime.now().month) + '/' + str(datetime.datetime.now().day) + '/' + str(datetime.datetime.now().year)
        xml_repr = ET.Element('DateCollected')
        xml_repr.set('Value', date_collected)
        xml_tree.append(xml_repr)

        xml_repr = ET.Element('FpdtParserVersion')
        xml_repr.set('Value', FpdtParserVersion)
        xml_tree.append(xml_repr)

    #
    # 2. Inspect FPDT: header and the basic boot perf record in it, if relevant log both in XML and/or in text
    #

    Table = SystemFirmwareTable()
    (errorcode, data, errorstring) = Table.GetAcpiTable('FPDT')
    fpdt_header = ACPI_TABLE_HEADER(data)

    # Store FPDT header in text and/or XML tree
    if(options.output_text_file):
        text_log.write(str(fpdt_header))
    if(options.output_xml_file):
        xml_tree.append(fpdt_header.toXml())

    # This assumes we only have one perf record - Firmware Basic Boot Performance Record
    if ((fpdt_header.Length - ACPI_TABLE_HEADER.size) > FW_BASIC_BOOT_PERFORMANCE_RECORD.size):
        logging.critical('Extra records are present in FPDT but will be ignored')

    # Parse the basic boot perf record
    fbbpr = FW_BASIC_BOOT_PERFORMANCE_RECORD(data[ACPI_TABLE_HEADER.size:])

    # Store the basic boot perf record in text and/or XML tree
    if(options.output_text_file):
        text_log.write(str(fbbpr))
    if(options.output_xml_file):
        xml_tree.append(fbbpr.toXml())

    #
    # 3. Read FBPT. Parse its header and the contents.
    #
    (return_code, fbpt_buffer) = Table.GetFbpt()

    if (return_code != 0):
        logging.critical(r"This version of Windows doesn't support access to FBPT - aborting")
        sys.exit(1)
    else:
        # GetFbpt returned expected return_code, so let's use the buffer it returned
        fbpt_file_w = open('FBPT.BIN', 'wb')
        fbpt_file_w.write(fbpt_buffer)
        fbpt_file_w.close()
        fbpt_file = open('FBPT.BIN', 'rb')


    fbpt_header = FW_BASIC_BOOT_PERFORMANCE_TABLE_HEADER(fbpt_file.read(FW_BASIC_BOOT_PERFORMANCE_TABLE_HEADER.size))
    # Store header into text log and/or XML tree
    if(options.output_text_file):
        text_log.write(str(fbpt_header))
    if(options.output_xml_file):
        # Store FBPT header and records under a separate element under FPDT
        fbpt_tree = fbpt_header.toXml()
        xml_tree.append(fbpt_tree)

    #
    # 4. Parse the FBPT records and store record objects into the list
    #

    fbpt_records_list = list()

    # This helper function parses through the  FBPT records and populates the list with records
    fbpt_parse_result = FbptParsingFactory(fbpt_file, fbpt_records_list)

    fbpt_file.close()
    os.remove('FBPT.BIN')

    if (fbpt_parse_result == 1):
        if(options.output_text_file):
            text_log.close()
        logging.shutdown()
        sys.exit(1)

    #
    # 5. Iterate over the records list to store records into the XML tree and then an XML file,
    #    store into text log if specified via args
    #

    if(options.output_xml_file):
        for record in fbpt_records_list:
            fbpt_tree.append(record.toXml())

        xml_file = open(options.output_xml_file, 'w')
        xml_file.write(ET.tostring(xml_tree))
        xml_file.close()

    if(options.output_text_file):
        for record in fbpt_records_list:
            text_log.write(str(record))
        text_log.write('\nFBPT Record count: %d\n' % len(fbpt_records_list))
        text_log.close()

    #
    # 6. Finish: Print success, turn off logging, exit with 0
    #

    logging.critical('SUCCESS, %d record(s) parsed' % len(fbpt_records_list))
    logging.shutdown()
    sys.exit(0)

