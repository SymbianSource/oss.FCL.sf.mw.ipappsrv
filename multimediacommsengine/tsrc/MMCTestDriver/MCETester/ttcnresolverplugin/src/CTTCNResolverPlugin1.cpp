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
* Description:    Implementation.
*
*/



#include "CTTCNResolverPlugin1.h"
#include <apacmdln.h>

/*_LIT8(KCapabilities,
"<SIP_CLIENT ALLOW_STARTING=\"NO\">\
<SIP_HEADERS>\
<ACCEPT value=\"application/sdp\"/>\
<ACCEPT value=\"\"/>\
<ALLOW_EVENTS value=\"ttcn\"/>\
</SIP_HEADERS>\
<SDP_LINES>\
<LINE name=\"m\" value=\"audio 0 RTP/AVP 96\"/>\
<LINE name=\"m\" value=\"application 0 udp TBCP\"/>\
</SDP_LINES>\
</SIP_CLIENT>");*/


_LIT8(KCapabilities,
//"<SIP_CLIENT ALLOW_STARTING=\"YES\"><SIP_HEADERS><ACCEPT value=\"application/sdp\"/><ACCEPT value=\"\"/></SIP_HEADERS><SDP_LINES><LINE name=\"m\" value=\"audio 0 RTP/AVP 0\"/></SDP_LINES></SIP_CLIENT>");
//"<SIP_CLIENT ALLOW_STARTING=\"NO\"><SIP_HEADERS><ACCEPT value=\"application/sdp\"/><ACCEPT value=\"\"/><ALLOW_EVENTS value=\"dialog\"/></SIP_HEADERS><SDP_LINES><LINE name=\"m\" value=\"audio 0 RTP/AVP 96\"/><LINE name=\"m\" value=\"application 0 udp TBCP\"/></SDP_LINES></SIP_CLIENT>" );
//"<SIP_CLIENT ALLOW_STARTING=\"NO\"><SIP_HEADERS><ALLOW_EVENTS value=\"TestEvent\"/><ALLOW_EVENTS value=\"ttcn\"/><ALLOW_EVENTS value=\"refer\"/><ACCEPT_CONTACT value=\"*;+testdriver\"/></SIP_HEADERS></SIP_CLIENT>" );
"<SIP_CLIENT ALLOW_STARTING=\"NO\"><SIP_HEADERS><ACCEPT_CONTACT value=\"*;+mcetester\"/></SIP_HEADERS></SIP_CLIENT>" );


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
    return KCapabilities;
    }