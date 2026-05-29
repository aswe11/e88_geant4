//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-28, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//E16ANA_ErrorMessage.cc 150208 by S. Yokkaichi
//    Last modified at <2015-03-28 13:41:58 >
//
//from
/* Error messages by Tsuguchika Tabaru 
 */

#include <iostream>
#include <stdarg.h>
#include <string.h>

#include "E16ANA_ErrorMessage.hh"

const int ErrorCodeNum = 17;

const ushort EC_GOOD				=  0;
const ushort EC_NO_ERROR			=  0;
const ushort EC_BAD_ARGUMENT			= 64;
const ushort EC_BAD_PARAMETER			= 65;
const ushort EC_SYNTAX_ERROR			= 66;
const ushort EC_CANNOT_OPEN_FILE		= 67;
const ushort EC_CANNOT_CREATE_FILE		= 68;
const ushort EC_CANNOT_ALLOCATE_MEMORY		= 69;
const ushort EC_DIFFERENT_CLASS			= 70;
const ushort EC_PARSE_ERROR			= 71;
const ushort EC_OUT_OF_RANGE			= 72;
const ushort EC_INTERNAL_ERROR			= 73;
const ushort EC_STREAM_ERROR			= 74;
const ushort EC_OVERFLOW			= 75;
const ushort EC_TOO_LONG_WORD			= 76;
const ushort EC_TOO_LONG_LINE			= 77;
const ushort EC_UNTERMINATED_STRING		= 78;
const ushort EC_CPP_ERROR			= 79;

char* StrErrorMessage( ushort ErrorCode );

inline void PError( ushort ErrorCode ){
  std::cerr << StrErrorMessage( ErrorCode ) << std::endl;
}

using namespace std;

const char* sourceFileName = __FILE__;

const char* EM_UNDEFINED		 = "Undefined error.";
const char* EM_NO_ERROR			 = "No error.";
const char* EM_BAD_ARGUMENT		 = "Bar argument.";
const char* EM_BAD_PARAMETER		 = "Bad parameter.";
const char* EM_SYNTAX_ERROR		 = "Syntax error.";
const char* EM_CANNOT_OPEN_FILE		 = "Cannot open file.";
const char* EM_CANNOT_CREATE_FILE	 = "Cannot creat file.";
const char* EM_CANNOT_ALLOCATE_MEMORY	 = "Cannot allocate memory.";
const char* EM_DIFFERENT_CLASS		 = "Different class.";
const char* EM_PARSE_ERROR		 = "Parse error.";
const char* EM_OUT_OF_RANGE		 = "Out of range.";
const char* EM_INTERNAL_ERROR		 = "Internal error.";
const char* EM_STREAM_ERROR		 = "Stream error.";
const char* EM_OVERFLOW			 = "Overflow.";
const char* EM_TOO_LONG_WORD		 = "Too long word.";
const char* EM_TOO_LONG_LINE		 = "Too long line.";
const char* EM_UNTERMINATED_STRING	 = "Unterminated string.";
const char* EM_CPP_ERROR		 = "C pre processor error.";

struct ErrorMessageTable {
  public:
    ushort code;
    const char* message;
};

static ErrorMessageTable EMT[ErrorCodeNum] = {
  { EC_NO_ERROR,		EM_NO_ERROR			},
  { EC_BAD_ARGUMENT,		EM_BAD_ARGUMENT			},
  { EC_BAD_PARAMETER,		EM_BAD_PARAMETER		},
  { EC_SYNTAX_ERROR,		EM_SYNTAX_ERROR			},
  { EC_CANNOT_OPEN_FILE,	EM_CANNOT_OPEN_FILE		},
  { EC_CANNOT_CREATE_FILE,	EM_CANNOT_CREATE_FILE		},
  { EC_CANNOT_ALLOCATE_MEMORY,	EM_CANNOT_ALLOCATE_MEMORY	},
  { EC_DIFFERENT_CLASS,		EM_DIFFERENT_CLASS		},
  { EC_PARSE_ERROR,		EM_PARSE_ERROR			},
  { EC_OUT_OF_RANGE,		EM_OUT_OF_RANGE			},
  { EC_INTERNAL_ERROR,		EM_INTERNAL_ERROR		},
  { EC_STREAM_ERROR,		EM_STREAM_ERROR			},
  { EC_OVERFLOW,		EM_OVERFLOW			},
  { EC_TOO_LONG_WORD,		EM_TOO_LONG_WORD		},
  { EC_TOO_LONG_LINE,		EM_TOO_LONG_LINE		},
  { EC_UNTERMINATED_STRING,	EM_UNTERMINATED_STRING		},
  { EC_CPP_ERROR,		EM_CPP_ERROR			}
};

