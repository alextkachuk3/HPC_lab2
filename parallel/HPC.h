#pragma once

#include <mpi.h>

#include "../serial/Matrix.h"

class HPC
{
public:
	HPC(int argc, char* argv[]);
	~HPC();
	Matrix matrix_multiplication(Matrix* const& A = nullptr, Matrix* const& B = nullptr);

	int get_process_rank();

private:
	void create_grid_communicators();
	void process_initialization();
	void data_distribution();
	void checkerboard_matrix_scatter(double*& matrix, double*& matrix_block);
	void parallel_result_calculation();
	void A_block_communication(const int& iter);
	void B_block_communication();
	void result_collection();

	void log(const std::string& message);

	int process_num;
	int process_rank;
	int grid_size;
	int grid_coords[2];
	MPI_Comm grid_comm;
	MPI_Comm col_comm;
	MPI_Comm row_comm;

	int size;
	int block_size;
	Matrix* A;
	Matrix* B;
	Matrix C;
	Matrix A_block;
	Matrix B_block;
	Matrix C_block;
	Matrix matrix_A_block;
};
