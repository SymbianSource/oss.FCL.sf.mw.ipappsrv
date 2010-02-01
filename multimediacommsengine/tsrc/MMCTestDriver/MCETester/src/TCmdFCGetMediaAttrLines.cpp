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
#include "TCmdFCGetMediaAttrLines.h"
#include "CTcMCEContext.h"



void TCmdFCGetMediaAttrLines::ExecuteL()
	{	
	// ---------- Setup ------------------------------------
	
	// Get session
	CMceSession* session = 
		reinterpret_cast<CMceSession*>(GetObjectForIdL(KSessionId, ETrue));	
	
	// ---------- Execution --------------------------------


	// ---------- Response creation ----------------------- 

	MDesC8Array* mediaAttrLines = iContext.FC().MediaAttributeLinesL(*session);
	TCleanupItem cleanup(DeleteMediaAttrLines, mediaAttrLines);
	CleanupStack::PushL(cleanup);
	
	if ( mediaAttrLines->MdcaCount() > 0 )
		{
		AddArrayResponseL( KResponseFCMediaAttrLines, *mediaAttrLines );
		}
		
	CleanupStack::PopAndDestroy(mediaAttrLines);

 	AddIdResponseL(KSessionId, *session);
 	
	}
	
TBool TCmdFCGetMediaAttrLines::Match( const TTcIdentifier& aId )
	{
	//return TTcMceCommandBase::Match( aId, _L8("Establish") );
	return TTcMceCommandBase::Match( aId, _L8("FCGetMediaAttributeLines") );
	}

TTcCommandBase* TCmdFCGetMediaAttrLines::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdFCGetMediaAttrLines( aContext );
	}
	
void TCmdFCGetMediaAttrLines::DeleteMediaAttrLines( TAny* ptr)
	{
	MDesC8Array* mediaAttrLines = reinterpret_cast<MDesC8Array*>(ptr);
	delete mediaAttrLines;
	}
