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

[payload, header] = qd_read_stream(fd);

switch(header.class)
    case 'WFGridCartesian'
        [data, meta] = qd_decode_WFGridCartesian(payload);
    case 'WFLevel'
        [data, meta] = qd_decode_WFLevel(payload);
    otherwise
        if (nargin == 2)
            [data, meta] = decode_fcn(payload);
        else
            error(['Unknown class: ' header.class '\nprovide a decode fuction!']);
        end
end


