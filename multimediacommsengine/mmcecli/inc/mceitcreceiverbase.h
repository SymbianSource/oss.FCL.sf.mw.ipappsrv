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





#ifndef CMCEITCRECEIVERBASE_H
#define CMCEITCRECEIVERBASE_H


#include <e32base.h>
#include <s32mem.h>
#include "mcedefs.h"
#include "mceclientserver.h"
class MMceItc;
class CMCERequestElements;
class CMCEResponseElements;
class MMCEHttpDigestChallengeObserver;




class CMceItcReceiverBase : public CActive
	{
	
public:

	virtual ~CMceItcReceiverBase();

protected:

    CMceItcReceiverBase ( MMceItc& aITC );
    
    void Init();
    

	virtual void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
	                             const TDesC8& aContext ) = 0;
	virtual void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds, 
	                             const TDesC8& aContext, 
	                             HBufC8* aContent ) = 0;
	                             
	virtual void IncomingEventL( TMceItcCallBack aEvent, TMceIds& aIds ) = 0;

    virtual void ErrorOccuredL ( TMceIds& aIds, TInt aError) = 0;

private: // From CActive

	void RunL();
	TInt RunError(TInt aError);
	void DoCancel();

private: // New functions

    void ReceiveNext();

    void IncomingEventL( TMceIds& aIds );
    
    void ReceiveDataLC( HBufC8*& aContext, HBufC8*& aContent );
    
    

protected: // Data

    TPckgBuf<TMceIds> iIdsPckg;
    
private: // Data

    MMceItc& iITC;
    TIpcArgs iITCMsgArgs;  
    TPckgBuf<TMceMessageBufSizes> iSizesPckg;

//for testing

    MCE_UNIT_TEST_DEFS

	};

#endif // CMCEITCRECEIVERBASE_H

// End of File
