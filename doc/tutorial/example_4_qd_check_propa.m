%
% Check the results of the propagation
%

% conversion factors
a0 = 0.529;
ev = 27.21;
t2au = 41.34;

% The convenient look a the time series
qd_view_propagation('propa');


%% Let's see how the output looks like and extract some data

%
% get the meta data
[~, wfmeta] = qd_read_wf('propa/WF_0'); % get the meta data of the WF
Nx  = double(wfmeta{1}.dim.size);
xmin  = wfmeta{1}.dim.xmin;
xmax  = wfmeta{1}.dim.xmax;

% calculate the Delta x
dx = (xmax - xmin) / (Nx - 1);

pmeta = qdlib_read_params('propa/Propagation.meta');
Nt = str2double(pmeta.Nt) / str2double(pmeta.Wcycle);
dt = str2double(pmeta.dt) * str2double(pmeta.Wcycle);
t = (1:Nt)*dt;

fint = @(bra,ket) sum(conj(bra) .* ket) * dx; % integral of to two WFs

%
% Calculate the el. population and el. coherences
% Note that the populations are also in 'propa/norm'
%

pop = zeros(Nt,2);  % populations (should add up to 1)
coh = zeros(Nt,1);  % coherences

for i=1:Nt
    wf = qd_read_wf(['propa/WF_' num2str(i)]);
    pop(i,:) = real([fint(wf{1},wf{1}) fint(wf{2},wf{2})]); % populations
    coh(i) = abs(fint(wf{1},wf{2})); % magnitude of the coherence
end

figure();
plot(t/t2au,pop, t/t2au, coh,'k-');
xlabel('t [fs]');
title('Populations and coherences')

