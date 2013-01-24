/* ============================================================================
 * I B E X - HC4 Revise (forward-backward algorithm)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Feb 27, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_CTC_FWDBWD_H__
#define __IBEX_CTC_FWDBWD_H__

#include "ibex_Ctc.h"
#include "ibex_HC4Revise.h"
#include "ibex_NumConstraint.h"

namespace ibex {

/**
 * \ingroup contractor
 * \brief Forward-backward contractor (HC4Revise).
 *
 */
class CtcFwdBwd: public Ctc {

public:
	/**
	 * \brief HC4Revise on f(x)=0 or f(x)<=0.
	 *
	 * \param op: by default: true.
	 */
	CtcFwdBwd(Function& f, CmpOp op=EQ);

	/**
	 * \remark ctr is not kept by reference.
	 */
	CtcFwdBwd(const NumConstraint& ctr);

	/**
	 * \brief Contract the box.
	 */
	virtual void contract(IntervalVector& box);

	/*
	 * \brief Whether this contractor is idempotent (optional)
	 */
	// TODO
	// bool idempotent();
	//

	/**
	 * The constraint.
	 */
	const NumConstraint ctr;

	/**
	 * HC4Revise algorithm.
	 */
	HC4Revise hc4r;
private:

};

} // namespace ibex
#endif // __IBEX_CTC_FWDBWD_H__
