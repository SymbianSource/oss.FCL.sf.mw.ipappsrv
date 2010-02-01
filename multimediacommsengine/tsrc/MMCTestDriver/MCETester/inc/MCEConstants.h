/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __MCECONSTANTS_H__
#define __MCECONSTANTS_H__

// INCLUDES
#include "TesterConstants.h"

// CONSTANTS

/// MCE Ids

_LIT8( KSessionId, "SessionId");
_LIT8( KEventId, "EventId");
_LIT8( KReferId, "ReferId");
_LIT8( KProfileObj, "ProfileObj" ); // Reference to TesterCores Registry-object
_LIT8( KRegistryId, "RegistryId" );
_LIT8( KStreamId, "StreamId" );
_LIT8( KCodecId, "CodecId" );
_LIT8( KSourceId, "SourceId" );
_LIT8( KSinkId, "SinkId" );

/// MCE Session states
_LIT8(KIdle, "Idle"); 				// Session is idle
_LIT8(KEstablishing, "Establishing"); // Outgoing session establishment in progress
_LIT8(KIncoming, "Incoming"); 		// Session received an incoming call
_LIT8(KAnswering, "Answering"); 	// Answering an incoming call
_LIT8(KProceeding, "Proceeding"); 	// Session media negotiations proceeding
_LIT8(KEstablished, "Established"); // Session established
_LIT8(KUpdating, "Reserving"); 		// when calling UpdateL to cause re-Invite 
								
_LIT8(KCancelling, "Cancelling");	// Cancelling outgoing session establishment
_LIT8(KTerminating, "Terminating");	// Terminating session
_LIT8(KTerminated, "Terminated");	// Session terminated
_LIT8(KOnHold, "OnHold");			// Session is on hold 
_LIT8(KReferred, "Referred");		// Session has been referred
_LIT8(KUnknownState, "Unknown session state");
_LIT8(KOffering, "Offering");
_LIT8(KPending, "Pending");
/// MCE Stream states
_LIT8( KStreamEnable, "Enable" );
_LIT8( KStreamDisable, "Disable" );

//MCE Mediastream states
_LIT8( KUninitialized, "Uninitialized" );
_LIT8( KInitialized, "Initialized" );
_LIT8( KBuffering, "Buffering" );
_LIT8( KmsIdle, "Idle" );
_LIT8( KStreaming, "Streaming" );
_LIT8( KDisabled, "Disabled" );
_LIT8( KNoResources, "NoResources" );
_LIT8( KTranscodingRequired, "TranscodingRequired" );
_LIT8( KTranscoding, "Transcoding" );
// OnHold defined in session states
_LIT8( KRemoving, "Removing" );


/// Additional state for MCE Events
_LIT8( KActive, "Active" );

/// Additional state for MCE Refers
_LIT8( KAccepted, "Accepted" );


/// Misc
///
const TInt KMceMessageAtIndex( 0 );
/// Default receive timeout
const TInt KDefaultReceiveTimeout(5);
/// Microseconds in one second
const TInt KSecondAsMicros( 1000000 ); 
/// SIP Header separator character
const TUint8 KHeaderSeparator( ':' );
/// Whitespace
_LIT8( KSpace, " ");
/// Horizontal tab
_LIT8( KTab, "\t" );
/// Floor control TBCP type
_LIT8( KFCTypeTBCP, "TBCP" );
/// MCE Servers processname
_LIT( KMceServerProcessName, "*MceServer*");

