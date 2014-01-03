function [grid_system_header] = pb_read_QDLIB__GridSystemHeader(buffer, buffer_start, buffer_end)
%pb_read_QDLIB__GridSystemHeader Reads the protobuf message GridSystemHeader.
%   function [grid_system_header] = pb_read_QDLIB__GridSystemHeader(buffer, buffer_start, buffer_end)
%
%   INPUTS:
%     buffer       : a buffer of uint8's to parse
%     buffer_start : optional starting index to consider of the buffer
%                    defaults to 1
%     buffer_end   : optional ending index to consider of the buffer
%                    defaults to length(buffer)
%
%   MEMBERS:
%     dims           : required uint32, defaults to uint32(0).
%     dim            : repeated <a href="matlab:help pb_read_QDLIB__GridSystemHeader__dim_description_t">QDLIB.GridSystemHeader.dim_description_t</a>, defaults to struct([]).
%     data_size      : optional uint64, defaults to uint64(0).
%
%   See also pb_read_QDLIB__GridSystemHeader__dim_description_t.
  
  if (nargin < 1)
    buffer = uint8([]);
  end
  if (nargin < 2)
    buffer_start = 1;
  end
  if (nargin < 3)
    buffer_end = length(buffer);
  end
  
  descriptor = pb_descriptor_QDLIB__GridSystemHeader();
  grid_system_header = pblib_generic_parse_from_string(buffer, descriptor, buffer_start, buffer_end);
  grid_system_header.descriptor_function = @pb_descriptor_QDLIB__GridSystemHeader;
