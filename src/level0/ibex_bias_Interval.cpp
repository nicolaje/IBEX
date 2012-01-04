/* ============================================================================
 * I B E X - Implementation of the Interval class based on Profil/BIAS
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 23, 2011
 * ---------------------------------------------------------------------------- */

extern void BiasInit ();
extern void BiasFuncInit ();

static void init() {
	BiasInit();
	BiasFuncInit();
}

namespace ibex {

// The following code is only used to force Constants.C to be
// always included in the executable code.

const Interval Interval::EMPTY_SET((init(), NEG_INFINITY), (init(), NEG_INFINITY));
const Interval Interval::ALL_REALS(NEG_INFINITY, POS_INFINITY);
const Interval Interval::NEG_REALS(NEG_INFINITY, 0.0);
const Interval Interval::POS_REALS(0.0, POS_INFINITY);
const Interval Interval::ZERO(0.0);
const Interval Interval::ONE(1.0);
const Interval Interval::PI(3.14159265358979323846, Succ(3.14159265358979323846));
const Interval Interval::TWO_PI = PI*2.0;
const Interval Interval::HALF_PI = PI/2.0;

std::ostream& operator<<(std::ostream& os, const Interval& x) {
	return os << x.itv;
}

} // end namespace

