%Convets a WF of QD (from inputfile) to WF struct for QDng
%Standard arguments: Infile,class,N1 (points in dim 1),min1 (min in
%dim 1),max1 (max in dim 1) ...
%implemented only for one dimmension yet!
%struct WF:
%WF.class => class of the WF
%WF.dims => #dims
%WF.grid => array (#dims x 3)
%            grid(?,1)=>#points in dim?
%            grid(?,2)=>min dim?
%            grid(?,3)=>max dim?
%WF.data => data

function WF=convert_QDng_WF(infilename,class,varargin)

optargin = size(varargin,2);

if optargin == 3
   dims=1;
   file=fopen(infilename,'r');
   N1=varargin{1};
   min1=varargin{2};
   max1=varargin{3};
   WF_all=fread(file,'double');
   grid=zeros(dims,3);
   grid(1,1)=N1;
   grid(1,2)=min1;
   grid(1,3)=max1;
   data=zeros(N1,1);
   for i=2:2:2*N1
       data(i/2,1)=WF_all(i-1)+j*WF_all(i);
   end
   WF=struct('class',class,'dims',dims,'grid',grid,'data',data);
else
    disp('Error! Check inputarguments');
end
