function qd_cmd_xml(xml)
%
% qd_cmd_xml(xml)
%
% Send a XML programm definition to a QDng in interactive mode
% and run the programm
% 

% Assemble message
cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 10);
cmd = pblib_set(cmd, 'XML', xml);

qd_write_cmd(cmd);
