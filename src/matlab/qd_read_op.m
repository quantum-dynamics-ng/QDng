function [data, meta, header] = qd_read_op(fname, class ,decode_fcn)
%
% [data, meta, header] = qd_read_wf(fname, class)
% [data, meta, header] = qd_read_wf(fname, class ,decode_fcn)
%
% Read an operator through a running QDng 
% 

fd = qd_get_socket('tx');

% request the Operator
cmd = pblib_create_message(pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 4);
cmd = pblib_set(cmd, 'param1', fname);
cmd = pblib_set(cmd, 'param2', class);

qd_write_cmd(cmd);


%
% handle the answer
%

[payload, header] = qd_read_stream(fd);

if strncmp('OGrid', header.class, 5)
    [data, meta] = qd_decode_OGridSystem(payload, header.class);
elseif strcmp ('Laser', header.class)
    [data, meta] = qd_decode_Laser(payload, header.class);
else
        if (nargin == 3)
            [data, meta] = decode_fcn(payload,  header.class);
        else
            error(['Unknown class: ' header.class' '\nprovide a decode fuction!']);
        end   
end
    