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






#ifndef MCECOMLOGS_H
#define MCECOMLOGS_H

#include "mcelogs.h"

// INCLUDES

// FORWARD DECLARATIONS

// CLASS DECLARATION



#define MCE_DEBUG_CALLBACK( c, str, callback )\
    switch( callback )\
        {\
        case EMceItcStateChanged:{ c::Print( _L( str ), _L( "EMceItcStateChanged" ) );break; }\
        case EMceItcObjectAdded:{ c::Print( _L( str ), _L( "EMceItcObjectAdded" ) );break; }\
        case EMceItcSessionConnectionStateChanged:{ c::Print( _L( str ), _L( "EMceItcSessionConnectionStateChanged" ) );break; }\
        case EMceItcSRReceived:{ c::Print( _L( str ), _L( "EMceItcSRReceived" ) );break; }\
        case EMceItcRRReceived:{ c::Print( _L( str ), _L( "EMceItcRRReceived" ) );break; }\
        case EMceItcInactivityTimeout:{ c::Print( _L( str ), _L( "EMceItcInactivityTimeout" ) );break; }\
        case EMceItcFCMsgReceived:{ c::Print( _L( str ), _L( "EMceItcFCMsgReceived" ) );break; }\
        case EMceItcNotifyReceived:{ c::Print( _L( str ), _L( "EMceItcNotifyReceived" ) );break; }\
        case EMceItcIncomingUpdate:{ c::Print( _L( str ), _L( "EMceItcIncomingUpdate" ) );break; }\
        case EMceItcActivity:{ c::Print( _L( str ), _L( "EMceItcActivity" ) );break; }\
        case EMceItcInProgress:{ c::Print( _L( str ), _L( "EMceItcInProgress" ) );break; }\
        case EMceItcTranscodingInProgress:{ c::Print( _L( str ), _L( "EMceItcTranscodingInProgress" ) );break; }\
        case EMceItcTranscodingCompleted:{ c::Print( _L( str ), _L( "EMceItcTranscodingCompleted" ) );break; }\
        default:{ c::Print( _L( str ), callback );break; }\
        }

#define MCE_DEBUG_DATATYPE( c, str, datatype )\
    switch( datatype )\
        {\
        case EMceItcMsgTypeWriteString:{ c::Print( _L( str ), _L( "EMceItcMsgTypeWriteString" ) );break; }\
        case EMceItcMsgTypeReadString:{ c::Print( _L( str ), _L( "EMceItcMsgTypeReadString" ) );break; }\
        case EMceItcMsgTypeTextArray:{ c::Print( _L( str ), _L( "EMceItcMsgTypeTextArray" ) );break; }\
        case EMceItcMsgTypeSIPData:{ c::Print( _L( str ), _L( "EMceItcMsgTypeSIPData" ) );break; }\
        case EMceItcMsgTypeSIPReply:{ c::Print( _L( str ), _L( "EMceItcMsgTypeSIPReply" ) );break; }\
        case EMceItcMsgTypeMessageArray:{ c::Print( _L( str ), _L( "EMceItcMsgTypeMessageArray" ) );break; }\
        case EMceItcMsgTypeEvent:{ c::Print( _L( str ), _L( "EMceItcMsgTypeEvent" ) );break; }\
        case EMceItcMsgTypeSession:{ c::Print( _L( str ), _L( "EMceItcMsgTypeSession" ) );break; }\
        case EMceItcMsgTypeStream:{ c::Print( _L( str ), _L( "EMceItcMsgTypeStream" ) );break; }\
        case EMceItcMsgTypeSource:{ c::Print( _L( str ), _L( "EMceItcMsgTypeSource" ) );break; }\
        case EMceItcMsgTypeSink:{ c::Print( _L( str ), _L( "EMceItcMsgTypeSink" ) );break; }\
        case EMceItcMsgTypeCodec:{ c::Print( _L( str ), _L( "EMceItcMsgTypeCodec" ) );break; }\
        case EMceItcMsgTypeFC:{ c::Print( _L( str ), _L( "EMceItcMsgTypeFC" ) );break; }\
        default:{ c::Print( _L( str ), datatype );break; }\
        }
        
        
