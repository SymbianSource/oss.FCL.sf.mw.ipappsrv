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



// INCLUDE FILES
#include "mceclilogs.h"
#include "mcedatasender.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceDataSender::CMceDataSender
// -----------------------------------------------------------------------------
//
CMceDataSender::CMceDataSender( )
	: CActive( EPriorityStandard ), iReady( EFalse )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMceDataSender::ConstructL
// -----------------------------------------------------------------------------
//
void CMceDataSender::ConstructL()
    {
    MCECLI_DEBUG("CMceDataSender::ConstructL, Entry");
    
    TBuf16<30> aMessageBuffer(_L("RecvQueueForMCCMSRP"));
    
    TInt aNo = AssignNameToMessageQueueL(aMessageBuffer);
    aMessageBuffer.AppendNum(aNo);
    
    User::LeaveIfError(iMsgQueue.OpenGlobal(aMessageBuffer));
    MCECLI_DEBUG("CMceDataSender::ConstructL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceDataSender::NewL
// -----------------------------------------------------------------------------
//
CMceDataSender* CMceDataSender::NewL( )
    {
    CMceDataSender* self = 
    	new ( ELeave ) CMceDataSender();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceDataSender::Send
// -----------------------------------------------------------------------------
//
void CMceDataSender::Send( const TDesC8& aData )
	{	
	TQueueBuffer buffer( aData );
	
    TInt err = iMsgQueue.Send( buffer );
    
    if ( err == KErrOverflow )
        {
        if ( !IsActive() )
            {
            iBuffer.Copy( aData );            
            iMsgQueue.NotifySpaceAvailable( iStatus );    
    	    SetActive();
            }
        else
            {
            // TODO
            // BUFFER OR DROP
            }
        }
	}

TInt CMceDataSender::AssignNameToMessageQueueL(const TDes16 &aMessageBuffer)
    {
    MCECLI_DEBUG("CMceDataSender::AssignNameToMessageQueueL, Entry");
    // queue to be opened for Sending Data
    for(TInt i=1; i<=10; i++)
        {
        TBuf16<30> tempMessage1 = aMessageBuffer;
        tempMessage1.AppendNum(i);
        TInt err1 = iMsgQueue1.CreateGlobal(tempMessage1, 5, EOwnerProcess );
        if(err1 == KErrAlreadyExists)
            {
            TBuf16<30> tempMessage2 = aMessageBuffer;
            tempMessage2.AppendNum(i+1);
            TInt err2 = iMsgQueue2.CreateGlobal(tempMessage2, 5, EOwnerProcess );
            if(err2 == KErrNone)
                {
                iMsgQueue2.Close();
                MCECLI_DEBUG_DVALUE("CMceDataSender::AssignNameToMessageQueueL, Entry", i);
                return i;
                }
            }
        else
            {
            iMsgQueue1.Close();
            }
        }
    return 0;
    }
	
// -----------------------------------------------------------------------------
// CMceDataSender::~CMceDataSender
// -----------------------------------------------------------------------------
//
CMceDataSender::~CMceDataSender()
    {
    Cancel();
	iMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMceDataSender::DoCancel
// -----------------------------------------------------------------------------
//
void CMceDataSender::DoCancel()
	{
	iMsgQueue.CancelSpaceAvailable();	
	}

// -----------------------------------------------------------------------------
// CMceDataSender::RunL
// -----------------------------------------------------------------------------
//	
void CMceDataSender::RunL()
	{
    iMsgQueue.Send( iBuffer );
    iBuffer.Zero();
    }

// -----------------------------------------------------------------------------
// CMceDataSender::RunError
// -----------------------------------------------------------------------------
//	
TInt CMceDataSender::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
