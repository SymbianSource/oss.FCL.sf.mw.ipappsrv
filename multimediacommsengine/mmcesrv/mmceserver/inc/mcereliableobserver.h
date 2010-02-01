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




#ifndef MMCERELIABLEOBSERVER_H
#define MMCERELIABLEOBSERVER_H

#include <e32std.h> 
#include <sipservertransaction.h>  


// Forwared declation.

class CMceComSession;

class MMceReliableObserver
    {
    public://new
    
  /**
   * Callback function to indicate PRACK is not contained in SIP transaction.
   * @param aTransaction
   * @return void
   */        
        virtual void NoPrackReceived( CSIPServerTransaction& aTransaction ) = 0;
        virtual void ReliableFailed( CSIPServerTransaction& aTransaction, TInt aError ) = 0;
	};
	
#endif	
	