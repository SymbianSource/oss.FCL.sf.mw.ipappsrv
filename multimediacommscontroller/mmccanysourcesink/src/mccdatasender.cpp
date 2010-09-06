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
#include "mccdatasender.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccDataSender::CMccDataSender
// -----------------------------------------------------------------------------
//
CMccDataSender::CMccDataSender( )
	: CActive( EPriorityStandard ), iReady( EFalse )
	{
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CMccDataSender::ConstructL
// -----------------------------------------------------------------------------
//
void CMccDataSender::ConstructL()
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataSender::ConstructL" )
            
    TBuf16<30> aMessageBuffer(_L("SendQueueForMCCMSRP"));
    
    TInt aNo = AssignNameToMessageQueueL(aMessageBuffer);
    aMessageBuffer.AppendNum(aNo);
       
    
    TInt err = iSendMsgQueue.CreateGlobal(aMessageBuffer, 5, EOwnerProcess );
    if ( err )
        {
        User::LeaveIfError( iSendMsgQueue.OpenGlobal(aMessageBuffer));
        }
    }

// -----------------------------------------------------------------------------
// CMccDataSender::NewL
// -----------------------------------------------------------------------------
//
CMccDataSender* CMccDataSender::NewL( )
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataSender::NewL" )
    CMccDataSender* self = 
    	new ( ELeave ) CMccDataSender();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

TInt CMccDataSender::AssignNameToMessageQueueL(const TDes16 &aMessageBuffer)
    {
        __ANYSOURCESINK_CONTROLL( "CMccDataSender::AssignNameToMessageQueueL, Entry" )
    for(TInt i =1; i<=10; i++)
        {
        TBuf16<30> tempMessage1 = aMessageBuffer;
        tempMessage1.AppendNum(i);
        TInt err1 = iSendMsgQueue1.CreateGlobal(tempMessage1, 5, EOwnerProcess );
        
        if(err1 == KErrNone)
            {
            TBuf16<30> tempMessage2 = aMessageBuffer;
            tempMessage2.AppendNum(i+1);
            TInt err2 = iSendMsgQueue2.CreateGlobal(tempMessage2, 5, EOwnerProcess );
            
            if(err2 == KErrNone)
                {
                iSendMsgQueue1.Close();
                iSendMsgQueue2.Close();
                __ANYSOURCESINK_CONTROLL_INT1( "CMccDataSender::AssignNameToMessageQueueL", i )
                return i;
                }
            else
                {
                iSendMsgQueue1.Close();
                }
            }
        }    
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccDataSender::Send
// -----------------------------------------------------------------------------
//
void CMccDataSender::Send( const TDesC8& aData )
	{
    __ANYSOURCESINK_CONTROLL( "CMccDataSender::Send" )
	TQueueBuffer buffer( aData );
	
    TInt err = iSendMsgQueue.Send( buffer );
    
    if ( err == KErrOverflow )
        {
        if ( !IsActive() )
            {
            iBuffer.Copy( aData );            
            iSendMsgQueue.NotifySpaceAvailable( iStatus );    
    	    SetActive();
            }
        else
            {
            // TODO
            // BUFFER OR DROP
            __ANYSOURCESINK_CONTROLL( "CMccDataSender::Send, Dropping Packet" )
            }
        }
	}
	
// -----------------------------------------------------------------------------
// CMccDataSender::~CMccDataSender
// -----------------------------------------------------------------------------
//
CMccDataSender::~CMccDataSender()
    {
    __ANYSOURCESINK_CONTROLL( "CMccDataSender::~CMccDataSender" )
    Cancel();
	iSendMsgQueue.Close();
    }

// -----------------------------------------------------------------------------
// CMccDataSender::DoCancel
// -----------------------------------------------------------------------------
//
void CMccDataSender::DoCancel()
	{
    __ANYSOURCESINK_CONTROLL( "CMccDataSender::DoCancel" )
	iSendMsgQueue.CancelSpaceAvailable();	
	}

// -----------------------------------------------------------------------------
// CMccDataSender::RunL
// -----------------------------------------------------------------------------
//	
void CMccDataSender::RunL()
	{
    __ANYSOURCESINK_CONTROLL( "CMccDataSender::RunL" )
    iSendMsgQueue.Send( iBuffer );
    iBuffer.Zero();
    }

// -----------------------------------------------------------------------------
// CMccDataSender::RunError
// -----------------------------------------------------------------------------
//	
TInt CMccDataSender::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
