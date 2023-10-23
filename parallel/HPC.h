#pragma once

#include <mpi.h>

#include "../serial/Matrix.h"

class HPC
{
public:
	HPC(int argc, char* argv[]);
	~HPC();

	int get_process_rank();

private:
	int process_num;
	int process_rank;
};
