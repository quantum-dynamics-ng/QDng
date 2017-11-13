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
% .split_view Show Multistate WF in separate graphs


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
    
    if ~isfield(opts, 'split_view')
        opts.split_view = true;
    end
else
    fh = figure();
    [data, meta] = qd_read_wf(path);
    opts = struct('split_view',true);
end

% check for multistate WF
if iscell(data)
    X = prepare_axis(meta{1});
    if ~opts.split_view
        ah = axes;
        set(ah, 'ColorOrder', [0 0 0; 0 0 1; 0 1 0; 1 0 0]);
        set(gca, 'LineStyleOrder', {'-', '--', ':'});
        hold all
        for i=1:length(data)
            y = prepare_data(data{i}, opts);
            plot_wf(X, y, meta{i});
        end
        hold off
    else
        for i=1:length(data)
            subplot(length(data),1,length(data)-i+1);
             y = prepare_data(data{i}, opts);
             plot_wf(X, y, meta{i});
             title(['state ' num2str(i)'])
        end
             
    end
else
    if isfield(opts, 'dims')
        dims = length(opts.dims);
    else
        dims = meta.dims;
    end
    
    X = prepare_axis(meta, opts);
    y = prepare_data(data, meta, opts);
    plot_wf(X, y, meta, opts);
end


end

    % prepare data
    function y = prepare_data(data, meta, opts)
        if nargin > 2 && isfield(opts, 'dims')
            y = qd_int(data, setdiff(double(1:meta.dims),opts.dims));
        else
            y = data;
        end
    end


    function X = prepare_axis(meta, opts)
        if nargin > 1 && isfield(opts, 'dims')
            dims = opts.dims;
        else
            dims = 1:meta.dims;
        end
        
        switch (length(dims))
            case 1
                X = linspace(meta.dim(dims).xmin, meta.dim(dims).xmax, meta.dim(dims).size);
            case 2
                x = linspace(meta.dim(dims(1)).xmin, meta.dim(dims(1)).xmax, meta.dim(dims(1)).size);
                y = linspace(meta.dim(dims(2)).xmin, meta.dim(dims(2)).xmax, meta.dim(dims(2)).size);
                [X{1}, X{2}] = ndgrid(x,y);
            otherwise
                error('Multi-dim view not implemented yet. Choose dimensions with opts.dims');             
        end
    end

    function plot_wf(x, y, meta, opts)
        if nargin > 3 && isfield(opts, 'dims')
            dims = opts.dims;
        else
            dims = 1:meta.dims;
        end    
        switch (length(dims))
            case 1
                plot(x, abs(y));
            case 2
                surf(x{1}, x{2}, abs(y), 'LineStyle', 'None'); view([0 90])
                colorbar();
                xlim([meta.dim(dims(1)).xmin meta.dim(dims(1)).xmax]);
                ylim([meta.dim(dims(2)).xmin meta.dim(dims(2)).xmax]);
            otherwise
                error('Multi-dim view not implemented yet. Choose dimensions with opts.dims');
        end
    end
        

