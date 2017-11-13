function [data, meta, header] = qd_read_op(fname,decode_fcn)
%
% [data, meta, header] = qd_read_wf(fname, class)
% [data, meta, header] = qd_read_wf(fname, class ,decode_fcn)
%
% Read an operator through a running QDng 
% 

fd = fopen([fname '.op']);

if fd < 0
    error('qdng:qd_read_op:file_not_found', [fname '.op not found']);
end

%
% handle the answer
%

[payload, header] = qd_read_stream(fd);
fclose(fd);

if header.compression == 1
    payload = qlib_zlib_decompress(payload);
end

if nargin == 2
    [data, meta] = decode_fcn(payload,  header.class);
    meta.class = header.class(2:end);
    return;
end

if strncmp('OGrid', header.class, 5)
    [data, meta] = qd_decode_OGridSystem(payload);
    meta.class = header.class(2:end);
elseif strcmp ('Laser', header.class)
    [data, dt] = qd_decode_Laser(payload);
    meta.dt = dt;    
    meta.class = header.class;
else
    error('qdng:qd_read_op:unknown_class',['Unknown class: ' header.class' '\nprovide a decode fuction!']);   
end



