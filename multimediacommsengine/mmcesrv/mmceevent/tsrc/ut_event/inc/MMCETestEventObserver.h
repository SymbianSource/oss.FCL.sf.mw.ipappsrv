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
* Description:    Test observer class
*
*/





#ifndef CMCETESTEVENTOBSERVER_H
#define CMCETESTEVENTOBSERVER_H

#include <e32base.h>
#include <badesca.h>
#include "mcerefer.h"
#include "mceevent.h"

class MMCETestEventObserver
    {

public: 
    
    virtual void IncomingSubscribeL(
		CDesC8Array* aHeaders,
        HBufC8* aContentType,
		HBufC8* aBody)=0;
						
    virtual void IncomingReferL(
		HBufC8* aReferTo,
		CDesC8Array* aHeaders,
        HBufC8* aContentType,
		HBufC8* aBody)=0;

	virtual void EventStateChangedL( TUint32 aState ) = 0;
		
	virtual void NotifyReceivedL(CDesC8Array* aHeaders,
                				HBufC8* aContentType,
								HBufC8* aBody) = 0;
    };

#endif
