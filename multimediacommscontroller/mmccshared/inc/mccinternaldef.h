/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Mcc Internal definitions
*
*/




#ifndef MCCINTERNALDEF_H
#define MCCINTERNALDEF_H

// INCLUDES
#include <e32base.h>
#include <mmf/common/mmfcontroller.h>
#include <tconvbase64.h>  
#include <e32math.h>
#include <3gplibrary/mp4lib.h>


#include "rtpdef.h"
#include "mmccinterfacedef.h"
#include "mccinternalcodecs.h"
#include "mccuids.hrh"

// CONSTANTS
const TUid KMccControllerUid        = { KImplUidMccController };
const TUid KMccAmrFormatterUid      = { KDllUidAmrPayloadFormat };
const TUid KMccJitterBufferUid      = { KDllUidMccJitterBuffer };
const TUid KMccMultiplexerUid       = { KDllUidMccMultiplexer };
const TUid KMccDtmfFormatterUid     = { KDllUidDTMFPayloadFormat };
const TUid KMccMedianattraversalUid = { KImplUidMccmedianattraversal };

const TInt KMaxBitrates = 12;

// VoIP codecs have a two byte header
const TInt KVoIPHeaderLength = 2;

// VoIP codec header bytes can be audio, comfort noise or unvoiced non-sid
// (null/no data) frame. Unvoiced non sid frame can be detected by header
// byte being something else than audio or comfort noise.
const TUint8 KVoIPAudioFrame = 1;
const TUint8 KVoIPCNFrame = 2;

// Bits per byte
const TInt KBitsPerByte = 8;

// Defines unassigned payload type
const TUint8 KMccPTNotDefined = 128;

// Used to define exitence of codec which is not supported by mcc
const TUint32 KMccFourCCUnsupported = 0x4EEEEEEE;

const TInt KMccFourCCArraySize = 10;

// Use TPckgBuf<TInt> as a parameter value. Should not overlap with values from API.
const TUint32 KMccPrerollTime = 60;

// Increased RTP socket size in bytes, default socket size is around 4KB
const TInt KMccIncreasedRtpSocketSize = 10000;

// Values used to force better interoperability against some vendors
const TReal KMccH263ProfileZeroMaxFramerateIOP = 15;
const TUint32 KMccH263ProfileZeroMinBitRateIOP = 64001;

// MACROS

#ifdef _DEBUG
    #define TRACE_MCC_CONTROLLER
    #define TRACE_MCC_INTERFACE
#endif


#ifdef TRACE_MCC_CONTROLLER
    #define MCC_CONTROLLER_WRITE(a) \
    	{ _LIT( KName, a); RDebug::Print(KName); }
    #define MCC_CONTROLLER_WRITE2(a, b) \
    	{ _LIT(KName, a); RDebug::Print(KName, b); }
   	#define MCC_CONTROLLER_WRITE3(a, b, c) \
    	{ _LIT(KName, a); RDebug::Print(KName, b, c); }
    #define MCC_CONTROLLER_WRITE4(a, b, c, d) \
    	{ _LIT(KName, a); RDebug::Print(KName, b, c, d); }
#else
   	#define MCC_CONTROLLER_WRITE(a) 
   	#define MCC_CONTROLLER_WRITE2(a, b)
   	#define MCC_CONTROLLER_WRITE3(a, b, c)
   	#define MCC_CONTROLLER_WRITE4(a, b, c, d)
#endif

#define MCC_CONTROL_ALL_ENDPOINTS(a) ( a == 0 )

#define MCC_ENDPOINT_ID(a) reinterpret_cast<TUint32>( a )

