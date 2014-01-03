function [multi_state_identifier] = pb_read_QDLIB__MultiStateIdentifier(buffer, buffer_start, buffer_end)
%pb_read_QDLIB__MultiStateIdentifier Reads the protobuf message MultiStateIdentifier.
%   function [multi_state_identifier] = pb_read_QDLIB__MultiStateIdentifier(buffer, buffer_start, buffer_end)
%
%   INPUTS:
%     buffer       : a buffer of uint8's to parse
%     buffer_start : optional starting index to consider of the buffer
%                    defaults to 1
%     buffer_end   : optional ending index to consider of the buffer
%                    defaults to length(buffer)
%
%   MEMBERS:
%     state          : required int32, defaults to int32(0).
%
%   See also pb_read_QDLIB__FileSingleHeader.
  
  if (nargin < 1)
    buffer = uint8([]);
  end
  if (nargin < 2)
    buffer_start = 1;
  end
  if (nargin < 3)
    buffer_end = length(buffer);
  end
  
  descriptor = pb_descriptor_QDLIB__MultiStateIdentifier();
  multi_state_identifier = pblib_generic_parse_from_string(buffer, descriptor, buffer_start, buffer_end);
  multi_state_identifier.descriptor_function = @pb_descriptor_QDLIB__MultiStateIdentifier;
