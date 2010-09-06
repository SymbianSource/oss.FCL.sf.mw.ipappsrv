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
* Description:    
*
*/






#ifndef MCCDATARECEIVER_H
#define MCCDATARECEIVER_H
#include <E32Base.h>
#include <e32msgqueue.h>

#include "mccdatareceiverobserver.h"

typedef TBuf8<50> TQueueBuffer;

// CLASS DECLARATION

/**
*  CMccDataReceiver class declaration
*/
class CMccDataReceiver : public CActive
	{
    public:
    
	    static CMccDataReceiver* NewL( MMccDataReceiverObserver& aObserver );
		
	    ~CMccDataReceiver(); 
		
	 	
    public: // From CActive

		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);	

    private:
        CMccDataReceiver( MMccDataReceiverObserver& aObserver );
	    void ConstructL(); // second construction phase
	    TInt AssignNameToMessageQueueL(const TDes16 &aMessageBuffer);

    private:
	    
		RMsgQueue<TQueueBuffer> iMsgQueue;	
		RMsgQueue<TQueueBuffer> iMsgQueue1;    
		RMsgQueue<TQueueBuffer> iMsgQueue2;    
		MMccDataReceiverObserver* iObserver;
    };


#endif
