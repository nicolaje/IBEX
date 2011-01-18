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
#pragma ident "@(#) $Id: svset.cpp,v 1.34 2010/09/16 17:45:04 bzfgleix Exp $"

#include <assert.h>

#include "spxdefines.h"
#include "svset.h"
#include "message.h"

namespace soplex
{

void SVSet::ensureMem(int n)
{
   if (memSize() + n > memMax())
   {
      int newMax = int( memFactor * memMax() );
      if ( memSize() + n > newMax )
         newMax  = memSize() + n;
      memRemax( newMax );
   }
}

void SVSet::add(const SVector svec[], int n)
{
   assert(n >= 0);

   int i, len;
   for (i = len = 0; i < n; ++i)
      len += svec[i].size();

   ensurePSVec(n);
   ensureMem(len + n);

   for (i = 0; i < n; ++i)
      *create(svec[i].size()) = svec[i];
}

void SVSet::add(DataKey nkey[], const SVector svec[], int n)
{
   add(svec, n);
   for (int i = num() - 1; --n; --i)
      nkey[n] = key(i);
}

void SVSet::add(const SVSet& pset)
{
   int i, n, len;
   n = pset.num();
   for (i = len = 0; i < n; ++i)
      len += pset[i].size();

   ensurePSVec(n);
   ensureMem(len + n);

   for (i = 0; i < n; ++i)
      *create(pset[i].size()) = pset[i];
}

void SVSet::add(DataKey nkey[], const SVSet& pset)
{
   add(pset);

   int i = size();
   int n = pset.size();

   while(n > 0)
      nkey[--n] = key(--i);
}

SVector* SVSet::create(int idxmax)
{
   DLPSV* ps;

   if (list.last())
   {
      ps = list.last();
      removeLast(ps->max() - ps->size());
      ps->set_max( ps->size() );
   }

   if (idxmax < 0)
   {
      ensureMem(2);
      idxmax = memMax() - memSize() - 1;
   }
   else
      ensureMem(idxmax + 1);

   ensurePSVec(1);
   
   // We must call ensureMem() before insert() below, since insert() doesn't
   // know about the pointers into the NZE memory and therefore doesn't update
   // them if a realloc is necessary.
   ensureMem( memSize() + idxmax + 1 );
   assert( memMax() >= memSize() + idxmax + 1 );
   
   ps = set.create();
   list.append(ps);
   insert(memSize(), idxmax + 1);
   ps->setMem(idxmax + 1, &last() - idxmax);
   return ps;
}

SVector* SVSet::create(DataKey& nkey, int idxmax)
{
   SVector* ps = create(idxmax);
   nkey = key(num() - 1);
   return ps;
}

void SVSet::xtend(SVector& svec, int newmax)
{
   if (svec.max() < newmax)
   {
      if (possiblyUnusedMem * memFactor > memSize())
         memPack(); 

      assert(has(&svec));
      DLPSV* ps = static_cast<DLPSV*>( & svec );

      if (ps == list.last())
      {
         int sz = ps->size();
         ensureMem (newmax - ps->max() + 1);
         insert(memSize(), newmax - ps->max());
         ps->setMem (newmax + 1, ps->mem());
         ps->set_size( sz );
      }
      else
      {
         ensureMem(newmax + 1);
         SVector newps(newmax + 1, &last() + 1);
         int sz = ps->size();
         insert(memSize(), newmax + 1);
         newps = svec;

         if (ps != list.first())
         {
            SVector* prev = ps->prev();
            int prevsz = prev->size();
            prev->setMem (prev->max()
                           + ps->max() + 2, prev->mem());
            prev->set_size(prevsz);
            
            possiblyUnusedMem += ps->max();
         }
         list.remove(ps);
         list.append(ps);

         ps->setMem(newmax + 1, newps.mem());
         ps->set_size(sz);
      }
   }
}

void SVSet::add2(SVector &svec, int idx, Real val)
{
   xtend(svec, svec.size() + 1);
   svec.add(idx, val);
}

void SVSet::add2(SVector &svec, int n, const int idx[], const Real val[])
{
   xtend(svec, svec.size() + n);
   svec.add(n, idx, val);
}

void SVSet::remove(const DataKey& removekey)
{
   DLPSV* ps = &set[removekey];

   if (list.last() == ps)
      removeLast(ps->max() + 1);

   else if (list.first() != ps)
   {
      SVector* prev = ps->prev();
      int sz = prev->size();
      prev->setMem (prev->max() + ps->max() + 2, prev->mem());
      prev->set_size(sz);
   }
   list.remove(ps);
   set.remove(removekey);
}

void SVSet::remove(int perm[])
{
   int j = num();

   for (int i = 0; i < j; ++i)
   {
      if (perm[i] < 0)
      {
         DLPSV* ps = &set[i];

         if (list.last() == ps)
            removeLast(ps->max() + 1);

         else if (list.first() != ps)
         {
            SVector* prev = ps->prev();
            int sz = prev->size();
            prev->setMem (prev->max() + ps->max() + 2, prev->mem());
            prev->set_size(sz);
         }
         list.remove(ps);
      }
   }
   set.remove(perm);
}

void SVSet::remove(const DataKey keys[], int n, int* perm)
{
   for (int i = num() - 1; i >= 0; --i)
      perm[i] = i;
   while (n--)
      perm[ number(*keys++) ] = -1;
   remove(perm);
}

void SVSet::remove(const int nums[], int n, int* perm)
{
   for (int i = num() - 1; i >= 0; --i)
      perm[i] = i;
   while (n--)
      perm[ *nums++ ] = -1;
   remove(perm);
}


/*      \SubSection{Memory Management}
 */
void SVSet::reMax(int newmax)
{
   list.move(set.reMax(newmax));
}

void SVSet::memRemax(int newmax)
{
   ptrdiff_t delta = DataArray < SVector::Element > ::reMax(newmax);

   if (delta != 0)
   {
      for (DLPSV* ps = list.first(); ps; ps = list.next(ps))
      {
         // Extract the size and maximum capacity of the SVector, which for some reason is stored
         // as the first element.
         SVector::Element * info = reinterpret_cast<SVector::Element*>(reinterpret_cast<char*>(ps->mem()) + delta);
         int sz = info->idx;
         int l_max = int( info->val );
         assert(l_max >= sz );
         ps->setMem (l_max + 1, info);
         ps->set_max (l_max);
         ps->set_size(sz);
      }
   }
}

/** garbage collection in nonzero memory.
 *
 * Pack the svectors together as tightly as possible. This removes all additional unused memory,
 * i.e., size = max for every svector after the call.
 *
 * Note: do *not* call isConsistent() here, because the following might happen: In
 * SPxLP::doAddRows(const LPRowSet& p_set), when adding rows, the sizes of the vectors for the
 * columns of the LP are increased (without yet filling in the data) to recieve the additional
 * entries. This is done by calling xtend() above. xtend() in turn might call this method, which
 * checks the yet unfilled positions, i.e., isConsistent() is likely to fail. In general,
 * isConsistent() should not be called within this class, but in classes further up in the
 * hierarchy.
 */
void SVSet::memPack()
{
   int used;
   int j;
   DLPSV* ps;
   for (used = 0, ps = list.first(); ps; ps = list.next(ps))
   {
      const int sz = ps->size();

      if (ps->mem() != &this->SVSetBase::operator[](used))
      {
         // cannot use memcpy, because the memory might overlap
         for (j = 0; j <= sz; ++j)
            this->SVSetBase::operator[](used + j) = ps->mem()[j];
         ps->setMem(sz + 1, &this->SVSetBase::operator[](used));
         ps->set_size(sz);
      }
      else
         ps->set_max(sz);
      used += sz + 1;
   }
   SVSetBase::reSize(used);

   possiblyUnusedMem = 0;
}

#ifndef NO_CONSISTENCY_CHECKS
bool SVSet::isConsistent() const
{
   DLPSV* ps;
   DLPSV* next;
   for (ps = list.first(); ps; ps = next)
   {
      if (!ps->isConsistent())
         return MSGinconsistent("SVSet");
      if (ps->mem() > &last())
         return MSGinconsistent("SVSet");
      next = list.next(ps);
      if (next && ps->mem() + ps->max() + 1 != next->mem()) {
         return MSGinconsistent("SVSet");
      }
   }
   return DataArray < SVector::Element > ::isConsistent() 
      && set.isConsistent() && list.isConsistent();
}
#endif

SVSet& SVSet::operator=(const SVSet& rhs)
{
   if (this != &rhs)
   {
      clear();

      if (rhs.size() > 0)
      {
         DataArray < SVector::Element > ::operator=(rhs);
         set = rhs.set;

         DLPSV* ps;
         DLPSV* newps;

         void* delta0 = &(*(static_cast<SVSetBase*>(this)))[0];
         void* delta1 = &(*(static_cast<SVSetBase*>(
            const_cast<SVSet*>(&rhs))))[0];
         ptrdiff_t delta = reinterpret_cast<char*>(
            delta0) - reinterpret_cast<char*>(delta1);

         for (ps = rhs.list.first(); ps; ps = rhs.list.next(ps))
         {
            newps = & set[ rhs.number(ps) ];
            list.append(newps);
            newps->setMem(ps->max() + 1, 
               reinterpret_cast<SVector::Element*>(
                  reinterpret_cast<char*>(ps->mem()) + delta));
            newps->set_size( ps->size() );
         }
      }
   }
   assert(isConsistent());

   return *this;
}

SVSet::SVSet(const SVSet& old)
   : DataArray < SVector::Element > ()
   , possiblyUnusedMem (old.possiblyUnusedMem)
   , factor (old.factor)
{
   *this = old;
   assert(SVSet::isConsistent());
}
} // namespace soplex

//-----------------------------------------------------------------------------
//Emacs Local Variables:
//Emacs mode:c++
//Emacs c-basic-offset:3
//Emacs tab-width:8
//Emacs indent-tabs-mode:nil
//Emacs End:
//-----------------------------------------------------------------------------
