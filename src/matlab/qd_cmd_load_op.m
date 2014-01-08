function qd_cmd_load_op(xml)
%
% qd_cmd_load_op(xml)
%
% Load an operator in QDng for later usage by reference
% Note that the operator needs to define a access key, otherwise
% it is lost.

cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 6);
cmd = pblib_set(cmd, 'XML', xml);

qd_write_cmd(cmd);

qd_handle_response();

end