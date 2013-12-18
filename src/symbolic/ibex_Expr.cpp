/* ============================================================================
 * I B E X - Expressions
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 5, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Expr.h"
#include "ibex_DimException.h"
#include "ibex_Function.h"
#include "ibex_ExprPrinter.h"
#include "ibex_ExprSubNodes.h"
#include "ibex_ExprSize.h"
#include "ibex_String.h"
#include <sstream>
#include <limits.h>
#include <stdio.h>
#include <set>

using namespace std;

namespace ibex {

namespace {

int id_count=0;

int max_height(const ExprNode& n1, const ExprNode& n2) {
	if (n1.height>n2.height) return n1.height;
	else return n2.height;
}

int max_height(const ExprNode** args, int n) {
	int max=0;
	for (int i=0; i<n; i++)
		if (args[i]->height > max) max = args[i]->height;
	return max;
}

} // end anonymous namespace

ExprNode::ExprNode(int height, int size, const Dim& dim) :
  height(height), size(size), id(id_count++), dim(dim) {

}

void cleanup(const Array<const ExprNode>& expr, bool delete_symbols) {
	ExprSubNodes nodes(expr);
	for (int i=0; i<nodes.size(); i++)
		if (delete_symbols || (!dynamic_cast<const ExprSymbol*>(&nodes[i])))
			delete (ExprNode*) &nodes[i];
}

ExprIndex::ExprIndex(const ExprNode& subexpr, int index)
: ExprNode(subexpr.height+1, subexpr.size+1, subexpr.dim.index_dim()), expr(subexpr), index(index) {
	if (index<0 || index>subexpr.dim.max_index())
		throw DimException("index out of bounds");
	((ExprNode&) (subexpr)).fathers.add(*this);
}

bool ExprIndex::indexed_symbol() const {
	// we prefer to use directly a dynamic cast here
	// instead of creating a visitor class (or adding a new field in ExprNode)
	if (dynamic_cast<const ExprSymbol*>(&expr)) return true;

	const ExprIndex* expr_index=dynamic_cast<const ExprIndex*>(&expr);
	if (!expr_index) return false;

	return expr_index->indexed_symbol();
}

ExprNAryOp::ExprNAryOp(const ExprNode** _args, int n, const Dim& dim) :
		ExprNode(max_height(_args,n)+1, nary_size(_args,n), dim), nb_args(n) {

	args = new const ExprNode*[n];
	for (int i=0; i<n; i++) {
		args[i]=_args[i];
		((ExprNode&) *args[i]).fathers.add(*this);
	}
}

ExprNAryOp::~ExprNAryOp() {
	delete[] args;
}

static Array<const Dim> dims(const ExprNode** comp, int n) {
	Array<const Dim> a(n);
	for (int i=0; i<n; i++) a.set_ref(i,comp[i]->dim);
	return a;
}

const ExprVector& ExprVector::new_(const ExprNode** comp, int n, bool in_row) {
	return *new ExprVector(comp,n,in_row);
}

const ExprVector& ExprVector::new_(const ExprNode& e1, const ExprNode& e2, bool in_row) {
	const ExprNode** comp=new const ExprNode*[2];
	comp[0]=&e1;
	comp[1]=&e2;
	ExprVector* res=new ExprVector(comp, 2, in_row);
	delete[] comp;
	return *res;
}

const ExprVector& ExprVector::new_(const Array<const ExprNode>& components, bool in_rows) {
	int n=components.size();
	const ExprNode** nodes=new const ExprNode*[n];
	for (int i=0; i<n; i++)
		nodes[i]=&components[i];
	ExprVector* res=new ExprVector(nodes,n,in_rows);
	delete[] nodes;
	return *res;
}

ExprVector::ExprVector(const ExprNode** comp, int n, bool in_row) :
		ExprNAryOp(comp, n, vec_dim(dims(comp,n),in_row)) {
}

const ExprChi& ExprChi::new_(const ExprNode** args) {
	if (!(args[0]->type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(args[1]->type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(args[2]->type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	return *new ExprChi(args);
}

const ExprChi& ExprChi::new_(const ExprNode& a, const ExprNode& b, const ExprNode& c) {
	if (!(a.type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(b.type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	if (!(c.type() == Dim::SCALAR)) throw DimException("\"chi\" expects scalar arguments");
	const ExprNode* args2[3] = {&a,&b,&c};
	return *new ExprChi(args2);
}


ExprApply::ExprApply(const Function& f, const ExprNode** args) :
		ExprNAryOp(args,f.nb_arg(),f.expr().dim),
		func(f) {
	for (int i=0; i<f.nb_arg(); i++) {

		if (args[i]->dim.is_vector()) {
			// we allow automatic transposition of vector arguments
			if (f.arg(i).dim.is_vector() && (args[i]->dim.vec_size()==f.arg(i).dim.vec_size())) continue;
		} else {
			// otherwise, dimensions must match exactly.
			if (args[i]->dim == f.arg(i).dim) continue;
		}

		stringstream s;
		s << "dimension of the " << (i+1) << "th argument passed to \"" << f.name << "\" ";
		s << "do not match that of the formal argument \"" << f.arg_name(i) << "\"";
		throw DimException(s.str());
	}
}

ExprSymbol::ExprSymbol(const Dim& dim) : ExprLeaf(dim),
		name(strdup(next_generated_var_name())), key(-1) {
}

const ExprSymbol& ExprSymbol::new_(const Dim& dim) {
	return new_(next_generated_var_name(), dim);
	//return new_(generated_name_buff, dim);
}

ExprConstant::ExprConstant(const Interval& x)
  : ExprLeaf(Dim()),
    value(Dim()) {

	value.i() = x;
}

ExprConstant::ExprConstant(const IntervalVector& v, bool in_row)
  : ExprLeaf(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())),
    value(in_row? Dim::row_vec(v.size()) : Dim::col_vec(v.size())) {

	value.v() = v;
}

ExprConstant::ExprConstant(const IntervalMatrix& m)
  : ExprLeaf(Dim::matrix(m.nb_rows(),m.nb_cols())),
    value(Dim::matrix(m.nb_rows(),m.nb_cols())) {

	value.m() = m;
}

ExprConstant::ExprConstant(const IntervalMatrixArray& ma)
  : ExprLeaf(Dim::matrix_array(ma.size(),ma.nb_rows(),ma.nb_cols())),
    value(Dim::matrix_array(ma.size(),ma.nb_rows(),ma.nb_cols())) {

	value.ma() = ma;
}

ExprConstant::ExprConstant(const Domain& d, bool reference) : ExprLeaf(d.dim), value(d,reference) {
}

bool ExprConstant::is_zero() const {
	switch(dim.type()) {
	case Dim::SCALAR:     return value.i()==Interval::ZERO; break;
	case Dim::ROW_VECTOR:
	case Dim::COL_VECTOR: return value.v().is_zero(); break;
	case Dim::MATRIX:     return value.m().is_zero(); break;
	default:              return false;
	}
}

const ExprConstant& ExprConstant::copy() const {
	return new_(value);
}

ExprBinaryOp::ExprBinaryOp(const ExprNode& left, const ExprNode& right, const Dim& dim) :
		ExprNode(	max_height(left,right)+1,
					bin_size(left,right),
					dim ),
		left(left), right(right) {

	((ExprNode&) left).fathers.add(*this);
	((ExprNode&) right).fathers.add(*this);
}

ExprAdd::ExprAdd(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,left.dim) {
	if (!(left.dim == right.dim)) throw DimException("mismatched dimensions in addition");
}

ExprMul::ExprMul(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,mul_dim(left.dim,right.dim)) {
}

ExprSub::ExprSub(const ExprNode& left, const ExprNode& right) :
				ExprBinaryOp(left,right,left.dim) {
	if (!(left.dim == right.dim)) throw DimException("mismatched dimensions in subtraction");
}

ExprDiv::ExprDiv(const ExprNode& left, const ExprNode& right) :
						ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("cannot divide a non-scalar expression");
	if (!(right.type() == Dim::SCALAR)) throw DimException("cannot divide by a non-scalar expression");
}

ExprMax::ExprMax(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("\"max\" expects scalar arguments");
	if (!(right.type() == Dim::SCALAR)) throw DimException("\"max\" expects scalar arguments");
}

ExprMin::ExprMin(const ExprNode& left, const ExprNode& right) :
		ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("\"min\" expects scalar arguments");
	if (!(right.type() == Dim::SCALAR)) throw DimException("\"min\" expects scalar arguments");
}

ExprAtan2::ExprAtan2(const ExprNode& left, const ExprNode& right) :
			ExprBinaryOp(left,right,Dim()) {
	if (!(left.type() == Dim::SCALAR)) throw DimException("\"atan2\" expects scalar arguments");
	if (!(right.type() == Dim::SCALAR)) throw DimException("\"atan2\" expects scalar arguments");
}

ExprUnaryOp::ExprUnaryOp(const ExprNode& subexpr, const Dim& dim) :
				ExprNode(subexpr.height+1, subexpr.size+1, dim), expr(subexpr) {
	((ExprNode&) expr).fathers.add(*this);
}

ExprSign::ExprSign(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sign\" expects a scalar argument");
}

ExprAbs::ExprAbs(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"abs\" expects a scalar argument");
}

ExprSqr::ExprSqr(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sqr\" expects a scalar argument");
}

ExprSqrt::ExprSqrt(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sqrt\" expects a scalar argument");
}

ExprExp::ExprExp(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"exp\" expects a scalar argument");
}

ExprLog::ExprLog(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"log\" expects a scalar argument");
}

ExprCos::ExprCos(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"cos\" expects a scalar argument");
}

ExprSin::ExprSin(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sin\" expects a scalar argument");
}

ExprTan::ExprTan(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"tan\" expects a scalar argument");
}

ExprCosh::ExprCosh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"cosh\" expects a scalar argument");
}

ExprSinh::ExprSinh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"sinh\" expects a scalar argument");
}

ExprTanh::ExprTanh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"tanh\" expects a scalar argument");
}

ExprAcos::ExprAcos(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"acos\" expects a scalar argument");
}

ExprAsin::ExprAsin(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"asin\" expects a scalar argument");
}

ExprAtan::ExprAtan(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"atan\" expects a scalar argument");
}

ExprAcosh::ExprAcosh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"acosh\" expects a scalar argument");
}

ExprAsinh::ExprAsinh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"asinh\" expects a scalar argument");
}

ExprAtanh::ExprAtanh(const ExprNode& expr) : ExprUnaryOp(expr,expr.dim) {
	if (!expr.dim.is_scalar()) throw DimException("\"atanh\" expects a scalar argument");
}

std::ostream& operator<<(std::ostream& os, const ExprNode& expr) {
  ExprPrinter().print(os,expr);
  return os;
}

} // end namespace ibex
