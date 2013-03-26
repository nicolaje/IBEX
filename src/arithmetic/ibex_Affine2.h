/* ============================================================================
 * I B E X - Affine2 definition
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Bug fixes   :
 * Created     : Nov 12, 2012
 * ---------------------------------------------------------------------------- */

#ifndef IBEX_AFFINE2_H_
#define IBEX_AFFINE2_H_

#include "ibex_Interval.h"
//#include "ibex_IntervalVector.h"
#include <cmath>
#include <cassert>

namespace ibex {


/**
 * \ingroup arithmetic
 *
 * \brief Affine Arithmetic AF2
 *
 */

class Affine2 {

private:

	int _n; 		// dimension (size of val)-1  , ie number of variable
	double * _val; 		// vector of elements of the affine form
	Interval _err; 	// error of the affine form, corresponded to the last term

	static double AF_EM() {
		return __builtin_powi(2.0, -51);
	}
	static double AF_EC() {
		return __builtin_powi(2.0, -55);
	}
	static double AF_EE() {
		return 2.0;
	}

	/**
	 * \brief  Compute x = alpha*x+y+beta
	 * The boolean indicate which operation need to be done.
	 *  ! Boolean: scalar, add, constant, error
	 */
	Affine2& saxpy(double alpha, const Affine2& y, double beta, double delta,
			bool B1, bool B2, bool B3, bool B4);

public:

	typedef enum {
		AF_SQRT,
		AF_EXP,
		AF_LOG,
		AF_INV,
		AF_COS,
		AF_SIN,
		AF_TAN,
		AF_ABS,
		AF_ACOS,
		AF_ASIN,
		AF_ATAN,
		AF_COSH,
		AF_SINH,
		AF_TANH
	} affine2_expr; // ...etc...

	/** \brief Create an empty affine form. */
	Affine2();

	/** \brief Create an affine form with n variables, initialized with  [-oo,+oo]. */
	explicit Affine2(int n);

	/** \brief Create an affine form with n variables and  initialized val[0] with d. */
	Affine2(int n, const double d);

	/** \brief Create an affine form with n variables and  initialized val[0] with  itv. */
	Affine2(int n, const Interval& itv);

	/** \brief Create an affine form with n variables and  initialized the m^th variable with  itv. */
	Affine2(int n, int m, const Interval& itv);


	/** \brief Create an affine form with n variables, initialized with x. */
	Affine2(const Affine2& x);

	/** \brief Create an affine form with n variables, initialized with -x if (b) elseif (!b) with x. */
	Affine2(const Affine2& x, bool b);

	/** \brief  Delete the affine form */
	~Affine2();

	/**
	 *\brief compute the min-range linearization of an unary operator
	 */
	Affine2& linMinRange(affine2_expr num);
	Affine2& linMinRange(affine2_expr num, const Interval itv);

	/**
	 *\brief compute the chebyshev linearization of an unary operator
	 */
	Affine2& linChebyshev(affine2_expr num);
	Affine2& linChebyshev(affine2_expr num, const Interval itv);

	/** \brief Return sqr(*this) */
	Affine2& sqr(const Interval itv);

	/** \brief Return pow(*this,n) */
	Affine2& power(int n, const Interval itv);

//operator

	/** \brief True iff *this and x are exactly the same intervals. */
	 bool operator==(const Affine2& x) const;

