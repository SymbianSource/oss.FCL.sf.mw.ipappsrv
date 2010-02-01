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
* Description:    Implementation.
*
*/




#ifndef __MCETESTUIENGINECONSTANTS_H
#define __MCETESTUIENGINECONSTANTS_H

#include <e32base.h>

// USER NOTIFICATIONS

// Profile
_LIT( KUserNotificationProfileRegistered, "Profile registered" );
_LIT( KUserNotificationProfileDeregistered, "Profile unregistered" );
_LIT( KUserNotificationProfileUpdated, "Profile updated" );
_LIT( KUserNotificationProfileUnknownEvent, "Unknown profile event occurred" );

// Session
_LIT( KUserNotificationSessionCreated, "Session created" );
_LIT( KUserNotificationSessionCannotCreate, "Session can't be created" );
_LIT( KUserNotificationSessionStateChanged, "Session state changed");
_LIT( KUserNotificationSessionFailed, "Session failed");

// Event
_LIT(KUserNotificationEventCreated, "Event created");
_LIT( KUserNotificationEventCannotCreate, "Event can't be created");
_LIT( KUserNotificationEventStateChanged, "Event state changed");
_LIT( KUserNotificationEventFailed, "Event failed");
_LIT( KUserNotificationNotifyReceived, "Notify Received");

// USER QUESTIONS

// Profile
_LIT( KUserQuestionSelectProfile, "Select profile" );

// Session
_LIT( KUserQuestionSelectUsedProfile, "Select used profile" );
_LIT( KUserQuestionInputRecipient, "Recipient:" );
_LIT( KUserQuestionSelectDirection, "Select direction" );
_LIT( KUserQuestionOptionSendOnly, "SendOnly" );
_LIT( KUserQuestionOptionReceiveOnly, "RecvOnly" );
_LIT( KUserQuestionOptionSendReceive, "SendRecv" );
_LIT( KUserQuestionOptionNoStreams, "No streams" );
_LIT( KUserQuestionNewIncomingSession, "New incoming session" );
_LIT( KUserQuestionOptionAccept, "Accept" );
_LIT( KUserQuestionOptionAcceptWithCustomReason, "Accept (custom)" );
_LIT( KUserQuestionOptionReject, "Reject" );
_LIT( KUserQuestionOptionRejectWithCustomReason, "Reject (custom)" );
_LIT( KUserQuestionOptionHandleLater, "Handle later" );
_LIT( KUserQuestionOptionEstablish, "Establish");
_LIT( KUserQuestionOptionEstalishWithValues, "Establish with values");
_LIT( KUserQuestionOptionUpdate, "Update");
_LIT( KUserQuestionOptionUpdateWithValues,"Update with values");
_LIT( KUserQuestionSendInfo,"Send INFO"); 
_LIT( KUserQuestionSendInfoDTMF,"Send INFO with DTMF"); 
_LIT( KUserQuestionSendInfoResponse, "Send INFO response" ); 
_LIT( KUserQuestionSendInfoResponseCaption, "Select transaction ID" ); 
_LIT( KUserQuestionSendInfoNoTransactions, "No received INFOs" ); 

//Event
_LIT( KUserQuestionInputEventHeader,"EventHeader:");
_LIT( KUserQuestionInputEventContentType,"Content type:");
_LIT( KUserQuestionInputEventContent,"Content:");
// COMMAND CAPTIONS

// Profile

_LIT( KCommandCaptionRegister, "Register" );
_LIT( KCommandCaptionSelectProfile, "Select profile" );
_LIT( KCommandCaptionUnregister, "Unregister" );
_LIT( KCommandCaptionDisplayProfileUser, "Display user" );

