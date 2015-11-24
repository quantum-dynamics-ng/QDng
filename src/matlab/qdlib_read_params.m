function params = qdlib_read_params(fname)
%
% params = qdlib_read_params(fname)
%
% Read a param file and return the key/value pairs
%

params = struct();

fd = fopen(fname,'r');

if fd < 1
    error(['Can''t open param file: ' fname]);
end

line = fgetl(fd);

while ischar(line)
    kv = regexp(line, '\s+=\s+', 'split')
    params.(kv{1}) = kv{2};
    line = fgetl(fd);
end

fclose(fd);
end