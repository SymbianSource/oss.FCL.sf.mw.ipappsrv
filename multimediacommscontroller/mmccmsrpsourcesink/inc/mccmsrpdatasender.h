/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef MCCDATASENDER_H
#define MCCDATASENDER_H
#include <E32Base.h>
#include <e32msgqueue.h>
#include "mccexpirationhandler.h"
#include "mcctimermanager.h"

#include <CMSRP.h>
#include <CMSRPSession.h>


typedef TBuf8<50> TQueueBuffer;

// CLASS DECLARATION

/**
*  CMccPeriodicRunner class declaration
*/
class CMccMsrpDataSender : public CActive, public MMccExpirationHandler
	{
    public:
    
	    static CMccMsrpDataSender* NewL();
		
	    ~CMccMsrpDataSender(); 
		
	public:
	 
	 	void SendL( const TDesC8& aData , CMSRPSession* aMsrpSession);
	 	void SendPacketL(CMSRPSession* aMsrpSession);
	 	
    public: // From CActive

		void DoCancel();
		void RunL();
		TInt RunError(TInt aError);	

    private:
        CMccMsrpDataSender();
	    void ConstructL(); // second construction phase
	    
    protected: // From MMccExpirationHandler
        
        void TimerExpiredL( TMccTimerId aTimerId, TAny* aTimerParam );

    private:
	    
		RMsgQueue<TQueueBuffer> iMsgQueue;
		TQueueBuffer iBuffer;
		
	    CMccTimerManager* iTimeoutTimer;
	    TMccTimerId iTimerId;
	    TUint32 iTimeoutTimeMilliseconds;
		 
    };


#endif
