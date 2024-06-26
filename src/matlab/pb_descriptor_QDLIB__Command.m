function [descriptor] = pb_descriptor_QDLIB__Command()
%pb_descriptor_QDLIB__Command Returns the descriptor for message Command.
%   function [descriptor] = pb_descriptor_QDLIB__Command()
%
%   See also pb_read_QDLIB__Command
  
  descriptor = struct( ...
    'name', 'Command', ...
    'full_name', 'QDLIB.Command', ...
    'filename', 'IACmds.proto', ...
    'containing_type', '', ...
    'fields', [ ...
      struct( ...
        'name', 'cmd', ...
        'full_name', 'QDLIB.Command.cmd', ...
        'index', 1, ...
        'number', uint32(1), ...
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
        'name', 'XML', ...
        'full_name', 'QDLIB.Command.XML', ...
        'index', 2, ...
        'number', uint32(2), ...
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
        'name', 'param_s1', ...
        'full_name', 'QDLIB.Command.param_s1', ...
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
        'name', 'param_s2', ...
        'full_name', 'QDLIB.Command.param_s2', ...
        'index', 4, ...
        'number', uint32(4), ...
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
        'name', 'param_s3', ...
        'full_name', 'QDLIB.Command.param_s3', ...
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
        'name', 'param_d1', ...
        'full_name', 'QDLIB.Command.param_d1', ...
        'index', 6, ...
        'number', uint32(6), ...
        'type', uint32(1), ...
        'matlab_type', uint32(5), ...
        'wire_type', uint32(1), ...
        'label', uint32(1), ...
        'default_value', double(0), ...
        'read_function', @(x) typecast(x, 'double'), ...
        'write_function', @(x) typecast(double(x), 'uint8'), ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'param_d2', ...
        'full_name', 'QDLIB.Command.param_d2', ...
        'index', 7, ...
        'number', uint32(7), ...
        'type', uint32(1), ...
        'matlab_type', uint32(5), ...
        'wire_type', uint32(1), ...
        'label', uint32(1), ...
        'default_value', double(0), ...
        'read_function', @(x) typecast(x, 'double'), ...
        'write_function', @(x) typecast(double(x), 'uint8'), ...
        'options', struct('packed', false) ...
      ), ...
      struct( ...
        'name', 'param_d3', ...
        'full_name', 'QDLIB.Command.param_d3', ...
        'index', 8, ...
        'number', uint32(8), ...
        'type', uint32(1), ...
        'matlab_type', uint32(5), ...
        'wire_type', uint32(1), ...
        'label', uint32(1), ...
        'default_value', double(0), ...
        'read_function', @(x) typecast(x, 'double'), ...
        'write_function', @(x) typecast(double(x), 'uint8'), ...
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
  put(descriptor.field_indeces_by_number, uint32(8), 8);
  
