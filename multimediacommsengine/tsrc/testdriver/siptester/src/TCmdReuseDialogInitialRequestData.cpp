/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "TCmdReuseDialogInitialRequestData.h"
#include <sipdialogassocbase.h>
#include <sipdialog.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	-
 *   IDs:			InviteDialogId*, SubscribeDialogId*
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdReuseDialogInitialRequestData::ExecuteL()
	{
	CSIPDialogAssocBase* dialogAssoc = GetAnyDialogAssocL();
    if ( dialogAssoc )
        {
        User::LeaveIfError( dialogAssoc->Dialog().ReuseInitialRequestData() );
        }
	}

TBool TCmdReuseDialogInitialRequestData::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("ReuseDialogInitialRequestData") );
	}

TTcCommandBase* TCmdReuseDialogInitialRequestData::CreateL( 
    MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdReuseDialogInitialRequestData( aContext );
	}
