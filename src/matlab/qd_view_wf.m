function [fh, data, meta] = qd_view_wf(path, opts)
%
% qd_view_wf(path)
% qd_view_wf(path, opts)
%
% plot a wave function 
%
% opts can contain the following fields;
% .basename   The wave function base name
% .index      index of the wf
% .decode_fcn a customized decode_fcn for qd_read_wf
% .fh         a figure handle. If not given a new figure is created.
% .dims       The dimensions to be shown. Is relevant for ndims >= 2


if exist('opts','var')
    if ~isa(opts, 'struct')
        error('opts must be a struct');
    end

    % create filename
    if isfield(opts, 'basename')
        fname = [path '/' opts.basename '_' num2str(opts.index)];
    else
        fname = path;
    end

    % read data
    if isfield(opts, 'decode_fcn')
        [data, meta] = qd_read_wf(fname, opts.decode_fcn);
    else
        [data, meta] = qd_read_wf(fname);
    end

    % pepare plot
    if isfield(opts, 'fh')
        fh = figure(opts.fh);
    else
        fh = figure();
    end
else
    fh = figure();
    [data, meta] = qd_read_wf(path);
    opts = struct();
end

% check for multistate WF
if iscell(data)
    X = prepare_axis(meta{1});
    ah = axes;
    set(ah, 'ColorOrder', [0 0 0; 0 0 1; 0 1 0; 1 0 0]);
    set(gca, 'LineStyleOrder', {'-', '--', ':'});
    hold all
    for i=1:length(data)
        y = prepare_data(data{i}, opts);
        plot_wf(X, y, meta{i}.dims);
    end
    hold off
else
    X = prepare_axis(meta);
    y = prepare_data(data, opts);
    plot_wf(X, y, meta.dims);
end


end

    % prepare data
    function y = prepare_data(data, opts)
        if isfield(opts, 'dims')
            y = qd_int(data, opts.dims);
        else
            y = data;
        end
    end


    function X = prepare_axis(meta)
        switch (meta.dims)
            case 1
                X = linspace(meta.dim.xmin, meta.dim.xmax, meta.dim.size);
            case 2
                x = linspace(meta.dim(1).xmin, meta.dim(1).xmax, meta.dim(1).size);
                y = linspace(meta.dim(2).xmin, meta.dim(2).xmax, meta.dim(2).size);
                [X{1}, X{2}] = ndgrid(x,y);
            otherwise
                error('Multi-dim view not implemented yet. Choose dimensions with opts.dims');
        end
    end

    function plot_wf(x, y, d)
        switch (d)
            case 1
                plot(x, abs(y));
            case 2
                surf(x{1}, x{2}, abs(y));
            otherwise
                error('Multi-dim view not implemented yet. Choose dimensions with opts.dims');
        end
    end
        

