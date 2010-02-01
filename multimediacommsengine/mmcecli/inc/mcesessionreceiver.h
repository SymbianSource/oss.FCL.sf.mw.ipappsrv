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





#ifndef CMCESESSIONRECEIVER_H
#define CMCESESSIONRECEIVER_H

#include "mceitcreceiverbase.h"
class CMceSession;
class MMceSessionObserver;
class RMce;
class CMceMsgBase;

class CMceSessionReceiver : public CMceItcReceiverBase
	{
public:

	CMceSessionReceiver( RMce& aMce, CMceSession& aSession );

	~CMceSessionReceiver();
	
	void ReplaceSession( CMceSession& aNewSession );

private://from CMceItcReceiverBase

	 void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
	                             const TDesC8& aContext );
	                             
	 void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
	                             const TDesC8& aContext, 
	                             HBufC8* aContent );
	                             
	 void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds );

     void ErrorOccuredL ( TMceIds& aIds, TInt aError);


private:// methods


    CMceMsgBase* DecodeMessageLC( TMceIds& aIds, 
                                  const TDesC8& aContext );
                                                   
    
private:// data

    CMceSession* iSession;
    
    // For testing
	MCE_UNIT_TEST_DEFS

	};

#endif // end of CMCESESSIONRECEIVER_H

// End of File
