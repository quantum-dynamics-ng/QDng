function buffer = qd_convert_complex_to_uint8(data)

    buffer = zeros(1,2*numel(data));
    
    buffer(1:2:end-1) = real(data(:));
    buffer(2:2:end) = imag(data(:));
    buffer = typecast(buffer,  'uint8');
end
