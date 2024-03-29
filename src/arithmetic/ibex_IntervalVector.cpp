/* ============================================================================
 * I B E X - Interval Vector definition
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 05, 2011
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalVector.h"
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <float.h>
#include <algorithm>


#include "ibex_TemplateVector.cpp_"

namespace ibex {

IntervalVector::IntervalVector(int nn) : n(nn), vec(new Interval[nn]) {
	assert(nn>=1);
	for (int i=0; i<nn; i++) vec[i]=Interval::ALL_REALS;
}

IntervalVector::IntervalVector(int n1, const Interval& x) : n(n1), vec(new Interval[n1]) {
	assert(n1>=1);
	for (int i=0; i<n1; i++) vec[i]=x;
}

IntervalVector::IntervalVector(const IntervalVector& x) : n(x.n), vec(new Interval[x.n]) {
	assert(x.vec!=NULL); // forbidden to copy uninitialized boxes
	for (int i=0; i<n; i++) vec[i]=x[i];
}

IntervalVector::IntervalVector(int n1, double bounds[][2]) : n(n1), vec(new Interval[n1]) {
	if (bounds==0) // probably, the user called IntervalVector(n,0) and 0 is interpreted as NULL!
		for (int i=0; i<n1; i++)
			vec[i]=Interval::ZERO;
	else
		for (int i=0; i<n1; i++)
			vec[i]=Interval(bounds[i][0],bounds[i][1]);
}

IntervalVector::IntervalVector(const Vector& x) : n(x.size()), vec(new Interval[n]) {
	for (int i=0; i<n; i++) vec[i]=x[i];
}

void IntervalVector::init(const Interval& x) {
	for (int i=0; i<size(); i++)
		(*this)[i]=x;
}


void IntervalVector::resize(int n2) {
	assert(n2>=1);
	assert((vec==NULL && n==0) || (n!=0 && vec!=NULL));

	if (n2==size()) return;

	Interval* newVec=new Interval[n2];
	int i=0;
	for (; i<size() && i<n2; i++)
		newVec[i]=vec[i];
	for (; i<n2; i++)
		newVec[i]=Interval::ALL_REALS;
	if (vec!=NULL) // vec==NULL happens when default constructor is used (n==0)
		delete[] vec;

	n   = n2;
	vec = newVec;
}


IntervalVector& IntervalVector::operator&=(const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (size()!=x.size()) throw InvalidIntervalVectorOp("Cannot intersect IntervalVectores with different dimensions");

	if (is_empty()) return *this;
	if (x.is_empty()) { set_empty(); return *this; }

	for (int i=0; i<size(); i++) {
		(*this)[i] &= x[i];
		if ((*this)[i].is_empty()) {
			set_empty();
			return *this;
		}
	}
	return *this;
}

IntervalVector& IntervalVector::operator|=(const IntervalVector& x)  {
	// dimensions are non zero henceforth
	if (size()!=x.size()) throw InvalidIntervalVectorOp("Cannot make the hull of IntervalVectores with different dimensions");

	if (x.is_empty()) return *this;
	if (is_empty()) { *this=x; return *this; }

	for (int i=0; i<size(); i++) {
		(*this)[i] |= x[i];
	}
	return *this;
}


namespace {

const IntervalVector* tmp;
bool diam_lt(const int& i, const int& j) {
	return (*tmp)[i].diam()<(*tmp)[j].diam();
}
bool diam_gt(const int& i, const int& j) {
	return (*tmp)[i].diam()>(*tmp)[j].diam();
}

} // end anonymous namespace

void IntervalVector::sort_indices(bool min, int tab[]) const {
	for (int i=0; i<n; i++) tab[i]=i;
	tmp=this;
	std::sort(tab,tab+n,min? diam_lt:diam_gt);
}

double distance(const IntervalVector& x1, const IntervalVector& x2) {
	assert(x1.size()==x2.size());

	double max = ibex::distance(x1[0],x2[0]);
	for (int i=1; i<x1.size(); i++) {
		double cand = ibex::distance(x1[i],x2[i]);
		if (max<cand) max = cand;
	}
	return max;
}

double IntervalVector::maxdelta(const IntervalVector& x) {
	double max = (*this)[0].delta(x[0]);
	for (int i=1; i<size(); i++) {
		double cand = (*this)[i].delta(x[i]);
		if (max<cand) max = cand;
	}
	return max;
}


namespace { // to create anonymous structure/functions

/** \brief Complementary of an Interval
 *
 * Compute the complementary of x. The result is (c1 union c2)
 */
