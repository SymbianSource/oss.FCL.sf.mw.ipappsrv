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
#include "ut_cmcestateclientestablishing.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"

#include "mcesipsession.h"
#include "mcesipstatemachine.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcesipconnection.h"
#include "mcecomsession.h"
#include "mcestateclientestablishing.h"
#include <sdporiginfield.h>



// CONSTRUCTION
UT_CMceStateClientEstablishing* UT_CMceStateClientEstablishing::NewL()
    {
    UT_CMceStateClientEstablishing* self = UT_CMceStateClientEstablishing::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateClientEstablishing* UT_CMceStateClientEstablishing::NewLC()
    {
    UT_CMceStateClientEstablishing* self = new( ELeave ) UT_CMceStateClientEstablishing();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateClientEstablishing::~UT_CMceStateClientEstablishing()
    {
    }

// Default constructor
UT_CMceStateClientEstablishing::UT_CMceStateClientEstablishing()
    {
    }

// Second phase construct
void UT_CMceStateClientEstablishing::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateClientEstablishing::SetupL(  )
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();
    
    //TMceIds ids;
    iIds.iProfileID = 1;
    iIds.iManagerType = KMceCSSIPSession;
    iIds.iDialogType = KMceDlgTypeInvite;
    iServerCore = CMceServerCore::NewL();
    
    iStorage = CMCETls::Storage();
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( iIds, *msgParams );
    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iSession = CMceCsSession::NewL(*iServerCore);
    iSession->CreateSubSessionL(iIds, RMessage2());
    iSipSession =  static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[0]);
    iSipSession->NextState( KMceStateClientEstablishing );
    iState = static_cast<CMceStateClientEstablishing*>(&iSipSession->CurrentState());
    }

void UT_CMceStateClientEstablishing::Teardown()
    {
    CMCETls::Close();
    delete iSession;
    delete iServerCore;
    SdpCodecStringPool::Close();
    }


void UT_CMceStateClientEstablishing::UT_CMceStateClientEstablishing_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;

    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );
    EUNIT_ASSERT ( iState->AcceptL( event1 ) );

    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    TMceStateTransitionEvent event3( *iSipSession, EMceCancel );
    TRAPD( e1, iState->AcceptL( event3 ) );
    EUNIT_ASSERT ( e1 == KErrTotalLossOfPrecision );
    
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaSessionStopped );
    TRAPD( e2, iState->AcceptL( event4 ) );
    EUNIT_ASSERT ( e2 == KErrTotalLossOfPrecision );
    
    
    }

void UT_CMceStateClientEstablishing::UT_CMceStateClientEstablishing_EntryLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcCancel, ids, *msg );

    iState->EntryL( event1 );
    
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 0 );

    iSipSession->iOffer = CSdpDocument::NewL();
    // Origin field need to be set at the moment, otherwise CSdpDocument::CloneL will crash.
    _LIT8( KTestOriginField, "o=userb 1234567890 1234567890 IN IP4 10.20.180.72\r\n" );
    CSdpOriginField* origin = CSdpOriginField::DecodeL( KTestOriginField() );
    iSipSession->iOffer->SetOriginField( origin );
    
    delete iSipSession->Body()->iSIPContentType;
    delete iSipSession->Body()->iSIPContent;
    iSipSession->Body()->iSIPContent = NULL;
    iSipSession->Body()->iSIPContentType = NULL;
    
    iSipSession->Body()->iSIPContentType = _L8("mime").AllocL();
    iSipSession->Body()->iSIPContent = _L8("this is multipart content").AllocL();

    TMceStateTransitionEvent event2( *iSipSession, EMceMediaUpdated );
    iState->EntryL( event2 );
    
    EUNIT_ASSERT ( iSipSession->PendingTransactions().Count() == 1 );
    EUNIT_ASSERT ( iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EInvite ) );
    EUNIT_ASSERT ( iStorage->iSipSentMessage != NULL );


    TMceStateTransitionEvent event3( *iSipSession, EMceMediaError );
    iState->EntryL( event3 );
    
    TMceStateTransitionEvent event4( *iSipSession,  TMceMediaEventCode ( KErrNotFound ));
    iState->EntryL( event4 ); 
    
    MCE_RESET_STUBS();
                      
    // Enable & disable
    
    MCE_ASSERT_ENDPOINT_ENABLE_AND_DISABLE();
    
    }
    
void UT_CMceStateClientEstablishing::UT_CMceStateClientEstablishing_ExitLL()
    {
    CMceMsgBase* msg = NULL;
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    iSipSession->iBody->iState = CMceSession::EOffering;
    
    TMceIds ids;
    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaUpdated );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateOffering );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EOffering );
    

    iSipSession->NextState( KMceStateClientEstablishing );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateClientEstablishing );
   
    iSipSession->iSubState = CMceSipSession::EOffering;
    TMceStateTransitionEvent event2( *iSipSession, EMceItcCancel, ids, *msg );
    iState->ExitL( event2 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateTerminated );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::ETerminated );
    EUNIT_ASSERT ( ids.iState == CMceSession::ETerminated );

    iSipSession->iSubState = CMceSipSession::EUpdating;
    TMceStateTransitionEvent event3( *iSipSession, EMceItcCancel, ids, *msg );
    iState->ExitL( event3 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    
    TMceStateTransitionEvent event4( *iSipSession, EMceMediaError );
    iState->ExitL( event4 );
    
    //No affect on the storget and state. 
    TMceStateTransitionEvent event5 ( *iSipSession,TMceMediaEventCode ( KErrNotFound ) );
    iState->ExitL( event5 ); 
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateEstablished );
    EUNIT_ASSERT ( iSipSession->iBody->iState == CMceSession::EEstablished );
    EUNIT_ASSERT ( ids.iState == CMceSession::EEstablished );
    
  
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateClientEstablishing,
    "UT_CMceStateClientEstablishing",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateClientEstablishing",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateClientEstablishing_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateClientEstablishing",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateClientEstablishing_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateClientEstablishing",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateClientEstablishing_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
