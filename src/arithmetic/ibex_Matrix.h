/* ============================================================================
 * I B E X - Matrix of reals
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 18, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_MATRIX_H__
#define __IBEX_MATRIX_H__

#include "ibex_Vector.h"
#include <iostream>

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Real matrix.
 */
class Matrix {

public:
	/**
	 * \brief Create a (nb_rows x nb_cols) matrix.
	 */
	Matrix(int nb_rows, int nb_cols);

	/**
	 * \brief Create a (nb_rows x nb_cols) matrix and initialize
	 * all the elements with x.
	 */
	Matrix(int nb_rows, int nb_cols, double x);

	/**
	 * \brief Duplicate a matrix.
	 */
	Matrix(const Matrix& m);

	/**
	 * \brief Create an interval matrix from an array of doubles.
	 *
	 * Create the Matrix: <br>
	 *  x[0] ; ... ; x[n-1]   <br>
	 *  x[n] ; ... ; x[2n-1]  <br>
	 *  ... <br>
	 *  x[(m-1)n] ; ... ; x[2n-1]  <br>
	 *
	 * \param bounds an (mxn) array of doubles
	 * \pre m>0, n>0
	 */
	Matrix(int m, int n, double x[]);

	/**
	 * \brief Delete *this.
	 */
	~Matrix();

	/**
	 * \brief Set *this to m.
	 */
	Matrix& operator=(const Matrix& x);

	/**
	 * \brief True if the entries of (*this) coincide with m.
	 *
	 */
	bool operator==(const Matrix& m) const;

	/**
	 * \brief True if one entry of (*this) differs from m.
	 */
	bool operator!=(const Matrix& m);

	/**
	 * \brief Return the number of columns.
	 */
	int nb_cols() const;

	/**
	 * \brief Return the number of rows.
	 */
	int nb_rows() const;

	/**
	 * \brief Return the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	Vector& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const Vector& operator[](int i) const;

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 */
	Vector& row(int i);

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 */
	const Vector& row(int i) const;

	/**
	 * \brief Return a column
	 */
	Vector col(int i) const;

	/**
	 * \brief Set a row.
	 */
	void set_row(int row, const Vector& v);

	/**
	 * \brief Set a column.
	 */
	void set_col(int col, const Vector& v);

    /**
     * \brief Set *this to (*this)+m.
     */
    Matrix& operator+=(const Matrix& m);

    /**
     * \brief Set *this to (*this)-m.
     */
    Matrix& operator-=(const Matrix& m);

    /**
     * \brief Set *this to a*(*this).
     */
    Matrix& operator*=(double a);

    /**
     * \brief Set *this to (*this)*m.
     */
    Matrix& operator*=(const Matrix& m);

    /**
     * Return the identity matrix n*n
     */
    static Matrix eye(int n);

    /**
     * Return a n*n matrix of zeros
     */
    static Matrix zeros(int n);

    /**
     * Return a m*n matrix of zeros
     */
    static Matrix zeros(int m, int n);

    /**
     * Return a n*n matrix of ones
     */
    static Matrix ones(int n);

    /**
     * Return a m*n matrix of ones
     */
    static Matrix ones(int m, int n);

private:
	int _nb_rows;
	int _nb_cols;
	Vector* M;
};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief -M
 */
Matrix operator-(const Matrix& m);

/**
 * \brief $[m]_1+[m]_2$.
 */
Matrix operator+(const Matrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
Matrix operator-(const Matrix& m1, const Matrix& m2);

/**
 * \brief Scalar multiplication of a matrix.
 */
Matrix operator*(double d, const Matrix& m);

/**
 * \brief $[m]_1*[m]_2$.
 */
Matrix operator*(const Matrix& m1, const Matrix& m2);

/**
 * \brief $[m]*[x]$.
 */
Vector operator*(const Matrix& m, const Vector& x);

/**
 * \brief $[x]*[m]$.
 */
Vector operator*(const Vector& x, const Matrix& m);

/**
 * \brief |x|.
 */
Matrix abs(const Matrix& x);

/**
 * \brief Stream out a matrix.
 */
std::ostream& operator<<(std::ostream& os, const Matrix&);

/*@}*/

/*================================== inline implementations ========================================*/


inline bool Matrix::operator!=(const Matrix& m) {
	return !(*this==m);
}

inline int Matrix::nb_cols() const {
	return _nb_cols;
}

inline int Matrix::nb_rows() const {
	return _nb_rows;
}

inline Vector& Matrix::operator[](int i) {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline const Vector& Matrix::operator[](int i) const {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline Vector& Matrix::row(int i) {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline const Vector& Matrix::row(int i) const {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline void Matrix::set_row(int row, const Vector& v) {
	assert(row>=0 && row<nb_rows());
	assert(nb_cols()==v.size());
	M[row]=v;
}

inline Matrix Matrix::eye(int n) {
	Matrix M(n,n);
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			M[i][j]=i==j ? 1.0 : 0.0;
	return M;
}

inline Matrix Matrix::zeros(int n) {
	return Matrix(n,n,0.0);
}

inline Matrix Matrix::zeros(int m, int n) {
	return Matrix(m,n,0.0);
}

inline Matrix Matrix::ones(int n){
	return Matrix(n,n,1.0);
}

inline Matrix Matrix::ones(int m, int n) {
	return Matrix(m,n,1.0);
}

} // namespace ibex
#endif // __IBEX_MATRIX_H__
