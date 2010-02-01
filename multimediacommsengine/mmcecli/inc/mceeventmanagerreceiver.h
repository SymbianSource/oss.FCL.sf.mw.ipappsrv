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





#ifndef CMCEEVENTMANAGERRECEIVER_H
#define CMCEEVENTMANAGERRECEIVER_H

#include "mceitcreceiverbase.h"
#include "mcedefs.h"

class RMce;
class MMceEventObserver;
class CMceManager;

class CMceEventManagerReceiver : public CMceItcReceiverBase
	{
	public:

		CMceEventManagerReceiver( RMce& aMce, 
							CMceManager& aManager);
                        
		~CMceEventManagerReceiver();

	private: // From CMceItcReceiverBase

		void IncomingEventL(TMceItcCallBack aEvent, TMceIds& aIds, 
									 const TDesC8& aContext );
									 
		void IncomingEventL(TMceItcCallBack aEvent, TMceIds& aIds, 
									 const TDesC8& aContext, 
									 HBufC8* aContent);
									
		void IncomingEventL(TMceItcCallBack aEvent, TMceIds& aIds);

		void ErrorOccuredL (TMceIds& aIds, TInt aError);
    
	private:

		void HandleIncomingEventL ( const TDesC8& aContext, 
									HBufC8* aContent,
									TMceIds& aIds );
        
	private: // Data
		
		CMceManager& iManager;
		MMceEventObserver* iObserver;
       
	//for testing

		MCE_UNIT_TEST_DEFS
	};

#endif

// End of File