char* StrErrorMessage( ushort ErrorCode ){
  int i = ErrorCodeNum;
  while ( i -- ){
    if ( ErrorCode == EMT[i].code ) return ( char* )( EMT[i].message );
  }
  return ( char* )( EM_UNDEFINED );
}

char* TsuguCashedMessage::Delim = (char*)(": "); // must be a string with the length 3.


TsuguCashedMessage::~TsuguCashedMessage(){
  if ( count > 0 ){
    cerr << "(" << count << " times repeated)";
    FormPrinter();
  }

  delete[] message;
  len = 0;
}

int TsuguCashedMessage::Create( int level,
			const char* s0, const char* s1, const char* s2 ){
  int len0, len1, len2;
  len0 = min( strlen( s0 ), PhiErrorMessage::MaxLength - 6 );
  len1 = min( strlen( s1 ), PhiErrorMessage::MaxLength - 6 - len0 );
  len2 = min( strlen( s2 ), PhiErrorMessage::MaxLength - 6 - len0 - len1 );
  len = len0 + len1 + len2 + 6;
  message = new char[len];
  //  if ( message == CPNULL ){
  if ( message == (char*)(NULL) ){
    cerr << "FATAL: " << sourceFileName << Delim;
    cerr << "int TsuguCashedMessage::Create(char*&, int, char*, char*, char*)";
    cerr << ": Cannot allocate memory for message ";
    cerr << s0 << Delim << s1 << Delim << s2 << endl;
    return 0;
  }

  char* pointer = message;
  //  * ( pointer ++ ) = char( level & 0xff );
  * pointer = char( level & 0xff );
  pointer ++;
  strncpy( pointer, s0, len0 );
  pointer += len0;
  strncpy( pointer, Delim, 2 );
  pointer += 2;
  strncpy( pointer, s1, len1 );
  pointer += len1;
  strncpy( pointer, Delim, 2 );
  pointer += 2;
  strncpy( pointer, s2, len2 );
  //  pointer[len2] = CNULL;
  pointer[len2] = (char(NULL));

  if ( ( level == FATAL ) || ( level == INFORMATION ) ){
    limit = -1;
    period = 1;
  }

  return len;
}

char* TsuguErrorMessageBase::levelName[TsuguErrorMessageBase::NumName] = {
  (char*)"SHADOWED: ", 
  (char*)"MESSAGE: ", 
  (char*)"WARNING: ", 
  (char*)"FATAL: ", 
  (char*)"INFORMATION: "
};

void PhiErrorMessage::Init( int warnFlag, int msgFlag, int iNumAllowd ){
  cerr<<" ErrorMessage facility("<<__FILE__<<") initialization...";

  if ( iNumAllowd > MaxMessage ){
    cerr<<endl;
    cerr << "WARNING: " << sourceFileName << TsuguCashedMessage::Delim;
    cerr << "void PhiErrorMessage::Init( int warnFlag, int msgFlag )";
    cerr << ": Too large number of cash buffer are given.  ";
    cerr << "Ensmall it from " << iNumAllowd << " to " << MaxMessage << endl;
    iNumAllowd = MaxMessage;
  }
  printWarning = defaultWarning = warnFlag;
  printMessage = defaultMessage = msgFlag;
  numCashed = 0;
  cashedElem = new TsuguCashedMessage[iNumAllowd];
  numAllowed = 0;
  if ( cashedElem == ( TsuguCashedMessage* )( NULL ) ){
    cerr << "FATAL: " << sourceFileName << TsuguCashedMessage::Delim;
    cerr << "void PhiErrorMessage::Init( int warnFlag, int msgFlag )";
    cerr << ": Cannot allocate memory for cash buffer.  ";
    cerr << "Requied number is " << iNumAllowd << endl;
  }
  numAllowed = iNumAllowd;
  cerr<<"...end"<<endl;
}

