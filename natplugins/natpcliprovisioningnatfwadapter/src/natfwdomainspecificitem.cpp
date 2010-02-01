/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  NAT-FW Traversal domain specific settings
*
*/


// INCLUDE FILES
#include    <e32base.h>
#include    <e32cmn.h>
#include    <centralrepository.h>
#include    "natfwdomainspecificitem.h"
#include    "wpnatfwdebug.h"
#include    "natfwdefaults.h"
#include    <unsafprotocolscrkeys.h>
#include    "stunserveritem.h"
#include    "turnserveritem.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::NewL
// 
// ---------------------------------------------------------------------------
//
CNATFWDomainSpecificItem* CNATFWDomainSpecificItem::NewL()
    {
    CNATFWDomainSpecificItem* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::NewLC
// 
// ---------------------------------------------------------------------------
//
CNATFWDomainSpecificItem* CNATFWDomainSpecificItem::NewLC()
    {
    CNATFWDomainSpecificItem* self = new (ELeave) CNATFWDomainSpecificItem();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::ConstructL()
    {
    DBG_PRINT( "CNATFWDomainSpecificItem::ConstructL - begin" );
    iDomain            = HBufC8::NewL( 0 );
    iSTUNServerAddr    = HBufC8::NewL( 0 );
    iSTUNUsername      = HBufC8::NewL( 0 );
    iSTUNPassword      = HBufC8::NewL( 0 );
    iUsedNatProtocol   = HBufC8::NewL( 0 );
    DBG_PRINT( "CNATFWDomainSpecificItem::ConstructL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::~CNATFWDomainSpecificItem
// 
// ---------------------------------------------------------------------------
//
CNATFWDomainSpecificItem::~CNATFWDomainSpecificItem()
    {
    DBG_PRINT( "CNATFWDomainSpecificItem::~CNATFWDomainSpecificItem - begin" );
    delete iDomain;
    delete iSTUNServerAddr;
    delete iSTUNUsername;
    delete iSTUNPassword;
    delete iUsedNatProtocol;
    delete iNatUtilities;
    iStunServers.ResetAndDestroy();
    iStunServers.Close();
    iTurnServers.ResetAndDestroy();
    iTurnServers.Close();
    DBG_PRINT( "CNATFWDomainSpecificItem::~CNATFWDomainSpecificItem - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetDomainKey
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetDomainKey( TUint32 aDomainKey )
    {
    iDomainKey = aDomainKey;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetDomainL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetDomainL( const TDesC8& aDomain )
    {
    delete iDomain;
    iDomain = NULL;
    iDomain = aDomain.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetSTUNServerAddrL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetSTUNServerAddrL( 
    const TDesC8& aSTUNServerAddr )
    {
    delete iSTUNServerAddr;
    iSTUNServerAddr = NULL;
    iSTUNServerAddr = aSTUNServerAddr.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetSTUNServerPort
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetSTUNServerPort( 
    TUint aSTUNServerPort )
    {
    iSTUNServerPort = aSTUNServerPort;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetNATRefreshTCP
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetNATRefreshTCP( 
    TInt aNATRefreshTCP )
    {
    iNATRefreshTCP = aNATRefreshTCP;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetNATRefreshUDP
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetNATRefreshUDP( 
    TInt aNATRefreshUDP )
    {
    iNATRefreshUDP = aNATRefreshUDP;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetEnableCRLFRefresh
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetEnableCRLFRefresh( 
    TInt aEnableCRLFRefresh )
    {
    iEnableCRLFRefresh = aEnableCRLFRefresh;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetSTUNUsernameL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetSTUNUsernameL( 
    const TDesC8& aSTUNUsername )
    {
    delete iSTUNUsername;
    iSTUNUsername = NULL;
    iSTUNUsername = aSTUNUsername.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetSTUNPasswordL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetSTUNPasswordL( 
    const TDesC8& aSTUNPassword )
    {
    delete iSTUNPassword;
    iSTUNPassword = NULL;
    iSTUNPassword = aSTUNPassword.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetDefaultValues
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetDefaultValuesL()
    {
    // Default values for STUN server keys are set when saving settings if
    // no STUN servers are provisioned.
    delete iUsedNatProtocol;
    iUsedNatProtocol         = NULL;
    iUsedNatProtocol         = KDefaultNatProtocol().AllocL();
    iDisableStunSharedSecret = KDefaultDisableStunSharedSecret;
    iStartPortRange          = KDefaultStartPort;
    iEndPortRange            = KDefaultEndPort;
    // ICE settings.
    delete iNatUtilities;
    iNatUtilities            = NULL;
    iNatUtilities            = KDefaultNatUtilities().AllocL();
    iHostPref                = KDefaultHostPref;
    iServerRefPref           = KDefaultServerReflexivePref;
    iRelayPref               = KDefaultRelayPref;
    iPeerRefPref             = KDefaultPeerReflexivePref;
    iIPv4Pref                = KDefaultIPv4Pref;
    iIPv6Pref                = KDefaultIPv6Pref;
    iVPNPref                 = KDefaultVPNPref;
    iUDPPref                 = KDefaultUDPPref;
    iTCPPref                 = KDefaultTCPPref;
    iTCPActivePref           = KDefaultTCPActivePref;
    iTCPPassivePref          = KDefaultTCPPassivePref;
    iTCPSimultPref           = KDefaultTCPSimultaneousPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetDisableStunSharedSecret
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetDisableStunSharedSecret( 
    TBool aDisableStunSharedSecret )
    {
    iDisableStunSharedSecret = aDisableStunSharedSecret;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetStartPortRange
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetStartPortRange( TInt aStartPortRange )
    {
    iStartPortRange = aStartPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetEndPortRange
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetEndPortRange( TInt aEndPortRange )
    {
    iEndPortRange = aEndPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetNatProtocolL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetNatProtocolL( const TDesC8& aNatProtocol )
    {
    delete iUsedNatProtocol;
    iUsedNatProtocol = NULL;
    iUsedNatProtocol = aNatProtocol.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetNatUtilitiesL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetNatUtilitiesL( const TDesC8& aNatUtilities )
    {
    delete iNatUtilities;
    iNatUtilities = NULL;
    iNatUtilities = aNatUtilities.AllocL();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetHostPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetHostPref( TInt aHostPref )
    {
    iHostPref = aHostPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetServerRefPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetServerRefPref( TInt aServerRefPref )
    {
    iServerRefPref = aServerRefPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetRelayPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetRelayPref( TInt aRelayPref )
    {
    iRelayPref = aRelayPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetPeerRefPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetPeerRefPref( TInt aPeerRefPref )
    {
    iPeerRefPref = aPeerRefPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetIPv4Pref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetIPv4Pref( TInt aIPv4Pref )
    {
    iIPv4Pref = aIPv4Pref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetIPv6Pref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetIPv6Pref( TInt aIPv6Pref )
    {
    iIPv6Pref = aIPv6Pref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetVpnPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetVpnPref( TInt aVPNPref )
    {
    iVPNPref = aVPNPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetUdpPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetUdpPref( TInt aUDPPref )
    {
    iUDPPref = aUDPPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetTcpPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetTcpPref( TInt aTCPPref )
    {
    iTCPPref = aTCPPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetTcpActivePref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetTcpActivePref( TInt aTCPActivePref )
    {
    iTCPActivePref = aTCPActivePref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetTcpPassivePref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetTcpPassivePref( TInt aTCPPassivePref )
    {
    iTCPPassivePref = aTCPPassivePref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::SetTcpSimultPref
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::SetTcpSimultPref( TInt aTCPSimultPref )
    {
    iTCPSimultPref = aTCPSimultPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::AddStunServerItemL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::AddStunServerItemL( 
    CWPStunServerItem* aStunServerItem )
    {
    __ASSERT_ALWAYS( aStunServerItem != NULL, User::Leave( KErrBadHandle ) );
    iStunServers.AppendL( aStunServerItem );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::AddTurnServerItemL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::AddTurnServerItemL( 
    CWPTurnServerItem* aTurnServerItem )
    {
    __ASSERT_ALWAYS( aTurnServerItem != NULL, User::Leave( KErrBadHandle ) );
    iTurnServers.AppendL( aTurnServerItem );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetDomain
// 
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainSpecificItem::GetDomain() const
    {
    return *iDomain;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetDomainKey
// 
// ---------------------------------------------------------------------------
//
TUint32 CNATFWDomainSpecificItem::GetDomainKey() const
    {
    return iDomainKey;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetSTUNAddr
// 
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainSpecificItem::GetSTUNAddr() const
    {
    return *iSTUNServerAddr;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetSTUNPort
// 
// ---------------------------------------------------------------------------
//
TUint CNATFWDomainSpecificItem::GetSTUNPort() const
    {
    return iSTUNServerPort;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetNATRefreshTCP
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::GetNATRefreshTCP() const
    {
    return iNATRefreshTCP;
    }
    
// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetNATRefreshUDP
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::GetNATRefreshUDP() const
    {
    return iNATRefreshUDP;
    }
    
// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetEnableCRLFRefresh
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::GetEnableCRLFRefresh() const
    {
    return iEnableCRLFRefresh;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetSTUNUsername
// 
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainSpecificItem::GetSTUNUsername() const
    {
    return *iSTUNUsername;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::GetSTUNPassword
// 
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainSpecificItem::GetSTUNPassword() const
    {
    return *iSTUNPassword;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::DisableStunSharedSecret
//
// ---------------------------------------------------------------------------
//
TBool CNATFWDomainSpecificItem::DisableStunSharedSecret() const
    {
    return iDisableStunSharedSecret;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::StartPortRange
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::StartPortRange() const
    {
    return iStartPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::EndPortRange
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::EndPortRange() const
    {
    return iEndPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::UsedNatProtocol
// 
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainSpecificItem::UsedNatProtocol() const
    {
    return *iUsedNatProtocol;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::NatUtilities
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainSpecificItem::NatUtilities() const
    {
    return *iNatUtilities;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::HostPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::HostPref() const
    {
    return iHostPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::ServerRefPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::ServerRefPref() const
    {
    return iServerRefPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::RelayPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::RelayPref() const
    {
    return iRelayPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::PeerRefPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::PeerRefPref() const
    {
    return iPeerRefPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::IPv4Pref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::IPv4Pref() const
    {
    return iIPv4Pref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::IPv6Pref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::IPv6Pref() const
    {
    return iIPv6Pref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::VpnPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::VpnPref() const
    {
    return iVPNPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::UdpPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::UdpPref() const
    {
    return iUDPPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::TcpPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::TcpPref() const
    {
    return iTCPPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::TcpActivePref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::TcpActivePref() const
    {
    return iTCPActivePref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::TcpPassivePref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::TcpPassivePref() const
    {
    return iTCPPassivePref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::TcpSimultPref
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainSpecificItem::TcpSimultPref() const
    {
    return iTCPSimultPref;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::StoreStunServerItemsL()
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::StoreStunServerItemsL( 
    CRepository* aRepository, TUint32 aKey, TUint32 aDomainKey )
    {
    DBG_PRINT( "CNATFWDomainSpecificItem::StoreStunServerItemsL - begin" );
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );

    RArray<TUint32> stunKeys;
    CleanupClosePushL( stunKeys ); // CS:1

    TInt err = aRepository->FindL( 
        aDomainKey|KUNSAFProtocolsSTUNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );

    if ( KErrNotFound != err )
        {
        User::LeaveIfError( err );
        }

    TInt stunKeyCount = stunKeys.Count();
    for ( TInt counter = 0 ; counter < stunKeyCount; counter++ )
        {
        TUint32 key = KUNSAFProtocolsSTUNAddressMask^
            stunKeys[counter];
        aRepository->Delete( key|KUNSAFProtocolsSTUNAddressMask );
        aRepository->Delete( key|KUNSAFProtocolsSTUNPortMask );
        aRepository->Delete( key|KUNSAFProtocolsSTUNUsernameMask );
        aRepository->Delete( key|KUNSAFProtocolsSTUNPasswordMask );
        }
    stunKeys.Close();
    CleanupStack::PopAndDestroy( &stunKeys ); // CS:0

    TInt stunCount = iStunServers.Count();
    TUint32 stunKey = aKey;
    for ( TInt counter = 0; counter < stunCount; counter++ )
        {
        stunKey |= KUNSAFProtocolsSubTableFieldTypeMask;
        stunKey++;
        stunKey |= KUNSAFProtocolsSubTableFieldTypeMask;
        stunKey ^= KUNSAFProtocolsSubTableFieldTypeMask;
        stunKey |= aDomainKey;

        DBG_PRINT_2( "CNATFWDomainSpecificItem::StoreStunServerItemsL - \
Create 'additional STUN server' keys (base key %d )",
            stunKey );

        // ==============================
        // STUNServerAddress
        // ==============================
        //
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsSTUNAddressMask|stunKey,
            iStunServers[counter]->StunSrvAddr() ) );

        // ==============================
        // STUNServerPort
        // ==============================
        //  
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsSTUNPortMask|stunKey,
            iStunServers[counter]->StunSrvPort() ) );

        // ==============================
        // STUNServerUsername
        // ==============================
        // 
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsSTUNUsernameMask|stunKey,
            iStunServers[counter]->StunUsername() ) );

        // ==============================
        // STUNServerPassword
        // ==============================
        //
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsSTUNPasswordMask|stunKey,
            iStunServers[counter]->StunPassword() ) );

        DBG_PRINT( "CNATFWDomainSpecificItem::StoreStunServerItemsL - \
'additional STUN server' keys created." );
        } // for
    DBG_PRINT( "CNATFWDomainSpecificItem::StoreStunServerItemsL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainSpecificItem::StoreTurnServerItemsL()
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainSpecificItem::StoreTurnServerItemsL( 
    CRepository* aRepository, TUint32 aKey )
    {
    DBG_PRINT( "CNATFWDomainSpecificItem::StoreTurnServerItemsL - begin" );
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );

    RArray<TUint32> turnKeys;
    CleanupClosePushL( turnKeys ); // CS:1

    TInt err = aRepository->FindL( 
        aKey|KUNSAFProtocolsTURNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );

    if ( KErrNotFound != err )
        {
        User::LeaveIfError( err );
        }

    TInt turnCount( turnKeys.Count() );

    for ( TInt counter = 0; counter < turnCount; counter++ )
        {
        TUint32 key = KUNSAFProtocolsTURNAddressMask^
            turnKeys[counter];
        aRepository->Delete( key|KUNSAFProtocolsTURNAddressMask );
        aRepository->Delete( key|KUNSAFProtocolsTURNPortMask );
        aRepository->Delete( key|KUNSAFProtocolsTURNUsernameMask );
        aRepository->Delete( key|KUNSAFProtocolsTURNPasswordMask );
        }

    turnKeys.Close();
    CleanupStack::PopAndDestroy( &turnKeys );

    turnCount = iTurnServers.Count();

    // Store TURN settings to Central Repository.
    TUint32 turnKey = aKey;
    for ( TInt counter = 0; counter < turnCount; counter++ )
        {
        turnKey |= KUNSAFProtocolsSubTableFieldTypeMask;
        turnKey++;
        turnKey |= KUNSAFProtocolsSubTableFieldTypeMask;
        turnKey ^= KUNSAFProtocolsSubTableFieldTypeMask;
        turnKey |= aKey;

        DBG_PRINT_2( "CNATFWDomainSpecificItem::StoreTurnServerItemsL - \
Create TURN server keys (base key %d)", turnKey );

        // ==============================
        // TURNServerAddress
        // ==============================
        // 
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsTURNAddressMask|turnKey,
            iTurnServers[counter]->TurnSrvAddr() ) );

        // ==============================
        // TURNServerPort
        // ==============================
        //  
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsTURNPortMask|turnKey,
            iTurnServers[counter]->TurnSrvPort() ) );

        // ==============================
        // TURNServerUsername
        // ==============================
        // 
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsTURNUsernameMask|turnKey,
            iTurnServers[counter]->TurnUsername() ) );

        // ==============================
        // TURNServerPassword
        // ==============================
        // 
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsTURNPasswordMask|turnKey,
            iTurnServers[counter]->TurnPassword() ) );

        DBG_PRINT( "CNATFWDomainSpecificItem::StoreTurnServerItemsL - \
TURN server keys created" );
        }
    DBG_PRINT( "CNATFWDomainSpecificItem::StoreTurnServerItemsL - end" );
    }

//  End of File

