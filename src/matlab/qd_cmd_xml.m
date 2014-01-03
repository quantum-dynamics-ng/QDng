function qd_cmd_xml(xml)
%
% Send a quit to a QDng in interactive mode
% 

% Assemble message
cmd = pb_read_QDLIB__Command([]);
cmd = pblib_set(cmd, 'cmd', 1);
cmd = pblib_set(cmd, 'XML', xml);

qd_write_cmd(cmd);