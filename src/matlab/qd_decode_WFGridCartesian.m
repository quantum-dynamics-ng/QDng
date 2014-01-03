function [data, meta] = qd_decode_WFGridCartesian(pl)
%
% decode a serialization of WFGridCartesian
%

msg_len = typecast(pl(1:4), 'uint32')
meta = pb_read_QDLIB__GridSystemHeader(pl( (1:msg_len)+4 ));

offs = 4 + msg_len + 1;
    
data = pl( (offs:end))
data = qd_convert_to_complex(data);

if (meta.dims > 1)
    data = reshape(data, [ meta.dim(:).size] );
end


end