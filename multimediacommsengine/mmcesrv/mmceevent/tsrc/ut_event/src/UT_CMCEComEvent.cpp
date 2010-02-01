/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "ut_cmcecomevent.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <uri8.h>
#include <sipeventheader.h>
#include <sipmessageelements.h>

//  INTERNAL INCLUDES
#include "mcecomevent.h"
#include "sip.h"
#include "mcesipevent.h"
#include "SIPServerTransaction.h"
#include "sipnotifydialogassoc.h"
#include "sipreferdialogassoc.h"
#include "sipprofileregistry.h"
#include "siperr.h"
#include "siprefertoheader.h"
#include "sipfromheader.h"
#include "MCEEventTestConsts.h"
#include "siprequestelements.h"
#include "sipstrconsts.h"
#include "mcesipeventhelper.h"
#include "sipprofile.h"
#include "mcesipconnection.h"
#include "mceserial.h"

// CONSTRUCTION
UT_CMceComEvent* UT_CMceComEvent::NewL()
    {
    UT_CMceComEvent* self = UT_CMceComEvent::NewLC(); 
    CleanupStack::Pop(self);

    return self;
    }

UT_CMceComEvent* UT_CMceComEvent::NewLC()
    {
    UT_CMceComEvent* self = new( ELeave ) UT_CMceComEvent();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceComEvent::~UT_CMceComEvent()
    {
    }

// Default constructor
UT_CMceComEvent::UT_CMceComEvent()
    {
    }

// Second phase construct
void UT_CMceComEvent::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceComEvent::SetupL(  )
    {
    SIPStrings::OpenL();
	iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL( *iConnection, *this );
	iEvent = iSIPEvent->iEvent;
	}
     
void UT_CMceComEvent::SetupReferL(  )
    {
    SIPStrings::OpenL();
    iSipProfile = CSIPProfile::NewL();
	iConnection = CMceSipConnection::NewL( *iSipProfile );
	iSIPEvent = CMceSipEvent::NewL(  *iConnection, *this );
	iSIPEvent->SetEventStateType( EMtRefer );
    iEvent = iSIPEvent->iEvent;
	}

void UT_CMceComEvent::Teardown(  )
    {
    delete iSIPEvent;
	delete iConnection;
	iEvent=0;
	delete iSipProfile;
	SIPStrings::Close();
	}

void UT_CMceComEvent::TeardownRefer(  )
    {
    delete iSIPEvent;
	delete iConnection;
	delete iSipProfile;
	iEvent=0;
	SIPStrings::Close();
	}

void UT_CMceComEvent::UT_CMceComEvent_NewLL(  )
    {
    }

void UT_CMceComEvent::UT_CMceComEvent_DestructorL(  )
    {
    CMceComEvent* event = CMceComEvent::NewL( *iSIPEvent,
								               KDefaultExpires );
	CleanupStack::PushL( event );								               
    CDesC8Array* hdrs = new (ELeave) CDesC8ArrayFlat(1);
    event->iMsgHeaders = hdrs;
    
    CleanupStack::PopAndDestroy( event );
    
    // Another, with no headers.
    CMceComEvent* event2 = CMceComEvent::NewL( *iSIPEvent,
								               KDefaultExpires );
	delete event2;
    }

void UT_CMceComEvent::UT_CMceComEvent_PerformActionLL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    HBufC8* emptyContentType = KNullDesC8().AllocLC();   
    HBufC8* emptyContent = KNullDesC8().AllocLC();
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								
	iEvent->SetMsgContentL( emptyContentType, emptyContent );
    CleanupStack::Pop( emptyContent );
	CleanupStack::Pop( emptyContentType );
	
    // Error case first.
    EUNIT_ASSERT_LEAVE( iEvent->PerformActionL ( EMceItcReferEvent ) );
    
    // The successfull case
    CSIPReferDialogAssoc* dialog = CSIPReferDialogAssoc::NewL();
    iSIPEvent->SetDialog( dialog );
    iEvent->PerformActionL ( EMceItcReferEvent );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceEstablishingEventStateIndex);
        				
    }

