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
#include "ut_cmcestateserverinitializing.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "MCETestHelper.h"

#include "mcesipsession.h"
#include "mcenatsipsession.h"
#include "mceservercore.h"
#include "mcecssession.h"
#include "mcecssessionimplementation.h"
#include "mcestateserverinitializing.h"
#include "mceevent.h"


// CONSTRUCTION
UT_CMceStateServerInitializing* UT_CMceStateServerInitializing::NewL()
    {
    UT_CMceStateServerInitializing* self =
    	UT_CMceStateServerInitializing::NewLC();
    CleanupStack::Pop();
    return self;
    }

UT_CMceStateServerInitializing* UT_CMceStateServerInitializing::NewLC()
    {
    UT_CMceStateServerInitializing* self =
    	new ( ELeave ) UT_CMceStateServerInitializing();
    CleanupStack::PushL( self );
	self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMceStateServerInitializing::~UT_CMceStateServerInitializing()
    {
    }

// Default constructor
UT_CMceStateServerInitializing::UT_CMceStateServerInitializing()
    {
    }

// Second phase construct
void UT_CMceStateServerInitializing::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceStateServerInitializing::SetupL()
    {
    CMCETls::OpenL();
    SdpCodecStringPool::OpenL();

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
    iSipSession = static_cast<CMceSipSession*>
    	((iSession->Implementation().SubSessions())[0]);    

    CMceSipManager& manager = iServerCore->Manager();
    CSIPProfile& profile = manager.ProfileL( iIds.iProfileID );
    
    iNatSession = CMceNatSipSession::NewL( *iSession,
		iSipSession->SIPConnection(),
		*iSipSession->iStateMachine,
		profile,
		NULL );
	iNatSession->NextState( KMceStateServerInitializing );
    iState = static_cast<CMceStateServerInitializing*>
    	(&iNatSession->CurrentState());
    }

void UT_CMceStateServerInitializing::Teardown()
    {
    CMCETls::Close();
	delete iSession;
	iSession = NULL;
	delete iNatSession;
	iNatSession = NULL;
    delete iServerCore;
    iServerCore = NULL;

    SdpCodecStringPool::Close();
    }

void UT_CMceStateServerInitializing::UT_CMceStateServerInitializing_DoAcceptLL()
    {
    EUNIT_ASSERT( IsAcceptedL( EMceServerInitialized ) );

	EUNIT_ASSERT( !IsAcceptedL( EMceResponse ) );
	EUNIT_ASSERT( !IsAcceptedL( EMceInvite ) );
	EUNIT_ASSERT( !IsAcceptedL( EMceCancel ) );
    }

void UT_CMceStateServerInitializing::UT_CMceStateServerInitializing_EntryLL()
    {    
    if ( !iNatSession->iBody )
    	{
    	iNatSession->iBody = CMceComSession::NewL();	
    	}
    if ( !iNatSession->iOffer )
    	{
    	iNatSession->iOffer = CSdpDocument::NewL();
    	}

	CSIPServerTransaction* sta =
        CSIPServerTransaction::NewL(
        	SIPStrings::StringF( SipStrConsts::EInvite ) );
	CleanupStack::PushL( sta );
	

	TUriParser8 uriParser;
    User::LeaveIfError( uriParser.Parse( _L8("sip:u1@170.85.90.120") ) );
    CUri8* remoteUri = CUri8::NewLC( uriParser );
	CSIPRequestElements* elem = CSIPRequestElements::NewL( remoteUri );
	CleanupStack::Pop( remoteUri );
	CleanupStack::PushL( elem );

	CSIPAddress* addr = CSIPAddress::DecodeL( _L8( "sip:58.94.37.21" ) );
	CleanupStack::PushL( addr );
	CSIPToHeader* to = CSIPToHeader::NewL( addr );
	CleanupStack::Pop( addr );
	CleanupStack::PushL( to );
	elem->SetToHeaderL( to );
	CleanupStack::Pop( to );
	
	addr = CSIPAddress::DecodeL( _L8( "sip:58.94.37.21" ) );
	CleanupStack::PushL( addr );
	CSIPFromHeader* from = CSIPFromHeader::NewL( addr );
	CleanupStack::Pop( addr );
	CleanupStack::PushL( from );
	elem->SetFromHeaderL( from );
	CleanupStack::Pop( from );
	
	sta->SetRequestElements( elem );
	CleanupStack::Pop( elem );


	iNatSession->iDialog = CSIPInviteDialogAssoc::NewL( *sta );
	iNatSession->iPendingReceivedRequests.AppendL( sta );
	CleanupStack::Pop( sta );
    
    TMceStateTransitionEvent event( *iNatSession, EMceServerInitialized );    
    iState->EntryL( event );
    }
    
void UT_CMceStateServerInitializing::UT_CMceStateServerInitializing_ExitLL()
	{
	// No error
	TMceStateTransitionEvent event( *iNatSession, EMceServerInitialized );
	event.ParamStatus() = KErrNone;
	
	iState->ExitL( event );

	EUNIT_ASSERT( iNatSession->CurrentState().Id() ==
				  KMceStateServerEstablishing );


	// Error result
	event.ParamStatus() = KErrGeneral;
	iNatSession->NextState( KMceStateServerInitializing );
	
	iState->ExitL( event );

	EUNIT_ASSERT( iNatSession->CurrentState().Id() == KMceStateTerminated );
	}

TBool UT_CMceStateServerInitializing::IsAcceptedL( enum TMceSipEventCode aEventCode )
    {
    TMceStateTransitionEvent event( *iNatSession, aEventCode );
    return iState->AcceptL( event );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceStateServerInitializing,
    "UT_CMceStateServerInitializing",
    "UNIT" )

EUNIT_TEST(
    "DoAcceptL - test ",
    "CMceStateServerInitializing",
    "DoAcceptL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerInitializing_DoAcceptLL, Teardown)

EUNIT_TEST(
    "EntryL - test ",
    "CMceStateServerInitializing",
    "EntryL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerInitializing_EntryLL, Teardown)

EUNIT_TEST(
    "ExitL - test ",
    "CMceStateServerInitializing",
    "ExitL",
    "FUNCTIONALITY",
    SetupL, UT_CMceStateServerInitializing_ExitLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
