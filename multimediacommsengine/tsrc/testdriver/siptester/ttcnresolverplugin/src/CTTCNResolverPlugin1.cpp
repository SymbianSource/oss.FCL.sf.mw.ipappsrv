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

#include "CTTCNResolverPlugin1.h"
#include <apacmdln.h>

_LIT8(KCapabilities,
"<SIP_CLIENT ALLOW_STARTING=\"YES\">\
<SIP_HEADERS>\
<ACCEPT value=\"text/plain\"/>\
<ACCEPT value=\"application/sdp\"/>\
<ACCEPT value=\"mime/submime\"/>\
<ACCEPT value=\"corrupted/corrupted\"/>\
<ACCEPT value=\"\"/>\
<ACCEPT_CONTACT value=\"*;+testdriver\"/>\
</SIP_HEADERS>\
<SDP_LINES>\
<LINE name=\"m\" value=\"audio 30000 RTP/AVP 98\"/>\
<LINE name=\"m\" value=\"audio 54321 RTP/AVP 104\"/>\
<LINE name=\"m\" value=\"audio 54321 RTP/AVT 104\"/>\
<LINE name=\"m\" value=\"audio 30000 RTP/AVP 98\"/>\
<LINE name=\"m\" value=\"audio 30002 RTP/AVP 0/8\"/>\
<LINE name=\"m\" value=\"audio 30000 RTP/AVP 0/8\"/>\
<LINE name=\"m\" value=\"audio 30000 RTP/AVP 99\"/>\
<LINE name=\"m\" value=\"audio 49172 RTP/AVP 0\"/>\
<LINE name=\"m\" value=\"audio 49154 RTP/AVP 105\"/>\
<LINE name=\"m\" value=\"control 54321 RTP/POC 104\"/>\
<LINE name=\"m\" value=\"control 49154 RTP/POC 104\"/>\
<LINE name=\"m\" value=\"application 5062 TCP X-game\"/>\
<LINE name=\"m\" value=\"application 5062 tcp X-game\"/>\
<LINE name=\"m\" value=\"application 5062 UDP X-game\"/>\
<LINE name=\"m\" value=\"application 5062 TCP X-ZZZ\"/>\
<LINE name=\"m\" value=\"application 6544 udp 98\"/>\
</SDP_LINES>\
</SIP_CLIENT>");

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin1::NewL
// -----------------------------------------------------------------------------
//
CTTCNResolverPlugin1* CTTCNResolverPlugin1::NewL()
	{
	CTTCNResolverPlugin1* self = new( ELeave ) CTTCNResolverPlugin1;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin1::~CTTCNResolverPlugin1
// -----------------------------------------------------------------------------
//	
CTTCNResolverPlugin1::~CTTCNResolverPlugin1()
    {
    delete iCapabilities;
    }
	
// -----------------------------------------------------------------------------
// CTTCNResolverPlugin1::MyUid
// -----------------------------------------------------------------------------
//
TInt32 CTTCNResolverPlugin1::MyUid() const
    {
    return KTTCNResolverPluginUID1;
    }
    
// -----------------------------------------------------------------------------
// CTTCNResolverPlugin1::Capabilities
// -----------------------------------------------------------------------------
//
const TDesC8& CTTCNResolverPlugin1::Capabilities()
    {
    TRAP_IGNORE( CapabilitiesL() )
    
    return *iCapabilities;
    }

// -----------------------------------------------------------------------------
// CTTCNResolverPlugin1::ConstructL
// -----------------------------------------------------------------------------
//    
void CTTCNResolverPlugin1::ConstructL()
    {
    CTTCNResolverPluginBase::ConstructL();
    
    iCapabilities = KCapabilities().AllocL();
    }
    
// -----------------------------------------------------------------------------
// CTTCNResolverPlugin1::CapabilitiesL
// -----------------------------------------------------------------------------
//
void CTTCNResolverPlugin1::CapabilitiesL()
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

