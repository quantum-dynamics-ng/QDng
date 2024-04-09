% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function pl = qd_encode_WFLevel(data, comp)
%
% Serialize WFLevel
%


% Compression
if comp ~= 0
    if comp == 1
        comp = 1e-6;
    end
    data(abs(data) < comp) = 0;
end

% Add header and convert to bytestream
pl = [typecast(uint64(numel(data)), 'uint8') typecast(uint64(numel(data)*16), 'uint8')];
pl = [pl qd_convert_complex_to_uint8(data)];

if comp ~= 0
    pl = qlib_zlib_compress(pl);
end

end