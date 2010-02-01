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
* Description:    Implementation.
*
*/




#include "TesterConstants.h"
#include "tcuid.h"
#include "TcLog.h"
#include <flogger.h>
#include "CTcCTRLCodec.h"

#include <sip.h>

#include "CTcMCEContext.h"
#include "MCEConstants.h"
#include "TTcMceCommandBase.h"
#include "CTcMCEFactory.h"
#include "CTcMCEReceiveQueue.h"
#include "CTcMCEReceived.h"
#include "CTcMCEProfileContainer.h"
#include "CTcMCEMultimediaAPIObserver.h"
#include "CTcMCEFloorControlObserver.h"
#include "CTcMCEEventAPIObserver.h"

#include "CommandIndex.h"


const TUid KUidMCEtestUI = { 0x10203355 }; // UID of the ECOM implementation

CTcMCEContext* CTcMCEContext::NewLC( CTcCTRLCodec& aCodec,
									 TInt aIAPId )
	{
	CTcMCEContext* self = new( ELeave ) CTcMCEContext( aCodec );

	CleanupStack::PushL( self );
	self->ConstructL( aIAPId );

	return self;
	}

CTcMCEContext::~CTcMCEContext()
	{	
	
	delete iSipProfile;
	
    // Deleting manager before session and events should be possible.
    // At this moment (29.8.2005) it is not, so we have to clean up
    // the object registry before deleting MCEManager. So next line is 
    // not needed, when MMCE is fixed.
    iRegistry.DestroyAll(); 

	delete iFCObserver;
	delete iFC;
		
	delete iEventObserver;
	delete iMMObserver;
	delete iMCEManager; 
    	
	delete iSip;

	}

CTcMCEContext::CTcMCEContext( CTcCTRLCodec& aCodec )
	: CTcContextBase( aCodec )
	{
	}

void CTcMCEContext::ConstructL( TInt /*aIAPId*/ )
	{

    //iIAPId = aIAPId;
    iSip = CSIP::NewL( TUid::Null(), *this ); 
	iSipProfile = CTcMCEProfileContainer::NewL( *this, *iSip );

	
	iMCEManager = CMceManager::NewL( KUidMCEtestUI, &iDataContainer );	
	
	iMMObserver = CTcMCEMultimediaAPIObserver::NewL( *this );
	iMCEManager->SetSessionObserver( iMMObserver );
	iMCEManager->SetInSessionObserver( iMMObserver );
	iMCEManager->SetMediaObserver( iMMObserver );
	iMCEManager->SetRtpObserver( iMMObserver );
	iMCEManager->SetTransactionObserver( iMMObserver );
	iMCEManager->SetInTransactionObserver( iMMObserver );

	iEventObserver = CTcMCEEventAPIObserver::NewL( *this );
	iMCEManager->SetEventObserver( iEventObserver );
	iMCEManager->SetInEventObserver( iEventObserver );
	iMCEManager->SetReferObserver( iEventObserver);
	iMCEManager->SetInReferObserver( iEventObserver );
	
	iFCObserver = CTcMCEFloorControlObserver::NewL( *this );
	iFC = CMceFcMsgExchange::NewL( *iMCEManager, KFCTypeTBCP );

	}

// -- CTcContextBase functions ------------------------------------------------

TTcCommandBase* CTcMCEContext::CreateCommandL( TTcIdentifier& aId,
											   MTcTestContext& aContext )
	{
	// Use base class to actually create commands. Just provide the command
	// array and its size. This creates a command instance.
	return CTcContextBase::CreateCommandL( KTcSscCommands,
										   sizeof( KTcSscCommands ),
										   aId, aContext );
	}


// -- Received queue handling -------------------------------------------------


CTcMCEReceived& CTcMCEContext::ReceivedSessionItemL( TInt aTimeout )
	{
	return iMMObserver->ReceivedSessionItemL( aTimeout );	
	}

CTcMCEReceived& CTcMCEContext::ReceivedUpdatedSessionItemL( TInt aTimeout )
	{
	return iMMObserver->ReceivedUpdatedSessionItemL( aTimeout );	
	}

CTcMCEReceived& CTcMCEContext::ReceivedStreamItemL( TInt aTimeout )
	{
	return iMMObserver->ReceivedStreamItemL( aTimeout );	
	}

CTcMCEReceived& CTcMCEContext::ReceivedRTPItemL( TInt aTimeout )
	{
	return iMMObserver->ReceivedRTPItemL( aTimeout );	
	}	

CTcMCEReceived& CTcMCEContext::ReceivedEventItemL( TInt aTimeout )
	{
	return iEventObserver->ReceivedEventItemL( aTimeout );	
	}

CTcMCEReceived& CTcMCEContext::ReceivedFCItemL( TInt aTimeout )
	{
	return iFCObserver->ReceivedFCItemL( aTimeout );	
	}
	
	
// -- from SIPObserver ---------------------------------------------------------

void CTcMCEContext::IncomingRequest(
			TUint32 /*aIapId*/,
			CSIPServerTransaction* /*aTransaction*/)
	{
		// NOP, dummy
	}

void CTcMCEContext::TimedOut(CSIPServerTransaction& /*aTransaction*/)
    {
    	// NOP, dummy
    }   	
