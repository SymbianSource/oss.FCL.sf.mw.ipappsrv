/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation of VoIP client resolver for incoming SIP messages
*
*/




// INCLUDE FILES
#include <apacmdln.h>
#include "VoIPPlugin.h"
#include "EComImplementationProxy.h"
#include <SIPHeaderBase.h>
#include <e32base.h>
#include <e32std.h>
#include <in_sock.h>
#include <badesca.h>
#include <apacmdln.h>

// CONSTANTS
const TUid KMyApplicationUid = {0x10202870}; 

// The capabilities defined here are valid, but not in use, since they are defined in reseource file.
_LIT8(KCapabilities,
//"<SIP_CLIENT ALLOW_STARTING=\"YES\"><SIP_HEADERS><ACCEPT value=\"application/sdp\"/><ACCEPT value=\"\"/></SIP_HEADERS><SDP_LINES><LINE name=\"m\" value=\"audio 0 RTP/AVP 0\"/></SDP_LINES></SIP_CLIENT>");
"<SIP_CLIENT ALLOW_STARTING=\"NO\"><SIP_HEADERS><ACCEPT value=\"application/sdp\"/><ACCEPT value=\"\"/><ALLOW_EVENTS value=\"dialog\"/></SIP_HEADERS><SDP_LINES><LINE name=\"m\" value=\"audio 0 RTP/AVP 96\"/><LINE name=\"m\" value=\"application 0 udp TBCP\"/></SDP_LINES></SIP_CLIENT>" );


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVoIPPlugin::CVoIPPlugin
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVoIPPlugin::CVoIPPlugin() 
    {                      
    }                      


// -----------------------------------------------------------------------------
// CVoIPPlugin::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVoIPPlugin::ConstructL()
    {
	RImplInfoPtrArray infoArray;
	REComSession::ListImplementationsL( KSIPResolvedClientIFUid, infoArray );
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, &infoArray ) );

	TBool found = EFalse;
	for (TInt i=0; !found && i<infoArray.Count(); i++)
		{
		CImplementationInformation* info = infoArray[ i ];
		if ( info->ImplementationUid() == KMyApplicationUid )
			{
			TLex8 lex( info->DataType() );
			TUint value( 0 );
			User::LeaveIfError( lex.Val( value, EHex ) );
		    iApplicationUID.iUid = value;
			found = ETrue;
			}
		}

	CleanupStack::PopAndDestroy( 1 ); // infoArray

	if ( !found )
		{
		User::Leave( KErrNotFound );
		}
    }


// -----------------------------------------------------------------------------
// CVoIPPlugin::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVoIPPlugin* CVoIPPlugin::NewL()
    {
    CVoIPPlugin* self = new( ELeave ) CVoIPPlugin;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

    
// Destructor
CVoIPPlugin::~CVoIPPlugin()
    {
    }


// -----------------------------------------------------------------------------
// CVoIPPlugin::ChannelL
// Implementation of ChannelL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUid CVoIPPlugin::ChannelL( RStringF /*aMethod*/,
    	                          	const TDesC8& /*aRequestUri*/,
	                              	const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
        	                     	const TDesC8& /*aContent*/,
            	                 	const CSIPContentTypeHeader* /*aContentType*/)
    {
	const TUid KServerUid3 = {0x1020E4C7};
    return KServerUid3;
    
	//return iApplicationUID;
    }


// -----------------------------------------------------------------------------
// ChannelL::ConnectL
// ConnectL, does nothing
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVoIPPlugin::ConnectL( TUid /*aUid*/ )
    {
    }


// -----------------------------------------------------------------------------
// CVoIPPlugin::Capabilities
// Returns application capabilities if not defined in rss-file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC8& CVoIPPlugin::Capabilities()
    {
    return KCapabilities;
    }


// -----------------------------------------------------------------------------
// CVoIPPlugin::ResetAndDestroy
// Implementation of ResetAndDestroy
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVoIPPlugin::ResetAndDestroy( TAny* anArray )
	{
    RImplInfoPtrArray* array = reinterpret_cast<RImplInfoPtrArray*>( anArray );
    if (array)
        {
        array->ResetAndDestroy();
        }
	}

//  End of File



