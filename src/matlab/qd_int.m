function y = qd_int(x, y_dims)
%
% y = qd_int(x, y_dims)
% Integrate over the dimensions given in y_dims.
%
% returns the density in the remaining dimensions

if isa(x,'htensor')
    y = qd_int_htd(x, y_dims); % redirect
    y = conj(y) .* y;
else
    y = conj(x) .* x;
    for k=1:length(y_dims)
        y = sum(y, y_dims(k));
    end
end
