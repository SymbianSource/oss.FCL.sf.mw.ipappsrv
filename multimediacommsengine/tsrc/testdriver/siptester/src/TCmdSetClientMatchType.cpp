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

#include "TCmdSetClientMatchType.h"
#include "SIPConstants.h"
#include "CTcSIPContext.h"

_LIT8( KMatchAcceptContactHeader, "MatchAcceptContact" );
_LIT8( KMatchEventHeader, "MatchEvent" );
_LIT8( KMatchWholeRequest, "MatchRequest" );

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	ApplicationUid, ClientMatchType
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdSetClientMatchType::ExecuteL()
	{
	TUid clientUid = ExtractUidL( KParamApplicationUid, ETrue );
	HBufC8* clientMatchTypeBuf = ExtractHBufLC( KParamClientMatchType );
	
	RSipClientSimulator::TMatchType matchType;
	
	if ( clientMatchTypeBuf->CompareF( KMatchAcceptContactHeader ) == 0 )
	    {
	    matchType = RSipClientSimulator::ESipAcceptContactHeaderMatch;
	    }
    else if ( clientMatchTypeBuf->CompareF( KMatchEventHeader ) == 0 )
        {
        matchType = RSipClientSimulator::ESipEventHeaderMatch;
        }
    else if ( clientMatchTypeBuf->CompareF( KMatchWholeRequest ) == 0 )
        {
        matchType = RSipClientSimulator::ESipRequestMatch;
        }
    else
        {
        matchType = RSipClientSimulator::ESipNoClientMatch;
        }

    User::LeaveIfError(
        iContext.SipClientSimulator().SetClientMatchType( 
            matchType, clientUid ) );
	
	CleanupStack::PopAndDestroy( clientMatchTypeBuf );
	}

TBool TCmdSetClientMatchType::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("SetClientMatchType") );
	}

TTcCommandBase* TCmdSetClientMatchType::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdSetClientMatchType( aContext );
	}

