%Creates a OP struct for QDng
%Standard arguments: OP,class ,N1 (points in dim 1),min1 (min in
%dim 1),max1 (max in dim 1) ... | OP, x-vectror
%implemented only for one dimmension yet!
%struct OP:
%
%OP.dims => #dims
%OP.class => class
%OP.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%OP.data => data

function OP=make_QDng_OP(OP_in,class,varargin)

optargin = size(varargin,2);

if optargin == 1
    
    x=varargin{1};
    
    if size(x,1) == 1
        dims=1;
        grid=zeros(dims,3);
        grid(1,1)=size(x,2);
        grid(1,2)=x(1,1);
        grid(1,3)=x(1,end);
        if strncmp('Grid', class,4)
            data=OP_in;
        end
        
        
    elseif size(x,2) == 1
        dims=1;
        grid=zeros(dims,3);
        grid(1,1)=size(x,1);
        grid(1,2)=x(1,1);
        grid(1,3)=x(end,1);
        if strncmp('Grid', class,4)
            data=OP_in;
        end    
    end

elseif optargin == 3
    
   dims=1;
   grid=zeros(dims,3);
   grid(1,1)=varargin{1};
   grid(1,2)=varargin{2};
   grid(1,3)=varargin{3};
   if strncmp('Grid', class,4)
       data=OP_in;
   end
   
else
    disp('Error! Check inputarguments');
end
OP=struct('class',class,'dims',dims,'grid',grid,'data',data);