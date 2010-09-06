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



#ifndef MCEDATARECEIVER_H
#define MCEDATARECEIVER_H
#include <E32Base.h>
#include <e32msgqueue.h>

#include "mcedatasinkobserver.h"

typedef TBuf8<50> TQueueBuffer;

// CLASS DECLARATION

/**
*  CMccDataReceiver class declaration
*/
class CMceDataReceiver : public CActive
	{
    public:
    
	    static CMceDataReceiver* NewL( MMceDataSinkObserver& aObserver );
		
	    ~CMceDataReceiver(); 
		
	 	
    public: // From CActive

		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);	

    private:
        CMceDataReceiver( MMceDataSinkObserver& aObserver );
	    void ConstructL(); // second construction phase
        TInt AssignNameToMessageQueueL(const TDes16 &aMessageBuffer);

    private:
	    
	    TBool iReady;
		RMsgQueue<TQueueBuffer> iSendMsgQueue;		
	    RMsgQueue<TQueueBuffer> iSendMsgQueue1;    
	    RMsgQueue<TQueueBuffer> iSendMsgQueue2;
		MMceDataSinkObserver* iObserver;
    };


#endif
