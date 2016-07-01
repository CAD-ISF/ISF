/**CFile****************************************************************

  FileName    [edaCreateFaultNode.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Package for create a fault node]

  Synopsis    [Create fault node.]

  Author      [Cheng Han Lin]

  Affiliation [NTU]

  Date        [June 19 , 2016.]

 ***********************************************************************/

#include "eda.h"
#include "base/main/mainInt.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

// external methods
// main methods
int Eda_InsertFaultNode( char *, Abc_Obj_t * );
int Eda_ReplaceWithFaultNode( char *, Abc_Obj_t * );
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

int
Eda_InsertFaultNode( char * faultType, Abc_Obj_t * pNode)
{
    Abc_Obj_t * pNodeConst, * pNodeInsert;

    // Transfer all the fanout of pNode to pNodeInsert if pNode is not
    // primary output
    // connect pNode to pNodeInsert;
    pNodeInsert = Abc_NtkCreateObj( pNode->pNtk, ABC_OBJ_NODE );
    Abc_ObjTransferFanout( pNode, pNodeInsert );
    Abc_ObjAddFanin( pNodeInsert, pNode );


    if ( strcmp(faultType, "SA0") == 0 )
    {
        // create a const0 node
        pNodeConst = Abc_NtkCreateObj( pNode->pNtk, ABC_OBJ_NODE );
        Abc_ObjSetData( pNodeConst, Abc_SopCreateConst0((Mem_Flex_t *)pNodeConst->pNtk->pManFunc) );
        // add the const0 to the fanin of pNodeInsert
        // assign pNodeInsert as And gate
        Abc_ObjAddFanin( pNodeInsert, pNodeConst );
        Abc_ObjSetData( pNodeInsert, Abc_SopCreateAnd((Mem_Flex_t *)pNodeInsert->pNtk->pManFunc, 2, NULL) );
    }
    else if ( strcmp(faultType, "SA1") == 0 )
    {
        // create a const0 node
        pNodeConst = Abc_NtkCreateObj( pNode->pNtk, ABC_OBJ_NODE );
        Abc_ObjSetData( pNodeConst, Abc_SopCreateConst1((Mem_Flex_t *)pNodeConst->pNtk->pManFunc) );
        // add the const1 to the fanin of pNodeInsert
        // assign pNodeInsert as OR gate
        Abc_ObjAddFanin( pNodeInsert, pNodeConst );
        Abc_ObjSetData( pNodeInsert, Abc_SopCreateOr((Mem_Flex_t *)pNodeInsert->pNtk->pManFunc, 2, NULL) );
    }
    else if ( strcmp(faultType, "NEG") == 0 )
    {
        // assign pNodeInsert as an inverter
        Abc_ObjSetData( pNodeInsert, Abc_SopCreateInv((Mem_Flex_t *)pNodeInsert->pNtk->pManFunc) );
    }
    // invalid fault type
    else return 0;

    return 1;
}

int
Eda_ReplaceWithFaultNode( char * faultType, Abc_Obj_t * pNodeOld )
{
    Abc_Obj_t * pNodeFault, * pNodeFanin;
    int i;
    // the fault node setup
    // append all the fanins of pNodeOld to pNodeFault
    // replace the pNodeOld with pNodeFault
    pNodeFault = Abc_NtkCreateObj( pNodeOld->pNtk, ABC_OBJ_NODE );
    Abc_ObjForEachFanin( pNodeOld, pNodeFanin, i)
        Abc_ObjAddFanin( pNodeFault, pNodeFanin );
    Abc_ObjReplace( pNodeOld, pNodeFault );

    if ( Abc_ObjFaninNum(pNodeFault) == 1 )
    {
        if ( strcmp(faultType, "RDOB_NOT") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateInv((Mem_Flex_t *)pNodeFault->pNtk->pManFunc) );
        }
        else if ( strcmp(faultType, "RDOB_BUFF") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateBuf((Mem_Flex_t *)pNodeFault->pNtk->pManFunc) );
        }
        else return 0;
    }
    else if ( Abc_ObjFaninNum(pNodeFault) == 2)
    {
        if ( strcmp(faultType, "RDOB_AND") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateAnd((Mem_Flex_t *)pNodeFault->pNtk->pManFunc, 2, NULL) );
        }
        else if ( strcmp(faultType, "RDOB_NAND") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateNand((Mem_Flex_t *)pNodeFault->pNtk->pManFunc, 2) );
        }
        else if ( strcmp(faultType, "RDOB_OR") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateOr((Mem_Flex_t *)pNodeFault->pNtk->pManFunc, 2, NULL) );
        }
        else if ( strcmp(faultType, "RDOB_NOR") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateNor((Mem_Flex_t *)pNodeFault->pNtk->pManFunc, 2) );
        }
        else if ( strcmp(faultType, "RDOB_XOR") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateXor((Mem_Flex_t *)pNodeFault->pNtk->pManFunc, 2) );
        }
        else if ( strcmp(faultType, "RDOB_NXOR") == 0 )
        {
            Abc_ObjSetData( pNodeFault, Abc_SopCreateNxor((Mem_Flex_t *)pNodeFault->pNtk->pManFunc, 2) );
        }
        else return 0;
    }
    else return 0;

    return 1;
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

