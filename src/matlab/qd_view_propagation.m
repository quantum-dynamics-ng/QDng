function qd_view_propagation(path, opts)
%
% qd_view_propagation(path)
% qd_view_propagation(path, opts)
%
% plot a wave function propagation time series
%
% opts can contain the following fields;
% .basename   The wave function base name
% .decode_fcn a customized decode_fcn for qd_read_wf
% .fh         a figure handle. If not given a new figure is created.
% .dims       The dimensions to be shown. Is relevant for ndims >= 2
% .pause      The break time inbetween two WFs

if ~exist('opts', 'var')
    opts.basename = 'WF';
    opts.pause = 0.1;
    opts.fh = figure();
elseif ~isstruct(opts)
    error('opts must be given as struct');
end

% open figure
if ~isfield(opts, 'fh')
    opts.fh = figure();
end

if ~isfield(opts, 'pause')
    opts.pause = 0.1;
end

if ~isfield(opts, 'basename')
    opts.basename = 'WF';
end

pmeta = qdlib_read_params([path '/Propagation.meta']);

pmeta.Nt = str2double(pmeta.Nt); 
pmeta.dt = str2double(pmeta.dt);
pmeta.Wcycle = str2double(pmeta.Wcycle); 

for i=0:ceil(pmeta.Nt/pmeta.Wcycle)
    opts.index = i;
    qd_view_wf(path, opts);
    fprintf('t = %.2f\n', pmeta.Wcycle*pmeta.dt*i);
    if opts.pause > 0
        pause(opts.pause);
    else
        drawnow();
        input('Press key');
    end
end

end
