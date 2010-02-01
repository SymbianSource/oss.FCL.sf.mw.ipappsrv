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





#ifndef CMCEMANAGERRECEIVER_H
#define CMCEMANAGERRECEIVER_H

#include "mceitcreceiverbase.h"
#include "mcedefs.h"

class RMce;
class MMceInSessionObserver;
class CMceManager;

class CMceManagerReceiver : public CMceItcReceiverBase
	{
public:

	CMceManagerReceiver( RMce& aMce, 
                        CMceManager& aManager );
                        
	~CMceManagerReceiver();

private: // From CMceItcReceiverBase


	void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
	                             const TDesC8& aContext );
	                             
	void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
	                             const TDesC8& aContext, 
	                             HBufC8* aContent );
	                            
    void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds );
    void ErrorOccuredL (TMceIds& aIds, TInt aError);
    
private:

    void HandleIncomingSessionL ( const TDesC8& aContext, 
                                  HBufC8* aContent,
                                  TMceIds& aIds );

private: // Data
	
	MMceInSessionObserver* iObserver;
    CMceManager& iManager;
    TBool iDetailedCallbacks;

//for testing

    MCE_UNIT_TEST_DEFS

	};

#endif // CMCEMANAGERRECEIVER_H

// End of File
