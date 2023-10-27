#include "Matrix.h"

Matrix::Matrix(const size_t& size)
{
	outputWide = defaultOutputWide;
	submatrix_index = 0;

	this->width = size;
	this->height = size;

	values = new double[size * size] {0};
	this->delete_counter = new int{ 1 };
}

Matrix::Matrix(const size_t& width, const size_t& height)
{
	outputWide = defaultOutputWide;
	submatrix_index = 0;

	this->width = width;
	this->height = height;

	values = new double[width * height] {0};
	this->delete_counter = new int{ 1 };
}

Matrix::~Matrix()
{
	delete_counter--;
	if (delete_counter == 0)
	{
		delete[] values;
		delete delete_counter;
	}
}

std::string Matrix::to_string() const
{
	std::stringstream string;
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			string << std::setw(outputWide) << std::fixed << std::setprecision(3) << values[i * width + j];
		}
		string << std::setw(0) << std::endl;
	}
	return string.str();
}

size_t Matrix::get_width() const
{
	return width;
}

size_t Matrix::get_height() const
{
	return height;
}

size_t Matrix::get_submatrix_index() const
{
	return submatrix_index;
}

double*& Matrix::get_values()
{
	return values;
}

void Matrix::set_submutrix_index(const size_t& submatrix_index)
{
	this->submatrix_index = submatrix_index;
}

void Matrix::set_output_wide(const size_t& outputWide)
{
	this->outputWide = outputWide;
}

bool Matrix::operator==(const Matrix& other)
{
	static double Accuracy = 1.e-6;

	if (this->width != other.width && this->height != other.height)
	{
		return false;
	}

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			if (fabs(other.values[i * width + j] - values[i * width + j] >= Accuracy))
			{
				return false;
			}
		}
	}

	return true;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			values[i * width + j] += other.values[i * width + j];
		}
	}
	return *this;
}

void Matrix::dummy_data_initialization()
{
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			values[i * width + j] = 1.0;
		}
	}
}

void Matrix::random_data_initialization()
{	
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			values[i * width + j] = rand() / 1000.0;
		}
	}
}

Matrix operator*(const Matrix& left, const Matrix& right)
{
	if (left.width != right.height)
	{
		throw std::invalid_argument("Left matrix width not equal right matrix height!");
	}
	Matrix result(right.width, left.height);

	for (int i = 0; i < left.height; i++)
	{
		for (int j = 0; j < right.width; j++)
		{
			for (int k = 0; k < right.height; k++)
			{
				result.values[i * right.width + j] += left.values[i * left.width + k] * right.values[k * right.width + j];
			}
		}
	}

	return result;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
	out << matrix.to_string();

	return out;
}
