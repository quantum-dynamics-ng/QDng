%Creates a OPL struct for QDng
%Standard arguments: OPL ,Nt timesteps,dt timestep | OP, t
%struct OPL:
%
%OPL.class => class (Laser)
%OPL.Nt => Timesteps
%OPL.dt => Timestep
%OPL.data => Pulse

function OPL=make_QDng_OPL(OPL_in,varargin)

optargin = size(varargin,2);
class = 'Laser';

if optargin == 0
    error('Wrong inputarguments');
end

if optargin == 1
    
    t=varargin{1};
    
    if size(t,1) == 1
        
        Nt=size(t,2);
        dt=t(1,2)-t(1,1);
        data=OPL_in;
        
    elseif size(t,2) == 1
        
        Nt=size(t,1);
        dt=t(2,1)-t(1,1);
        data=OPL_in;
        
    end
    OPL=struct('class',class,'Nt',Nt,'dt',dt,'data',data);
elseif optargin == 2
    
  
   Nt=varargin{1};
   dt=varargin{2};
   if Nt ~=max(size(OPL_in))
      error('Dimension mismatch in make_QDng_OPL'); 
   end
   data=OPL_in;
   OPL=struct('class',class,'Nt',Nt,'dt',dt,'data',data);
else
    disp('Error! Check inputarguments');
end