void UT_CMceComEvent::UT_CMceComEvent_ProceedL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								
	EUNIT_ASSERT_LEAVE( iEvent->ProceedL() );
    EUNIT_ASSERT(iEvent->EventContext().CurrentState()==
    				KMceIdleEventStateIndex);
    
    }

void UT_CMceComEvent::UT_CMceComEvent_ReceivedRequestL(  )
    {
    // Error case first
    EUNIT_ASSERT_LEAVE( iEvent->ReceivedRequestL() );
    // Then successfull case 
    CSIPNotifyDialogAssoc* dialog = CSIPNotifyDialogAssoc::NewL();
    iSIPEvent->SetDialog( dialog );
    
    CSIPServerTransaction* serverTran = CSIPServerTransaction::NewL();
    serverTran->SetType( 2 ); // refer
    iSIPEvent->iRequest = serverTran;
    
    TUriParser8 uriParser;
    User::LeaveIfError(uriParser.Parse(KRemoteUri()));
    CUri8* remoteUri = CUri8::NewL(uriParser);
    CleanupStack::PushL(remoteUri);
    
    CSIPRequestElements* elem =CSIPRequestElements::NewL(remoteUri);
    CleanupStack::Pop(remoteUri);
    serverTran->SetRequestElements(elem);
    
    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC(&headers); 
    
    CSIPReferToHeader* referTo = CSIPReferToHeader::DecodeL( KReferTo );
    CleanupStack::PushL( referTo );
    
    headers.AppendL( referTo );
    CleanupStack::Pop( referTo );
    elem->MessageElements().SetUserHeadersL( headers );
    CleanupStack::Pop( &headers ); 
    
    iEvent->ReceivedRequestL();
    }

void UT_CMceComEvent::UT_CMceComEvent_EventContextL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								
	TMceEventStateIndex state = iEvent->EventContext().CurrentState();
    EUNIT_ASSERT(KMceIdleEventStateIndex== state);
    
    }

void UT_CMceComEvent::UT_CMceComEvent_PreviousActionL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								
	TInt action =iEvent->PreviousAction();
    EUNIT_ASSERT( action == KErrNotFound );
    }

void UT_CMceComEvent::UT_CMceComEvent_SetPreviousActionL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								
	iEvent->SetPreviousAction( EMceItcReferEvent );
    EUNIT_ASSERT( iEvent->PreviousAction() == EMceItcReferEvent );
    }

void UT_CMceComEvent::UT_CMceComEvent_RefreshIntervalL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, KDefaultExpires );
    iSIPEvent->iEvent = iEvent;								
	EUNIT_ASSERT(iEvent->RefreshInterval() == KDefaultExpires );
    }

void UT_CMceComEvent::UT_CMceComEvent_SetRefreshIntervalL(  )
    {
    iSIPEvent->SetEventStateType(EMoRefer);
    const TUint32 refreshInterval = 60;
    MCE_DELETE( iSIPEvent->iEvent );
    iEvent = CMceComEvent::NewL( *iSIPEvent, refreshInterval );
    iSIPEvent->iEvent = iEvent;								
	EUNIT_ASSERT(iEvent->iRefreshInterval==refreshInterval);
	const TUint32 newInterval = 180;
	iEvent->SetRefreshInterval( newInterval );
	EUNIT_ASSERT( iEvent->iRefreshInterval == newInterval );
    }

void UT_CMceComEvent::UT_CMceComEvent_SetReasonPhraseLL()
    {
    HBufC8* reasonPhrase = KReasonPhrase().AllocLC();
	HBufC8* reasonPhrase2 = KReasonPhrase2().AllocLC();
    iEvent->SetReasonPhraseL( KStatusCode, reasonPhrase );
    EUNIT_ASSERT( iEvent->iStatusCode == KStatusCode );
    EUNIT_ASSERT( ( *( iEvent->iReason ) ).Compare( KReasonPhrase ) == 0 );
        
    iEvent->SetReasonPhraseL( KStatusCode2, reasonPhrase2 );
    EUNIT_ASSERT( iEvent->iStatusCode == KStatusCode2 );
    EUNIT_ASSERT( ( *( iEvent->iReason ) ).Compare( KReasonPhrase2 ) == 0 );
    
    CleanupStack::Pop( reasonPhrase2 );
    CleanupStack::Pop( reasonPhrase );
    // NULL
    iEvent->SetReasonPhraseL( KErrNotFound, NULL );
    EUNIT_ASSERT( iEvent->iStatusCode == KErrNotFound );
    EUNIT_ASSERT( ( *( iEvent->iReason ) ).Compare( KNullDesC8 ) == 0 );
    
    }
         
