//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprDiff.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 25, 2013
// Last Update : Feb 25, 2013
//============================================================================

#ifndef __IBEX_EXPR_DIFF_H__
#define __IBEX_EXPR_DIFF_H__

#include "ibex_ExprVisitor.h"
#include "ibex_NodeMap.h"
#include "ibex_Function.h"

namespace ibex {

/**
 * \brief Differentiate an expression.
 *
 */
class ExprDiff : public virtual ExprVisitor {
public:

	const ExprNode& diff(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y);

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

	void add_grad_expr(const ExprNode& node, const ExprNode& expr);

	NodeMap<const ExprNode*> grad;
};

} // end namespace ibex
#endif // IBEX_EXPR_DIFF_H_
