function pl = qd_encode_OGridSystem(data, meta)
%
% Serialize a qd_encode_WFGridCartesian
%
% The meta data can be provied in two diffrent forms:
% - correct protobuf header as returtend from the read function
% - a raw struc twith the neccessary information:
%   meta.xmin => [1 dim] array
%   meta.xmax => [1 dim] array
%
% data provides the Op data as a n-d array

if ~isstruct(meta)
    error('meta data must be a struct either in protobuf or raw form');
end

% prepare meta data
if isfield(meta,'xmin') && isfield(meta,'xmax')
    meta_pb = pblib_create_message(@pb_descriptor_QDLIB__GridSystemHeader);
    dims = numel(size(data));
    meta_pb = pblib_set (meta_pb, 'dims', dims);
    
    meta_pb.dim = pblib_create_message(@pb_descriptor_QDLIB__GridSystemHeader__dim_description_t);
    put(meta_pb.has_field, 'dim', 1);
     
    for k=1:dims
        meta_dim = pblib_create_message(@pb_descriptor_QDLIB__GridSystemHeader__dim_description_t);
        meta_dim = pblib_set(meta_dim, 'size', size(data,k));
        meta_dim = pblib_set(meta_dim, 'xmin', meta.xmin(k));
        meta_dim = pblib_set(meta_dim, 'xmax', meta.xmax(k));
        meta_pb.dim(k) = meta_dim
    end
else
    meta_pb = meta;
end

data = qd_convert_double_to_uint8(data);

% generate header and pack data
meta_pb = pblib_set (meta_pb, 'data_size', numel(data));
meta_buf = pblib_generic_serialize_to_string(meta_pb);

pl = [typecast(uint32(numel(meta_buf)), 'uint8') meta_buf data'];

end