function qd_write_cmd(cmd, payload)
%
% Send a valid command to QDng
% 
fd = qd_get_socket('rx');

msg_wire = pblib_generic_serialize_to_string(cmd);
fwrite(fd, uint32(numel(msg_wire)), 'uint32');
fwrite(fd, msg_wire, 'uint8');

if nargin > 1
   fwrite(fd, payload, 'uint8')

end

fclose(fd);

end