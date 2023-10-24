#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>

class Matrix
{
public:
	Matrix() = default;
	Matrix(const size_t& size);
	Matrix(const size_t& width, const size_t& height);
	~Matrix();

	void dummy_data_initialization();
	void random_data_initialization();

	std::string to_string() const;
	size_t get_width() const;
	size_t get_height() const;
	size_t get_submatrix_index() const;
	double*& get_values();

	void set_submutrix_index(const size_t& submatrix_index);
	void set_output_wide(const size_t& outputWide);

	bool operator==(const Matrix& other);
	friend Matrix operator*(const Matrix& left, const Matrix& right);
	Matrix& operator+=(const Matrix& other);

	friend std::ostream& operator<< (std::ostream& out, const Matrix& matrix);

private:

	size_t height;
	size_t width;
	size_t submatrix_index;
	double* values;

	int* delete_counter;

	size_t outputWide;
	const static size_t defaultOutputWide = 7;
};
