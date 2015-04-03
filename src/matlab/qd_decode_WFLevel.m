function [data, meta] = qd_decode_WFLevel(pl, comp)
%
% decode a serialization of WFLevel
%

% decompress
if comp == 1
    pl = qlib_zlib_decompress(pl);
end

meta.size = typecast(pl(1:8), 'uint64');
sizeBytes = typecast(pl(9:16), 'uint64');

if (meta.size*16 ~= sizeBytes) || (sizeBytes ~= numel(pl)-16)
    error('Inconsistet header');
end
    
data = qd_convert_to_complex(pl(17:end));
data = data(:);

meta.class = 'WFLevel';

end