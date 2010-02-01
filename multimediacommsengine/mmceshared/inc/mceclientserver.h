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






#ifndef MCECLIENTSERVER_H
#define MCECLIENTSERVER_H

#include <e32std.h>
#include "mcedefs.h"

_LIT( KMceServerName,"mceserver" );
_LIT( KMceServerFilename, "mceserver" ); 
_LIT( KMceServerSemaphoreName, "MceServerSemaphore" );
_LIT( KMceServerPanicString, "MceServer" );

const TUid KServerUid3 = {0x1020E4C7};

#if (defined (__WINS__) || defined(__WINSCW__))
static const TUint KServerMinHeapSize =  0x1000;  //   4K
static const TUint KServerMaxHeapSize = 0x80000;  // 512K 
#endif

class TMceCameraInfo;
class TMceFileInfo;

// The server version.
// A version must be specifyed when creating a session with the server
const TUint KMceServerMajorVersionNumber=0;
const TUint KMceServerMinorVersionNumber=1;
const TUint KMceServerBuildVersionNumber=1;

const TUint32 KMceNotAssigned = (TUint32)KMaxTUint32;
const TInt32 KMceNotAssignedInt = KMaxTInt32;
const TUint KMceSecToMicrosec = 1000000;
const TUint KMceSecToMillisec = 1000;

//
// callbacks
enum TMceItcCallBack
    {
    EMceItcNotAssigned = KMaxTUint16,
    EMceItcStateChanged = 1,
    EMceItcObjectAdded = 2,
    EMceItcSessionConnectionStateChanged = 3,
    EMceItcSRReceived = 4,
    EMceItcRRReceived = 5,
    EMceItcInactivityTimeout = 6,
    EMceItcFCMsgReceived = 7,
    EMceItcFCErrorReceived = 8,
    EMceItcNotifyReceived = 9,
    EMceItcIncomingUpdate = 10,
    EMceItcUpdateFailed = 11,
    EMceItcActivity = 12,
    EMceItcInProgress = 13,
    EMceItcTranscodingInProgress = 14,
    EMceItcTranscodingCompleted = 15,
    EMceItcDtmfSendStarted = 16,
    EMceItcDtmfSendCompleted = 17,
    EMceItcDtmfSendError = 18,
    EMceItcLinkCreated = 19, 
    EMceItcResponseReceived = 20,
    EMceItcRequestReceived = 21
    };


//data types
enum TMceItcDataType
	{
	//basics
	EMceItcMsgTypeWriteString,
	EMceItcMsgTypeReadString,
	EMceItcMsgTypeTextArray,
	//structs
	EMceItcMsgTypeSIPData,
	EMceItcMsgTypeSIPReply,
	EMceItcMsgTypeMessageArray,
	EMceItcMsgTypeEvent,
	EMceItcMsgTypeSIPRequest,
	//objects
	EMceItcMsgTypeSession,
	EMceItcMsgTypeStream,
	EMceItcMsgTypeSink,
	EMceItcMsgTypeSource,
	EMceItcMsgTypeCodec,
	EMceItcMsgTypeFC
	};

enum TMceItcEventType
    {
    EMceItcEventTypeEither = -1,
    EMceItcEventTypeSubscribe = 1,
    EMceItcEventTypeRefer = 2
    };

typedef TPckgBuf<TInt> TMceItcArgTInt;
typedef TPckgBuf<TUint32> TMceItcArgTUint32;
typedef TPckgBuf<TMceCameraInfo> TMceItcArgTCameraInfo;
typedef TPckgBuf<TRect> TMceItcArgTRect;
typedef TPckgBuf<TTimeIntervalMicroSeconds> TMceItcArgTime;
typedef TPckgBuf<TMceFileInfo> TMceItcArgTFileInfo;

typedef TUint32 TMceManagerType;
typedef TUint32 TMceCsSessionType;
typedef TUint32 TMceDialogType;

const TMceCsSessionType KMceCSSIPSession = 1;
const TMceCsSessionType KMceCSSIPEvent = 2;

const TMceDialogType KMceDlgTypeInvite = 1;
const TMceDialogType KMceDlgTypeRefer = 2;
const TMceDialogType KMceDlgTypeSubscribe = 3;

/*
 * media id
 *
 */
