package QDLIB;

//
// Simple header describing a grid system.
//
message GridSystemHeader {
	required uint32 dims = 1 [default = 0];
	repeated dim_description_t dim = 2;
	
	message dim_description_t {
		required uint32 size = 1 [default = 0];
		required double xmin = 2 [default = 0];
		required double xmax = 3 [default = 0];
	}
	
	optional uint64 data_size = 3  [default = 0];
}