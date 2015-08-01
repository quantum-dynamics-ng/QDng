function qd_write_op(name, data, meta, encode_fcn)
%
%
% qd_write_op(name, data, meta)
% qd_write_op(name, data, meta, encode_fcn)
%
% Write an operator to a file
%
%

% Encode the op data and meta data.
if nargin == 4
    [payload] = encode_fcn(data, meta);
else
    if strncmp('Grid', meta.class, 4)
        meta.class = ['O' meta.class];
        payload = qd_encode_OGridSystem(data, meta);
    elseif strcmp ('Laser', meta.class)
        payload = qd_encode_Laser(data, meta.dt);
    else
        error(['Unknown class: ' class  '\nprovide a decode fuction!']);   
    end
end

% Create header
header = pblib_create_message(@pb_descriptor_QDLIB__FileSingleHeader);
header = pblib_set (header, 'class', meta.class);
header = pblib_set (header, 'meta_data', '');
% handle compression flags
if  isfield(meta, 'compress') && meta.compress ~= 0
    header = pblib_set (header, 'compression', 1);
    payload = qlib_zlib_compress(payload);
else
    header = pblib_set (header, 'compression', 0);
end
header = pblib_set (header, 'more_files_follow', 0);
header = pblib_set (header, 'payload_size', numel(payload));

stream = qd_get_stream_intro();

header_buf = pblib_generic_serialize_to_string(header);
stream = [stream typecast(uint32(numel(header_buf)), 'uint8') header_buf payload];

% write to file
fd = fopen([name '.op'], 'w');
fwrite(fd, stream);
fclose(fd);

end
