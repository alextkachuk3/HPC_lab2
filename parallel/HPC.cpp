#include "HPC.h"

HPC::HPC(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
}

HPC::~HPC()
{
	MPI_Finalize();
}

int HPC::get_process_rank()
{
	return process_rank;
}
