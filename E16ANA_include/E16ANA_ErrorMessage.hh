//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-08-20, uploaded by yokkaich
//2015-05-29, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//E16ANA_ErrorMessage.hh 150208 by S. Yokkaichi
//    Last modified at <2015-08-19 18:20:55 >
//
//from
/* phiErrorMessage.hh 970810 S.YOK.
 * modified by tsugu on 15 September 1997
 */

#ifndef E16_ERRORMESSAGE_HH
#define E16_ERRORMESSAGE_HH

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <stdarg.h>

//#include <phiConst.hh>

//---------------------------------------------

// Grade of messages---------------------------------
//
// MESSAGE
//    usual display
//    could be used as a counter
//
// WARNING
//    for un-intensional usage and/or behavior
//
// FATAL (cannot suppress)
//    for  the FATAL error, like it should be exit.
//
// INFO  (cannot suppress)
//    (fataly) important information, but should not exit
//    like 'XXX is Initialized'
//


//---------------------------------------------
class TsuguErrorMessageConst {
  public:
    enum { SHADOWED = 0, 
	   MESSAGE = 1, 
	   WARNING = 2, 
	   FATAL = 3, 
	   INFORMATION = 4,
			NumName = 5 };
    enum { Imprintable = -4, Printable = -8 };
    enum { NeverPrint = -1, Failed = -2 };
};

class TsuguErrorMessageBase : private TsuguErrorMessageConst {
  public:
  static char* levelName[NumName];
};

class TsuguCashedMessage : private TsuguErrorMessageConst {
  public:
    enum { DefaultLimit = 16, DefaultPeriod = 128 };
    
    TsuguCashedMessage(){
      period = DefaultPeriod;
      limit = DefaultLimit;
      len = count = 0;
      printable = Printable;
      //      message = CPNULL;
      message = (char*)(NULL);
    }

    TsuguCashedMessage( int level, const char* s0,
			const char* s1, const char* s2 ){
      period = DefaultPeriod;
      limit = DefaultLimit;
      count = 0;
      printable = Printable;
      Create( level, s0, s1, s2 );
    }

    ~TsuguCashedMessage();

    operator char*(){ return message; }

    void Printer( va_list& ap ) const {
      //      std::cerr << TsuguErrorMessageBase::levelName[ * message ];
      //      std::cerr.vform( message + 1, ap ) << std::endl;
      fprintf(stderr,"%s",TsuguErrorMessageBase::levelName[ int(* message) ]);
      vfprintf(stderr,message+1, ap);
      fprintf(stderr,"\n");
    }

    void FormPrinter() const {
      std::cerr << TsuguErrorMessageBase::levelName[ int(* message) ]
	   << ( message + 1 ) << std::endl;
    }

    void SetMessage( int level, const char* s0, const char* s1,
						const char* s2 ){
      delete[] message;
      Create( level, s0, s1, s2 );
    }

    void SetPeriod( int iPeriod, int iLimit ){
      if ( ( * message == char( FATAL ) )
	|| ( * message == char( INFORMATION ) ) ){
	period = 1;
	limit = -1;
      }
      else {
	if ( iPeriod == NeverPrint ) printable = Imprintable;
	else period = ( iPeriod == Failed ) ? period : iPeriod;
	limit = ( iLimit == Failed ) ? limit : iLimit;
      }
    }

    void MakePrintable(){ printable = Printable; }
    void MakeImprintable(){ printable = Imprintable; }

    int TimeToPrint(){
      return ( ( count < limit ) || ( ! ( count % period ) ) )
	  && ( printable == Printable );
    }

    int Counter(){ return count; }
    int Period(){ return period; }
    int Limit(){ return limit; }
    int Level(){ return int( * message ); }

  private:
    char* message;
    int limit, period, count, len, printable;

      static char* Delim; // Must be a string with the length 3.


    int Create( int level, const char* s0, const char* s1, const char* s2 );
    /* CAUTION: The function above uses new operator to store message.
     *          Never forget to delete the area.
     */

