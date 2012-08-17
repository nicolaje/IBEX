//============================================================================
//                                  I B E X                                   
// File        : ibex_Optimizer.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "ibex_Optimizer.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Timer.h"
#include "ibex_OptimProbing.cpp_"
#include "ibex_OptimSimplex.cpp_"
#include "ibex_CtcProj.h"
#include "ibex_ExprCopy.h"

using namespace std;

namespace ibex {

const double Optimizer::default_prec = 1e-07;
const double Optimizer::default_goal_rel_prec = 1e-07;
const double Optimizer::default_goal_abs_prec = 1e-07;
const int Optimizer::default_sample_size = 10;
const char* Optimizer::goal_name = "y";


// \pre ctrs must be non empty.
void Optimizer::build_ext_csp(const Array<NumConstraint>& ctrs) {

	assert(!ctrs.is_empty()); // <=> m>0

	const Array<const ExprSymbol>& x=ctrs[0].f.symbols();
	int nb=x.size();  // warning: x.size()<>n in general

	// ---------- check all constraints have same variables ---------
	for (int i=0; i<m; i++) {
		assert(ctrs[i].f.nb_symbols()==nb);
		for (int j=0; j<nb; j++)
			assert(ctrs[i].f.symbol(j).dim==x[j].dim);
	}
	// -------------------------------------------------------------

	// ---------- build a copy of variables ----------------
	Array<const ExprSymbol> x_copy(nb);
	for (int j=0; j<nb; j++) {
		x_copy.set_ref(j,ExprSymbol::new_(x[j].name, x[j].dim));
	}
	// -------------------------------------------------------------


	// ---------- create the vector-valued function g ----------------
	vector<const ExprNode*> vec_g;
	for (int i=0; i<m; i++) {
		const ExprNode& g_i=ExprCopy().copy(ctrs[i].f.symbols(),x_copy,ctrs[i].f.expr());
		switch (ctrs[i].op) {
		case NumConstraint::LT:  warning("warning: strict inequality (<) replaced by inequality (<=).");
		case NumConstraint::LEQ: vec_g.push_back(&g_i);
		                         break;
		case NumConstraint::EQ:  not_implemented("Optimization with equality constraints");
		                         break;
		case NumConstraint::GT:  warning("warning: strict inequality (>) replaced by inequality (>=).");
		case NumConstraint::GEQ: vec_g.push_back(&(-g_i)); // reverse the inequality
		                         break;
		}
	}
	g.init(x_copy, vec_g.size()>1 ? ExprVector::new_(vec_g,false) : *vec_g[0]);
	// -------------------------------------------------------------


	// ---------- build the extended set of variables ----------------
	const ExprSymbol& y=ExprSymbol::new_(goal_name,Dim()); // y is a scalar

	Array<const ExprSymbol> ext_vars(nb+1);
	for (int j=0; j<nb; j++) {
		ext_vars.set_ref(j,ExprSymbol::new_(x[j].name, x[j].dim));
	}
	// warning: y must be added at the end (goal_var is set to n in constructor)
	// We set goal_var to n (<=>y variable is the nth variable)
	// to simplify the copy of expressions (see ibex_ExprCopy).
	ext_vars.set_ref(nb,y);
	// -------------------------------------------------------------


	// ---------- create the extended function ext_f_g ----------------
	const ExprNode& f_copy=ExprCopy().copy(f.symbols(),ext_vars,f.expr());
	vector<const ExprNode*> vec;

	vec.push_back(&(y-f_copy)); // warning: must be added first (goal_ctr is set to 0 in constructor)

	for (int i=0; i<m; i++) {
		const ExprNode& g_copy=ExprCopy().copy(g[i].symbols(),ext_vars,g[i].expr());
		vec.push_back(&g_copy);
	}

	ext_f_g.init(ext_vars,ExprVector::new_(vec,false));
	// -------------------------------------------------------------


	// ---------- create the extended CSP ----------------
	// warning: must be added first (goal_ctr is set to 0 in constructor)
	ext_csp.set_ref(0,*new NumConstraint(ext_f_g[0])); // equality (by default)

	for (int i=0; i<m; i++)
		ext_csp.set_ref(i+1,*new NumConstraint(ext_f_g[i+1], NumConstraint::LEQ));

}

void Optimizer::write_ext_box(const IntervalVector& box, IntervalVector& ext_box) {
	int i2=0;
	for (int i=0; i<n; i++,i2++) {
		if (i2==goal_var) i2++; // skip goal variable
		ext_box[i2]=box[i];
	}
}

void Optimizer::read_ext_box(const IntervalVector& ext_box, IntervalVector& box) {
	int i2=0;
	for (int i=0; i<n; i++,i2++) {
		if (i2==goal_var) i2++; // skip goal variable
		box[i]=ext_box[i2];
	}
}

Optimizer::Optimizer(Function& f, Array<NumConstraint>& ctrs, Bsc& bsc, double prec,
		double goal_rel_prec, double goal_abs_prec, int sample_size) :
		n(f.input_size()), m(ctrs.size()), f(f),
		ext_csp(m+1), goal_ctr(0), goal_var(n), bsc(bsc), buffer(n),
		prec(prec), goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec),
		sample_size(sample_size), mono_analysis_flag(true), in_HC4_flag(true), trace(false),
		timeout(1e08), loup(POS_INFINITY), loup_point(n),
		uplo_of_epsboxes(POS_INFINITY) {

	build_ext_csp(ctrs);

	// ====== build the propagation of f(x)=0 and all g_i(x)<=0 =====
	ctc = new CtcHC4(ext_csp);
	// =============================================================

	// ====== build the reversed inequalities g_i(x)>0 ===============
	Array<Ctc> ng(m);
	for (int i=0; i<m; i++) {
		ng.set_ref(i, *new CtcProj(g[i],NumConstraint::GT));
	}
	is_inside=new CtcUnion(ng);
	// =============================================================

	if (trace) cout.precision(12);

	nb_simplex=0;
	diam_simplex=0;
	nb_rand=0;
	diam_rand=0;
}

