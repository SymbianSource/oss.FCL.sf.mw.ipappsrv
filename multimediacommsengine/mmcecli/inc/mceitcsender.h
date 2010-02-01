/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef CMCEITCSENDER_H
#define CMCEITCSENDER_H

#include <e32base.h>
#include <s32mem.h>
#include <badesca.h>
#include "mceclientserver.h"

class MMceItc;
class CMceSession;
class CMceMsgBase;

class CMceItcSender : public CBase
	{
public:

    static CMceItcSender* NewL( MMceItc& aITC );
    static CMceItcSender* NewLC( MMceItc& aITC );
    ~CMceItcSender ();

    TInt Send( TMceIds& aIds, TMceItcFunctions aITCFunction );

	void SendL( TMceIds& aIds, TMceItcFunctions aITCFunction );

    void SendL( TMceIds& aIds,
			    TMceItcFunctions aITCFunction,
                TUint32& aData );

    void SendL( TMceIds& aIds,
                TMceItcFunctions aITCFunction,
                CMceMsgBase& aMessage,
                HBufC8* aBody = NULL,
                TBool aTakeOwnershipOfBody = ETrue );		
 
public:

	void WriteL( TMceIds& aIds,
			     TMceItcFunctions aITCFunction,
                 const TDesC8& aData );

 	void ReadL( TMceIds& aIds,
                TMceItcFunctions aITCFunction,
	            TDes8& aData );
 
 	CDesC8Array* ReadArrayL( TMceIds& aIds,
 					         TMceItcFunctions aITCFunction );
 					
 	CDesC8Array* ReadArrayL( TMceIds& aIds,
 					         TMceItcFunctions aITCFunction,
 					         const TDesC8& aData );
 					
    HBufC8* ReadStringL( TMceIds& aIds,
				         TMceItcFunctions aITCFunction );
 	
private:

	CMceItcSender( MMceItc& aITC );
    
	// data
    TIpcArgs iITCMsgArgs;
    MMceItc& iITC;
    
    TBufC8<1> iEmptyBody;
	};


#endif // CMCEITCSENDER_H

// End of File
