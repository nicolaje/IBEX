/* ============================================================================
 * I B E X - InHC4Revise Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 3, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __TEST_IN_HC4_REVISE_H__
#define __TEST_IN_HC4_REVISE_H__

#include "cpptest.h"
#include "utils.h"

namespace ibex {

class TestInHC4Revise : public TestIbex {

public:
	TestInHC4Revise() {
		TEST_ADD(TestInHC4Revise::add01);
		TEST_ADD(TestInHC4Revise::add02);
		TEST_ADD(TestInHC4Revise::dist01);
		TEST_ADD(TestInHC4Revise::apply01);
		TEST_ADD(TestInHC4Revise::apply02);
	}

	void add01();
	void add02();
	void dist01();
	void apply01();
	void apply02();
};

} // end namespace

#endif /* __TEST_IN_HC4_REVISE_H__ */
