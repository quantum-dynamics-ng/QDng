% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function expec = qd_cmd_expec(data, meta, class, op, encode_fcn)
%
% expec = qd_cmd_expec(data, meta, class, op) 
% expec = qd_cmd_expec(data, meta, class, op, encode_fcn)
%
% Calculate an expecation value
% The rules for data, meta, class, and encode_fcn are the
% same as for qd_write_wf. The parameter op defines an operator
% by either giving an XML operator defintion or the just a reference key
% 

% create command
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 40);

% Autodetect if op is given as reference or a full XML defintion
if strncmp(op, '<', 1)
    cmd = pblib_set(cmd, 'XML', op);
else
    cmd = pblib_set(cmd, 'param_s1', op);
end

% Send WF
if (nargin > 4)
    stream = qd_wf_to_stream(data, meta, class, encode_fcn);
else
    stream = qd_wf_to_stream(data, meta, class);
end

resp = qd_write_cmd(cmd, stream);
expec = resp.result;

