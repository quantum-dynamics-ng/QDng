function qd_cmd_quit()
%
% Send a quit to a QDng in interactive mode
% 

% Assemble message
cmd = pblib_create_message(pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 0);

qd_write_cmd(cmd);

end