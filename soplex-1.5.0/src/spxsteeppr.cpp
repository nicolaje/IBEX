/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the class library                   */
/*       SoPlex --- the Sequential object-oriented simPlex.                  */
/*                                                                           */
/*    Copyright (C) 1996      Roland Wunderling                              */
/*                  1996-2010 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SoPlex is distributed under the terms of the ZIB Academic Licence.       */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SoPlex; see the file COPYING. If not email to soplex@zib.de.  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma ident "@(#) $Id: spxsteeppr.cpp,v 1.43 2010/09/16 17:45:04 bzfgleix Exp $"

//#define DEBUGGING 1

#include <assert.h>
#include <iostream>

#include "spxdefines.h"
#include "message.h"
#include "spxsteeppr.h"
#include "random.h"

namespace soplex
{

// #define EQ_PREF 1000

void SPxSteepPR::clear()
{
   thesolver = 0;
   prefSetup = 0;
}

void SPxSteepPR::load(SPxSolver* base)
{
   thesolver = base;

   if (base)
   {
      workVec.clear();
      workVec.reDim(base->dim());
      workRhs.clear();
      workRhs.reDim(base->dim());

      leavePref.reSize(base->dim());
      coPref.reSize (base->dim());
      pref.reSize (base->coDim());
      prefSetup = 0;
   }
}

void SPxSteepPR::setType(SPxSolver::Type type)
{
   int i;

   workRhs.setEpsilon(thesolver->epsilon());

   pref.reSize (thesolver->coDim());
   coPref.reSize(thesolver->dim());
   setupPrefs(type);

   if (setup == DEFAULT)
   {
      if (type == SPxSolver::ENTER)
      {
         coPenalty.reDim(thesolver->dim());
         for (i = thesolver->dim() - 1; i >= 0; --i)
            // coPenalty[i] = 10;
            coPenalty[i] = 2;
         penalty.reDim(thesolver->coDim());
         for (i = thesolver->coDim() - 1; i >= 0; --i)
            // penalty[i] = 10;
            penalty[i] = 1;
         // penalty[i] = 1 + thesolver->vector(i).size() / thesolver->dim();
      }
      else
      {
         assert(type == SPxSolver::LEAVE);
         coPenalty.reDim(thesolver->dim());
         for (i = thesolver->dim() - 1; i >= 0; --i)
         {
            // coPenalty[i] = 1;
            const SPxId id = thesolver->basis().baseId(i);
            const int n    = thesolver->number(id);
            assert(n >= 0);
            leavePref[i]   = thesolver->isId(id) ? pref[n] : coPref[n];
            coPenalty[i]   = 1.0 + thesolver->basis().baseVec(i).size() / Real(thesolver->dim());
         }
      }
   }
   else
   {
      MSG_ERROR( spxout << "ESTEEP01 sorry, no exact setup for steepest "
                        << "edge multipliers implemented" << std::endl; )

      if (type == SPxSolver::ENTER)
      {
         coPenalty.reDim(thesolver->dim());
         for (i = thesolver->dim() - 1; i >= 0; --i)
            coPenalty[i] = 1;
         penalty.reDim(thesolver->coDim());
         for (i = thesolver->coDim() - 1; i >= 0; --i)
            penalty[i] = 1 + thesolver->vector(i).length2();
      }
      else
      {
         assert(type == SPxSolver::LEAVE);
         coPenalty.reDim(thesolver->dim());
         for (i = thesolver->dim() - 1; i >= 0; --i)
         {
            coPenalty[i] = 1 + thesolver->basis().baseVec(i).size()
                           / Real(thesolver->dim());
         }
      }
   }
   workVec.clear();
   workRhs.clear();
}

void SPxSteepPR::setupPrefsX(
   Real mult, 
   Real /*tie*/, 
   Real /*cotie*/,
   Real shift, 
   Real coshift)
{
   DataArray<Real>* p;
   DataArray<Real>* cp;
   // Real rtie;
   // Real ctie;
   Real rshift;
   Real cshift;
   int  i;

   if (thesolver->rep() == SPxSolver::COLUMN)
   {
      cp = &pref;
      p  = &coPref;
      // ctie = tie;
      // rtie = cotie;
      cshift = shift;
      rshift = coshift;
   }
   else
   {
      p  = &pref;
      cp = &coPref;
      // rtie = tie;
      // ctie = cotie;
      rshift = shift;
      cshift = coshift;
   }

   //      p[i] += rtie * thesolver->rowVector(i).size() / Real(thesolver->nCols());
   //      p[i] += EQ_PREF * (thesolver->rhs(i) == thesolver->lhs(i));
   //      p[i] += EQ_PREF * (thesolver->rhs(i) >=  infinity
   //                     &&  thesolver->lhs(i) <= -infinity);
   for(i = 0; i < thesolver->nRows(); ++i)
      (*p)[i] = rshift;

   //      cp[i] += ctie * thesolver->colVector(i).size() / Real(thesolver->nRows());
   //      cp[i] += EQ_PREF * (thesolver->upper(i) == thesolver->lower(i));
   //      cp[i] += EQ_PREF * (thesolver->upper(i) >=  infinity
   //                      &&  thesolver->lower(i) <= -infinity);
   for(i = 0; i < thesolver->nCols(); ++i)
      (*cp)[i] = cshift;

   for(i = 0; i < coPref.size(); ++i)
      coPref[i] *= 1.0 - mult * i;

   for(i = 0; i < pref.size(); ++i)
      pref[i] *= 1.0 + mult * i;
}

void SPxSteepPR::setupPrefs(SPxSolver::Type tp)
{
   if (tp != prefSetup)
   {
      Real mult = 1e-8 / Real(1 + thesolver->dim() + thesolver->coDim());

      if (tp == SPxSolver::ENTER)
         setupPrefsX(-mult, -1e-5, -1e-5, 1.0, 1.0);
      else
         setupPrefsX(mult, 1e-5, 1e-5, 1.0, 1.0);

      prefSetup = tp;
   }
}

void SPxSteepPR::setRep(SPxSolver::Representation)
{
   if (workVec.dim() != thesolver->dim())
   {
      DVector tmp = penalty;
      penalty = coPenalty;
      coPenalty = tmp;

      workVec.clear();
      workVec.reDim(thesolver->dim());
   }
}

void SPxSteepPR::left4(int n, SPxId id)
{
   assert(thesolver->type() == SPxSolver::LEAVE);

   //  Update preference multiplier in #leavePref#
   if (thesolver->isId(id))
      leavePref[n] = pref[thesolver->number(id)];
   else if (thesolver->isCoId(id))
      leavePref[n] = coPref[thesolver->number(id)];

   if (id.isValid())
   {
      // Real               delta         = 0.1;   // thesolver->epsilon();
      Real        delta         = 0.1 + 1.0 / thesolver->basis().iteration();
      Real*       coPenalty_ptr = coPenalty.get_ptr();
      const Real* workVec_ptr   = workVec.get_const_ptr();
      const Real* rhoVec        = thesolver->fVec().delta().values();
      Real        rhov_1        = 1.0 / rhoVec[n];
      Real        beta_q        = thesolver->coPvec().delta().length2() * rhov_1 * rhov_1;

      //TK: I gave the 0.5 extra, because I am not sure how hard this assert is.
#ifndef NDEBUG
      if (fabs(rhoVec[n]) < theeps * 0.5)
      {
         MSG_ERROR( spxout << "WSTEEP04: rhoVec = "
                           << rhoVec[n] << " with smaller absolute value than 0.5*theeps = " << 0.5*theeps << std::endl; )
      }
#endif  // NDEBUG

      //  Update #coPenalty# vector
      const IdxSet& rhoIdx = thesolver->fVec().idx();
      int           len    = thesolver->fVec().idx().size();

      for(int i = 0; i < len; ++i)
      {
         int  j = rhoIdx.index(i);
         
         coPenalty_ptr[j] += rhoVec[j] * (beta_q * rhoVec[j] - 2.0 * rhov_1 * workVec_ptr[j]);

         if (coPenalty_ptr[j] < delta)
            coPenalty_ptr[j] = delta; // coPenalty_ptr[j] = delta / (1+delta-x);
         else if (coPenalty_ptr[j] >= infinity)
            coPenalty_ptr[j] = 1.0 / theeps;
      }
      coPenalty_ptr[n] = beta_q;
      //@ coPenalty_ptr[n] = 0.999*beta_q;
      //@ coPenalty_ptr[n] = 1.001*beta_q;
   }
}

int SPxSteepPR::selectLeave()
{
   assert(isConsistent());

   const Real* coPenalty_ptr = coPenalty.get_const_ptr();
   const Real* fTest         = thesolver->fTest().get_const_ptr();
   //    const Real* low     = thesolver->lbBound();
   //    const Real* up      = thesolver->ubBound();
   const Real* p             = leavePref.get_const_ptr();

   Real best = -infinity;
   Real x;

   int lastIdx = -1;

   for (int i = thesolver->dim() - 1; i >= 0; --i)
   {
      x = fTest[i];

      if (x < -theeps)
      {         
         /**@todo this was an assert! is an assertion correct?*/
         // assert(coPenalty_ptr[i] >= theeps);
         if( coPenalty_ptr[i] < theeps )
         {
#if ENABLE_ADDITIONAL_CHECKS
            MSG_WARNING( spxout << "WSTEEP02 SPxSteepPR::selectLeaveX(): coPenalty too small ("
                                << coPenalty_ptr[i] << "), assuming epsilon (" << theeps << ")!" << std::endl; )
#endif

            x = x * x / theeps * p[i];
         }
         else
            x = x * x / coPenalty_ptr[i] * p[i];

         if (x > best)
         {
            best = x;
            lastIdx = i;
         }
      }
   }

   if (lastIdx >= 0)
   {
      assert( thesolver->coPvec().delta().isConsistent() );
      thesolver->basis().coSolve(thesolver->coPvec().delta(),
                                 thesolver->unitVector(lastIdx));
      workRhs.setEpsilon(accuracy);
      assert( thesolver->coPvec().delta().isConsistent() );
      workRhs.setup_and_assign(thesolver->coPvec().delta());
      thesolver->setup4solve(&workVec, &workRhs);
   }

   return lastIdx;
}


/* Entering Simplex
 */
void SPxSteepPR::entered4(SPxId /* id */, int n)
{
   assert(thesolver->type() == SPxSolver::ENTER);

   if (n >= 0 && n < thesolver->dim())
   {
      Real delta = 2 + 1.0 / thesolver->basis().iteration();
      Real* coPenalty_ptr = coPenalty.get_ptr();
      Real* penalty_ptr = penalty.get_ptr();
      const Real* workVec_ptr = workVec.get_const_ptr();
      const Real* pVec = thesolver->pVec().delta().values();
      const IdxSet& pIdx = thesolver->pVec().idx();
      const Real* coPvec = thesolver->coPvec().delta().values();
      const IdxSet& coPidx = thesolver->coPvec().idx();
      Real xi_p = 1 / thesolver->fVec().delta()[n];
      int i, j;
      Real xi_ip, x;

      assert(thesolver->fVec().delta()[n] > thesolver->epsilon()
              || thesolver->fVec().delta()[n] < -thesolver->epsilon());

      for (j = coPidx.size() - 1; j >= 0; --j)
      {
         i = coPidx.index(j);
         xi_ip = xi_p * coPvec[i];
         x = coPenalty_ptr[i] += xi_ip * (xi_ip * pi_p - 2 * workVec_ptr[i]);
         /*
         if(x < 1)
             coPenalty_ptr[i] = 1 / (2-x);
         */
         if (x < delta)
            coPenalty_ptr[i] = delta;
         // coPenalty_ptr[i] = 1;
         else if (x > infinity)
            coPenalty_ptr[i] = 1 / thesolver->epsilon();
      }

      for (j = pIdx.size() - 1; j >= 0; --j)
      {
         i = pIdx.index(j);
         xi_ip = xi_p * pVec[i];
         x = penalty_ptr[i] += xi_ip * (xi_ip * pi_p - 2.0 * (thesolver->vector(i) * workVec));
         /*
         if(x < 1)
             penalty_ptr[i] = 1 / (2-x);
         */
         if (x < delta)
            penalty_ptr[i] = delta;
         // penalty_ptr[i] = 1;
         else if (x > infinity)
            penalty_ptr[i] = 1.0 / thesolver->epsilon();
      }
   }

   /*@
       if(thesolver->isId(id))
           penalty[   thesolver->number(id) ] *= 1.0001;
       else if(thesolver->isCoId(id))
           coPenalty[ thesolver->number(id) ] *= 1.0001;
   */

}

SPxId SPxSteepPR::selectEnter()
{
   const Real* p             = pref.get_const_ptr();
   const Real* cp            = coPref.get_const_ptr();
   const Real* test          = thesolver->test().get_const_ptr();
   const Real* coTest        = thesolver->coTest().get_const_ptr();
   const Real* penalty_ptr   = penalty.get_const_ptr();
   const Real* coPenalty_ptr = coPenalty.get_const_ptr();

   SPxId lastId;
   Real  best = -infinity;
   Real  x;
   int   i;
   int   end;

   for(i = 0, end = thesolver->coDim(); i < end; ++i)
   {
      x = test[i];

      if (x < -theeps)
      {
         x *= x / penalty_ptr[i];
         x *= p[i];
         // x *= 1 + p[i];
         if (x > best)
         {
            best   = x;
            lastId = thesolver->id(i);
         }
      }
   }
   for (end = thesolver->dim(), i = 0; i < end; ++i)
   {
      x = coTest[i];
      if (x < -theeps)
      {
         x *= x / coPenalty_ptr[i];
         x *= cp[i];
         // x *= 1 + cp[i];
         if (x > best)
         {
            best = x;
            lastId = thesolver->coId(i);
         }
      }
   }
   assert(isConsistent());

   if (lastId.isValid())
   {
      SSVector& delta = thesolver->fVec().delta();

      thesolver->basis().solve4update(delta, thesolver->vector(lastId));

      // workRhs.epsilon = 0.1*accuracy;
      workRhs.setEpsilon(accuracy);
      workRhs.setup_and_assign(delta);
      pi_p = 1 + delta.length2();

      thesolver->setup4coSolve(&workVec, &workRhs);
   }
   return lastId;
}

void SPxSteepPR::addedVecs(int n)
{
   n = penalty.dim();
   pref.reSize (thesolver->coDim());
   penalty.reDim(thesolver->coDim());

   if (thesolver->type() == SPxSolver::ENTER)
   {
      setupPrefs(thesolver->type());
      for (; n < penalty.dim(); ++n)
         penalty[n] = 2;
   }
   prefSetup = 0;
}

void SPxSteepPR::addedCoVecs(int n)
{
   n = coPenalty.dim();

   leavePref.reSize(thesolver->dim());
   coPref.reSize (thesolver->dim());
   setupPrefs(thesolver->type());

   workVec.reDim (thesolver->dim());
   coPenalty.reDim (thesolver->dim());
   for (; n < coPenalty.dim(); ++n)
      coPenalty[n] = 1;
   prefSetup = 0;
}

void SPxSteepPR::removedVec(int i)
{
   assert(thesolver != 0);
   penalty[i] = penalty[penalty.dim()];
   penalty.reDim(thesolver->coDim());
   prefSetup = 0;
}

void SPxSteepPR::removedVecs(const int perm[])
{
   assert(thesolver != 0);
   if (thesolver->type() == SPxSolver::ENTER)
   {
      int i;
      int j = penalty.dim();
      for (i = 0; i < j; ++i)
         if (perm[i] >= 0)
            penalty[perm[i]] = penalty[i];
   }
   penalty.reDim(thesolver->coDim());
   prefSetup = 0;
}

void SPxSteepPR::removedCoVec(int i)
{
   assert(thesolver != 0);
   coPenalty[i] = coPenalty[coPenalty.dim()];
   coPenalty.reDim(thesolver->dim());
   prefSetup = 0;
}

void SPxSteepPR::removedCoVecs(const int perm[])
{
   assert(thesolver != 0);
   int i;
   int j = coPenalty.dim();
   for (i = 0; i < j; ++i)
      if (perm[i] >= 0)
         coPenalty[perm[i]] = coPenalty[i];
   coPenalty.reDim(thesolver->dim());
   prefSetup = 0;
}

#ifndef NO_CONSISTENCY_CHECKS
bool SPxSteepPR::isConsistent() const
{
   if (thesolver != 0 && thesolver->type() == SPxSolver::LEAVE && setup == EXACT)
   {
      int i;
      SSVector tmp(thesolver->dim(), thesolver->epsilon());
      Real x;
      for (i = thesolver->dim() - 1; i >= 0; --i)
      {
         thesolver->basis().coSolve(tmp, thesolver->unitVector(i));
         x = coPenalty[i] - tmp.length2();
         if (x > thesolver->delta() || -x > thesolver->delta())
         {
            MSG_ERROR( spxout << "ESTEEP03 x[" << i << "] = " << x << std::endl; )
         }
      }
   }

   if (thesolver != 0 && thesolver->type() == SPxSolver::ENTER)
   {
      int i;
      for (i = thesolver->dim() - 1; i >= 0; --i)
         if (coPenalty[i] < thesolver->epsilon())
            return MSGinconsistent("SPxSteepPR");

      for (i = thesolver->coDim() - 1; i >= 0; --i)
         if (penalty[i] < thesolver->epsilon())
            return MSGinconsistent("SPxSteepPR");
   }
   return true;
}
#endif
} // namespace soplex

//-----------------------------------------------------------------------------
//Emacs Local Variables:
//Emacs mode:c++
//Emacs c-basic-offset:3
//Emacs tab-width:8
//Emacs indent-tabs-mode:nil
//Emacs End:
//-----------------------------------------------------------------------------
