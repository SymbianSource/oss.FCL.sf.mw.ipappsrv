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
* Description:  NAT FW SDP Provider test console release tests impl.
*
*/

#include <e32base.h>
#include <e32cons.h>
#include <centralrepository.h>
#include "nspplugin.h"
#include "nspreleasetest.h"
#include "nsptestconsolesession.h"
#include "nsptestconsolestream.h"
#include "nsptestasyncservice.h"
#include "nsptestconsolemacros.h"

_LIT( KRelease, "Release test" );
_LIT( KNewSession, "NewSession test" );
_LIT( KCreateOffer, "CreateOffer test" );
_LIT( KDecodeAnswer, "DecodeAnswer test" );
_LIT( KResolve, "Resolve test" );
_LIT( KUpdate, "Update test" );

#define PTR_DEL( aPtr ) delete aPtr; aPtr = NULL

// -----------------------------------------------------------------------------
// CNSPReleaseTest::NewL
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest* CNSPReleaseTest::NewL( CNSPPlugin& aApi, CRepository& aRep )
    {
    CNSPReleaseTest* self = CNSPReleaseTest::NewLC( aApi, aRep );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPReleaseTest::NewLC
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest* CNSPReleaseTest::NewLC( CNSPPlugin& aApi, CRepository& aRep )
    {
    CNSPReleaseTest* self = new ( ELeave ) CNSPReleaseTest;
    CleanupStack::PushL( self );
    self->ConstructL( aApi, aRep );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPReleaseTest::~CNSPReleaseTest
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::~CNSPReleaseTest()
    {
    iTestArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPReleaseTest
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPReleaseTest()
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::ConstructL( CNSPPlugin& aApi, CRepository& aRep )
    {
    CNSPTest* test = NULL;
    
    test = CNSPTestNewSession::NewLC( aApi, aRep );
    iTestArray.AppendL( test );
    CleanupStack::Pop( test );
    
    test = CNSPTestCreateOffer::NewLC( aApi, aRep );
    iTestArray.AppendL( test );
    CleanupStack::Pop( test );
    
    test = CNSPTestDecodeAnswer::NewLC( aApi, aRep );
    iTestArray.AppendL( test );
    CleanupStack::Pop( test );
    
    /*
    test = CNSPTestResolve::NewLC( aApi, aRep );
    iTestArray.AppendL( test );
    CleanupStack::Pop( test );
    
	test = CNSPTestUpdate::NewLC( aApi, aRep );
    iTestArray.AppendL( test );
    CleanupStack::Pop( test );
    */
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::SetupL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::SetupL()
	{
    iResult.iDescription.Copy( KRelease() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::Teardown
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::Teardown()
	{
	TResult::TState result;
	iResult.iState = TResult::EPass;
	
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		result = iTestArray[i]->State();
		iResult.iState = ( TResult::EPass != result ?
				 TResult::EFail : iResult.iState );
		}
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::ExecuteL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::ExecuteL( CConsoleBase& aConsole )
	{
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->StartL( aConsole );
		}
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::Cancel
// -----------------------------------------------------------------------------
//
void CNSPReleaseTest::Cancel()
	{
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->Cancel();
		}
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::AcceptL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::AcceptL( MNSPTestVisitor& aVisitor )
	{
	aVisitor.VisitL( *this );
	
	for ( TInt i = 0; i < iTestArray.Count(); i++ )
		{
		iTestArray[i]->AcceptL( aVisitor );
		}
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::NewLC
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestNewSession* CNSPReleaseTest::CNSPTestNewSession::NewLC(
		CNSPPlugin& aApi, CRepository& aRep )
	{
	CNSPReleaseTest::CNSPTestNewSession* self =
			new ( ELeave ) CNSPReleaseTest::CNSPTestNewSession( aApi, aRep );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::ConstructL
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestNewSession::CNSPTestNewSession( CNSPPlugin& aApi, CRepository& aRep )
	: iApi( aApi ),
	  iRep( aRep )
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestNewSession::ConstructL()
	{
	iAsyncService = CNSPTestAsyncService::NewL( iTimerStatus );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::CNSPTestNewSession
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestNewSession::~CNSPTestNewSession()
	{
	PTR_DEL( iAsyncService );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::SetupL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestNewSession::SetupL()
	{
	iResult.iDescription.Copy( KNewSession() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::Teardown
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestNewSession::Teardown()
	{
	PTR_DEL( iSession );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::ExecuteL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestNewSession::ExecuteL( CConsoleBase& aConsole )
	{
	PTR_DEL( iSession );
	iSession = CNSPTestConsoleSession::NewL( iApi, iRep, *iAsyncService );
	WAIT( aConsole, "INITIALIZED" );
	iAsyncService->StartScheduler(); // BLOCKING until callback(Initialized)
	COMPLETED_L( aConsole, "INITIALIZED",  iTimerStatus );
	__ASSERT( CNSPTestConsoleSession::EIdle == iSession->State() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::Cancel
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestNewSession::Cancel()
	{
	iAsyncService->Cancel();
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::NewLC
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestCreateOffer* CNSPReleaseTest::CNSPTestCreateOffer::NewLC(
		CNSPPlugin& aApi, CRepository& aRep )
	{
	CNSPReleaseTest::CNSPTestCreateOffer* self =
			new ( ELeave ) CNSPReleaseTest::CNSPTestCreateOffer( aApi, aRep );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::ConstructL
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestCreateOffer::CNSPTestCreateOffer( CNSPPlugin& aApi, CRepository& aRep )
	: CNSPTestNewSession( aApi, aRep )
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestCreateOffer::ConstructL()
	{
	CNSPTestNewSession::ConstructL();
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestNewSession::CNSPTestCreateOffer
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestCreateOffer::~CNSPTestCreateOffer()
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::SetupL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestCreateOffer::SetupL()
	{
	iResult.iDescription.Copy( KCreateOffer() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::Teardown
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestCreateOffer::Teardown()
	{
	PTR_DEL( iSession );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::ExecuteL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestCreateOffer::ExecuteL( CConsoleBase& aConsole )
	{
	CNSPTestNewSession::ExecuteL( aConsole ); // BLOCKING until callback.(Initialized)
	
	CTestConsoleStream* outStream = CTestConsoleStream::NewL();
	iSession->StoreOutStream( outStream ); // ownership changed.
	TNatReturnStatus status = iSession->CreateOfferL( *outStream );
	
	if ( KNatAsync == status )
		{
		__ASSERT( CNSPTestConsoleSession::ECreateOffer == iSession->State() );
		WAIT( aConsole, "OFFER READY" );
		iAsyncService->StartScheduler(); // BLOCKING until callback(OfferReady)
		COMPLETED_L( aConsole, "OFFER READY",  iTimerStatus );
		}
	
	__ASSERT( CNSPTestConsoleSession::EIdle == iSession->State() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestCreateOffer::NewLC
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestDecodeAnswer* CNSPReleaseTest::CNSPTestDecodeAnswer::NewLC(
		CNSPPlugin& aApi, CRepository& aRep )
	{
	CNSPReleaseTest::CNSPTestDecodeAnswer* self =
			new ( ELeave ) CNSPReleaseTest::CNSPTestDecodeAnswer( aApi, aRep );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestDecodeAnswer::CNSPTestDecodeAnswer
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestDecodeAnswer::CNSPTestDecodeAnswer( CNSPPlugin& aApi, CRepository& aRep )
	: CNSPTestCreateOffer( aApi, aRep )
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestDecodeAnswer::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestDecodeAnswer::ConstructL()
	{
	CNSPTestCreateOffer::ConstructL();
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestDecodeAnswer::CNSPTestDecodeAnswer
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestDecodeAnswer::~CNSPTestDecodeAnswer()
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestDecodeAnswer::SetupL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestDecodeAnswer::SetupL()
	{
	iResult.iDescription.Copy( KDecodeAnswer() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestDecodeAnswer::Teardown
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestDecodeAnswer::Teardown()
	{
	PTR_DEL( iSession );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestDecodeAnswer::ExecuteL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestDecodeAnswer::ExecuteL( CConsoleBase& aConsole )
	{
	CNSPTestCreateOffer::ExecuteL( aConsole ); // BLOCKING until callback.(OfferReady)
	
	CTestConsoleStream* inStream = CTestConsoleStream::NewL();
	iSession->StoreInStream( inStream ); // ownership changed.
	TNatReturnStatus status = iSession->DecodeAnswerL( *inStream );
	
	if ( KNatAsync == status )
		{
		__ASSERT( CNSPTestConsoleSession::EDecodeAnswer == iSession->State() );
		WAIT( aConsole, "ANSWER READY" );
		iAsyncService->StartScheduler(); // BLOCKING until callback(AnswerReady)
		COMPLETED_L( aConsole, "ANSWER READY",  iTimerStatus );
		}
	
	__ASSERT( CNSPTestConsoleSession::EIdle == iSession->State() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::NewLC
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestResolve* CNSPReleaseTest::CNSPTestResolve::NewLC(
		CNSPPlugin& aApi, CRepository& aRep )
	{
	CNSPReleaseTest::CNSPTestResolve* self =
	new ( ELeave ) CNSPReleaseTest::CNSPTestResolve( aApi, aRep );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::CNSPTestResolve
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestResolve::CNSPTestResolve( CNSPPlugin& aApi, CRepository& aRep )
	: CNSPTestNewSession( aApi, aRep )
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestResolve::ConstructL()
	{
	CNSPTestNewSession::ConstructL();
	}


// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::~CNSPTestResolve
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestResolve::~CNSPTestResolve()
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::SetupL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestResolve::SetupL()
	{
	iResult.iDescription.Copy( KResolve() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::Teardown
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestResolve::Teardown()
	{
	PTR_DEL( iSession );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestResolve::ExecuteL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestResolve::ExecuteL( CConsoleBase& aConsole )
	{
	CNSPTestNewSession::ExecuteL( aConsole ); // BLOCKING until callback.(Initialized)
	
	CTestConsoleStream* inStream = CTestConsoleStream::NewL();
	iSession->StoreInStream( inStream ); // ownership changed.
	CTestConsoleStream* outStream = CTestConsoleStream::NewL();
	iSession->StoreInStream( outStream ); // ownership changed.
	
	TNatReturnStatus status = iSession->ResolveL( *inStream, *outStream );
	
	if ( KNatAsync == status )
		{
		__ASSERT( CNSPTestConsoleSession::ECreateAnswer == iSession->State() );
		WAIT( aConsole, "ANSWER READY" );
		iAsyncService->StartScheduler(); // BLOCKING until callback(AnswerReady)
		COMPLETED_L( aConsole, "ANSWER READY",  iTimerStatus );
		
		__ASSERT( CNSPTestConsoleSession::EDecodeOffer == iSession->State() );
		WAIT( aConsole, "OFFER READY" );
		iAsyncService->StartScheduler(); // BLOCKING until callback(OfferReady)
		COMPLETED_L( aConsole, "OFFER READY",  iTimerStatus );
		}
	
	__ASSERT( CNSPTestConsoleSession::EIdle == iSession->State() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::NewLC
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestUpdate* CNSPReleaseTest::CNSPTestUpdate::NewLC(
		CNSPPlugin& aApi, CRepository& aRep )
	{
	CNSPReleaseTest::CNSPTestUpdate* self =
	new ( ELeave ) CNSPReleaseTest::CNSPTestUpdate( aApi, aRep );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::CNSPTestUpdate
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestUpdate::CNSPTestUpdate( CNSPPlugin& aApi, CRepository& aRep )
	: CNSPTestNewSession( aApi, aRep )
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestUpdate::ConstructL()
	{
	CNSPTestNewSession::ConstructL();
	}


// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::~CNSPTestUpdate
// -----------------------------------------------------------------------------
// 
CNSPReleaseTest::CNSPTestUpdate::~CNSPTestUpdate()
	{
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::SetupL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestUpdate::SetupL()
	{
	iResult.iDescription.Copy( KResolve() );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::Teardown
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestUpdate::Teardown()
	{
	PTR_DEL( iSession );
	}

// -----------------------------------------------------------------------------
// CNSPReleaseTest::CNSPTestUpdate::ExecuteL
// -----------------------------------------------------------------------------
// 
void CNSPReleaseTest::CNSPTestUpdate::ExecuteL( CConsoleBase& aConsole )
	{
	CNSPTestNewSession::ExecuteL( aConsole ); // BLOCKING until callback.(Initialized)
	
	CTestConsoleStream* inStream = CTestConsoleStream::NewL();
	iSession->StoreInStream( inStream ); // ownership changed.
	CTestConsoleStream* outStream = CTestConsoleStream::NewL();
	iSession->StoreInStream( outStream ); // ownership changed.
	
	TNatReturnStatus status = iSession->ResolveL( *inStream, *outStream );
	
	if ( KNatAsync == status )
		{
		__ASSERT( CNSPTestConsoleSession::ECreateAnswer == iSession->State() );
		WAIT( aConsole, "ANSWER READY" );
		iAsyncService->StartScheduler(); // BLOCKING until callback(AnswerReady)
		COMPLETED_L( aConsole, "ANSWER READY",  iTimerStatus );
		
		inStream = CTestConsoleStream::NewL();
		iSession->StoreInStream( inStream ); // ownership changed.
		iSession->UpdateL( *inStream ); // TBD: ASSERT NO LEAVE
		
		__ASSERT( CNSPTestConsoleSession::EDecodeOffer == iSession->State() );
		WAIT( aConsole, "OFFER READY" );
		iAsyncService->StartScheduler(); // BLOCKING until callback(OfferReady)
		COMPLETED_L( aConsole, "OFFER READY",  iTimerStatus );
		}
	
	__ASSERT( CNSPTestConsoleSession::EIdle == iSession->State() );
	}