	/** \brief True iff *this and x are exactly the same intervals. */
	 bool operator==(const Interval& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	 bool operator!=(const Affine2& x) const;

	/** \brief True iff *this and x are not exactly the same intervals. */
	 bool operator!=(const Interval& x) const ;

	/** \brief Set this interval to the empty set. */
	 void set_empty();

	/** \brief Set *this to x.
	 */
	Affine2& operator=(const Affine2& x);

	/** \brief Set *this to d.
	 */
	Affine2& operator=(double x);

	/** \brief Set *this to itv.
	 */
	Affine2& operator=(const Interval& itv);

/* Union and Intersection of two affine2 form must not be implemented
 * That could produce to much confusion.
 */
	/** \brief Intersection of *this and x.
	 * \param x - the interval to compute the intersection with.*/
//	Affine2& operator&=(const Affine2& x);
	/** \brief Union of *this and I.
	 * \param x - the interval to compute the hull with.*/
//	Affine2& operator|=(const Affine2& x);

	/**
	 * \brief Add [-rad,+rad] to *this.
	 *
	 * Return a reference to *this.
	 */
	 Affine2& inflate(double radd);

	/**
	 * \brief Change the number
	 */
	void resize(int n);

	/**
	 * \brief number of variable represented
	 */
	 int size() const;

	/**
	 * \brief Range of the affine form
	 */
	 const Interval itv() const ;

	/**
	 * \brief return _val
	 */
	 double * val() const;
	/**
	 * \brief return _val[i]
	 */
	 double val(int i) const;

	/**
	 * \brief return _err
	 */
	 const Interval err() const;

	/**
	 * \brief return 1 if the affine form is actif and valid
	 */
	 bool is_actif() const;

	/**
	 * \brief True iff *this is empty.
	 */
	 bool is_empty() const;

	/**
	 * \brief True if one bound of *this is infinite.
	 *
	 * \note An empty affine form is always bounded.
	 */
	 bool is_unbounded() const;

	/** \brief Add \a d to *this and return the result.  */
	 Affine2& operator+=(double d);

	/** \brief Subtract \a d to *this and return the result. */
	 Affine2& operator-=(double d);

	/** \brief Multiply *this by \a d and return the result. */
	 Affine2& operator*=(double d);

	/** \brief Divide *this by \a d and return the result. */
	 	Affine2& operator/=(double d) ;

	/** \brief Add \a x to *this and return the result. */
	 Affine2& operator+=(const Interval& x);

	/** \brief Subtract \a x to *this and return the result. */
	 Affine2& operator-=(const Interval& x);

	/** \brief Multiply *this by \a x and return the result. */
	 Affine2& operator*=(const Interval& x);

	/** \brief Divide *this by \a x and return the result.*/
	 Affine2& operator/=(const Interval& x);

	/** \brief Add \a x to *this and return the result. */
	 Affine2& operator+=(const Affine2& x);

	/** \brief Subtract \a x to *this and return the result. */
	 Affine2& operator-=(const Affine2& x);

	/** \brief Multiply *this by \a x and return the result. */
	Affine2& operator*=(const Affine2& x);

	/** \brief Divide *this by \a x and return the result. */
	 Affine2& operator/=(const Affine2& x);

};

/** \ingroup arithmetic */
/*@{*/

/** \brief Stream out \a x. */
std::ostream& operator<<(std::ostream& os, const Affine2& x);

/** \brief Return -x. */
 Affine2 operator-(const Affine2& x);

/** \brief $AF[x]_1+AF[x]_2$. */
 Affine2 operator+(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]+d$. */
 Affine2 operator+(const Affine2& x, double d);

/** \brief $d+AF[x]$. */
 Affine2 operator+(double d, const Affine2& x);

/** \brief $AF[x]_1+[x]_2$. */
 Affine2 operator+(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1+AF[x]_2$. */
 Affine2 operator+(const Interval& x1, const Affine2& x2);

/** \brief $AF[x]_1-AF[x]_2$. */
 Affine2 operator-(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]-d$. */
 Affine2 operator-(const Affine2& x, double d);

/** \brief $d-AF[x]$. */
 Affine2 operator-(double d, const Affine2& x);

/** \brief $AF[x]_1-[x]_2$. */
 Affine2 operator-(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1-AF[x]_2$. */
 Affine2 operator-(const Interval& x1, const Affine2& x2);

/** \brief $AF[x]_1*AF[x]_2$. */
 Affine2 operator*(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]*d$. */
 Affine2 operator*(const Affine2& x, double d);

/** \brief $d*AF[x]$. */
 Affine2 operator*(double d, const Affine2& x);

/** \brief $AF[x]_1*[x]_2$. */
 Affine2 operator*(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1*AF[x]_2$. */
 Affine2 operator*(const Interval& x1, const Affine2& x2);

/** \brief $AF[x]_1/AF[x]_2$. */
 Affine2 operator/(const Affine2& x1, const Affine2& x2);

/** \brief $AF[x]/d$. */
 Affine2 operator/(const Affine2& x, double d);

/** \brief $d/AF[x]$. */
 Affine2 operator/(double d, const Affine2& x);

/** \brief $AF[x]_1/[x]_2$. */
 Affine2 operator/(const Affine2& x1, const Interval& x2);

/** \brief $[x]_1/AF[x]_2$. */
 Affine2 operator/(const Interval& x1, const Affine2& x2);

/** \brief $1/AF[x]$ */
 Affine2 inv(const Affine2& x);
 Affine2 inv(const Affine2& x, const Interval itv);

/** \brief $AF[x]^2$ */
 Affine2 sqr(const Affine2& x);
 Affine2 sqr(const Affine2& x, const Interval itv);

/** \brief $\sqrt{AF[x]}$ */
 Affine2 sqrt(const Affine2& x);
 Affine2 sqrt(const Affine2& x, const Interval itv);

/** \brief $\exp(AF[x])$. */
 Affine2 exp(const Affine2& x);
 Affine2 exp(const Affine2& x, const Interval itv);

/** \brief $\log(AF[x])$. */
 Affine2 log(const Affine2& x);
 Affine2 log(const Affine2& x, const Interval itv);

/** \brief $AF[x]^n$. */
Affine2 pow(const Affine2& x, int n);
Affine2 pow(const Affine2& x, int n, const Interval itv);

/** \brief $AF[x]^d$. */
Affine2 pow(const Affine2& x, double d);
Affine2 pow(const Affine2& x, double d, const Interval itv);

/** \brief $AF[x]^[y]$. */
 Affine2 pow(const Affine2 &x, const Interval &y);
 Affine2 pow(const Affine2 &x, const Interval &y, const Interval itvx);

/** \brief $AF[x]^AF[y]$. */
 Affine2 pow(const Affine2 &x, const Affine2 &y);

/** \brief $n^{th}$ root of $AF[x]$. */
Affine2 root(const Affine2& x, int n);
Affine2 root(const Affine2& x, int n, const Interval itv);

/** \brief $\cos(AF[x])$. */
 Affine2 cos(const Affine2& x);
 Affine2 cos(const Affine2& x, const Interval itv);

/** \brief $\sin(AF[x])$. */
 Affine2 sin(const Affine2& x);
 Affine2 sin(const Affine2& x, const Interval itv);

/** \brief $\tan(AF[x])$. */
 Affine2 tan(const Affine2& x);
 Affine2 tan(const Affine2& x, const Interval itv);

/** \brief $\acos(AF[x])$. */
 Affine2 acos(const Affine2& x);
 Affine2 acos(const Affine2& x, const Interval itv);

/** \brief $\asin(AF[x])$. */
 Affine2 asin(const Affine2& x);
 Affine2 asin(const Affine2& x, const Interval itv);

/** \brief $\atan(AF[x])$. */
 Affine2 atan(const Affine2& x);
 Affine2 atan(const Affine2& x, const Interval itv);

 /** \brief $\cosh(AF[x])$. */
 Affine2 cosh(const Affine2& x);
 Affine2 cosh(const Affine2& x, const Interval itv);

/** \brief $\sinh(AF[x])$. */
 Affine2 sinh(const Affine2& x);
 Affine2 sinh(const Affine2& x, const Interval itv);

/** \brief $\tanh(AF[x])$. */
 Affine2 tanh(const Affine2& x);
 Affine2 tanh(const Affine2& x, const Interval itv);

/** \brief $\abs(AF[x]) = sqrt(sqr(AF[x]))$. */
 Affine2 abs(const Affine2 &x);
 Affine2 abs(const Affine2 &x, const Interval itv);


/**
 * \brief Return the largest integer interval included in x.
 *  TODO  not yet implemented integer
 */
 Affine2 integer(const Affine2& x);

/** \brief Sign of $AF[x]$.
 *
 *  Return \f$sign(AF[x]) = hull \{ sign{x}, x\inAF[x] \}\f$.
 * \remark By convention, \f$ 0\inAF[x] \Longrightarrow sign(AF[x])=[-1,1]\f$. */
Affine2 sign(const Affine2& x);
Affine2 sign(const Affine2& x, const Interval itv);


}

/*@}*/

/*============================================ inline implementation ============================================ */



namespace ibex {

inline Affine2::~Affine2() {
	if (_val!=NULL) delete[] _val;
}

inline Affine2& Affine2::linMinRange(affine2_expr num) {
	return linMinRange(num,this->itv());
}

inline Affine2& Affine2::linChebyshev(affine2_expr num) {
	return linChebyshev(num,this->itv());
}


inline Interval& Interval::operator&=(const Affine2& x) {
	return (*this &= x.itv());
}

inline Interval& Interval::operator|=(const Affine2& x) {
	return (*this |= x.itv());
}

inline Interval& Interval::operator=(const Affine2& x) {
	return (*this = x.itv());
}

inline Interval operator&(const Affine2& x1, const Affine2& x2) {
	return ((x1.itv())&(x2.itv()));
}

inline Interval operator&(const Interval& x1, const Affine2& x2) {
	return ((x1)&(x2.itv()));
}

inline Interval operator&(const Affine2& x1, const Interval& x2) {
	return ((x1.itv())&(x2));
}

inline Interval operator|(const Affine2& x1, const Affine2& x2) {
	return ((x1.itv())|(x2.itv()));
}

inline Interval operator|(const Interval& x1, const Affine2& x2) {
	return ((x1)|(x2.itv()));
}

inline Interval operator|(const Affine2& x1, const Interval& x2) {
	return ((x1.itv())|(x2));
}


/** \brief True iff *this and x are exactly the same intervals. */
inline bool Affine2::operator==(const Affine2& x) const{
	return (this->itv() == x.itv());
}

/** \brief True iff *this and x are exactly the same intervals. */
inline bool Affine2::operator==(const Interval& x) const{
	return (this->itv() == x);
}

/** \brief True iff *this and x are not exactly the same intervals. */
inline bool Affine2::operator!=(const Affine2& x) const{
	return (this->itv() == x.itv());
}

/** \brief True iff *this and x are not exactly the same intervals. */
inline bool Affine2::operator!=(const Interval& x) const {
	return (this->itv() != x);
}

/** \brief Set this interval to the empty set. */
inline void Affine2::set_empty(){
	_err.set_empty();
}

inline Affine2& Affine2::inflate(double radd){
	_err += radd;
	return *this;
}

/** \brief number of variable represented */
inline int Affine2::size() const{
	return _n;
}


/** \brief return _val*/
inline double * Affine2::val() const{
	return _val;
}

/** \brief return _val[i] */
inline double Affine2::val(int i) const{
	assert(i>=0 && i<=_n);
	return _val[i];
}

/** \brief return _err*/
inline const Interval Affine2::err() const{
	return _err;
}

inline bool Affine2::is_actif() const{
	return (_n>-1);
}

inline bool Affine2::is_empty() const{
	return (_err.is_empty());
}

inline bool Affine2::is_unbounded() const{
	return (_err.is_unbounded());
}


/** \brief Add \a d to *this and return the result.  */
inline Affine2& Affine2::operator+=(double d){
	return saxpy(1.0, Affine2(0), d, 0.0, false, false, true, false);
}

/** \brief Subtract \a d to *this and return the result. */
inline Affine2& Affine2::operator-=(double d){
	return saxpy(1.0, Affine2(0), (-d), 0.0, false, false, true, false);
}

/** \brief Multiply *this by \a d and return the result. */
inline Affine2& Affine2::operator*=(double d){
	return saxpy(d, Affine2(0), 0.0, 0.0, true, false, false, false);
}

/** \brief Divide *this by \a d and return the result. */
inline 	Affine2& Affine2::operator/=(double d) {
	return *this *= (1.0 / Interval(d)) ;
}

/** \brief Add \a x to *this and return the result. */
inline Affine2& Affine2::operator+=(const Interval& x){
	return saxpy(1.0, Affine2(0), x.mid(), x.rad(), false, false, true, true);
}

/** \brief Subtract \a x to *this and return the result. */
inline Affine2& Affine2::operator-=(const Interval& x){
	return *this += (-x);
}


/** \brief Divide *this by \a x and return the result.*/
inline Affine2& Affine2::operator/=(const Interval& x){
	return *this *= (1.0/x) ;
}

/** \brief Add \a x to *this and return the result. */
inline Affine2& Affine2::operator+=(const Affine2& x){
	return saxpy(1.0, x, 0.0, 0.0, false, true, false, false);
}

/** \brief Subtract \a x to *this and return the result. */
inline Affine2& Affine2::operator-=(const Affine2& x){
	return *this += Affine2(x,true);
}

inline Affine2& Affine2::operator/=(const Affine2& x){
	return *this *= (Affine2(x).linChebyshev(Affine2::AF_INV));
}

/** \brief Return -x. */
inline Affine2 operator-(const Affine2& x){
	return Affine2(x, true); // y = -x
}

/** \brief $AF[x]_1+AF[x]_2$. */
inline Affine2 operator+(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) += x2;
}

/** \brief $AF[x]+d$. */
inline Affine2 operator+(const Affine2& x, double d){
	return Affine2(x) += d;
}

/** \brief $d+AF[x]$. */
inline Affine2 operator+(double d, const Affine2& x){
	return Affine2(x) += d;
}

/** \brief $AF[x]_1+[x]_2$. */
inline Affine2 operator+(const Affine2& x1, const Interval& x2){
	return Affine2(x1) += x2;
}

/** \brief $[x]_1+AF[x]_2$. */
inline Affine2 operator+(const Interval& x1, const Affine2& x2){
	return Affine2(x2) += x1;
}

/** \brief $AF[x]_1-AF[x]_2$. */
inline Affine2 operator-(const Affine2& x1, const Affine2& x2){
	return Affine2(x2, true) += x1;
}

/** \brief $AF[x]-d$. */
inline Affine2 operator-(const Affine2& x, double d){
	return Affine2(x) -= d;
}

/** \brief $d-AF[x]$. */
inline Affine2 operator-(double d, const Affine2& x){
	return Affine2(x, true) += d;
}

/** \brief $AF[x]_1-[x]_2$. */
inline Affine2 operator-(const Affine2& x1, const Interval& x2) {
	return Affine2(x1) -= x2;
}

/** \brief $[x]_1-AF[x]_2$. */
inline Affine2 operator-(const Interval& x1, const Affine2& x2) {
	return Affine2(x2, true) += x1;
}

/** \brief $AF[x]_1*AF[x]_2$. */
inline Affine2 operator*(const Affine2& x1, const Affine2& x2) {
	return Affine2(x1) *= x2;
}

/** \brief $AF[x]*d$. */
inline Affine2 operator*(const Affine2& x, double d){
	return Affine2(x) *= d;
}

/** \brief $d*AF[x]$. */
inline Affine2 operator*(double d, const Affine2& x){
	return Affine2(x) *= d;
}

/** \brief $AF[x]_1*[x]_2$. */
inline Affine2 operator*(const Affine2& x1, const Interval& x2){
	return Affine2(x1) *= x2;
}

/** \brief $[x]_1*AF[x]_2$. */
inline Affine2 operator*(const Interval& x1, const Affine2& x2){
	return Affine2(x2) *= x1;
}

/** \brief $AF[x]_1/AF[x]_2$. */
inline Affine2 operator/(const Affine2& x1, const Affine2& x2){
	return Affine2(x1) /= x2;
}

/** \brief $AF[x]/d$. */
inline Affine2 operator/(const Affine2& x, double d){
	return Affine2(x) /= d;
}

/** \brief $d/AF[x]$. */
inline Affine2 operator/(double d, const Affine2& x){
	return Affine2(x.size(), d) *= (Affine2(x).linChebyshev(Affine2::AF_INV));
}

/** \brief $AF[x]_1/[x]_2$. */
inline Affine2 operator/(const Affine2& x1, const Interval& x2){
	return Affine2(x1) /= x2;
}

/** \brief $[x]_1/AF[x]_2$. */
inline Affine2 operator/(const Interval& x1, const Affine2& x2){
	return Affine2(x2.size(), x1) *= (Affine2(x2).linChebyshev(Affine2::AF_INV));
}

/** \brief $1/AF[x]$ */
inline Affine2 inv(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_INV);
}

inline Affine2 inv(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_INV, itv);
}

/** \brief $AF[x]^2$ */
inline Affine2 sqr(const Affine2& x){
	return Affine2(x).sqr(x.itv());
}
inline Affine2 sqr(const Affine2& x, const Interval itv){
	return Affine2(x).sqr(itv);
}

/** \brief $\sqrt{AF[x]}$ */
inline Affine2 sqrt(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_SQRT);
}

inline Affine2 sqrt(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_SQRT);
}