/// CTRL parameter names
_LIT8( KParamRTPInactivityTimer, "InactivityTimer" );
_LIT8( KParamPreferredEncodingDecodingDeviceUid, "EncodingDecoding");
_LIT8( KParamFrameRate, "FrameRate" );
_LIT8( KParamConfigKeys, "ConfigKeys" );
_LIT8( KParamAllowedFrameRates, "AllowedFrameRates");
_LIT8( KParamAllowedBitRate, "AllowedBitRate");
_LIT8( KParamAllowedResolutions, "AllowedResolutions");
_LIT8( KParamResolution, "Resolution");
_LIT8( KParamMaxBitRate, "MaxBitRate" );
_LIT8( KParamFCMessage, "FCMessage");
_LIT8( KParamFCMessageCount, "FCMessageCount");
_LIT8( KParamFCPort, "FloorControlPort");
_LIT8( KParamTimeout, "Timeout" );
_LIT8( KParamRecipient, "Recipient" );
_LIT8( KParamOriginator, "Originator" );
_LIT8( KArrayCodecsList, "CodecsOrderedList" );
_LIT8( KParamToHeader, "ToHeader" );
_LIT8( KParamDeleteSession, "DeleteSession");
_LIT8( KParamSessionUpdate, "SessionUpdate");
_LIT8( KParamSessionType, "SessionType" );
_LIT8( KParamSessionPriorityUplink, "SessionPriorityUplink");  //adamcand
_LIT8( KParamSessionPriorityDownlink, "SessionPriorityDownlink");  //adamcand
_LIT8( KParamAudioPrefUplink, "SessionPrefUplink");  //adamcand
_LIT8( KParamAudioPrefDownlink, "SessionPrefDownlink");  //adamcand
_LIT8( KParamReasonPhrase, "ReasonPhrase");
_LIT8( KParamStatusCode, "StatusCode" );
_LIT8( KParamSipHeaders, "Headers" );
_LIT8( KParamBody, "Body");
_LIT8( KParamContentType, "MultiPartType");
_LIT8( KParamFCMediaAttrLines, "FCMediaAttributeLines");
_LIT8( KParamReferTo, "ReferTo" );
_LIT8( KParamSuppression, "Suppression" );
_LIT8( KParamProfileId, "ProfileId" );
_LIT8( KParamRemoteURI, "RemoteURI" );
_LIT8( KParamEventHeader, "EventHeader" );
_LIT8( KParamRefreshInterval, "RefreshInterval" );
_LIT8( KParamEventOriginator, "EventOriginator" );
_LIT8( KParamEnable, "Enable" );
_LIT8( KParamActivity, "Activity" );
_LIT8( KParamUseFloorControl, "UseFloorControl" );
_LIT8( KParamMediaLines, "MediaLines" );
_LIT8( KParamSinks, "Sinks" );
_LIT8( KParamSink, "Sink" );
_LIT8( KParamSource, "Source" );
_LIT8( KParamSourceId, "SourceId" );
_LIT8( KParamCodecs, "Codecs" );
_LIT8( KParamBitrates, "Bitrate" );
_LIT8( KParamCodecMode, "CodecMode" );
_LIT8( KParamPayloadType, "PayloadType" );
_LIT8( KParamPTime, "PTime" );
_LIT8( KParamMaxPTime, "MaxPTime" );
_LIT8( KParamSamplingFreq, "SamplingFreq" );
_LIT8( KParamVAD, "VAD" );
_LIT8( KParamNotifyReceiver, "NotifyReceiver" );
_LIT8( KParamNotifyReceiverEventId, "NotifyReceiverEventId" );
_LIT8( KParamSDPLines, "SDPLines" );
_LIT8( KParamContentHeaders, "ContentHeaders");
_LIT8( KParamMediaStreamLocalPort, "MediaStreamLocalPort");
_LIT8( KParamCallbackType, "CallbackType" );
_LIT8( KParamMediaSDPLines, "MediaSDPLines" ); //Will  be removed?
_LIT8( KParamPropertyUid, "PropertyUid" );
_LIT8( KParamPropertyKey, "PropertyKey" );
_LIT8( KParamPropertyValue, "PropertyValue" );
_LIT8( KParamPropertyTextValue, "PropertyTextValue" );
_LIT8( KParamKeepAliveValue, "KeepAliveValue" );
_LIT8( KParamSuppressRtcp, "SuppressRtcp" );
_LIT8( KParamSetDtmfPri, "SetDtmfPriorities" );
_LIT8( KParamMethod, "Method" );
_LIT8( KParamTransactionId, "TransactionId" );
_LIT8( KParamObserverId, "ObserverId" );
_LIT8( KParamSetObserver, "SetObserver" );


