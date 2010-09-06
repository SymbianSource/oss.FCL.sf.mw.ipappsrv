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
#include "mcedatareceiver.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMceDataReceiver::CMceDataReceiver
// -----------------------------------------------------------------------------
//
CMceDataReceiver::CMceDataReceiver( MMceDataSinkObserver& aObserver )
    : CActive( EPriorityStandard ), iReady( EFalse ), iObserver( &aObserver )
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CMceDataReceiver::ConstructL
// -----------------------------------------------------------------------------
//
void CMceDataReceiver::ConstructL()
    {
    MCECLI_DEBUG("CMceDataReceiver::ConstructL, Entry");
    
    TBuf16<30> aMessageBuffer(_L("SendQueueForMCCMSRP"));
    
    TInt aNo = AssignNameToMessageQueueL(aMessageBuffer);
    aMessageBuffer.AppendNum(aNo);
    
    User::LeaveIfError( iSendMsgQueue.OpenGlobal(aMessageBuffer));
        
    iSendMsgQueue.NotifyDataAvailable( iStatus );
    SetActive();
    }


TInt CMceDataReceiver::AssignNameToMessageQueueL(const TDes16 &aMessageBuffer)
    {
    MCECLI_DEBUG( "CMccDataReceiver::AssignNameToMessageQueueL" )
            
    
    // queue to be opened for Sending Data
    for(TInt i=1; i<=10; i++)
        {
        TBuf16<30> tempMessage1 = aMessageBuffer;
        tempMessage1.AppendNum(i);
        TInt err1 = iSendMsgQueue1.CreateGlobal(tempMessage1, 5, EOwnerProcess );
        if(err1 == KErrAlreadyExists)
            {
            TBuf16<30> tempMessage2 = aMessageBuffer;
            tempMessage2.AppendNum(i+1);
            TInt err2 = iSendMsgQueue2.CreateGlobal(tempMessage2, 5, EOwnerProcess );
            if(err2 == KErrNone)
                {
                iSendMsgQueue2.Close();
                MCECLI_DEBUG_DVALUE( "CMccDataReceiver::AssignNameToMessageQueueL" , i )
                return i;
                }
            }
        else
            {
            iSendMsgQueue1.Close();
            }
        }
    return 0;
            

    }

// -----------------------------------------------------------------------------
// CMceDataReceiver::NewL
// -----------------------------------------------------------------------------
//
CMceDataReceiver* CMceDataReceiver::NewL( MMceDataSinkObserver& aObserver )
    {
    MCECLI_DEBUG("CMceDataReceiver::NewL, Entry");
    CMceDataReceiver* self = 
        new ( ELeave ) CMceDataReceiver( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CMceDataReceiver::~CMceDataReceiver
// -----------------------------------------------------------------------------
//
CMceDataReceiver::~CMceDataReceiver()
    {
    MCECLI_DEBUG("CMceDataReceiver::~CMceDataReceiver, Entry");
    Cancel();
    iSendMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMceDataReceiver::DoCancel
// -----------------------------------------------------------------------------
//
void CMceDataReceiver::DoCancel()
    {
    MCECLI_DEBUG("CMceDataReceiver::DoCancel, Entry");
    iSendMsgQueue.CancelDataAvailable();    
    }

// -----------------------------------------------------------------------------
// CMceDataReceiver::RunL
// -----------------------------------------------------------------------------
//  
void CMceDataReceiver::RunL()
    {
    MCECLI_DEBUG("CMceDataReceiver::RunL, Entry");
    CMceMediaStream* dummy1 = NULL;
    CMceMediaSink* dummy2 = NULL;
    TQueueBuffer buffer;
    iSendMsgQueue.Receive( buffer );
    iObserver->DataReceived( *dummy1, *dummy2, buffer );
    iSendMsgQueue.NotifyDataAvailable( iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CMceDataReceiver::RunError
// -----------------------------------------------------------------------------
//  
TInt CMceDataReceiver::RunError(TInt /*aError*/)
    {
    MCECLI_DEBUG("CMceDataReceiver::RunError, Entry");
    return KErrNone;
    }
