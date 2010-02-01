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
* Description:    
*
*/




#include "mcclink.h"

#include <mmccinterface.h>
//#include <mmccnetworksettings.h>
#include "natfwtestconsolelogs.h"
#include "mccsession.h"

// ---------------------------------------------------------------------------
// CMccLink::ConstructL
// ---------------------------------------------------------------------------
// 
void CMccLink::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CMccLink::NewL
// ---------------------------------------------------------------------------
//     
CMccLink* CMccLink::NewL( TInt aLinkType, TUint aIapId,
    CMccSession* aSession )
    {
    CMccLink* self = new( ELeave ) CMccLink( aLinkType, aIapId, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccLink::CMccLink
// ---------------------------------------------------------------------------
// 
CMccLink::CMccLink( TInt aLinkType, TUint aIapId, CMccSession* aSession ) :
    iLinkType( aLinkType ), iIapId( aIapId ), iSession( aSession )
    {
    }

// ---------------------------------------------------------------------------
// CMccLink::~CMccLink
// ---------------------------------------------------------------------------
// 
CMccLink::~CMccLink()
    {
    Close();
    }

// ---------------------------------------------------------------------------
// CMccLink::CreateLink
// ---------------------------------------------------------------------------
//     
void CMccLink::CreateLinkL( TInt32& aLinkId, TInetAddr& aDestAddr,
    TRequestStatus& aStatus )
    {
    __NATFWTESTCONSOLE( "CMccLink::CreateLinkL" )
    
    // NET settings value
    TMccNetSettings netsettings;
    netsettings.iRemoteAddress.SetAddress( aDestAddr.Address() );
    netsettings.iRemoteAddress.SetPort( aDestAddr.Port() );
    netsettings.iRemoteRtcpPort = KDefaultRtcpPort;

    netsettings.iLocalAddress.SetAddress( KInetAddrAny );
    netsettings.iLocalAddress.SetPort( KDefaultRtpPort );

    netsettings.iLocalPublicAddress.SetAddress( KInetAddrAny );
    netsettings.iLocalRtcpPort = KDefaultRtcpPort;
    netsettings.iLocalPublicRtcpPort = KDefaultRtcpPort;
    netsettings.iMediaQosValue = KDefaultMediaQos;
    netsettings.iIapId = iIapId;
    
    // create Mcc link
    User::LeaveIfError( iSession->MccInterface()->CreateLink(
        iSession->MccSessionId(), iLinkType, iLinkId, netsettings ) ); 
    
    aLinkId = iLinkId;
    aStatus = KRequestPending;
    }

// ---------------------------------------------------------------------------
// CMccLink::CreateLink
// ---------------------------------------------------------------------------
//     
void CMccLink::CreateLinkL( TInt32& aLinkId, TInetAddr& aDestAddr,
    TUint aRtcpPort, TRequestStatus& aStatus )
    {
    __NATFWTESTCONSOLE( "CMccLink::CreateLinkL" )
    
    // NET settings value
    TMccNetSettings netsettings;
    netsettings.iRemoteAddress.SetAddress( aDestAddr.Address() );
    netsettings.iRemoteAddress.SetPort( aDestAddr.Port() );
    netsettings.iRemoteRtcpPort = aRtcpPort;

    netsettings.iLocalAddress.SetAddress( KInetAddrAny );
    netsettings.iLocalAddress.SetPort( KDefaultRtpPort );

    netsettings.iLocalPublicAddress.SetAddress( KInetAddrAny );
    netsettings.iLocalRtcpPort = KDefaultRtcpPort;
    netsettings.iLocalPublicRtcpPort = KDefaultRtcpPort;
    netsettings.iMediaQosValue = KDefaultMediaQos;
    netsettings.iIapId = iIapId;
    
    // create Mcc link
    User::LeaveIfError( iSession->MccInterface()->CreateLink(
        iSession->MccSessionId(), iLinkType, iLinkId, netsettings ) ); 
    
    aLinkId = iLinkId;
    aStatus = KRequestPending;
    }
    
// ---------------------------------------------------------------------------
// CMccLink::SetRemoteAddressL
// ---------------------------------------------------------------------------
// 
void CMccLink::SetRemoteAddressL( TInetAddr& aDestAddr )
    {
    __NATFWTESTCONSOLE( "CMccLink::SetRemoteAddressL" )
    // TODO: is using TMccNetSettings unnecessary 
    /*
    TMccNetSettings netsettings;
    netsettings.iRemoteAddress.SetAddress( aDestAddr.Address() );
    netsettings.iRemoteAddress.SetPort( aDestAddr.Port() );
    */
    User::LeaveIfError( iSession->MccInterface()->SetRemoteAddress(
        iSession->MccSessionId(), iLinkId, aDestAddr ) );
    }

// ---------------------------------------------------------------------------
// CMccLink::SetRemoteAddressL
// ---------------------------------------------------------------------------
// 
void CMccLink::SetRemoteAddressL( TInetAddr& aDestAddr, TUint aRtcpPort )
    {
    __NATFWTESTCONSOLE( "CMccLink::SetRemoteAddressL + rtcp" )
    
    // TODO: is using TMccNetSettings unnecessary
    /*
    TMccNetSettings netsettings;
    netsettings.iRemoteAddress.SetAddress( aDestAddr.Address() );
    netsettings.iRemoteAddress.SetPort( aDestAddr.Port() );
    netsettings.iRemoteRtcpPort = aRtcpPort;
    */
    
    User::LeaveIfError( iSession->MccInterface()->SetRemoteAddress(
        iSession->MccSessionId(), iLinkId, aDestAddr, 
        aRtcpPort ) );
    }

// ---------------------------------------------------------------------------
// CMccLink::Close
// ---------------------------------------------------------------------------
// 
TInt CMccLink::Close()
    {
    return iSession->MccInterface()->CloseLink( iSession->MccSessionId(),
        iLinkId );
    }

// ---------------------------------------------------------------------------
// CMccLink::LinkId
// ---------------------------------------------------------------------------
//     
TUint32 CMccLink::LinkId()
    {
    return iLinkId;
    }
