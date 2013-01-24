//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprCopy.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 19, 2012
// Last Update : Jun 19, 2012
//============================================================================


#ifndef __IBEX_EXPR_COPY_H__
#define __IBEX_EXPR_COPY_H__

#include "ibex_ExprVisitor.h"
#include "ibex_Array.h"
#include "ibex_Domain.h"

namespace ibex {


/**
 * \brief Deep copy of an array of variables.
 *
 * As for ExprCopy, the size of \a dest may be greater than \a src (but not smaller).
 *
 * Useful to duplicate functions, via #Copy.
 */
void varcopy(const Array<const ExprSymbol>& src, Array<const ExprSymbol>& dest);



/**
 * \brief Duplicate an expression
 */
class ExprCopy : public virtual ExprVisitor {

public:
	/*
	 * \brief Duplicate an expression (with new symbols).
	 *
	 * \pre Each symbol in y must belong to "old_x".
	 *
	 * Symbols in \a old_x are matched to symbols in \a new_x with respect to their order.
	 *
	 * \param fold_cst - if true, all constant subexpressions are "folded" into a single node.
	 *
	 * \pre The size of \a new_x must be greater or equal to the size of \a old_x. It is not
	 *      required to be the same size to allow the use of extra variables (that do not occurr in the expression).
	 *      This is used, e.g., in ibex_Optimizer to transform a function x->g(x) into (x,y)->g(x).
	 */
	const ExprNode& copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, bool fold_cst=false);

	/**
	 * \brief Duplicate y[i], where y is an expression.
	 *
	 * If \a y is a vector, the ith argument of y is copied, i.e., copy-of(y[i])
	 * instead of returning (copy-of(y))[i].
	 */
	const ExprNode& index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, int i, bool fold_cst=false);

	/**
	 * \brief Duplicate y[i][j], where y is an expression.
	 *
	 * If \a y is a matrix, a copy of the (i,j) entry of y is returned, i.e., copy-of(y[i][j]).
	 * Else if \a y is a vector, a copy-of(y[i])[j] is returned.
	 * Otherwise, the returned expression is (copy-of(y))[i][j].
	 */
	const ExprNode& index_copy(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y, int i, int j, bool fold_cst=false);

protected:
	void visit(const ExprNode& e);
	void visit(const ExprIndex& i);
	void visit(const ExprNAryOp& e);
	void visit(const ExprLeaf& e);
	void visit(const ExprBinaryOp& b);
	void visit(const ExprUnaryOp& u);
	void visit(const ExprSymbol& x);
	void visit(const ExprConstant& c);
	void visit(const ExprVector& e);
	void visit(const ExprApply& e);
	void visit(const ExprAdd& e);
	void visit(const ExprMul& e);
	void visit(const ExprSub& e);
	void visit(const ExprDiv& e);
	void visit(const ExprMax& e);
	void visit(const ExprMin& e);
	void visit(const ExprAtan2& e);
	void visit(const ExprMinus& e);
	void visit(const ExprTrans& e);
	void visit(const ExprSign& e);
	void visit(const ExprAbs& e);
	void visit(const ExprPower& e);
	void visit(const ExprSqr& e);
	void visit(const ExprSqrt& e);
	void visit(const ExprExp& e);
	void visit(const ExprLog& e);
	void visit(const ExprCos& e);
	void visit(const ExprSin& e);
	void visit(const ExprTan& e);
	void visit(const ExprCosh& e);
	void visit(const ExprSinh& e);
	void visit(const ExprTanh& e);
	void visit(const ExprAcos& e);
	void visit(const ExprAsin& e);
	void visit(const ExprAtan& e);
	void visit(const ExprAcosh& e);
	void visit(const ExprAsinh& e);
	void visit(const ExprAtanh& e);

	bool fold;

};

} // end namespace ibex

#endif // __IBEX_EXPR_COPY_H__
