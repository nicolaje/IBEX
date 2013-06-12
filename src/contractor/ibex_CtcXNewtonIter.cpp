//============================================================================
//                                  I B E X                                   
// File        : ibex_XNewton.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_CtcXNewtonIter.h"
using namespace std;

namespace ibex {

const double CtcXNewtonIter::default_max_diam_deriv =1e6;


// the constructor
CtcXNewtonIter::CtcXNewtonIter(const System& sys1, std::vector<corner_point>& cpoints1,
		ctc_mode cmode, linear_mode lmode1, int max_iter1, double max_diam_deriv1, double max_diam_box):
			CtcLinearRelaxation(sys1,cmode,max_iter1,max_diam_box),
			cpoints(cpoints1),
			max_diam_deriv(max_diam_deriv1),
			lmode(lmode1),
			linear_coef(sys1.nb_ctr, sys1.nb_var) {

	last_rnd = new int[sys.nb_var];
	base_coin = new int[sys.nb_var];
	linear = new bool[sys.nb_ctr];

//	linear_coef= new IntervalMatrix(sys.nb_ctr, sys.nb_var);

	for(int ctr=0; ctr<sys.nb_ctr;ctr++){

		IntervalVector G(sys.nb_var);

		if(ctr==goal_ctr) {
			IntervalVector box1(sys.nb_var-1);
			for (int i=0; i<sys.nb_var-1; i++)
				box1[i]=sys.box[i];


			IntervalVector G1(sys.nb_var-1);
			sys.goal->gradient(box1,G1);
			for (int i=0; i<sys.nb_var-1; i++)
				G[i]=G1[i];
			G[goal_var]=0;  //G[sys.nb_var-1]=0;
		}
		else {
			sys.ctrs[ctr].f.gradient(sys.box,G);
		}

		linear[ctr]=true;
		// for testing if a function is linear (with scalar coefficients) w.r.t all its variables, we test the diameter of the gradient components.
		for(int i=0;i<sys.nb_var;i++)  {
			if (G[i].diam() >1e-10) {
				linear[ctr]=false;
				break;
			}

		}

		linear_coef.row(ctr) = G;  // in case of a linear function, the coefficients are already computed.
	}

}

CtcXNewtonIter::~CtcXNewtonIter() {
	delete[] last_rnd;
	delete[] base_coin;
	delete[] linear;
}


void CtcXNewtonIter::contract (IntervalVector & box){
	if (box.max_diam() > max_diam_box) return; // is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
	//		cout << " box before XNewtonIter " << box << endl;
	try {
		//returns the number of constraints in the linearized system
		int cont = linearization(box);

		if(cont<1)  return;
		optimizer(box);

		//	mylinearsolver->writeFile("LP.lp");
		//		system ("cat dump.lp");
		//cout << " box after  XNewtonIter " << box << endl;
		mylinearsolver->cleanConst();

	}
	catch(EmptyBoxException& ){
		box.set_empty(); // empty the box before exiting in case of EmptyBoxException
		mylinearsolver->cleanConst();
		throw EmptyBoxException();
	}

}


/*********generation of the linearized system*********/
int CtcXNewtonIter::linearization( IntervalVector & box)  {

	int cont =0;
	// Update the bounds the variables
	mylinearsolver->initBoundVar(box);


	// Create the linear relaxation of each constraint
	for(int ctr=0; ctr<sys.nb_ctr;ctr++){

		IntervalVector G(sys.nb_var);
		if (linear[ctr]) G= linear_coef.row(ctr); // constant derivatives have been already computed
		else if(lmode==TAYLOR){ //derivatives are computed once (Taylor)
			gradient_computation(box, G, ctr);
		}

		int nb_nonlinear_vars;
		if(cpoints[0]==K4){
			for(int j=0; j<4; j++)
				cont += X_Linearization(box, ctr, K4, G, j, nb_nonlinear_vars);
		}else  //  linearizations k corners per constraint
			for(int k=0;k<(cpoints.size());k++){
				cont += X_Linearization(box, ctr, cpoints[k],  G, k, nb_nonlinear_vars);

			}
	}
	return cont;
}


// TODO A quoi sert "nb_nonlinear_vars" ?
int CtcXNewtonIter::X_Linearization(IntervalVector& box, int ctr, corner_point cpoint,
		IntervalVector& G, int id_point, int& nb_nonlinear_vars){

	CmpOp op= sys.ctrs[ctr].op;

	if(op!=EQ && isInner(box, sys, ctr)) return 0; //the constraint is satified

	int cont=0;
	if(ctr==goal_ctr) op = LEQ;
	if(op==EQ){
		cont+=X_Linearization(box, ctr, cpoint, LEQ, G, id_point, nb_nonlinear_vars);
		cont+=X_Linearization(box, ctr, cpoint, GEQ, G, id_point, nb_nonlinear_vars);

	} else
		cont+=X_Linearization(box, ctr, cpoint, op,  G, id_point, nb_nonlinear_vars);

	return cont;
}

//return 0 only when the linearization is not performed
int CtcXNewtonIter::X_Linearization(IntervalVector& box,
		int ctr, corner_point cpoint, CmpOp op, 
		IntervalVector& G, int id_point, int& nb_nonlinear_vars) {
	int n = sys.nb_var;
	int nonlinear_var = 0;

	if (id_point != 0 && linear[ctr])
		return 0; // only one corner for a linear constraint
	//Linear variable y<=loup (should be y<=Sup(y)?)

	/*
	 bool* corner=NULL;
	 if(!linear[ctr]){
	 // best not implemented in version 2.0 BNE

	 if(cpoint==BEST){
	 corner= new bool[n];
	 best_corner(ctr, op, G, corner);
	 }

	 }
	 */
	IntervalVector savebox(box);
	Interval ev(0.0);
	Interval tot_ev(0.0);
	Vector row1(n);

	if (goal_ctr == ctr) {
		row1[goal_var] += -1.0;
	}
	for (int j = 0; j < n; j++) {
		if ((j == goal_var) && (goal_ctr > -1))
			continue; //the variable y!

		if (sys.ctrs[ctr].f.used(j)) {
			if (lmode == HANSEN && !linear[ctr]) {
				gradient_computation(box,G,ctr);
			}
		} 
		else  continue;

		if (G[j].diam() > max_diam_deriv) {
			box = savebox;
			return 0; //to avoid problems with SoPleX
		}

		if (linear[ctr])
			cpoint = INF_X;
		else if (G[j].diam() > 1e-10)
			nonlinear_var++;

		bool inf_x;

		/*  for GREEDY heuristics :not implemented in v2
		 IntervalVector save;
		 double fh_inf, fh_sup;
		 double D;
		 */

		switch (cpoint) {
		/*
		 case BEST:
		 inf_x=corner[j]; last_rnd[j]=inf_x? 0:1;  break;
		 */
		case INF_X:
			inf_x = true;
			break;
		case SUP_X:
			inf_x = false;
			break;
		case RANDOM:
			last_rnd[j] = rand();
			inf_x = (last_rnd[j] % 2 == 0);
			break;
		case K4:

			if (id_point == 0) {
				inf_x = (rand() % 2 == 0);
				base_coin[j] = inf_x;
			} else if (nb_nonlinear_vars < 3) {
				if (id_point == 1)
					inf_x = !base_coin[j];
				else
					return 0;
			} else if (G[j].diam() <= 1e-10) {
				inf_x = (rand() % 2 == 0);
			} else if (id_point == 1) {
				if (((double) nonlinear_var) <= (((double) nb_nonlinear_vars )/ 3.0))
					inf_x = base_coin[j];
				else
					inf_x = !base_coin[j];
			} else if (id_point == 2) {
				if (((double) nonlinear_var )> ((double) nb_nonlinear_vars )/ 3.0
						&& (double) nonlinear_var
						<= 2 * (double) nb_nonlinear_vars / 3.0)
					inf_x = base_coin[j];
				else
					inf_x = !base_coin[j];
			} else if (id_point == 3) {
				if (((double) nonlinear_var)
						> 2 * ((double) nb_nonlinear_vars) / 3.0)
					inf_x = base_coin[j];
				else
					inf_x = !base_coin[j];
			}
			break;
		case RANDOM_INV:
			inf_x = (last_rnd[j] % 2 != 0);
			break;
		case NEG:
			inf_x = (last_rnd[j] % 2 != 0);
			break;
			/*  not implemented in v2.0
             case GREEDY1:
	       inf_x=((abs(Inf(G(j+1))) < abs(Sup(G(j+1))) && (op == LEQ || op== LT)) ||
                        (abs(Inf(G(j+1))) >= abs(Sup(G(j+1))) && (op == GEQ || op== GT))  )? true:false;
	       break;
	  case MONO:
	    if (ctr==goal_ctr && ((Inf(G(j+1)) >0 && (op == LEQ || op== LT)) || 
				  (Sup (G(j+1)) < 0 && (op == GEQ || op== GT))))
	      inf_x = true;
	    else if 
	      (ctr == goal_ctr  &&
	      ((Sup(G(j+1)) <0 && (op == LEQ || op== LT)) || 
	       (Inf(G(j+1)) > 0 && (op == GEQ || op== GT))))
	      inf_x=false;
	    else inf_x = (rand()%2==0);
	    last_rnd[j]=inf_x? 0:1;
	    break;
	  case NEGMONO:
	    if ((Inf(G(j+1)) >0 && (op == LEQ || op== LT)) || 
		(Sup (G(j+1)) < 0 && (op == GEQ || op== GT)))
	      inf_x = false;
	    else if 
	      ((Sup(G(j+1)) <0 && (op == LEQ || op== LT)) || 
	       (Inf(G(j+1)) > 0 && (op == GEQ || op== GT)))
	      inf_x=true;
	    else inf_x = (rand()%2==0);
	    last_rnd[j]=inf_x? 0:1;
	    break;

			 */


			/*
              case GREEDY7:
                 //select the coin nearest to the loup 
                 if(goal_ctr!=-1 && Dimension(Optimizer::global_optimizer())>0){
//                     if(j==0)cout<< Optimizer::global_optimizer()<<end;
                    inf_x=(abs(Optimizer::global_optimizer()(j+1)-Inf(savebox(j+1))) < 
                      abs(Optimizer::global_optimizer()(j+1)-Sup(savebox(j+1))))? true:false;
                 }else{ 
                   inf_x=(rand()%2==0);
		   }
                 break;
			 */
			/*
             case GREEDY6:
                 save=space.box;
                 fh_inf, fh_sup;
                 D=Diam(savebox(j+1));
                 space.box=Mid(space.box);
                 space.box(j+1)=Inf(savebox(j+1));
                 fh_inf=Mid(sys.ctr(ctr).eval(space));
                 space.box(j+1)=Sup(savebox(j+1));
                 fh_sup=Mid(sys.ctr(ctr).eval(space));
                 if (op == LEQ || op== LT)
                    inf_x=((fh_sup-fh_inf)/(REAL)(n-1) - 0.5*D*(Inf(G(j+1))+Sup(G(j+1)))  > 0)? false:true;
                 else
                    inf_x=((fh_sup-fh_inf)/(REAL)(n-1) - 0.5*D*(Inf(G(j+1))+Sup(G(j+1))) < 0)? false:true;
		 last_rnd[j]=inf_x? 0:1;  
		 space.box=save;
              break;
             case GREEDY5:
                 save=space.box;
                 fh_inf, fh_sup;
                 D=Diam(savebox(j+1));
                 space.box=Mid(space.box);
                 space.box(j+1)=Inf(savebox(j+1));
                 fh_inf=Mid(sys.ctr(ctr).eval(space));
                 space.box(j+1)=Sup(savebox(j+1));
                 fh_sup=Mid(sys.ctr(ctr).eval(space));
                 if (op == LEQ || op== LT)
                    inf_x=(fh_sup-fh_inf - 0.5*D*(Inf(G(j+1))+Sup(G(j+1)))  > 0)? false:true;
                 else
                    inf_x=(fh_sup-fh_inf - 0.5*D*(Inf(G(j+1))+Sup(G(j+1))) < 0)? false:true;
		 last_rnd[j]=inf_x? 0:1;  
		 space.box=save;
              break;
			 */
		default:
			last_rnd[j] = rand();
			inf_x = (last_rnd[j] % 2 == 0);
			break;

		}

		//      cout << " j " << j <<  " " << savebox[j] << G[j] << endl;
		box[j]=inf_x? savebox[j].lb():savebox[j].ub();
		Interval a = ((inf_x && (op == LEQ || op== LT)) ||
				(!inf_x && (op == GEQ || op== GT)))	? G[j].lb() : G[j].ub();
		row1[j] +=  a.mid();
		ev -= a*box[j];

	}

	// cout << " ev  " << ev << endl;
	/*  used in BEST not implemented in v2.0
	if(corner) delete[] corner;
	 */
	if(ctr==goal_ctr)
		ev+= sys.goal->eval(box);
	else
		ev+= sys.ctrs[ctr].f.eval(box);

	if(id_point==0) nb_nonlinear_vars=nonlinear_var;


	for(int j=0;j<n;j++)
		tot_ev+=row1[j]*savebox[j]; //natural evaluation of the left side of the linear constraint


	bool added=false;
	if(op ==LEQ || op== LT){
		//g(xb) + a1' x1 + ... + an xn <= 0
		if(tot_ev.lb()>(-ev).ub())   throw EmptyBoxException(); //the constraint is not satisfied
		if((-ev).ub() <tot_ev.ub()){ //otherwise the constraint is satisfied for any point in the box
			mylinearsolver->addConstraint( row1, LEQ, (-ev).ub());
			added=true;
		}
	}else{
		if(tot_ev.ub()<(-ev).lb())  throw EmptyBoxException();
		if((-ev).lb()>tot_ev.lb()){
			mylinearsolver->addConstraint( row1, GEQ, (-ev).lb() );
			added=true;
		}
	}

	box=savebox;


	return (added)? 1:0;



}



/* Computes the gradient G of the constraint ctr : special case if ctr==ctr */

void CtcXNewtonIter::gradient_computation (IntervalVector& box, IntervalVector& G, int ctr)
{
	if(goal_ctr==ctr)  // objective function  in optimization
	{
		IntervalVector box1(sys.nb_var-1);
		for (int i=0; i<sys.nb_var-1; i++)
			box1[i]=box[i];

		IntervalVector G1(sys.nb_var-1);
		sys.goal->gradient(box1,G1);
		for (int i=0; i<sys.nb_var-1; i++)
			G[i]=G1[i];

		G[goal_var]=0; //G[sys.nb_var-1]=0;
	}

	else
		sys.ctrs[ctr].f.gradient(box,G);
}





/*  pas implanté en version 2

void CtcXNewtonIter::best_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner){
   int n=sys.nb_var;

   int total_backtracks=1; int nb_var=0;
   for(int i=0;i<n;i++){
      corner[i]=false;
      if(i==n-1 && goal_ctr!=-1) continue;
      if(!isvar[ctr][i]) continue;
      if(Diam(G(i+1))<=1e-11) continue; //linear variable
      total_backtracks*=2;
      nb_var++;
   }

   REAL best_eval=eval_corner(ctr, op, G, corner);
   bool corner_tmp[n];

   for(int i=1;i<total_backtracks;i++){
      int ii=i;

      for(int j=0;j<nb_var;j++){
         if(j==n-1 && goal_ctr!=-1) continue;
         if(!isvar[ctr][j]) continue;
         if(Diam(G(j+1))<=1e-11) continue; //linear variable
         corner_tmp[j]=(ii%2==1);
         ii/=2;
      }
      REAL eval=eval_corner(ctr, op, G, corner_tmp);
      if(eval>best_eval){
         best_eval=eval;
         for(int k=0;k<n;k++){
            corner[k]=corner_tmp[k];
         }
      }
   }
}


REAL CtcXNewtonIter::eval_corner(int ctr, int op, INTERVAL_VECTOR& G, bool* corner){
   INTERVAL_VECTOR savebox(sys.box);
   INTERVAL ev(0.0);
   int n=sys.nb_var;

   for (int j=0; j<n; j++){
      if(j==n-1 && goal_ctr!=-1) continue; //the variable y!
      if(!isvar[ctr][j]) continue;

      if(Diam(G(j+1))> max_diam_deriv)
	{
         sys.box=savebox;
         return 0; //to avoid problems with SoPleX
	}

      bool inf_x=corner[j];
      sys.box[j]=inf_x? Inf(savebox(j+1)):Sup(savebox(j+1));
      INTERVAL a = ((inf_x && (op == LEQ || op== LT)) ||
            (!inf_x && (op == GEQ || op== GT)))? Inf(G(j+1)):Sup(G(j+1));
      ev+=(inf_x)? 0.5*a*Diam(savebox(j+1)):-0.5*a*Diam(savebox(j+1));
   }

    //f(x^c)
   try {
   if(ctr==goal_ctr){
      goal->forward(space);
      ev+=goal->output();
   }else{
      ev+=sys.ctr(ctr).eval(space);

   }
   }
   catch (EmptyBoxException& )
     {cout  << endl; space.box=savebox; return 0;}

   space.box=savebox;
   return Mid(ev);

}
 */



}
