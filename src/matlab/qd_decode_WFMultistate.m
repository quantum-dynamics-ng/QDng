% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function [data, meta] = qd_decode_WFMultistate(fd, pl, decode_fcn)
%
% decode a serialization of WFMultistate and the
% encapsulated WFs. An optional decode function
% can be given to decode the encapsulated WFs.
%

states = typecast(pl, 'uint32');

if states <= 0 && states > 128 % this is arbitrary !?
    error('Invalid number of states');
end


more_files_follow = true;
state = 1;
data = cell(states,1);
meta = cell(states,1);

while more_files_follow ~= 0
    % Parse next header in stream
    msg_len = fread(fd, 1, 'uint32');
    buffer  = fread(fd, msg_len, '*uint8')';
    header = pb_read_QDLIB__FileSingleHeader(buffer);

    more_files_follow = header.more_files_follow;
    payload = fread(fd, header.payload_size, '*uint8');
    if nargin == 3
        [data{state}, meta{state}] = qd_decode_WF(fd, payload, header.class, decode_fcn);
    else
        [data{state}, meta{state}] = qd_decode_WF(fd, payload, header);
    end
    
    state = state + 1;
end

end