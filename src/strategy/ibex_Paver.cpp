//============================================================================
//                                  I B E X                                   
// File        : ibex_Paver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#include "ibex_Paver.h"
#include "ibex_EmptyBoxException.h"
#include "ibex_Timer.h"

using namespace std;

namespace ibex {

Paver::Paver(const Array<Ctc>& c, Bsc& b, CellBuffer& buffer) :
		capacity(-1), ctc_loop(true), ctc(c), bsc(b), buffer(buffer) {

	assert(ctc.size()>0);
}

void Paver::contract(Cell& cell, SubPaving* paving) {
	int i=0; // contractor number

	int n=ctc.size(); // number of contractors

	// count the number of contractors in a row
	// that could not modify the box. When fix_count=n,
	// fixpoint is reached.
	// In a future version, we may use an AC3 loop instead.
	int fix_count=0;

	// used to compare boxes before and after contraction
	IntervalVector tmpbox(cell.box.size());

	try {
		while (fix_count<n) {

			//cout << "[contractor " << i << "] box=" << endl;

			// 	  for (int j=1; j<=box.size(); j++) {
			// 	    cout.precision(17);
			// 	    cout << "box[" << j << "]=" << box[j] << endl;
			// 	  }
			if (trace)  cout << "    ctc " << i;
			tmpbox=cell.box;

			ctc[i].contract(cell.box);

			if (tmpbox.rel_distance(cell.box)>0) {
				fix_count=0;

				paving[i].add(tmpbox,cell.box);

				if (trace) cout << " -> contracts" << endl;

			} else {
				fix_count++;
				if (trace) cout << " -> nothing" << endl;
			}

			i = (i+1)%ctc.size();

		}
	} catch(EmptyBoxException&) {
		assert(cell.box.is_empty());
		if (trace) cout << " -> empty set" << endl;

		paving[i].add(tmpbox);
	}

}

void Paver::bisect(Cell& c) {

	pair<IntervalVector,IntervalVector> boxes=bsc.bisect(c);
	pair<Cell*,Cell*> new_cells=c.bisect(boxes.first,boxes.second);

	delete buffer.pop();
	buffer.push(new_cells.first);
	buffer.push(new_cells.second);
}

SubPaving* Paver::pave(const IntervalVector& init_box) {

	SubPaving* paving=new SubPaving[ctc.size()];

	buffer.flush();

	Cell* root=new Cell(init_box);

	// add data required by the contractors
//	for (int i=0; i<ctc.size(); i++) {
//		ctc[i].init_root(*root);
//	}
	// add data required by the bisector
	bsc.add_backtrackable(*root);

	buffer.push(root);

	while (!buffer.empty()) {
		Cell* c=buffer.top();

		if (trace) cout << buffer << endl;

		contract(*c, paving);

		Timer::check(timeout);
		check_capacity(paving);

		if (c->box.is_empty()) delete buffer.pop();
		else bisect(*c);
	}

	return paving;
}


void Paver::check_capacity(SubPaving* paving) {
	if (capacity==-1) return;

	int size=0;
	for (int i=0; i<ctc.size(); i++) size+=paving[i].size();

	if (size>capacity) throw CapacityException();
}

} // end namespace ibex
