function msg = pblib_create_message(descriptor)
%
% msg = pblib_create_message(descriptor)
%
% Create an empty message from the descriptor function
% (must be given as a function handle)
% 


% Create the has_field map and set default values
msg.has_field = java.util.HashMap;

desc = descriptor();

for field=desc.fields
   put(msg.has_field, field.name, 0);
   msg.(field.name) = field.default_value;
end

msg.unknown_fields = [];
msg.descriptor_function = descriptor;