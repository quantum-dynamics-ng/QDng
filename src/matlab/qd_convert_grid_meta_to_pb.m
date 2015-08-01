function meta_pb = qd_convert_grid_meta_to_pb(data, meta)
%
% meta_pb = qd_convert_grid_meta_to_pb(meta)
%
% The meta data can be provied in two diffrent forms:
% - correct protobuf header as returtend from the read function
%   (QDLIB_GridSystemHeader)
% - a raw struc twith the neccessary information:
%   meta.xmin => [1 dim] array
%   meta.xmax => [1 dim] array
%   meta.size => [1 dim] array

if ~isstruct(meta)
    error('meta data must be a struct either in protobuf or raw form');
end

% prepare meta data
if isfield(meta,'xmin') && isfield(meta,'xmax')
    
    meta_pb = pblib_create_message(@pb_descriptor_QDLIB__GridSystemHeader);
    dims = length(meta.xmin);
    meta_pb = pblib_set (meta_pb, 'dims', dims);
    
    meta_pb.dim = pblib_create_message(@pb_descriptor_QDLIB__GridSystemHeader__dim_description_t);
    put(meta_pb.has_field, 'dim', 1);
     
    for k=1:dims
        meta_dim = pblib_create_message(@pb_descriptor_QDLIB__GridSystemHeader__dim_description_t);
        if dims ==1 
            meta_dim = pblib_set(meta_dim, 'size', length(data));
        else
            meta_dim = pblib_set(meta_dim, 'size', size(data,k));
        end
        meta_dim = pblib_set(meta_dim, 'xmin', meta.xmin(k));
        meta_dim = pblib_set(meta_dim, 'xmax', meta.xmax(k));
        meta_pb.dim(k) = meta_dim;
    end
else
    meta_pb = meta;
end