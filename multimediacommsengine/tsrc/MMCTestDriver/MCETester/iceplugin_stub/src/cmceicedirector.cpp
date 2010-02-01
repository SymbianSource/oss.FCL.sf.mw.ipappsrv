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
* Description:    Plugin interface implementation.
*
*/




#include "cmceicedirector.h"
#include "mcetesterpskeys.h"
#include "cmceicepropertywatcher.h"
#include "cmceiceerrortrigger.h"
#include <e32math.h>
#include <e32property.h>
#include <SdpConnectionField.h>
#include <SdpMediaField.h>

const TInt KDefaultInitializationDelay = 50000;
const TInt KDefaultSecondCallbackDelay = 5000000;

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CMceIceDirector::CMceIceDirector
// ---------------------------------------------------------------------------
//
CMceIceDirector::CMceIceDirector() :
   	iRequestType( CMceIceDirector::ENone ),
   	iSessionId( 0 )
	{
    }


// ---------------------------------------------------------------------------
// CMceIceDirector::NewL
// ---------------------------------------------------------------------------
//
CMceIceDirector* CMceIceDirector::NewL()
    {
    CMceIceDirector* self = new( ELeave ) CMceIceDirector;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// CMceIceDirector::ConstructL
// ---------------------------------------------------------------------------
//
void CMceIceDirector::ConstructL()
    {
    iLocalAddressWatcher =
    	CMceIcePropertyWatcher::NewL( KUidMceTesterPSKeys,
    								  KMceTesterNatPluginLocalAddress );
    iRemoteAddressWatcher =
    	CMceIcePropertyWatcher::NewL( KUidMceTesterPSKeys,
    								  KMceTesterNatPluginRemoteAddress );
	iErrorTrigger =
		CMceIceErrorTrigger::NewL( KUidMceTesterPSKeys,
	    						   KMceTesterNatPluginErrorCallback );
	}


// ---------------------------------------------------------------------------
// CMceIceDirector::~CMceIceDirector
// ---------------------------------------------------------------------------
//
CMceIceDirector::~CMceIceDirector() 
    {
    if ( iDTimer )
    	{
    	iDTimer->Remove( iDeltaEntry );
    	}

	delete iDTimer;
	delete iLocalAddressWatcher;
	delete iRemoteAddressWatcher;
	delete iErrorTrigger;
	delete iOffer;
	delete iAnswer;
	}


// ---------------------------------------------------------------------------
// CMceIceDirector::NewSessionL
// ---------------------------------------------------------------------------
//
TUint CMceIceDirector::NewSessionL( MNSPSessionObserver& aSessionObserver,
                                    TUint32 /*aIapId*/,
                                    const TDesC8& /*aDomain*/,
                                    TUint /*aProtocol*/ )
                          
    {
	RDebug::Print(_L("ICEPlugin Dummy: Entry (CMceIceDirector::NewSessionL)"));

	iOfferReady = EFalse;
    iAnswerReady = EFalse;
	iInitialized = EFalse;

	TInt delay( 0 );
	switch ( RProperty::Get( KUidMceTesterPSKeys,
    				    	 KMceTesterNatPluginCallbackDelay,
    				    	 delay ) )
		{
		case KErrNotFound: // Initialization is always async
			delay = KDefaultInitializationDelay;
			break;

		case KErrNone:
			break;

		default:
			User::Leave( KErrArgument );
		}

	delete iDTimer;
	iDTimer = NULL;
	iDTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
	TCallBack cb( TimerExpire, this );
	iDeltaEntry.Set( cb );

   	iSessionOb = &aSessionObserver;

   	iSessionId = Math::Random();
   	iErrorTrigger->SetObserver( *iSessionOb, iSessionId );

   	// Set the timer to get fired for the Initialized Event
	StartTimer( delay );

	RDebug::Print(_L("ICEPlugin Dummy: Exit (CMceIceDirector::NewSessionL)"));	
	return iSessionId;
    }


// ---------------------------------------------------------------------------
// CMceIceDirector::CloseSessionL
// ---------------------------------------------------------------------------
//
void CMceIceDirector::CloseSessionL( TUint /*aSessionId*/ )
    {
   	iSessionId = 0;
   	iSessionOb = NULL;

   	if ( iDTimer )
    	{
    	iDTimer->Remove( iDeltaEntry );
    	}
    }


// ---------------------------------------------------------------------------
// CMceIceDirector::CreateOfferL
// ---------------------------------------------------------------------------
//
TNatReturnStatus
CMceIceDirector::CreateOfferL( TUint aSessionId, CSdpDocument*& aOffer)
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CMceIceDirector::CreateOfferL)"));
    __ASSERT_ALWAYS( aOffer != NULL, User::Leave( KErrArgument ) );

	iRequestType = CMceIceDirector::ECreateOffer;
    iSessionId = aSessionId;

	TNatReturnStatus status = GetNatReturnStatus( ETrue );
    if ( status == KNatReady )
    	{
    	UpdateOfferL( *aOffer );
    	}
    else
    	{
    	CloneSdpL( *aOffer, iOffer );
    	}
    	

    RDebug::Print(_L("ICEPlugin Dummy: Exit (CMceIceDirector::CreateOfferL)"));
    return status;
    }


