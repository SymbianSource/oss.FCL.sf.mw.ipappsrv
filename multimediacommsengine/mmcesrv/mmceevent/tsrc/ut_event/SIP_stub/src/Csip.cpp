/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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




#include <badesca.h> 
#include "sip.h"
#include "sipobserver.h"
#include "sipstrings.h"


EXPORT_C CSIP* CSIP::NewL(const TUid& aUid,MSIPObserver& aSIPObserver)
	{
	CSIP* self = new(ELeave) CSIP(aSIPObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aUid);
	CleanupStack::Pop(self);
	return self;
	}


void CSIP::ConstructL(const TUid& /*aUid*/)
	{
    SIPStrings::OpenL();
	}


CSIP::CSIP(MSIPObserver& aSIPObserver): iObserver(aSIPObserver)
	{
	}
	

EXPORT_C CSIP::~CSIP()
	{
	SIPStrings::Close();
	} 


EXPORT_C CDesC8Array* CSIP::SupportedSecurityMechanismsL() const
    {
    _LIT8(Ksipsec3gpp , "ipsec-3gpp");
    _LIT8(Ksipdigest, "digest");
    
    CDesC8ArraySeg* mechanisms = new(ELeave)CDesC8ArraySeg(1);
    CleanupStack::PushL(mechanisms);
  
    mechanisms->AppendL(Ksipsec3gpp);
  	mechanisms->AppendL(Ksipdigest);
        
    CleanupStack::Pop(mechanisms);
    
	return mechanisms;
    }


EXPORT_C CSIPConnection* CSIP::Connection(TUint32 /*aIapId*/) const
	{
	return iConnections[0];
	}
