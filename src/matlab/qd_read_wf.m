function [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% [data, meta, header] = qd_read_wf(fname)
% [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% Read a wave function through a running QDng 
% 

fd = qd_get_socket('tx');

% request the WF
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 2);
cmd = pblib_set(cmd, 'param1', fname);

qd_write_cmd(cmd);

%
% handle the answer
%

[payload, header] = qd_read_stream(fd);

if nargin == 2
    [data, meta] = qd_decode_WF(fd, payload, header.class, decode_fcn);
else
    [data, meta] = qd_decode_WF(fd, payload, header.class);
end

