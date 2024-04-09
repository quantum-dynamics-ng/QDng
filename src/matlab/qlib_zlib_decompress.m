% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function decomp_out = qlib_zlib_decompress(data)
%
% decomp_out = qlib_zlib_decompress(data)
%
% Compress binary data with zlib algorithm
%
% Input must be uint8

if ~isa(data, 'uint8')
    error('qlib_zlib_decompress: Input must be uint8');
end

buf = java.io.ByteArrayOutputStream();
zlib = java.util.zip.InflaterOutputStream(buf);
zlib.write(data, 0, numel(data));
zlib.close();
decomp_out = typecast(buf.toByteArray(), 'uint8')';

end