/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mccanysourcesinklogs.h"
#include "mccdatareceiver.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccDataReceiver::CMccDataReceiver
// -----------------------------------------------------------------------------
//
CMccDataReceiver::CMccDataReceiver( MMccDataReceiverObserver& aObserver )
	: CActive( EPriorityStandard ), iObserver( &aObserver )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMccDataReceiver::ConstructL
// -----------------------------------------------------------------------------
//
void CMccDataReceiver::ConstructL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataReceiver::ConstructL" )
            
    TBuf16<30> aMessageBuffer(_L("RecvQueueForMCCMSRP"));
    
    TInt aNo = AssignNameToMessageQueueL(aMessageBuffer);
    aMessageBuffer.AppendNum(aNo);
    
    User::LeaveIfError(iMsgQueue.CreateGlobal(aMessageBuffer, 5, EOwnerProcess ));
    
    iMsgQueue.NotifyDataAvailable( iStatus );
    SetActive();

    }

TInt CMccDataReceiver::AssignNameToMessageQueueL(const TDes16 &aMessageBuffer)
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataReceiver::AssignNameToMessageQueueL" )
            
        
    for(TInt i =1; i<=10; i++)
        {
        TBuf16<30> tempMessage1 = aMessageBuffer;
        tempMessage1.AppendNum(i);
        TInt err1 = iMsgQueue1.CreateGlobal(tempMessage1, 5, EOwnerProcess );
        
        if(err1 == KErrNone)
            {
            TBuf16<30> tempMessage2 = aMessageBuffer;
            tempMessage2.AppendNum(i+1);
            TInt err2 = iMsgQueue2.CreateGlobal(tempMessage2, 5, EOwnerProcess );
            
            if(err2 == KErrNone)
                {
                iMsgQueue1.Close();
                iMsgQueue2.Close();
                __ANYSOURCESINK_CONTROLL_INT1( "CMccDataReceiver::AssignNameToMessageQueueL", i )
                return i;
                }
            else
                {
                iMsgQueue1.Close();
                }
            }
        }    
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccDataReceiver::NewL
// -----------------------------------------------------------------------------
//
CMccDataReceiver* CMccDataReceiver::NewL( MMccDataReceiverObserver& aObserver )
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataReceiver::NewL" )
    CMccDataReceiver* self = 
    	new ( ELeave ) CMccDataReceiver( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
	
// -----------------------------------------------------------------------------
// CMccDataReceiver::~CMccDataReceiver
// -----------------------------------------------------------------------------
//
CMccDataReceiver::~CMccDataReceiver()
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataReceiver::~CMccDataReceiver" )
    Cancel();
	iMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMccDataReceiver::DoCancel
// -----------------------------------------------------------------------------
//
void CMccDataReceiver::DoCancel()
	{
    __ANYSOURCESINK_CONTROLL( "CMccDataReceiver::DoCancel" )
	iMsgQueue.CancelDataAvailable();	
	}

// -----------------------------------------------------------------------------
// CMccDataReceiver::RunL
// -----------------------------------------------------------------------------
//	
void CMccDataReceiver::RunL()
	{
    __ANYSOURCESINK_CONTROLL( "CMccDataReceiver::RunL" )
	TQueueBuffer buffer;
	iMsgQueue.Receive( buffer );
	iObserver->DataReceivedL( buffer );
    iMsgQueue.NotifyDataAvailable( iStatus );
    SetActive();
	}

// -----------------------------------------------------------------------------
// CMccDataReceiver::RunError
// -----------------------------------------------------------------------------
//	
TInt CMccDataReceiver::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
