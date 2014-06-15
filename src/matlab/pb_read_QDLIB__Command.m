function [command] = pb_read_QDLIB__Command(buffer, buffer_start, buffer_end)
%pb_read_QDLIB__Command Reads the protobuf message Command.
%   function [command] = pb_read_QDLIB__Command(buffer, buffer_start, buffer_end)
%
%   INPUTS:
%     buffer       : a buffer of uint8's to parse
%     buffer_start : optional starting index to consider of the buffer
%                    defaults to 1
%     buffer_end   : optional ending index to consider of the buffer
%                    defaults to length(buffer)
%
%   MEMBERS:
%     cmd            : required enum, defaults to int32(0).
%     XML            : optional string, defaults to ''.
%     param_s1       : optional string, defaults to ''.
%     param_s2       : optional string, defaults to ''.
%     param_s3       : optional string, defaults to ''.
%     param_d1       : optional double, defaults to double(0).
%     param_d2       : optional double, defaults to double(0).
%     param_d3       : optional double, defaults to double(0).
%
%   See also pb_read_QDLIB__Response.
  
  if (nargin < 1)
    buffer = uint8([]);
  end
  if (nargin < 2)
    buffer_start = 1;
  end
  if (nargin < 3)
    buffer_end = length(buffer);
  end
  
  descriptor = pb_descriptor_QDLIB__Command();
  command = pblib_generic_parse_from_string(buffer, descriptor, buffer_start, buffer_end);
  command.descriptor_function = @pb_descriptor_QDLIB__Command;