Optimizer::~Optimizer() {

	delete &ext_csp[0];

	for (int i=0; i<m; i++) {
		delete &(is_inside->list[i]);
		delete &ext_csp[i+1];
	}

	delete ctc;
	delete is_inside;
}

bool Optimizer::update_loup(const IntervalVector& box) {
	int box_loup_changed = update_loup_probing (box);
	box_loup_changed |= update_loup_simplex(box);
	return box_loup_changed;
}

bool Optimizer::contract_and_bound(Cell& c) {

	/*======================== contract y with y<=loup ========================*/
	Interval& y=c.box[goal_var];

	//cout << "loup=" << loup << endl;

	double ymax;
	if (loup==POS_INFINITY) ymax=POS_INFINITY;
	else ymax=loup - goal_rel_prec*fabs(loup);

	if (loup - goal_abs_prec < ymax)
		ymax = loup - goal_abs_prec;

	//cout << "ymax=" << ymax << endl;

	if (y.lb() > ymax) {
		c.box.set_empty();
		throw EmptyBoxException();
	}
	if (y.ub() > ymax) y = Interval(y.lb(),ymax);

	/*================ contract x with f(x)=y and g(x)<=0 ================*/
//	cout << "y=f(x) and g(x)<=0 " << endl;
//	cout << "   x before=" << c.box << endl;
//	cout << "   y before=" << y << endl;

	ctc->contract(c.box); // may throw EmptyBoxException

//	cout << "   x after=" << c.box << endl;
//	cout << "   y after=" << y << endl;
	// TODO: no more cell in argument here (just a box). Does it matter?
	/*====================================================================*/

	// there is still something left to be contracted in the box
	/*========================= update loup ==============================*/
	IntervalVector tmp_box(n);
	read_ext_box(c.box,tmp_box);
	bool loup_changed = update_loup(tmp_box);
	write_ext_box(tmp_box,c.box);
	/*====================================================================*/

	if (tmp_box.max_diam()<=prec) {
		// rem1: tmp_box and not c.box because y is [-inf;loup]
		// rem2: do not use a precision contractor here since it would make the box empty (and y==(-inf,-inf)!!)

		// the box is a "solution"
		if (uplo_of_epsboxes > y.lb()) {
			if (trace) {
				cout.precision(12);
				cout << "uplo of eps-boxes:" << y.lb() << endl;
			}
			uplo_of_epsboxes = y.lb();
		}
		throw EmptyBoxException();
	}

	return loup_changed;
}

void Optimizer::optimize(const IntervalVector& init_box) {

	buffer.flush();

	Cell* root=new Cell(IntervalVector(n+1));

	write_ext_box(init_box,root->box);

	// add data required by the contractor
	//ctc.init_root(*root); // we know there is none (not incremental HC4).

	// add data required by the bisector
	bsc.init_root(*root);

	buffer.push(root);

	while (!buffer.empty()) {

		//if (trace) cout << ((CellBuffer&) buffer) << endl;

		Cell* c=buffer.top();

		try {
			bool loup_changed=contract_and_bound(*c);
			if (loup_changed) {
				// In case of a new upper bound (loup_changed == true), all the boxes
				// with a lower bound greater than (loup - goal_prec) are removed and deleted.
				double ymax=loup - goal_rel_prec*fabs(loup);
				if (loup - goal_abs_prec < ymax)
					ymax = loup - goal_abs_prec;
				((CellHeap&) buffer ).contract_heap(ymax);

				if (trace) cout << "uplo " << ymax << endl;
			}
			// Note: in principle, even if the loup has changed, the current
			// cell "c" should not been removed by the previous call to contract_heap.
			// because this cell is precisely the one where a new loup has been found.
			// However, because of goal_xxx_ceil, it can actually be removed. But, in the
			// latter case, the buffer is necessarily empty: this cell is the one with
			// the lowest lower bound on the criterion. If it has been removed, all
			// the subsequent cells in the heap are removed.
			if (!buffer.empty()) {
				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
			}
		} catch(EmptyBoxException&) {
			delete buffer.pop();
		}

		Timer::check(timeout);
	}
}

void Optimizer::report() {
	double uplo=loup - goal_rel_prec*fabs(loup);
	if (loup - goal_abs_prec < uplo)
		uplo = loup - goal_abs_prec;

	if (uplo > uplo_of_epsboxes) uplo=uplo_of_epsboxes;

	cout << " best bound in: [" << loup << "," << uplo << "]" << endl;

	double rel_prec;
	if (loup==POS_INFINITY) rel_prec=0;
	else rel_prec=(loup-uplo)/(fabs (loup));

	double abs_prec=loup-uplo;

	cout << " relative precision obtained on objective function: " << rel_prec << " " <<
			(rel_prec < goal_rel_prec? " [passed]" : " [failed]") << endl;

	cout << " absolute precision obtained on objective function: " << abs_prec << " " <<
			(abs_prec < goal_abs_prec? " [passed]" : " [failed]") << endl;
}

} // end namespace ibex
