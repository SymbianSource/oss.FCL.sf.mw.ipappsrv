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






#ifndef MCESRVLOGS_H
#define MCESRVLOGS_H

#include <in_sock.h>
#include "mcecomlogs.h"
#include "mceevents.h"
#include "mceclientserver.h"
#include "mcesession.h"
#include "mceaudiostream.h"
#include "sipstrconsts.h"

// INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION

#ifdef MCESRV_UNIT_TEST_COVERAGE

#undef _DEBUG

#endif

/**
 * Class provides logging through RDebug in debug builds
 * 'MCE: ' added automatically in fron of every logged line
 */ 
class TMceSrvDebug 
	{
	public:

#ifdef _DEBUG

	inline static void Print( const TDesC16& aStr ) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Server] %S");

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

	inline static void Print( const TDesC16& aStr1, TUint32 aValue1 )
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Server] %S=%d");
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
			_LIT(KMceFormat, "[Server] %S=%S");
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

	inline static void Print( const TDesC16& aStr1, TUint32 aValue1, const TDesC16& aStr2, TUint32 aValue2 ) 
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Server] %S=%d, %S=%d");
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

	inline static void Print( const TDesC16& aStr1, TUint32 aValue1, const TDesC16& aStr2 ) 
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "[Server] %S=%d %S");
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

			_LIT(KMceFormat, "[Server] %S=\"%S\"");
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
	    
    inline static void Print( const TDesC16& aStr, const TInetAddr& aAddr )
        {
        const TInt KIPv6AddrMaxLen = 39;
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
            TBuf<KIPv6AddrMaxLen> addrBuf;
            aAddr.Output(addrBuf);
    		_LIT(KMceFormat, "MCE: [Server] %S=%S port %d");
    		TBuf<256> str;
            str.Format(KMceFormat, &aStr, &addrBuf, aAddr.Port());
            RDebug::RawPrint(str);
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

#define MCESRV_DEBUG( a ) { TMceSrvDebug::Print( _L( a ) ); }
#define MCESRV_DEBUG2( a, b ) { TMceSrvDebug::Print( _L( a ), _L( b ) ); }
#define MCESRV_DEBUG_DVALUE( a, b ) { TMceSrvDebug::Print( _L( a ),  b  ); }
#define MCESRV_DEBUG_ADDRVALUE( a, b ) { TMceSrvDebug::Print( _L( a ),  b  ); }
#define MCESRV_DEBUG_DVALUES( a, b, c, d ) { TMceSrvDebug::Print( _L( a ),  b, _L( c ), d  ); }
#define MCESRV_DEBUG_SVALUE( a, b ) { TMceSrvDebug::Print( _L( a ), b ); }
#define MCESRV_DEBUG_SVALUE16( a, b ) { TMceSrvDebug::Print( _L( a ), b ); }

#define MCESRV_DEBUG_CALLBACK( str, callback )\
        MCE_DEBUG_CALLBACK( TMceSrvDebug, str, callback )

#define MCESRV_DEBUG_DATATYPE( str, datatype )\
        MCE_DEBUG_DATATYPE( TMceSrvDebug, str, datatype )
        
#define MCESRV_DEBUG_CLISTATE( str, state )\
        MCE_DEBUG_CLISTATE( TMceSrvDebug, str, state )
        
#define MCESRV_DEBUG_ITC( str, event )\
        MCE_DEBUG_ITC( TMceSrvDebug, str, event )

#define MCESRV_DEBUG_SESSIONTYPE( str, type )\
        MCE_DEBUG_SESSIONTYPE( TMceSrvDebug, str, type )

#define MCESRV_DEBUG_DIALOGTYPE( str, type )\
        MCE_DEBUG_DIALOGTYPE( TMceSrvDebug, str, type )

#define MCESRV_DEBUG_MEDIASTATE( str, state )\
        MCE_DEBUG_MEDIASTATE( TMceSrvDebug, str, state )

#define MCESRV_DEBUG_EVENTSTATE( str, state )\
        MCE_DEBUG_EVENTSTATE( TMceSrvDebug, str, state )

#define MCESRV_DEBUG_ARRAY( str, line, array )\
    if ( array )\
        {\
        MCESRV_DEBUG( str );\
        for( TInt _mced_array_ndx=0;_mced_array_ndx < array->MdcaCount();_mced_array_ndx++ )\
            {\
            MCESRV_DEBUG_SVALUE( line, array->MdcaPoint( _mced_array_ndx ) );\
            }\
        }


//        MCE_DEBUG_ARRAY( TMceSrvDebug, str, line, array )

#define MCESRV_DEBUG_METHOD( str, method )\
    switch( method )\
        {\
        case SipStrConsts::EInvite:{ MCESRV_DEBUG2( str, "INVITE" );break; }\
        case SipStrConsts::EUpdate:{ MCESRV_DEBUG2( str, "UPDATE" );break; }\
        case SipStrConsts::EPrack:{ MCESRV_DEBUG2( str, "PRACK" );break; }\
        case SipStrConsts::EBye:{ MCESRV_DEBUG2( str, "BYE" );break; }\
        case SipStrConsts::ESubscribe:{ MCESRV_DEBUG2( str, "SUBSCRIBE" );break; }\
        case SipStrConsts::ERefer:{ MCESRV_DEBUG2( str, "REFER" );break; }\
        case SipStrConsts::EOptions:{ MCESRV_DEBUG2( str, "OPTIONS" );break; }\
        case SipStrConsts::EAck:{ MCESRV_DEBUG2( str, "ACK" );break; }\
        default:{ MCESRV_DEBUG2( str, "UNKNOWN METHOD" );break; }\
        }
    
    
#define MCESRV_DEBUG_STATE( str, state )\
    switch( state )\
        {\
        case KMceStateIdle:{ MCESRV_DEBUG2( str, "KMceStateIdle" );break; }\
        case KMceStateClientEstablishing:{ MCESRV_DEBUG2( str, "KMceStateClientEstablishing" );break; }\
        case KMceStateOffering:{ MCESRV_DEBUG2( str, "KMceStateOffering" );break; }\
        case KMceStateEstablished:{ MCESRV_DEBUG2( str, "KMceStateEstablished" );break; }\
        case KMceStateTerminating:{ MCESRV_DEBUG2( str, "KMceStateTerminating" );break; }\
        case KMceStateTerminated:{ MCESRV_DEBUG2( str, "KMceStateTerminated" );break; }\
        case KMceStateServerEstablishing:{ MCESRV_DEBUG2( str, "KMceStateServerEstablishing" );break; }\
        case KMceStateUpdating:{ MCESRV_DEBUG2( str, "KMceStateUpdating" );break; }\
        case KMceStateUpdated:{ MCESRV_DEBUG2( str, "KMceStateUpdated" );break; }\
        case KMceStateAnswering:{ MCESRV_DEBUG2( str, "KMceStateAnswering" );break; }\
        case KMceStateError:{ MCESRV_DEBUG2( str, "KMceStateError" );break; }\
        case KMceStateCanceled:{ MCESRV_DEBUG2( str, "KMceStateCanceled" );break; }\
        default:{ MCESRV_DEBUG_DVALUE( str, state );break; }\
        }

#define MCESRV_DEBUG_MCE_EVENT( str, event )\
    switch( (TMceMediaEventCode)event )\
        {\
        case EMceMediaSessionStopped:{ MCESRV_DEBUG2( str, "EMceMediaSessionStopped" );break; }\
        case EMceMediaError:{ MCESRV_DEBUG2( str, "EMceMediaError" );break; }\
        case EMceMediaReserved:{ MCESRV_DEBUG2( str, "EMceMediaReserved" );break; }\
        case EMceMediaUpdated:{ MCESRV_DEBUG2( str, "EMceMediaUpdated" );break; }\
        case EMceMediaEventReceived:{ MCESRV_DEBUG2( str, "EMceMediaEventReceived" );break; }\
        default:{ MCESRV_DEBUG_SIP_EVENT( str, event );break; }\
        }
        

#define MCESRV_DEBUG_SIP_EVENT( str, event )\
    switch( (TMceSipEventCode)event )\
        {\
        case EMceRegister:{ MCESRV_DEBUG2( str, "EMceRegister" );break; }\
        case EMceInvite:{ MCESRV_DEBUG2( str, "EMceInvite" );break; }\
        case EMceCancel:{ MCESRV_DEBUG2( str, "EMceCancel" );break; }\
        case EMceAck:{ MCESRV_DEBUG2( str, "EMceAck" );break; }\
        case EMceBye:{ MCESRV_DEBUG2( str, "EMceBye" );break; }\
        case EMcePrack:{ MCESRV_DEBUG2( str, "EMcePrack" );break; }\
        case EMceUpdate:{ MCESRV_DEBUG2( str, "EMceUpdate" );break; }\
        case EMceSubscribe:{ MCESRV_DEBUG2( str, "EMceSubscribe" );break; }\
        case EMceRefer:{ MCESRV_DEBUG2( str, "EMceRefer" );break; }\
        case EMceMessage:{ MCESRV_DEBUG2( str, "EMceMessage" );break; }\
        case EMceInfo:{ MCESRV_DEBUG2( str, "EMceInfo" );break; }\
        case EMceResponse:{ MCESRV_DEBUG2( str, "EMceResponse" );break; }\
        case EMceProvisionalResponse:{ MCESRV_DEBUG2( str, "EMceProvisionalResponse" );break; }\
        case EMceRedirectionResponse:{ MCESRV_DEBUG2( str, "EMceRedirectionResponse" );break; }\
        case EMceErrorResponse:{ MCESRV_DEBUG2( str, "EMceErrorResponse" );break; }\
        case EMceStandAloneRequest:{ MCESRV_DEBUG2( str, "EMceStandAloneRequest" );break; }\
        case EMceStandAloneResponse:{ MCESRV_DEBUG2( str, "EMceStandAloneResponse" );break; }\
        case EMceSessionExpired:{ MCESRV_DEBUG2( str, "EMceSessionExpired" );break; }\
        case EMceSessionRefresh:{ MCESRV_DEBUG2( str, "EMceSessionRefresh" );break; }\
        default:{ MCESRV_DEBUG_DVALUE( str, event );break; }\
        }

#define MCESRV_DEBUG_EVENT( str, event )\
    if( (TMceItcFunctions)event <= EMceItcCancelTranscode )\
        {\
        MCESRV_DEBUG_ITC( str, event );\
        }\
    else if ( (TMceMediaEventCode)event >= EMceMediaSessionStopped && (TMceMediaEventCode)event <= EMceMediaEventReceived )\
        {\
        MCESRV_DEBUG_MCE_EVENT( str, event );\
        }\
    else if ( (TMceSipEventCode)event >= EMceRegister && (TMceSipEventCode)event <= EMceReliableFailed )\
        {\
        MCESRV_DEBUG_SIP_EVENT( str, event );\
        }
    

#define MCESRV_DEBUG_SUBSTATE( str, state )\
    switch( state )\
        {\
        case CMceSipSession::ENone:{ MCESRV_DEBUG2( str, "ENone" );break; }\
        case CMceSipSession::EOffering:{ MCESRV_DEBUG2( str, "EOffering" );break; }\
        case CMceSipSession::EAnswering:{ MCESRV_DEBUG2( str, "EAnswering" );break; }\
        case CMceSipSession::EUpdating:{ MCESRV_DEBUG2( str, "EUpdating" );break; }\
        case CMceSipSession::ERefreshing:{ MCESRV_DEBUG2( str, "ERefreshing" );break; }\
        default:{ MCESRV_DEBUG_DVALUE( str, state );break; }\
        }
        
#define MCESRV_DEBUG_IDS( str, ids )\
    MCESRV_DEBUG( str );\
    MCESRV_DEBUG_DVALUE("application uid", ids.iAppUID );\
    MCESRV_DEBUG_SESSIONTYPE("session type", ids.iManagerType );\
    MCESRV_DEBUG_DVALUE("session id", ids.iSessionID );\
    MCESRV_DEBUG_DIALOGTYPE("dialog type", ids.iDialogType );\
    MCESRV_DEBUG_DVALUE("dialog id", ids.iDialogID );\
    MCESRV_DEBUG_DVALUE("profile id", ids.iProfileID );\
    MCESRV_DEBUG_DATATYPE("message type", ids.iMsgType );\
    MCESRV_DEBUG_CALLBACK( "callback", ids.iCallbackID );\
    MCESRV_DEBUG_CLISTATE("state", ids.iState );\
    MCESRV_DEBUG_DVALUE("spare1", ids.iSpare1 );\
    MCESRV_DEBUG_DVALUE("spare2", ids.iSpare2 );\
    MCESRV_DEBUG_DVALUE("status", ids.iStatus );\
    MCESRV_DEBUG_DVALUE("media id", ids.iMediaID.iId );\
    MCESRV_DEBUG_DVALUE("source id", ids.iSourceID.iId );\
    MCESRV_DEBUG_DVALUE("sink id", ids.iSinkID.iId );\
    MCESRV_DEBUG_DVALUE("codec id", ids.iCodecID.iId )


#define MCESRV_DEBUG_CODEC( str, codec )\
    if ( codec )\
        {\
        MCESRV_DEBUG( str );\
        MCESRV_DEBUG_DVALUES("  iID.iAppId", codec->iID.iAppId, "iID.iId", codec->iID.iId );\
        MCESRV_DEBUG_DVALUE( "  iEnableVAD", codec->iEnableVAD );\
        MCESRV_DEBUG_DVALUE( "  iSamplingFreq", codec->iSamplingFreq );\
        MCESRV_DEBUG_SVALUE( "  iSdpName", codec->iSdpName );\
        MCESRV_DEBUG_DVALUE( "  iPTime", codec->iPTime );\
        MCESRV_DEBUG_DVALUE( "  iMaxPTime", codec->iMaxPTime );\
        MCESRV_DEBUG_DVALUE( "  iBitrate", codec->iBitrate );\
        MCESRV_DEBUG_DVALUE( "  iAllowedBitrates", codec->iAllowedBitrates );\
        MCESRV_DEBUG_DVALUE( "  iPayloadType", codec->iPayloadType );\
        MCESRV_DEBUG_DVALUE( "  iCodecMode", codec->iCodecMode );\
        MCESRV_DEBUG_DVALUE( "  iFourCC", codec->iFourCC );\
        MCESRV_DEBUG_SVALUE( "  iFmtpAttr", codec->iFmtpAttr ? *codec->iFmtpAttr : KNullDesC8() );\
        MCESRV_DEBUG_DVALUE( "  iFrameSize", codec->iFrameSize );\
        MCESRV_DEBUG_DVALUE( "  iStream", (TUint32)codec->Stream() );\
        }

#define _MCESRV_DEBUG_MEDIASTREAM( str, media )\
        MCESRV_DEBUG( str );\
        MCESRV_DEBUG_DVALUES(" iID.iAppId", media->iID.iAppId, "iID.iId", media->iID.iId );\
        MCESRV_DEBUG_DVALUE( " iType", media->iType );\
        MCESRV_DEBUG_DVALUE( " iIsEnabled", media->iIsEnabled );\
        MCESRV_DEBUG_DVALUE( " iLocalMediaPort", media->iLocalMediaPort );\
        MCESRV_DEBUG_DVALUE( " iRemoteMediaPort", media->iRemoteMediaPort );\
        MCESRV_DEBUG_DVALUE( " iRemoteRtcpPort", media->iRemoteRtcpPort );\
        MCESRV_DEBUG_ARRAY(  " iLocalMediaSDPLines:", "  medialine", media->iLocalMediaSDPLines );\
        MCESRV_DEBUG_ARRAY(  " iRemoteMediaSDPLines:", "  medialine", media->iRemoteMediaSDPLines );\
        MCESRV_DEBUG_DVALUE( " iSession", (TUint32)media->Session() )

        
#define MCESRV_DEBUG_MEDIASTREAM( str, media )\
    if ( media )\
        {\
        _MCESRV_DEBUG_MEDIASTREAM( str, media );\
        if ( media->iType == KMceAudio )\
            {\
            CMceComAudioStream* audio = static_cast<CMceComAudioStream*>( media );\
            TInt _mced_codec_ndx = 0;\
            for( _mced_codec_ndx=0;_mced_codec_ndx < audio->CodecCount();_mced_codec_ndx++)\
                {\
                MCESRV_DEBUG_CODEC( "  codec:", audio->CodecL( _mced_codec_ndx ) );\
                }\
            if ( audio->iLinkedStream )\
                {\
                _MCESRV_DEBUG_MEDIASTREAM( " linked stream:", audio->iLinkedStream );\
                if ( audio->iLinkedStream->iType == KMceAudio )\
                    {\
                    audio = static_cast<CMceComAudioStream*>( audio->iLinkedStream );\
                    for( _mced_codec_ndx=0;_mced_codec_ndx < audio->CodecCount();_mced_codec_ndx++)\
                        {\
                        MCESRV_DEBUG_CODEC( "  codec:", audio->CodecL( _mced_codec_ndx ) );\
                        }\
                    }\
                }\
            }\
        }

/*lint -e666 */

#define MCESRV_DEBUG_COMSESSION( body )\
    MCESRV_DEBUG_DVALUE("id", body->iID );\
    MCESRV_DEBUG_DVALUE("type", body->iType );\
    MCESRV_DEBUG_DVALUE("state", body->iState );\
    MCESRV_DEBUG_SVALUE("recipient", body->iRecipient ? *body->iRecipient : KNullDesC8() );\
    MCESRV_DEBUG_SVALUE("originator", body->iOriginator ? *body->iOriginator : KNullDesC8() );\
    MCESRV_DEBUG_DVALUE("connectionActive", body->iIsConnectionActive );\
    MCESRV_DEBUG_DVALUE("dialog id", body->iDialogId );\
    MCESRV_DEBUG_DVALUE("iap id", body->iIapId );\
    MCESRV_DEBUG_DVALUE("timeout", body->iTimeout );\
    MCESRV_DEBUG_DVALUE("min se", body->iMinSE );\
    MCESRV_DEBUG_DVALUE("refresher", body->iRefresh );\
    MCESRV_DEBUG_DVALUE("mcc id", body->iMccID );\
    MCESRV_DEBUG_ARRAY( "SIP headers:", " header", body->iSIPHeaders );\
    MCESRV_DEBUG_ARRAY( "local session SDP lines:", "  medialine", body->iLocalSessionSDPLines );\
    MCESRV_DEBUG_ARRAY( "remote session SDP lines:", "  medialine", body->iRemoteSessionSDPLines );\
    MCESRV_DEBUG_SVALUE("content type", body->iSIPContentType ? *body->iSIPContentType : KNullDesC8() );\
    for( TInt _mced_stream_ndx=0; _mced_stream_ndx < body->Streams().Count(); _mced_stream_ndx++ )\
        {\
        MCESRV_DEBUG_MEDIASTREAM( " stream:",  body->Streams()[ _mced_stream_ndx ] );\
        }

#else


#define MCESRV_DEBUG( a )
#define MCESRV_DEBUG2( a, b )
#define MCESRV_DEBUG_DVALUE( a, b )
#define MCESRV_DEBUG_ADDRVALUE( a, b )
#define MCESRV_DEBUG_DVALUES( a, b, c, d )
#define MCESRV_DEBUG_SVALUE( a, b )
#define MCESRV_DEBUG_SVALUE16( a, b )
#define MCESRV_DEBUG_CALLBACK( str, callback )
#define MCESRV_DEBUG_DATATYPE( str, datatype )
#define MCESRV_DEBUG_CLISTATE( str, state )
#define MCESRV_DEBUG_ITC( str, event )
#define MCESRV_DEBUG_SESSIONTYPE( str, type )
#define MCESRV_DEBUG_DIALOGTYPE( str, type )
#define MCESRV_DEBUG_MEDIASTATE( str, state )
#define MCESRV_DEBUG_EVENTSTATE( str, state )
#define MCESRV_DEBUG_ARRAY( str, line, array )
#define MCESRV_DEBUG_METHOD( str, method )
#define MCESRV_DEBUG_STATE( str, state )
#define MCESRV_DEBUG_CLISTATE( str, state )
#define MCESRV_DEBUG_EVENT( str, event )
#define MCESRV_DEBUG_MCE_EVENT( str, event )
#define MCESRV_DEBUG_SIP_EVENT( str, event )
#define MCESRV_DEBUG_SUBSTATE( str, state )
#define MCESRV_DEBUG_IDS( str, ids )
#define MCESRV_DEBUG_CALLBACK( str, callback )
#define MCESRV_DEBUG_CODEC( str, codec )
#define MCESRV_DEBUG_MEDIASTREAM( str, audio )
#define _MCESRV_DEBUG_MEDIASTREAM( str, media )
#define MCESRV_DEBUG_COMSESSION( body )

#endif

#endif

// End of File
