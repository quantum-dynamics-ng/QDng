function [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% [data, meta, header] = qd_read_wf(fname)
% [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% Read a wave function through a running QDng 
% 



% request the WF
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 20);
cmd = pblib_set(cmd, 'param_s1', fname);

resp = qd_write_cmd(cmd);

if ~resp.data_follows
    error('No data!');
end

%
% handle the data
%
fd = qd_get_socket('tx');
[payload, header] = qd_read_stream(fd);

if nargin == 2
    [data, meta] = qd_decode_WF(fd, payload, header.class, decode_fcn);
else
    [data, meta] = qd_decode_WF(fd, payload, header.class);
end

