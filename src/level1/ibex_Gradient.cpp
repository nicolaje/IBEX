/* ============================================================================
 * I B E X - ibex_Gradient.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 27, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_Gradient.h"

namespace ibex {



Gradient::Gradient(const Function& f) : f(f,GradDecorator()), eval((const CompiledFunction<EvalLabel>&) f) {

}

void Gradient::calculate(const Domain& box, IntervalVector& g) const {
	eval.forward(box);
	f.backward(*this);
	//for (int i=0; i<)
	// TO COMPLETE
}

} // namespace ibex