void UT_CMceComEvent::UT_CMceComEvent_ReasonPhraseL()
    {
    iEvent->iReason = KReasonPhrase().AllocL();
    EUNIT_ASSERT( iEvent->ReasonPhrase().Compare( KReasonPhrase ) == 0 );
    }
         
void UT_CMceComEvent::UT_CMceComEvent_StatusCodeL()
    {
    iEvent->iStatusCode = KStatusCode;
    EUNIT_ASSERT( iEvent->StatusCode() == KStatusCode );
    }


void UT_CMceComEvent::UT_CMceComEvent_SetMsgContentL()
    {
    HBufC8* contentType = KContentType().AllocLC();
    HBufC8* contentType2 = KContentType2().AllocLC();
    HBufC8* content = KContent().AllocLC(); 
    HBufC8* content2 = KContent2().AllocLC(); 
    // NULL
         
    iEvent->SetMsgContentL( NULL, NULL );
        
    // Null the variables first for coverage
    delete iEvent->iMsgContentType; 
    iEvent->iMsgContentType = NULL;
    delete iEvent->iMsgContent;
    iEvent->iMsgContent = NULL;
    //EUNIT_ASSERT_NO_LEAVE( iEvent->SetMsgContentL( contentType, content ) );
    iEvent->SetMsgContentL( contentType, content );
    EUNIT_ASSERT( ( *( iEvent->iMsgContentType ) ).Compare( KContentType ) == 0 )
    EUNIT_ASSERT( ( *( iEvent->iMsgContent ) ).Compare( KContent ) == 0 )
    
    //EUNIT_ASSERT_NO_LEAVE( iEvent->SetMsgContentL( contentType2, content2 ) );
    iEvent->SetMsgContentL( contentType2, content2 );
    EUNIT_ASSERT( ( *( iEvent->iMsgContentType ) ).Compare( KContentType2 ) == 0 )
    EUNIT_ASSERT( ( *( iEvent->iMsgContent ) ).Compare( KContent2 ) == 0 )
    
    CleanupStack::Pop( content2 );
    CleanupStack::Pop( content );
    CleanupStack::Pop( contentType2 );
    CleanupStack::Pop( contentType );
    }
    
void UT_CMceComEvent::UT_CMceComEvent_SIPEventL(  )
    {
    EUNIT_ASSERT( &iEvent->SIPEvent() == iSIPEvent );
    }

void UT_CMceComEvent::UT_CMceComEvent_SetMsgHeadersL( )
    {
    EUNIT_ASSERT( !iEvent->iMsgHeaders );
    // Call with NULL
    iEvent->SetMsgHeaders( NULL );
    EUNIT_ASSERT( iEvent->iMsgHeaders == NULL );
    // Then successfull
    CDesC8Array* array = new (ELeave) CDesC8ArrayFlat(1);
    CleanupStack::PushL( array );
    CSIPEventHeader* event = CSIPEventHeader::DecodeL( KReferEventHeader );
    CleanupStack::PushL( event );
    HBufC8* eventTxt = event->ToTextValueL();
    CleanupStack::PopAndDestroy( event );
    CleanupStack::PushL( eventTxt );
    array->AppendL( *eventTxt );
    CleanupStack::PopAndDestroy( eventTxt );
    iEvent->SetMsgHeaders( array );
    CleanupStack::Pop( array );
    EUNIT_ASSERT( iEvent->iMsgHeaders == array );
    }

