function pl = qd_encode_WFGridCartesian(data, meta)
%
% Serialize a qd_encode_WFGridCartesian
%
% The meta data can be provied in two diffrent forms:
% - correct protobuf header as returtend from the read function
% - a raw struc twith the neccessary information:
%   meta.xmin => [1 dim] array
%   meta.xmax => [1 dim] array
%
% data provides the (complex) WF data as a n-d array

if ~isstruct(meta)
    error('meta data must be a struct either in protobuf or raw form');
end

%
data = qd_convert_complex_to_uint8(data);

% prepare meta data
if isfield(meta,'xmin') && isfield(meta,'xmax')
    meta_pb = pb_read_QDLIB__GridSystemHeader([]);
    meta_pb = pblib_set (meta_pb, 'dims', numel(size(data)));
else
    meta_pb = meta;
end

% generate header and pack data
meta_pb = pblib_set (meta_pb, 'data_size', numel(data));
meta_buf = pblib_generic_serialize_to_string(meta_pb);

pl = [typecast(uint32(numel(meta_buf)), 'uint8') meta_buf data];





