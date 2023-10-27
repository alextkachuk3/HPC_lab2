#include <iostream>
#include <iomanip>
#include <chrono>

#include "Matrix.h"

bool print_values = false;
bool evaluation_test = false;

void test_matrix_vector_multiplication(const size_t& size)
{
	Matrix matrix_left(size);
	Matrix matrix_right(size);
	matrix_left.random_data_initialization();
	matrix_right.random_data_initialization();

	auto start = std::chrono::high_resolution_clock::now();

	Matrix result = matrix_left * matrix_right;

	auto finish = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() * 1.0e-9;

	if (print_values)
	{
		size_t outputWide = 10;
		matrix_left.set_output_wide(outputWide);
		matrix_right.set_output_wide(outputWide);
		result.set_output_wide(outputWide);

		std::cout << "Left matrix" << std::endl << matrix_left;
		std::cout << "Right matrix" << std::endl << matrix_right;
		std::cout << "Result vector:" << std::endl << result;
	}

	std::cout << "Time of execution: " << std::fixed << std::setprecision(12) << duration << std::endl;
}

int main(int argc, char* argv[])
{
	srand(clock());

	for (size_t i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0)
		{
			print_values = true;
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			evaluation_test = true;
		}
	}

	if (evaluation_test)
	{
		size_t evaluation_sizes[] = { 10, 100, 500, 1000, 1500, 2000, 2500, 3000 };

		for (size_t i = 0; i < sizeof(evaluation_sizes) / sizeof(size_t); i++)
		{
			Matrix matrix_left(evaluation_sizes[i]);
			Matrix matrix_right(evaluation_sizes[i]);
			matrix_left.random_data_initialization();
			matrix_right.random_data_initialization();

			std::cout << std::endl << "Matrix size " << evaluation_sizes[i] << "x"
				<< evaluation_sizes[i] << ":" << std::endl;

			test_matrix_vector_multiplication(evaluation_sizes[i]);
		}
		return 0;
	}

	size_t size;

	std::cout << "Enter size of matrix:";

	std::cin >> size;

	test_matrix_vector_multiplication(size);

	return 0;

}
