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
* Description:  Holds and stores one NAT/FW settings item.
*
*/


// INCLUDE FILES
#include    <e32std.h>
#include    <e32cmn.h>
#include    <charconv.h>
#include    <CWPCharacteristic.h>
#include    "WPAdapterUtil.h"
//#include    "versit.h"

// Central Repository keys for NAT-FW Traversal settings
#include    <centralrepository.h>
#include    <unsafprotocolscrkeys.h>
#include    "wpnatfwitem.h"
#include    "natfwdomainspecificitem.h"
#include    "natfwiapspecificitem.h"
#include    "natfwdefaults.h"
#include    "wpnatfwdebug.h"
#include    "stunserveritem.h"
#include    "turnserveritem.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CWPNATFWItem::NewL
//
// ---------------------------------------------------------------------------
//
CWPNATFWItem* CWPNATFWItem::NewL() 
    {
    CWPNATFWItem* self = new (ELeave) CWPNATFWItem;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::ConstructL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::ConstructL()
    {
    DBG_PRINT( "CWPNATFWItem::ConstructL - begin" );
    // Initialization with empty string or KNotSaved
    iItemId                          = TUint( KNotSaved );
    iName                            = HBufC::NewL( 0 );
    iAppRef                          = HBufC8::NewL( 0 );
    iSaveData                        = HBufC8::NewL( 0 );
    iSavedDomainId                   = HBufC8::NewL( 0 );
    DBG_PRINT( "CWPNATFWItem::ConstructL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::~CWPNATFWItem
//
// ---------------------------------------------------------------------------
//
CWPNATFWItem::~CWPNATFWItem()
    {
    DBG_PRINT( "CWPNATFWItem::~CWPNATFWItem - begin" );
    iDomainItems.ResetAndDestroy();
    iDomainItems.Close();
    iIAPItems.ResetAndDestroy();
    iIAPItems.Close();
    delete iName;
    delete iAppRef;
    delete iSaveData;
    delete iSavedDomainId;
    DBG_PRINT( "CWPNATFWItem::~CWPNATFWItem - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::SetItemId
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::SetItemId( TUint aItemId )
    {
    iItemId = aItemId;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::SetNameL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::SetNameL( const TDesC& aName )
    {
    delete iName;
    iName = NULL;
    iName = aName.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::SetAppRefL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::SetAppRefL( const TDesC8& aAppRef )
    {
    delete iAppRef;
    iAppRef = NULL;
    iAppRef = aAppRef.AllocL();
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::AppRef
//
// ---------------------------------------------------------------------------
//
const TDesC8& CWPNATFWItem::AppRef() const
    {
    return *iAppRef;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::AddDomainSpecific
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::AddDomainSpecificL( CNATFWDomainSpecificItem* 
    aNATFWDomainSpecificItem )
    {
    __ASSERT_ALWAYS( aNATFWDomainSpecificItem != NULL, 
        User::Leave( KErrArgument ) );
    iDomainItems.AppendL( aNATFWDomainSpecificItem );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::AddIAPSpecific
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::AddIAPSpecificL( CNATFWIAPSpecificItem* 
    aNATFWIAPSpecificItem )
    {
    __ASSERT_ALWAYS( aNATFWIAPSpecificItem != NULL,
        User::Leave( KErrArgument ) );
    iIAPItems.AppendL( aNATFWIAPSpecificItem );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::IAPItems
//
// ---------------------------------------------------------------------------
//
const RPointerArray<CNATFWIAPSpecificItem>& CWPNATFWItem::IAPItems() const
    {
    return iIAPItems;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::Name
//
// ---------------------------------------------------------------------------
//
const TDesC& CWPNATFWItem::Name() const
    {
    return *iName;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::ItemId
//
// ---------------------------------------------------------------------------
//
TUint CWPNATFWItem::ItemId() const
    {
    return iItemId;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::CreateIceSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::CreateIceSettingsL( CRepository* aRepository, TInt aIndex,
    TUint aKey )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );

    DBG_PRINT( "CWPNATFWItem::CreateIceSettingsL - begin" );
    TUint32 currentDomainKey( KErrNone );
    currentDomainKey = aKey|KUNSAFProtocolsDomainMask;
    currentDomainKey &= KUNSAFProtocolsKeyMask;

    // ==============================
    // ICE: NatUtilities
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsNATUtilities,
        iDomainItems[aIndex]->NatUtilities() ) );

    // ==============================
    // ICE: Host preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsHostPref,
        iDomainItems[aIndex]->HostPref() ) );

    // ==============================
    // ICE: Server-reflexive preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsServerRefPref,
        iDomainItems[aIndex]->ServerRefPref() ) );

    // ==============================
    // ICE: Relay-preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsRelayPref,
        iDomainItems[aIndex]->RelayPref() ) );

    // ==============================
    // ICE: Peer-reflexive preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsPeerRefPref,
        iDomainItems[aIndex]->PeerRefPref() ) );

    // ==============================
    // ICE: IPv4 preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsIPV4Pref,
        iDomainItems[aIndex]->IPv4Pref() ) );

    // ==============================
    // ICE: IPv6 preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsIPV6Pref,
        iDomainItems[aIndex]->IPv6Pref() ) );

    // ==============================
    // ICE: VPN preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsVPNPref,
        iDomainItems[aIndex]->VpnPref() ) );

    // ==============================
    // ICE: UDP preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsUDPPref,
        iDomainItems[aIndex]->UdpPref() ) );

    // ==============================
    // ICE: TCP preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsTCPPref,
        iDomainItems[aIndex]->TcpPref() ) );

    // ==============================
    // ICE: TCP-active preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsTCPActivePref,
        iDomainItems[aIndex]->TcpActivePref() ) );

    // ==============================
    // ICE: TCP-passive preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsTCPPassivePref,
        iDomainItems[aIndex]->TcpPassivePref() ) );

    // ==============================
    // ICE: TCP-simultaneous-open preference
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsTCPSimultPref,
        iDomainItems[aIndex]->TcpSimultPref() ) );

    DBG_PRINT( "CWPNATFWItem::CreateIceSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::CreateTurnSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::CreateTurnSettingsL( CRepository* aRepository, TInt aIndex,
    TUint aKey )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );

    DBG_PRINT( "CWPNATFWItem::CreateTurnSettingsL - begin" );
    TUint32 currentDomainKey( KErrNone );
    currentDomainKey = aKey|KUNSAFProtocolsDomainMask;
    currentDomainKey &= KUNSAFProtocolsKeyMask;

    iDomainItems[aIndex]->StoreTurnServerItemsL( aRepository, 
        currentDomainKey );

    DBG_PRINT( "CWPNATFWItem::CreateTurnSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::CreateStunSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::CreateStunSettingsL( CRepository* aRepository, TInt aIndex,
    TUint aKey, HBufC8* aTmpSavedata )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );
    __ASSERT_ALWAYS( aTmpSavedata != NULL, User::Leave( KErrArgument ) );

    DBG_PRINT( "CWPNATFWItem::CreateStunSettingsL - begin" );
    TUint32 currentDomainKey( KErrNone );
    currentDomainKey = aKey|KUNSAFProtocolsDomainMask;
    currentDomainKey &= KUNSAFProtocolsKeyMask;

    TUint32 currentNewDomainKey = currentDomainKey;
    currentNewDomainKey = aKey|KUNSAFProtocolsFieldTypeMask;
    currentNewDomainKey ^= KUNSAFProtocolsFieldTypeMask;

    TUint32 stunKey;
    stunKey = KUNSAFProtocolsSubTableFieldTypeMask;
    stunKey ^= KUNSAFProtocolsSubTableFieldTypeMask;
    stunKey |= currentNewDomainKey;

    // Store "additional STUN servers".
    iDomainItems[aIndex]->StoreStunServerItemsL( aRepository, stunKey, 
        currentNewDomainKey );

    // ==============================
    // STUNServerAddress
    // ==============================
    // 
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsSTUNServerMask|currentDomainKey, 
        iDomainItems[aIndex]->GetSTUNAddr() ) );

    // Set the same value into STUN server table.
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsSTUNAddressMask|stunKey,
        iDomainItems[aIndex]->GetSTUNAddr() ) );

    // ==============================
    // STUNServerPort
    // ==============================
    //  
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsSTUNServerPortMask|currentDomainKey, 
        ( TInt )iDomainItems[aIndex]->GetSTUNPort() ) );

    // Set the same value into STUN server table.
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsSTUNPortMask|stunKey,
        ( TInt )iDomainItems[aIndex]->GetSTUNPort() ) );

    // ==============================
    // NATRefreshUDP
    // ==============================
    // 
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsDomainIntervalUDPMask|currentDomainKey, 
        iDomainItems[aIndex]->GetNATRefreshUDP() ) );

    // ==============================
    // NATRefreshTCP
    // ==============================
    // 
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsDomainIntervalTCPMask|currentDomainKey, 
        iDomainItems[aIndex]->GetNATRefreshTCP() ) );

    // ==============================
    // EnableCRLFRefresh
    // ==============================
    //  
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsDomainEnableCRLFRefresh|currentDomainKey,
        iDomainItems[aIndex]->GetEnableCRLFRefresh() ) );

    // ==============================
    // STUNServerUsername
    // ==============================
    // 
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsSTUNUsernameMask|stunKey, 
        iDomainItems[aIndex]->GetSTUNUsername() ) );

    // ==============================
    // STUNServerPassword
    // ==============================
    // 
    User::LeaveIfError( aRepository->Create( 
        KUNSAFProtocolsSTUNPasswordMask|stunKey, 
        iDomainItems[aIndex]->GetSTUNPassword() ) );

    // ==============================
    // STUNSharedSecret
    // ==============================
    //
    User::LeaveIfError( aRepository->Create( currentDomainKey|
        KUNSAFProtocolsDomainSharedSecretNotSupported,
        iDomainItems[aIndex]->DisableStunSharedSecret() ) );

    // ==============================
    // StartPortRange
    // ==============================
    //
    User::LeaveIfError( aRepository->Create( 
        currentDomainKey|KUNSAFProtocolsPortPoolStartPortMask,
        iDomainItems[aIndex]->StartPortRange() ) );

    // ==============================
    // EndPortRange
    // ==============================
    //
    User::LeaveIfError( aRepository->Create( 
        currentDomainKey|KUNSAFProtocolsPortPoolEndPortMask,
        iDomainItems[aIndex]->EndPortRange() ) );

    // ==============================
    // UsedNATProtocol
    // ==============================
    //
    User::LeaveIfError( aRepository->Create(
        currentDomainKey|KUNSAFProtocolsUsedNATProtocolMask,
        iDomainItems[aIndex]->UsedNatProtocol() ) );

    if ( 0 == aIndex )
        {
        HBufC8* tmpId = HBufC8::NewL( KMaxCharsInTUint32 );

        // APPREF is always reference to first domain specific
        // characteristic.
        tmpId->Des().AppendNum( 
            currentDomainKey|KUNSAFProtocolsDomainMask );
        delete iSavedDomainId;
        iSavedDomainId = NULL;
        iSavedDomainId = tmpId;
        tmpId = NULL;    
        }
    DBG_PRINT( "CWPNATFWItem::CreateStunSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::CreateIapSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::CreateIapSettingsL( CRepository* aRepository,
    HBufC8* aTmpSavedata )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );
    __ASSERT_ALWAYS( aTmpSavedata != NULL, User::Leave( KErrArgument ) );

    DBG_PRINT( "CWPNATFWItem::CreateIapSettingsL - begin" );
    for ( TInt counter = 0; counter < iIAPItems.Count(); counter++ )
        {
        RArray<TUint32> keys;
        CleanupClosePushL( keys );

        // Get next free IAP key
        aRepository->FindL( KUNSAFProtocolsIAPIdMask,
            KUNSAFProtocolsFieldTypeMask, keys );
        TInt keyCount = keys.Count();   
        TUint32 tmp ( 0 );
        if ( keyCount > 0 )
            {
            tmp = keys[keyCount - 1] + 1;
            }
        else
            {
            tmp = KUNSAFProtocolsIAPTableMask;
            }

        TUint32 currentIAPKey( KErrNone );
        currentIAPKey = tmp|KUNSAFProtocolsIAPIdMask;
        currentIAPKey &= KUNSAFProtocolsKeyMask;

        // ==============================
        // IAPId
        // ==============================
        //
        User::LeaveIfError( aRepository->Create(
            currentIAPKey|KUNSAFProtocolsIAPIdMask, 
            (TInt)iIAPItems[counter]->GetIAPId() ) );

        aTmpSavedata->Des().AppendNumFixedWidth( 
            currentIAPKey|KUNSAFProtocolsIAPIdMask, 
            EDecimal, KMaxCharsInTUint32 );

        // ==============================
        // IntervalUDP
        // ==============================
        //

        // If there are no domain entries and no value for UDP
        // refresh interval has been set for IAP item, use default value.
        if ( ( 0 == iDomainItems.Count() ) &&
            KNotSet == iIAPItems[counter]->GetIntervalUDP() )
            {
            iIAPItems[counter]->SetIntervalUDP( 
                KDefaultUDPRefreshInterval );
            }
        if ( KNotSet != iIAPItems[counter]->GetIntervalUDP() )
            {
            User::LeaveIfError( aRepository->Create(
                KUNSAFProtocolsIntervalUDPMask|currentIAPKey, 
                iIAPItems[counter]->GetIntervalUDP() ) );
            }

        // ==============================
        // IntervalTCP
        // ==============================
        //

        // If there are no domain entries and no value for TCP
        // refresh interval has been set for IAP item, use default value.
        if ( ( 0 == iDomainItems.Count() ) &&
            KNotSet == iIAPItems[counter]->GetIntervalTCP() )
            {
            iIAPItems[counter]->SetIntervalTCP( 
                KDefaultTCPRefreshInterval );
            }
        if ( KNotSet != iIAPItems[counter]->GetIntervalTCP() )
            {
            User::LeaveIfError( aRepository->Create( 
                KUNSAFProtocolsIntervalTCPMask|currentIAPKey,
                iIAPItems[counter]->GetIntervalTCP() ) );
            }

        // ==============================
        // STUNRetransmitTimer
        // ==============================
        //
        User::LeaveIfError( aRepository->Create( 
            KUNSAFProtocolsSTUNRetransmitTimerMask|
            currentIAPKey, iIAPItems[counter]->
            GetSTUNRetransmitTimer() ) );
        keys.Close();
        CleanupStack::PopAndDestroy( &keys );    
        }//for
    DBG_PRINT( "CWPNATFWItem::CreateIapSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::UpdateIceSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::UpdateIceSettingsL( CRepository* aRepository, TInt aIndex,
    TUint aKey )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );

    DBG_PRINT( "CWPNATFWItem::UpdateIceSettingsL - begin" );
    TUint32 currentDomainKey( KErrNone );
    currentDomainKey = aKey|KUNSAFProtocolsDomainMask;
    currentDomainKey &= KUNSAFProtocolsKeyMask;
    TInt err ( KErrNone );

    // ==============================
    // ICE: NAT Utilities
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsNATUtilities,
        iDomainItems[aIndex]->NatUtilities() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsNATUtilities,
            iDomainItems[aIndex]->NatUtilities() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: Host preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsHostPref,
        iDomainItems[aIndex]->HostPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
        currentDomainKey|KUNSAFProtocolsHostPref,
        iDomainItems[aIndex]->HostPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: Server-reflexive preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsServerRefPref,
        iDomainItems[aIndex]->ServerRefPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsServerRefPref,
            iDomainItems[aIndex]->ServerRefPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: Relay-preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsRelayPref,
        iDomainItems[aIndex]->RelayPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsRelayPref,
            iDomainItems[aIndex]->RelayPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: Peer-reflexive preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsPeerRefPref,
        iDomainItems[aIndex]->PeerRefPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsPeerRefPref,
            iDomainItems[aIndex]->PeerRefPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: IPv4 preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsIPV4Pref,
        iDomainItems[aIndex]->IPv4Pref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsIPV4Pref,
            iDomainItems[aIndex]->IPv4Pref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: IPv6 preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsIPV6Pref,
        iDomainItems[aIndex]->IPv6Pref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsIPV6Pref,
            iDomainItems[aIndex]->IPv6Pref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: VPN preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsVPNPref,
        iDomainItems[aIndex]->VpnPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsVPNPref,
            iDomainItems[aIndex]->VpnPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: UDP preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsUDPPref,
        iDomainItems[aIndex]->UdpPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsUDPPref,
            iDomainItems[aIndex]->UdpPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: TCP preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsTCPPref,
        iDomainItems[aIndex]->TcpPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsTCPPref,
            iDomainItems[aIndex]->TcpPref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: TCP-active preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsTCPActivePref,
        iDomainItems[aIndex]->TcpActivePref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsTCPActivePref,
            iDomainItems[aIndex]->TcpActivePref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: TCP-passive preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsTCPPassivePref,
        iDomainItems[aIndex]->TcpPassivePref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsTCPPassivePref,
            iDomainItems[aIndex]->TcpPassivePref() );
        }
    User::LeaveIfError( err );

    // ==============================
    // ICE: TCP-simultaneous-open preference
    // ==============================
    //
    err = aRepository->Set(
        currentDomainKey|KUNSAFProtocolsTCPSimultPref,
        iDomainItems[aIndex]->TcpSimultPref() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create(
            currentDomainKey|KUNSAFProtocolsTCPSimultPref,
            iDomainItems[aIndex]->TcpSimultPref() );
        }
    User::LeaveIfError( err );

    DBG_PRINT( "CWPNATFWItem::UpdateIceSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::UpdateTurnSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::UpdateTurnSettingsL( CRepository* aRepository, TInt aIndex,
    TUint aKey )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );

    DBG_PRINT( "CWPNATFWItem::UpdateTurnSettingsL - begin" );
    TUint32 currentDomainKey( KErrNone );
    currentDomainKey = aKey|KUNSAFProtocolsDomainMask;
    currentDomainKey &= KUNSAFProtocolsKeyMask;

    iDomainItems[aIndex]->StoreTurnServerItemsL( aRepository, 
        currentDomainKey );

    DBG_PRINT( "CWPNATFWItem::UpdateTurnSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::UpdateStunSettingsL
//
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::UpdateStunSettingsL( CRepository* aRepository, TInt aIndex,
    TUint aKey, HBufC8* aTmpSavedata )
    {
    __ASSERT_ALWAYS( aRepository != NULL, User::Leave( KErrBadHandle ) );
    __ASSERT_ALWAYS( aTmpSavedata != NULL, User::Leave( KErrArgument ) );

    DBG_PRINT( "CWPNATFWItem::UpdateStunSettingsL - begin" );
    TUint32 currentDomainKey( KErrNone );
    currentDomainKey = aKey|KUNSAFProtocolsDomainMask;
    currentDomainKey &= KUNSAFProtocolsKeyMask;

    TUint32 currentNewDomainKey = currentDomainKey;
    currentNewDomainKey = aKey|KUNSAFProtocolsFieldTypeMask;
    currentNewDomainKey ^= KUNSAFProtocolsFieldTypeMask;

    TUint32 stunKey;
    stunKey = KUNSAFProtocolsSubTableFieldTypeMask;
    stunKey ^= KUNSAFProtocolsSubTableFieldTypeMask;
    stunKey |= currentNewDomainKey;

    // Store "additional STUN servers".
    iDomainItems[aIndex]->StoreStunServerItemsL( aRepository, stunKey,
        currentNewDomainKey );

    TInt err ( KErrNone );

    // ==============================
    // STUNServerAddress
    // ==============================
    // 
    err = aRepository->Set( 
        KUNSAFProtocolsSTUNServerMask|currentDomainKey, 
        iDomainItems[aIndex]->GetSTUNAddr() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsSTUNServerMask|currentDomainKey, 
            iDomainItems[aIndex]->GetSTUNAddr() );
        }
    User::LeaveIfError( err );

    // Set the same value into STUN server table.
    err = aRepository->Set( 
        KUNSAFProtocolsSTUNAddressMask|stunKey,
        iDomainItems[aIndex]->GetSTUNAddr() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsSTUNAddressMask|stunKey,
            iDomainItems[aIndex]->GetSTUNAddr() );
        }
    User::LeaveIfError( err );
    
    // ==============================
    // STUNServerPort
    // ==============================
    //  
    err = aRepository->Set( 
        KUNSAFProtocolsSTUNServerPortMask|currentDomainKey, 
        ( TInt )iDomainItems[aIndex]->GetSTUNPort() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsSTUNServerPortMask|currentDomainKey, 
            ( TInt )iDomainItems[aIndex]->GetSTUNPort() );
        }
    User::LeaveIfError( err );

    // Set the same value into STUN server table.
    err = aRepository->Set( 
        KUNSAFProtocolsSTUNPortMask|stunKey,
        ( TInt )iDomainItems[aIndex]->GetSTUNPort() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsSTUNPortMask|stunKey,
            ( TInt )iDomainItems[aIndex]->GetSTUNPort() );
        }
    User::LeaveIfError( err );

    // ==============================
    // NATRefreshUDP
    // ==============================
    // 
    err = aRepository->Set( 
        KUNSAFProtocolsDomainIntervalUDPMask|currentDomainKey, 
        iDomainItems[aIndex]->GetNATRefreshUDP() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsDomainIntervalUDPMask|currentDomainKey, 
            iDomainItems[aIndex]->GetNATRefreshUDP() );
        }
    User::LeaveIfError( err );

    // ==============================
    // NATRefreshTCP
    // ==============================
    // 
    err = aRepository->Set( 
        KUNSAFProtocolsDomainIntervalTCPMask|currentDomainKey, 
        iDomainItems[aIndex]->GetNATRefreshTCP() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsDomainIntervalTCPMask|currentDomainKey, 
            iDomainItems[aIndex]->GetNATRefreshTCP() );
        }
    User::LeaveIfError( err );

    // ==============================
    // EnableCRLFRefresh
    // ==============================
    //  
    err = aRepository->Set( 
        KUNSAFProtocolsDomainEnableCRLFRefresh|
        currentDomainKey, iDomainItems[aIndex]->
        GetEnableCRLFRefresh() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsDomainEnableCRLFRefresh|currentDomainKey,
            iDomainItems[aIndex]->GetEnableCRLFRefresh() );
        }
    User::LeaveIfError( err );

    // ==============================
    // STUNServerUsername
    // ==============================
    // 
    err = aRepository->Set( 
        KUNSAFProtocolsSTUNUsernameMask|stunKey,
        iDomainItems[aIndex]->GetSTUNUsername() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsSTUNUsernameMask|stunKey,
            iDomainItems[aIndex]->GetSTUNUsername() );
        }
    User::LeaveIfError( err );

    // ==============================
    // STUNServerPassword
    // ==============================
    // 
    err = aRepository->Set( 
        KUNSAFProtocolsSTUNPasswordMask|stunKey,
        iDomainItems[aIndex]->GetSTUNPassword() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            KUNSAFProtocolsSTUNPasswordMask|stunKey,
            iDomainItems[aIndex]->GetSTUNPassword() );

        }
    User::LeaveIfError( err );

    // ==============================
    // STUNSharedSecret
    // ==============================
    //
    err = aRepository->Set( currentDomainKey|
        KUNSAFProtocolsDomainSharedSecretNotSupported,
        iDomainItems[aIndex]->DisableStunSharedSecret() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( currentDomainKey|
            KUNSAFProtocolsDomainSharedSecretNotSupported,
            iDomainItems[aIndex]->DisableStunSharedSecret() );
        }
    User::LeaveIfError( err );

    // ==============================
    // StartPortRange
    // ==============================
    //
    err = aRepository->Set( 
        currentDomainKey|KUNSAFProtocolsPortPoolStartPortMask,
        iDomainItems[aIndex]->StartPortRange() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            currentDomainKey|KUNSAFProtocolsPortPoolStartPortMask,
            iDomainItems[aIndex]->StartPortRange() );
        }
    User::LeaveIfError( err );

    // ==============================
    // EndPortRange
    // ==============================
    //
    err = aRepository->Set( 
        currentDomainKey|KUNSAFProtocolsPortPoolEndPortMask,
        iDomainItems[aIndex]->EndPortRange() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            currentDomainKey|KUNSAFProtocolsPortPoolEndPortMask,
            iDomainItems[aIndex]->EndPortRange() );
        }
    User::LeaveIfError( err );

    // ==============================
    // UsedNATProtocol
    // ==============================
    //
    err = aRepository->Set( 
        currentDomainKey|KUNSAFProtocolsUsedNATProtocolMask,
        iDomainItems[aIndex]->UsedNatProtocol() );
    if ( KErrNotFound == err )
        {
        err = aRepository->Create( 
            currentDomainKey|KUNSAFProtocolsUsedNATProtocolMask,
            iDomainItems[aIndex]->UsedNatProtocol() );
        }
    User::LeaveIfError( err );

    if ( 0 == aIndex )
        {
        HBufC8* tmpId = HBufC8::NewL( KMaxCharsInTUint32 );

        // APPREF is always reference to first domain specific
        // characteristic.
        
        tmpId->Des().AppendNum(
            currentDomainKey|KUNSAFProtocolsDomainMask );
        
        delete iSavedDomainId;
        iSavedDomainId = NULL;
        iSavedDomainId = tmpId;
        tmpId = NULL;    
        }

    DBG_PRINT( "CWPNATFWItem::UpdateStunSettingsL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::StoreL
// 1) Save domain specific settings.
// 2) Save iap specific settings.
// ---------------------------------------------------------------------------
//
void CWPNATFWItem::StoreL()
    {
    DBG_PRINT( "CWPNATFWItem::StoreL - begin" );

    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1

    // For savedata
    HBufC8* tmpSavedata = HBufC8::NewLC( KMaxCharsInTUint32 * 
        ( iIAPItems.Count() + iDomainItems.Count() ) ); // CS:2

    // ==============================
    // DomainSpecific 
    // add commands 
    // ==============================
    //
    for ( TInt counter = 0; counter < iDomainItems.Count(); counter++ )
        {
        DBG_PRINT( "CWPNATFWItem::StoreL - Save DomainSpecific settings.." );

        RArray<TUint32> keys;
        CleanupClosePushL( keys ); // CS:3

        // Get next free Domain key.
        rep->FindL( KUNSAFProtocolsDomainMask, 
            KUNSAFProtocolsFieldTypeMask, keys );
        TInt keyCount = keys.Count();
        TInt tmp = 0;
        if ( keyCount > 0 )
            {
            tmp = keys[keyCount - 1] + 1;
            }
        else
            {
            tmp = KUNSAFProtocolsDomainTableMask;
            }

        DBG_PRINT_2( "CWPNATFWItem::StoreL - Domain key is %d", tmp );
        // Before adding check whether there already are settings
        // for this domain.
        rep->FindEqL( KUNSAFProtocolsDomainMask, KUNSAFProtocolsFieldTypeMask,
            iDomainItems[counter]->GetDomain(), keys );
        TBool updateValue ( EFalse );
        if ( keys.Count() )
            {
            tmp = keys[0];
            updateValue = ETrue;
            }

        if ( !updateValue )
            {
            // Domain
            TUint32 currentDomainKey( KErrNone );
            currentDomainKey = tmp|KUNSAFProtocolsDomainMask;
            currentDomainKey &= KUNSAFProtocolsKeyMask;

            User::LeaveIfError( rep->Create(
                currentDomainKey|KUNSAFProtocolsDomainMask,
                iDomainItems[counter]->GetDomain() ) );

            // Append to saved keys if creation was successful.
            tmpSavedata->Des().AppendNumFixedWidth( currentDomainKey|
                KUNSAFProtocolsDomainMask, EDecimal, KMaxCharsInTUint32 );

            // STUN
            CreateStunSettingsL( rep, counter, tmp, tmpSavedata );

            // ICE
            CreateIceSettingsL( rep, counter, tmp );

            // TURN
            CreateTurnSettingsL( rep, counter, tmp );
            }
        else
            {
            // Domain
            TUint32 currentDomainKey( KErrNone );
            currentDomainKey = tmp|KUNSAFProtocolsDomainMask;
            currentDomainKey &= KUNSAFProtocolsKeyMask;

            User::LeaveIfError( rep->Set( 
                currentDomainKey|KUNSAFProtocolsDomainMask,
                iDomainItems[counter]->GetDomain() ) );

            tmpSavedata->Des().AppendNumFixedWidth(
                currentDomainKey|KUNSAFProtocolsDomainMask, 
                EDecimal, KMaxCharsInTUint32 );

            // STUN
            UpdateStunSettingsL( rep, counter, tmp, tmpSavedata );

            // ICE
            UpdateIceSettingsL( rep, counter, tmp );

            // TURN
            UpdateTurnSettingsL( rep, counter, tmp );
            }

        keys.Close();
        CleanupStack::PopAndDestroy( &keys ); // CS:2

        DBG_PRINT( "CWPNATFWItem::StoreL - DomainSpecific settings saved." );
        }

    // ==============================
    // IAPSpecific 
    // add commands 
    // ==============================
    //

    CreateIapSettingsL( rep, tmpSavedata );

    DBG_PRINT( "CWPNATFWItem::StoreL - IAP specific settings saved." );

    // set whole savedata for possibility to delete settings afterwards.    
    if ( tmpSavedata->Length() )
        {
        delete iSaveData;
        iSaveData = NULL;
        CleanupStack::Pop( tmpSavedata );
        iSaveData = tmpSavedata;
        tmpSavedata = NULL;
        }
    else
        {
        CleanupStack::PopAndDestroy( tmpSavedata ); // CS:1
        }
    CleanupStack::PopAndDestroy( rep ); // CS:0

    DBG_PRINT( "CWPNATFWItem::StoreL - end" );
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::SaveData
//
// ---------------------------------------------------------------------------
//
const TDesC8& CWPNATFWItem::SaveData() const
    {
    // In iSaveData we have all keys that are created/updated.
    return *iSaveData;
    }

// ---------------------------------------------------------------------------
// CWPNATFWItem::SavedDomainId
//
// ---------------------------------------------------------------------------
//
const TDesC8& CWPNATFWItem::SavedDomainId() const
    {
    return *iSavedDomainId;
    }

//  End of File
