function qd_write_op(name, data, meta, class, compression, encode_fcn)
%
%
% qd_write_op(name, data, meta, class)
% qd_write_op(name, data, meta, class, compression)
% qd_write_op(name, data, meta, class, compression, encode_fcn)
%
% Write an operator to a file
%
%

% Encode the op data and meta data.
if strncmp('Grid', class, 4)
    payload = qd_encode_OGridSystem(data, meta);
elseif strcmp ('Laser', class)
    payload = qd_encode_Laser(data, meta);
else
        if (nargin == 6)
            [payload] = encode_fcn(data, meta, class);
        else
            error(['Unknown class: ' class  '\nprovide a decode fuction!']);
        end   
end

% Create header
header = pblib_create_message(@pb_descriptor_QDLIB__FileSingleHeader);
header = pblib_set (header, 'class', class);
header = pblib_set (header, 'meta_data', '');
header = pblib_set (header, 'compression', 0);
header = pblib_set (header, 'more_files_follow', 0);
header = pblib_set (header, 'payload_size', numel(payload));

stream = qd_get_stream_intro();

header_buf = pblib_generic_serialize_to_string(header);
stream = [stream typecast(uint32(numel(header_buf)), 'uint8') header_buf payload];

% Send command
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 31); 
cmd = pblib_set(cmd, 'param_s1', name);
cmd = pblib_set(cmd, 'param_s2', class);

if nargin < 5
    compression = 0;
end

if compression > 0
    cmd = pblib_set(cmd, 'param3', 'gzip');
end

qd_write_cmd(cmd, stream);

end
