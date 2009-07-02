%reads a OP file from QDng and creates a OP struct
%Standard arguments: WF filename (witout extention) 
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


function OP=read_QDng_OP(OP_in_filename)

meta_in=[OP_in_filename '.meta'];
meta_file=fopen(meta_in,'r');
data_in=[OP_in_filename '.op'];
op_file=fopen(data_in,'rb');
class = '';    

while 1
    
    l_meta_data = fgetl(meta_file);
    if ~ischar(l_meta_data),   break,   end
    [name value]= strread(l_meta_data,'%s%s', 'delimiter', '=');
    if strmatch('#', name), continue , end
    if strmatch('dims', name), dims=value{1:end}; dims=sscanf(dims,'%d'); continue , end
    
end


frewind(meta_file)

if dims == 1
    
    grid=zeros(dims,3);
    
    while 1
    
        l_meta_data = fgetl(meta_file);
        if ~ischar(l_meta_data),   break,   end
        [name value]= strread(l_meta_data,'%s%s', 'delimiter', '=');
        if strmatch('#', name), continue , end
        if strmatch('CLASS', name), value=value{1:end};class=sscanf(value,'%s'); continue , end
        if strmatch('xmax0', name), value=value{1:end};grid(1,3)=sscanf(value,'%g');  continue , end
        if strmatch('xmin0', name), value=value{1:end};grid(1,2)=sscanf(value,'%g');  continue , end
        if strmatch('N0', name), value=value{1:end};grid(1,1)=sscanf(value,'%d');  continue , end
        
    end
    
    fclose(meta_file);
    OP_all=fread(op_file,'double');
    data=zeros(grid(1,1),1);
    for i=1:1:grid(1,1)
        data(i,1)=OP_all(i);
    end
    
        fclose(op_file);
    end
    
    OP=struct('class',class,'dims',dims,'grid',grid,'data',data);
    
end
