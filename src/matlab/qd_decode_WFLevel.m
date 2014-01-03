function [data, meta] = qd_decode_WFLevel(pl)
%
% decode a serialization of WFLevel
%
    meta = typecast(pl(1:8), 'uint64');
    sizeBytes = typecast(pl(9:16), 'uint64');
    
    if (meta*16 ~= sizeBytes) || (sizeBytes ~= numel(pl)-16)
        error('Inconsistet header');
    end
    
    data = qd_convert_to_complex(pl(17:end));
end