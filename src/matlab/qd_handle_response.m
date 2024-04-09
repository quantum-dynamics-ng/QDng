% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function resp = qd_handle_response(fd)
%
% qd_handle_response()
% 
% Process a response message from QDng and display possible errors
%

% read from fifo
msg_len = fread(fd, 1, '*uint32');
buffer = fread(fd, msg_len, '*uint8');

% check answer
resp = pb_read_QDLIB__Response(buffer);

switch (resp.response)
    case 1
        fclose(fd);
        error(resp.msg);
    case 2
        fclose(fd);
        error('An undefined error occured while writing the WF');
end