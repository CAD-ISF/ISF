/**CFile****************************************************************

  FileName    [eda.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Package for fault injection.]

  Synopsis    [Command file.]

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

static int EdaCommandHello              ( Abc_Frame_t * pAbc, int argc, char **argv );
static int EdaCommandInjectFault        ( Abc_Frame_t * pAbc, int argc, char **argv );

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Start / Stop the eda package]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void 
Eda_Init( Abc_Frame_t * pAbc )
{
   Cmd_CommandAdd( pAbc , "z EDA" , "hello" , EdaCommandHello , 0 );
   Cmd_CommandAdd( pAbc , "z EDA" , "inject_fault" , EdaCommandInjectFault , 0 );
}

void 
Eda_End()
{
}

/**Function*************************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

int 
EdaCommandHello( Abc_Frame_t * pAbc , int argc , char ** argv )
{
    int fVerbose , c;

    fVerbose = 0;
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "vh" ) ) != EOF )
    {
       switch ( c )
       {
          case 'v':
             fVerbose ^= 1;
             break;
          case 'h':
             goto usage;
          default:
             goto usage;
       }
    }

    Eda_SayHello( fVerbose );
    return 0;

usage:
    fprintf( pAbc->Err, "usage: hello [-vh]\n" );
    fprintf( pAbc->Err, "\t         Let ABC say hello to everyone\n" );
    fprintf( pAbc->Err, "\t-v     : toggle verbose hello [default = %s]\n", fVerbose ? "yes":"no" );
    fprintf( pAbc->Err, "\t-h     : prints the command summary\n" );
    return 1;
}

/**Function*************************************************************

  Synopsis    [Inject Fault.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

int 
EdaCommandInjectFault( Abc_Frame_t * pAbc , int argc , char ** argv )
{
    Abc_Ntk_t * pNtk, * pNtkRes;
    Abc_Obj_t * pNodeOld;
    char * pName;
    int id;
    char * faultType;
    /*int fVerbose; */
    int c;

    pNtk = Abc_FrameReadNtk(pAbc);
    /*fVerbose = 0;*/
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "h" ) ) != EOF )
    {
       switch ( c )
       {
          /*case 'v':*/
             /*fVerbose ^= 1;*/
             /*break;*/
          case 'h':
             goto usage;
          default:
             goto usage;
       }
    }

    if ( argc != globalUtilOptind + 2 )
        goto usage;

    // check if the network is available to inject fault
    if ( pNtk == NULL )
    {
        Abc_Print( -1, "Empty network.\n" );
        goto usage;
    }
    if ( !Abc_NtkIsLogic(pNtk) )
    {
        Abc_Print( -1, "Can only inject fault on a logic network.\n" );
        goto usage;
    }

    pName = argv[globalUtilOptind];
    faultType = argv[globalUtilOptind + 1];
    
    pNtkRes = Abc_NtkDup( pNtk );
    id = Nm_ManFindIdByName( pNtk->pManName, pName + 1, -1);
    pNodeOld = Abc_NtkObj( pNtk, id );
    /*pNodeOld = Abc_NtkFindNode(pNtkRes, pName + 1);*/
    
    
    /*id = atoi( pName + 1 );*/
    // pNodeFault = Abc_NtkCreateNode( pNtk, ABC_OBJ_NODE );
    // SA0, SA1, NEG
    if ( strncmp( faultType, "RDOB", 4 ) != 0 ) 
    {
        if ( !Eda_InsertFaultNode(id, faultType, pNodeOld) )
        {
            Abc_Print( -1, "Invalid fault type.\n" );
            goto usage;
        }
    }
    // RDOB
    // the node cannot be Pi if want to replace driver operator
    else if ( Abc_ObjFaninNum(pNodeOld) > 0 ) 
    {
        if ( !Eda_ReplaceWithFaultNode(id, faultType, pNodeOld) )
        {
            Abc_Print( -1, "Invalid fault type\n");
            goto usage;
        }
    }
    else
    {
        Abc_Print( -1, "The fanin of node should not be Pi\n");
        goto usage;
    }
    
    // set the setted network to be the current network
    // implicitly backup the origin network
    Abc_FrameSetCurrentNetwork( pAbc, pNtkRes );

    return 0;

usage:
    fprintf( pAbc->Err, "usage: inject_fault <nodeId> <faultType>\n" );
    fprintf( pAbc->Err, "\t         Inject a fault at a given node id of the network\n" );
    fprintf( pAbc->Err, "\t         nodeId should be a start with 'n', following with the id of the node\n" );
    fprintf( pAbc->Err, "\t         faultType should be one of the following fault type:\n" );
    fprintf( pAbc->Err, "\t         SA0, SA1, NEG, RDOB_AND, RDOB_NAND, RDOB_OR, RDOB_NOR, RDOB_AND\n" ); 
    fprintf( pAbc->Err, "\t         RDOB_XOR, RDOB_NXOR, RDOB_NOT, RDOB_BUFF\n" );
    fprintf( pAbc->Err, "\t-h     : prints the command usage\n" );
    return 1;
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END