/** \brief $\exp(AF[x])$. */
inline Affine2 exp(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_EXP);
}

inline Affine2 exp(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_EXP);
}

/** \brief $\log(AF[x])$. */
inline Affine2 log(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_LOG);
}

inline Affine2 log(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_LOG);
}

/** \brief $AF[x]^n$. */
inline Affine2 pow(const Affine2& x, int n) {
	return pow(x,n,x.itv());
}

/** \brief $AF[x]^d$. */
inline Affine2 pow(const Affine2& x, double d){
	return pow(x,d,x.itv());
}

/** \brief $AF[x]^[y]$. */
inline Affine2 pow(const Affine2 &x, const Interval &y){
	// return exp(y * log(x));
	return (y * Affine2(x).linChebyshev(Affine2::AF_LOG)).linChebyshev(Affine2::AF_EXP);
}

inline Affine2 pow(const Affine2 &x, const Interval &y, const Interval itvx){
	// return exp(y * log(x));
	return (y * Affine2(x).linChebyshev(Affine2::AF_LOG, itvx)).linChebyshev(Affine2::AF_EXP, y*log(itvx));
}

/** \brief $AF[x]^AF[y]$. */
inline Affine2 pow(const Affine2 &x, const Affine2 &y){
	return pow(x, y.itv());
}