int PhiErrorMessage::GetID( int level, const char* str0, const char* str1,
						const char* str2 ) const {
  int id = numCashed;
  TsuguCashedMessage target( level, str0, str1, str2 );
  if ( target.len ){
    id = numCashed;
    while ( id -- ){
      if ( ! strcmp( target, cashedElem[id] ) ) break;
    }
    id = ( id == -1 ) ? numCashed : id;
  }
  return id;
}

int PhiErrorMessage::PrePrinter( int level, const char* srcName,
		const char* funcName, const char* message, va_list& ap ){
  int id = GetID( level, srcName, funcName, message );
  if ( id < numAllowed ){
    if ( id == numCashed ){
      cashedElem[id].SetMessage( level, srcName, funcName, message );
      SetDefaultPrintability( id );
      numCashed ++;
    }
    cashedElem[id].IncCounter();
    if ( cashedElem[id].TimeToPrint() ) cashedElem[id].Printer( ap );
    return id;
  }
  TsuguCashedMessage temp( level, srcName, funcName, message );
  temp.Printer( ap );
  return Failed;
}

int PhiErrorMessage::PrePrinter( int id, va_list& ap ){
  if ( id < numAllowed ){
    if ( id >= 0 ){
      cashedElem[id].IncCounter();
      if ( cashedElem[id].TimeToPrint() ) cashedElem[id].Printer( ap );
      return id;
    }
  }
  return Failed;
}

int PhiErrorMessage::Define( int level, const char* srcName,
      const char* funcName, const char* message, int iPeriod, int iLimit ){
  int id = GetID( level, srcName, funcName, message );
#ifdef DEBUG
  cerr << "Define: ID is " << id << endl;
  cerr << "        Str is " << srcName << ": " << funcName << endl;
#endif //DEBUG
  if ( id < numAllowed ){
    if ( id == numCashed ){
      cashedElem[id].SetMessage( level, srcName, funcName, message );
      cashedElem[id].SetPeriod( iPeriod, iLimit );
      SetDefaultPrintability( id );
      numCashed ++;
      return id;
    }
    else {
      cerr << "WARNING: " << sourceFileName << TsuguCashedMessage::Delim;
      cerr << "void PhiErrorMessage::Define( int, const char*, ";
      cerr << "const char*, const char*, int, int ): ";
      cerr << "Multiple definition for ";
      cerr << srcName << TsuguCashedMessage::Delim << funcName;
      cerr << TsuguCashedMessage::Delim << message << endl;
      return id;
    }
  }
  return Failed;
}

int PhiErrorMessage::ChangePeriod( int id, int iPeriod, int iLimit ){
  if ( ( id < numCashed ) && ( id >= 0 ) ){
    cashedElem[id].SetPeriod( iPeriod, iLimit );
    return id;
  }
  return Failed;
}

void PhiErrorMessage::ChangeAllPeriods( int iPeriod, int iLimit ){
  int id = numCashed;
  while ( id -- ) cashedElem[id].SetPeriod( iPeriod, iLimit );
}

void PhiErrorMessage::ChangeLevelPeriods( int level, int iPeriod, int iLimit ){
  if ( ( level == FATAL ) || ( level == INFORMATION ) ) return;
  int id = numCashed;
  char cLevel = char( level );
  while ( id -- ){
    if ( * cashedElem[id] == cLevel ){
      cashedElem[id].SetPeriod( iPeriod, iLimit );
    }
  }
}

void PhiErrorMessage::MakeGroupPrintable( int level ){
  if ( ( level == FATAL ) || ( level == INFORMATION ) ) return;
  int id = numCashed;
  char cLevel = char( level );
  while ( id -- ){
    if ( * cashedElem[id] == cLevel ) cashedElem[id].MakePrintable();
  }
}

int PhiErrorMessage::TimeToPrint( int id ){
  if ( id < numCashed ){
    if ( id >= 0 ){
      int result = cashedElem[id].TimeToPrint();
      switch ( * cashedElem[id] ){
	case WARNING: return result && printWarning;
	case MESSAGE: return result && printMessage;
	case INFORMATION:
	case FATAL:   return True;
	default:      break;
      }
    }
  }
  return True;
}

//PhiErrorMessage phiErrMsg;

// End of file

