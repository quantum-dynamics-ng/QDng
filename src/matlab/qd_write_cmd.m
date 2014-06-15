function resp = qd_write_cmd(cmd, payload)
%
% resp = qd_write_cmd(cmd)
% resp = qd_write_cmd(cmd, payload)
%
% Send a valid command to QDng.
% cmd must be a valid protobuf struct of QDLIB__Command
% Data payload can be sent optional if the command requires it.
%
% 
fd_out = qd_get_socket('rx');
fd_in = qd_get_socket('tx');

% send command
msg_wire = pblib_generic_serialize_to_string(cmd);
fwrite(fd_out, uint32(numel(msg_wire)), 'uint32');
fwrite(fd_out, msg_wire, 'uint8');

resp = qd_handle_response(fd_in);

% send data
if nargin > 1
   fwrite(fd_out, payload, 'uint8');
   resp = qd_handle_response(fd_in);
end

fclose(fd_out);
fclose(fd_in);

end