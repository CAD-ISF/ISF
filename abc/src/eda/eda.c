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
/*#include <iostream>*/
/*#include <fstream>*/


ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

static int EdaCommandHello              ( Abc_Frame_t * pAbc, int argc, char ** argv );
static int EdaCommandInjectFault        ( Abc_Frame_t * pAbc, int argc, char ** argv );
static int EdaCommandCec		        ( Abc_Frame_t * pAbc, int argc, char ** argv );

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
   Cmd_CommandAdd( pAbc , "z EDA" , "eda_cec" , EdaCommandCec , 0 );
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
    id = Nm_ManFindIdByName( pNtkRes->pManName, pName + 1, -1);
    pNodeOld = Abc_NtkObj( pNtkRes, id );
    
    // pNodeFault = Abc_NtkCreateNode( pNtk, ABC_OBJ_NODE );
    // SA0, SA1, NEG
    if ( strncmp( faultType, "RDOB", 4 ) != 0 ) 
    {
        if ( !Eda_InsertFaultNode(faultType, pNodeOld) )
        {
            Abc_Print( -1, "Invalid fault type.\n" );
            goto usage;
        }
    }
    // RDOB
    // the node cannot be Pi if want to replace driver operator
    else if ( Abc_ObjFaninNum(pNodeOld) > 0 ) 
    {
        if ( !Eda_ReplaceWithFaultNode(faultType, pNodeOld) )
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

/*int */
/*EdaCommandIfs( Abc_Frame_t *pAbc, int argc, char ** argv )*/
/*{*/
	/*// argv[0] : .bench*/
	/*// argv[2n+1] : fault_gate_id*/
	/*// argv[2n+2] : fault_type*/
	/*int optpos = 1, argpos; // optind : option index, argpos : argument position ( = optind + 1 )*/
	/*char Command[1000];*/

	/*Abc_FrameDeleteAllNetworks( pAbc );*/

	/*// clear all networks in frame*/
	/*sprintf( Command, "read %s", argv[optind] );*/
	/*if ( Cmd_CommandExecute( pAbc, Command ) )*/
	/*{*/
		/*fprintf( stdout, "Cannot execute command \"%s\".\n", Command );*/
		/*return 1;*/
	/*}*/
	/*optpos += 1;*/
	/*argpos = optpos + 1;*/

	/*// create array for pointing fault networks*/
	/*Abc_Ntk_t ** vNtk = malloc( sizeof( Abc_Ntk_t * ) );*/

	/*// if still fault unread*/
	/*while ( argpos < argc )*/
	/*{*/
		/*// inject fault*/
		/*sprintf( Command, "inject_fault %s %s", argv[optind], argv[argpos] );*/
		/*if ( Cmd_CommandExecute( pAbc, Command ) )*/
		/*{*/
			/*fprintf( stdout, "Cannot execute command \"%s\".\n", Command );*/
			/*return 1;*/
		/*}*/
		/*// vNtk[i] = Abc_FrameReadNtk( pAbc )*/
	/*}*/

	/*// cec vNtk[0] vNtk[i]*/

	/*return 0;*/
/*}*/
		

int 
EdaCommandCec( Abc_Frame_t * pAbc, int argc, char ** argv )
{
    char Buffer[16];
    Abc_Ntk_t * pNtk, * pNtk1, * pNtk2;
    int fDelete1, fDelete2;
    char ** pArgvNew;
    int nArgcNew;
    int c;
    int fSat;
    int fVerbose;
    int nSeconds;
    int nPartSize;
    int nConfLimit;
    int nInsLimit;
    int fPartition;
    int fIgnoreNames;

    extern void Abc_NtkCecSat( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nConfLimit, int nInsLimit );
    extern int Eda_NtkCecFraig( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int fVerbose );
    extern void Abc_NtkCecFraigPart( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int nPartSize, int fVerbose );
    extern void Abc_NtkCecFraigPartAuto( Abc_Ntk_t * pNtk1, Abc_Ntk_t * pNtk2, int nSeconds, int fVerbose );

    pNtk = Abc_FrameReadNtk(pAbc);
    // set defaults
    fSat     =  0;
    fVerbose =  0;
    nSeconds = 20;
    nPartSize  = 0;
    nConfLimit = 10000;
    nInsLimit  = 0;
    fPartition = 0;
    fIgnoreNames = 0;
    Extra_UtilGetoptReset();
    while ( ( c = Extra_UtilGetopt( argc, argv, "TCIPpsnvh" ) ) != EOF )
    {
        switch ( c )
        {
        case 'T':
            if ( globalUtilOptind >= argc )
            {
                Abc_Print( -1, "Command line switch \"-T\" should be followed by an integer.\n" );
                goto usage;
            }
            nSeconds = atoi(argv[globalUtilOptind]);
            globalUtilOptind++;
            if ( nSeconds < 0 )
                goto usage;
            break;
        case 'C':
            if ( globalUtilOptind >= argc )
            {
                Abc_Print( -1, "Command line switch \"-C\" should be followed by an integer.\n" );
                goto usage;
            }
            nConfLimit = atoi(argv[globalUtilOptind]);
            globalUtilOptind++;
            if ( nConfLimit < 0 )
                goto usage;
            break;
        case 'I':
            if ( globalUtilOptind >= argc )
            {
                Abc_Print( -1, "Command line switch \"-I\" should be followed by an integer.\n" );
                goto usage;
            }
            nInsLimit = atoi(argv[globalUtilOptind]);
            globalUtilOptind++;
            if ( nInsLimit < 0 )
                goto usage;
            break;
        case 'P':
            if ( globalUtilOptind >= argc )
            {
                Abc_Print( -1, "Command line switch \"-P\" should be followed by an integer.\n" );
                goto usage;
            }
            nPartSize = atoi(argv[globalUtilOptind]);
            globalUtilOptind++;
            if ( nPartSize < 0 )
                goto usage;
            break;
        case 'p':
            fPartition ^= 1;
            break;
        case 's':
            fSat ^= 1;
            break;
        case 'n':
            fIgnoreNames ^= 1;
            break;
        case 'v':
            fVerbose ^= 1;
            break;
        default:
            goto usage;
        }
    }

    if ( pNtk && pNtk->vPhases != NULL )
    {
        Abc_Print( -1, "Cannot compare networks with phases defined.\n" );
        return 1;
    }

    pArgvNew = argv + globalUtilOptind;
    nArgcNew = argc - globalUtilOptind;
    if ( !Abc_NtkPrepareTwoNtks( stdout, pNtk, pArgvNew, nArgcNew, &pNtk1, &pNtk2, &fDelete1, &fDelete2 ) )
        return 1;

    if ( fIgnoreNames )
    {
        if ( !fDelete1 )
        {
            pNtk1 = Abc_NtkStrash( pNtk1, 0, 1, 0 );
            fDelete1 = 1;
        }
        if ( !fDelete2 )
        {
            pNtk2 = Abc_NtkStrash( pNtk2, 0, 1, 0 );
            fDelete2 = 1;
        }
        Abc_NtkShortNames( pNtk1 );
        Abc_NtkShortNames( pNtk2 );
    }

    // perform equivalence checking
    if ( fPartition )
        Abc_NtkCecFraigPartAuto( pNtk1, pNtk2, nSeconds, fVerbose );
    else if ( nPartSize )
        Abc_NtkCecFraigPart( pNtk1, pNtk2, nSeconds, nPartSize, fVerbose );
    else if ( fSat )
        Abc_NtkCecSat( pNtk1, pNtk2, nConfLimit, nInsLimit );
    else
        Eda_NtkCecFraig( pNtk1, pNtk2, nSeconds, fVerbose );

    if ( fDelete1 ) Abc_NtkDelete( pNtk1 );
    if ( fDelete2 ) Abc_NtkDelete( pNtk2 );
    return 0;

usage:
    if ( nPartSize == 0 )
        strcpy( Buffer, "unused" );
    else
        sprintf(Buffer, "%d", nPartSize );
    Abc_Print( -2, "usage: cec [-T num] [-C num] [-I num] [-P num] [-psnvh] <file1> <file2>\n" );
    Abc_Print( -2, "\t         performs combinational equivalence checking\n" );
    Abc_Print( -2, "\t-T num : approximate runtime limit in seconds [default = %d]\n", nSeconds );
    Abc_Print( -2, "\t-C num : limit on the number of conflicts [default = %d]\n",    nConfLimit );
    Abc_Print( -2, "\t-I num : limit on the number of clause inspections [default = %d]\n", nInsLimit );
    Abc_Print( -2, "\t-P num : partition size for multi-output networks [default = %s]\n", Buffer );
    Abc_Print( -2, "\t-p     : toggle automatic partitioning [default = %s]\n", fPartition? "yes": "no" );
    Abc_Print( -2, "\t-s     : toggle \"SAT only\" and \"FRAIG + SAT\" [default = %s]\n", fSat? "SAT only": "FRAIG + SAT" );
    Abc_Print( -2, "\t-n     : toggle how CIs/COs are matched (by name or by order) [default = %s]\n", fIgnoreNames? "by order": "by name" );
    Abc_Print( -2, "\t-v     : toggle verbose output [default = %s]\n", fVerbose? "yes": "no" );
    Abc_Print( -2, "\t-h     : print the command usage\n");
    Abc_Print( -2, "\tfile1  : (optional) the file with the first network\n");
    Abc_Print( -2, "\tfile2  : (optional) the file with the second network\n");
    Abc_Print( -2, "\t         if no files are given, uses the current network and its spec\n");
    Abc_Print( -2, "\t         if one file is given, uses the current network and the file\n");
    return 1;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END