#define IS_MCC_EVENT(a)\
( a.iEventType == KUidMediaTypeAudio ||\
a.iEventType == KUidMediaTypeVideo ||\
a.iEventType == KMccFileSinkUid ||\
a.iEventType == KMccFileSourceUid ||\
a.iEventType == KMccRtpSourceUid ||\
a.iEventType == KMccRtpSinkUid ||\
a.iEventType == KMccVideoSourceUid ||\
a.iEventType == KMccVideoSinkUid ||\
a.iEventType == KMccJitterBufferUid ||\
a.iEventType == KMccAmrFormatterUid ||\
a.iEventType == KMccMultiplexerUid ||\
a.iEventType == KMccDtmfFormatterUid ||\
a.iEventType == KMccAnySourceUid ||\
a.iEventType == KMccAnySinkUid )

#define MCC_STREAM_STATE_CHANGE_EVENT( aEvent )\
( aEvent->iEventCategory == KMccEventCategoryStream &&\
( aEvent->iEventType == KMccStreamPrepared ||\
  aEvent->iEventType == KMccStreamStarted ||\
  aEvent->iEventType == KMccStreamPaused ||\
  aEvent->iEventType == KMccStreamResumed ||\
  aEvent->iEventType == KMccStreamStopped ) )

#define MCC_INTERNAL_ENDPOINT( aEndpointUid )\
( aEndpointUid == KImplUidMccFileSource ||\
  aEndpointUid == KImplUidMccFileSink ||\
  aEndpointUid == KImplUidMccJitterBuffer ||\
  aEndpointUid == KImplUidRtpDataSource ||\
  aEndpointUid == KImplUidRtpDataSink ||\
  aEndpointUid == KImplUidMccVideoSource ||\
  aEndpointUid == KImplUidMccVideoSink ||\
  aEndpointUid == KImplUidMccAnySource ||\
  aEndpointUid == KImplUidMccAnySink )


#define MCC_IS_AVC_USER_ENTRY( a ) \
( a && a->iCodecInfo.iSdpName.CompareF( KAVCSdpName ) == 0 )

#define MCC_IS_H263_USER_ENTRY( a ) \
( a && ( a->iCodecInfo.iSdpName.CompareF( KH263SdpName ) == 0 || \
a->iCodecInfo.iSdpName.CompareF( KH2632000SdpName ) == 0 || \
a->iCodecInfo.iSdpName.CompareF( KH2631998SdpName ) == 0 ) )

#define MCC_IS_AVC_PRIMARY_CODEC \
( iUsers.Count() > 0 && MCC_IS_AVC_USER_ENTRY( iUsers[ 0 ] ) )

#define MCC_IS_H263_PRIMARY_CODEC \
( iUsers.Count() > 0 && MCC_IS_H263_USER_ENTRY( iUsers[ 0 ] ) )


// DATA TYPES
enum TMccMediaQualityCustomCmd
    {
    EStartMediaQualityObserving,
    ECancelMediaQualityObserving
    };

enum TSrCustomCommandMessages
    {
    ERequestEventNotification = 0,
    EMccClose,
    EMccCreateSession,
    EMccSetRemoteAddress,
    EMccSetRemoteRtcpAddr,
    EMccCloseSession,
    EMccCreateLink,
    EMccCloseLink,
    EMccCreateStream,
    EMccPrepareStream,
    EMccInactivityTimerStart,
    EMccInactivityTimerStop,
    EMccDeleteStream,
    EMccStartStream,
    EMccStopStream,
    EMccPauseStream,
    EMccResumeStream,
    EMccGetCodec,
    EMccGetFmtpAttr,
    EMccSetCodec,
    EMccGetSupportedCodecs,
    EMccStreamSettings,
    EMccSetAudioPriority,
    EMccCancel,
    EMccSendMediaSignal,
    EMccSendRtcpRR,
    EMccSendRtcpSR,
    EMccSendRtcpData,
    EMccGetSupportedBitrates,
    EMccGetSSRC,
    EMccGetSinkId,
    EMccGetSourceId,
    EMccEnableViewFinder,
    EMccDisableViewFinder,
    EMccGetViewFinderSettings,
    EMccGetCamInfo,
    EMccGetNumOfCams,
    EMccSetCamValue1,
    EMccGetCamValue1,
    EMccSetCamValue2,
    EMccGetCamValue2,
    EMccLinkExists,
    EMccStreamsExists,
    EMccUpdateDataSource,
    EMccUpdateDataSink,
    EMccReuse,
    EMccSetAudioRoute,
    EMccGetAudioRoute,
    EMccSetEndpointParameter,
    EMccGetEndpointParameter,
    EMccGetReferenceCount,
    EMccTranscodeFile,
    EMccCancelTranscodeFile,
    EMccBindContextIntoStream,
    EMccRemoveContext
    };
    

