/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation.
*
*/

#include "CTTCNResolverPlugin2.h"
#include <apacmdln.h>

_LIT8(KCapabilities,
"<SIP_CLIENT ALLOW_STARTING=\"YES\">\
<SIP_HEADERS>\
<ACCEPT value=\"application/sdp\"/>\
<ACCEPT_CONTACT value=\"*;+g.3gpp.cs-voice;require;explicit\"/>\
</SIP_HEADERS>\
<SDP_LINES>\
<LINE name=\"m\" value=\"video 60000 RTP/AVP 98\">\
<MEDIA_ATTRIBUTE value=\"application:com.nokia.rtvoip\"/>\
</LINE>\
</SDP_LINES>\
</SIP_CLIENT>");



/*_LIT8(KCapabilities,
"<SIP_CLIENT ALLOW_STARTING=\"YES\">\
<SIP_HEADERS>\
<ACCEPT value=\"application/sdp\"/>\
</SIP_HEADERS>\
<SDP_LINES>\
<LINE name=\"m\" value=\"video 60000  RTP/AVP 98\"><MEDIA_ATTRIBUTE value=\"application2:com.nokia.rtvoip\"/></LINE>\
</SDP_LINES>\
<ACCEPT_CONTACT value=\"*;+g.3gpp.cs-voice;require;explicit\"/>\
</SIP_CLIENT>");
*/

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin2::NewL
// -----------------------------------------------------------------------------
//
CTTCNResolverPlugin2* CTTCNResolverPlugin2::NewL()
	{
	CTTCNResolverPlugin2* self = new( ELeave ) CTTCNResolverPlugin2;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin2::~CTTCNResolverPlugin2
// -----------------------------------------------------------------------------
//	
CTTCNResolverPlugin2::~CTTCNResolverPlugin2()
    {
    delete iCapabilities;
    }
	
// -----------------------------------------------------------------------------
// CTTCNResolverPlugin2::MyUid
// -----------------------------------------------------------------------------
//
TInt32 CTTCNResolverPlugin2::MyUid() const
    {
    return KTTCNResolverPluginUID2;
    }
    
// -----------------------------------------------------------------------------
// CTTCNResolverPlugin2::Capabilities
// -----------------------------------------------------------------------------
//
const TDesC8& CTTCNResolverPlugin2::Capabilities()
    {
    TRAP_IGNORE( CapabilitiesL() )
    
    return *iCapabilities;
    }

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin2::ConstructL
// -----------------------------------------------------------------------------
//    
void CTTCNResolverPlugin2::ConstructL()
    {
    CTTCNResolverPluginBase::ConstructL();
    
    iCapabilities = KCapabilities().AllocL();
    }
    
// -----------------------------------------------------------------------------
// CTTCNResolverPlugin2::CapabilitiesL
// -----------------------------------------------------------------------------
//
void CTTCNResolverPlugin2::CapabilitiesL()
    {
    // Read capabilities data from c:\ttcnresolverplugincaps_????????.xml,
    // where ???????? is uid of the plugin.
    // If file doesn't exist, default capabilites data will be used.
    TFileName capabilitiesLocation;
    ConstructUidFileNameL( _L("c:\\ttcnresolverplugincaps"), 
                           MyUid(), 
                           _L(".xml"),
                           capabilitiesLocation );
    
   	HBufC8* tempCaps = ReadFileContentsL( capabilitiesLocation );
    if ( tempCaps )
        {
        delete iCapabilities;
	    iCapabilities = tempCaps;
        }
    }

