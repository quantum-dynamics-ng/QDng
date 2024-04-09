% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function [data, meta] = qd_decode_WFGridCartesian(pl, compressed)
%
% decode a serialization of WFGridCartesian
%


if compressed == 1
    pl = qlib_zlib_decompress(pl);
end

msg_len = typecast(pl(1:4), 'uint32');
if msg_len > numel(pl) - 4
    error('Faulty header length');
end
    
meta = pb_read_QDLIB__GridSystemHeader(pl( (1:msg_len)+4 ));

offs = 4 + msg_len + 1;
    
data = pl( (offs:end));

if meta.data_size ~= numel(data)
    error('qd_decode_WFGridCartesian: Mismatch in data size\n')
end
data = qd_convert_to_complex(data);

if (meta.dims > 1)
    data = reshape(data, [ meta.dim(:).size] );
else
    data = data(:);
end

if compressed == 1
    data = ifftn(data);
end

meta.class = 'WFGridCartesian';

end