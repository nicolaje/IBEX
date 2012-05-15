//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcUnion.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_CTC_UNION_H__
#define __IBEX_CTC_UNION_H__

#include "ibex_Array.h"
#include "ibex_Ctc.h"

namespace ibex {

/**
 * \brief Union of contractors
 *
 * For a box [x] the union of {c_0,...c_n} performs
 * c_1([x]) \cup ... \cup c_n([x]).
 */
class CtcUnion : public Ctc {
public:

	CtcUnion(const Array<Ctc>& list);

	/**
	 * \brief Contract a box.
	 */
	virtual void contract(IntervalVector& box);

	/**
	 * \brief Delete *this.
	 */
	virtual ~CtcUnion();

protected:
	/** The list of sub-contractors */
	Array<Ctc> list;
};

} // end namespace ibex
#endif // IBEX_CTCUNION_H_
