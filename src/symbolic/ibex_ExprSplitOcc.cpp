//============================================================================
//                                  I B E X                                   
// File        : ibex_ExprSplitOcc.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 17, 2013
// Last Update : Dec 17, 2013
//============================================================================

#include "ibex_ExprSplitOcc.h"
#include "ibex_ExprSplitOcc.h"
#include "ibex_Expr.h"
#include "ibex_String.h"
#include <stdlib.h>

using namespace std;

namespace ibex {

ExprSplitOcc::ExprSplitOcc(const Array<const ExprSymbol>& x, const ExprNode& y) : old_y(y) {

	// count the number of variables

	int nb_new_var=0;

	// This loops initializes nb_clones for
	// all symbols & indices and special_node
	// for symbols.
	for (int i=0; i<x.size(); i++) {

		SymbolClone* s=new SymbolClone();
		symbol_clone.insert(x[i],s);

		s->nb_clones=0; // by default

		if (x[i].dim.type()==Dim::MATRIX || x[i].dim.type()==Dim::MATRIX_ARRAY)
			not_implemented("occurrence splitting with matrix symbols");

		if (x[i].fathers.size()==0) {
			// a variable must be considered
			// even if not involved in the expression
			s->nb_clones=1;
			nb_new_var++;
		}
		else {
			for (int k=0; k<x[i].fathers.size(); k++) {
				// check if the kth father of x is an expression x[j]
				const ExprIndex* idx=dynamic_cast<const ExprIndex*>(&x[i].fathers[k]);
				cout << "father " << k << "=" << x[i].fathers[k] << endl;
				if (idx!=NULL) {
					// the special clone is only created once.
					if (s->special_clone==NULL) {
						s->special_clone=&ExprSymbol::new_(x[i].name, x[i].dim);
						nb_new_var++;
					}

					// then count the number of fathers of x[j]: the first
					// occurrence corresponds to an indexation of the special
					// symbol. Additional occurrences will be new symbols.
					if (s->indices.find(idx->index)!=s->indices.end()) {
						IndexClone* ic=s->indices[idx->index];
						ic->nb_clones += idx->fathers.size();
						nb_new_var    += idx->fathers.size();
					} else {
						IndexClone* ic=new IndexClone();
						s->indices.insert(std::pair<int,IndexClone*>(idx->index,ic));
						if (idx->fathers.size()>1) { // rem fathers.size()==0 possible if x[j] is the entire DAG
						    // "-1"++ because 1st occurrence uses the special clone of the symbol.
							ic->nb_clones  = idx->fathers.size();
							nb_new_var    += idx->fathers.size()-1;
						} else { // 0 or 1
							ic->nb_clones  = 1;
						}
					}
				} else {
					s->nb_clones++;
					nb_new_var++;
				}
			}
		}
		cout << "number of clones for " << x[i].name << ":" << s->nb_clones << endl;
	}

	new_x.resize(nb_new_var);

	//fill the array of new variables
	int j=0; // index for the new
	int j2=0; // clone counter for each symbol

	for (int i=0; i<x.size(); i++) {
		SymbolClone* s=symbol_clone[x[i]];
		if (x[i].fathers.size()==0) {
			s->clones=new const ExprSymbol*[1];
			s->clones[0]=&ExprSymbol::new_(x[i].name, x[i].dim);
			new_x.set_ref(j++, *(s->clones[0]));
		} else {
			if (s->special_clone!=NULL) {
				new_x.set_ref(j++,*s->special_clone);
			}

			s->clones=new const ExprSymbol*[s->nb_clones];
			j2=0;
			for (int k=0; k<x[i].fathers.size(); k++) {
				// check if the kth father of x is an expression x[j]
				const ExprIndex* idx=dynamic_cast<const ExprIndex*>(&x[i].fathers[k]);
				if (idx==NULL) { // indices are handled after
					char* name=append_index(x[i].name, '_','_',j2);
					const ExprSymbol* sbl=&ExprSymbol::new_(name, x[i].dim);
					s->clones[j2++]=sbl;
					free(name);
					new_x.set_ref(j++,*sbl);
				}
			}
			assert(j2==s->nb_clones);

			// indices
			for (IBEX_INT_MAP(IndexClone*)::iterator it=s->indices.begin(); it!=s->indices.end(); it++) {
				IndexClone* ic=it->second;
				ic->clones = new const ExprNode*[ic->nb_clones];
				ic->clones[0] = &((*s->special_clone)[it->first]); // create new subexpression
				cout << "number of clones for index " << x[i].name << "[" << it->first << "]:" << ic->nb_clones << endl;

				for (int k=1; k<ic->nb_clones; k++) { // additional occurrence --> new symbol
					char* name=append_index(s->special_clone->name, '[',']',it->first);
					char* name2=append_index(name,'_','_',k);
					const ExprSymbol* sbl=&ExprSymbol::new_(name2,x[i].dim.index_dim());
					ic->clones[k]=sbl;
					free(name2);
					free(name);
					new_x.set_ref(j++,*sbl);
				}
			}
		}
	}
	assert(j==new_x.size());

	visit(y);
}

ExprSplitOcc::~ExprSplitOcc() {
	for (IBEX_NODE_MAP(SymbolClone*)::const_iterator it=symbol_clone.begin(); it!=symbol_clone.end(); it++) {
		for (IBEX_INT_MAP(IndexClone*)::const_iterator it2=it->second->indices.begin(); it2!=it->second->indices.end(); it2++) {
			delete[] it2->second->clones;
			delete it2->second;
		}
		delete[] it->second->clones;
		delete it->second;
	}
}

void ExprSplitOcc::visit(const ExprNode& e) {
	// if height=0, it might be symbol: we
	// need to visit it even if it has already been visited
	if (dynamic_cast<const ExprIndex*>(&e)!=NULL ||
		dynamic_cast<const ExprSymbol*>(&e)!=NULL ||
		!clone.found(e)) {
		e.acceptVisitor(*this);
	}
}

void ExprSplitOcc::visit(const ExprIndex& i) {
	if (i.indexed_symbol()) {
		const ExprSymbol& s=(const ExprSymbol&) i.expr;
		IndexClone* ic=symbol_clone[s]->indices[i.index];
		const ExprNode* occurrence=ic->clones[ic->clone_counter++];
		if (!clone.found(i)) {
			clone.insert(i,occurrence); // insert the first occurrence
		} else {
			clone[i] = occurrence;      // or replace the old one
		}
	} else {
		visit(i.expr);
		clone.insert(i, &(*clone[i.expr])[i.index]);
	}
}

// (useless so far)
void ExprSplitOcc::visit(const ExprNAryOp& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprLeaf& e) {
	e.acceptVisitor(*this);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprBinaryOp& b) {
	b.acceptVisitor(*this);
}

// (useless so far)
void ExprSplitOcc::visit(const ExprUnaryOp& u) {
	u.acceptVisitor(*this);
}


void ExprSplitOcc::visit(const ExprSymbol& e) {
	// each call to this function correspond
	// to a new occurrence of the symbol
	// that is placed in the the "clone" structure
	SymbolClone* sc = symbol_clone[e];
	const ExprNode* occurrence=sc->clones[sc->clone_counter++];
	if (!clone.found(e)) {
		clone.insert(e,occurrence); // insert the first occurrence
	} else {
		clone[e] = occurrence;      // or replace the old one
	}
}

void ExprSplitOcc::visit(const ExprConstant& c) {
	if (!clone.found(c)) {
		clone.insert(c,&c.copy());
	}
}

void ExprSplitOcc::visit(const ExprVector& e) {
	Array<const ExprNode> new_args(e.nb_args);
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		// warning: if the same symbol appears several times in e.args,
		// the node "clone[e.arg(i)]" change each time.
		new_args.set_ref(i,*clone[e.arg(i)]);
	}
	clone.insert(e, &ExprVector::new_(new_args,e.row_vector()));
}

