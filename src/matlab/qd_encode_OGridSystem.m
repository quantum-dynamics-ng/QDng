% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function pl = qd_encode_OGridSystem(data, meta)
%
% Serialize a qd_encode_WFGridCartesian
%
% The meta data can be provied in two diffrent forms:
% - correct protobuf header as returtend from the read function
% - a raw struc twith the neccessary information:
%   meta.xmin => [1 dim] array
%   meta.xmax => [1 dim] array
%
% data provides the Op data as a n-d array

meta_pb = qd_convert_grid_meta_to_pb(data, meta);
data = qd_convert_double_to_uint8(data);

% generate header and pack data
meta_pb = pblib_set (meta_pb, 'data_size', numel(data));
meta_buf = pblib_generic_serialize_to_string(meta_pb);

pl = [typecast(uint32(numel(meta_buf)), 'uint8') meta_buf data'];

end
