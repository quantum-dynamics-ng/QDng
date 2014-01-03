function [response] = pb_read_QDLIB__Response(buffer, buffer_start, buffer_end)
%pb_read_QDLIB__Response Reads the protobuf message Response.
%   function [response] = pb_read_QDLIB__Response(buffer, buffer_start, buffer_end)
%
%   INPUTS:
%     buffer       : a buffer of uint8's to parse
%     buffer_start : optional starting index to consider of the buffer
%                    defaults to 1
%     buffer_end   : optional ending index to consider of the buffer
%                    defaults to length(buffer)
%
%   MEMBERS:
%     response       : required enum, defaults to int32(0).
%     msg            : optional string, defaults to ''.
%
%   See also pb_read_QDLIB__Command.
  
  if (nargin < 1)
    buffer = uint8([]);
  end
  if (nargin < 2)
    buffer_start = 1;
  end
  if (nargin < 3)
    buffer_end = length(buffer);
  end
  
  descriptor = pb_descriptor_QDLIB__Response();
  response = pblib_generic_parse_from_string(buffer, descriptor, buffer_start, buffer_end);
  response.descriptor_function = @pb_descriptor_QDLIB__Response;