// Session
_LIT( KCommandCaptionCreateCustomSession, "Create custom session using profile" );
_LIT( KCommandCaptionCreateCustomSessionAPList, "Create session using APList selection" );
_LIT( KCommandCaptionCreateCustomSessionAPNumber, "Create session using access point number" );
_LIT( KCommandCaptionCreatePoCSession, "Create PoC session" );
_LIT( KCommandCaptionCreateVoIPSession, "Create VoIP session" );
_LIT( KCommandCaptionCreate2WayVideoSession, "Create 2-way video session" );
_LIT( KCommandCaptionDeleteSession, "Delete session" );
_LIT( KCommandCaptionEstablish, "Establish" );
_LIT( KCommandCaptionAnswerSession, "Answer session" );
_LIT( KCommandCaptionAcceptSession, "Accept session" );
_LIT( KCommandCaptionRespondSession, "Respond session" );
_LIT( KCommandCaptionRejectSession, "Reject session" );
_LIT( KCommandCaptionUpdateSession, "Update" );
_LIT( KCommandCaptionHoldSession, "Hold / Unhold call" );
_LIT( KCommandCaptionTerminateSession, "Terminate session" );
_LIT( KCommandCaptionRingingSession, "Ringing ");
_LIT( KCommandCaptionCancelSession, "Cancel Session ");
_LIT (KCommandCaptionSendFCSession, "Send FC Message");
_LIT (KCommandCaptionWriteSdpToFile, "Write SDP to File");
_LIT (KCommandCaptionSessionEnableVideo, "Enable Video");
_LIT (KCommandCaptionSessionDisableVideo, "Disable Video");
// Stream
_LIT( KCommandCaptionEnableStream, "Enable " );
_LIT( KCommandCaptionDisableStream, "Disable " );
_LIT( KCommandCaptionRemoveStream, "Remove Stream " );
_LIT( KCommandCaptionEnableSource, "Enable (Locally)" );
_LIT( KCommandCaptionDisableSource, "Disable (Locally)" );
_LIT( KCommandCaptionEnableSink, "Enable (Locally)" );
_LIT( KCommandCaptionDisableSink, "Disable (Locally)" );
_LIT( KCommandCaptionDeleteAudioCodec, "Delete Audio codec");
_LIT( KCommandCaptionDeleteVideoCodec, "Delete Video codec");

//Event
_LIT( KCommandCaptionSubscribeEvent, "Subscribe Event");
_LIT( KCommandCaptionCreateEvent, "Create Event");
_LIT( KCommandCaptionTerminateEvent, "Terminate Event");
_LIT( KCommandCaptionDeleteEvent, "Delete Event");
_LIT( KCommandCaptionAcceptEvent, "Accept Event");
_LIT( KCommandCaptionRejectEvent, "Reject Event");
_LIT( KCommandCaptionNotifyEvent, "Notify Event");
_LIT( KCommandCaptionUpdateEvent, "Update Event");
_LIT( KUserNotificationEventNotifyReceived,"Event Notify Received");

//Refer
_LIT( KUserNotificationReferCreated, "Refer created");
_LIT( KCommandCaptionCreateRefer, "Create Refer");
_LIT( KUserNotificationReferCannotCreate, "Cannot create");
_LIT( KCommandCaptionDeleteRefer, "Delete Refer");
_LIT( KCommandCaptionAcceptRefer, "Accept Refer");
_LIT( KCommandCaptionRejectRefer, "Reject Refer");
_LIT( KCommandCaptionRespondRefer, "Respond Refer");
_LIT( KCommandCaptionRefer, "Refer");
_LIT( KUserNotificationReferStateChanged,"Refer state changed");
_LIT( KUserNotificationReferFailed, "Refer Failed ");
_LIT (KCommandCaptionCreateSessionFromRefer, "Create Session from Refer");


// SESSION STATES

_LIT( KSessionStateIdle, "Idle" );
_LIT( KSessionStateOffering, "Offering" );
_LIT( KSessionStateIncoming, "Incoming" );
_LIT( KSessionStateReserving, "Reserving" );
_LIT( KSessionStateAnswering, "Answering" );
_LIT( KSessionStateProceeding, "Proceeding" );
_LIT( KSessionStateEstablished, "Established" );
_LIT( KSessionStateCancelling, "Cancelling" );
_LIT( KSessionStateTerminating, "Terminating" );
_LIT( KSessionStateTerminated, "Terminated" );
_LIT( KSessionStateUnknown, "Unknown state" );

// EVENT STATES
_LIT( KEventStateIdle, "Idle" );
_LIT( KEventStatePending, "Pending " );
_LIT( KEventStateActive, "Active " );
_LIT( KEventStateTerminated, "Terminated" );
_LIT( KEventStateUnknown, "Unknown state" );

//REFER STATES

