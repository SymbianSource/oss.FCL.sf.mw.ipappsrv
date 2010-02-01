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
* Description:    fcreceiver.cpp
*
*/




#include "fcreceiver.h"

// -----------------------------------------------------------------------------
// CFCReceiver::NewL
// -----------------------------------------------------------------------------
//
CFCReceiver* CFCReceiver::NewL(RSocket& aSocket, MFCNotifier& aMFCNotifier)
	{
	CFCReceiver* self = NewLC(aSocket, aMFCNotifier);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CFCReceiver::NewLC
// -----------------------------------------------------------------------------
//
CFCReceiver* CFCReceiver::NewLC(RSocket& aSocket, MFCNotifier& aMFCNotifier)
	{
	CFCReceiver* self = new(ELeave) CFCReceiver(aSocket, aMFCNotifier);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CFCReceiver::ConstructL
// -----------------------------------------------------------------------------
//
void CFCReceiver::ConstructL()
	{
	iReceivedMsg = HBufC8::NewL(KMaxMessageLength);
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CFCReceiver::CFCReceiver
// -----------------------------------------------------------------------------
//
CFCReceiver::CFCReceiver(RSocket& aSocket, MFCNotifier& aMFCNotifier) 
: CActive(EPriorityStandard),
iFCNotifier(aMFCNotifier),
iReceivedMsgPtr(0,0,0),
iSocket(aSocket)
	{
	}

// -----------------------------------------------------------------------------
// CFCReceiver::~CFCReceiver
// -----------------------------------------------------------------------------
//
CFCReceiver::~CFCReceiver()
	{
	delete iReceivedMsg;
	}

// -----------------------------------------------------------------------------
// CFCReceiver::DoCancel
// -----------------------------------------------------------------------------
//
void CFCReceiver::DoCancel()
	{
	iSocket.CancelRecv();
	}

// -----------------------------------------------------------------------------
// CFCReceiver::Listen
// -----------------------------------------------------------------------------
//
void CFCReceiver::Listen()
	{
	if(!IsActive())
		{
		iReceivedMsgPtr.Set (iReceivedMsg->Des());
		iSocket.RecvFrom(iReceivedMsgPtr, iAddr, 0, iStatus);
		SetActive();
		}
	}

// -----------------------------------------------------------------------------
// CFCReceiver::RunL
// -----------------------------------------------------------------------------
//
void CFCReceiver::RunL()
	{
	switch(iStatus.Int())
		{
		case KErrNone:
			{
			HBufC8* data = HBufC8::NewL(iReceivedMsg->Length());
			TPtr8 cptr = data->Des();
			cptr.Copy(*iReceivedMsg);
			ReceivedData(data);			
			Listen();
			break;
			}
		case KErrCancel:
			break;
		case KErrNoMemory:
			{
			User::Leave(iStatus.Int());
			break;
			}
		default:
            
            iFCNotifier.ErrorNotify(iStatus.Int());
            break;
		}
	}

// -----------------------------------------------------------------------------
// CFCReceiver::ReceivedDataL
// -----------------------------------------------------------------------------
//
void CFCReceiver::ReceivedData(HBufC8* aData)
	{
	iFCNotifier.ReceivedData(aData) ;
	}

// -----------------------------------------------------------------------------
// CFCReceiver::RunError
// -----------------------------------------------------------------------------
//
TInt CFCReceiver::RunError(TInt aError)
	{
	Listen();
	if (aError != KErrNoMemory)
		{
		return KErrNone;
		}
	return aError;
	}