/*
* Message class for redundancy configuration
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/    
class TMccRedundancy
    {
    public:
        TMccRedundancy() :
            iSessionID( 0 ),
            iLinkID( 0 ),
            iStreamID( 0 ),
            iStatus( EFalse ),
            iRedundancyPT( 0 ),
            iRedCount( 0 )
            {}

        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        TBool iStatus;
        TUint8 iRedundancyPT;
        TInt iRedCount;
    };

typedef TPckgBuf<TMccRedundancy> TMccRedundancyPckg;

/*
* Message class for creating Mcc sessions
* @since Series 60 3.0
* @lib MMccController.dll
*/
class TMccCreateSession
    {
    public:
        TMccCreateSession() :
            iSessionID( 0 )
            {}

        TUint32 iSessionID;
    };

typedef TPckgBuf<TMccCreateSession> TMccCreateSessionPckg;

/*
* Message class for creating Mcc links
* @since Series 60 3.0
* @lib MMccController.dll
*/
class TMccCreateLink
    {
    public:
        TMccCreateLink() :
            iSessionID( 0 ),
            iLinkType( KMccLinkGeneral ),
            iLinkID( 0 ),
            iRemoteAddress( KInetAddrAny ),
            iLocalAddress( KInetAddrAny ),
    	    iLocalRtcpAddress( KInetAddrAny ),
            iIapId( 0 ),
            iIpTOS( 0 ),
            iMediaSignaling( 0 )
            {}

        TUint32 iSessionID;
        TInt iLinkType;
        TUint32 iLinkID;
        TInetAddr iRemoteAddress;
        TInetAddr iLocalAddress;
        TInetAddr iLocalRtcpAddress;
        TInt iIapId;
        TInt iIpTOS;
        TInt iMediaSignaling;
    };

typedef TPckgBuf<TMccCreateLink> TMccCreateLinkPckg;
    
/*
* Message class for starting Mcc inactivity timer
* @since Series 60 3.0
* @lib MMccController.dll
*/
class TMccInactivityTimer
    {
    public:
        TMccInactivityTimer():
            iSessionID( 0 ),
            iLinkID( 0 ),
            iStreamID( 0 ),
            iTimeoutTime( 0 )
            {}
        
        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        TUint32 iTimeoutTime;
    };

typedef TPckgBuf<TMccInactivityTimer> TMccInactivityTimerPckg;

