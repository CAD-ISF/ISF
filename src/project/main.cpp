#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <IFS/ifs.h>
//#include <ifs.h>
#include <IFS/myHash.h>
//#include <myHash.h>
#include <stdlib.h>
#include <stdio.h>
//#include <circuit/util.h>
#include <circuit/circuit.h>
//#include <circuit.h>

using namespace std;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////
void printfec(vector<vector<int> >&);
string fenum2string( FaultType );
void str2Parse(string, vector<string> &);

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
//extern void   Abc_Start();
//extern void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
//extern void *	   Abc_FrameGetGlobalFrame();
//extern void	*	   Abc_NtkDup( void * pNtk );
//extern void		   Abc_NtkDelete( void * pNtk );
//extern void *	   Abc_FrameReadNtk( void * p );
//extern void		   Abc_FrameSetCurrentNetwork( void * p, void * pNtkNew );
//extern void		   Abc_FrameSwapCurrentAndBackup( void * p );
//extern void		   Abc_FrameReplaceCurrentNetwork( void * p, void * pNtk );
//extern void		   Abc_FrameDeleteAllNetworks( void * p );
//extern int		   Abc_NtkCecFraig( void * pNtk1, void * pNtk2, int nSeconds, int fVerbose );
//extern int		   Cmd_CommandExecute( void * pAbc, char * sCommand );

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

	int fseq = 0, cntseq = 0;
	char Command[100000];
	const char * scriptFileName = "../tmp/script.txt", 
			   * fecgFileName = "../tmp/fecgroup.txt", 
			   * cmdFileName = "../tmp/cmd.txt";
	string line;
	ifstream cirF( argv[1] );

	while ( getline( cirF, line ) ) {
		if ( line[0] == '#' ) {
			cntseq ++;
		}
		if ( cntseq == 3 ) {
			if ( line[2] != '0' ) {
				fseq = 1;
			}
			break;
		}
	}
	cirF.close();

	vector<vector<int> > temp;
	sprintf( Command, "rm %s" , fecgFileName );
	system( Command );
	sprintf( Command, "touch %s" , fecgFileName );
	system( Command );

	for ( size_t i = 0; i < ( * ( ifs.getFecGroup() ) ).size(); i++ ) {
		//temp.clear();
		vector<int> &fecgroup = ( * ( ifs.getFecGroup() ) )[i];
		sprintf( Command, "rm %s" , scriptFileName );
		system( Command );
		sprintf( Command, "touch %s" , scriptFileName );
		system( Command );
		
		ofstream cmdF( cmdFileName );
		sprintf( Command, "../abc/abc -c \"ifs %s %s" , (fseq)? "-s": "-c", argv[1] );
		cmdF << Command;

		for ( size_t j = 0; j < fecgroup.size(); j++ ) {
			int Id = fecgroup[j],
				fId = ifs.getfGIdbyfId( Id );
			line = fenum2string( ifs.getfTbyfId( Id ) );
			sprintf( Command, " n%d %s", fId, line.c_str() );
			cmdF << Command;
		}
		//sprintf( Command, "\" | tee %s", scriptFileName );
		sprintf( Command, "\" | tee %s >> %s", scriptFileName, fecgFileName );
		cmdF << Command;

		cmdF.close();
		ifstream icmdF( cmdFileName );
		getline( icmdF, line );
		icmdF.close();
		//cout << line << '\n';
		//sprintf( line, "echo yes" );
		system( line.c_str() );


		size_t fcount = 1, fcnt, checked;
		bool need = true;
		ifstream scriptF( scriptFileName );
		vector<int> tempeq, tempneq;
		while ( getline( scriptF, line ) ) {
			//cout << line << '\n';
			fcnt = fcount;
			for ( size_t j = 0; j < fecgroup.size(); j++ ) {
				if ( fcnt + 1 > ( j + 1 ) * fecgroup.size() ) {
					fcnt += j + 2;
				}
			}

			vector<string> parse;
			str2Parse( line, parse );
			if ( parse[0] == "Networks" ) {
				//cout << parse[0] << ' ' << parse[1] << ' ' << parse[2] << '\n';
				if ( fcnt % fecgroup.size() == fcnt / fecgroup.size() + 1 ) {
					tempeq.clear();
					tempneq.clear();
					tempeq.push_back( fecgroup[ fcnt / fecgroup.size() ] );
				}
				if ( parse[1] == "are" && parse[2] == "NOT" ) {
					tempneq.push_back( fecgroup[ fcnt % fecgroup.size() ] );
					//cout << "neq";
					//for ( size_t t = 0; t < tempneq.size(); t++ ) {
						//cout << ' ' << tempneq[t];
					//}
					//cout << '\n';
				}
				else if ( parse[1] == "are" && ( parse[2] == "equivalent" || parse[2] == "equivalent." ) ) {
					tempeq.push_back( fecgroup[ fcnt % fecgroup.size() ] );
					//cout << "eq";
					//for ( size_t t = 0; t < tempeq.size(); t++ ) {
						//cout << ' ' << tempeq[t];
					//}
					//cout << '\n';
				}
				//cout << line << '\n';
				fcount ++;
				fcnt ++;
			}
			else ;

			if ( fcnt % fecgroup.size() == 0 ) {
				temp.push_back( tempeq );
				//cout << '[' << temp.size() << ']' << ' ';
				//for ( size_t j = 0; j < tempeq.size(); j++ ) {
					//cout << tempeq[j] << ' ';
				//}
				//cout << '\n';
				checked = fcnt / fecgroup.size();
				//cout << checked << '\n';
				if ( tempneq.size() == 0 ) break;
				for ( size_t j = checked; j < fecgroup.size(); j++ ) {
					if ( tempneq[0] != fecgroup[ j ] ) {
						//cout << j << '\n';
						for ( size_t k = 0; k < fecgroup.size() - j - 1; k++ ) {
							while ( getline( scriptF, line ) ) {
								vector<string> parse;
								str2Parse( line, parse );
								if ( parse[0] == "Networks" ) {
									//cout << line << '\n';
									fcount ++;
									break;
								}
							}
						}
					}
					else break;
				}
			}
		}
	}


	vector<vector<int> > &fecgroup = (*(ifs.getFecGroup()));
	fecgroup.clear();
	for ( int i = 0; i < temp.size(); i++) {
		for( int j = 0; j < temp[i].size(); j++ ) {
			cout << temp[i][j] << ' ';
		}
		cout << '\n';
		if ( temp[i].size() > 1 ) {
			for ( int j = 1; j < temp[i].size(); j++ ) {
				fecgroup.push_back(vector<int>());
				fecgroup.back().push_back(temp[i][0]);
				fecgroup.back().push_back(temp[i][j]);
			}
		}
	}
	printfec(fecgroup);
	

	return 0;

	//////////////////////////////////////////////////////////////////////////
	// ABC
    // parameters
    //int fUseResyn2  = 0;
    //int fPrintStats = 1;
    //int fVerify     = 1;
	// variables
    //void * pAbc, * pNtk;
    //const char * pFileName;
    //char Command[1000];
    //clock_t clkRead, clkResyn, clkVer, clk;
	//int cntconflict = 0;

    //////////////////////////////////////////////////////////////////////////
    // get the input file name
    //if ( argc != 3 )
    //{
        //printf( "Wrong number of command-line arguments.\n" );
        //return 1;
    //}
    //pFileName = argv[1];

    //////////////////////////////////////////////////////////////////////////
    // start the ABC framework
    //Abc_Start();
	//pAbc = Abc_FrameGetGlobalFrame();
	//cout << "abc start\n";
	//////////////////////////////////////////////////////////////////
	// read the file
	//sprintf( Command, "read %s", pFileName );
	//if ( Cmd_CommandExecute( pAbc, Command ) )
	//{
		//fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
		//return 1;
	//}
			
	//vector<vector<int> > newfecgroup;
	//for ( size_t i = 0; i < ifs.getFecGroup()->size(); i++ ) {
		//vector<int> &fecgroup = (*ifs.getFecGroup())[i];
		//map< int, void * > fNtk;

		//for ( size_t j = 0; j < fecgroup.size(); j++ ) {

			////////////////////////////////////////////////////////////////////
			// inject fault node
			//sprintf( Command, "inject_fault n%d %d", 
					 //ifs.getfGIdbyfId( fecgroup[j] ), ifs.getfTbyfId( fecgroup[j] ) );
			//if ( Cmd_CommandExecute( pAbc, Command ) )
			//{
				//fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
				//return 1;
			//}

			//////////////////////////////////////////////////////////////////
			// balance
			//sprintf( Command, "balance" );
			//if ( Cmd_CommandExecute( pAbc, Command ) )
			//{
				//fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
				//return 1;
			//}

			//////////////////////////////////////////////////////////////////
			// write file
			//sprintf( Command, "write %s.%d", pFileName, fecgroup[j] );
			//if ( Cmd_CommandExecute( pAbc, Command ) )
			//{
				//fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
				//return 1;
			//}

			//////////////////////////////////////////////////////////////////
			// delete current ntk and use backup ntk
			//pNtk = Abc_FrameReadNtk( pAbc );
			//fNtk[ fecgroup[j] ] = Abc_NtkDup( pNtk );
			//Abc_NtkDelete( pNtk );
			//Abc_FrameSwapCurrentAndBackup( pAbc );
		//}

		//for ( size_t j = 0; j < fecgroup.size() - 1; j++ ) {
			//newfecgroup.push_back(vector<int>());
			//newfecgroup.back().push_back( fecgroup[j] );
			//for ( size_t k = j + 1; k < fecgroup.size(); k++ ) {
				
				//////////////////////////////////////////////////////////////
				// cec ( SAT + Fraig )
				//int sat = Abc_NtkCecFraig( fNtk[ fecgroup[j] ], fNtk[ fecgroup[k] ], 20, 0 );
				//if ( sat == -1 ) cntconflict++;		// undefined
				//else if ( sat == 0 ) cntconflict++;	// not equivalent
				//else if ( sat == 1 ) {	// equivalent
					//newfecgroup.back().push_back( fecgroup[k] );
				//}
			//}
		//}

		//for ( map< int, void * >::iterator it = fNtk.begin(); it != fNtk.end(); it++ ) {
			//Abc_NtkDelete( it->second );
		//}
	//}
	//Abc_FrameDeleteAllNetworks( pAbc );
	//Abc_Stop();

}

