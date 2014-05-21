function qd_write_wf(name, data, meta, class, compression, encode_fcn)
%
% Write a wave function
%
% qd_write_wf(name, data, meta, class)
% qd_write_wf(name, data, meta, class, compression)
% qd_write_wf(name, data, meta, class, compression, encode_fcn)
%
%

if (nargin > 5)
    stream = qd_wf_to_stream(data, meta, class, encode_fcn);
else
    stream = qd_wf_to_stream(data, meta, class);
end

if (nargin < 5)
    compression = 0;
end

% Write direct to file if compression is not needed
% if compression == 3
%     fd = fopen(name,'w');
%     fwrite(fd, stream);
%     fclose(fd);
%     return
% end

% Send command
cmd = pb_read_QDLIB__Command();
cmd = pblib_set(cmd, 'cmd', 3); 
cmd = pblib_set(cmd, 'param1', name);
if compression > 0
    cmd = pblib_set(cmd, 'param2', 'gzip');
end

qd_write_cmd(cmd, stream);

qd_handle_response();
