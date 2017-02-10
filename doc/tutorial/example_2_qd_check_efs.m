%
% Check the generated efs and calculate some Franck-Condon factors
% Have a look at efs_g/energy.dat
% 

% Check the vibrational eigenfunctions of the electronic ground state
% (press enter to jump to the next one)
qd_view_efs('efs_g')

%% Read the 

[gs, wfmeta, wfheader] = qd_read_wf('efs_g/ef_0');

% get the meta data
Nx  = double(wfmeta.dim.size);
xmin  = wfmeta.dim.xmin;
xmax  = wfmeta.dim.xmax;

% calculate the Delta x
dx = (xmax - xmin) / (Nx - 1);

nrm = sum(abs(gs).^2) * dx;

fprintf('Norm (should be 1): %.6f\n', nrm);

% Let's calculate some Franck-Condon factors

Nef = 15;
FC = zeros(1,Nef);

for i = 1:Nef
    ef_e  = qd_read_wf(['efs_e/ef_' num2str(i)]);
    FC(i) = abs(sum(conj(gs) .* ef_e)) * dx;
end

clf
bar(0:Nef-1, FC);
xlabel('vib. quantum number');
title('Franck-Condon factors');

