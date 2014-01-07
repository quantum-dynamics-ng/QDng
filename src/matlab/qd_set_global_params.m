function pm = qd_set_global_params(keyval)
%
% pm = get_global_params()
%
% Get the global parameter set
%

cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 21);
cmd = pblib_set(cmd, 'param1', keyval);
qd_write_cmd(cmd);

fd = qd_get_socket('tx');
msg_len = fread(fd, 1, 'uint32');

buf = fread(fd, msg_len, '*uint8')';
fclose(fd);

resp = pb_read_QDLIB__Response(buf);
pm = resp.msg;

end