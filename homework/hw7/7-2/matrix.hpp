#ifndef MATRIX
#define MATRIX
#include <iostream>
#include <vector>

template <typename T>
class Matrix {
 public:
  Matrix();
  Matrix(unsigned int rows, unsigned int cols, const T& value = T());

  T& operator()(unsigned int r, unsigned int c);

  void Print();
  void GaussJordan();

 private:
  std::vector<std::vector<T>> matrix_;
  int colums_number_, rows_number_;
  // Your implementation here.
};

// constructor
template <typename T>
Matrix<T>::Matrix() : colums_number_(0), rows_number_(0) {}
template <typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, const T& value)
    : matrix_(rows, std::vector<T>(cols, value)),
      colums_number_(cols),
      rows_number_(rows) {}

// operator
template <typename T>
T& Matrix<T>::operator()(unsigned int r, unsigned int c) {
  return this->matrix_[r][c];
}

// other functions
template <typename T>
void Matrix<T>::Print() {
  for (const auto column : matrix_) {
    for (const auto entry : column) std::cout << entry << " ";
    std::cout << std::endl;
  }
}
template <typename T>
void Matrix<T>::GaussJordan() {
  int pivot_row = 0, pivot_column = 0;
  while (pivot_row < rows_number_ && pivot_column < colums_number_) {
    int i_max = pivot_row, i_min = pivot_row;
    for (int i = pivot_row + 1; i < rows_number_; ++i) {
      if (matrix_[i_max][pivot_column] < matrix_[i][pivot_column]) i_max = i;
      if (matrix_[i_min][pivot_column] > matrix_[i][pivot_column]) i_min = i;
    }
    if (matrix_[i_max][pivot_column] == 0) i_max = i_min;
    if (matrix_[i_max][pivot_column] == 0)
      pivot_column++;
    else {
      std::swap(matrix_[pivot_column], matrix_[i_max]);
      for (int i = pivot_row + 1; i < rows_number_; ++i) {
        T factor = matrix_[i][pivot_column] / matrix_[pivot_row][pivot_column];
        matrix_[i][pivot_column] = 0;
        for (int j = pivot_column + 1; j < colums_number_; ++j)
          matrix_[i][j] -= matrix_[pivot_row][j] * factor;
      }
      ++pivot_row;
      ++pivot_column;
    }
  }
}

#endif