// ---------------------------------------------------------------------------
// CMceIceDirector::ResolveL
// This is MT case: we've received offer and are creating answer.
// Set public remote address to offer and local address to answer.
// ---------------------------------------------------------------------------
//
TNatReturnStatus CMceIceDirector::ResolveL( TUint aSessionId,
			                                CSdpDocument*& aOffer,
			                                CSdpDocument*& aAnswer )
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CMceIceDirector::ResolveL)"));
    __ASSERT_ALWAYS( aOffer != NULL && aAnswer != NULL,
    				 User::Leave( KErrArgument ) );

    iRequestType = CMceIceDirector::EResolve;
    iSessionId = aSessionId;

	TNatReturnStatus status = GetNatReturnStatus( ETrue );
    if ( status == KNatReady )
    	{
		UpdateAddressL( *aOffer, *iRemoteAddressWatcher );
		UpdateAddressL( *aAnswer, *iLocalAddressWatcher );
    	}
    else
		{
		CloneSdpL( *aOffer, iOffer );
		CloneSdpL( *aAnswer, iAnswer );
		}

    RDebug::Print(_L("ICEPlugin Dummy: Exit (CMceIceDirector::ResolveL)"));
    return status;
    }


// ---------------------------------------------------------------------------
// CMceIceDirector::DecodeAnswerL
// ---------------------------------------------------------------------------
//
TNatReturnStatus
CMceIceDirector::DecodeAnswerL( TUint aSessionId, CSdpDocument*& aAnswer )
    {
    RDebug::Print(
    	_L("ICEPlugin Dummy: Entry (CMceIceDirector::DecodeAnswerL)"));
	__ASSERT_ALWAYS( aAnswer != NULL, User::Leave( KErrArgument ) );

    iRequestType = CMceIceDirector::EDecodeAnswer;
    iSessionId = aSessionId;

	TNatReturnStatus status = GetNatReturnStatus( ETrue );
    if ( status == KNatReady )
    	{
    	UpdateAnswerL( *aAnswer );
    	}
    else
    	{
    	CloneSdpL( *aAnswer, iAnswer );
    	}

    RDebug::Print(
    	_L("ICEPlugin Dummy: Exit (CMceIceDirector::DecodeAnswerL)"));
    return status;
    }


// ---------------------------------------------------------------------------
// CMceIceDirector::UpdateL
// ---------------------------------------------------------------------------
//
void CMceIceDirector::UpdateL( TUint aSessionId, CSdpDocument*& aOffer )
    {
    RDebug::Print(_L("ICEPlugin Dummy: Entry (CMceIceDirector::UpdateL)"));
    __ASSERT_ALWAYS( aOffer != NULL, User::Leave( KErrArgument ) );

    iRequestType = CMceIceDirector::EResolve;
	iSessionId = aSessionId;
	CloneSdpL( *aOffer, iOffer );
	
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CMceIceDirector::UpdateL)"));
    }
    

// ---------------------------------------------------------------------------
// CMceIceDirector::RestartL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CMceIceDirector::RestartL( TUint aSessionId )
	{
	RDebug::Print(_L("ICEPlugin Dummy: Entry (CMceIceDirector::RestartL)"));   
	iSessionId = aSessionId;	
	RDebug::Print(_L("ICEPlugin Dummy: Exit (CMceIceDirector::RestartL)"));
	return GetNatReturnStatus( EFalse );
	}


