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






#ifndef MCEEVENTTESTCONSTS_H
#define MCEEVENTTESTCONSTS_H
#pragma warn_illtokenpasting off

_LIT8(KEventHeader, "Subscribe");
_LIT8(KReferEventHeader, "refer");
_LIT8(KRemoteUri, "sip:any@at.com");
_LIT8(KRemoteUri2, "sip:any1@at.com");
_LIT8(KReferTo, "sip:chuan@any.com");
_LIT8(KReferTo2, "sip:chuan.wang@any.com");
_LIT8(KModifyHeader, "Refer-To: chuan@nokia.com");
_LIT8(KNotifyContent, "SIP/2.0 100 Trying");
_LIT8(KNotifyContent2, "SIP/2.0 200 OK");
_LIT8(KContentType, "text/plain");
_LIT8(KContentType2, "application/sdp");
_LIT8(KContentTypeMessage, "message");
_LIT8(KContentTypeRefer, "message/sipfrag" );
_LIT8(KContent, "content plain text" );
_LIT8(KContent2, "SDP content" );
_LIT8(KReasonPhrase, "test reason phrase");
_LIT8(KReasonPhrase2, "another test reson phrase");
_LIT8(KReferEventPackage, "refer" );
_LIT8(KEventPackage, "dialog" );
_LIT8(KSubscrStateActive, "active" );
_LIT8(KSubscrStatePending, "pending" );
_LIT8(KSubscrStateTerminated, "terminated" );
_LIT8(KSubscrStateOther, "other" );
_LIT8(KSupprValueTrue, "true");
_LIT8(KIdParamValue, "11235");
_LIT8(KIdParamValue2, "112358");
_LIT8(KFeatureTag, "+featuretag");
_LIT8(KParamValue, "param=value" );
_LIT8(KParam, "param" );
_LIT8(KValue, "value" );
_LIT8(KSIPProfileDefAOR, "sip:user@aa");
_LIT8(KContactValue, "sip:user@localhost");

const TUint KStatusCode = 567;
const TUint KStatusCode2 = 444;
const TUint KDefaultExpires = 180;
const TUint KCSeqValue = 11235;



static const TUid KTestUid = { 0x10000001 };

// enum for received requests
enum TMceEventTestRequest
    {
    ENoRequest = 0,
    ESubscribeRequest,
    EReferRequest,
    ENotifyRequest
    };

#define MCE_EVENT_UNITTEST_DEFS\
    friend class UT_CMceComEvent;\
    friend class UT_CMceEventContext;\
    friend class UT_CMceEventSubscribeMoIdleState;\
    friend class UT_CMceEventSubscribeMtIdleState;\
    friend class UT_CMceEventSubscribeMoEstablishingState;\
    friend class UT_CMceEventSubscribeMtEstablishingState;\
    friend class UT_CMceEventSubscribeMoEstablishedState;\
    friend class UT_CMceEventSubscribeMtEstablishedState;\
    friend class UT_CMceEventSubscribeTerminatingState;\
    friend class UT_CMceEventSubscribeTerminatedState;\
    friend class UT_CMceEventReferMoIdleState;\
    friend class UT_CMceEventReferMtIdleState;\
    friend class UT_CMceEventReferMoEstablishingState;\
    friend class UT_CMceEventReferMtEstablishingState;\
    friend class UT_CMceEventReferMoEstablishedState;\
    friend class UT_CMceEventReferMtEstablishedState;\
    friend class UT_CMceEventReferTerminatingState;\
    friend class UT_CMceEventReferTerminatedState;\
    friend class UT_MCESIPEventHelper;
    

#endif // MCEEVENTTESTCONSTS_H

// End of File
