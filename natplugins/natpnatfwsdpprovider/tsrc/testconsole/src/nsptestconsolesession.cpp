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

#include <e32std.h>
#include <e32base.h>
#include <e32debug.h>
#include <e32cons.h>
#include <badesca.h>
#include <sdpdocument.h>
#include <sdporiginfield.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include "nspplugin.h"
#include "nsptestconsolesession.h"
#include "nsptestasyncservice.h"
#include "nsptestconsolecenrep.h"
#include "nsptestconsolestream.h"

#define __PANIC( aError ) User::Panic( _L( "NSP Test console session" ), aError )
#define PTR_DEL( aPtr ) delete aPtr; aPtr = NULL

_LIT8( KOriginField, "o=alice 2890844526 2890844527 IN IP4 131.117.0.4\r\n" );
_LIT8( KConnFieldLocal, "c=IN IP4 10.0.0.6\r\n" );
_LIT8( KConnFieldRemote, "c=IN IP4 192.168.0.6\r\n" );
_LIT8( KAudioMedia, "m=audio 2330 RTP/AVP 96\r\n" );

const TUint KProtocolUdp = 2;

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::NewL
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleSession* CNSPTestConsoleSession::NewL(
		CNSPPlugin& aApi, CRepository& aCenrep, MAsyncService& aAsync )
    {
    CNSPTestConsoleSession* self = new ( ELeave ) CNSPTestConsoleSession( aApi, aAsync );
    CleanupStack::PushL( self );
    self->ConstructL( aCenrep );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::NewLC
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleSession* CNSPTestConsoleSession::NewLC(
		CNSPPlugin& aApi, CRepository& aCenrep, MAsyncService& aAsync )
    {
    CNSPTestConsoleSession* self = new ( ELeave ) CNSPTestConsoleSession( aApi, aAsync );
    CleanupStack::PushL( self );
    self->ConstructL( aCenrep );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::CNSPTestConsoleSession
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleSession::CNSPTestConsoleSession( CNSPPlugin& aApi, MAsyncService& aAsync )
	: iApi( aApi ),
	  iAsync( aAsync )
	{
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::ConstructL( CRepository& aCenrep )
    {
    TUint32 iap;
    TBuf8<255> buffer;
	TCenrepReader reader( aCenrep );
	
	reader.ReadIapL( iap );
	reader.ReadDomainL( buffer );
	
	iSessionId = iApi.NewSessionL( *this, iap, buffer, KProtocolUdp );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::~CNSPTestConsoleSession
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleSession::~CNSPTestConsoleSession()
    {
    TRAP_IGNORE( iApi.CloseSessionL( iSessionId ) );
    PTR_DEL( iOffer );
    PTR_DEL( iAnswer );
    PTR_DEL( iOutStream );
    PTR_DEL( iInStream );
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::Initialized
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::Initialized( TUint aSessionId )
	{
	__ASSERT_ALWAYS( aSessionId == iSessionId, __PANIC( KErrTotalLossOfPrecision ) );
	iState = ( EInitializing == iState ? EIdle :
				(TSessionState) KErrTotalLossOfPrecision );
	__ASSERT_ALWAYS( iState > 0,  __PANIC( KErrTotalLossOfPrecision ) );
	iAsync.Stop();
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::OfferReady
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::OfferReady( TUint aSessionId, CSdpDocument* aOffer )
	{
	__ASSERT_ALWAYS( aSessionId == iSessionId, __PANIC( KErrTotalLossOfPrecision ) );
	iState = ( ECreateOffer == iState ? EIdle :
				( EDecodeOffer == iState ? EIdle :
					(TSessionState) KErrTotalLossOfPrecision ) );
	StoreOffer( aOffer );
	iAsync.Stop();
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::AnswerReady
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::AnswerReady( TUint aSessionId, CSdpDocument* aAnswer )
	{
	__ASSERT_ALWAYS( aSessionId == iSessionId, __PANIC( KErrTotalLossOfPrecision ) );
	iState = ( ECreateAnswer == iState ? EDecodeOffer :
				( EDecodeAnswer == iState ? EIdle :
					(TSessionState) KErrTotalLossOfPrecision ) );
	StoreAnswer( aAnswer );
	iAsync.Stop();
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::UpdateSdp
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::UpdateSdp( TUint aSessionId, CSdpDocument* aOffer )
	{
	__ASSERT_ALWAYS( aSessionId == iSessionId, __PANIC( KErrTotalLossOfPrecision ) );
	iState = ( EIdle == iState ? EUpdating :
				(TSessionState) KErrTotalLossOfPrecision );
	StoreOffer( aOffer );
	iAsync.Stop();
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::ErrorOccurred
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::ErrorOccurred( TUint aSessionId, TInt aError )
	{
	__ASSERT_ALWAYS( aSessionId == iSessionId, __PANIC( KErrTotalLossOfPrecision ) );
	iState = (TSessionState) aError;
	iAsync.Stop();
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::IcmpErrorOccurred
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::IcmpErrorOccurred( TUint aSessionId, TInt aError )
	{
	__ASSERT_ALWAYS( aSessionId == iSessionId, __PANIC( KErrTotalLossOfPrecision ) );
	iState = (TSessionState) aError;
	iAsync.Stop();
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::CreateOfferL
// -----------------------------------------------------------------------------
// 
TInt CNSPTestConsoleSession::CreateOfferL( CTestConsoleStream& aOutStream )
	{
	__ASSERT_ALWAYS( &aOutStream, User::Leave( KErrArgument ) );
	__ASSERT_ALWAYS( EIdle == iState, User::Leave( KErrTotalLossOfPrecision ) );
	iState = ECreateOffer;
	
	delete iOffer;
	iOffer = CSdpDocument::NewL();
	iOffer->SetConnectionField( CSdpConnectionField::DecodeL( KConnFieldLocal() ) );
	iOffer->SetOriginField( CSdpOriginField::DecodeL( KOriginField() ) );
	CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KAudioMedia() );
	iOffer->MediaFields().AppendL( mediafield );
	CleanupStack::Pop( mediafield );
	aOutStream.EncodeL( *mediafield );
	
	return iApi.CreateOfferL( iSessionId, iOffer );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::ResolveL
// -----------------------------------------------------------------------------
// 
TInt CNSPTestConsoleSession::ResolveL(
		CTestConsoleStream& aInStream, CTestConsoleStream& aOutStream )
	{
	__ASSERT_ALWAYS( &aInStream, User::Leave( KErrArgument ) );
	__ASSERT_ALWAYS( &aOutStream, User::Leave( KErrArgument ) );
	__ASSERT_ALWAYS( EIdle == iState, User::Leave( KErrTotalLossOfPrecision ) );
	iState = ECreateAnswer;
	
	CSdpMediaField* mediafield = NULL;
	
	delete iOffer;
	iOffer = CSdpDocument::NewL();
	iOffer->SetConnectionField( CSdpConnectionField::DecodeL( KConnFieldRemote() ) );
	iOffer->SetOriginField( CSdpOriginField::DecodeL( KOriginField() ) );
	mediafield = CSdpMediaField::DecodeLC( KAudioMedia() );
	iOffer->MediaFields().AppendL( mediafield );
	CleanupStack::Pop( mediafield );
	aInStream.DecodeL( *mediafield );
	
	delete iAnswer;
	iAnswer = CSdpDocument::NewL();
	iAnswer->SetConnectionField( CSdpConnectionField::DecodeL( KConnFieldLocal() ) );
	iAnswer->SetOriginField( CSdpOriginField::DecodeL( KOriginField() ) );
	mediafield = CSdpMediaField::DecodeLC( KAudioMedia() );
	iAnswer->MediaFields().AppendL( mediafield );
	CleanupStack::Pop( mediafield );
	aOutStream.EncodeL( *mediafield );
	
	return iApi.ResolveL( iSessionId, iOffer, iAnswer );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::DecodeAnswerL
// -----------------------------------------------------------------------------
// 
TInt CNSPTestConsoleSession::DecodeAnswerL( CTestConsoleStream& aInStream )
	{
	__ASSERT_ALWAYS( &aInStream, User::Leave( KErrArgument ) );
	__ASSERT_ALWAYS( EIdle == iState, User::Leave( KErrTotalLossOfPrecision ) );
	iState = EDecodeAnswer;
	
	delete iAnswer;
	iAnswer = CSdpDocument::NewL();
	iAnswer->SetConnectionField( CSdpConnectionField::DecodeL( KConnFieldRemote() ) );
	iAnswer->SetOriginField( CSdpOriginField::DecodeL( KOriginField() ) );
	CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KAudioMedia() );
	iAnswer->MediaFields().AppendL( mediafield );
	CleanupStack::Pop( mediafield );
	aInStream.DecodeL( *mediafield );
	
	return iApi.DecodeAnswerL( iSessionId, iAnswer );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::UpdateL
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::UpdateL( CTestConsoleStream& aInStream )
	{
	__ASSERT_ALWAYS( &aInStream, User::Leave( KErrArgument ) );
	
	delete iOffer;
	iOffer = CSdpDocument::NewL();
	iOffer->SetConnectionField( CSdpConnectionField::DecodeL( KConnFieldRemote() ) );
	iOffer->SetOriginField( CSdpOriginField::DecodeL( KOriginField() ) );
	CSdpMediaField* mediafield = CSdpMediaField::DecodeLC( KAudioMedia() );
	iOffer->MediaFields().AppendL( mediafield );
	CleanupStack::Pop( mediafield );
	aInStream.DecodeL( *mediafield );
	
	iApi.UpdateL( iSessionId, iOffer );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::State
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleSession::TSessionState CNSPTestConsoleSession::State() const
	{
	return iState;
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::StoreOutStream
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::StoreOutStream( CTestConsoleStream* aOutStream )
	{
	if ( iOutStream != aOutStream )
		{
		delete iOutStream;
		iOutStream = aOutStream;
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::StoreInStream
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::StoreInStream( CTestConsoleStream* aInStream )
	{
	if ( iInStream != aInStream )
		{
		delete iInStream;
		iInStream = aInStream;
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::StoreOffer
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::StoreOffer( CSdpDocument* aOffer )
	{
	if ( iOffer != aOffer )
		{
		delete iOffer;
		iOffer = aOffer;
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleSession::StoreAnswer
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleSession::StoreAnswer( CSdpDocument* aAnswer )
	{
	if ( iAnswer != aAnswer )
		{
		delete iAnswer;
		iAnswer = aAnswer;
		}
	}

