function qd_write_wf(name, data, meta, class, compression, encode_fcn)
%
% Write a wave function
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

%
% handle response
%

% read from fifo
fd = qd_get_socket('tx');
msg_len = fread(fd, 1, '*uint32');
buffer = fread(fd, msg_len, '*uint8');
fclose(fd);

% check answer
resp = pb_read_QDLIB__Response(buffer);

switch (resp.response)
    case 1
        error(resp.msg);
    case 2
        error('An undefined error occured while writing the WF');
end


