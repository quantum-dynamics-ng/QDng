% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function [data, meta] = qd_decode_OGridSystem(pl)
%
% [data, meta] = qd_decode_OGridSystem(pl)
%
% De-serialize a OGridSystem type operator
%

msg_len = typecast(pl(1:4), 'uint32');
meta = pb_read_QDLIB__GridSystemHeader(pl( (1:msg_len)+4 ));

offs = 4 + msg_len + 1;
    
data = pl( (offs:end));
data = typecast(data, 'double');

if (meta.dims > 1)
    data = reshape(data, [ meta.dim(:).size] );
end

end