void UT_CMceComEvent::UT_CMceComEvent_MsgHeadersL( )
    {
    EUNIT_ASSERT( !iEvent->iMsgHeaders );
    EUNIT_ASSERT( &iEvent->MsgHeaders( SipStrConsts::EBye ) );
    CDesC8Array* hdrs = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( hdrs );
    hdrs->AppendL( KReferEventHeader );
    iEvent->iMsgHeaders = hdrs;
    CleanupStack::Pop( hdrs );
    CDesC8Array* hdrs2 = &iEvent->MsgHeaders( SipStrConsts::EBye );
    EUNIT_ASSERT( hdrs2 );
    EUNIT_ASSERT( hdrs2->MdcaCount() == 1 );
    EUNIT_ASSERT( hdrs2->MdcaPoint(0).Compare( KReferEventHeader ) == 0 );
    }

void UT_CMceComEvent::UT_CMceComEvent_SetReferTypeL()
    {
    EUNIT_ASSERT( iEvent->iReferType == CMceRefer::ENoSuppression );
    RArray< CMceRefer::TType > types;
    CleanupClosePushL( types );
    types.AppendL( CMceRefer::ENoSuppression );
    types.AppendL( CMceRefer::ESuppressed );
    types.AppendL( CMceRefer::EMultipleRefer );
    
    for ( TInt ind = 0; ind<types.Count(); ind++ )
        {
        CMceRefer::TType type = types[ ind ];
        iEvent->SetReferType( type );
        EUNIT_ASSERT( iEvent->iReferType == type );
        }
    CleanupStack::PopAndDestroy();
    }

void UT_CMceComEvent::UT_CMceComEvent_ReferTypeL()
    {
    EUNIT_ASSERT( iEvent->iReferType == CMceRefer::ENoSuppression );
    RArray< CMceRefer::TType > types;
    CleanupClosePushL( types );
    types.AppendL( CMceRefer::ENoSuppression );
    types.AppendL( CMceRefer::ESuppressed );
    types.AppendL( CMceRefer::EMultipleRefer );
    
    for ( TInt ind = 0; ind<types.Count(); ind++ )
        {
        CMceRefer::TType type = types[ ind ];
        iEvent->iReferType = type;
        EUNIT_ASSERT( iEvent->ReferType() == type );
        }
    CleanupStack::PopAndDestroy();
    
    }

void UT_CMceComEvent::UT_CMceComEvent_SetSilentSuppressionOnL()
    {
    EUNIT_ASSERT( !iEvent->iSilentSuppression );
    iEvent->SetSilentSuppressionOn();
    EUNIT_ASSERT( iEvent->iSilentSuppression );
    }
         
void UT_CMceComEvent::UT_CMceComEvent_SilentSuppressionL()
    {
    EUNIT_ASSERT( !iEvent->SilentSuppression() );
    iEvent->iSilentSuppression = ETrue;
    EUNIT_ASSERT( iEvent->SilentSuppression() );
    iEvent->iSilentSuppression = EFalse;
    EUNIT_ASSERT( !iEvent->SilentSuppression() );
    }

void UT_CMceComEvent::UT_CMceComEvent_IdValueL()
    {
    EUNIT_ASSERT( iEvent->IdValue() == KMceEventIdNotAssigned );
    iEvent->iIdValue = KCSeqValue;
    EUNIT_ASSERT( iEvent->IdValue() == KCSeqValue );
    }

void UT_CMceComEvent::UT_CMceComEvent_SetIdValueL()
    {
    EUNIT_ASSERT( iEvent->iIdValue == KMceEventIdNotAssigned );
    iEvent->SetIdValue( KCSeqValue );
    EUNIT_ASSERT( iEvent->iIdValue == KCSeqValue );    
    }
    