_LIT( KReferStateIdle,"Idle");
_LIT( KReferStatePending,"Pending");
_LIT( KReferStateAccepted,"Accepted");
_LIT( KReferStateTerminated,"Terminated");
_LIT( KReferStateUnknown,"Unknown state");

// SESSION DIRECTIONS

_LIT( KSessionDirectionSendRecv, "SR" );
_LIT( KSessionDirectionRecvOnly, "ROnly" );
_LIT( KSessionDirectionSendOnly, "SOnly" );
_LIT( KSessionDirectionNotSpesified, " NA " );

// STREAM STATES
_LIT( KStreamStateEnabled, "Enabled" );
_LIT( KStreamStateDisabled, "Disabled" );
_LIT( KStreamUninitialized, "Uninitialized");      
_LIT( KStreamInitialized,"Initialized");
_LIT( KStreamBuffering,"Buffering");
_LIT( KStreamIdle,"Idle");
_LIT( KStreamStreaming,"Streaming");
_LIT( KStreamDisabled,"Disabled");        
_LIT( KStreamNoResources,"NoResources");
_LIT( KStreamTranscodingRequired,"TranscodingRequired");
_LIT( KStreamTranscoding,"Transcoding");      
_LIT( KUserNotificationStreamStateChanged,"Stream state changed");        

// STREAM DIRECTIONS            
_LIT( KStreamDirectionUpLink, "UpLink" );
_LIT( KStreamDirectionDownLink, "DownLink" );
_LIT( KStreamDirectionNotSpesified, " NA " );


// SOURCE STATES
_LIT( KSourceStateEnabled, "Enabled" );
_LIT( KSourceStateDisabled, "Disabled" );

// SINK STATES
_LIT( KSinkStateEnabled, "Enabled" );
_LIT( KSinkStateDisabled, "Disabled" );


