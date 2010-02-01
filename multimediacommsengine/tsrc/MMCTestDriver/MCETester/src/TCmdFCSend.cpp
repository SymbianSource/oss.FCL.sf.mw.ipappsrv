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
* Description:    Implementation
*
*/



#include <MCESession.h>

#include "MCEConstants.h"
#include "TCmdFCSend.h"
#include "CTcMCEContext.h"

void TCmdFCSend::ExecuteL()
	{	
	// ---------- Setup ------------------------------------	

	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution --------------------------------
	
	HBufC8* message = HBufCParameterL(ExtractTextL(KParamFCMessage,EFalse)); 
	
	CleanupStack::PushL(message);												   

	//testing Added by chuan
	TInt fcmsgCount =0;
	fcmsgCount = ExtractIntegerL( KParamFCMessageCount, 0, EFalse );
	if(fcmsgCount==0)
		{
		iContext.FC().SendL(*session, message);
		CleanupStack::Pop(message);
		}
	else
		{
		for ( int i=0; i< fcmsgCount; i++)
			{
			HBufC8* temp= message->AllocLC();
			iContext.FC().SendL(*session, temp);
			CleanupStack::Pop(temp);
			}
		CleanupStack::PopAndDestroy(message);
		}	
	// ---------- Response creation -----------------------	
	AddIdResponseL(KSessionId, *session);
 	
	}
	
TBool TCmdFCSend::Match( const TTcIdentifier& aId )
	{
	return TTcMceCommandBase::Match( aId, _L8("FCSend") );
	}

TTcCommandBase* TCmdFCSend::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdFCSend( aContext );
	}