//alternative mainstream
_LIT8( KParamMainStreamAudioOut, "MainStreamAudioOut" ); 

_LIT8( KParamSelectCodec, "SelectCodec" );
_LIT8( KParamAudioFileSource, "AudioFileSource" );
_LIT8( KParamMediaFileSink, "MediaFileSink" );
_LIT8(KTestAudioFileName, "C:\\Test.3gp");

//file handling params
_LIT8( KParamGetFileInfo,"GetFileInfo");
_LIT8( KParamDeleteFile,"DeleteFile");
_LIT8( KParamCheckFileExists,"CheckFileExists");
_LIT8( KParamGetFilesFromFolder,"GetFilesFromFolder");
_LIT8( KParamFindFile,"FindFile");


_LIT8( KParamSessionModifier, "SessionModifier" );

_LIT8( KParamDtmfTones, "DtmfTones" );
_LIT8( KParamDtmfDuration, "DtmfDuration" );
_LIT8( KParamDtmfIsSingleTone, "DtmfSingleTone" );

/// CTRL parameter values
_LIT8( KValueSessionTypeAudio, "audio" );
_LIT8( KValueSessionTypeVideo, "video" );
_LIT8( KValueSessionTypeAudioVideo, "audio/video" );
_LIT8( KValueSourceRTP, "RTP" );
_LIT8( KValueSourceMic, "Mic" );
_LIT8( KValueSourceFile, "FileSource" );
_LIT8( KValueSourceCamera, "Camera" );
_LIT8( KValueSinkDisplay, "Display" );
_LIT8( KValueSourceUnknown, "Unknown" );
_LIT8( KValueSinkRTP, "RTP" );
_LIT8( KValueSinkSpeaker, "Speaker" );
_LIT8( KValueSinkFile, "FileSink" );
_LIT8( KValueSinkUnknown, "Unknown" );
_LIT8( KValueCodecAll, "All" );
_LIT8( KValueCodecNone, "None" );
_LIT8( KValueCodecAMR, "AMR" );
_LIT8( KValueCodecH263, "H263" );
_LIT8( KValueCodecDtmf, "telephone-event" );
_LIT8( KValueStreamTypeAudio, "Audio" );
_LIT8( KValueStreamTypeVideo, "Video" );
_LIT8( KValueStreamTypeUnknown, "Unknown" );

_LIT8( KValueSessionModifier100RelNotUsed, "NotUsed" );
_LIT8( KValueSessionModifier100RelSupported, "Supported" );
_LIT8( KValueSessionModifier100RelRequired, "Required" );
_LIT8( KValueControlPathSecurityUnknown, "Unknown" );
_LIT8( KValueControlPathSecure, "Secure" );
_LIT8( KValueControlPathUnsecure, "Unsecure" );


/// Parameter names in a CTRL response

