%Plots a WF struct from QDng
%Standard arguments: WF struct
%struct WF:
%WF.class => class of the WF
%WF.dims => #dims
%WF.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%WF.data => data

function return_val=plot_QDng_WF(WF_in)
dx=(WF_in.grid(1,3)-WF_in.grid(1,2))/(WF_in.grid(1,1)-1);
x=WF_in.grid(1,2):dx:WF_in.grid(1,3);
plot(x,abs(WF.data),'-b',x,real(WF.data),'-g',x,imag(WF.data),'-r');
return_val=1;