//============================================================================
//                                  I B E e
// File        : ibex_ExprDiff.cpp
// Author      : Gilles Chabert
// Diffright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 25, 2013
// Last Update : Feb 25, 2013
//============================================================================

#include "ibex_ExprDiff.h"
#include "ibex_ExprCopy.h"
#include "ibex_ExprNodes.h"
#include "ibex_Expr.h"
#include "ibex_NodeSet.h"

using namespace std;

namespace ibex {

#define ONE          ExprConstant::new_scalar(1.0)

void ExprDiff::add_grad_expr(const ExprNode& node, const ExprNode& _expr_) {

	if (grad.found(node))
		grad[node]= & (*grad[node] +_expr_);
	else
		// not found means "zero"
		grad.insert(node, &_expr_);
}

const ExprNode& ExprDiff::diff(const Array<const ExprSymbol>& old_x, const Array<const ExprSymbol>& new_x, const ExprNode& y) {

	grad.clean();

	SubNodes nodes(y);

	int n=y.size;
	int nb_var=old_x.size();

	add_grad_expr(nodes[0],ONE);

	// visit nodes in topological order
	 for (int i=0; i<n; i++) {
		visit(nodes[i]);
	}

	Array<const ExprNode> dX(nb_var);

	for (int i=0; i<nb_var; i++) {
		if (!grad.found(old_x[i])) { // this symbol does not appear in the expression -> null derivative
			leaves.push_back(&old_x[i]);
			Domain d(old_x[i].dim);
			d.clear();
			grad.insert(old_x[i], &ExprConstant::new_(d));
		}
		dX.set_ref(i,*grad[old_x[i]]);
	}

	const ExprNode& df=ExprVector::new_(dX,false);

	// Note: it is better to proceed in this way: (1) differentiate
	// and (2) copy the expression for two reasons
	// 1-we can eliminate the constant expressions such as (1*1)
	//   generated by the differentiation
	// 2-the "dead" branches corresponding to the partial derivative
	//   w.r.t. ExprConstant leaves will are deleted properly (if
	//   we had proceeded in the other way around, there would be
	//   memory leaks).

	const ExprNode& result=ExprCopy().copy(old_x,new_x,df,true);

	// ------------------------- CLEANUP -------------------------
	// cleanup(df,true); // don't! some nodes are shared with y

	// don't! some grad are references to nodes of y!
	//	for (int i=0; i<n; i++)
	//	  delete grad[*nodes[i]];

	// we build the vector of the partial derivatives
	// wrt all the leaves, including constants.
	Array<const ExprNode> _dAll(leaves.size());

	for (unsigned int i=0; i<leaves.size(); i++) {
		_dAll.set_ref(i,*grad[*leaves[i]]);
	}

	// build the global DAG
	const ExprNode* dAll=&ExprVector::new_(_dAll,false);

	SubNodes gnodes(*dAll);
	int k=dAll->size;
	for (int j=0; j<k; j++) {
		if (!nodes.found(gnodes[j])) { // if the node is not in the original expression
			//cout << "not found:" << *gnodes[j] << endl;
			delete &gnodes[j];      // delete it.
		}
	}

	delete &df;

	return result;
}

void ExprDiff::visit(const ExprNode& e) {
	e.acceptVisitor(*this);
}

void ExprDiff::visit(const ExprIndex& i) {
	not_implemented("diff with index");
}

void ExprDiff::visit(const ExprSymbol& x) {
	leaves.push_back(&x);
}

void ExprDiff::visit(const ExprConstant& c) {
	leaves.push_back(&c);
}

// (useless so far)
void ExprDiff::visit(const ExprNAryOp& e) {
	assert(false);
}

void ExprDiff::visit(const ExprLeaf& e) {
	assert(false);
}

// (useless so far)
void ExprDiff::visit(const ExprBinaryOp& b) {
	assert(false);
}

// (useless so far)
void ExprDiff::visit(const ExprUnaryOp& u) {
	assert(false);
}


void ExprDiff::visit(const ExprVector& e) {

	for (int i=0; i<e.nb_args; i++) {
		add_grad_expr(e.arg(i), ((const ExprVector*) grad[e])->get(i));
	}
}

void ExprDiff::visit(const ExprApply& e) {

	//TODO
	not_implemented("diff with apply");
}

void ExprDiff::visit(const ExprAdd& e)   { add_grad_expr(e.left,  *grad[e]);
                                           add_grad_expr(e.right, *grad[e]); }
void ExprDiff::visit(const ExprMul& e)   { add_grad_expr(e.left,  e.right * (*grad[e]));
                                           add_grad_expr(e.right, e.left * (*grad[e])); }
void ExprDiff::visit(const ExprSub& e)   { add_grad_expr(e.left,  *grad[e]);
										   add_grad_expr(e.right, -*grad[e]); }
void ExprDiff::visit(const ExprDiv& e)   { add_grad_expr(e.left,  *grad[e]/e.right);
		                                   add_grad_expr(e.right, -(e.left*(*grad[e])/sqr(e.right))); }
void ExprDiff::visit(const ExprMax& e)   { not_implemented("diff with max");; }
void ExprDiff::visit(const ExprMin& e)   { not_implemented("diff with min"); }
void ExprDiff::visit(const ExprAtan2& e) { not_implemented("diff with atan2"); }


void ExprDiff::visit(const ExprPower& e) {
	add_grad_expr(e.expr,Interval(e.expon)*pow(e.expr,e.expon-1)*(*grad[e]));
}

void ExprDiff::visit(const ExprMinus& e) { add_grad_expr(e.expr, *grad[e]); }
void ExprDiff::visit(const ExprTrans& e) { not_implemented("diff with transpose"); }
void ExprDiff::visit(const ExprSign& e)  { not_implemented("diff with sign"); }
void ExprDiff::visit(const ExprAbs& e)   { not_implemented("diff with abs"); }
void ExprDiff::visit(const ExprSqr& e)   { add_grad_expr(e.expr, (*grad[e])*Interval(2.0)*e.expr); }
void ExprDiff::visit(const ExprSqrt& e)  { add_grad_expr(e.expr, (*grad[e])*Interval(0.5)/sqrt(e.expr)); }
void ExprDiff::visit(const ExprExp& e)   { add_grad_expr(e.expr, (*grad[e])*exp(e.expr)); }
void ExprDiff::visit(const ExprLog& e)   { add_grad_expr(e.expr, (*grad[e])/e.expr ); }
void ExprDiff::visit(const ExprCos& e)   { add_grad_expr(e.expr,-(*grad[e])*sin(e.expr) ); }
void ExprDiff::visit(const ExprSin& e)   { add_grad_expr(e.expr, (*grad[e])*cos(e.expr) ); }
void ExprDiff::visit(const ExprTan& e)   { add_grad_expr(e.expr, (*grad[e])*(ONE+sqr(tan(e.expr)))); }
void ExprDiff::visit(const ExprCosh& e)  { add_grad_expr(e.expr, (*grad[e])*sinh(e.expr)); }
void ExprDiff::visit(const ExprSinh& e)  { add_grad_expr(e.expr, (*grad[e])*cosh(e.expr)); }
void ExprDiff::visit(const ExprTanh& e)  { add_grad_expr(e.expr, (*grad[e])*(ONE - sqr(tanh(e.expr)))); }
void ExprDiff::visit(const ExprAcos& e)  { add_grad_expr(e.expr,-(*grad[e])/sqrt(1.0-sqr(e.expr))); }
void ExprDiff::visit(const ExprAsin& e)  { add_grad_expr(e.expr, (*grad[e])/sqrt(1.0-sqr(e.expr))); }
void ExprDiff::visit(const ExprAtan& e)  { add_grad_expr(e.expr, (*grad[e])/(1.0+sqr(e.expr))); }
void ExprDiff::visit(const ExprAcosh& e) { add_grad_expr(e.expr, (*grad[e])/sqrt(sqr(e.expr) -1.0)); }
void ExprDiff::visit(const ExprAsinh& e) { add_grad_expr(e.expr, (*grad[e])/sqrt(1.0+sqr(e.expr))); }
void ExprDiff::visit(const ExprAtanh& e) { add_grad_expr(e.expr, (*grad[e])/(1.0-sqr(e.expr))); }

} // end namespace ibex
