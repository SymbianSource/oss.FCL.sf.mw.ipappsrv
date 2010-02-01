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






#ifndef MCEEVENTSLOGS_H
#define MCEEVENTSLOGS_H

#include "mcelogs.h"

// INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * Class provides logging through RDebug in debug builds
 * 'MCE: ' added automatically in fron of every logged line
 */ 
class TMceEventsDebug 
	{
	public:

#ifdef _DEBUG

	inline static void Print(const TDesC16& aStr) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[EVENTS] %S");

			TBuf<256> str;
			str.Format(KMceFormat, &aStr);
			TMceDebug::Print( str );
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
			_LIT(KMceFormat, "[EVENTS] %S=%d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, aValue1);
			TMceDebug::Print( str );
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
			_LIT(KMceFormat, "[EVENTS] %S=%S");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, &aStr2);
			TMceDebug::Print( str );
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
			_LIT(KMceFormat, "[EVENTS] %S=%d, %S=%d");
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

	inline static void Print( const TDesC16& aStr1, const TDesC8& aValue1 ) 
	    {
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<100> str2;
			CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aValue1);

			_LIT(KMceFormat, "[EVENTS] %S=\"%S\"");
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

#define MCEEVENTS_DEBUG( a ) { TMceEventsDebug::Print( _L( a ) ); }
#define MCEEVENTS_DEBUG2( a, b ) { TMceEventsDebug::Print( _L( a ), _L( b ) ); }
#define MCEEVENTS_DEBUG_DVALUE( a, b ) { TMceEventsDebug::Print( _L( a ),  b  ); }
#define MCEEVENTS_DEBUG_DVALUES( a, b, c, d ) { TMceEventsDebug::Print( _L( a ),  b, _L( c ), d  ); }
#define MCEEVENTS_DEBUG_SVALUE( a, b ) { TMceEventsDebug::Print( _L( a ), b ); }
#define MCEEVENTS_DEBUG_SVALUE16( a, b ) { TMceEventsDebug::Print( _L( a ), b ); }
#define MCEEVENTS_DEBUG_NEXTSTATE( str, nextState )\
    switch( nextState )\
        {\
        case 0:{ MCEEVENTS_DEBUG2( str, "KMceTerminatingEventStateIndex" );break; }\
        case 1:{ MCEEVENTS_DEBUG2( str, "KMceTerminatedEventStateIndex" );break; }\
        case 2:{ MCEEVENTS_DEBUG2( str, "KMceIdleEventStateIndex" );break; }\
        case 3:{ MCEEVENTS_DEBUG2( str, "KMceEstablishingEventStateIndex" );break; }\
        case 4:{ MCEEVENTS_DEBUG2( str, "KMceEstablishedEventStateIndex" );break; }\
        default:{ MCEEVENTS_DEBUG_DVALUE( str, nextState );break; }\
        }

#define MCEEVENTS_DEBUG_NEXTCLIENTSTATE( str, nextClientState )\
    switch( nextClientState )\
        {\
        case CMceEvent::EIdle:{ MCEEVENTS_DEBUG2( str, "EIdle" );break; }\
        case CMceEvent::EPending:{ MCEEVENTS_DEBUG2( str, "EPending" );break; }\
        case CMceEvent::EActive:{ MCEEVENTS_DEBUG2( str, "EActive" );break; }\
        case CMceEvent::ETerminated:{ MCEEVENTS_DEBUG2( str, "ETerminated" );break; }\
        default:{ MCEEVENTS_DEBUG_DVALUE( str, nextClientState );break; }\
        }
    
#else

#define MCEEVENTS_DEBUG( a )
#define MCEEVENTS_DEBUG2( a, b )
#define MCEEVENTS_DEBUG_DVALUE( a, b )
#define MCEEVENTS_DEBUG_DVALUES( a, b, c, d )
#define MCEEVENTS_DEBUG_SVALUE( a, b )
#define MCEEVENTS_DEBUG_NEXTSTATE( str, state )
#define MCEEVENTS_DEBUG_NEXTCLIENTSTATE( str, state )


#endif

#endif

// End of File

