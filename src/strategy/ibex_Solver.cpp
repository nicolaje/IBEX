//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Solver.h"
#include "ibex_EmptyBoxException.h"
#include <cassert>

using namespace std;

namespace ibex {

Solver::Solver(Ctc& ctc, Bsc& bsc, CellBuffer& buffer, double prec) :
		ctc(ctc), bsc(bsc), buffer(buffer), prec(ctc.nb_var,prec), cell_limit(-1),
		trace(false) {

	nb_cells=0;

}

vector<IntervalVector> Solver::solve(const IntervalVector& init_box) {
	buffer.flush();

	Cell* root=new Cell(init_box);

	// add data required by the contractor
	//ctc.init_root(*root);

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.init_root(*root);

	buffer.push(root);

	vector<IntervalVector> sols;
	IntervalVector tmpbox(init_box.size());

	BoolMask impact(init_box.size());
	impact.set_all();
	int v;                              // last bisected var.

	while (!buffer.empty()) {

		if (trace) cout << buffer << endl;

		Cell* c=buffer.top();

		tmpbox = c->box;

		try {
			v=c->get<BisectedVar>().var;
			if (v!=-1) impact.set(v);

			ctc.contract(c->box,impact);

			if (v!=-1) impact.unset(v);

			try {
				((Ctc&) prec).contract(c->box);

				pair<IntervalVector,IntervalVector> boxes=bsc.bisect(*c);
				pair<Cell*,Cell*> new_cells=c->bisect(boxes.first,boxes.second);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (nb_cells==cell_limit) throw Exception();

			} catch(EmptyBoxException&) {
				assert(c->box.is_empty());
				sols.push_back(tmpbox);
				delete buffer.pop();
				impact.set_all();
			}

		} catch(EmptyBoxException&) {
			assert(c->box.is_empty());
			delete buffer.pop();
			impact.set_all();
		}

	}

	return sols;
}

} // end namespace ibex
