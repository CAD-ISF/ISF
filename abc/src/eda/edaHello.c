/**CFile****************************************************************

  FileName    [edaHello.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Package for demo how to plug in a package.]

  Synopsis    [Hello methods.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [June 3 , 2016.]

***********************************************************************/

#include "eda.h"
#include "base/main/mainInt.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

// external methods
// main methods
void Eda_SayHello( int );
// private helpers

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void 
Eda_SayHello( int fVerbose )
{
   printf( "Hello everyone!\n" );
   if ( fVerbose ) printf( "This package is written to show how to add a command in ABC\n" );
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END