void complI(const Interval& x, Interval& c1, Interval& c2) {
	if (x.is_empty() || x.is_degenerated()) { // x.is_empty() should not happen if called from compl()
		c1=Interval::ALL_REALS;
		c2=Interval::EMPTY_SET;
		return;
	}
	else {
		if (x.lb()>NEG_INFINITY) {
			c1=Interval(NEG_INFINITY,x.lb());
			if (x.ub()<POS_INFINITY)
				c2=Interval(x.ub(),POS_INFINITY);
			else
				c2=Interval::EMPTY_SET;
		} else if (x.ub()<POS_INFINITY) {
			c1=Interval(x.ub(),POS_INFINITY);
			c2=Interval::EMPTY_SET;
		} else {
			c1=c2=Interval::EMPTY_SET;
		}
	}
}

/** \brief x\y
 *
 */
void diffI(const Interval& x, const Interval& y, Interval& c1, Interval& c2) {
	complI(y,c1,c2);
	c1 &= x;
	if (c1.is_degenerated()) c1=Interval::EMPTY_SET;
	c2 &= x;
	if (c2.is_degenerated()) c2=Interval::EMPTY_SET;

	if (c1.is_empty()) {
		c1=c2;
		c2=Interval::EMPTY_SET;
	}
}

} // end namespace


int IntervalVector::diff(const IntervalVector& y, IntervalVector*& result) const {
	const int nn=size();
	const IntervalVector& x=*this;
	IntervalVector *tmp = new IntervalVector[2*nn]; // in the worst case, there is 2n boxes
	Interval c1, c2;
	int b=0;
	if (y.is_empty()) {
		tmp[b].resize(nn);
		tmp[b]=x; // copy of this
		b++;
	} else {
		for (int var=0; var<nn; var++) {

			diffI(x[var],y[var],c1,c2);

			if (!c1.is_empty()) {
				tmp[b].resize(nn);
				IntervalVector& v=tmp[b++];
				for (int i=0; i<var; i++) v[i]=y[i];
				v[var]=c1;
				for (int i=var+1; i<nn; i++) v[i]=x[i];

				if (!c2.is_empty()) {
					tmp[b].resize(nn);
					IntervalVector& v=tmp[b++];
					for (int i=0; i<var; i++) v[i]=y[i];
					v[var]=c2;
					for (int i=var+1; i<nn; i++) v[i]=x[i];
				}
			}
		}
	}

	if (b==0) {
		result = new IntervalVector[1];
		result[0].resize(nn);
		result[0].set_empty();
		b=1;
	} else {
		result=new IntervalVector[b];
		for (int i=0; i<b; i++) {
			result[i].resize(nn);
			result[i]=tmp[i];
		}
	}
	delete[] tmp;

	return b;
}

int IntervalVector::complementary(IntervalVector*& result) const {
	return IntervalVector(size()).diff(*this,result);
}


bool proj_add(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2) {
	x1 &= y-x2;
	x2 &= y-x1;
	return !x1.is_empty() && !x2.is_empty();
}

bool proj_sub(const IntervalVector& y, IntervalVector& x1, IntervalVector& x2) {
	x1 &= y+x2;
	x2 &= x1-y;
	return !x1.is_empty() && !x2.is_empty();
}

