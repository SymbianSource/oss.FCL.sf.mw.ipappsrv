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




#include "mcefcmsgexchange.h"
#include "mcemanager.h"
#include "mceitcsender.h"
#include "mceserial.h"

EXPORT_C CMceFcMsgExchange* CMceFcMsgExchange::NewL(
				CMceManager& aManager,
				const TDesC8& aType)
	{
    CMceFcMsgExchange* self = new (ELeave) CMceFcMsgExchange(aManager);
    CleanupStack::PushL(self);
    self->ConstructL(aType);
    CleanupStack::Pop(self);
    return self;
	}
        
CMceFcMsgExchange::~CMceFcMsgExchange()
	{
	delete iType;
	}

EXPORT_C void CMceFcMsgExchange::InitializeL(
	CMceSession& aSession, TInt aPort, MMceFcMsgObserver& aObserver)
	{
	aSession.SetFCObserver(&aObserver);
    
    TMceIds ids;
	ids.iAppUID = iManager.AppUid().iUid;
	ids.iSessionID = aSession.Id();
    
	ids.Set( KMceIdsIndexFCPort, aPort );
	    
	if(aSession.iFlatData->iType == CMceComSession::EOutSession)
		{
		aSession.ITCSender().WriteL(ids, EMceItcInitFC, *iType );				
		}
    else // insession
        {
        aSession.ITCSender().WriteL(ids, EMceItcUpdateFC, *iType );				
        }
	}

EXPORT_C void CMceFcMsgExchange::SendL(CMceSession& aSession, HBufC8* aMessage)
	{
	TMceIds ids;
	ids.iAppUID = iManager.AppUid().iUid;
	ids.iSessionID = aSession.Id();
	
	aSession.ITCSender().WriteL(ids, EMceItcSendFCMsg, *aMessage );
    delete aMessage;	
	}

EXPORT_C MDesC8Array*
CMceFcMsgExchange::MediaAttributeLinesL(CMceSession& aSession)
	{
	TMceIds ids;
	ids.iAppUID = iManager.AppUid().iUid;
	ids.iSessionID = aSession.Id();
	ids.iMsgType = EMceItcMsgTypeFC;
	
	return aSession.ITCSender().ReadArrayL(ids, EMceItcFCGetMediaLines );
	}

EXPORT_C void CMceFcMsgExchange::UpdateL(
					 CMceSession& aSession,
					 CDesC8Array* aMediaAttributeLines)
	{
	TMceIds ids;
	ids.iAppUID = iManager.AppUid().iUid;
	ids.iSessionID = aSession.Id();
	ids.iMsgType = EMceItcMsgTypeFC;

	CMceMsgTextArray* attributeLines =
		new (ELeave) CMceMsgTextArray( *aMediaAttributeLines );
	CleanupStack::PushL( attributeLines );
	aSession.ITCSender().SendL( ids, EMceItcFCSetMediaLines, *attributeLines );
	CleanupStack::PopAndDestroy( attributeLines );
	
	delete aMediaAttributeLines;
	}

void CMceFcMsgExchange::ConstructL(const TDesC8& aType)
	{
	iType = aType.AllocL();
	}

CMceFcMsgExchange::CMceFcMsgExchange(CMceManager& aManager):
	iManager(aManager)	
	{	
	}