/*
* Message class for session messages
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccSession
    {
    public:
        TMccSession() :
            iSessionID( 0 ),
            iLinkID( 0 ),
            iStreamID( 0 ),
            iEndpointID( 0 ),
            iStreamType( KMccAudioUplinkStream ),
            iCodec(),
            iFourCCArray( TFixedArray<TFourCC, KMccFourCCArraySize>() ),
            iPort( 0 ),
            iPayloadType( 0 ),
            iEnableVAD( NULL ),
            iPacketizationRate( 1 ),
            iPTime( 20 ),
            iMaxPTime( 200 )
            {}

        
        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        TUint32 iEndpointID;
        TInt iStreamType;
        TFourCC iCodec;
        TFixedArray<TFourCC, KMccFourCCArraySize> iFourCCArray;
        TUint iPort;
        TUint8 iPayloadType;
        TBool iEnableVAD;
        TInt iPacketizationRate;
        TUint iPTime;
        TUint iMaxPTime;
    };

typedef TPckgBuf<TMccSession> TMccSessionPckg;

/*
* Message class for stream messages
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccStream
    {
    public:
        TMccStream() : 
            iSessionID( 0 ),
            iLinkID( 0 ),    
            iStreamID( 0 ),
            iEndpointID( 0 ),
            iStreamType( KMccAudioUplinkStream ),
            iCodec(),
            iBitrate( 0 ),
            iPriority( 0 ),
            iPayloadType( 0 ),
            iEnableVAD( NULL ),
            iPacketizationRate( 1 ),
            iPTime( 20 ),
            iMaxPTime( 200 ),
            iEnableRTCP( EFalse ),
            iStreamPaused( EFalse )
            {}

        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        TUint32 iEndpointID;
        TInt iStreamType;
        TFourCC iCodec;
        TInt iBitrate;
        TInt iPriority;
        TUint8 iPayloadType;
        TBool iEnableVAD;
        TInt iPacketizationRate;
        TUint iPTime;
        TUint iMaxPTime;
        TBool iEnableRTCP;
        TBool iStreamPaused;
    };

typedef TPckgBuf<TMccStream> TMccStreamPckg;

/*
* Message class for stream creation parameters
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccCreateStreamParam
    {
    public:
        TMccCreateStreamParam() : 
            iPayloadType( 0 ),
            iFourCC( 0 ),
            iStreamType( KMccAudioUplinkStream ),
            iPrioritySettings(),
            iStandByTimer( 0 )
            {}
        
        TUint8 iPayloadType;
        TFourCC iFourCC;
        TInt iStreamType;
        TMMFPrioritySettings iPrioritySettings;
        TUint iStandByTimer;
    };

typedef TPckgBuf<TMccCreateStreamParam> TMccCreateStreamParamPckg;
   
/*
* Message class for setting remote address
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccAddress
    {
    public:
        TMccAddress() :
            iSessionID( 0 ),
            iLinkID( 0 ),
            iStreamID( 0 ),
            iAddress()
            {}

        TUint32 iSessionID;
        TUint32 iLinkID;
        TUint32 iStreamID;
        TInetAddr iAddress;
    };

typedef TPckgBuf<TMccAddress> TMccAddressPckg;


/*
* Message class for setting remote address
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccSSRC
    {
    public:
        TMccSSRC() :
            iSSRC( 0 )
            {}

        TUint32 iSSRC;
    };

typedef TPckgBuf<TMccSSRC> TMccSSRCPckg;


/*
* Message class for payload messages
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccPayload
    {
    public:
        TMccPayload() 
            :
            iSessionID( 0 ), 
            iStreamID( 0 ),
            iCurrentDlPacketCount( 0 ),
            iRtcpType( 0 ),
            iSSRC( 0 ),
            iApp(),
            iSdesParams(),
            iReason( NULL ),
            iEventType( 0 ),
            iError( 0 )
            {
            iTimeStamps.iNTPTimeStampSec = 0;
            iTimeStamps.iNTPTimeStampFrac = 0;
            iTimeStamps.iTimeStamp = 0;
            }

        TUint32 iSessionID;
        TUint32 iStreamID;
        TInt iCurrentDlPacketCount;   
        TInt iRtcpType;
        TRtpSSRC iSSRC;
        TRtcpApp iApp;
        TTimeStamps iTimeStamps;
        TRtpSdesParams iSdesParams;
        TBuf8<KMaxRtcpReason> iReason;
        TInt iEventType;
        TInt iError;
    };
    
typedef TPckgBuf<TMccPayload> TMccPayloadPckg;

/*
* Message class for bitrate messages
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccBitrates
    {
    public:
        TMccBitrates()
            {
            for ( TInt i = 0; i < KMaxBitrates; i++ )
                {
                iBitrates[i] = 0;
                }
            }
        TUint32 iBitrates[KMaxBitrates];
    };

typedef TPckgBuf<TMccBitrates> TMccBitratesPckg;

	
/**
* Class for generic messages
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccGenericMessage
    {
    public:
        inline TMccGenericMessage() :
            iSessionID( 0 ),
            iLinkID( 0 ),
            iStreamID( 0 ),
            iEndpointID( 0 ),
            iMessageFunction( 0 ),
            iParam1( KNullDesC8 ),
            iParam2( 0 ),
            iParam3( 0 ),
            iParam4( 0 )
        {   }
        
    public:
        // Session ID
        TUint32 iSessionID;
        
        // Link ID
        TUint32 iLinkID;

        // Stream ID
        TUint32 iStreamID;
        
        // Stream ID
        TUint32 iEndpointID;

        // Message function
        TInt iMessageFunction;
        
        TBuf8<256> iParam1;
        
        TUint32 iParam2;
        
        TReal iParam3;
        
        TInt iParam4;
        
    };
    
// Typedef so we can use it in IPC/ITC messages
typedef TPckgBuf<TMccGenericMessage> TMccGenericMsgBuffer;    

/**
* Class for security use
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/
class TMccSecurityDestination
    {
    public:
        inline TMccSecurityDestination() :
            iSessionID( 0 ),
            iLinkID( 0 ),
            iStreamID( 0 ),
            iEndpointID( 0 )
        {   }
        
    public:
        // Session ID
        TUint32 iSessionID;
        
        // Link ID
        TUint32 iLinkID;

        // Stream ID
        TUint32 iStreamID;
        
        // Stream ID
        TUint32 iEndpointID;
        
    };
    
typedef TPckgBuf<TMccSecurityDestination> TMccSecurityDestinationBuffer;

/*
* Mcc Event handling interface
* @since Series 60 3.0
* @lib mmcccontroller.dll
*/    
class MMccEventHandler
    {
public:

    /**
    * Pure virtual function SendMccEventToClient.
    * Send Mcc event to client
    * @since Series 60 3.0
    * @param aEvent - [input] Event to be handled
    * @return KErrNone if successful
    */
    virtual TInt SendMccEventToClient( TMccEvent& aEvent ) = 0;
    
    /**
    * Event handling interface
    * @since Series 60 3.0
    * @param [input] aState The new state
    * @param [input] aLink The link in which the state change occurred
    * @return void
    */
    virtual void StateChange( TInt aState, TUint32 aLink ) = 0;

    /**
    * Error passing to the client
    * @since Series 60 3.0
    * @param [input] aError The error that occured
    * @param [input] aLink The link in which the error occurred
    * @return void
    */
    virtual void ErrorOccured( TInt aError, 
                               TUint32 aSessionId,
                               TUint32 aLinkId,
    	                       TUint32 aStreamId, 
    	                       TUint32 aEndpointId ) = 0;
    };

