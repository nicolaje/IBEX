//============================================================================
//                                  I B E X                                   
// File        : ibex_CellBuffer.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_BUFFER_H__
#define __IBEX_CELL_BUFFER_H__

#include "ibex_Cell.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Cell Buffer Overflow Exception.
 */
class CellBufferOverflow : Exception {

};

/** \ingroup strategy
 *
 * \brief Cell Buffer
 *
 * This class allows to control the order in which cells are treated by
 * the paver, i.e., to control the way domains are explored.
 * For instance, if the container is implemented as a stack, the paver
 * will perform a depth-first search. If it is a file, the paver will
 * perform a breadth-first search, and so on.
 *
 */
class CellBuffer {

 public:
	/**
	 * \brief Limit the buffer size.
	 *
	 * When the number of cells exceeds this number,
	 * a #ibex::CellBufferOverflow exception must be raised.
	 * Special value "-1" means no limit. By default, it is -1.
	 */
	int capacity;

	/**
	 * \brief Create a buffer
	 */
	CellBuffer();

	/** Delete *this. */
	virtual ~CellBuffer();

	/** Flush the buffer.
	 * All the remaining cells will be *deleted* */
	virtual void flush()=0;

	/** Return the size of the buffer. */
	virtual int size() const=0;

	/** Return true if the buffer is empty. */
	virtual bool empty() const=0;

	/** Push a new cell on the stack. */
	virtual void push(Cell* cell)=0;

	/** Pop a cell from the stack and return it.*/
	virtual Cell* pop()=0;

	/** Return the next box (but does not pop it).*/
	virtual Cell* top() const=0;

	/** Count the number of cells pushed since
	 * the object is created. */
	int nb_cells;

 private:
	friend std::ostream& operator<<(std::ostream& os, const CellBuffer&);

	mutable int screen; // only for display (debug)
};

std::ostream& operator<<(std::ostream& os, const CellBuffer&);

} // end namespace ibex
#endif // __IBEX_CELL_BUFFER_H__
