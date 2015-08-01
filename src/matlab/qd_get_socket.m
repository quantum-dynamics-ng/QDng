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

if exist('getpid') == 5  
    pid = getpid(); % octave
else
    pid = feature('getpid'); % MATLAB
end

basename = ['/tmp/cmd_fifo_' num2str(pid)];

% check if we already have a running qdng
persistent qdng_pid;

if exist('qdng_pid') == 1
    [res,out] = system(['ps ' num2str(qdng_pid)]);
    if res == 0
       qdng_pid = start_qdng(basename)
    end
else 
    qdng_pid = start_qdng(basname);
end

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


function pid = start_qdng(basename)
    [res, sout] = system(['qdng -c ' basename ' &']);
    if res ~= 0
       error('QDng startup failed');
    end
    pid = textscan(sout, '%d');
    pid = pid{1}(1);
end

end
