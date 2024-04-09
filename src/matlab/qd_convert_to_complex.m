% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function data = qd_convert_to_complex(buffer)
%
% Convert a raw byte sequence into double, complex format
%

    raw = typecast(buffer, 'double');
    data = complex(raw(1:2:end-1), raw(2:2:end));
end