//============================================================================
//                                  I B E X                                   
// File        : ibex_System.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 12, 2012
// Last Update : Jun 12, 2012
//============================================================================

#include "ibex_System.h"
#include "ibex_SyntaxError.h"
#include "ibex_UnknownFileException.h"
#include "ibex_ExprCopy.h"
#include "ibex_SystemCopy.cpp_"
#include "ibex_SystemNormalize.cpp_"
#include "ibex_SystemMerge.cpp_"
#include <stdio.h>
#include <sstream>
#include <vector>

extern int ibexparse();
extern void ibexparse_string(const char* syntax);
//extern int ibex_lineno;
extern void ibexrestart(FILE *);
extern FILE* ibexin;

using namespace std;

namespace ibex {

namespace parser {
extern System* system;
}

System::System() : nb_var(0), nb_ctr(0), box(1) /* tmp */ {

}

System::System(const char* filename) : nb_var(0), nb_ctr(0), box(1) /* tmp */ {
	FILE *fd;
	if ((fd = fopen(filename, "r")) == NULL) throw UnknownFileException(filename);
	load(fd);
}

System::System(int n, const char* syntax) : nb_var(n), /* NOT TMP (required by parser) */
		                                    nb_ctr(0), box(1) /* tmp */ {
	try {
		parser::system=this;
		ibexparse_string(syntax);
		parser::system=NULL;
	} catch(SyntaxError& e) {
		parser::system=NULL;
		throw e;
	}
}

System::System(const System& sys, copy_mode mode) : nb_var(0), nb_ctr(0), func(0), box(1) {

	switch(mode) {
	case COPY :      init(SystemCopy(sys,COPY)); break;
	case NORMALIZE : init(SystemNormalize(sys)); break;
	case INEQ:       init(SystemCopy(sys,INEQ)); break;
	}

}

System::System(const System& sys1, const System& sys2) : nb_var(0), nb_ctr(0), func(0), box(1) {
	init(SystemMerge(sys1,sys2));
}

std::ostream& operator<<(std::ostream& os, const System& sys) {

	os << "variables: " << endl << "  ";
	for (int i=0; i<sys.args.size(); i++)
		os << sys.args[i] << " ";
	os << endl;

	os << "goal: " << endl;
	if (sys.goal!=NULL)
	    os << "  " << sys.goal->expr() << endl;
	else
		os << "  (none)" << endl;

	os << "main func size= " << sys.f.expr().size << endl;

	os << "constraints:" << endl;
	for (int i=0; i<sys.ctrs.size(); i++)
		os << "  " << sys.ctrs[i] << " " << sys.ctrs[i].f.expr().size << endl;
	os << endl;

	return os;
}

void System::load(FILE* fd) {
	ibexin = fd;

	try {
		parser::system=this;
		ibexparse();
		parser::system=NULL;
	}

	catch(SyntaxError& e) {
		parser::system=NULL;
		fclose(fd);
		ibexrestart(ibexin);
		throw e;
	}

	fclose(fd);
}

System::~System() {
	for (int i=0; i<func.size(); i++)
		delete &func[i];

	if (goal) delete goal;

	for (int i=0; i<ctrs.size(); i++)
		delete &ctrs[i];
}

} // end namespace ibex
