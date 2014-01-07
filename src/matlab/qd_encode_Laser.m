function pl = qd_encode_Laser(data, dt)
%
% qd_encode_Laser(data, meta)
%
% Serialize a laser field
%

size = typecast(uint64(numel(data)),'uint8');
dt = typecast(dt,'uint8');
data = qd_convert_double_to_uint8(data);
pl = [size dt data];

