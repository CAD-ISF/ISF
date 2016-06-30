/**CFile****************************************************************

  FileName    [edaCec.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Package for CEC]

  Synopsis    [CEC methods.]

  Author      [Cheng-Han Lin]
  
  Affiliation [NTU]

  Date        [June 30 , 2016.]

***********************************************************************/

#include "eda.h"
#include "base/main/mainInt.h"
#include "proof/fraig/fraig.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

// external methods
// main methods
int Eda_NtkCecFraig( Abc_Ntk_t *, Abc_Ntk_t *, int, int );
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

int Eda_NtkCecFraig( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int fVerbose )
{
    Prove_Params_t Params, * pParams = &Params;
//    Fraig_Params_t Params;
//    Fraig_Man_t * pMan;
    Abc_Ntk_t * pMiter, * pTemp;
    Abc_Ntk_t * pExdc = NULL;
    int RetValue = -1;

    if ( pNtk1->pExdc != NULL || pNtk2->pExdc != NULL )
    {
	if ( pNtk1->pExdc != NULL && pNtk2->pExdc != NULL )
	{
	    printf( "Comparing EXDC of the two networks:\n" );
	    Eda_NtkCecFraig( pNtk1->pExdc, pNtk2->pExdc, nSeconds, fVerbose );
	    printf( "Comparing networks under EXDC of the first network.\n" );
	    pExdc = pNtk1->pExdc;
	}
	else if ( pNtk1->pExdc != NULL )
	{
	    printf( "Second network has no EXDC. Comparing main networks under EXDC of the first network.\n" );
	    pExdc = pNtk1->pExdc;
	}
	else if ( pNtk2->pExdc != NULL ) 
	{
	    printf( "First network has no EXDC. Comparing main networks under EXDC of the second network.\n" );
	    pExdc = pNtk2->pExdc;
	}
	else assert( 0 );
    }

    // get the miter of the two networks
    pMiter = Abc_NtkMiter( pNtk1, pNtk2, 1, 0, 0, 0 );
    if ( pMiter == NULL )
    {
	printf( "Miter computation has failed.\n" );
	return RetValue;
    }
    // add EXDC to the miter
    if ( pExdc )
    {
	assert( Abc_NtkPoNum(pMiter) == 1 );
	assert( Abc_NtkPoNum(pExdc) == 1 );
	pMiter = Abc_NtkMiter( pTemp = pMiter, pExdc, 1, 0, 1, 0 );
	Abc_NtkDelete( pTemp );
    }
    // handle trivial case
    RetValue = Abc_NtkMiterIsConstant( pMiter );
    if ( RetValue == 0 )
    {
	printf( "Networks are NOT EQUIVALENT after structural hashing.\n" );
	// report the error
	pMiter->pModel = Abc_NtkVerifyGetCleanModel( pMiter, 1 );
	Abc_NtkVerifyReportError( pNtk1, pNtk2, pMiter->pModel );
	ABC_FREE( pMiter->pModel );
	Abc_NtkDelete( pMiter );
	return RetValue;
    }
    if ( RetValue == 1 )
    {
	printf( "Networks are equivalent after structural hashing.\n" );
	Abc_NtkDelete( pMiter );
	return RetValue;
    }
    // solve the CNF using the SAT solver
    Prove_ParamsSetDefault( pParams );
    pParams->nItersMax = 5;
//    RetValue = Abc_NtkMiterProve( &pMiter, pParams );
//    pParams->fVerbose = 1;
    RetValue = Abc_NtkIvyProve( &pMiter, pParams );
    if ( RetValue == -1 )
	printf( "Networks are undecided (resource limits is reached).\n" );
    else if ( RetValue == 0 )
    {
	int * pSimInfo = Abc_NtkVerifySimulatePattern( pMiter, pMiter->pModel );
	if ( pSimInfo[0] != 1 )
	    printf( "ERROR in Abc_NtkMiterProve(): Generated counter-example is invalid.\n" );
	else
	    printf( "Networks are NOT EQUIVALENT.\n" );
	ABC_FREE( pSimInfo );
    }
    else
	printf( "Networks are equivalent.\n" );
    if ( pMiter->pModel )
	Abc_NtkVerifyReportError( pNtk1, pNtk2, pMiter->pModel );
    Abc_NtkDelete( pMiter );
	return RetValue;
}

