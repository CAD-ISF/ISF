/**CFile****************************************************************

  FileName    [eda.h]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Package for demo how to plug in a package.]

  Synopsis    [Header file for external declarations.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [June 3 , 2016.]

***********************************************************************/

#ifndef __EDA_H__
#define __EDA_H__


////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "base/abc/abc.h"

////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////

ABC_NAMESPACE_HEADER_START

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/*=== edaHello.c ==========================================================*/
extern void Eda_SayHello( int );
/*=== edaCreateFaultNode.c ================================================*/
extern int Eda_InsertFaultNode( int, char *, Abc_Obj_t * );
extern int Eda_ReplaceWithFaultNode( int, char *, Abc_Obj_t * );



ABC_NAMESPACE_HEADER_END
#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

