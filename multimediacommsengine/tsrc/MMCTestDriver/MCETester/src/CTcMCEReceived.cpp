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



#include "CTcMCEReceived.h"
#include <MceInSession.h>
#include <MceInEvent.h>
#include <MceInRefer.h>
#include <MCETransactionDataContainer.h>

CTcMCEReceived::CTcMCEReceived():
	iReferTo(KNullDesC8)
	{	
	}

CTcMCEReceived::~CTcMCEReceived()
	{
	delete iHeaders;
	delete iContentType;
	delete iBody;
	delete iReasonPhrase;
	delete iFCMessage;
	delete iInEvent;
	delete iInSession;
	delete iInRefer;
	delete iMethod;
	}

void CTcMCEReceived::SetSession( const CMceSession* aSession )
	{
	iSession = aSession;
	}

void CTcMCEReceived::SetMediaStream( const CMceMediaStream* aMediaStream )
	{
	iMediaStream = aMediaStream;
	}

void CTcMCEReceived::SetMediaSource( const CMceMediaSource* aMediaSource )
	{
	iMediaSource = aMediaSource;
	}

void CTcMCEReceived::SetMediaSink( const CMceMediaSink* aMediaSink )
	{
	iMediaSink = aMediaSink;
	}
	
void CTcMCEReceived::SetRtpSource( const CMceRtpSource* aRtpSource )
	{
	iRtpSource = aRtpSource;
	}

void CTcMCEReceived::SetInSession( CMceInSession* aInSession )
	{
	iInSession = aInSession;
	}

void CTcMCEReceived::SetIncomingUpdateSession( CMceSession& /*aOrigSession*/,
											CMceInSession* aInSession )
	{
	if( iInSession)
		{
		delete iInSession; iInSession = NULL;
		}
	iInSession = aInSession;		
	}
	
void CTcMCEReceived::SetHeaders( CDesC8Array* aHeaders ) 
	{
	delete iHeaders;
	iHeaders = aHeaders;
	}
	
void CTcMCEReceived::SetContentType( HBufC8* aContentType )  
	{
	delete iContentType;
	iContentType = aContentType;
	}
	
void CTcMCEReceived::SetBody( HBufC8* aBody ) 
	{
	delete iBody;
	iBody = aBody;
	}
	
void CTcMCEReceived::SetStatusCode( TInt32 aStatusCode ) 
	{
	iStatusCode = aStatusCode;
	}

void CTcMCEReceived::SetReasonPhrase( HBufC8* aReasonPhrase ) 
	{
	delete iReasonPhrase;
	iReasonPhrase = aReasonPhrase;
	}

void CTcMCEReceived::SetFCMessage( HBufC8* aFCMessage )
	{
	delete iFCMessage;
	iFCMessage = aFCMessage;	
	}	

void CTcMCEReceived::SetEvent( const CMceEvent* aEvent )
	{
	iEvent = aEvent;
	}

void CTcMCEReceived::SetInEvent( CMceInEvent* aInEvent )
	{
	delete iInEvent;
	iInEvent = aInEvent;
	}

void CTcMCEReceived::SetRefer( CMceRefer* aRefer )
    {
    iRefer = aRefer;
    }

void CTcMCEReceived::SetInRefer( CMceInRefer* aInRefer )
    {
    delete iInRefer;
    iInRefer = aInRefer;
    }

void CTcMCEReceived::SetReferTo( const TDesC8& aReferTo )
	{
	iReferTo = aReferTo;
	}

void CTcMCEReceived::SetError( TInt aError )
	{
	iError = aError;
	}

void CTcMCEReceived::SetMethodL( const TDesC8& aMethod )
	{
	HBufC8* method = aMethod.AllocL();
	delete iMethod;
	iMethod = method;
	}
			
void CTcMCEReceived::SetTransactionId( TMceTransactionId aTransactionId )
	{
	iTransactionId = aTransactionId;
	}

void CTcMCEReceived::SetSessionState(CMceSession::TState aState )
	{
	iSessionState = aState;
	}
	
void CTcMCEReceived::SetEventState(CMceEvent::TState aState )
	{
	iEventState = aState;
	}

void CTcMCEReceived::SetReferState(CMceRefer::TState aState )
	{
	iReferState = aState;
	}

void CTcMCEReceived::SetStreamState(CMceMediaStream::TState aState )
	{
	iStreamState = aState;
	}

void CTcMCEReceived::SetTransactionData( 
        TMceTransactionDataContainer* aContainer )
    {
	if ( aContainer )
	    {
	    SetStatusCode( aContainer->GetStatusCode() );
	    SetReasonPhrase( aContainer->GetReasonPhrase() );
		SetHeaders( aContainer->GetHeaders() );
		SetContentType( aContainer->GetContentType() );
		SetBody( aContainer->GetContent() );
	    }    
    }
