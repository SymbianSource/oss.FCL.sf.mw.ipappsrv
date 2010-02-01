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





#ifndef CMCEEVENTRECEIVER_H
#define CMCEEVENTRECEIVER_H

#include "mceitcreceiverbase.h"
class CMceEvent;
class MMceEventObserver;
class MMceReferObserver;
class MMCESubscribeObserver;
class RMce;
class CMceMsgBase;


class CMceEventReceiver : public CMceItcReceiverBase
	{
	public:

		CMceEventReceiver(RMce& aMce, CMceEvent& aEvent );

		CMceEventReceiver();

		~CMceEventReceiver ();

	private://from CMceItcReceiverBase

		void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
		                             const TDesC8& aContext );
		                             
		void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
		                             const TDesC8& aContext, 
		                             HBufC8* aContent );
		                             
		void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds );

	    void ErrorOccuredL ( TMceIds& aIds, TInt aError);

	private:// methods

	    /*void HandleSessionStateChangedL( TMceIds& aIds, 
	                                     const TDesC8& aContext, 
	                                     HBufC8* aContent );
	    */
    	CMceMsgBase* DecodeMessageLC( TMceIds& aIds, 
                                  const TDesC8& aContext );
                                  
	private:// data

	    CMceEvent& iEvent;
	    
	    // For testing
	    MCE_UNIT_TEST_DEFS
        

	};

#endif // end of CMCESESSIONRECEIVER_H

// End of File