class TMceMediaId
    {
public:

	inline TMceMediaId(): iAppId( KMceNotAssigned ), iId( KMceNotAssigned ) 
	    {
	    }

	inline TMceMediaId( TUint32 aId ): iAppId( 0 ), iId( aId )
	    {
	    }
	    
	inline TMceMediaId( TUid aAppUid, TUint32 aId ): iAppId( aAppUid.iUid ), iId( aId )
	    {
	    }
	    
    inline TBool operator==( const TMceMediaId aId ) const
        {
        return ( iAppId == aId.iAppId &&
                 iId == aId.iId );
        
        }
        
    inline TBool operator!=( const TMceMediaId aId ) const
        {
        return !( iAppId == aId.iAppId &&
                 iId == aId.iId );
        }


    inline TBool IsAssigned() const
        {
        return ( iAppId != KMceNotAssigned &&
                 iId != KMceNotAssigned );
        }
        
    inline TUint64 Uint64() const
        {
        return MAKE_TUINT64( iAppId, iId );
        }
        
    
public:

    TUint32 iAppId;
    TUint32 iId;
    
    };

#define KMceMediaIdNotAssigned TMceMediaId()

const TInt KMceArrayIndexOriginator = 0;
const TInt KMceArrayIndexRecipient = 1;
const TInt KMceArrayIndexIAPId = 2;

const TInt KMceArrayIndexEventHeader = 0;
const TInt KMceArrayIndexEventReferTo = 0;
const TInt KMceArrayIndexEventRemoteURI = 1;
const TInt KMceArrayIndexEventOriginator = 2;

const TInt KMceIDsMaxIndex = 12;
const TInt KMceIDsLastReservedIndex = 9;

#define MCEIDS_DEFINE_SPARE_INDEX_1( name )\
    const TInt name = KMceIDsLastReservedIndex + 1

#define MCEIDS_DEFINE_SPARE_INDEX_2( name )\
    const TInt name = KMceIDsLastReservedIndex + 2

#define MCEIDS_DEFINE_SPARE_INDEX_3( name )\
    const TInt name = KMceIDsLastReservedIndex + 3

//#define MCEIDS_DEFINE_SPARE_INDEX_4( name )\
//    const TInt name = KMceIDsLastReservedIndex + 4
    

MCEIDS_DEFINE_SPARE_INDEX_1( KMceIdsIndexFCPort );
MCEIDS_DEFINE_SPARE_INDEX_1( KMceIdsIndexTCId );

#define MCE_ENCODE_DOUBLE_STATE( state1, state2 ) \
( ( (TUint8)state1 ) << 8 ) | (TUint8)state2 & 0x0000FFFF

#define MCE_DECODE_DOUBLE_STATE( doubleState, state1, state2 ) \
state2 = doubleState & 0x000000FF;\
state1 = ( ( doubleState & 0x0000FFFF ) >> 8 ) & 0x000000FF

#define MCE_IDS_ITC_CONTEXT( ids ) \
    reinterpret_cast<const RMessage2*>( ids.iItcContext )

// A container for MCE IDs

