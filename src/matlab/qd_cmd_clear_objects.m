% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function qd_cmd_clear_objects()
%
% qd_cmd_clear_objects()
%
% Clear the object storage of a running programm
%

% Assemble message
cmd = pblib_create_message(pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 3);

qd_write_cmd(cmd);

end