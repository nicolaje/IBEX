/* ============================================================================
 * I B E X - HC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 03, 2012
 * ---------------------------------------------------------------------------- */

#include "TestInHC4Revise.h"
#include "ibex_Function.h"
#include "ibex_Expr.h"

using namespace std;

namespace ibex {

void TestInHC4Revise::add01() {

	Variable x,y;
	Function f(x,y,x+y);

	double init_xy[][2]= { {-1,1}, {-1,1} };
	IntervalVector box(2,init_xy);

	f.iproj(Interval(0,1),box);

	TEST_ASSERT((box[0]+box[1]).is_subset(Interval(0,1)));
	check(box[0].lb()+box[1].lb(),0);
	check(box[0].ub()+box[1].ub(),1);
}


void TestInHC4Revise::add02() {

	Variable x;
	Function f(x,x+Interval(0,1));

	IntervalVector box(1,Interval(0,1));

	f.iproj(Interval(0,1),box);

	TEST_ASSERT(box.is_empty() || // TEMPORARY WRAP
			box[0]==Interval(1,1));
}



void TestInHC4Revise::dist01() {
	Variable x(2),y(2);
	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));
	IntervalVector box(4,Interval(0,4));
	double _pt[][2] = { {0.5,0.5} , {0.5,0.5} , {1.0,1.0} , {1.0,1.0} };
	IntervalVector pt(4,_pt);
	f.iproj(Interval(0,1),box,pt);

	// check the box is inside by testing two corners
	TEST_ASSERT(f.eval(box.lb()).is_subset(Interval(0,1)));
	TEST_ASSERT(f.eval(box.ub()).is_subset(Interval(0,1)));

}

void TestInHC4Revise::apply01() {
	Variable x(2),y(2);
	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));

	Function g(x,y,f(x,y));
	IntervalVector box(4,Interval(0,4));
	double _pt[][2] = { {0.5,0.5} , {0.5,0.5} , {1.0,1.0} , {1.0,1.0} };
	IntervalVector pt(4,_pt);
	g.iproj(Interval(0,1),box,pt);

	// check the box is inside by testing two corners
	TEST_ASSERT(f.eval(box.lb()).is_subset(Interval(0,1)));
	TEST_ASSERT(f.eval(box.ub()).is_subset(Interval(0,1)));

}

void TestInHC4Revise::apply02() {
	Variable x(2),y(2);
	Function f(x,y,sqrt(sqr(x[0]-y[0])+sqr(x[1]-y[1])));

	IntervalVector constY(2,Interval::ONE);
	Function g(x, f(x,constY));
	IntervalVector box(2,Interval(0,4));
	double _pt[][2] = { {0.5,0.5} , {0.5,0.5} };
	IntervalVector pt(2,_pt);
	g.iproj(Interval(0,1),box,pt);

	// check the box is inside by testing two corners
	TEST_ASSERT(almost_eq(g.eval(box.lb()),Interval::ONE,1e-07));
	TEST_ASSERT(almost_eq(g.eval(box.ub()),Interval::ONE,1e-07));
}

} // end namespace

