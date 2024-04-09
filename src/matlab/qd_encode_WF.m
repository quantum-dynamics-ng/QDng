% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function payload = qd_encode_WF(data, meta, encode_fcn)
%
% qd_encode_WF(data, meta, class)
% qd_encode_WF(data, meta, class, encode_fcn)
%
% Encode a WF to stream format.

if ~isfield(meta, 'compress')
    meta.compress = 0;
end

% custom encode
if (nargin > 3)
    payload = encode_fcn(data, meta);
    return;
end

% Well known classes
switch (meta.class)
    case 'WFLevel'
        payload = qd_encode_WFLevel(data, meta.compress);
    case 'WFGridCartesian'
        payload = qd_encode_WFGridCartesian(data, meta);
    otherwise
        error('Unknown WF class. Please provide an enocde_fcn')
end

end