void UT_CMceComEvent::IncomingSubscribeL(
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
void UT_CMceComEvent::IncomingReferL(
					HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody)
	{
	if ( aReferTo )
	    delete aReferTo;
	
	if ( aHeaders )
	    delete aHeaders;
	
	if ( aContentType )
	    delete aContentType;
	
	if ( aBody )
	    delete aBody;	
	}
void UT_CMceComEvent::EventStateChangedL(TUint32 aState)
	{
	iState= aState;		
	}
		
void UT_CMceComEvent::NotifyReceivedL(CDesC8Array* aHeaders,
                    		HBufC8* aContentType,
							HBufC8* aBody)
	{
	delete aHeaders;
	delete aContentType;
	delete aBody;	
	}
									
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceComEvent,
    "UT_CMceComEvent",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceComEvent",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_NewLL, Teardown)

EUNIT_TEST(
    "Destructor - test ",
    "CMceComEvent",
    "~CMceComEvent",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_DestructorL, TeardownRefer)

EUNIT_TEST(
    "PerformActionL - test ",
    "CMceComEvent",
    "PerformActionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_PerformActionLL, Teardown)

EUNIT_TEST(
    "Proceed - test ",
    "CMceComEvent",
    "Proceed",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_ProceedL, Teardown)

EUNIT_TEST(
    "ReceivedRequest - test ",
    "CMceComEvent",
    "ReceivedRequest",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_ReceivedRequestL, TeardownRefer)

EUNIT_TEST(
    "EventContext - test ",
    "CMceComEvent",
    "EventContext",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_EventContextL, Teardown)

EUNIT_TEST(
    "PreviousAction - test ",
    "CMceComEvent",
    "PreviousAction",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_PreviousActionL, Teardown)

EUNIT_TEST(
    "SetPreviousAction - test ",
    "CMceComEvent",
    "SetPreviousAction",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_SetPreviousActionL, Teardown)

EUNIT_TEST(
    "RefreshInterval - test ",
    "CMceComEvent",
    "RefreshInterval",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_RefreshIntervalL, Teardown)

EUNIT_TEST(
    "SetRefreshInterval - test ",
    "CMceComEvent",
    "SetRefreshInterval",
    "FUNCTIONALITY",
    SetupL, UT_CMceComEvent_SetRefreshIntervalL, Teardown)

EUNIT_TEST(
    "SetReasonPhraseL - test ",
    "CMceComEvent",
    "SetReasonPhraseL",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SetReasonPhraseLL, TeardownRefer)
    
EUNIT_TEST(
    "ReasonPhrase - test ",
    "CMceComEvent",
    "ReasonPhrase",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_ReasonPhraseL, TeardownRefer)

EUNIT_TEST(
    "StatusCode - test ",
    "CMceComEvent",
    "StatusCode",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_StatusCodeL, TeardownRefer)

EUNIT_TEST(
    "SetMsgContent - test ",
    "CMceComEvent",
    "SetMsgContent",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SetMsgContentL, TeardownRefer)

EUNIT_TEST(
    "SIPEvent - test ",
    "CMceComEvent",
    "SIPEvent",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SIPEventL, TeardownRefer)

EUNIT_TEST(
    "SetMsgHeaders - test ",
    "CMceComEvent",
    "SetMsgHeaders",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SetMsgHeadersL, TeardownRefer)

EUNIT_TEST(
    "MsgHeaders - test ",
    "CMceComEvent",
    "MsgHeaders",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_MsgHeadersL, TeardownRefer)


EUNIT_TEST(
    "SetReferType - test ",
    "CMceComEvent",
    "SetReferType",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SetReferTypeL, TeardownRefer)

EUNIT_TEST(
    "ReferType - test ",
    "CMceComEvent",
    "ReferType",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_ReferTypeL, TeardownRefer)

EUNIT_TEST(
    "SetSilentSuppression - test ",
    "CMceComEvent",
    "SetSilentSuppression",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SetSilentSuppressionOnL, TeardownRefer)

EUNIT_TEST(
    "SilentSuppression - test ",
    "CMceComEvent",
    "SilentSuppression",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SilentSuppressionL, TeardownRefer)

EUNIT_TEST(
    "IdValue - test ",
    "CMceComEvent",
    "IdValue",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_IdValueL, TeardownRefer)

EUNIT_TEST(
    "SetIdValue - test ",
    "CMceComEvent",
    "SetIdValue",
    "FUNCTIONALITY",
    SetupReferL, UT_CMceComEvent_SetIdValueL, TeardownRefer)


EUNIT_END_TEST_TABLE

//  END OF FILE
