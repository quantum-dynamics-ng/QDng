function [fd] = qd_get_socket(rxtx)
%
% qd_get_socket()
% qd_get_socket(rxtx)
%
% Get a pipe file descriptor to a running QDng
% Without any arguments a send file descriptor is returned
% 

if nargin == 1
    if ~strcmp(rxtx, 'rx') && ~strcmp(rxtx, 'tx')
        error('rxtx must be either rx or tx');
    end
elseif nargin == 0
    rxtx = 'rx';
else
end

if  strcmp(rxtx, 'rx')
    fd = fopen('/tmp/cmd_fiforx', 'a');
else
    fd = fopen('/tmp/cmd_fifotx', 'r');
end


end