void ExprSplitOcc::visit(const ExprApply& e) {
	const ExprNode* new_args[e.nb_args];
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		// warning: if the same symbol appears several times in e.args,
		// the node "clone[e.arg(i)]" change each time.
		new_args[i]=clone[e.arg(i)];
	}
	clone.insert(e, &ExprApply::new_(e.func,new_args));
}

void ExprSplitOcc::visit(const ExprChi& e) {
	const ExprNode* new_args[e.nb_args];
	for (int i=0; i<e.nb_args; i++) {
		visit(e.arg(i));
		// warning: if the same symbol appears several times in e.args,
		// the node "clone[e.arg(i)]" change each time.
		new_args[i]=clone[e.arg(i)];
	}
	clone.insert(e, &ExprChi::new_(new_args));
}

void ExprSplitOcc::binary_copy(const ExprBinaryOp& e, const ExprNode& (*f)(const ExprNode&, const ExprNode&)) {
	visit(e.left);
	const ExprNode& l=*clone[e.left];

	visit(e.right);
	// warning: if e.left is the same symbol as e.right,
	// now the node "clone" points to has changed.
	const ExprNode& r=*clone[e.right];

	clone.insert(e, &f(l,r));
}

void ExprSplitOcc::visit(const ExprPower& e) {
	visit(e.expr);
	clone.insert(e, &pow(*clone[e.expr],e.expon));
}

