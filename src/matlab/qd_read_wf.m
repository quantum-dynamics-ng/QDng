function [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% [data, meta, header] = qd_read_wf(fname)
% [data, meta, header] = qd_read_wf(fname, decode_fcn)
%
% Read a wave function through
% 

fd = fopen([fname '.wf']);
[payload, header] = qd_read_stream(fd);

if nargin == 2
    [data, meta] = qd_decode_WF(fd, payload, header, decode_fcn);
else
    [data, meta] = qd_decode_WF(fd, payload, header);
end

fclose(fd);

end
