% SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
%
% SPDX-License-Identifier: gpl-3-only

function qd_write_wf(name, data, meta, encode_fcn)
%
% Write a wave function
%
% qd_write_wf(name, data, meta)
% qd_write_wf(name, data, meta, encode_fcn)
%
%

if (nargin > 3)
    stream = qd_wf_to_stream(data, meta, encode_fcn);
else
    stream = qd_wf_to_stream(data, meta);
end


% Write direct to file
fd = fopen([name '.wf'],'w');
fwrite(fd, stream);
fclose(fd);

