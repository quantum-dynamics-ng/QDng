function expec = qd_cmd_matel(bra, ket, meta, class, op, encode_fcn)
%
% expec = qd_cmd_matel(bra, ket, meta, class, op) 
% expec = qd_cmd_matel(bra, ket, meta, class, op, encode_fcn)
%
% Calculate a matrix element value
% The rules for bra, ket, meta, class, and encode_fcn are the
% same as for qd_write_wf. The parameter op defines an operator
% by either giving an XML operator defintion or the just a reference key
% 

% create command
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 41);

% Autodetect if op is given as reference or a full XML defintion
if strncmp(op, '<', 1)
    cmd = pblib_set(cmd, 'XML', op);
else
    cmd = pblib_set(cmd, 'param1', op);
end

qd_write_cmd(cmd);
qd_handle_response();

% Send WFs
if (nargin > 5)
    stream = qd_wf_to_stream(bra, meta, class, encode_fcn);
    stream = [stream qd_wf_to_stream(ket, meta, class, encode_fcn)];
else
    stream = qd_wf_to_stream(bra, meta, class);
    stream = [stream qd_wf_to_stream(ket, meta, class)];
end

fd = qd_get_socket('rx');
fwrite(fd, stream, 'uint8');
fclose(fd);

% get result
resp = qd_handle_response();
expec = complex(resp.result(1), resp.result(2));