void str2Parse(string str, vector<string> &parStr) {
	size_t fSpc = 0, fnSpc = 0;   // First space pos, First not space pos
	while( fnSpc != str.npos) {
		str = str.substr(fnSpc);
		fSpc = str.find_first_of(" ");
		fnSpc = str.find_first_not_of(" ");
		if (fSpc == str.npos) {
			fSpc = str.length();
		}
		parStr.push_back(str.substr(0, fSpc - fnSpc));
		str = str.substr(fSpc);
		fnSpc = str.find_first_not_of(" ");
	}
}


string fenum2string ( FaultType temp ) {
		if(temp==SA0)
			return "SA0";
		else if(temp==SA1)
			return "SA1";
		else if(temp==NEG)
			return "NEG";
		else if(temp==RDOB_AND)
			return "RDOB_AND";
		else if(temp==RDOB_NAND)
			return "RDOB_NAND";
		else if(temp==RDOB_OR)
			return "RDOB_OR";
		else if(temp==RDOB_NOR)
			return "RDOB_NOR";
		else if(temp==RDOB_XOR)
			return "RDOB_XOR";
		else if(temp==RDOB_NXOR)
			return "RDOB_NXOR";
		else if(temp==RDOB_NOT)
			return "RDOB_NOT";
		else if(temp==RDOB_BUFF)
			return "RDOB_BUFF";
		else 
			return "";
}

void printfec ( vector<vector<int> >& a ) {
	cout << "fec group : " << endl;
	int count=0;
	for ( int i = 0; i < a.size(); i++ ) {
		cout << "[" << i + 1 << "] = ";
		for ( int j = 0; j < (a[i]).size(); j++ ) {
			cout << "  " << (a[i])[j];
		}
		//cout << (a[i]).size();
		cout << endl;
		count++;
	}
	cout << "group count = " << count << endl;
}