_LIT8( KResponseTestClientIds, "TestClientIds" );
_LIT8( KResponseCode, "ResponseCode" );
_LIT8( KResponseHeaders, "Headers" );
_LIT8( KResponseBody, "Body" );
_LIT8( KResponseContentType, "ContentType");
_LIT8( KResponseStatusCode, "StatusCode");
_LIT8( KResponseReasonPhrase, "ReasonPhrase");
_LIT8( KResponseFCMessage, "FCMessage");
_LIT8( KResponseFCMediaAttrLines, "FCMediaAttributeLines");
_LIT8( KResponseActionReturnCode, "ActionReturnCode" );
_LIT8( KResponseState, "State");
_LIT8( KResponseStateText, "StateText");
_LIT8( KResponseEventState, "EventState");
_LIT8( KResponseEventStateText, "EventStateText");
_LIT8( KResponseReferState, "EventState");
_LIT8( KResponseReferStateText, "ReferStateText");
_LIT8( KResponseConnectionActive, "ConnectionActive");
_LIT8( KResponseReferTo, "ReferTo");
_LIT8( KResponseError, "ErrorCode");
_LIT8( KResponseProfileId, "ProfileId");
_LIT8( KResponseOriginator, "Originator" );
_LIT8( KResponseRecipient, "Recipient" );
_LIT8( KResponseAudioCodecs, "AudioCodecs" );
_LIT8( KResponseVideoCodecs, "VideoCodecs" );
_LIT8( KResponseStreams, "Streams" );
_LIT8( KResponseSinks, "Sinks" );
_LIT8( KResponseSinkIds, "SinkIds" );
_LIT8( KResponseSource, "Source" );
_LIT8( KResponseSourceId, "SourceId" );
_LIT8( KResponseCodecs, "Codecs" );
_LIT8( KResponseStreamState, "StreamState" );
_LIT8( KResponseStreamStateText, "StreamStateText" );
_LIT8( KResponseBoundStreamStateText, "BoundStreamStateText" );
_LIT8( KResponseLocalMediaPort, "LocalMediaPort" );
_LIT8( KResponseBoundStream, "BoundStream" );
_LIT8( KResponseBoundStreamCodecs, "BoundStreamCodecs" );
_LIT8( KResponseBoundStreamSource, "BoundStreamSource" );
_LIT8( KResponseBoundStreamSourceId, "BoundStreamSourceId" );
_LIT8( KResponseBoundStreamSinks, "BoundStreamSinks" );
_LIT8( KResponseBoundStreamSinkIds, "BoundStreamSinkIds" );
_LIT8( KResponseBoundStreamState, "BoundStreamState" );
_LIT8( KResponseBoundLocalMediaPort, "BoundLocalMediaPort" );
_LIT8( KResponseStreamType, "StreamType" );
_LIT8( KResponseBoundStreamType, "BoundStreamType" );
_LIT8( KResponseBitrates, "Bitrate" );
_LIT8( KResponseCodecMode, "CodecMode" );
_LIT8( KResponseFourCC, "FourCC" );
_LIT8( KResponsePayloadType, "PayloadType" );
_LIT8( KResponsePTime, "PTime" );
_LIT8( KResponseMaxPTime, "MaxPTime" );
_LIT8( KResponseSamplingFreq, "SamplingFreq" );
_LIT8( KResponseCodecName, "CodecName" );
_LIT8( KResponseVAD, "VAD" );
_LIT8( KResponseActivity, "Activity");
_LIT8( KResponseSupportedAudioCodecs, "SupportedAudioCodecs" );
_LIT8( KResponseSupportedVideoCodecs, "SupportedVideoCodecs" );
_LIT8( KResponseAllowedBitRate, "AllowedBitRate");
_LIT8( KResponseMediaStreamStateText, "MediaStreamStateText" );
_LIT8( KResponseMediaStreamState, "MediaStreamState" );
_LIT8( KResponseBoundMediaStreamStateText, "BoundMediaStreamStateText" );
_LIT8( KResponseBoundMediaStreamState, "BoundMediaStreamState" );
_LIT8( KResponseControlPathSecurity, "ControlPathSecurity");
_LIT8( KResponsePropertyValue, "PropertyValue");
_LIT8( KResponsePropertyTextValue, "PropertyValue");
_LIT8( KResponseMethod, "Method" );
_LIT8( KResponseTransactionId, "TransactionId" );

/// Error codes

_LIT( KMceErrOOMInObserver, "MCETester" );


// Callback type constants

typedef TInt TTcMceCallbackType;

const TTcMceCallbackType KTcMceUnknown = 0;
const TTcMceCallbackType KTcMceRTPSRReceived = 1;
const TTcMceCallbackType KTcMceRTPRRReceived = 2;
const TTcMceCallbackType KTcMceInactivityTimeout = 3;
const TTcMceCallbackType KTcMceActivity = 4;

// Possible values for parameter KParamObserverId
const TInt KSessionObserver 		= 0;
const TInt KInSessionObserver 		= 1;
const TInt KMediaObserver 			= 2;
const TInt KRtpObserver 			= 3;
const TInt KTransactionObserver 	= 4;
const TInt KInTransactionObserver 	= 5;
const TInt KEventObserver 			= 6;
const TInt KInEventObserver 		= 7;
const TInt KReferObserver 			= 8;
const TInt KInReferObserver 		= 9;
		
#endif	// __MCECONSTANTS_H__
