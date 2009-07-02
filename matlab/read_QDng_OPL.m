%reads a OPL file from QDng and creates a OP struct
%Standard arguments: WF filename (witout extention) 
%implemented only for one dimmension yet!
%struct OPL:
%
%OPL.class => class (Laser)
%OPL.Nt => Timesteps
%OPL.dt => Timestep
%OPL.data => Pulse


function OPL=read_QDng_OPL(OPL_in_filename)

meta_in=[OPL_in_filename '.meta'];
meta_file=fopen(meta_in,'r');
data_in=[OPL_in_filename '.op'];
op_file=fopen(data_in,'rb');
class = '';    

    
while 1
     l_meta_data = fgetl(meta_file);
     if ~ischar(l_meta_data),   break,   end
     [name value]= strread(l_meta_data,'%s%s', 'delimiter', '=');
     if strmatch('#', name), continue , end
     if strmatch('CLASS', name), value=value{1:end};class=sscanf(value,'%s'); continue , end
     if strmatch('Nt', name), value=value{1:end};Nt=sscanf(value,'%d');  continue , end
     if strmatch('dt', name), value=value{1:end};dt=sscanf(value,'%g');  continue , end
        
end
    
fclose(meta_file);
OPL_all=fread(op_file,'double');
data=zeros(Nt,1);
for i=1:1:Nt
    data(i,1)=OPL_all(i);
end
    
fclose(op_file);

    
OPL=struct('class',class,'Nt',Nt,'dt',dt,'data',data);
    