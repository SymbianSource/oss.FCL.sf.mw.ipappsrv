/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/

        
// INCLUDE FILES
#include "mccmsrpdatasender.h"
#include "mccmsrpsourcesinklogs.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::CMccMsrpDataSender
// -----------------------------------------------------------------------------
//
CMccMsrpDataSender::CMccMsrpDataSender( )
	: CActive( EPriorityStandard )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::ConstructL
// -----------------------------------------------------------------------------
//
void CMccMsrpDataSender::ConstructL()
    {
    iTimeoutTimer = CMccTimerManager::NewL();
    }

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::NewL
// -----------------------------------------------------------------------------
//
CMccMsrpDataSender* CMccMsrpDataSender::NewL( )
    {
    CMccMsrpDataSender* self = 
    	new ( ELeave ) CMccMsrpDataSender();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::Send
// -----------------------------------------------------------------------------
//
void CMccMsrpDataSender::SendL( const TDesC8& aData, CMSRPSession* aMsrpSession )
	{	
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpDataSender::Send, Entry" )
    __ASSERT_ALWAYS( aMsrpSession, User::Leave( KErrArgument ) );
	
    if ( !IsActive() )
        {
        iBuffer.Copy( aData );
        SendPacketL(aMsrpSession);
        __MSRPSOURCESINK_CONTROLL( "CMccMsrpDataSender::Send, Exit" )
        }
	}


// -----------------------------------------------------------------------------
// CMccRtpSender::SendPacketL
// -----------------------------------------------------------------------------
//
void CMccMsrpDataSender::SendPacketL( CMSRPSession* aMsrpSession )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpDataSender::SendPacketL, entry" )
    __ASSERT_ALWAYS( aMsrpSession, User::Leave( KErrArgument ) );
    
    if ( iBuffer.Size() )
        {
        __MSRPSOURCESINK_CONTROLL( "CMccMsrpDataSender::SendBuffer, entry" )
        HBufC8* messageID = aMsrpSession->SendBuffer(iBuffer, _L8("text/plain"));
        // message Id might be useful for the file sharing use case
        delete messageID;
        }
        
        iTimeoutTimer->Stop( iTimerId );
        iTimerId = iTimeoutTimer->StartL( this, iTimeoutTimeMilliseconds );
        
    }

	
// -----------------------------------------------------------------------------
// CMccMsrpDataSender::~CMccMsrpDataSender
// -----------------------------------------------------------------------------
//
CMccMsrpDataSender::~CMccMsrpDataSender()
    {
    Cancel();
    
    delete iTimeoutTimer;
	iMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::DoCancel
// -----------------------------------------------------------------------------
//
void CMccMsrpDataSender::DoCancel()
	{
	iMsgQueue.CancelSpaceAvailable();	
	}

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::RunL
// -----------------------------------------------------------------------------
//	
void CMccMsrpDataSender::RunL()
	{
    iMsgQueue.Send( iBuffer );
    iBuffer.Zero();
    }

// -----------------------------------------------------------------------------
// CMccMsrpDataSender::RunError
// -----------------------------------------------------------------------------
//	
TInt CMccMsrpDataSender::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CMccRtpSender::InactivityTimeout
// -----------------------------------------------------------------------------
//    
void CMccMsrpDataSender::TimerExpiredL( 
    TMccTimerId /*aTimerId*/, TAny* /*aTimerParam*/ )
    {
    
/*    if ( iTimeoutTimeMilliseconds > KMccRtpSendTimeoutLongMillisecs )
        {
        // Timeout value is so long that if sending of one rtp packet really
        // took so long, receiver will have big problems and it's better
        // to drop all frames queued in order to avoid delay increase 
        // at receiver side.
        Clear();
        }
    else
        {
        // Sending timeout, cancel send and discard that packet
        Cancel();
        
        // Do also cleanup of related packets as those are useless as this 
        // packet is removed because of timeout
        RemovePacket( ETrue ); 
        
        // Send next packet anyway
        TRAPD( err, SendPacketL() )
        if ( err )
            {
            ErrorReport( err );
            }
        } */
    
    }
