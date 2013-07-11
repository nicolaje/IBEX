//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcXNewton.h
// Author      : Ignacio Araya, 
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2012
// Last Update : Jul 02, 2013 (Gilles Chabert)
//============================================================================


#ifndef __IBEX_CTC_XNEWTON_H__
#define __IBEX_CTC_XNEWTON_H__

#include "ibex_Ctc.h"
#include "ibex_System.h"
#include "ibex_NumConstraint.h"
#include "ibex_CtcLinearRelaxationIter.h"
#include "ibex_LinearSolver.h"

#include <vector>

namespace ibex {

/**
 * \ingroup ctcgroup
 * \brief X_Newton contractor 
 *
 * This class is an implementation of the X-Newton algorithm
 */

class CtcXNewton : public CtcLinearRelaxationIter {

public:

	/**
	 * \brief Strategies for selecting corner points.
	 */
	typedef enum  {RANDOM_INV, NEG, INF_X, SUP_X, RANDOM, GREEDY1, GREEDY5, GREEDY6, BEST , MONO,NEGMONO, K4} corner_point;

	/** Default max_diam_deriv value, set to 1e6  **/
	static const double default_max_diam_deriv;

	/**
	 * \brief Creates the X_Newton contractor.
	 *
	 * \param sys             - The system (extended or not).
	 * \param cpoints         - The vector of corner selection in linearization (X_INF, X_SUP, RANDOM, RANDOM_INV)
	 * \param cmode           - X_NEWTON (contracts all the box) | LOWER_BOUNDING (in optimization only improves the left bound of the variable y)
	 * \param lmode           - TAYLOR | HANSEN : linear relaxation method.
	 * \param max_iter_soplex - The maximum number of iterations for Soplex (default value 100)
	 * \param max_diam_deriv  - The maximum diameter of the derivatives for calling Soplex (default value 1.e5)
	 * \param max_diam_box    - The maximum diameter of the variables for calling Soplex (default value 1.e4) Soplex may lose solutions when it is called with "big" domains.
	 */
	CtcXNewton(const System& sys, std::vector<corner_point>& cpoints,
			ctc_mode cmode=ALL_BOX, linear_mode lmode=HANSEN, int max_iter=LinearSolver::default_max_iter,
			double max_diam_deriv=default_max_diam_deriv, Interval max_diam_box=LinearSolver::default_limit_diam_box,
			bool init_lp=true);

	/**
	 * \brief Deletes this instance.
	 */
	~CtcXNewton();

	/**
	 * \brief The vector of corner selection in linearization
	 *
	 * Can be either X_INF, X_SUP, RANDOM, or RANDOM_INV.
	 */
	std::vector<corner_point>& cpoints;

	/**
	 * \brief Generation of the linearized system
	 *
	 * Linearize the system and performs 2n calls to Simplex in order to reduce
	 * the 2 bounds of each variable
	 */
	int linearization( IntervalVector & box, LinearSolver *mysolver);


private:
	/* Computes the gradient G of the constraint ctr : special case if ctr==goal_ctr */
	void gradient_computation (IntervalVector& box, IntervalVector& G, int ctr);

	/**
	 * \brief Goal constraint (in case of extended system, -1 otherwise).
	 */
	const int goal_ctr;

	/** Maximum diameter of the derivatives for calling linear solver (default value 1.e5) */
	double max_diam_deriv;

	/** TAYLOR | HANSEN : the linear relaxation method */
	linear_mode lmode;

	/** Stores the coefficients of linear constraints */
	IntervalMatrix linear_coef;

	/* For implementing RANDOM_INV one needs to store the last random corners */
	int* last_rnd;

	int* base_coin;

	/** Indicates if the constraint is linear */
	bool* linear;

	/**
	 * Initialize the #linear and #linear_coef fields.
	 */
	void init_linear_coeffs();

	/**
	 * \brief Tries to add a linearization in the model mysoplex.
	 *
	 * \return 0 only when the linearization is not performed
	 */
	int X_Linearization(IntervalVector & box, int ctr, corner_point cpoint,  IntervalVector &G,
			int id_point, int& non_linear_vars, LinearSolver *mysolver);

	int X_Linearization(IntervalVector& box, int ctr, corner_point cpoint, CmpOp op,
			IntervalVector &G, int id_point, int& non_linear_vars, LinearSolver *mysolver);

//	// used in greedy heuristics :  not implemented in v2.0
//	inline double abs(double a){
//		return (a>=0)? a:-a;
//	}
//
//	//Evaluation of the corner by using relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC
//	double eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);
//
//	//finds the corner with the minimal evaluation of relation (4) in Taylorisation par intervalles convexe: premiers résultats, JFPC
//	void best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner);

};

} // end namespace ibex

#endif /* __IBEX_CTC_XNEWTON_H__ */