class TMceIds
	{
public:

	inline TMceIds()
		: iAppUID(KMceNotAssigned),
		  iManagerType( KMceNotAssigned ),
		  iSessionID(KMceNotAssigned),
		  iDialogID(KMceNotAssigned),
		  iDialogType(KMceNotAssigned),
		  iProfileID(KMceNotAssigned),
		  iCallbackID( KMceNotAssigned ),
		  iMsgType(KMceNotAssigned),
		  iState(KMceNotAssigned),
		  iTransactionID(KMceNotAssigned),
          iSpare1(KMceNotAssigned),
          iSpare2(KMceNotAssigned),
          iSpare3(KMceNotAssigned),
          iSecureFlag(EFalse),
          iStatus( KErrNone ),
          iMediaID(KMceMediaIdNotAssigned),
		  iSourceID(KMceMediaIdNotAssigned),
		  iSinkID(KMceMediaIdNotAssigned),
		  iCodecID(KMceMediaIdNotAssigned),
		  iItcContext( NULL ) {}

/*	inline TMceIds(const TMceIds& aIds)
		: iAppUID(aIds.iAppUID),
		  iManagerType( aIds.iManagerType ),
		  iSessionID(aIds.iSessionID),
		  iDialogID(aIds.iDialogID),
		  iDialogType(aIds.iDialogType),
		  iProfileID( aIds.iProfileID),
		  iCallbackID(aIds.iCallbackID),
		  iMsgType(aIds.iMsgType),
		  iState(aIds.iState),
		  iSpare1(aIds.iSpare1),
		  iSpare2(aIds.iSpare2),
		  iSpare3(aIds.iSpare3),
		  iSecureFlag(aIds.iSecureFlag),
		  iStatus(aIds.iStatus),
          iMediaID(aIds.iMediaID),
		  iSourceID(aIds.iSourceID),
		  iSinkID(aIds.iSinkID),
		  iCodecID(aIds.iCodecID),
		  iItcContext( aIds.iItcContext )
		  {
		  Mem::Copy(this, &aIds, sizeof(TMceIds));
		  }
*/
 
    inline TBool IsAppUIDId() const
        {
        return (
            iAppUID != KMceNotAssigned &&
            iSessionID == KMceNotAssigned );
        }

    inline TBool IsSessionId() const
        {
        return (
            iSessionID != KMceNotAssigned &&
            iMediaID == KMceMediaIdNotAssigned );
        }

    inline TBool IsMediaId() const
        {
        return (
            iMediaID != KMceMediaIdNotAssigned &&
            ( iSourceID == KMceMediaIdNotAssigned && 
              iSinkID == KMceMediaIdNotAssigned && 
              iCodecID == KMceMediaIdNotAssigned ) );
        }

    inline TBool IsSourceId() const
        {
        return (
            iSourceID != KMceMediaIdNotAssigned &&
            iSinkID == KMceMediaIdNotAssigned && 
            iCodecID == KMceMediaIdNotAssigned );
        }

    inline TBool IsSinkId() const
        {
        return (
            iSinkID != KMceMediaIdNotAssigned &&
            iSourceID == KMceMediaIdNotAssigned &&
            iCodecID == KMceMediaIdNotAssigned );
        }

    inline TBool IsCodecId() const
        {
        return (
            iCodecID != KMceMediaIdNotAssigned &&
            iSourceID == KMceMediaIdNotAssigned &&
            iSinkID == KMceMediaIdNotAssigned );
        }

    inline void Set( TInt aIndex, TUint32 aValue )
        {
        __ASSERT_ALWAYS( aIndex <= KMceIDsMaxIndex, 
                         User::Panic( KMceServerPanicString, KErrArgument ) );
        TUint32* id = (TUint32*)this;
        id = id + aIndex;
        *id = aValue;
        }

    inline TUint32 Get( TInt aIndex )
        {
        __ASSERT_ALWAYS( aIndex <= KMceIDsMaxIndex, 
                         User::Panic( KMceServerPanicString, KErrArgument ) );
        TUint32* id = (TUint32*)this;
        id = id + aIndex;
        return *id;
        }
        

    inline void Copy( const TMceIds& aIds )
        {
        iAppUID = aIds.iAppUID != KMceNotAssigned ? aIds.iAppUID : iAppUID;
        iManagerType = aIds.iManagerType != KMceNotAssigned ? aIds.iManagerType : iManagerType;
        iSessionID = aIds.iSessionID != KMceNotAssigned ? aIds.iSessionID : iSessionID;
        iDialogID = aIds.iDialogID != KMceNotAssigned ? aIds.iDialogID : iDialogID;
        iDialogType = aIds.iDialogType != KMceNotAssigned ? aIds.iDialogType : iDialogType;
        iProfileID = aIds.iProfileID != KMceNotAssigned ? aIds.iProfileID : iProfileID;
        iCallbackID = aIds.iCallbackID;
        iMsgType = aIds.iMsgType;
        iState = aIds.iState;
        iTransactionID = aIds.iTransactionID; 
        iSpare1 = aIds.iSpare1;
        iSpare2 = aIds.iSpare2;
        iSpare3 = aIds.iSpare3;
        iSecureFlag= aIds.iSecureFlag;
        iStatus = aIds.iStatus;
        iMediaID = aIds.iMediaID;
        iSourceID = aIds.iSourceID;
        iSinkID = aIds.iSinkID;
        iCodecID = aIds.iCodecID;
        }
        
public://data

    TUint32     iAppUID;     //0
    TUint32     iManagerType;//1
    TUint32     iSessionID;  //2
    TUint32     iDialogID;   //3
    TUint32     iDialogType; //4
    TUint32     iProfileID;  //5
	TUint32     iCallbackID; //6
    TUint32     iMsgType;    //7
    TUint32     iState;      //8
    TMceTransactionId iTransactionID; //9   KMceIDsLastReservedIndex
    TUint32     iSpare1;              //10  first spare index
    TUint32     iSpare2;              //11
    TUint32     iSpare3;              //12  KMceIDsMaxIndex
    TBool     	iSecureFlag;          //13
    TInt        iStatus;
	TMceMediaId iMediaID;
	TMceMediaId iSourceID;
	TMceMediaId iSinkID;
	TMceMediaId iCodecID;	
	const TAny*	iItcContext;
	};


