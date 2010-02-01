/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Source for multiplexer, where Media Wrapper is mounted to
*
*/




#include "cncmmediasource.h"
#include "ncmconnectionmultiplexerlogs.h"
#include "mncmmediasourceobserver.h"
#include "natfwmediawrapper.h"
#include "mncmsourceobserver.h"
#include "ncmconnectionmultiplexerassert.h"

// ---------------------------------------------------------------------------
// CNcmMediaSource::CNcmMediaSource
// ---------------------------------------------------------------------------
//
CNcmMediaSource::CNcmMediaSource(
    MNcmMediaSourceObserver& aMediaObserver,
    MNATFWMediaWrapper& aMediaWrapper ) :
    iMediaObserver( aMediaObserver ),
    iMediaWrapper( aMediaWrapper ),
    iReceivingState( EStreamingStateUnknown )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::CNcmMediaSource" )
    }


// ---------------------------------------------------------------------------
// CNcmMediaSource::ConstructL
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::ConstructL( const RSocketServ& aSocketServ,
    const TInetAddr& aLocalAddr )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::ConstructL" )
            
    iMediaWrapper.RegisterMediaWrapperObserverL( this, aSocketServ,
                                                 aLocalAddr );    
    }   


// ---------------------------------------------------------------------------
// CNcmMediaSource::NewL
// ---------------------------------------------------------------------------
//
CNcmMediaSource* CNcmMediaSource::NewL(
    MNcmMediaSourceObserver& aMediaObserver,
    MNATFWMediaWrapper& aMediaWrapper,
    const RSocketServ& aSocketServ,
    const TInetAddr& aLocalAddr )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::NewL" )
    
    CNcmMediaSource* self = CNcmMediaSource::NewLC(
        aMediaObserver, aMediaWrapper, aSocketServ, aLocalAddr );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmMediaSource::NewLC
// ---------------------------------------------------------------------------
//
CNcmMediaSource* CNcmMediaSource::NewLC(
    MNcmMediaSourceObserver& aMediaObserver,
    MNATFWMediaWrapper& aMediaWrapper,
    const RSocketServ& aSocketServ,
    const TInetAddr& aLocalAddr )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::NewLC" )
    
    CNcmMediaSource* self =
        new( ELeave ) CNcmMediaSource( aMediaObserver, aMediaWrapper );
    CleanupStack::PushL( self );
    self->ConstructL( aSocketServ, aLocalAddr );
    return self;
    }


// ---------------------------------------------------------------------------
// CNcmMediaSource::~CNcmMediaSource
// ---------------------------------------------------------------------------
//
CNcmMediaSource::~CNcmMediaSource()
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::~CNcmMediaSource" )
    iMessageObserver = NULL;
    }


// ---------------------------------------------------------------------------
// CNcmMediaSource::RegisterMessageObserver
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::RegisterMessageObserver(
    MNcmSourceObserver& aMessageObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::RegisterMessageObserver" )
    
    iMessageObserver = &aMessageObserver;
    }
 
   
// ---------------------------------------------------------------------------
// CNcmMediaSource::UnregisterMessageObserver
// ---------------------------------------------------------------------------
//   
void CNcmMediaSource::UnregisterMessageObserver(
    const MNcmSourceObserver& aMessageObserver )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::UnRegisterMessageObserver" )
     __CONNECTIONMULTIPLEXER_ASSERT_L(
        &aMessageObserver == iMessageObserver, KErrArgument );
         
    iMessageObserver = NULL;     
    }


// ---------------------------------------------------------------------------
// CNcmMediaSource::SetReceivingState
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::SetReceivingState( TNATFWStreamingState aState )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::SetReceivingState" )

    iReceivingState = aState;
    }

    
// ---------------------------------------------------------------------------
// From class MMultiplexerSourceBase
//
// CNcmMediaSource::PutMessage
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::PutMessageL( const TDesC8& aMessage )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::PutMessage" )
    
    if ( EStreamingStateActive == iReceivingState )
        {
        TRAP_IGNORE( iMediaWrapper.SendL( aMessage ) )
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWMediaWrapperObserver
//
// CNcmMediaSource::SendingCompleted
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::SendingCompleted( TInt aError )
    {
    if ( aError )
        {
        iMediaObserver.WrapperSenderError( iMediaWrapper.StreamId(), aError );
        }
    }


// ---------------------------------------------------------------------------
// From class MNATFWMediaWrapperObserver
//
// CNcmMediaSource::ReceivedFrom
// ---------------------------------------------------------------------------
//
TInt CNcmMediaSource::ReceivedFrom( const TUint /*aStreamId*/,
    TDes8& aReceiveBuffer )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::ReceivedFrom" )
    
    if ( iMessageObserver )
        {
        if ( iFirstMediaPacketIsSent )
            {
            iMessageObserver->OutgoingMessageAvailable(
                aReceiveBuffer, NULL, KAFUnspec );           
            }
        else
            {
            iMessageObserver->OutgoingMessageAvailable(
                aReceiveBuffer, this, KAFUnspec );           
            }
        }

    return KErrNone;
    }

    
// ---------------------------------------------------------------------------
// From class MNATFWMediaWrapperObserver
//
// CNcmMediaSource::GetNewFreePort
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::GetNewFreePort( TUint& aPort )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::GetNewFreePort" )

    iMediaObserver.GetFreePort( iMediaWrapper.StreamId(), aPort );
    }


// ---------------------------------------------------------------------------
// From class MNcmSenderObserver
//
// CNcmMediaSource::MessageSent
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::MessageSent()
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::MessageSent" )
    iFirstMediaPacketIsSent = ETrue;
    iMediaObserver.FirstMediaPacketSent( iMediaWrapper.StreamId() );
    }

    
// ---------------------------------------------------------------------------
// From class MNcmSenderObserver
//
// CNcmMediaSource::MessageSentFailure
// ---------------------------------------------------------------------------
//
void CNcmMediaSource::MessageSentFailure( TInt /*aError*/ )
    {
    __CONNECTIONMULTIPLEXER( "CNcmMediaSource::MessageSentFailure" )
    }
