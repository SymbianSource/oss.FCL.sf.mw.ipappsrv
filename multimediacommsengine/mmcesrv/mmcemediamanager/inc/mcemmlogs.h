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






#ifndef MCEMMLOGS_H
#define MCEMMLOGS_H

#include <sdpcodecstringconstants.h>
#include "mcelogs.h"
#include "mcesrvsink.h"
#include "mcesrvsource.h"
#include "mcecommediasink.h"
#include "mcecommediasource.h"
#include "mcecomcodec.h"
#include "mceaudiostream.h"
#include "mcevideostream.h"

// INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION

#ifdef MCEMM_UNIT_TEST_COVERAGE

#undef _DEBUG

#endif

/**
 * Class provides logging through RDebug in debug builds
 * 'MCE: ' added automatically in fron of every logged line
 */ 
class TMceMMDebug 
	{
	public:

#ifdef _DEBUG

	inline static void Print(const TDesC16& aStr) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Media Manager] %S");

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
			_LIT(KMceFormat, "[Media Manager] %S=%d");
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
			_LIT(KMceFormat, "[Media Manager] %S=%S");
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
			_LIT(KMceFormat, "[Media Manager] %S=%d, %S=%d");
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

			_LIT(KMceFormat, "[Media Manager] %S=\"%S\"");
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



#if ( defined _DEBUG ) && ( !defined EUNIT_UNIT_TEST_COVERAGE ) 

#define MCEMM_DEBUG( a ) { TMceMMDebug::Print( _L( a ) ); }
#define MCEMM_DEBUG2( a, b ) { TMceMMDebug::Print( _L( a ), _L( b ) ); }
#define MCEMM_DEBUG_DVALUE( a, b ) { TMceMMDebug::Print( _L( a ),  b  ); }
#define MCEMM_DEBUG_DVALUES( a, b, c, d ) { TMceMMDebug::Print( _L( a ),  b, _L( c ), d  ); }
#define MCEMM_DEBUG_SVALUE( a, b ) { TMceMMDebug::Print( _L( a ), b ); }
#define MCEMM_DEBUG_SVALUE16( a, b ) { TMceMMDebug::Print( _L( a ), b ); }
#define MCEMM_DEBUG_STATE( str, state )\
    switch( state )\
        {\
        case KMceMediaIdle:{ MCEMM_DEBUG2( str, "KMceMediaIdle" );break; }\
        case KMceOfferingMedia:{ MCEMM_DEBUG2( str, "KMceOfferingMedia" );break; }\
        case KMceAnsweringMedia:{ MCEMM_DEBUG2( str, "KMceAnsweringMedia" );break; }\
        case KMceAnsweringMediaUpdate:{ MCEMM_DEBUG2( str, "KMceAnsweringMediaUpdate" );break; }\
        case KMceMediaNegotiated:{ MCEMM_DEBUG2( str, "KMceMediaNegotiated" );break; }\
        case KMcePreparingOffererStreams:{ MCEMM_DEBUG2( str, "KMcePreparingOffererStreams" );break; }\
        case KMceStartingOffererReceiveStreams:{ MCEMM_DEBUG2( str, "KMceStartingOffererReceiveStreams" );break; }\
        case KMceStartingOffererSendStreams:{ MCEMM_DEBUG2( str, "KMceStartingOffererSendStreams" );break; }\
        case KMcePreparingAnswererStreams:{ MCEMM_DEBUG2( str, "KMcePreparingAnswererStreams" );break; }\
        case KMceStartingAnswererStreams:{ MCEMM_DEBUG2( str, "KMceStartingAnswererStreams" );break; }\
        default:{ MCEMM_DEBUG_DVALUE( str, state );break; }\
        }

#define MCEMM_DEBUG_STATE_1( str, session )\
    if ( &session.NegotiationState() )\
    	{\
    	MCEMM_DEBUG_STATE( str, session.NegotiationState().Id() );\
    	}\
	else\
		{\
		MCEMM_DEBUG( "no state!" );\
		}


#define MCEMM_DEBUG_MEDIASTATE( str, state )\
    switch( state )\
        {\
        case CMceSrvStream::EAdopted:{ MCEMM_DEBUG2( str, "EAdopted" );break; }\
        case CMceSrvStream::ECreated:{ MCEMM_DEBUG2( str, "ECreated" );break; }\
        case CMceSrvStream::ECreatingLink:{ MCEMM_DEBUG2( str, "ECreatingLink" );break; }\
        case CMceSrvStream::ELinkCreated:{ MCEMM_DEBUG2( str, "ELinkCreated" );break; }\
        case CMceSrvStream::EPreparing:{ MCEMM_DEBUG2( str, "EPreparing" );break; }\
        case CMceSrvStream::EPrepared:{ MCEMM_DEBUG2( str, "EPrepared" );break; }\
        case CMceSrvStream::EStarting:{ MCEMM_DEBUG2( str, "EStarting" );break; }\
        case CMceSrvStream::EPending:{ MCEMM_DEBUG2( str, "EPending" );break; }\
        case CMceSrvStream::EStopped:{ MCEMM_DEBUG2( str, "EStopped" );break; }\
        case CMceSrvStream::EInactive:{ MCEMM_DEBUG2( str, "EInactive" );break; }\
        case CMceSrvStream::EStarted:{ MCEMM_DEBUG2( str, "EStarted" );break; }\
        case CMceSrvStream::EPausing:{ MCEMM_DEBUG2( str, "EPausing" );break; }\
        case CMceSrvStream::EPaused:{ MCEMM_DEBUG2( str, "EPaused" );break; }\
        default:{ MCEMM_DEBUG_DVALUE( str, state );break; }\
        }

