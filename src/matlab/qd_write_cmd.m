function qd_write_cmd(cmd)
%
% Send a valid command to QDng
% 
fd = qd_get_socket('rx');

msg_wire = pblib_generic_serialize_to_string(cmd);
fwrite(fd, uint32(numel(msg_wire)), 'uint32');
fwrite(fd, msg_wire, 'uint8');

fclose(fd);
end