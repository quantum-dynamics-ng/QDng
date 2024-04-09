% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function comp_out = qlib_zlib_compress(data)
%
% comp_out = qlib_zlib_compress(data)
%
% Compress binary data with zlib algorithm
%
% Input must either be char or uint8 or it's typecasted to uint8

if ischar(data)
  data = uint8(data);
elseif ~isa(data,'uint8');
  data = typecast(data, 'uint8');
end

buf = java.io.ByteArrayOutputStream();
zlib = java.util.zip.DeflaterOutputStream(buf);
zlib.write(data, 0, numel(data));
zlib.close();
comp_out = typecast(buf.toByteArray(), 'uint8')';

end