// MISC
_LIT( KSpace, " ");
_LIT( KMiscIncomingSession, "Incoming session" );
_LIT( KMiscIncomingVoIPSession, "Incoming VoIP session" );
_LIT( KMiscIncomingEvent,"Incoming Event");
_LIT8( KAcceptContactMCETestUI, "Accept-Contact : *;MCETESTUI" );
_LIT( KRecipent, "sip:a@");
_LIT( KNone, "None");
_LIT( KContentType, "application/sdp");
_LIT( KNotifyContentType, "message/sipfrag;version=2.0"); /* 0*/
_LIT( KNotifyContent, "SIP/2.0 200 OK");
_LIT( KUserNotificationSRReceived,"SR Received");
_LIT( KUserNotificationRRReceived, "RR Received");
_LIT( KLeftParenthesis, " (" );
_LIT( KRightParenthesis, ")" );
_LIT( KOutSession, "OutSession: " );
_LIT( KInSession, "InSession: " );
_LIT( KSession, "Session" );
_LIT( KProfileId, "PrId:" );
_LIT( KAudioStream, "AudioStream " );
_LIT( KVideoStream, "VideoStream " );
_LIT(KLabelRTPSource, "RtpSource ");
_LIT(KLabelFileSource, "FileSource ");
_LIT(KLabelMicSource, "MicSource");
_LIT(KLabelRTPSink, "RtpSink");
_LIT(KLabelSpeakerSink, "SpeakerSink");
_LIT(KLabelDisplaySink, "DisplaySink");
_LIT(KLabelFileSink, "FileSink");
_LIT(KLabelCamSource, "CameraSource");
_LIT( KTab, "\t" );
_LIT( KSource, "Source " );
_LIT( KSink, "Sink " );
_LIT( KCodec, "Codec " );
_LIT( KMCEManager, "MCEManager" );
_LIT( KProfile, "Profile: " );
_LIT( KSessionCount, "Sessions:" );
_LIT( KRegistered, "Registered" );
_LIT( KOutEvent, "OutEvent: " );
_LIT( KInEvent, "InEvent: " );
_LIT( KEventCount, "Events:" );
_LIT( KOutRefer, "OutRefer: " );
_LIT( KInRefer, "InRefer: " );
_LIT( KReferCount, "Refers:" );
_LIT( KNotRegistered, "Not registered" );
_LIT( KLeaveMessage, "Command left with value: " );
_LIT( KStream, "Stream" );
_LIT8( KMceCodecAMR, "AMR" );
_LIT8( KMceCodecPCMA, "PCMA" );
_LIT8( KMceCodecPCMU, "PCMU" );
_LIT8( KSDPNameiLBC, "iLBC");
_LIT( KEventHeader, "Presence");
_LIT( KMiscIncomingRefer,"Incoming Refer ");
_LIT (KUserQuestionInputReferTo,"Refer To");
_LIT( KFCMessageReceived,"FCMessage Received");
_LIT( KCallStateActivated,"CS Call activated");
_LIT( KCallStateDeActivated,"CS Call deactivated");
_LIT( KFCMessageErrorOccured,"FCMessage Error Occured");
_LIT (KCommandCaptionCreateMceSettings, "Settings");
_LIT (KCommandCaptionCreateMceValueSettings, "Constants");
_LIT8(KFCMessage, "Here is some FC Message");
_LIT( KUserInactivityTimeoutReceived,"Inactivitytimeout received");
_LIT (KUserActivityTimeReceived, "ACtivitytime received");
_LIT( KCommandCaptionEnableRtpSourceInActivityTimer, "Enable Inactivity Timer");
_LIT(KCommandCaptionSendRR, "Send RR");
_LIT(KCommandCaptionSendSR, "Send SR");
_LIT(KCommandCaptionSetAudioRouting, "Set AudioRouting");
_LIT(KCommandCaptionSetGain,"SetGain");
_LIT(KUserQuestionInputTranscodeFileName,"Transcode FileName");
_LIT(KCommandCaptionTranscode,"Transcode File");
_LIT(KCommandCaptionCancelTranscode,"Cancel Transcode");
_LIT(KDefaultTranscodeFile,"c:\\.3gp");
_LIT(KDefaultAcceptContactValue,"Accept-Contact:*;+g.3gpp.cs-voice");
_LIT(KUpdateFailed,"Update Failed");
_LIT(KCommandCaptionDTMFStart,"Start DTMF Tone");
_LIT(KCommandCaptionDTMFStop,"Stop DTMF Tone");
_LIT(KCommandCaptionDTMFSendTone,"Send DTMF Tone");
_LIT(KCommandCaptionDTMFSendSequence,"Send DTMF Tone Sequence");
_LIT(KNotifyDTMFFailed,"DTMF Failed");
_LIT(KRequestFailed,"Sending Request Failed");
_LIT(KDtmfToneReceived,"DTMF Tone Received:");
_LIT(KDtmfEventReceived,"DTMF Event Received:");
_LIT(KDtmfStarted,"Started");
_LIT(KDtmfCompleted,"Completed");
_LIT(KDtmfErrorOccured,"DTMF Error Occured!");
_LIT(KCommandCaptionCurrentCryptoContext,"Current CryptoContext");
_LIT(KCommandCaptionSupportedCryptoContext,"Supported CryptoContext");
_LIT(KCommandCaptionSetCryptoContext,"Set CryptoConext");
_LIT(KCommandCaptionAddCodec, "Add Codec");
_LIT(KUserNotificationReferActiveConnectionStateChangedReceived,"Active Refer Connection State Received");
_LIT(KUserNotificationReferNotActiveConnectionStateChangedReceived,"Not Active Refer Connection State Received");
_LIT(KUserNotificationEventActiveConnectionStateChangedReceived,"Active Event Connection State Received");
_LIT(KUserNotificationEventNotActiveConnectionStateChangedReceived,"Not Active Event Connection State Received");
_LIT(KCommandCaptionStandByVideoCodec,"StandBy");
_LIT(KCommandCaptionEnableVideoCodec,"Enable");
_LIT(KCommandCaptionDisableVideoCodec,"Disable");
_LIT(KCodecStateEnabled,"Enabled");
_LIT(KCodecStateDisabled,"Disabled");
_LIT(KCodecStateStandBy,"StandBy");
_LIT(KResponseReceived,"ResponseReceived");		
_LIT(KTransactionFailed,"TransactionFailed");	
_LIT(KRequestReceived,"RequestReceived");		
_LIT(KResponseSent,"ResponseSent");		

const TUint32 KMceTestUiAvcEncoderUid = 0x20001c13;

#endif	// __MCETESTUIENGINECONSTANTS_H





