% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function op_wf = qd_cmd_apply_op(data, meta, class, op, encode_fcn, decode_fcn)
%
% expec = qd_cmd_apply_op(data, meta, class, op) 
% expec = qd_cmd_apply_op(data, meta, class, op, encode_fcn, decode_fcn)
%
% Applies an operator to a wave function.
% The rules for data, meta, class, encode_fcn, and decode_fcn are the
% same as for qd_write_wf (qd_read_wf). The parameter op defines an operator
% by either giving an XML operator defintion or the just a reference key
% 

% create command
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 42);

% Autodetect if op is given as reference or a full XML defintion
if strncmp(op, '<', 1)
    cmd = pblib_set(cmd, 'XML', op);
else
    cmd = pblib_set(cmd, 'param1', op);
end

qd_write_cmd(cmd);
qd_handle_response();

% Send WF
if (nargin > 4)
    stream = qd_wf_to_stream(data, meta, class, encode_fcn);
else
    stream = qd_wf_to_stream(data, meta, class);
end

fd = qd_get_socket('rx');
fwrite(fd, stream, 'uint8');
fclose(fd);

% handle result
fd_tx = qd_get_socket('tx');
[payload, header] = qd_read_stream(fd_tx);

switch(header.class)
    case 'WFGridCartesian'
        [op_wf] = qd_decode_WFGridCartesian(payload);
    case 'WFLevel'
        [op_wf] = qd_decode_WFLevel(payload);
    otherwise
        if (nargin == 2)
            [op_wf] = decode_fcn(payload);
        else
            error(['Unknown class: ' header.class '\nprovide a decode fuction!']);
        end
end


end
