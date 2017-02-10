%
% setup up a 1D system and write QDng conforming data input files
%

% conversion factors
a0 = 0.529;
ev = 27.21;

% system parameters
Nx = 128;  % numberof grid points
xmin = 2;
xmax = 8;

% define a ground and excited state potential (MgH+)
fVg  = @(x) 0.08664*(1-exp(-0.764*(x-3.141))).^2;
fVe  = @(x) 0.08602*(1-exp(-0.5558*(x-3.757))).^2+0.1578;

% setup numerical grid
x = linspace(xmin, xmax, Nx);

Vg = fVg(x);
Ve = fVe(x);

% plot potentials
subplot(2,1,1);
plot(x*a0, Vg*ev, 'k-', x*a0, Ve*ev, 'r-');
xlabel('x [A]'); ylabel('E [eV]');
title('Potential Energy Curves');

% create a simple wave function guess and plot it
wfguess = exp(-(x-2/a0).^2/0.2^2/2);

subplot(2,1,2);
plot(x*a0, wfguess, 'k-');
xlabel('x [A]'); ylabel('\Psi');
title('Guess Wave Function');

% Prepare the meta data for the potential and write it
Vmeta = struct('class','GridPotential', 'xmin',xmin, 'xmax', xmax); % that's sufficient
qd_write_op('pot_Vg', Vg, Vmeta); % this will create pot_Vg.op
qd_write_op('pot_Ve', Ve, Vmeta); % this will create pot_Ve.op


% Prepare the meta data for the wave function and write it
WFmeta = struct('class','WFGridCartesian', 'xmin', xmin, 'xmax', xmax); % that's sufficient
qd_write_wf('wfguess', wfguess, WFmeta); % This will create wfguess.wf

% Now go ahead and run on the console 'qdng efs.qds'






