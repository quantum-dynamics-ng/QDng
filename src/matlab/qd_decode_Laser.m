function [data, dt] = qd_decode_Laser(pl)
%
% [data, dt] = qd_decode_Laser(payload)
%
% De-serialize a Laser

len = typecast(pl(1:8), 'uint64');
dt = typecast(pl(9:16), 'double');

if len ~= ((numel(pl) - 16)/8)
    error('Meta data missmatch in size');
end

if dt <= 0
    error(['Meta data missmatch in dt: ' num2str(dt)]);
end

data = typecast(pl(17:end), 'double');

end