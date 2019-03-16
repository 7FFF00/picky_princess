#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <type_traits> // needed for access to std remove_pointer

template <typename T>
class Matrix {
public:
	unsigned int width, height;
	Matrix() {}
	Matrix(unsigned int width, unsigned int height, bool is_pointer = false):
		width(width), height(height) {
		// checks if declared type is a pointer, and initializes accordingly
		// with a single deref'd version of the declared type to the pointed locations
		// otherwise just declares normally

		data = new T*[width]; // declare an initial row of pointers
			// to connect columns of pointers
		if (!is_pointer) {
			for (int i = 0; i < (int)width; i++) {
				data[i] = new T[height]; // declaring a column of pointers to data
					// per position on x/row
				for (int j = 0; j < (int)height; j++) {
					data[i][j] = T(); // initializing the pointed data at coord
						// (x, y) with default constructor of passed type
				}
			}
		} else if (is_pointer) {
			for (int i = 0; i < (int)width; i++) {
				data[i] = new T[height]; // declaring a column of pointers to data
					// per position on x/row
				for (int j = 0; j < (int)height; j++) {
					data[i][j] = new typename std::remove_pointer<T>::type();
						// initializing the pointed data at coord
						// with a pointer to a default constructed version of
						// passed type
				}
			}
		}
	}
	~Matrix() {}

	inline T& operator()(int x, int y) { return data[x][y]; } // allows call by matrix_var(x, y)
private:
	T **data; // **T can be interacted with like an array
};

#endif
