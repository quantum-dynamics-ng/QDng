% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function pl = qd_encode_WFGridCartesian(data, meta)
%
% Serialize a qd_encode_WFGridCartesian
%
% The meta data can be provied in two diffrent forms:
% - correct protobuf header as returtend from the read function
% - a raw struc twith the neccessary information:
%   meta.xmin => [1 dim] array
%   meta.xmax => [1 dim] array
%
% data provides the (complex) WF data as a n-d array

% handle meta data
cdata = qd_convert_complex_to_uint8(data);
meta_pb = qd_convert_grid_meta_to_pb(data, meta);
meta_pb = pblib_set (meta_pb, 'data_size', numel(cdata));

% generate header
meta_buf = pblib_generic_serialize_to_string(meta_pb);
pl = [typecast(uint32(numel(meta_buf)), 'uint8') meta_buf];

% compression
if meta.compress ~= 0
    comp = 1;
    if meta.compress == 1
        comp = 1e-6;
    else
        comp = meta.compress;
    end

    data = fftn(data);
    data(abs(data) < comp) = 0;
    cdata = qd_convert_complex_to_uint8(data);
end

pl = [pl cdata(:)'];

if meta.compress ~= 0
    pl = qlib_zlib_compress(pl);
end

end
