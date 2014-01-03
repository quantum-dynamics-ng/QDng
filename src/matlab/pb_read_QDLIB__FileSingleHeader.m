function [file_single_header] = pb_read_QDLIB__FileSingleHeader(buffer, buffer_start, buffer_end)
%pb_read_QDLIB__FileSingleHeader Reads the protobuf message FileSingleHeader.
%   function [file_single_header] = pb_read_QDLIB__FileSingleHeader(buffer, buffer_start, buffer_end)
%
%   INPUTS:
%     buffer       : a buffer of uint8's to parse
%     buffer_start : optional starting index to consider of the buffer
%                    defaults to 1
%     buffer_end   : optional ending index to consider of the buffer
%                    defaults to length(buffer)
%
%   MEMBERS:
%     more_files_follow: required uint32, defaults to uint32(0).
%     class          : required string, defaults to ''.
%     name           : optional string, defaults to ''.
%     sequence_number: optional uint32, defaults to uint32(0).
%     meta_data      : required string, defaults to ''.
%     payload_size   : required uint64, defaults to uint64(0).
%     compression    : required enum, defaults to int32(0).
%     sub_header     : optional uint8 vector, defaults to uint8('').
%
%   See also pb_read_QDLIB__MultiStateIdentifier.
  
  if (nargin < 1)
    buffer = uint8([]);
  end
  if (nargin < 2)
    buffer_start = 1;
  end
  if (nargin < 3)
    buffer_end = length(buffer);
  end
  
  descriptor = pb_descriptor_QDLIB__FileSingleHeader();
  file_single_header = pblib_generic_parse_from_string(buffer, descriptor, buffer_start, buffer_end);
  file_single_header.descriptor_function = @pb_descriptor_QDLIB__FileSingleHeader;
