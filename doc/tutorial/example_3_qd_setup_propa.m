%
% Create a laser field and a transition dipole moment
% in order to setup up the propagation.
%

% conversion factors
a0 = 0.529;
ev = 27.21;
t2au = 41.34;

% create time axis
Nt = 300;
dt = 5;
t = (0:Nt-1)*dt;

% create laser field - actinic pulse
w = 5./ev;
t0 = 8 * t2au;
sigma = 3 * t2au/1.66;

% Gaussian pulse with cos carrier wave.
Et = 0.15 * cos(w * (t-t0)) .* exp(-(t-t0).^2/sigma^2/2);

plot(t/t2au, Et);
xlabel('t [fs]'); ylabel('E [au]');
title('Laser field');

% Write the laser field - it's treated as an operator
Emeta = struct('class', 'Laser', 'dt', dt);
qd_write_op('Et', Et, Emeta);

% Create a simple transition dipole moment 
[~, mumeta] = qd_read_op('pot_Vg'); % grab the meta data;
mu = ones(mumeta.dim.size,1);  % we use a constant trans. dipole for now
qd_write_op('mu', mu, mumeta);

% run 'qdng propa.qds'

