function [sigN, sigFD] = qd_cfun_TA_spectrum(ctt, Nt, dt, wL, fwhm)
% 
% [sigN, sigFD] = qd_cfun_TA_spectrum(ctt, Nt, dt, wL, fwhm)
% 
% Calculate transient absorption spectrum
%
%


t=(0:Nt-1)*dt;
w=fftaxis(t,1); % TODO: Fix, this is not part of th QDng lib


% delays
NT = floor(Nt);
dT = dt;
T = (0:NT-1)*dT;


% pulse parameters
sigmat = fwhm/1.665;

% axis
[tau,tt] = ndgrid(t,t);

sigFD = zeros(length(t),length(T));
sigN = zeros(length(T),1);
Ew = exp(-(w'-wL).^2 * sigmat^2/2);

for i=1:length(T)
    if nargout == 2
        % frequency dispersed signal
        fint = ctt .* exp(-(tau-T(i)).^2/sigmat^2/2);
        fint = fint .* exp(-1i*wL*(tau-T(i)));
        fint = fftshift(ifft(sum(fint,1))) .* exp(-1i*w'*T(i));
        fint = fint .* Ew;
        sigFD(:,i) = -2*real(fint);
        %sigFD(:,i) = fint;
    end
   
    
    % integrated photon number
    fint = ctt .* exp(-(tt-T(i)).^2/sigmat^2/2) .* exp(-(tau-T(i)).^2/sigmat^2/2);
    fint = fint .* exp(-1i*wL*(tau-tt));
    fint = sum(fint(:))*dt^2;
    sigN(i) = -2*imag(fint);
    %sigN(i) = fint;
end




end