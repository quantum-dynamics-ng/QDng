% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function pm = qd_get_global_params()
%
% pm = get_global_params()
%
% Get the global parameter set
%

cmd = pblib_create_message(@pb_descriptor_QDLIB__Command);
cmd = pblib_set(cmd, 'cmd', 1);
qd_write_cmd(cmd);

fd = qd_get_socket('tx');
msg_len = fread(fd, 1, 'uint32');

buf = fread(fd, msg_len, '*uint8')';
fclose(fd);

resp = pb_read_QDLIB__Response(buf);
pm = resp.msg;

end
