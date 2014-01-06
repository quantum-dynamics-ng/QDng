function [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% [data, meta, header] = qd_read_wf(fname)
% [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% Read a wave function through a running QDng 
% 

fd = qd_get_socket('tx');

% request the WF
cmd = pb_read_QDLIB__Command([]);
cmd = pblib_set(cmd, 'cmd', 2);
cmd = pblib_set(cmd, 'param1', fname);

qd_write_cmd(cmd);

%
% handle the answer
%

% check for error
buffer = fread(fd, 4, '*uint8');

if strcmp(char(reshape(buffer,1,4)),'QDng') == false
    msg_len = typecast(buffer,'uint32');
    buffer  = fread(fd, msg_len, 'uint8');
    resp = pb_read_QDLIB__Response(buffer);
    
    if resp.response == 1
        error(resp.msg);
    else
        error('Unknown error');
    end
end

buffer = fread(fd, 8, 'char');
if strcmp(char(reshape(buffer(1:7),1,7)),'PrtoBuf')  == false
     error('Header error');
end

buffer = fread(fd, 1, 'uint32');

if buffer ~= 1
    error('Wrong protocol version');
end

% Parse file header
msg_len = fread(fd, 1, 'uint32');
buffer  = fread(fd, msg_len, '*uint8');
header = pb_read_QDLIB__FileSingleHeader(buffer);

payload = fread(fd, header.payload_size, '*uint8');
numel(payload)
switch(header.class')
    case 'WFGridCartesian'
        [data, meta] = qd_decode_WFGridCartesian(payload);
    case 'WFLevel'
        [data, meta] = qd_decode_WFLevel(payload);
    otherwise
        if (nargin == 2)
            [data, meta] = decode_fcn(payload);
        else
            error(['Unknown class: ' header.class' '\nprovide a decode fuction!']);
        end
end