/*
* Class encapsulating key-value pairs.
* @since    Series 60 3.2
* @lib      mmcccontroller.dll
*/  
template<class K, class V >
class TMccKeyValuePair
    {
public:
    
    TMccKeyValuePair( K& aKey, V& aValue )
        :
        iKey( aKey ), iValue( aValue ) { };
    
    K Key() const { return iKey; };
    
    V Value() const { return iValue; };
    
    static TBool CompareKey( const K* aKey, const TMccKeyValuePair& aPair )
        {
        return ( aPair.iKey == *aKey );
        }
    
    static TBool CompareValue( const V* aValue, const TMccKeyValuePair& aPair )
        {
        return ( aPair.iValue == *aValue );
        }
    
public:
    
    K iKey;
    V iValue;
    };
    

/**
* Methods for accessing elements in "user" array
*/    
template<class UserEntry>
class MccUserArray
    {
    public:  
      
    static UserEntry* FindUserEntryForCurrent( 
        RArray<UserEntry>& aUsers, MAsyncEventHandler* aEventHandler )
        {
        UserEntry entry( aEventHandler );
        TIdentityRelation<UserEntry> comparison( UserMatch );
        TInt index = aUsers.Find( entry, comparison );
        if ( index != KErrNotFound )
            {
            return &aUsers[ index ];
            }
        return NULL;
        }
        
    static void RemoveCurrentUser( 
        RArray<UserEntry>& aUsers, MAsyncEventHandler* aEventHandler )
        {
        UserEntry entry( aEventHandler );
        TIdentityRelation<UserEntry> comparison( UserMatch );
        TInt index = aUsers.Find( entry, comparison );
        if ( index != KErrNotFound )
            {
            aUsers.Remove( index );
            }
        }
          
    static TBool UserMatch( 
        const UserEntry& aUser1, 
        const UserEntry& aUser2 )
        {
        // First argument is always the search term
        
        TBool match( EFalse );
        if ( aUser1.iEventHandler )
            {
            match = ( aUser1.iEventHandler == aUser2.iEventHandler );
            }
        return match;
        }
        
     static UserEntry* FindUserEntryForCurrent( 
        RPointerArray<UserEntry>& aUsers, MAsyncEventHandler* aEventHandler )
        {
        UserEntry entry( aEventHandler );
        TIdentityRelation<UserEntry> comparison( UserMatch );
        TInt index = aUsers.Find( &entry, comparison );
        if ( index != KErrNotFound )
            {
            return aUsers[ index ];
            }
        return NULL;
        }
    
    static TInt FindUserEntryIndexForCurrent( 
        RPointerArray<UserEntry>& aUsers, MAsyncEventHandler* aEventHandler )
        {
        UserEntry entry( aEventHandler );
        TIdentityRelation<UserEntry> comparison( UserMatch );
        return aUsers.Find( &entry, comparison );
        }
        
    static void RemoveCurrentUser( 
        RPointerArray<UserEntry>& aUsers, MAsyncEventHandler* aEventHandler )
        {
        UserEntry entry( aEventHandler );
        TIdentityRelation<UserEntry> comparison( UserMatch );
        TInt index = aUsers.Find( &entry, comparison );
        if ( index != KErrNotFound )
            {
            delete aUsers[ index ];
            aUsers.Remove( index );
            }
        }
    };

