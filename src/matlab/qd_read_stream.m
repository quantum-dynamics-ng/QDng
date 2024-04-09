% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function [payload, header] = qd_read_stream(fd, header_only)
%
% [payload, header] = qd_read_stream(fd)
% [header] = qd_read_stream(fd, header_only)
% 
% Read header and payload from a stream
%

% check for error
buffer = fread(fd, 4, '*uint8');

if strcmp(char(reshape(buffer,1,4)),'QDng') == false
    msg_len = typecast(buffer,'uint32');
    buffer  = fread(fd, msg_len, 'uint8');
    resp = pb_read_QDLIB__Response(buffer);
    
    if resp.response == 1
        error('qdng:qd_read_stream', resp.msg);
    else
        error('qdng:qd_read_stream', 'Unknown error');
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
buffer  = fread(fd, msg_len, '*uint8')';
header = pb_read_QDLIB__FileSingleHeader(buffer);

if nargin > 1 && header_only == 0
    return
end

payload = fread(fd, header.payload_size, '*uint8');
