package QDLIB;

//
// Simple header describing a grid system.
//
message GridSystemHeader {
	required uint32 dims = 1;
	repeated dim_description_t dim = 2;
	
	message dim_description_t {
		required uint32 size = 1;
		required double xmin = 2;
		required double xmax = 3;
	}
	
	optional uint64 data_size = 3;
}