// warning: if e.left is the same symbol as e.right,
// now the node "clone" points to has changed.
#define BINARY_COPY(f) visit(e.left); \
		               const ExprNode& l=*clone[e.left]; \
		               visit(e.right); \
		               const ExprNode& r=*clone[e.right]; \
		               clone.insert(e, &f(l,r));

#define UNARY_COPY(f) visit(e.expr); \
					  clone.insert(e,&f(*clone[e.expr]));

void ExprSplitOcc::visit(const ExprAdd& e)   { BINARY_COPY(operator+); }
void ExprSplitOcc::visit(const ExprMul& e)   { BINARY_COPY(operator*); }
void ExprSplitOcc::visit(const ExprSub& e)   { BINARY_COPY(operator-); }
void ExprSplitOcc::visit(const ExprDiv& e)   { BINARY_COPY(operator/); }
void ExprSplitOcc::visit(const ExprMax& e)   { BINARY_COPY(max); }
void ExprSplitOcc::visit(const ExprMin& e)   { BINARY_COPY(min); }
void ExprSplitOcc::visit(const ExprAtan2& e) { BINARY_COPY(atan2); }
void ExprSplitOcc::visit(const ExprMinus& e) { UNARY_COPY(operator-); }
void ExprSplitOcc::visit(const ExprTrans& e) { UNARY_COPY(transpose); }
void ExprSplitOcc::visit(const ExprSign& e)  { UNARY_COPY(sign); }
void ExprSplitOcc::visit(const ExprAbs& e)   { UNARY_COPY(abs); }
void ExprSplitOcc::visit(const ExprSqr& e)   { UNARY_COPY(sqr); }
void ExprSplitOcc::visit(const ExprSqrt& e)  { UNARY_COPY(sqrt); }
void ExprSplitOcc::visit(const ExprExp& e)   { UNARY_COPY(exp); }
void ExprSplitOcc::visit(const ExprLog& e)   { UNARY_COPY(log); }
void ExprSplitOcc::visit(const ExprCos& e)   { UNARY_COPY(cos); }
void ExprSplitOcc::visit(const ExprSin& e)   { UNARY_COPY(sin); }
void ExprSplitOcc::visit(const ExprTan& e)   { UNARY_COPY(tan); }
void ExprSplitOcc::visit(const ExprCosh& e)  { UNARY_COPY(cosh); }
void ExprSplitOcc::visit(const ExprSinh& e)  { UNARY_COPY(sinh); }
void ExprSplitOcc::visit(const ExprTanh& e)  { UNARY_COPY(tanh); }
void ExprSplitOcc::visit(const ExprAcos& e)  { UNARY_COPY(acos); }
void ExprSplitOcc::visit(const ExprAsin& e)  { UNARY_COPY(asin); }
void ExprSplitOcc::visit(const ExprAtan& e)  { UNARY_COPY(atan); }
void ExprSplitOcc::visit(const ExprAcosh& e) { UNARY_COPY(acosh); }
void ExprSplitOcc::visit(const ExprAsinh& e) { UNARY_COPY(asinh); }
void ExprSplitOcc::visit(const ExprAtanh& e) { UNARY_COPY(atanh); }

} // end namespace ibex
