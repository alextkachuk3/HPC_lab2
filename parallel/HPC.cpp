#include "HPC.h"

HPC::HPC(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	setvbuf(stdout, 0, _IONBF, 0);
}

HPC::~HPC()
{
	MPI_Finalize();
}

Matrix HPC::matrix_multiplication(Matrix* const& A, Matrix* const& B)
{
	this->A = A;
	this->B = B;

	grid_size = (int)sqrt((double)process_num);

	if (process_rank == 0)
	{
		size = (int)A->get_height();
		C = Matrix(size);
		block_size = size / grid_size;

		if (process_num != grid_size * grid_size) {
			throw std::invalid_argument("Number of processes must be a perfect square");
		}
	}

	create_grid_communicators();
	process_initialization();
	data_distribution();
	parallel_result_calculation();
	result_collection();

	return C;
}

int HPC::get_process_rank()
{
	return process_rank;
}

void HPC::create_grid_communicators()
{
	int dim_size[2];
	int periodic[2];
	int subdims[2];

	dim_size[0] = grid_size;
	dim_size[1] = grid_size;
	periodic[0] = 0;
	periodic[1] = 0;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, periodic, 1, &grid_comm);
	MPI_Cart_coords(grid_comm, process_rank, 2, grid_coords);

	subdims[0] = 0;
	subdims[1] = 1;
	MPI_Cart_sub(grid_comm, subdims, &row_comm);

	subdims[0] = 1;
	subdims[1] = 0;
	MPI_Cart_sub(grid_comm, subdims, &col_comm);
}

void HPC::process_initialization()
{
	if (size % grid_size != 0) {
		throw std::invalid_argument("Size of matrices must be divisible by the grid size!\n");
	}

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	block_size = size / grid_size;
	A_block = Matrix(block_size, block_size);
	B_block = Matrix(block_size, block_size);
	matrix_A_block = Matrix(block_size, block_size);
}

void HPC::data_distribution()
{
	checkerboard_matrix_scatter(A->get_values(), matrix_A_block.get_values());
	checkerboard_matrix_scatter(B->get_values(), B_block.get_values());
}

void HPC::checkerboard_matrix_scatter(double*& matrix, double*& matrix_block)
{
	double* matrix_row = new double[block_size * size];
	if (grid_coords[1] == 0) {
		if (process_rank != 0)
		{
			MPI_Scatter(nullptr, block_size * size, MPI_DOUBLE, matrix_row, block_size * size, MPI_DOUBLE, 0, col_comm);
		}
		else
		{
			MPI_Scatter(matrix, block_size * size, MPI_DOUBLE, matrix_row, block_size * size, MPI_DOUBLE, 0, col_comm);
		}		
	}
	for (int i = 0; i < block_size; i++) {
		MPI_Scatter(&matrix_row[i * size], block_size, MPI_DOUBLE, &(matrix_block[i * block_size]), block_size, MPI_DOUBLE, 0, row_comm);
	}
	delete[] matrix_row;
}

void HPC::parallel_result_calculation()
{
	for (int iter = 0; iter < grid_size; iter++) {
		A_block_communication(iter);
		C_block = A_block * B_block;
		B_block_communication();
	}
}

void HPC::A_block_communication(const int& iter)
{
	int pivot = (grid_coords[0] + iter) % grid_size;
	if (grid_coords[1] == pivot) {
		for (int i = 0; i < block_size * block_size; i++)
			A_block.get_values()[i] = matrix_A_block.get_values()[i];
	}
	MPI_Bcast(A_block.get_values(), block_size * block_size, MPI_DOUBLE, pivot, row_comm);
}

void HPC::B_block_communication()
{
	MPI_Status Status;
	int NextProc = grid_coords[0] + 1;
	if (grid_coords[0] == grid_size - 1) NextProc = 0;
	int PrevProc = grid_coords[0] - 1;
	if (grid_coords[0] == 0) PrevProc = grid_size - 1;
	MPI_Sendrecv_replace(B_block.get_values(), block_size * block_size, MPI_DOUBLE, NextProc, 0, PrevProc, 0, col_comm, &Status);
}

void HPC::result_collection()
{
	double* pResultRow = new double[size * block_size];
	for (int i = 0; i < block_size; i++) {
		MPI_Gather(&C_block.get_values()[i * block_size], block_size, MPI_DOUBLE,
			&pResultRow[i * size], block_size, MPI_DOUBLE, 0, row_comm);
	}
	if (grid_coords[1] == 0) {
		MPI_Gather(pResultRow, block_size * size, MPI_DOUBLE, C.get_values(),
			block_size * size, MPI_DOUBLE, 0, col_comm);
	}
	delete[] pResultRow;
}