#define MCE_DEBUG_CLISTATE( c, str, state )\
    switch( state )\
        {\
        case CMceSession::EIdle:{ c::Print( _L( str ), _L( "EIdle" ) );break; }\
        case CMceSession::EOffering:{ c::Print( _L( str ), _L( "EOffering" ) );break; }\
        case CMceSession::EIncoming:{ c::Print( _L( str ), _L( "EIncoming" ) );break; }\
        case CMceSession::EReserving:{ c::Print( _L( str ), _L( "EReserving" ) );break; }\
        case CMceSession::EAnswering:{ c::Print( _L( str ), _L( "EAnswering" ) );break; }\
        case CMceSession::EProceeding:{ c::Print( _L( str ), _L( "EProceeding" ) );break; }\
        case CMceSession::EEstablished:{ c::Print( _L( str ), _L( "EEstablished" ) );break; }\
        case CMceSession::ECancelling:{ c::Print( _L( str ), _L( "ECancelling" ) );break; }\
        case CMceSession::ETerminating:{ c::Print( _L( str ), _L( "ETerminating" ) );break; }\
        case CMceSession::ETerminated:{ c::Print( _L( str ), _L( "ETerminated" ) );break; }\
        default:{ c::Print( _L( str ), state );break; }\
        }

#define MCE_DEBUG_EVENTSTATE( c, str, state )\
    switch( state )\
        {\
        case CMceRefer::EIdle:{ c::Print( _L( str ), _L( "EIdle" ) );break; }\
        case CMceRefer::EPending:{ c::Print( _L( str ), _L( "EPending" ) );break; }\
        case CMceRefer::EAccepted:{ c::Print( _L( str ), _L( "EActive" ) );break; }\
        case CMceRefer::ETerminated:{ c::Print( _L( str ), _L( "ETerminated" ) );break; }\
        default:{ c::Print( _L( str ), state );break; }\
        }
        

#define MCE_DEBUG_MEDIASTATE( c, str, state )\
    switch( state )\
        {\
        case CMceMediaStream::EUninitialized:{ c::Print( _L( str ), _L( "EUninitialized" ) );break; }\
        case CMceMediaStream::EInitialized:{ c::Print( _L( str ), _L( "EInitialized" ) );break; }\
        case CMceMediaStream::EBuffering:{ c::Print( _L( str ), _L( "EBuffering" ) );break; }\
        case CMceMediaStream::EIdle:{ c::Print( _L( str ), _L( "EIdle" ) );break; }\
        case CMceMediaStream::EStreaming:{ c::Print( _L( str ), _L( "EStreaming" ) );break; }\
        case CMceMediaStream::EDisabled:{ c::Print( _L( str ), _L( "EDisabled" ) );break; }\
        case CMceMediaStream::ENoResources:{ c::Print( _L( str ), _L( "ENoResources" ) );break; }\
        case CMceMediaStream::ETranscodingRequired:{ c::Print( _L( str ), _L( "ETranscodingRequired" ) );break; }\
        case CMceMediaStream::ETranscoding:{ c::Print( _L( str ), _L( "ETranscoding" ) );break; }\
        default:{ c::Print( _L( str ), state );break; }\
        }

        