class MccConversionUtility
    {
public:
    
    static HBufC8* Base64EncodeLC( const TDesC8& aInput )
        {
        const TUint KMccFileB64MaxSizeMod = 2;
        HBufC8* output = HBufC8::NewLC( aInput.Length() * KMccFileB64MaxSizeMod );
        TPtr8 ptrOutput( output->Des() );
        TBase64 encoder;
    	User::LeaveIfError( encoder.Encode( aInput, ptrOutput ) );
    	return output;
        }
    
    static void FrameRateSanitizeL( 
        TReal& aSanitizedFrameRate, TReal aOrigFrameRate, TUint32 aVideoType )
        {
        // Round to have zero decimals for clearness sake
        User::LeaveIfError( Math::Round( aSanitizedFrameRate, aOrigFrameRate, 0 ) );
        
        if ( aVideoType == MP4_TYPE_H263_PROFILE_0 && 
             aSanitizedFrameRate > KMccH263ProfileZeroMaxFramerateIOP )
            {
            aSanitizedFrameRate = KMccH263ProfileZeroMaxFramerateIOP;
            }
        }
    
    static void BitRateSanitize( 
        TUint32& aSanitizedBitRate, TUint32 aOrigBitRate, TUint32 aVideoType )
        {
        if ( aVideoType == MP4_TYPE_H263_PROFILE_0 && 
             aOrigBitRate > 0 && 
             aOrigBitRate < KMccH263ProfileZeroMinBitRateIOP )
            {
            aSanitizedBitRate = KMccH263ProfileZeroMinBitRateIOP;
            }
        else 
            {
            aSanitizedBitRate = aOrigBitRate;
            }
        }
    
    };
        
#endif
