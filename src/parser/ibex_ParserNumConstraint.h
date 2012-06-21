//============================================================================
//                                  I B E X                                   
// File        : ibex_ParserNumConstraint.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#ifndef __IBEX_PARSER_NUM_CONSTRAINT_H__
#define __IBEX_PARSER_NUM_CONSTRAINT_H__

#include "ibex_CtrGenerator.h"
#include "ibex_NumConstraint.h"

namespace ibex {

namespace parser {

class P_NumConstraint {
public:
	virtual void acceptVisitor(CtrGenerator& g) const=0;

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	virtual ~P_NumConstraint()=0;
};

class P_OneConstraint : public P_NumConstraint {
public:
	P_OneConstraint(const ExprNode& left, NumConstraint::CompOp op, const ExprNode& right);

	void acceptVisitor(CtrGenerator& g) const {
		g.visit(*this);
	}

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	~P_OneConstraint();

	const ExprNode& expr;
	NumConstraint::CompOp op;
};

class P_ConstraintLoop : public P_NumConstraint {
public:
	P_ConstraintLoop(const char* iter, int first_value, int last_value, vector<P_NumConstraint*>& ctrs);

	void acceptVisitor(CtrGenerator& g) const {
		g.visit(*this);
	}

	/**
	 * delete all the nodes of the expression *excepted symbols*
	 */
	~P_ConstraintLoop();

	const char* iter;
	int first_value;
	int last_value;
	vector<P_NumConstraint*>& ctrs;

};

} // end namespace parser
} // end namespace ibex
#endif // __IBEX_PARSER_NUM_CONSTRAINT_H__
