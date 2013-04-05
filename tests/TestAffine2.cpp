/*
 * TestAffine2.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: nininjo
 */


#include "TestAffine2.h"

#include "ibex_Function.h"
#include "ibex_CtcFwdBwd.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_LargestFirst.h"

#include "ibex_CellStack.h"

#include "ibex_Solver.h"

//using namespace std;

void TestAffine2::test01() {
	Variable x(2);
	Function f(x,x[0]*pow(x[1],2)+exp(x[1]*x[0]));
	IntervalVector itv(2,Interval(1,2));
	TEST_ASSERT(check_af2(f,itv));

}

void TestAffine2::test02() {
	Variable x;
	Function f(x,cosh(x)-x);
	IntervalVector itv(1,Interval(1,2));
	TEST_ASSERT(check_af2(f,itv));

}

void TestAffine2::test_pow2() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,2));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_pow4() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,4));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}



void TestAffine2::test_pow5() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,5));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

/*
void TestAffine2::test_root2() {
	Variable x;
	Interval itv;
	Function f(x,root(x,2));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_root4() {
	Variable x;
	Interval itv;
	Function f(x,root(x,4));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}



void TestAffine2::test_root5() {
	Variable x;
	Interval itv;
	Function f(x,root(x,5));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

*/

void TestAffine2::test_powINT1() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,Interval(2,3)));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_powINT2() {
	Variable x;
	Interval itv;
	Function f(x,pow(x,Interval(-2,3)));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}