#define MCEMM_DEBUG_STREAMMEDIA( str, type )\
    switch( type )\
        {\
        case KMceAudio:{ MCEMM_DEBUG2( str, "audio" );break; }\
        case KMceVideo:{ MCEMM_DEBUG2( str, "video" );break; }\
        default:{ MCEMM_DEBUG_DVALUE( str, type );break; }\
        }

#define MCEMM_DEBUG_STREAMTYPE( str, type )\
    switch( type )\
        {\
        case CMceComMediaStream::ELocalStream:{ MCEMM_DEBUG2( str, "ELocalStream" );break; }\
        case CMceComMediaStream::ESendOnlyStream:{ MCEMM_DEBUG2( str, "ESendOnlyStream" );break; }\
        case CMceComMediaStream::EReceiveOnlyStream:{ MCEMM_DEBUG2( str, "EReceiveOnlyStream" );break; }\
        case CMceComMediaStream::ESendStream:{ MCEMM_DEBUG2( str, "ESendStream" );break; }\
        case CMceComMediaStream::EReceiveStream:{ MCEMM_DEBUG2( str, "EReceiveStream" );break; }\
        default:{ MCEMM_DEBUG_DVALUE( str, type );break; }\
        }

#define MCEMM_DEBUG_STREAMDIRECTION( str, type )\
    switch( type )\
        {\
        case SdpCodecStringConstants::EAttributeSendrecv:{ MCEMM_DEBUG2( str, "sendrecv" );break; }\
        case SdpCodecStringConstants::EAttributeRecvonly:{ MCEMM_DEBUG2( str, "recvonly" );break; }\
        case SdpCodecStringConstants::EAttributeSendonly:{ MCEMM_DEBUG2( str, "sendonly" );break; }\
        case SdpCodecStringConstants::EAttributeInactive:{ MCEMM_DEBUG2( str, "inactive" );break; }\
        default:{ MCEMM_DEBUG_DVALUE( str, type );break; }\
        }

/*lint -e666 */
#define MCEMM_DEBUG_STREAM( str, stream )\
    MCEMM_DEBUG( str );\
    if ( !(stream).IsAdopted() )\
        {\
        MCEMM_DEBUG_DVALUE(" Session id          ", (stream).SessionId() );\
        MCEMM_DEBUG_STREAMMEDIA(" Stream media        ", (stream).Data().iType );\
        MCEMM_DEBUG_DVALUE(" Stream id           ", (stream).Id() );\
        MCEMM_DEBUG_DVALUE(" Stream link id      ", (stream).LinkId() );\
        MCEMM_DEBUG_STREAMTYPE(" Stream type         ", (stream).StreamType() );\
        MCEMM_DEBUG_DVALUE(" Stream enabled      ", (stream).Data().IsEnabled() );\
        MCEMM_DEBUG_STREAMDIRECTION(" Stream direction    ", (stream).Data().Direction() );\
        MCEMM_DEBUG_MEDIASTATE(" Stream state        ", (stream).State() );\
        MCEMM_DEBUG_SVALUE( " Codec name          ", (stream).Codec().iSdpName );\
        MCEMM_DEBUG_DVALUE( " Codec enabled       ", (stream).Codec().iIsEnabled );\
        MCEMM_DEBUG_DVALUE( " Codec payload       ", (stream).Codec().iPayloadType );\
        MCEMM_DEBUG_DVALUE(" Sink id             ", (stream).Sink().Id() );\
        MCEMM_DEBUG_MEDIASTATE(" Sink state          ", (stream).Sink().State() );\
        MCEMM_DEBUG_DVALUE(" Sink enabled (l)    ", (stream).Sink().Data().IsEnabled() );\
        MCEMM_DEBUG_DVALUE(" Sink enabled (e2e)  ", (stream).Sink().Data().IsEnabled( KMceEnabledE2e ) );\
        MCEMM_DEBUG_DVALUE(" Source id           ", (stream).Source().Id() );\
        MCEMM_DEBUG_MEDIASTATE(" Source state        ", (stream).Source().State() );\
        MCEMM_DEBUG_DVALUE(" Source enabled (l)  ", (stream).Source().Data().IsEnabled() );\
        MCEMM_DEBUG_DVALUE(" Source enabled (e2e)", (stream).Source().Data().IsEnabled( KMceEnabledE2e ) );\
        }\
    else\
        {\
        MCEMM_DEBUG_DVALUE(" Session id          ", (stream).SessionId() );\
        MCEMM_DEBUG_DVALUE(" Stream id           ", (stream).Id() );\
        MCEMM_DEBUG_DVALUE(" Stream link id      ", (stream).LinkId() );\
        }

#else


#define MCEMM_DEBUG( a )
#define MCEMM_DEBUG2( a, b )
#define MCEMM_DEBUG_DVALUE( a, b )
#define MCEMM_DEBUG_DVALUES( a, b, c, d )
#define MCEMM_DEBUG_SVALUE( a, b )
#define MCEMM_DEBUG_STATE( str, state )
#define MCEMM_DEBUG_STATE_1( str, session )
#define MCEMM_DEBUG_MEDIASTATE( str, state )
#define MCEMM_DEBUG_STREAMTYPE( str, type )
#define MCEMM_DEBUG_STREAMDIRECTION( str, type )
#define MCEMM_DEBUG_STREAM( str, stream )
#define MCEMM_DEBUG_STREAMMEDIA( str, type )


#endif

#endif

// End of File
