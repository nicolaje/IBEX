//============================================================================
//                                  I B E X                                   
// File        : ibex_NodeMap.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Feb 27, 2013
// Last Update : Feb 27, 2013
//============================================================================

#ifndef __IBEX_NODE_MAP_H__
#define __IBEX_NODE_MAP_H__

#include "ibex_Expr.h"

#ifdef __GNUC__
#include <tr1/unordered_map>
#define IBEX_NODE_MAP(T) std::tr1::unordered_map<const ExprNode*,T>
#else
#include <unordered_map>
#define IBEX_NODE_MAP(T) std::unordered_map<const ExprNode*,T>
#endif

namespace ibex {

/**
 * \brief An unordered map which keys are expression nodes.
 *
 */
template <typename T>
class NodeMap {
public:

	/**
	 * \brief True if e is in the map
	 */
	bool found(const ExprNode& e) {
		return map.find(&e)!=map.end();
	}

	/**
	 * \brief Insert a pair <e,value>.
	 *
	 * The value is passed by copy.
	 */
	void insert(const ExprNode& e, const T& value) {
		map.insert(std::pair<const ExprNode*,T>(&e,value));
	}

	/**
	 * \brief Return the value associated to \a e.
	 *
	 * Return a non-const reference.
	 */
	T& operator[](const ExprNode& e) {
		return map[&e];
	}

	/**
	 * \brief Erase all elements.
	 */
	void clean() {
		map.clear();
	}

	typename IBEX_NODE_MAP(T)::iterator begin() {
		return map.begin();
	}

	typename IBEX_NODE_MAP(T)::iterator end() {
		return map.end();
	}

	typename IBEX_NODE_MAP(T)::const_iterator begin() const {
		return map.begin();
	}

	typename IBEX_NODE_MAP(T)::const_iterator end() const {
		return map.end();
	}

protected:
	IBEX_NODE_MAP(T) map;
};

} // end namespace ibex
#endif // __IBEX_NODE_MAP_H__