// A container for incoming buffer sizes
class TMceMessageBufSizes
    {
public:

    inline TMceMessageBufSizes()
        : iContextBufSize( KErrNotFound ),
          iContentBufSize( KErrNotFound ) {}
            

    inline TMceMessageBufSizes( TInt aContextBufSize, TInt aContentBufSize )
        : iContextBufSize( aContextBufSize ),
          iContentBufSize( aContentBufSize ) {}

	inline TMceMessageBufSizes(const TMceMessageBufSizes& aSizes)
        : iContextBufSize(aSizes.iContextBufSize),
          iContentBufSize(aSizes.iContentBufSize) {}

    TInt iContextBufSize;
    TInt iContentBufSize;
    };
    
    
	
// Oper. codes used in passing IPC messages between C/S-client and C/S-server
// To these IPC messages are refered in MCE server security policy file 
// mcecsplatsecpolicy.h
enum TMceItcFunctions
	{
	EMceItcSetAppUid,               //0 (AlwaysPass)
	EMceItcCreateSession,           //1 (NetworkServices)
	EMceItcDeleteSession,           //2 (NetworkServices)
	EMceItcSetDefaultHeaders,       //3 (NetworkServices)
	EMceItcGetDefaultHeaders,       //4 (NetworkServices)
	EMceItcEstablishSession,        //5 (NetworkServices)
	EMceItcTerminateSession,        //6 (NetworkServices)
	EMceItcAcceptSession,           //7 (NetworkServices)
	EMceItcRejectSession,           //8 (NetworkServices)
	EMceItcUpdate,                  //9 (NetworkServices)
	EMceItcRing,                    //10 (NetworkServices)
	EMceItcEnable,                  //11 (NetworkServices)
	EMceItcDisable,                 //12 (NetworkServices)
	EMceItcSendRR,                  //13 (NetworkServices)
	EMceItcSendSR,                  //14 (NetworkServices)
	EMceItcReceive,                 //15 (AlwaysPass)
	EMceItcClientReadyToReceive,    //16 (AlwaysPass)
	EMceItcCancel,                  //17 (AlwaysPass)
	EMceItcClientCancelReceive,     //18 (AlwaysPass)
	EMceItcInitFC,                  //19 (AlwaysPass)
	EMceItcUpdateFC,                //20 (AlwaysPass)
	EMceItcSendFCMsg,               //21 (NetworkServices)
	EMceItcFCGetMediaLines,         //22 (AlwaysPass)
	EMceItcFCSetMediaLines,         //23 (AlwaysPass)
	EMceItcGetSupportedAudioCodecs, //24 (AlwaysPass)
	EMceItcSubscribe,               //25 (NetworkServices)
	EMceItcSubscribeUpdate,         //26 (NetworkServices)
	EMceItcAcceptEvent,             //27 (NetworkServices)
	EMceItcRejectEvent,             //28 (NetworkServices)
	EMceItcRespondEvent,            //29 (NetworkServices)
	EMceItcTerminateEvent,          //30 (NetworkServices)
	EMceItcNotify,                  //31 (NetworkServices)
	EMceItcReferEvent,              //32 (NetworkServices)
	EMceItcSetVolume,               //33 (WriteDeviceData)
	EMceItcVolume,                  //34 (AlwaysPass)
	EMceItcMaxVolume,               //35 (AlwaysPass) 
	EMceItcSetGain,                 //36 (WriteDeviceData)
	EMceItcGain,                    //37 (AlwaysPass)
	EMceItcMaxGain,                 //38 (AlwaysPass)
	EMceItcEnableInactivityTimer,   //39 (AlwaysPass)
    EMceItcDisableInactivityTimer,  //40 (AlwaysPass)
    EMceItcGetSession,              //41 (AlwaysPass)
    EMceItcCameraInfo,              //42 (MultimediaDD)
    EMceItcCamerasAvailable,        //43 (MultimediaDD)
    EMceItcZoomFactor,              //44 (MultimediaDD)
    EMceItcDigitalZoomFactor,       //45 (MultimediaDD)
    EMceItcContrast,                //46 (MultimediaDD)
    EMceItcBrightness,              //47 (MultimediaDD)
    EMceItcExposure,                //48 (MultimediaDD)
    EMceItcWhiteBalance,            //49 (MultimediaDD)
    EMceItcDisplaysAvailable,       //50 (AlwaysPass)
    EMceItcDisplayRect,             //51 (AlwaysPass)
    EMceItcDisplayPriority,         //52 (AlwaysPass)
    EMceItcFileInfo,                //53 (AlwaysPass)
    EMceItcFilePosition,            //54 (AlwaysPass)
    EMceItcSetCameraIndex,          //55 (AlwaysPass)
    EMceItcSetZoomFactor,           //56 (AlwaysPass)
    EMceItcSetDigitalZoomFactor,    //57 (AlwaysPass)
    EMceItcSetContrast,             //58 (AlwaysPass)
    EMceItcSetBrightness,           //59 (AlwaysPass)
    EMceItcSetExposure,             //60 (AlwaysPass)
    EMceItcSetWhiteBalance,         //61 (AlwaysPass)
    EMceItcSetDisplayIndex,         //62 (AlwaysPass)
    EMceItcSetDisplayRect,          //63 (AlwaysPass)
    EMceItcSetDisplayPriority,      //64 (AlwaysPass)
    EMceItcSetAudioRouting,         //65 (AlwaysPass)
    EMceItcSetFilePosition,         //66 (AlwaysPass)
    EMceItcSetFastForward,          //67 (AlwaysPass)
    EMceItcSetFastRewind,           //68 (AlwaysPass)
    EMceItcGetSupportedVideoCodecs, //69 (AlwaysPass)
    EMceItcAudioRouting,            //70 (AlwaysPass)
    EMceItcSetFileInfo,             //71 (AlwaysPass)
    EMceItcTranscode,               //72 (AlwaysPass)
    EMceItcCancelTranscode,         //73 (AlwaysPass)
    EMceItcDisplayRotation,         //74 (AlwaysPass)
    EMceItcSetDisplayRotation,      //75 (AlwaysPass)
    EMceItcCallInQueue,             //76 (AlwaysPass)
    EMceItcIsDtmfActive,            //77 (AlwaysPass)
    EMceItcStartDtmfTone,           //78 (AlwaysPass)
    EMceItcStopDtmfTone,            //79 (AlwaysPass)
    EMceItcSendDtmfTone,            //80 (AlwaysPass)
    EMceItcSendDtmfToneSequence,    //81 (AlwaysPass)
    EMceItcCancelSendDtmfToneSequence,   //82 (AlwaysPass)
    EMceItcRequestSend,							// 83 (AlwaysPass) 
    EMceItcReplySend,								// 84 (AlwaysPass) 
    EMceItcTranscodeWithSession                // 85 (AlwaysPass) 
    };	


// Indexes used in ITC parameter array passed between C/S-client and C/S-server
//
// All the parameters except TMceItcArguments are packed into package buffers
// to enable ITC parameter in-out behaviour.
//
// NOTE: Remember to update the types in comments if the actual types change.
//
enum TMceItcArguments
	{
    EMceItcArgIds = 0,               // Type: TMceIds
    EMceItcArgContext = 1,
    EMceItcArgAppUid = 1,            // Type: TInt
    EMceItcArgId = 1,                // Type: TUint32
    EMceItcArgBufSizes = 1,          // Type: TMceMessageBufSizes
    EMceItcArgMessageContent = 2,    // Type: TDes8/TDesC8
	};
	

#define MCE_ENDPOINT_ITC_ALLOWED( endpoint )\
    ( (endpoint).Id().IsAssigned() &&\
      (endpoint).iStream &&\
      (endpoint).iStream->Session() &&\
      (endpoint).iStream->Session()->State() != CMceSession::EIdle &&\
      (endpoint).iStream->Session()->State() != CMceSession::EIncoming )


#endif // MCECLIENTSERVER_H

// End of File
