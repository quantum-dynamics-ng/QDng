function [Ctt, Nt, dt] = qd_read_Ctt(fname)
%
% [Ctt, Nt, dt] = qd_read_Ctt(fname)
%   
% Read a correlation function
% 

fh = fopen([fname '.cf']);
if (fh < 0)
    error(['Can not open ' fname]);
end

data = fread(fh, 'double');
fclose(fh);

data = qd_convert_to_complex(data);

pm = qdlib_read_params([fname '.meta']);
Nt = str2double(pm.Nt);
dt = str2double(pm.dt);
pm.dim = str2double(pm.dim);

if pm.dim == 2
    data = reshape(data, Nt, Nt);
end

%data = conj(data+tril(data,-1)');
Ctt = data;
    
end