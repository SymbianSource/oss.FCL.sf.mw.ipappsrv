/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    fcsender.cpp
*
*/




#include <e32base.h>
#include <in_sock.h>
#include "fcsender.h"
#include "fcconnectioncontainer.h"
#include "fcnotifier.h"

const TInt KSendBufferLen = 10;

// -----------------------------------------------------------------------------
// CFCSender::NewL
// -----------------------------------------------------------------------------
//
CFCSender* CFCSender::NewL(RSocket& aSocket, MFCNotifier& aNotifier)
	{
	CFCSender* self = new (ELeave) CFCSender(aSocket, aNotifier);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CFCSender::CFCSender
// -----------------------------------------------------------------------------
//
CFCSender::CFCSender(RSocket& aSocket, MFCNotifier& aNotifier) :
CActive(EPriorityStandard),
iFCNotifier( aNotifier )
	{
	iSocket = &aSocket;
	CActiveScheduler::Add(this);
	}
	
// -----------------------------------------------------------------------------
// CFCSender::~CFCSender
// -----------------------------------------------------------------------------
//
CFCSender::~CFCSender()
	{
	Cancel();
	delete iData;
	iSendBuffer.ResetAndDestroy();
	iSendBuffer.Close();
	}

// -----------------------------------------------------------------------------
// CFCSender::SendL
// -----------------------------------------------------------------------------
//
void CFCSender::SendL (HBufC8* aData,
		        TInetAddr& aAddr)
	{
	if (!IsActive() && iConnected )
		{
		iData = aData;
		iOutgoingData.Set(*iData);
		iSocket->SendTo(iOutgoingData, aAddr, 0, iStatus);
		SetActive();
		}
	else
		{
		User::LeaveIfError( iSendBuffer.Count() <= KSendBufferLen ?
		    KErrNone : KErrNotReady );
		iRemoteAddr = aAddr;
		iSendBuffer.AppendL(aData);	
		}
	}

// -----------------------------------------------------------------------------
// CFCSender::ConnectedL
// -----------------------------------------------------------------------------
//
void CFCSender::ConnectedL()
    {
    iConnected = ETrue;
    SendFromBufferL();
    }

// -----------------------------------------------------------------------------
// CFCSender::RunL
// -----------------------------------------------------------------------------
//
void CFCSender::RunL ()
	{
	TInt err = iStatus.Int();
	if (err == KErrNoMemory)
		{
		// This will leave to RunError-function. See below.
		User::Leave(err);
		}			
	if (err != KErrNone)
		{
		iFCNotifier.ErrorNotify(iStatus.Int());
		}
	delete iData;
	iData = 0;
	
	SendFromBufferL();

	}

// -----------------------------------------------------------------------------
// CFCSender::DoCancel 
// -----------------------------------------------------------------------------
//
void CFCSender::DoCancel ()
	{
	iSocket->CancelSend();
	}

// -----------------------------------------------------------------------------
// CFCSender::RunError 
// -----------------------------------------------------------------------------
//
TInt CFCSender::RunError (TInt aError)
	{
	if(aError != KErrNoMemory)
		{
		return KErrNone;
		}
	return aError;
	}

// -----------------------------------------------------------------------------
// CFCSender::RunError 
// -----------------------------------------------------------------------------
//
void CFCSender::SendFromBufferL()
    {
    if (iSendBuffer.Count() > 0)
        {
        HBufC8* data = iSendBuffer[0];
        iSendBuffer.Remove(0);
        SendL(data, iRemoteAddr);   
        }
    }