#define MCE_DEBUG_ITC( c, str, event )\
	if( event >= EMceItcSetVolume && event <= EMceItcMaxGain ||\
	    event >= EMceItcCameraInfo && event <= EMceItcSetFastRewind ||\
        event >= EMceItcAudioRouting && event <= EMceItcSetFileInfo )\
	    {\
        c::Print( _L( str ), event, _L("(media attribute get/set)") );\
        }\
    if( event >= EMceItcInitFC && event <= EMceItcFCSetMediaLines )\
	    {\
        c::Print( _L( str ), event, _L("(FC)") );\
        }\
    else\
        {\
        switch( event )\
            {\
            case EMceItcSetAppUid:{ c::Print( _L( str ), _L( "EMceItcSetAppUid" ) );break; }\
            case EMceItcCreateSession:{ c::Print( _L( str ), _L( "EMceItcCreateSession" ) );break; }\
            case EMceItcDeleteSession:{ c::Print( _L( str ), _L( "EMceItcDeleteSession" ) );break; }\
            case EMceItcEstablishSession:{ c::Print( _L( str ), _L( "EMceItcEstablishSession" ) );break; }\
            case EMceItcTerminateSession:{ c::Print( _L( str ), _L( "EMceItcTerminateSession" ) );break; }\
            case EMceItcAcceptSession:{ c::Print( _L( str ), _L( "EMceItcAcceptSession" ) );break; }\
            case EMceItcRejectSession:{ c::Print( _L( str ), _L( "EMceItcRejectSession" ) );break; }\
            case EMceItcUpdate:{ c::Print( _L( str ), _L( "EMceItcUpdate" ) );break; }\
            case EMceItcRing:{ c::Print( _L( str ), _L( "EMceItcRing" ) );break; }\
            case EMceItcEnable:{ c::Print( _L( str ), _L( "EMceItcEnable" ) );break; }\
            case EMceItcSendRR:{ c::Print( _L( str ), _L( "EMceItcSendRR" ) );break; }\
            case EMceItcSendSR:{ c::Print( _L( str ), _L( "EMceItcSendSR" ) );break; }\
            case EMceItcDisable:{ c::Print( _L( str ), _L( "EMceItcDisable" ) );break; }\
            case EMceItcReceive:{ c::Print( _L( str ), _L( "EMceItcReceive" ) );break; }\
            case EMceItcClientReadyToReceive:{ c::Print( _L( str ), _L( "EMceItcClientReadyToReceive" ) );break; }\
            case EMceItcCancel:{ c::Print( _L( str ), _L( "EMceItcCancel" ) );break; }\
            case EMceItcClientCancelReceive:{ c::Print( _L( str ), _L( "EMceItcClientCancelReceive" ) );break; }\
            case EMceItcGetSupportedAudioCodecs:{ c::Print( _L( str ), _L( "EMceItcGetSupportedAudioCodecs" ) );break; }\
            case EMceItcGetSupportedVideoCodecs:{ c::Print( _L( str ), _L( "EMceItcGetSupportedVideoCodecs" ) );break; }\
            case EMceItcSubscribe:{ c::Print( _L( str ), _L( "EMceItcSubscribe" ) );break; }\
            case EMceItcSubscribeUpdate:{ c::Print( _L( str ), _L( "EMceItcSubscribeUpdate" ) );break; }\
            case EMceItcAcceptEvent:{ c::Print( _L( str ), _L( "EMceItcAcceptEvent" ) );break; }\
            case EMceItcRejectEvent:{ c::Print( _L( str ), _L( "EMceItcRejectEvent" ) );break; }\
            case EMceItcRespondEvent:{ c::Print( _L( str ), _L( "EMceItcRespondEvent" ) );break; }\
            case EMceItcTerminateEvent:{ c::Print( _L( str ), _L( "EMceItcTerminateEvent" ) );break; }\
            case EMceItcNotify:{ c::Print( _L( str ), _L( "EMceItcNotify" ) );break; }\
            case EMceItcReferEvent:{ c::Print( _L( str ), _L( "EMceItcReferEvent" ) );break; }\
            case EMceItcEnableInactivityTimer:{ c::Print( _L( str ), _L( "EMceItcEnableInactivityTimer" ) );break; }\
            case EMceItcDisableInactivityTimer:{ c::Print( _L( str ), _L( "EMceItcDisableInactivityTimer" ) );break; }\
            case EMceItcGetSession:{ c::Print( _L( str ), _L( "EMceItcGetSession" ) );break; }\
            case EMceItcTranscode:{ c::Print( _L( str ), _L( "EMceItcTranscode" ) );break; }\
            case EMceItcCancelTranscode:{ c::Print( _L( str ), _L( "EMceItcCancelTranscode" ) );break; }\
            default:{ c::Print( _L( str ), event );break; }\
            }\
        }
        
#define MCE_DEBUG_SESSIONTYPE( c, str, type )\
    switch( type )\
        {\
        case KMceCSSIPSession:{ c::Print( _L( str ), _L( "KMceCSSIPSession" ) );break; }\
        case KMceCSSIPEvent:{ c::Print( _L( str ), _L( "KMceCSSIPEvent" ) );break; }\
        default:{ c::Print( _L( str ), type );break; }\
        }

#define MCE_DEBUG_DIALOGTYPE( c, str, type )\
    switch( type )\
        {\
        case KMceDlgTypeInvite:{ c::Print( _L( str ), _L( "KMceDlgTypeInvite" ) );break; }\
        case KMceDlgTypeRefer:{ c::Print( _L( str ), _L( "KMceDlgTypeRefer" ) );break; }\
        case KMceDlgTypeSubscribe:{ c::Print( _L( str ), _L( "KMceDlgTypeSubscribe" ) );break; }\
        default:{ c::Print( _L( str ), type );break; }\
        }





#endif

// End of File
