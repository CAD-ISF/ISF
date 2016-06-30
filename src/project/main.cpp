#include <iostream>
#include <IFS/ifs.h>
//#include <ifs.h>
#include <IFS/myHash.h>
//#include <myHash.h>
#include <stdlib.h>
#include <circuit/circuit.h>
//#include <circuit.h>

using namespace std;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////
void printfec(vector<vector<int> >&);

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
extern void   Abc_Start();
extern void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
extern void *	   Abc_FrameGetGlobalFrame();
extern void	*	   Abc_NtkDup( void * pNtk );
extern void		   Abc_NtkDelete( void * pNtk );
extern void *	   Abc_FrameReadNtk( void * p );
extern void		   Abc_FrameSetCurrentNetwork( void * p, void * pNtkNew );
extern void		   Abc_FrameSwapCurrentAndBackup( void * p );
extern void		   Abc_FrameReplaceCurrentNetwork( void * p, void * pNtk );
extern void		   Abc_FrameDeleteAllNetworks( void * p );
extern int		   Abc_NtkCecFraig( void * pNtk1, void * pNtk2, int nSeconds, int fVerbose );
extern int		   Cmd_CommandExecute( void * pAbc, char * sCommand );

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [The main() procedure.]

  Description [This procedure compiles into a stand-alone program for 
  DAG-aware rewriting of the AIGs. A BLIF or PLA file to be considered
  for rewriting should be given as a command-line argument. Implementation 
  of the rewriting is inspired by the paper: Per Bjesse, Arne Boralv, 
  "DAG-aware circuit compression for formal verification", Proc. ICCAD 2004.]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int main(int argc, const char** argv) {
	//////////////////////////////////////////////////////////////////////////
	// program start
	cout << "Enter ISF Project" << endl;
	Circuit c; 
	c.loadCircuit(argv[1]);
	//c.checkGateLists();
	IFS ifs;
	ifs.loadFault(argv[2]);
	//ifs.faultOut(c);
	//ifs.groupf(c);
	printfec(*(ifs.groupf(c)));

	//////////////////////////////////////////////////////////////////////////
	// ABC
    // parameters
    //int fUseResyn2  = 0;
    //int fPrintStats = 1;
    //int fVerify     = 1;
    // variables
    void * pAbc, * pNtk;
    const char * pFileName;
    char Command[1000];
    //clock_t clkRead, clkResyn, clkVer, clk;
	int cntconflict = 0;

    //////////////////////////////////////////////////////////////////////////
    // get the input file name
    if ( argc != 3 )
    {
        printf( "Wrong number of command-line arguments.\n" );
        return 1;
    }
    pFileName = argv[1];

    //////////////////////////////////////////////////////////////////////////
    // start the ABC framework
    Abc_Start();
	pAbc = Abc_FrameGetGlobalFrame();
	cout << "abc start\n";
	//////////////////////////////////////////////////////////////////
	// read the file
	sprintf( Command, "read %s", pFileName );
	if ( Cmd_CommandExecute( pAbc, Command ) )
	{
		fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
		return 1;
	}
			
	vector<vector<int> > newfecgroup;
	for ( size_t i = 0; i < ifs.getFecGroup()->size(); i++ ) {
		vector<int> &fecgroup = (*ifs.getFecGroup())[i];
		map< int, void * > fNtk;

		for ( size_t j = 0; j < fecgroup.size(); j++ ) {

			//////////////////////////////////////////////////////////////////
			// inject fault node
			sprintf( Command, "inject_fault n%d %d", 
					 ifs.getfGIdbyfId( fecgroup[j] ), ifs.getfTbyfId( fecgroup[j] ) );
			if ( Cmd_CommandExecute( pAbc, Command ) )
			{
				fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
				return 1;
			}

			//////////////////////////////////////////////////////////////////
			// balance
			sprintf( Command, "balance" );
			if ( Cmd_CommandExecute( pAbc, Command ) )
			{
				fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
				return 1;
			}

			//////////////////////////////////////////////////////////////////
			// write file
			sprintf( Command, "write %s.%d", pFileName, fecgroup[j] );
			if ( Cmd_CommandExecute( pAbc, Command ) )
			{
				fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
				return 1;
			}

			//////////////////////////////////////////////////////////////////
			// delete current ntk and use backup ntk
			pNtk = Abc_FrameReadNtk( pAbc );
			fNtk[ fecgroup[j] ] = Abc_NtkDup( pNtk );
			Abc_NtkDelete( pNtk );
			Abc_FrameSwapCurrentAndBackup( pAbc );
		}

		for ( size_t j = 0; j < fecgroup.size() - 1; j++ ) {
			newfecgroup.push_back(vector<int>());
			newfecgroup.back().push_back( fecgroup[j] );
			for ( size_t k = j + 1; k < fecgroup.size(); k++ ) {
				
				//////////////////////////////////////////////////////////////
				// cec ( SAT + Fraig )
				int sat = Abc_NtkCecFraig( fNtk[ fecgroup[j] ], fNtk[ fecgroup[k] ], 20, 0 );
				if ( sat == -1 ) cntconflict++;		// undefined
				else if ( sat == 0 ) cntconflict++;	// not equivalent
				else if ( sat == 1 ) {	// equivalent
					newfecgroup.back().push_back( fecgroup[k] );
				}
			}
		}

		for ( map< int, void * >::iterator it = fNtk.begin(); it != fNtk.end(); it++ ) {
			Abc_NtkDelete( it->second );
		}
	}
	Abc_FrameDeleteAllNetworks( pAbc );
	Abc_Stop();

	vector<vector<int> > &fecgroup = (*(ifs.getFecGroup()));
	fecgroup.clear();
	for ( int i = 0; i < newfecgroup.size(); i++) {
		if ( newfecgroup[i].size() > 1 ) {
			for ( int j = 0; j < newfecgroup[i].size() - 1 ; j++ ) {
				for ( int k = j + 1; k < newfecgroup[i].size(); k++ ) {
					fecgroup.push_back(vector<int>());
					fecgroup.back().push_back(newfecgroup[i][j]);
					fecgroup.back().push_back(newfecgroup[i][k]);
				}
			}
		}
	}
	
	printfec(fecgroup);
	
	return 0;
}

void printfec ( vector<vector<int> >& a ) {
	cout << "fec group : " << endl;
	int count=0;
	for ( int i = 0; i < a.size(); i++ ) {
		cout << "[" << i + 1 << "] = ";
		for ( int j = 0; j < (a[i]).size(); j++ ) {
			cout << "  " << (a[i])[j];
		}
		cout << endl;
		count++;
	}
	cout << "group count = " << count << endl;
}

