function resp = qd_handle_response()
%
% qd_handle_response()
% 
% Process a response message from QDng and display possible errors
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