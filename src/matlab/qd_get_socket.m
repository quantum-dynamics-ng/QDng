function [fd] = qd_get_socket(rxtx)
%
% qd_get_socket()
% qd_get_socket(rxtx)
%
% Get a pipe file descriptor to a running QDng
% Without any arguments a send file descriptor is returned
% 

% read or write
if nargin == 1
    if ~strcmp(rxtx, 'rx') && ~strcmp(rxtx, 'tx')
        error('rxtx must be either rx or tx');
    end
elseif nargin == 0
    rxtx = 'rx';
else
end

pid = feature('getpid'); % TODO: This is MATLAB specific

basename = ['/tmp/cmd_fifo_' num2str(pid) '_'];

% open socket 
if  strcmp(rxtx, 'rx')
    if ~ exist([basename 'rx'],'file')
        system(['qdng_cmdsrv ' num2str(pid) ' &']);
        pause(0.5)
        if ~ exist([basename 'rx'],'file')
            error(['fifo doesn''t exist: ' basename 'rx' '. Is qdng running?' ]);
        end
    end
    fd = fopen([basename 'rx'], 'a');
else
    if ~ exist([basename 'tx'],'file')
        system(['qdng_cmdsrv ' num2str(pid) ' &']);
        pause(0.5)
        if ~ exist([basename 'tx'],'file')
            error(['fifo doesn''t exist: ' basename 'tx' '. Is qdng running?' ]);
        end
    end    
    fd = fopen([basename 'tx'], 'r');
end


end