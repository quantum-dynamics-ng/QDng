function pl = qd_encode_WFLevel(data)
%
% Serialize WFLevel
%

pl = [typecast(uint64(numel(data)), 'uint8') typecast(uint64(numel(data)*16), 'uint8')];
pl = [pl qd_convert_complex_to_uint8(data)];

end