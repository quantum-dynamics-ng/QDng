%Creates a WF struct for QDng
%Standard arguments: WF_in, class ,N1 (points in dim 1),min1 (min in
%dim 1),max1 (max in dim 1) ... || WF_in, class , x-vectror
%implemented only for one dimmension yet!
%struct WF:
%WF.class => class of the WF
%WF.dims => #dims
%WF.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%WF.data => data

function WF=make_QDng_WF(WF_in,class,varargin)

optargin = size(varargin,2);

if optargin == 0
    error('Wrong inputarguments');
end

if optargin == 1
    
    x=varargin{1};
    
    if size(x,1) == 1
        dims=1;
        grid=zeros(dims,3);
        grid(1,1)=size(x,2);
        grid(1,2)=x(1,1);
        grid(1,3)=x(1,end);
        if strncmp('WFGrid', class,6) || strncmp('WFMultistate', class,12) || strncmp('Grid', class,4)
            data=WF_in;
        end    
        
        
    elseif size(x,2) == 1
        dims=1;
        grid=zeros(dims,3);
        grid(1,1)=size(x,1);
        grid(1,2)=x(1,1);
        grid(1,3)=x(end,1);
        if strncmp('WFGrid', class,6) || strncmp('WFMultistate', class,12) || strncmp('Grid', class,4)
            data=WF_in;
        end
        
    end
    WF=struct('class',class,'dims',dims,'grid',grid,'data',data);
elseif optargin == 3
    
   dims=1;
   grid=zeros(dims,3);
   grid(1,1)=varargin{1};
   grid(1,2)=varargin{2};
   grid(1,3)=varargin{3};
   if  grid(1,1)~=max(size(WF_in))
      error('Dimension mismatch in make_QDng_WF'); 
   end
   if strncmp('WFGrid', class,6) || strncmp('WFMultistate', class,12)
       data=WF_in;
   end
   WF=struct('class',class,'dims',dims,'grid',grid,'data',data);
else
    disp('Error! Check inputarguments');
end
