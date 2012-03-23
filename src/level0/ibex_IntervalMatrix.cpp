/* ============================================================================
 * I B E X - ibex_IntervalMatrix.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 6, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalMatrix.h"

namespace ibex {

IntervalMatrix::IntervalMatrix(int nb_rows, int nb_cols) : _nb_rows(nb_rows), _nb_cols(nb_cols) {
	assert(nb_rows>0);
	assert(nb_cols>0);

	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
	}
}

IntervalMatrix::IntervalMatrix(int nb_rows, int nb_cols, const Interval& x) : _nb_rows(nb_rows), _nb_cols(nb_cols) {
	assert(nb_rows>0);
	assert(nb_cols>0);

	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=x;
	}
}

IntervalMatrix::IntervalMatrix(int m, int n, double bounds[][2]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);

	int b=0; // counter for "bounds"
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			M[i].vec[j]=Interval(bounds[b][0],bounds[b][1]);
			b++;
		}
	}
}

IntervalMatrix& IntervalMatrix::operator&=(const IntervalMatrix& m) {
	assert(nb_rows()==m.nb_rows());
	assert(nb_cols()==m.nb_cols());

	if (is_empty()) return *this;
	if (m.is_empty()) { set_empty(); return *this; }

	for (int i=0; i<_nb_rows; i++) {
		if ((row(i) &= m.row(i)).is_empty()) { set_empty(); return *this; }
	}
	return *this;
}

bool IntervalMatrix::operator==(const IntervalMatrix& m) {
	if (m.nb_rows()!=nb_rows()) return false;
	if (m.nb_cols()!=nb_cols()) return false;

	if (is_empty()) return m.is_empty();
	if (m.is_empty()) return is_empty();

	for (int i=0; i<_nb_rows; i++) {
		if (row(i)!=m.row(i)) return false;
	}
	return true;
}

void IntervalMatrix::resize(int nb_rows, int nb_cols) {
	assert(nb_rows>0);
	assert(nb_cols>0);

	IntervalVector* M2 = new IntervalVector[nb_rows];
	for (int i=0; i<nb_rows; i++) {
		M2[i].resize(nb_cols);  // ALL_REALS by default
		if (i<_nb_rows) {
			int min_cols=nb_cols<_nb_cols?nb_cols:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=M[i][j];
		}
	}
	delete[] M;
	M=M2;
	_nb_rows = nb_rows;
	_nb_cols = nb_cols;
}

IntervalMatrix::IntervalMatrix(const IntervalMatrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=m[i][j];
	}
}

IntervalMatrix IntervalMatrix::submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) {
	assert(row_start_index>=0 && row_start_index<nb_rows());
	assert(row_end_index>=0 && row_end_index<nb_rows());
	assert(row_start_index<=row_end_index);
	assert(col_start_index>=0 && col_start_index<nb_cols());
	assert(col_end_index>=0 && col_end_index<nb_cols());
	assert(col_start_index<=col_end_index);

	IntervalMatrix sub(row_end_index-row_start_index+1, col_end_index-col_start_index+1);
	//cout << "m=" << (row_end_index-row_start_index+1) << "n=" << (col_end_index-col_start_index+1) << endl;
	//cout << sub << endl;
	int i2=0;
	for (int i=row_start_index; i<=row_end_index; i++, i2++) {
		int j2=0;
		for (int j=col_start_index; j<=col_end_index; j++,j2++)
			sub[i2][j2] = M[i][j];
	}
	return sub;
}

IntervalVector IntervalMatrix::col(int j) const {
	assert(j>=0 && j<nb_cols());

	IntervalVector res(nb_rows());
	for (int i=0; i<nb_rows(); i++)
		res[i]=(*this)[i][j];
	return res;
}

void IntervalMatrix::set_col(int col, const IntervalVector& v) {
	assert(col>=0 && col<nb_cols());
	assert(nb_rows()==v.size());

	for (int i=0; i<nb_rows(); i++)
		M[i][col]=v[i];
}

IntervalMatrix IntervalMatrix::operator-() const {
	IntervalMatrix res(nb_rows(),nb_cols());
	for (int i=0; i<nb_rows(); i++)
		res[i]=-(*this)[i];
	return res;
}

IntervalMatrix IntervalMatrix::operator+=(const IntervalMatrix& m) {
	assert(nb_rows()==m.nb_rows());
	assert(nb_cols()==m.nb_cols());

	for (int i=0; i<nb_rows(); i++)
		(*this)[i]+=m[i];

	return *this;
}

IntervalMatrix operator+(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

IntervalMatrix IntervalMatrix::operator-=(const IntervalMatrix& m) {
	assert(nb_rows()==m.nb_rows());
	assert(nb_cols()==m.nb_cols());

	for (int i=0; i<nb_rows(); i++)
		(*this)[i]-=m[i];

	return *this;
}

IntervalMatrix operator-(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	assert(m1.nb_rows()==m2.nb_rows());
	assert(m1.nb_cols()==m2.nb_cols());

	return IntervalMatrix(m1)-=m2;
}

IntervalMatrix operator*(double d, const IntervalMatrix& m) {
	IntervalMatrix res(m);
	for (int i=0; i<m.nb_rows(); i++)
		for (int j=0; j<m.nb_cols(); j++)
			res[i][j]*=d;
	return res;
}

IntervalMatrix operator*(const Interval& x, const IntervalMatrix& m) {
	IntervalMatrix res(m);
	for (int i=0; i<m.nb_rows(); i++)
		for (int j=0; j<m.nb_cols(); j++)
			res[i][j]*=x;
	return res;
}

IntervalMatrix IntervalMatrix::operator*=(const IntervalMatrix& m) {
	IntervalMatrix copy(*this);

	resize(nb_rows(),m.nb_cols());

	*this=copy*m;

	return *this;
}

IntervalMatrix operator*(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	assert(m1.nb_cols()==m2.nb_rows());

	IntervalMatrix m3(m1.nb_rows(),m2.nb_cols());

	for (int i=0; i<m1.nb_rows(); i++) {
		for (int j=0; j<m2.nb_cols(); j++) {
			m3[i][j]=0;
			for (int k=0; k<m1.nb_cols(); k++)
				m3[i][j]+=m1[i][k]*m2[k][j];
		}
	}

	return m3;
}

IntervalVector operator*(const IntervalMatrix& m, const IntervalVector& x) {
	assert(m.nb_cols()==x.size());

	IntervalVector y(m.nb_rows());

	for (int i=0; i<m.nb_rows(); i++) {
		y[i]=0;
		for (int k=0; k<x.size(); k++) {
			y[i]+=m[i][k]*x[k];
		}
	}

	return y;
}

std::ostream& operator<<(std::ostream& os, const IntervalMatrix& m) {
	os << "(";
	for (int i=0; i<m.nb_rows(); i++) {
		os << "(";
		for (int j=0; j<m.nb_rows(); j++) {
			os << m[i][j];
			if (j<m.nb_rows()-1) os << " ; ";
		}
		os << ")";
	}
	os << ")";
	return os;
}

/*
IntervalMatrix& IntervalMatrix::operator&=(const IntervalMatrix& x) {
	assert(x.nb_rows()==nb_rows());
	assert(x.nb_cols()==nb_cols());

	for (int i=0; i<nb_rows(); i++)
		(*this)[i] &= x[i];
	return *this;
}*/



} // namespace ibex
