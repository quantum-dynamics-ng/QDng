% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function pl = qd_encode_Laser(data, dt)
%
% qd_encode_Laser(data, meta)
%
% Serialize a laser field
%
% note: compression is ignored

size = typecast(uint64(numel(data)),'uint8');
dt = typecast(dt,'uint8');
data = qd_convert_double_to_uint8(data(:));
pl = [size dt data'];