bool proj_mul(const IntervalVector& y, Interval& x1, IntervalVector& x2) {
	assert(y.size()==x2.size());

	for (int i=0; i<x2.size(); i++)
		if (!proj_mul(y[i], x1, x2[i])) {
			x2.set_empty();
			return false;
		}
	return true;
}

bool proj_mul(const Interval& z, IntervalVector& x, IntervalVector& y) {
	assert(x.size()==y.size());
	int n=x.size();

	if (n==1) {
		if (proj_mul(z,x[0],y[0])) return true;
		else { x.set_empty(); y.set_empty(); return false; }
	}

	Interval* xy= new Interval[n];  // xy[i] := x[i]y[i]
	Interval* sum= new Interval[n-1]; // sum[i] := x[0]y[0]+...x[i]y[i]

	// ------------- forward --------------------
	for (int i=0; i<n; i++) xy[i]=x[i]*y[i];
	sum[0]=xy[0];
	for (int i=1; i<n-1; i++) sum[i]=sum[i-1]+xy[i];

	// ------------- backward -------------------
	// (rem: we have n>=2)
	if (!proj_add(z, sum[n-2], xy[n-1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=n-3; i>=0; i--)
		if (!proj_add(sum[i+1],sum[i],xy[i+1])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	if ((xy[0] &= sum[0]).is_empty()) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	for (int i=0; i<n; i++)
		if (!proj_mul(xy[i],x[i],y[i])) { x.set_empty(); y.set_empty(); delete[] sum; delete[] xy; return false; }

	delete[] sum; 
	delete[] xy;
	return true;
}


IntervalVector& IntervalVector::inflate(double rad1)                              { return _inflate(*this,rad1); }
IntervalVector  IntervalVector::subvector(int start_index, int end_index) const   { return _subvector(*this,start_index,end_index); }
void            IntervalVector::put(int start_index, const IntervalVector& x)     { _put(*this, start_index, x); }
IntervalVector& IntervalVector::operator=(const IntervalVector& x)                { return _assign(*this,x); }
bool            IntervalVector::operator==(const IntervalVector& x) const         { return _equals(*this,x); }
Vector          IntervalVector::lb() const                                        { return _lb(*this); }
Vector          IntervalVector::ub() const                                        { return _ub(*this); }
Vector          IntervalVector::mid() const                                       { return _mid(*this); }
Vector          IntervalVector::mig() const                                       { return _mig(*this); }
Vector          IntervalVector::mag() const                                       { return _mag(*this); }
bool            IntervalVector::is_flat() const                                   { return _is_flat(*this); }
bool            IntervalVector::contains(const Vector& x) const                   { return _contains(*this,x); }
bool            IntervalVector::is_unbounded() const                              { return _is_unbounded(*this); }
bool            IntervalVector::is_subset(const IntervalVector& x) const          { return _is_subset(*this,x); }
bool            IntervalVector::is_strict_subset(const IntervalVector& x) const   { return _is_strict_subset(*this,x); }
bool            IntervalVector::is_zero() const                                   { return _is_zero(*this); }
bool            IntervalVector::is_bisectable() const                             { return _is_bisectable(*this); }
Vector          IntervalVector::rad() const                                       { return _rad(*this); }
Vector          IntervalVector::diam() const                                      { return _diam(*this); }
int             IntervalVector::extr_diam_index(bool min) const                   { return _extr_diam_index(*this,min); }
std::ostream&   operator<<(std::ostream& os, const IntervalVector& x)             { return _display(os,x); }
double          IntervalVector::volume() const                                    { return _volume(*this); }
double          IntervalVector::perimeter() const                                 { return _perimeter(*this); }
double          IntervalVector::rel_distance(const IntervalVector& x) const       { return _rel_distance(*this,x); }
Vector          IntervalVector::random() const                                    { return _random<IntervalVector,Interval>(*this); }
std::pair<IntervalVector,IntervalVector> IntervalVector::bisect(int i, double ratio) const  { return _bisect(*this, i, ratio); }


} // end namespace

