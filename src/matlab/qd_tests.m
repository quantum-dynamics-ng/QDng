Tkin = '<A name="GridNablaSq" mass="1763.2,1763.2" key="T"/>';
qd_cmd_load_op(Tkin);

%%
x = linspace(-3,3,64);
meta = struct();
meta.xmin = [-3 -3];
meta.xmax = [3 3];

[X,Y] = ndgrid(x,x);
wf = exp(-X.^2) .* exp(-Y.^2);

surf(X,Y, wf);

qd_cmd_expec(wf, meta, 'WFGridCartesian', 'T')
%%
qd_cmd_matel(wf, wf, meta, 'WFGridCartesian', 'T')

%%
op_wf=qd_cmd_apply_op(wf, meta, 'WFGridCartesian', 'T');