void TestAffine2::test_sqrt() {
	Variable x;
	Interval itv;
	Function f(x,sqrt(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_exp() {
	Variable x;
	Interval itv;
	Function f(x,exp(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-10,50000000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-50000000,1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1000000000.,1000000000.001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_log() {
	Variable x;
	Interval itv;
	Function f(x,log(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1000000000.,1000000000.001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10,200000000);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_inv() {
	Variable x;
	Interval itv;
	Function f(x,1.0/x);
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10,20);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-20000000,-1);
	TEST_ASSERT(check_af2(f,itv));
}

void TestAffine2::test_cos() {
	Variable x;
	Interval itv;
	Function f(x,cos(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.0000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));


	itv =Interval::PI;
	TEST_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}

}

void TestAffine2::test_sin() {
	Variable x;
	Interval itv;
	Function f(x,sin(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.00001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));


	itv =Interval::PI;
	TEST_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}


}

void TestAffine2::test_tan() {
	Variable x;
	Interval itv;
	Function f(x,tan(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));

	itv =Interval::PI;
	TEST_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}

}

void TestAffine2::test_abs() {
	Variable x;
	Interval itv;
	Function f(x,abs(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}

void TestAffine2::test_acos() {
	Variable x;
	Interval itv;
	Function f(x,acos(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-10,20);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-10,0.2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(0.5,0.7);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);

	itv =Interval::PI;
	TEST_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}

}

void TestAffine2::test_asin() {
	Variable x;
	Interval itv;
	Function f(x,asin(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-10,20);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-10,0.2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(0.5,0.7);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);

	itv =Interval::PI;
	TEST_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}

}

void TestAffine2::test_atan() {
	Variable x;
	Interval itv;
	Function f(x,atan(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);

	itv =Interval::PI;
	TEST_ASSERT(check_af2(f,itv));
	for (int i =-20;i<34;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<15;i++) {
		itv =Interval(i*Interval::PI.lb()/6,(i+1)*Interval::PI.ub()/6);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+3)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+2)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<20;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+3)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<40;i++) {
		itv =Interval(i*Interval::PI.lb()/16,(i+4)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<30;i++) {
		itv =Interval(i*Interval::PI.lb()/8,(i+7)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<34;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/16,(i+1)*Interval::PI.ub()/16);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<18;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/8,(i+1)*Interval::PI.ub()/8);
		TEST_ASSERT(check_af2(f,itv));
	}
	for (int i =-20;i<10;i++) {
		itv =Interval(0.1+i*Interval::PI.lb()/4,(i+1)*Interval::PI.ub()/4);
		TEST_ASSERT(check_af2(f,itv));
	}

}

void TestAffine2::test_cosh() {
	Variable x;
	Interval itv;
	Function f(x,cosh(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}

void TestAffine2::test_sinh() {
	Variable x;
	Interval itv;
	Function f(x,sinh(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}



void TestAffine2::test_tanh() {
	Variable x;
	Interval itv;
	Function f(x,tanh(x));
	itv =Interval(1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,POS_INFINITY);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,-2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(NEG_INFINITY,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::ALL_REALS;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval::EMPTY_SET;
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,2);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(-1,-0.5);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,200000);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1,1.000000000001);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(1);
	TEST_ASSERT(check_af2(f,itv));
	itv =Interval(10000000,10000000.0001);
}




bool TestAffine2::check_af2 (Function& f, Interval& I){
	Affine2 faa;
	Interval itv =f.eval_affine2(IntervalVector(1,I), &faa);
	Interval itv2 =f.eval(IntervalVector(1,I));

	if (!(itv2.is_subset(faa.itv())))
	{
		std::cout  << " DEP = "<< I<< "  "  << f<< std::endl;
		std::cout  << " RES = "<< itv2 << " /// "<< itv << " ///// " << faa << std::endl;
	//	std::cout  << " RES = "<< itv2 << " ///// " << faa << std::endl;

		return false;
	}
	if (faa.size()>0) {
		Variable x;
		Function lininf(x,(faa.val(0)+faa.val(1)*(2* (x)-(I.lb()+I.ub()))/(I.diam())));

		Function linsup(x, lininf(x)+faa.err().lb());

		Function c_inf(x,lininf(x)+faa.err().lb()  -f(x));
		Function c_sup(x, f(x) -linsup(x));

		CtcFwdBwd ct1(c_inf,GT,AFFINE2_MODE);
		CtcFixPoint ft1(ct1,0.1);

		CtcFwdBwd ct2(c_sup,GT,AFFINE2_MODE);
		CtcFixPoint ft2(ct2,0.1);

		LargestFirst bbb;
		CellStack ccc;
		Solver sol1(ft1,bbb, ccc, 0.1 );
		Solver sol2(ft2,bbb, ccc, 0.1 );


		std::cout  << " DEP = "<< I<< "  "  << f<< std::endl;
		std::cout  << " RES = "<< itv2 << " /// "<< itv << " ///// " << faa << std::endl;
		std::cout<< " PAVING = "<< I.diam()<< " /// " << sol1.solve(IntervalVector(1,I)).size() << " et "<<sol2.solve(IntervalVector(1,I)).size() << std::endl;
//		return ((sol1.solve(IntervalVector(1,I)).empty()) && (sol2.solve(IntervalVector(1,I)).empty()));
	}

	return true;

}


bool TestAffine2::check_af2 (Function& f, IntervalVector& I){


	Affine2 faa;
	Interval itv =f.eval_affine2(I, &faa);
	Interval itv2 =f.eval(I);
	if (!(itv2.is_subset(faa.itv()))) {
		std::cout  << " DEP = "<< I<< "  "  << f<<std::endl;
		std::cout  << " RES = "<< itv2 << " /// "<< itv << " ///// " << faa << std::endl;
		return false;
	}
	if (faa.size()>0) {
		Variable x(I.size());

		const ExprNode *e =& (faa.val(0)+faa.val(1)*(2* (x[0])-(I[0].lb()+I[0].ub()))/(I[0].diam()));
		for (int i=1; i<I.size(); i++) {
			e =& ((*e) + (faa.val(i+1)*(2*x[i]-(I[i].lb()+I[i].ub()))/(I[i].diam())));
		}
		Function lininf(x,*e);


		Function linsup(x, lininf(x)+faa.err().lb());

		Function c_inf(x,lininf(x)+faa.err().lb()  -f(x));
		Function c_sup(x, f(x) -linsup(x));

		CtcFwdBwd ct1(c_inf,GT,AFFINE2_MODE);
		CtcFixPoint ft1(ct1,0.1);

		CtcFwdBwd ct2(c_sup,GT,AFFINE2_MODE);
		CtcFixPoint ft2(ct2,0.1);

		LargestFirst bbb;
		CellStack ccc;
		Solver sol1(ft1,bbb, ccc, 0.1 );
		Solver sol2(ft2,bbb, ccc, 0.1 );

		std::cout<< " PAVING = "<< I.max_diam()<< " ///  " << sol1.solve(I).size() << " et "<<sol2.solve(I).size() << std::endl;
		//return ((sol1.solve(I).empty()) && (sol2.solve(I).empty()));
	}


	return true;

}

