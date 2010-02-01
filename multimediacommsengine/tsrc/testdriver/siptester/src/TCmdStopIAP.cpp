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

#include "SIPConstants.h"
#include "TCmdStopIAP.h"
#include "CTcSIPContext.h"

#include <es_sock.h>
#include <es_enum_partner.h>

/**
 * INPUT:
 *   Headers:		-
 *   Parameters:	IAPName
 *   IDs:			-
 *
 * OUTPUT:
 *   Parameters:	-
 *   IDs:			-
 */
void TCmdStopIAP::ExecuteL()
	{
	// -- Setup ---------------------------------------------------------------

	// Get mandatory IAPName
	TPtrC8 name = ExtractTextL( KParamIAPName );

	TUint32 iapId = iContext.IAPIdL( name );
	
	RSocketServ socketServ;
	User::LeaveIfError( socketServ.Connect() );
	CleanupClosePushL( socketServ );	
	
	RConnection rcon;
    User::LeaveIfError( rcon.Open( socketServ ) );
    CleanupClosePushL( rcon );
    
    TUint activeCount( 0 );
    User::LeaveIfError( rcon.EnumerateConnections( activeCount ) );
    
    TBool closed( EFalse );
    
    // Search correct connection info
    if( activeCount > 0  )
        {
        for( TUint i = 1; i <= activeCount && !closed; i++ )
            {
            TPckgBuf<TConnectionInfoV2> connectionInfoV2;
            User::LeaveIfError( rcon.GetConnectionInfo( i, connectionInfoV2 ) );
            
            if ( connectionInfoV2().iIapId == iapId )
                {  
                // V2 con info cannot be used
                TConnectionInfo connectionInfo = static_cast<TConnectionInfo>( connectionInfoV2() );
                
                User::LeaveIfError( rcon.Attach( TPckgBuf<TConnectionInfo>( connectionInfo ), 
                                                 RConnection::EAttachTypeNormal ) );
                                                        
                // Stop the connection
                User::LeaveIfError( rcon.Stop() );
                closed = ETrue;                                       
                }
            }
        }
    
    
    __ASSERT_ALWAYS( closed, User::Leave( KErrNotFound ) );
    
    CleanupStack::PopAndDestroy( 2 ); // socketServ, rcon
	}

TBool TCmdStopIAP::Match( const TTcIdentifier& aId )
	{
	return TTcSIPCommandBase::Match( aId, _L8("StopIAP") );
	}

TTcCommandBase* TCmdStopIAP::CreateL( MTcTestContext& aContext )
	{
	return new( ELeave ) TCmdStopIAP( aContext );
	}
