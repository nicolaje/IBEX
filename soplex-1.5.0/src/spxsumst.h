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
#pragma ident "@(#) $Id: spxsumst.h,v 1.13 2010/09/16 17:45:04 bzfgleix Exp $"


/**@file  spxsumst.h
 * @brief Simple heuristic SPxStarter.
 */
#ifndef _SPXSUMST_H_
#define _SPXSUMST_H_


#include <assert.h>

#include "spxvectorst.h"

namespace soplex
{

/**@brief   Simple heuristic SPxStarter.
   @ingroup Algo

   Testing version of an SPxVectorST using a very simplistic heuristic to
   build up an approximated solution vector.
*/
class SPxSumST : public SPxVectorST
{
protected:

   //-------------------------------------
   /**@name Protected helpers */
   //@{
   /// sets up variable weights.
   void setupWeights(SPxSolver& base);
   //@}

public:

   //-------------------------------------
   /**@name Construction / destruction */
   //@{
   /// default constructor.
   SPxSumST()
   {
      m_name = "Sum";
   }
   /// copy constructor
   SPxSumST( const SPxSumST& old)
      : SPxVectorST(old)
   {
      assert(isConsistent());
   }
   /// assignment operator
   SPxSumST& operator=( const SPxSumST& rhs)
   {
      if(this != &rhs)
      {
         SPxVectorST::operator=(rhs);

         assert(isConsistent());
      }

      return *this;
   }
   /// destructor.
   virtual ~SPxSumST()
   {}  
   /// clone function for polymorphism
   inline virtual SPxStarter* clone() const
   {
      return new SPxSumST(*this);
   }
   //@}

};

} // namespace soplex
#endif // _SPXSUMST_H_

//-----------------------------------------------------------------------------
//Emacs Local Variables:
//Emacs mode:c++
//Emacs c-basic-offset:3
//Emacs tab-width:8
//Emacs indent-tabs-mode:nil
//Emacs End:
//-----------------------------------------------------------------------------


