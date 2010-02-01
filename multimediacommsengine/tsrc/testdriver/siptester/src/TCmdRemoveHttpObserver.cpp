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
* Description:  Implementation
*
*/

#include "TCmdRemoveHttpObserver.h"
#include "CTcSIPContext.h"

/**
 * INPUT:
 *   Headers: -
 *   Parameters: -
 *   IDs: -
 *
 * OUTPUT:
 *   Parameters: -
 *   IDs: -
 */
void TCmdRemoveHttpObserver::ExecuteL()
	{
	iContext.RemoveDigestContainer();
	}

TBool TCmdRemoveHttpObserver::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("RemoveHttpObserver") );
	}

TTcCommandBase* TCmdRemoveHttpObserver::CreateL( MTcTestContext& aContext )
	{
	return new ( ELeave ) TCmdRemoveHttpObserver( aContext );
	}