    void IncCounter(){ count ++; }

#ifndef min
    int min( int a, int b ){ return ( a < b ) ? a : b; }
#endif // min

  friend class PhiErrorMessage;
};


class PhiErrorMessage : public TsuguErrorMessageConst {
public:
  enum { 
    True=1, 
	 False=0, 
	 MaxLength = 256, 
	 MaxMessage = 4096, 
	 DefaultMessage = 256 };

  static PhiErrorMessage* GetInstance()  {
    	static PhiErrorMessage instance;  // only initialized at once
    	return &instance;
    }

private:
    PhiErrorMessage(){ Init( True, True, DefaultMessage ); }

    PhiErrorMessage( int warnFlag, int msgFlag,
			int iNumAllowd = DefaultMessage ){
      Init( warnFlag, msgFlag, iNumAllowd );
    }

public:

    ~PhiErrorMessage(){ delete[] cashedElem; }

    int GetID( int level, const char* str0, const char* str1,
					const char* str2 ) const;
    int NewID(){ return numCashed; }
    int TableSize(){ return numAllowed; }
    int TimeToPrint( int id );

    void StopWarning(){ printWarning = False; }
    void StopMessage(){ printMessage = False; }
    void StartWarning(){ printWarning = True; }
    void StartMessage(){ printMessage = True; }

    int Information( int id, ... ){
      va_list ap;
      va_start( ap, id );
      id = Print( id, ap );
      va_end( ap );
      return id;
    }

    int Fatal( int id, ... ){
      va_list ap;
      va_start( ap, id );
      id = Print( id, ap );
      va_end( ap );
      return id;
    }

    int Warning( int id, ... ){
      va_list ap;
      va_start( ap, id );
      id = Print( id, ap );
      va_end( ap );
      return id;
    }

    int Message( int id, ... ){
      va_list ap;
      va_start( ap, id );
      id = Print( id, ap );
      va_end( ap );
      return id;;
    }


    int Information( const char* srcName, const char* funcName,
					const char* message, ... ){
      va_list ap;
      va_start( ap, message );
      int id = PrePrinter( INFORMATION, srcName, funcName, message, ap );
      va_end( ap );
      return id;
    }

    int Fatal( const char* srcName, const char* funcName,
					const char* message, ... ){
      va_list ap;
      va_start( ap, message );
      int id = PrePrinter( FATAL, srcName, funcName, message, ap );
      va_end( ap );
      return id;
    }

    int Warning( const char* srcName, const char* funcName,
					const char* message, ... ){
      va_list ap;
      va_start( ap, message );
      int id = Failed;
      if ( printWarning ){
	id = PrePrinter( WARNING, srcName, funcName, message, ap );
      }
      va_end( ap );
      return id;
    }

    int Message( const char* srcName, const char* funcName,
					const char* message, ... ) {
      va_list ap;
      va_start( ap, message );
      int id = Failed;
      if ( printMessage ){
	id = PrePrinter( MESSAGE, srcName, funcName, message, ap );
      }
      va_end( ap );
      return id;
    }

    int Define( int level, const char* srcName, const char* funcName,
		const char* message,
		int iPeriod = Failed, int iLimit = Failed );

    void MakeGroupPrintable( int level );

    void ChangeAllPeriods( int iPeriod, int iLimit = Failed );
    void ChangeLevelPeriods( int level, int iPeriod, int iLimit = Failed );

    void ChangeWarningPeriods( int iPeriod, int iLimit = Failed ){
      ChangeLevelPeriods( WARNING, iPeriod, iLimit );
    }

    void ChangeMessagePeriods( int iPeriod, int iLimit = Failed ){
      ChangeLevelPeriods( MESSAGE, iPeriod, iLimit );
    }

    void NeverPrintAll(){
      SuppressWarning();
      SuppressMessage();
    }

    void PrintAll(){
      MakeGroupPrintable( WARNING );
      MakeGroupPrintable( MESSAGE );
    }