// ---------------------------------------------------------------------------
// CMceIceDirector::GetSessionParam
// Returns the resource reservation status: 0 = Not Reserved, 1 = Reserved
// ---------------------------------------------------------------------------
//
TInt CMceIceDirector::GetSessionParam( TUint /*aSessionId*/,
                       				   TNSPSessionParamKey /*aParamKey*/ )
 	{
	TInt dummy( 0 );
    if ( RProperty::Get( KUidMceTesterPSKeys,
    			 		 KMceTesterNatPluginResourceReservationStatus,
    			 		 dummy ) == KErrNone )
		{
		RDebug::Print( _L("ICEPlugin Dummy: GetSessionParam = 1" ) );
		return 1;
		}
	RDebug::Print( _L("ICEPlugin Dummy: GetSessionParam = 0" ) );
	return 0;
 	}


// ---------------------------------------------------------------------------
// CMceIceDirector::SetSessionParam
// NOP
// ---------------------------------------------------------------------------
//
TInt CMceIceDirector::SetSessionParam( TUint /*aSessionId*/,
	            TNSPSessionParamKey /*aParamKey*/, TUint /*aParamValue*/ )
	{
	return KErrNone;
	}
    
// -----------------------------------------------------------------------------
// CMceIceDirector::TimerExpire
// -----------------------------------------------------------------------------
//
TInt CMceIceDirector::TimerExpire( TAny *aSelf )
    { 
    CMceIceDirector* me = static_cast<CMceIceDirector *>( aSelf );
    if ( !me->iSessionOb )
    	{
    	return KErrNone;
    	}
    
    if ( !me->iInitialized )
    	{
    	RDebug::Print( _L( "ICEPlugin Dummy: (Plugin->Initialized)" ) );
    	me->iInitialized = ETrue;	
		me->iSessionOb->Initialized( me->iSessionId );
		}
    	
	else if ( me->iOfferReady )
		{
		RDebug::Print( _L( "ICEPlugin Dummy: (Plugin->OfferReady)" ) );
		me->iOfferReady = EFalse;
		// MT case: set public remote address to offer
		me->UpdateAddressL( *me->iOffer, *me->iRemoteAddressWatcher );
		me->MakeOfferReadyCallback( *me );
		}

	//TBD: For Future Use when Re-Invite for UpdateSDP is supported
	
	/*	
	else if ( me->iAnswerReady )
		{
		RDebug::Print( _L( "ICEPlugin Dummy: (Plugin->OfferReady)" ) );
		me->iOfferReady = EFalse;
		// Ownership of iOffer is transferred
		me->iSessionOb->UpdateSdp( 0, me->iOffer );	
		}
	*/
	else
		{
		switch ( me->iRequestType )
			{
			case ECreateOffer:
	            RDebug::Print( _L( "ICEPlugin Dummy: (Plugin->OfferReady)" ) );
				me->UpdateOfferL( *me->iOffer );
				me->MakeOfferReadyCallback( *me );
	            break;

			case EResolve:
	            RDebug::Print( _L( "ICEPlugin Dummy: (Plugin->AnswerReady)" ) );
				// MT case: set public local address to answer
				me->UpdateAddressL( *me->iAnswer, *me->iLocalAddressWatcher );
				me->MakeAnswerReadyCallback( *me );
	            me->iOfferReady = ETrue;
	            me->SetSecondCallbackTimer( *me );
	            break;

			case EDecodeAnswer:
	            RDebug::Print( _L( "ICEPlugin Dummy: (Plugin->AnswerReady) ") );
				me->iAnswerReady = ETrue;
				me->UpdateAnswerL( *me->iAnswer );
				me->MakeAnswerReadyCallback( *me );

				// TBD: For Future Use when Re-Invite for UpdateSDP is supported
				// me->StartTimer( delay*50 );
				break;
	      
	        default:
	            break; // NOP
			}
		}
	   
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CMceIceDirector::StartTimer
// -----------------------------------------------------------------------------
//
void CMceIceDirector::StartTimer( TInt aDelay )
	{
	RDebug::Print( _L("ICEPlugin Dummy: StartTimer %dus" ), aDelay );

	iDTimer->Remove( iDeltaEntry );
	iDTimer->Queue( aDelay, iDeltaEntry );
	}

// -----------------------------------------------------------------------------
// CMceIceDirector::GetNatReturnStatus
// If KMceTesterNatPluginCallbackDelay exists return "async", otherwise "ready".
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceIceDirector::GetNatReturnStatus( TBool aStartTimer )
	{
	TInt delay( 0 );
    if ( RProperty::Get( KUidMceTesterPSKeys,
    	 				 KMceTesterNatPluginCallbackDelay,
    					 delay ) != KErrNone )
    	{
		RDebug::Print( _L( "ICEPlugin Dummy: GetNatReturnStatus = nat ready" ) );
		return KNatReady;
		}

	if ( aStartTimer )
		{
		StartTimer( delay );
		}

	RDebug::Print( _L( "ICEPlugin Dummy: GetNatReturnStatus = nat async" ) );
	return KNatAsync;
	}


// -----------------------------------------------------------------------------
// CMceIceDirector::SetSecondCallbackTimer
// -----------------------------------------------------------------------------
//
void CMceIceDirector::SetSecondCallbackTimer( CMceIceDirector& aIceDirector )
	{
	TInt delay( 0 );
    if ( RProperty::Get( KUidMceTesterPSKeys,
    	 				 KMceTesterNatPluginSecondCallbackDelay,
    					 delay ) != KErrNone )
		{
		delay = KDefaultSecondCallbackDelay;
		}

   	aIceDirector.StartTimer( delay );
	}


// -----------------------------------------------------------------------------
// CMceIceDirector::CloneSdpL
// -----------------------------------------------------------------------------
//
void
CMceIceDirector::CloneSdpL( const CSdpDocument& aSource, CSdpDocument*& aDest )
	{
	delete aDest;
	aDest = NULL;
    aDest = aSource.CloneL();
	}

		
// -----------------------------------------------------------------------------
// CMceIceDirector::UpdateOfferL
// Set public local address. This is MO case or MT pull mode case.
// -----------------------------------------------------------------------------
//
void CMceIceDirector::UpdateOfferL( CSdpDocument& aOffer )
	{
	UpdateAddressL( aOffer, *iLocalAddressWatcher );
	}


// -----------------------------------------------------------------------------
// CMceIceDirector::UpdateAnswerL
// Set public remote address. This is MO case or MT pull mode case.
// -----------------------------------------------------------------------------
//
void CMceIceDirector::UpdateAnswerL( CSdpDocument& aAnswer )
	{
	UpdateAddressL( aAnswer, *iRemoteAddressWatcher );
	}


// -----------------------------------------------------------------------------
// CMceIceDirector::UpdateAddressL
// Ip-address is put to c-line and port to all m-lines.
// -----------------------------------------------------------------------------
//
void CMceIceDirector::UpdateAddressL( CSdpDocument& aSdpDocument,
									  const CMceIcePropertyWatcher& aProperty )
	{
	TInetAddr publicAddress;
	if ( aProperty.PropertyValue( publicAddress ) == KErrNone )
		{
		TBuf< KPropertyTextValueMaxLength > addressBuf;
		publicAddress.Output( addressBuf );
		TUint port = publicAddress.Port();
		RDebug::Print( _L( "ICEPlugin Dummy: UpdateAddressL ip=%S port=%d" ),
					   &addressBuf, port );

		CSdpConnectionField* conn = aSdpDocument.ConnectionField();
		if ( conn )
			{
			conn->SetInetAddressL( publicAddress );
			}

		RPointerArray< CSdpMediaField >& media = aSdpDocument.MediaFields();
		for ( TInt i = 0; i < media.Count(); ++i )
			{
			media[ i ]->SetPortL( port );
			}
		}
	}


// -----------------------------------------------------------------------------
// CMceIceDirector::MakeOfferReadyCallback
// -----------------------------------------------------------------------------
//
void CMceIceDirector::MakeOfferReadyCallback( CMceIceDirector& aIceDirector )
	{
	// Ownership of iOffer is transferred
	CSdpDocument* offer = aIceDirector.iOffer;
	aIceDirector.iOffer = NULL;
	aIceDirector.iSessionOb->OfferReady( aIceDirector.iSessionId, offer );
	}


// -----------------------------------------------------------------------------
// CMceIceDirector::MakeAnswerReadyCallback
// -----------------------------------------------------------------------------
//
void CMceIceDirector::MakeAnswerReadyCallback( CMceIceDirector& aIceDirector )
	{
	// Ownership of iAnswer is transferred
	CSdpDocument* answer = aIceDirector.iAnswer;
	aIceDirector.iAnswer = NULL;
    aIceDirector.iSessionOb->AnswerReady( aIceDirector.iSessionId, answer );
	}

// end of file
