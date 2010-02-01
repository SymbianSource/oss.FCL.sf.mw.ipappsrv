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
#include "ut_cmcestateerror.h"

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
#include "mcestateerror.h"
#include "mcestateterminated.h"


// CONSTRUCTION
UT_CMceStateError* UT_CMceStateError::NewL()
    {
    UT_CMceStateError* self = UT_CMceStateError::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceStateError* UT_CMceStateError::NewLC()
    {
    UT_CMceStateError* self = new( ELeave ) UT_CMceStateError();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateError::~UT_CMceStateError()
    {
    }

 
// Default constructor
UT_CMceStateError::UT_CMceStateError()
    {
    }

// Second phase construct
void UT_CMceStateError::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMceStateError::SetupL(  )
    {
    CMCETls::OpenL();
    
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
    
    iSipSession->NextState( KMceStateError );
    iState = static_cast<CMceStateError*>(&iSipSession->CurrentState());
    
    }

void UT_CMceStateError::Teardown()
    {
    CMCETls::Close();
	delete iSession;
    delete iServerCore;
    
    }


void UT_CMceStateError::UT_CMceStateError_AcceptLL()
    {
    TMceIds ids;
    CMceMsgBase* msg = NULL;
    
    
    TMceStateTransitionEvent event1( *iSipSession, EMceItcEstablishSession, ids, *msg );
    TRAPD( error, iState->AcceptL( event1 ) );
    EUNIT_ASSERT ( error != KErrNone );

    TMceStateTransitionEvent event2( *iSipSession, EMceAck );
    EUNIT_ASSERT ( iState->AcceptL( event2 ) );

    TMceStateTransitionEvent event3( *iSipSession, EMceMediaSessionStopped );
    EUNIT_ASSERT ( iState->AcceptL( event3 ) );
    
    //state terminated tests
    iSipSession->NextState( KMceStateTerminated );
    CMceStateTerminated* stateTerminated = static_cast<CMceStateTerminated*>(&iSipSession->CurrentState());
    
    TMceStateTransitionEvent event4( *iSipSession, EMceItcEstablishSession, ids, *msg );
    TRAPD( error1, stateTerminated->AcceptL( event4 ) );
    EUNIT_ASSERT ( error1 != KErrNone );
    
    
    }

void UT_CMceStateError::UT_CMceStateError_EntryLL()
    {
    
    EUNIT_ASSERT ( &iState->TransitionHandler() == iState );
    
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaError );
    iState->EntryL( event1 );
    
    }
    
void UT_CMceStateError::UT_CMceStateError_ExitLL()
    {
    iSipSession->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateError );
    
    TMceStateTransitionEvent event1( *iSipSession, EMceMediaError );
    iState->ExitL( event1 );
    EUNIT_ASSERT ( iSipSession->CurrentState().Id() == KMceStateError );
    
    
    }
    
    
        
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateError,
    "UT_CMceStateError",
    "UNIT" )

EUNIT_TEST(
    "AcceptL - test ",
    "CMceStateError",
    "AcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateError_AcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateError",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateError_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateError",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateError_ExitLL, Teardown)
    



EUNIT_END_TEST_TABLE

//  END OF FILE
