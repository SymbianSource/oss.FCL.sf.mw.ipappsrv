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



#ifndef MCEDATASENDER_H
#define MCEDATASENDER_H
#include <E32Base.h>
#include <e32msgqueue.h>


typedef TBuf8<50> TQueueBuffer;

// CLASS DECLARATION

/**
*  CMccPeriodicRunner class declaration
*/
class CMceDataSender : public CActive
	{
    public:
    
	    static CMceDataSender* NewL();
		
	    ~CMceDataSender(); 
		
	public:
	 
	 	void Send( const TDesC8& aData );
	 	
    public: // From CActive

		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);	

    private:
        CMceDataSender();
	    void ConstructL(); // second construction phase
	    TInt AssignNameToMessageQueueL(const TDes16 &aMessageBuffer);

    private:
	    
	    TBool iReady;
		RMsgQueue<TQueueBuffer> iMsgQueue;
	    RMsgQueue<TQueueBuffer> iMsgQueue1;    
	    RMsgQueue<TQueueBuffer> iMsgQueue2;
		TQueueBuffer iBuffer;
		 
    };


#endif