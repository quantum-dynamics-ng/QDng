function [dim_description_t] = pb_read_QDLIB__GridSystemHeader__dim_description_t(buffer, buffer_start, buffer_end)
%pb_read_QDLIB__GridSystemHeader__dim_description_t Reads the protobuf message dim_description_t.
%   function [dim_description_t] = pb_read_QDLIB__GridSystemHeader__dim_description_t(buffer, buffer_start, buffer_end)
%
%   INPUTS:
%     buffer       : a buffer of uint8's to parse
%     buffer_start : optional starting index to consider of the buffer
%                    defaults to 1
%     buffer_end   : optional ending index to consider of the buffer
%                    defaults to length(buffer)
%
%   MEMBERS:
%     size           : required uint32, defaults to uint32(0).
%     xmin           : required double, defaults to double(0).
%     xmax           : required double, defaults to double(0).
%
%   See also pb_read_QDLIB__GridSystemHeader.
  
  if (nargin < 1)
    buffer = uint8([]);
  end
  if (nargin < 2)
    buffer_start = 1;
  end
  if (nargin < 3)
    buffer_end = length(buffer);
  end
  
  descriptor = pb_descriptor_QDLIB__GridSystemHeader__dim_description_t();
  dim_description_t = pblib_generic_parse_from_string(buffer, descriptor, buffer_start, buffer_end);
  dim_description_t.descriptor_function = @pb_descriptor_QDLIB__GridSystemHeader__dim_description_t;