/** \brief $n^{th}$ root of $AF[x]$. */
inline Affine2 root(const Affine2& x, int n) {
	return root(x,n,x.itv());
}

/** \brief $\cos(AF[x])$. */
inline Affine2 cos(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_COS);
}

inline Affine2 cos(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_COS, itv);
}

/** \brief $\sin(AF[x])$. */
inline Affine2 sin(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_SIN);
}

inline Affine2 sin(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_SIN);
}


/** \brief $\tan(AF[x])$. */
inline Affine2 tan(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_TAN);
}

inline Affine2 tan(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_TAN);
}

/** \brief $\acos(AF[x])$. */
inline Affine2 acos(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_ACOS);
}

inline Affine2 acos(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ACOS, itv);
}

/** \brief $\asin(AF[x])$. */
inline Affine2 asin(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_ASIN);
}

inline Affine2 asin(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ASIN, itv);
}

/** \brief $\atan(AF[x])$. */
inline Affine2 atan(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_ATAN);
}

inline Affine2 atan(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ATAN, itv);
}

/** \brief $\cosh(AF[x])$. */
inline Affine2 cosh(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_COSH);
}

inline Affine2 cosh(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_COSH, itv);
}

/** \brief $\sinh(AF[x])$. */
inline Affine2 sinh(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_SINH);
}

