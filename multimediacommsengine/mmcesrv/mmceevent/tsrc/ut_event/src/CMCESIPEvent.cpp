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




#include <sipmessageelements.h>
#include <sdpdocument.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include "mcesipevent.h"
#include "mcecomevent.h"
#include "sipservertransaction.h"
#include "SIPRequestElements.h"
#include "MCEEventTestConsts.h"

_LIT(KTestError, "testerror");
// -----------------------------------------------------------------------------
// CMceSipEvent::NewL
// -----------------------------------------------------------------------------
//
CMceSipEvent* CMceSipEvent::NewL( CMceSipConnection& aConnection,
                                  MMCETestEventObserver& aObserver )
	{
    CMceSipEvent* self = CMceSipEvent::NewLC( aConnection, aObserver );
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::NewLC
// -----------------------------------------------------------------------------
//
CMceSipEvent* CMceSipEvent::NewLC ( CMceSipConnection& aConnection,
                                    MMCETestEventObserver& aObserver )
	{
    CMceSipEvent* self = new (ELeave) CMceSipEvent( aConnection, aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::CMceSipEvent
// -----------------------------------------------------------------------------
//
CMceSipEvent::CMceSipEvent ( CMceSipConnection& aConnection, 
                             MMCETestEventObserver& aObserver )
: iConnection( aConnection ), iObserver( aObserver )
	{


	}

// -----------------------------------------------------------------------------
// CMceSipEvent::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ConstructL ()
	{
	iDefaultHeaders = new( ELeave ) CDesC8ArrayFlat( 1 );
	iEvent = CMceComEvent::NewL( *this, KDefaultExpires );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::~CMceSipEvent
// -----------------------------------------------------------------------------
//
CMceSipEvent::~CMceSipEvent()
	{
	delete iDefaultHeaders;
    delete iEvent;		
    if ( iPendingTransactions.Count())
	    {
    	iPendingTransactions.ResetAndDestroy();
	    }
	iPendingTransactions.Close();
	if(iDialog)
		{
		delete iDialog;	
		}
    if ( iRequest )
        {
        delete iRequest;
        }
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::SIPConnection
// -----------------------------------------------------------------------------
//
CMceSipConnection& CMceSipEvent::SIPConnection() const
    {
    return iConnection;
    }


// -----------------------------------------------------------------------------
// CMceSipEvent::CurrentState
// -----------------------------------------------------------------------------
//
TBool CMceSipEvent::IsPending()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::HandleSIPEvent
// -----------------------------------------------------------------------------
//
void CMceSipEvent::HandleSIPEvent(  )
    {
    TRAP_IGNORE( iEvent->ProceedL() );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ResponseReceived()
    {
    HandleSIPEvent( );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ProvisionalResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ProvisionalResponseReceived()
    {
    HandleSIPEvent( );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::RedirectionResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::RedirectionResponseReceived()
    {
    HandleSIPEvent( );
    }
 
// -----------------------------------------------------------------------------
// CMceSipEvent::ErrorResponseReceived
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ErrorResponseReceived()
    {
    HandleSIPEvent( );
    }





// -----------------------------------------------------------------------------
// CMceSipEvent::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ErrorOccured( TInt aError )
    {
	ErrorOccured(aError);
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ErrorOccured( TInt aError, CSIPTransactionBase& /*aTransaction*/ )
    {
    ErrorOccured(aError);
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::ConnectionStateChanged
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ConnectionStateChanged( CSIPConnection::TState aState )
    {
    if ( iEvent )
        {
    if ( aState == CSIPConnection::ESuspended ||
                  aState == CSIPConnection::EInactive ||
                  aState == CSIPConnection::EUnavailable )
            {
            TRAPD (err , iEvent->PerformActionL(EMceItcTerminateEvent));           
            }
        }
    }
    

// -----------------------------------------------------------------------------
// CMceSipEvent::Canceled
// -----------------------------------------------------------------------------
//
void CMceSipEvent::Canceled()
    {
    }


// -----------------------------------------------------------------------------
// CMceSipEvent::ServiceL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::ServiceL( )
    {    
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::EventRequestedL
// -----------------------------------------------------------------------------
//    
void CMceSipEvent::EventRequestedL( CSIPServerTransaction* aTransaction )
    {
    if(aTransaction->Type()==SIPStrings::StringF(SipStrConsts::ESubscribe))
    	{
    	SetEventStateType(EMtSubscribe );		
    	}
    else
    	{
    	SetEventStateType(EMtRefer );		
    	}
    iEvent = CMceComEvent::NewL( *this,
                				NULL );
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::IncomingSubscribe
// -----------------------------------------------------------------------------
//
void CMceSipEvent::IncomingSubscribeL(CDesC8Array* aHeaders,
                    				HBufC8* aContentType,
									HBufC8* aBody)
	{
	iObserver.IncomingSubscribeL(aHeaders, aContentType, aBody);
	}
					
// -----------------------------------------------------------------------------
// CMceSipEvent::IncomingRefer
// -----------------------------------------------------------------------------
//									
void CMceSipEvent::IncomingReferL(HBufC8* aReferTo,
					CDesC8Array* aHeaders,
                    HBufC8* aContentType,
					HBufC8* aBody )
	{
	iObserver.IncomingReferL(aReferTo, aHeaders, aContentType, aBody);
	}
	
// -----------------------------------------------------------------------------
// CMceSipEvent::ClientStateChangedL
// -----------------------------------------------------------------------------
//									
void CMceSipEvent::ClientStateChangedL( TUint32 aState, TBool /*aIsResponse*/ )
	{
	iObserver.EventStateChangedL(aState);
	}
		
// -----------------------------------------------------------------------------
// CMceSipEvent::NotifyReceived
// -----------------------------------------------------------------------------
//				
void CMceSipEvent::NotifyReceivedL(CDesC8Array* aHeaders,
                    				HBufC8* aContentType,
									HBufC8* aBody )
	{
	iObserver.NotifyReceivedL(aHeaders, aContentType, aBody);
	}
		

// -----------------------------------------------------------------------------
// CMceSipEvent::SetEventStateType
// -----------------------------------------------------------------------------
// 
void CMceSipEvent::SetEventStateType(const TEventStateType aEventStateType )
	{
	iStateType= aEventStateType;	
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::SetResponseL
// -----------------------------------------------------------------------------
//
void CMceSipEvent::SetResponseL( CSIPClientTransaction& aTransaction )
    {
    TInt index = iPendingTransactions.Find( &aTransaction );
    __ASSERT_ALWAYS( index >= 0, User::Leave( KErrGeneral ) );
    
    if ( iResponse && iResponse != &aTransaction )
        {
        delete iResponse;
        iResponse = NULL;
        }

    iResponse = &aTransaction;
    
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::EventStateType
// -----------------------------------------------------------------------------
// 	
TEventStateType CMceSipEvent::EventStateType() const
	{
	return iStateType;	
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::Dialog
// -----------------------------------------------------------------------------
//
CSIPDialogAssocBase* CMceSipEvent::Dialog() const
    {
    
    return iDialog;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::SetDialog
// -----------------------------------------------------------------------------
//
void CMceSipEvent::SetDialog( CSIPDialogAssocBase* aDialog )
    {
    iDialog = aDialog;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent:Request
// -----------------------------------------------------------------------------
//
CSIPServerTransaction& CMceSipEvent::Request() 
    {
    return *iRequest;
    }
 
// -----------------------------------------------------------------------------
// CMceSipEvent::Response
// -----------------------------------------------------------------------------
//
CSIPClientTransaction& CMceSipEvent::Response() 
    {
    return *iResponse;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::SetPendingTransaction
// -----------------------------------------------------------------------------
//
void CMceSipEvent::SetPendingTransactionL( CSIPClientTransaction* aTransaction )
    {
    User::LeaveIfError( iPendingTransactions.Append( aTransaction ) );
    }
// -----------------------------------------------------------------------------
// CMceSipEvent::PendingTransactions
// -----------------------------------------------------------------------------
//
const RPointerArray<CSIPClientTransaction>& CMceSipEvent::PendingTransactions()
    {
    return iPendingTransactions;
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::SetCurrentState
// -----------------------------------------------------------------------------
//
void CMceSipEvent::SetCurrentState( TInt aIndex )
    {
    iEvent->EventContext().iCurrentState = (*(iEvent->EventContext().iStates))[aIndex];
    }

// -----------------------------------------------------------------------------
// CMceSipEvent::DefaultHeaders
// -----------------------------------------------------------------------------
//
CDesC8Array& CMceSipEvent::DefaultHeaders( TInt /*aMethodInd*/ )
    {
    return *iDefaultHeaders;
    }

