function [descriptor] = pb_descriptor_QDLIB__FileSingleHeader()
%pb_descriptor_QDLIB__FileSingleHeader Returns the descriptor for message FileSingleHeader.
%   function [descriptor] = pb_descriptor_QDLIB__FileSingleHeader()
%
%   See also pb_read_QDLIB__FileSingleHeader
  
  descriptor = struct( ...
    'name', 'FileSingleHeader', ...
    'full_name', 'QDLIB.FileSingleHeader', ...
    'filename', 'tools/FileSingleHeader.proto', ...
    'containing_type', '', ...
    'fields', [ ...
      struct( ...
        'name', 'more_files_follow', ...
        'full_name', 'QDLIB.FileSingleHeader.more_files_follow', ...
        'index', 1, ...
        'number', uint32(1), ...
        'type', uint32(8), ...
        'matlab_type', uint32(3), ...
        'wire_type', uint32(0), ...
        'label', uint32(2), ...
        'default_value', uint32(0), ...
        'read_function', @(x) pblib_helpers_first(typecast(x, 'uint32')), ...
        'write_function', @(x) typecast(uint32(x), 'uint32'), ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'class', ...
        'full_name', 'QDLIB.FileSingleHeader.class', ...
        'index', 2, ...
        'number', uint32(2), ...
        'type', uint32(9), ...
        'matlab_type', uint32(7), ...
        'wire_type', uint32(2), ...
        'label', uint32(2), ...
        'default_value', 'NoClass', ...
        'read_function', @(x) char(x{1}(x{2} : x{3})), ...
        'write_function', @uint8, ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'name', ...
        'full_name', 'QDLIB.FileSingleHeader.name', ...
        'index', 3, ...
        'number', uint32(3), ...
        'type', uint32(9), ...
        'matlab_type', uint32(7), ...
        'wire_type', uint32(2), ...
        'label', uint32(1), ...
        'default_value', '', ...
        'read_function', @(x) char(x{1}(x{2} : x{3})), ...
        'write_function', @uint8, ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'sequence_number', ...
        'full_name', 'QDLIB.FileSingleHeader.sequence_number', ...
        'index', 4, ...
        'number', uint32(4), ...
        'type', uint32(13), ...
        'matlab_type', uint32(3), ...
        'wire_type', uint32(0), ...
        'label', uint32(1), ...
        'default_value', uint32(0), ...
        'read_function', @(x) pblib_helpers_first(typecast(x, 'uint32')), ...
        'write_function', @(x) typecast(uint32(x), 'uint32'), ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'meta_data', ...
        'full_name', 'QDLIB.FileSingleHeader.meta_data', ...
        'index', 5, ...
        'number', uint32(5), ...
        'type', uint32(9), ...
        'matlab_type', uint32(7), ...
        'wire_type', uint32(2), ...
        'label', uint32(1), ...
        'default_value', '', ...
        'read_function', @(x) char(x{1}(x{2} : x{3})), ...
        'write_function', @uint8, ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'payload_size', ...
        'full_name', 'QDLIB.FileSingleHeader.payload_size', ...
        'index', 6, ...
        'number', uint32(6), ...
        'type', uint32(4), ...
        'matlab_type', uint32(4), ...
        'wire_type', uint32(0), ...
        'label', uint32(2), ...
        'default_value', uint64(0), ...
        'read_function', @(x) typecast(x, 'uint64'), ...
        'write_function', @(x) typecast(uint64(x), 'uint64'), ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'compression', ...
        'full_name', 'QDLIB.FileSingleHeader.compression', ...
        'index', 7, ...
        'number', uint32(7), ...
        'type', uint32(14), ...
        'matlab_type', uint32(10), ...
        'wire_type', uint32(0), ...
        'label', uint32(2), ...
        'default_value', int32(0), ...
        'read_function', @(x) pblib_helpers_first(typecast(x, 'int32')), ...
        'write_function', @(x) typecast(int32(x), 'uint32'), ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'sub_header', ...
        'full_name', 'QDLIB.FileSingleHeader.sub_header', ...
        'index', 8, ...
        'number', uint32(10), ...
        'type', uint32(12), ...
        'matlab_type', uint32(8), ...
        'wire_type', uint32(2), ...
        'label', uint32(1), ...
        'default_value', uint8(''), ...
        'read_function', @(x) uint8(x{1}(x{2} : x{3})), ...
        'write_function', @uint8, ...
        'options', struct('packed', false) ...
      ) ...
    ], ...
    'extensions', [ ... % Not Implemented
    ], ...
    'nested_types', [ ... % Not implemented
    ], ...
    'enum_types', [ ... % Not Implemented
    ], ...
    'options', [ ... % Not Implemented
    ] ...
  );
  
  descriptor.field_indeces_by_number = java.util.HashMap;
  put(descriptor.field_indeces_by_number, uint32(1), 1);
  put(descriptor.field_indeces_by_number, uint32(2), 2);
  put(descriptor.field_indeces_by_number, uint32(3), 3);
  put(descriptor.field_indeces_by_number, uint32(4), 4);
  put(descriptor.field_indeces_by_number, uint32(5), 5);
  put(descriptor.field_indeces_by_number, uint32(6), 6);
  put(descriptor.field_indeces_by_number, uint32(7), 7);
  put(descriptor.field_indeces_by_number, uint32(10), 8);
  
