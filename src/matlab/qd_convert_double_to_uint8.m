% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function buffer = qd_convert_double_to_uint8(data)

    buffer = typecast(data(:),  'uint8');
end
