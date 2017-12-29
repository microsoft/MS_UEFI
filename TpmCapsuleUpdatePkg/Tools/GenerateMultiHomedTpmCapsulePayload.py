## @file GenerateMultiHomedTpmCapsulePayload.py
# This script takes in multiple Infineon TPM update files and:
# - Checks to make sure they all share the same 'destination' version.
# - Packages them in a multi-homed payload for packaging inside a standard MS FW update capsule.
#
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
##

import os
import re
import struct
import argparse

def parse_options():
  parser = argparse.ArgumentParser(description='Add a header to a TPM FW payload.')
  parser.add_argument('-i', dest='input', metavar='<input file>', help='input file (can have multiple)', required=True, action='append')
  parser.add_argument('-o', dest='output', metavar='<output file>', help='output file', required=True)

  return parser.parse_args()

def get_infineon_versions_from_path(input_path):
  file_name = os.path.basename(input_path)
  version_parts = re.findall(r"\d+\.\d+\.\d+\.\d+", file_name)
  if version_parts is None or len(version_parts) != 2:
    raise ValueError("File name '%s' does not look like an Infineon filename!" % input_path)
  return {'from': version_parts[0], 'to': version_parts[1], "path": input_path}

def string_version_to_uint(string_version):
  version_parts = string_version.split('.')
  # This is very Infineon-specific.
  return (int(version_parts[1]) << 24) + (int(version_parts[2]) << 8) + int(version_parts[3])

def main():
  args = parse_options()

  #
  # Validate all the arguments.
  #
  # All input files must exist.
  for input_path in args.input:
    if not os.path.isfile(input_path):
      raise ValueError("All input files must exist!")
  # Output path must exist.
  if not os.path.isdir(os.path.dirname(args.output)):
    raise ValueError("Output path must exist!")

  #
  # Get all the "from" and "to" versions from Infineon filenames.
  #
  input_versions = list(get_infineon_versions_from_path(input_path) for input_path in args.input)
  # If there are multiple inputs, make sure they all make sense.
  if len(input_versions) > 0xFF:
    raise ValueError("Too many input files!")
  if len(input_versions) > 1:
    # Make sure that all inputs share the same "to" version.
    target_version = input_versions[0]["to"]
    for version in input_versions:
      if target_version != version["to"]:
        raise ValueError("All inputs must share the same \"to\" version!")

    # Make sure all inputs have unique "from" versions.
    accounted_for = set()
    for version in input_versions:
      if version["from"] in accounted_for:
        raise ValueError("All inputs must have unique \"from\" versions!")
      accounted_for.add(version["from"])

  #
  # Now, construct the output file, and we should be good.
  #
  with open(args.output, 'wb') as output_file:
    #
    # Write the header to Output.
    # NOTE: This header string must be kept in sync with the header structure
    #       in the TPM Capusle library in MsSurfaceModulePkg.
    #
    # typedef struct _TPM_CAPSULE_PAYLOAD_HEADER {
    #   UINT32      Signature;
    #   UINT32      Version;
    #   UINT16      SubPayloadCount;
    #   // TPM_CAPSULE_SUBPAYLOAD_HEADER    SubPayloads[];
    # } TPM_CAPSULE_PAYLOAD_HEADER;
    #
    # typedef struct _TPM_CAPSULE_SUBPAYLOAD_HEADER {
    #   UINT32      FromVersion;
    #   UINT32      PayloadSize;
    #   // UINT8       SubPayload[];
    # } TPM_CAPSULE_SUBPAYLOAD_HEADER;
    #
    header_struct = "<ccccLH"   ## Currently force little-endian.
    header_bytes = struct.pack(header_struct, 'M', 'T', 'C', 'P', 1, len(input_versions))
    output_file.write(header_bytes)

    #
    # Iterate through all input files and add them to the payload.
    #
    for input_info in input_versions:
      input_file_path = input_info["path"]
      input_file_size = os.stat(input_file_path).st_size

      # Write the header to the output.
      sub_header_struct = "<LL"   ## Currently force little-endian.
      sub_header_bytes = struct.pack(sub_header_struct, string_version_to_uint(input_info['from']), input_file_size)
      output_file.write(sub_header_bytes)

      # Write the contents of the input file to the output file.
      with open(input_info["path"], 'rb') as input_file:
        output_file.write(input_file.read())

  return

main()
