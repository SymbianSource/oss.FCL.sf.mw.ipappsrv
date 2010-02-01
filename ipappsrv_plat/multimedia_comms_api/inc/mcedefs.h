/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCEDEFS_H
#define MCEDEFS_H

#include <e32base.h>

const TInt KMceMaxSdpNameLength = 20;
const TInt KMceMaxFileNameLength = 512;
const TUint8 KMceMaxPacketrate = 10;
const TInt KMcePTimeFactor = 1000;
const TUint8 KMcePayloadTypeUndefined = 128;

const TInt KMceMaxSizeInReadStream = 1000;
const TInt KMceMaxSizeInSessionAsDesc = 3000;
const TInt KMceExternalizeBufferExpandSize = 100;
const TInt KMceArrayGranularity = 5;


_LIT8( KMceSDPNamePCMU, "PCMU" );
_LIT8( KMceSDPNamePCMA, "PCMA" );
_LIT8( KMceSDPNameAMR, "AMR" );
_LIT8( KMceSDPNameAMRWB, "AMR-WB" );
_LIT8( KMceSDPNameG729, "G729");
_LIT8( KMceSDPNameiLBC, "iLBC");
_LIT8( KMceSDPNameDtmf, "telephone-event" );
_LIT8( KMceSDPNameCn, "CN" );
_LIT8( KMceSDPNameRed, "red" );

_LIT8( KMceSDPNameH263, "H263" );
_LIT8( KMceSDPNameH2632000, "H263-2000" );
_LIT8( KMceSDPNameH2631998, "H263-1998" );
_LIT8( KMceSDPNameH264, "H264" );

typedef TInt TMceReturnStatus;
const TMceReturnStatus KMceAsync = 1;
const TMceReturnStatus KMceReady = 0;

const TInt KMceErrOldSchool = (-100);

const TInt KMceAnswerCryptoCount = 1;

#define MCE_NOT_NULL_PTR( ptr ) ( ptr ? ETrue : EFalse )
#define MCE_IS_NULL_PTR( ptr ) ( !ptr ? ETrue : EFalse )

/**
* Data type for MCE transactions identifiers
*/
typedef TUint32 TMceTransactionId;

// SESSION MODIFIERS

/**
* Data type for session modifiers.
*/
typedef TUint32 TMceSessionModifier;


/** 
* Usage of QOS preconditions within session;
* This modifier can be set only when session is in state MceSession::EIdle. 
* By default, session is in preconditions supported mode.
*
* KMcePreconditionsNotUsed indicates that preconditions are not used.
* KMcePreconditionsSupported indicates that segmented qos preconditions are supported
* KMcePreconditionsE2ESupported value is not supported by MCE qos precondition
*/
// KEY
const TMceSessionModifier KMcePreconditions = 0;
// VALUES
const TUint KMcePreconditionsNotUsed = 0;
const TUint KMcePreconditionsSupported = 1;
const TUint KMcePreconditionsE2ESupported = 2;


/** 
* Usage of reliable provisional responses (aka 100rel) within session.
* This modifier can be set only when session is in state MceSession::EIdle 
* or MceSession::EIncoming.
* By default, session is in 100rel supported mode.
* 
* KMce100RelNotUsed indicates that 100rel is not used.
* KMce100RelSupported indicates that 100rel is supported.
* KMce100RelRequired indicates that 100rel is required.
*/

// KEY
const TMceSessionModifier KMce100Rel = 1;
// VALUES
const TUint KMce100RelNotUsed = 0;
const TUint KMce100RelSupported = 1;
const TUint KMce100RelRequired = 2;


/** 
* Usage of direction attribute within session.
* This modifier can be set only when session is in state MceSession::EIdle 
* or MceSession::EIncoming.
* By default, session is in KMceMediaDirectionOnly.
* 
* KMceMediaDirectionOnly  indicates that only media direction attributes are
*   accepted.
* KMceMediaDirectionWithAddress indicates that media direction as well as
*   Address indicator are accepted.
*/

// KEY
const TMceSessionModifier KMceMediaDirection = 2;

// VALUES
const TUint KMceMediaDirectionOnly = 0;
const TUint KMceMediaDirectionWithAddress = 1;

/** 
* Usage of secure preconditon for SDP attribute within session.
* This modifier can be set only when session is in state MceSession::EIdle 
* or MceSession::EIncoming.
* By default, session is in KMceSecureNormal
* 
* KMceSecureNormal  indicates that media is in secure mode with SAVP/SAVPF 
*   profile.
* KMceSecurePlainAVP indicates that media is in secure mode with AVP profile
*/
// KEY
const TMceSessionModifier KMceSecureSession = 3;
// VALUES
const TUint KMceSecureNormal = 0;
const TUint KMceSecurePlainAVP = 1;

/** 
* Sets the KSoIpTOS socket option (see in_sock.h) for all signalling related
* to the session when profile is not used. When profile is used, its settings
* are used instead.
*/
// KEY
const TMceSessionModifier KMceSignallingTypeOfService = 4;
// VALUES
//The value is of type TUint and can be any supported type of service value.

/** 
* Sets the KSoIpTOS socket option (see in_sock.h) for all media related
* to the session.
*/
// KEY
const TMceSessionModifier KMceMediaTypeOfService = 5;
// VALUES
//The value is of type TUint and can be any supported type of service value.


/** 
* Usage of security preconditions within session;
* This modifier can be set only when session is in state MceSession::EIdle. 
* If security precondition is turing on then qos precondtion will be off
*
* KMcePreconditionsNotUsed indicates that preconditions are not used.
* KMcePreconditionsE2ESupported indicates that preconditions are supported
*/
// KEY
const TMceSessionModifier KMceSecPreconditions = 6;


#ifdef __MWERKS__
#pragma warn_emptydecl off
#endif

#ifdef MCE_UNIT_TEST
#define MCE_UNIT_TEST_DEFS\
    friend class CMceManagerTest;\
    friend class CMceSessionTest;\
    friend class CMceInSessionTest;\
    friend class CMceOutSessionTest;\
    friend class CMceMediaStreamTest;\
    friend class CMceAudioStreamTest;\
    friend class CMceVideoStreamTest;\
    friend class CMceMediaSinkTest;\
    friend class CMceRtpSinkTest;\
    friend class CMceDisplaySinkTest;\
    friend class CMceMediaSourceTest;\
	friend class CMceRtpSourceTest;\
	friend class CMceCameraSourceTest;\
    friend class CMceFileSourceTest;\
    friend class CMceAudioCodecTest;\
    friend class CMceAmrCodecTest;\
    friend class CMceAmrWbCodecTest;\
    friend class CMceVideoCodecTest;\
    friend class CMceEventTest;\
    friend class CMceOutEventTest;\
    friend class CMceInEventTest;\
    friend class CMceReferTest;\
    friend class CMceOutReferTest;\
    friend class CMceInReferTest;\
    friend class CMceFcMsgExchangeTest;\
    friend class CMceStreamBundleTest;\
    friend class CMceFileSinkTest;\
    friend class CMceAvSinkTest;\
    friend class CMceH263CodecTest;\
    friend class CMceG711CodecTest;\
    friend class CMceSecureInSessionTest;\
    friend class CMceAvcCodecTest;\
    friend class CMceSecureOutSessionTest;
#else
#define MCE_UNIT_TEST_DEFS
#endif

#endif