    void SuppressWarning(){
      ChangeLevelPeriods( WARNING, NeverPrint );
      defaultWarning = Imprintable;
    }

    void SuppressMessage(){
      ChangeLevelPeriods( MESSAGE, NeverPrint );
      defaultMessage = Imprintable;
    }

    void PrintWarning(){
      MakeGroupPrintable( WARNING );
      defaultWarning = Printable;
    }

    void PrintMessage(){
      MakeGroupPrintable( MESSAGE );
      defaultMessage = Printable;
    }

    void EmPrintable( int id ){
      if ( ( id < numCashed ) && ( id >= 0 ) ) cashedElem[id].MakePrintable();
    }

    void EmPrintable( int level, const char* srcName,
		const char* funcName, const char* message ){
	EmPrintable( GetID( level, srcName, funcName, message ) );
    }

    int ChangePeriod( int id, int iPeriod, int iLimit = Failed );
    int ChangePeriod( int level, const char* srcName, const char* funcName,
		const char* message, int iPeriod, int iLimit = Failed ){
      return ChangePeriod( GetID( level, srcName, funcName, message ),
		iPeriod, iLimit );
    }

  private:
    int printWarning, printMessage, defaultWarning, defaultMessage,
	numCashed, numAllowed;
    TsuguCashedMessage* cashedElem;

    void Init( int warnFlag, int msgFlag, int iNumAllowd );

    void SetDefaultPrintability( int id ){
      int level = cashedElem[id].Level();
      if ( ( ( level == MESSAGE ) && ( defaultMessage == Imprintable ) )
	|| ( ( level == WARNING ) && ( defaultWarning == Imprintable ) ) ){
	cashedElem[id].MakeImprintable();
      }
      else cashedElem[id].MakePrintable();
    }

    int Print( int id, va_list& ap ){
      if ( ( 0 <= id ) && ( id < numAllowed ) && ( id < numCashed ) ){
	int level = cashedElem[id].Level();
	if ( ( level == FATAL ) || ( level == INFORMATION )
	  || ( ( level == WARNING ) && ( printWarning ) )
	  || ( ( level == MESSAGE ) && ( printMessage ) ) ){
	  return PrePrinter( id, ap );
	}
      }
      return Failed;
    }

    int PrePrinter( int id, va_list& ap );
    int PrePrinter( int level, const char* srcName, const char* funcName,
		const char* message, va_list& ap );

  friend class TsuguCashedMessage;
};


#if 0
#define E16Message_MESSAGE(...)	\
PhiErrorMessage::GetInstance()->Message(__FILE__, __func__, __VA_ARGS__)

#define E16Message_WARNING(...)	\
PhiErrorMessage::GetInstance()->Warning(__FILE__, __func__, __VA_ARGS__)

#define E16Message_FATAL(...)	\
PhiErrorMessage::GetInstance()->Fatal(__FILE__, __func__, __VA_ARGS__)

#define E16Message_INFO(...)	\
PhiErrorMessage::GetInstance()->Information(__FILE__, __func__, __VA_ARGS__)
#endif


#define E16MESSAGE(...)	\
PhiErrorMessage::GetInstance()->Message(__FILE__, __func__, __VA_ARGS__)

#define E16WARNING(...)	\
PhiErrorMessage::GetInstance()->Warning(__FILE__, __func__, __VA_ARGS__)

#define E16FATAL(...)	\
PhiErrorMessage::GetInstance()->Fatal(__FILE__, __func__, __VA_ARGS__)

#define E16INFO(...)	\
PhiErrorMessage::GetInstance()->Information(__FILE__, __func__, __VA_ARGS__)



// Grade of messages---------------------------------
//
// MESSAGE
//    usual display
//    could be used as a counter
//
// WARNING
//    for un-intensional usage and/or behavior
//
// FATAL (cannot suppress)
//    for  the FATAL error, like it should be exit.
//
// INFO  (cannot suppress)
//    (fataly) important information, but should not exit
//    like 'XXX is Initialized'
//


#endif // E16_ERRORMESSAGE_HH

// End of file

