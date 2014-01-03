function data = qd_convert_to_complex(buffer)
%
% Convert a raw byte sequence into double, complex format
%

    raw = typecast(buffer, 'double');
    data = complex(raw(1:2:end-1), raw(2:2:end));
end