inline Affine2 sinh(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_SINH, itv);
}

/** \brief $\tanh(AF[x])$. */
inline Affine2 tanh(const Affine2& x){
	return Affine2(x).linChebyshev(Affine2::AF_TANH);
}

inline Affine2 tanh(const Affine2& x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_TANH, itv);
}

/** \brief $\abs(AF[x]) = sqrt(sqr(AF[x]))$. */
inline Affine2 abs(const Affine2 &x){
	return Affine2(x).linChebyshev(Affine2::AF_ABS);
}

inline Affine2 abs(const Affine2 &x, const Interval itv){
	return Affine2(x).linChebyshev(Affine2::AF_ABS, itv);
}


inline Affine2 integer(const Affine2& x){
	// TODO not yet implemented
	return Affine2(x);
}

inline Affine2 sign(const Affine2& x) {
	return sign(x, x.itv());
}

} // end namespace ibex

#endif /* IBEX_AFFINE2_H_ */

// TODO to finish the projections and other functions

/** \brief $\atan2(AF[x],AF[y])$. */
//Affine2 atan2(const Affine2& x, const Affine2& y);
/** \brief $\atan2([x],AF[y])$. */
//Affine2 atan2(const Interval& x, const Affine2& y);
/** \brief $\atan2(AF[x],[y])$. */
//Affine2 atan2(const Affine2& x, const Interval& y);
/** \brief $\cosh(AF[x])$. */
