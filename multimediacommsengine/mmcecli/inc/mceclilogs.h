/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    
*
*/






#ifndef MCECLILOGS_H
#define MCECLILOGS_H

#include "mcecomlogs.h"
#include "mceevents.h"
#include "mceclientserver.h"
#include "mcesession.h"

// INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION

#ifdef MCECLI_UNIT_TEST_COVERAGE

#undef _DEBUG

#endif

/**
 * Class provides logging through RDebug in debug builds
 * 'MCE: ' added automatically in fron of every logged line
 */ 
class TMceCliDebug 
	{
	public:

#ifdef _DEBUG

	inline static void Print(const TDesC16& aStr) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Client] %S");

			TBuf<256> str;
			str.Format(KMceFormat, &aStr);
			TMceDebug::Print( str, ETrue );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr1, TUint32 aValue1 )
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Client] %S=%d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, aValue1);
			TMceDebug::Print( str, ETrue );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print( const TDesC16& aStr1, const TDesC16& aStr2 )
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound  &&
		     aStr2.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Client] %S=%S");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, &aStr2);
			TMceDebug::Print( str, ETrue );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr1, TUint32 aValue1, const TDesC16& aStr2, TUint32 aValue2 ) 
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Client] %S=%d, %S=%d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, aValue1, &aStr2, aValue2 );
			TMceDebug::Print( str );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr1, TUint32 aValue1, 
	                         const TDesC16& aStr2, TUint32 aValue2,
	                         const TDesC16& aStr3, TUint32 aValue3 )
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Client] %S=%d, %S=%d, %S=%d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, aValue1, &aStr2, aValue2, &aStr3, aValue3 );
			TMceDebug::Print( str );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr1, TUint32 aValue1, const TDesC16& aStr2 ) 
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Client] %S=%d %S");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, aValue1, &aStr2 );
			TMceDebug::Print( str );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print( const TDesC16& aStr1, const TDesC8& aValue1 ) 
	    {
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<100> str2;
			CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aValue1);

			_LIT(KMceFormat, "[Client] %S=\"%S\"");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, &str2);
			TMceDebug::Print( str );
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}		    
	    }
	    

#endif

	};


#ifdef _DEBUG 

#define MCECLI_DEBUG( a ) { TMceCliDebug::Print( _L( a ) ); }
#define MCECLI_DEBUG2( a, b ) { TMceCliDebug::Print( _L( a ), _L( b ) ); }
#define MCECLI_DEBUG_DVALUE( a, b ) { TMceCliDebug::Print( _L( a ),  b  ); }
#define MCECLI_DEBUG_DVALUES( a, b, c, d ) { TMceCliDebug::Print( _L( a ),  b, _L( c ), d  ); }
#define MCECLI_DEBUG_DVALUES2( a, b, c, d, e, f ) { TMceCliDebug::Print( _L( a ),  b, _L( c ), d, _L( e ), f ); }
#define MCECLI_DEBUG_SVALUE( a, b ) { TMceCliDebug::Print( _L( a ), b ); }
#define MCECLI_DEBUG_SVALUE16( a, b ) { TMceCliDebug::Print( _L( a ), b ); }

    
#define MCECLI_DEBUG_CALLBACK( str, callback )\
        MCE_DEBUG_CALLBACK( TMceCliDebug, str, callback )

#define MCECLI_DEBUG_DATATYPE( str, datatype )\
        MCE_DEBUG_DATATYPE( TMceCliDebug, str, datatype )
        
#define MCECLI_DEBUG_CLISTATE( str, state )\
        MCE_DEBUG_CLISTATE( TMceCliDebug, str, state )
        
#define MCECLI_DEBUG_ITC( str, event )\
        MCE_DEBUG_ITC( TMceCliDebug, str, event )

#define MCECLI_DEBUG_SESSIONTYPE( str, type )\
        MCE_DEBUG_SESSIONTYPE( TMceCliDebug, str, type )

#define MCECLI_DEBUG_DIALOGTYPE( str, type )\
        MCE_DEBUG_DIALOGTYPE( TMceCliDebug, str, type )

#define MCECLI_DEBUG_MEDIASTATE( str, state )\
        MCE_DEBUG_MEDIASTATE( TMceCliDebug, str, state )

#define MCECLI_DEBUG_EVENTSTATE( str, state )\
        MCE_DEBUG_EVENTSTATE( TMceCliDebug, str, state )


        
#define MCECLI_DEBUG_IDS( str, ids )\
    MCECLI_DEBUG( str );\
    MCECLI_DEBUG_DVALUE("application uid", ids.iAppUID );\
    MCECLI_DEBUG_SESSIONTYPE("session type", ids.iManagerType );\
    MCECLI_DEBUG_DVALUE("session id", ids.iSessionID );\
    MCECLI_DEBUG_DIALOGTYPE("dialog type", ids.iDialogType );\
    MCECLI_DEBUG_DVALUE("dialog id", ids.iDialogID );\
    MCECLI_DEBUG_DVALUE("profile id", ids.iProfileID );\
    MCECLI_DEBUG_DATATYPE("message type", ids.iMsgType );\
    MCECLI_DEBUG_CALLBACK( "callback", ids.iCallbackID );\
    MCECLI_DEBUG_DVALUE( "status", ids.iStatus );\
    MCECLI_DEBUG_DVALUE("media id", ids.iMediaID.iId );\
    MCECLI_DEBUG_DVALUE("source id", ids.iSourceID.iId );\
    MCECLI_DEBUG_DVALUE("sink id", ids.iSinkID.iId );\
    MCECLI_DEBUG_DVALUE("codec id", ids.iCodecID.iId )



#else


#define MCECLI_DEBUG( a )
#define MCECLI_DEBUG2( a, b )
#define MCECLI_DEBUG_DVALUE( a, b )
#define MCECLI_DEBUG_DVALUES( a, b, c, d )
#define MCECLI_DEBUG_DVALUES2( a, b, c, d, e, f )
#define MCECLI_DEBUG_SVALUE( a, b )
#define MCECLI_DEBUG_CLISTATE( str, state )
#define MCECLI_DEBUG_DATATYPE( str, datatype )
#define MCECLI_DEBUG_ITC( str, event )
#define MCECLI_DEBUG_IDS( str, ids )
#define MCECLI_DEBUG_CALLBACK( str, callback )
#define MCECLI_DEBUG_SESSIONTYPE( str, type )
#define MCECLI_DEBUG_DIALOGTYPE( str, type )
#define MCECLI_DEBUG_MEDIASTATE( str, state )
#define MCECLI_DEBUG_EVENTSTATE( str, state )

#endif

#endif

// End of File
