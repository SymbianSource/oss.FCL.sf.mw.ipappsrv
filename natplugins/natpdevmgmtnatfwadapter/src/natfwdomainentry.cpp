/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device Management NAT-FW Traversal domain specific settings
*
*/


// INCLUDE FILES
#include    <e32base.h>
#include    "natfwdomainentry.h"
#include    "natfwdefaults.h"

const TInt KNotSet     = -1;
_LIT8( KValueNotSet, "NotSet" );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::NewL
// 
// ---------------------------------------------------------------------------
//
CNATFWDomainEntry* CNATFWDomainEntry::NewL()
    {
    CNATFWDomainEntry* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::NewLC
// 
// ---------------------------------------------------------------------------
//
CNATFWDomainEntry* CNATFWDomainEntry::NewLC()
    {
    CNATFWDomainEntry* self = new (ELeave) CNATFWDomainEntry();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::CNATFWDomainEntry
//
// ---------------------------------------------------------------------------
//
CNATFWDomainEntry::CNATFWDomainEntry()
    {
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::ConstructL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::ConstructL()
    {
    iDomain                  = HBufC8::NewL( 0 );
    iSTUNServerAddr          = HBufC8::NewL( 0 );
    iSTUNServerPort          = KDefaultSTUNPort;
    iNATRefreshTCP           = KDefaultTCPRefreshInterval;
    iNATRefreshUDP           = KDefaultUDPRefreshInterval;
    iSTUNUsername            = HBufC8::NewL( 0 );
    iSTUNPassword            = HBufC8::NewL( 0 );
    iDisableStunSharedSecret = KNotSet;
    iStartPortRange          = KNotSet;
    iEndPortRange            = KNotSet;
    iUsedNatProtocol         = HBufC8::NewL( 0 );
    iNatUtilities            = HBufC8::NewL( 0 );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::~CNATFWDomainEntry
//
// ---------------------------------------------------------------------------
//
CNATFWDomainEntry::~CNATFWDomainEntry()
    {
    delete iDomain;
    delete iSTUNServerAddr;
    delete iSTUNUsername;
    delete iSTUNPassword;
    delete iUsedNatProtocol;
    delete iNatUtilities;
    iStunSrvAddrArray.ResetAndDestroy();
    iStunSrvPortArray.Close();
    iStunUsernameArray.ResetAndDestroy();
    iStunPasswordArray.ResetAndDestroy();
    iTurnSrvAddrArray.ResetAndDestroy();
    iTurnSrvPortArray.Close();
    iTurnUsernameArray.ResetAndDestroy();
    iTurnPasswordArray.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetDomainKey
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetDomainKey( TUint32 aDomainKey )
    {
    iDomainKey = aDomainKey;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetDomainL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetDomainL( const TDesC8& aDomain )
    {
    HBufC8* temp = aDomain.AllocL();
    delete iDomain;
    iDomain = NULL;
    iDomain = temp;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetSTUNServerAddrL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetSTUNServerAddrL( const TDesC8& aSTUNServerAddr )
    {
    HBufC8* temp = aSTUNServerAddr.AllocL();
    delete iSTUNServerAddr;
    iSTUNServerAddr = NULL;
    iSTUNServerAddr = temp;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetSTUNServerPort
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetSTUNServerPort( TUint aSTUNServerPort )
    {
    iSTUNServerPort = aSTUNServerPort;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetNATRefreshTCP
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetNATRefreshTCP( TInt aNATRefreshTCP )
    {
    iNATRefreshTCP = aNATRefreshTCP;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetNATRefreshUDP
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetNATRefreshUDP( TInt aNATRefreshUDP )
    {
    iNATRefreshUDP = aNATRefreshUDP;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetEnableCRLFRefresh
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetEnableCRLFRefresh( TInt aEnableCRLFRefresh )
    {
    iEnableCRLFRefresh = aEnableCRLFRefresh;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetDomain
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::GetDomain() const
    {
    return *iDomain;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetDomainKey
//
// ---------------------------------------------------------------------------
//
TUint32 CNATFWDomainEntry::GetDomainKey() const
    {
    return iDomainKey;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetSTUNAddr
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::GetSTUNAddr() const
    {
    return *iSTUNServerAddr;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetSTUNPort
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::GetSTUNPort() const
    {
    return iSTUNServerPort;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetNATRefreshTCP
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::GetNATRefreshTCP() const
    {
    return iNATRefreshTCP;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetNATRefreshUDP
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::GetNATRefreshUDP() const
    {
    return iNATRefreshUDP;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetEnableCRLFRefresh
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::GetEnableCRLFRefresh() const
    {
    return iEnableCRLFRefresh;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetSTUNUsernameL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetSTUNUsernameL( const TDesC8& aSTUNUsername )
    {
    HBufC8* temp = aSTUNUsername.AllocL();
    delete iSTUNUsername;
    iSTUNUsername = NULL;
    iSTUNUsername = temp;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetSTUNPasswordL
//
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetSTUNPasswordL( const TDesC8& aSTUNPassword )
    {
    HBufC8* temp = aSTUNPassword.AllocL();
    delete iSTUNPassword;
    iSTUNPassword = NULL;
    iSTUNPassword = temp;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddStunSrvAddrL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddStunSrvAddrL( const TDesC8& aStunSrvAddr )
    {
    HBufC8* temp = aStunSrvAddr.AllocLC();
    iStunSrvAddrArray.AppendL( temp );
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddStunSrvPortL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddStunSrvPortL( TInt aStunSrvPort )
    {
    iStunSrvPortArray.AppendL( aStunSrvPort );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddStunUsernameL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddStunUsernameL( const TDesC8& aStunUsername )
    {
    HBufC8* temp = aStunUsername.AllocLC();
    iStunUsernameArray.AppendL( temp );
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddStunPasswordL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddStunPasswordL( const TDesC8& aStunPassword )
    {
    HBufC8* temp = aStunPassword.AllocLC();
    iStunPasswordArray.AppendL( temp );
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetDisableStunSharedSecret
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetDisableStunSharedSecret( 
    TInt aDisableStunSharedSecret )
    {
    iDisableStunSharedSecret = aDisableStunSharedSecret;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetStartPortRange
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetStartPortRange( TInt aStartPortRange )
    {
    iStartPortRange = aStartPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetEndPortRange
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetEndPortRange( TInt aEndPortRange )
    {
    iEndPortRange = aEndPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::SetNatProtocolL
// 
// ---------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetNatProtocolL( const TDesC8& aNatProtocol )
    {
    HBufC8* temp = aNatProtocol.AllocL();
    delete iUsedNatProtocol;
    iUsedNatProtocol = NULL;
    iUsedNatProtocol = temp;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetNatUtilitiesL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetNatUtilitiesL( const TDesC8& aNatUtilities )
    {
    HBufC8* temp = aNatUtilities.AllocL();
    delete iNatUtilities;
    iNatUtilities = NULL;
    iNatUtilities = temp;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetHostPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetHostPref( TInt aHostPref )
    {
    iHostPref = aHostPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetServerReflexPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetServerReflexPref( TInt aServerReflexPref )
    {
    iServerReflexPref = aServerReflexPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetRelayPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetRelayPref( TInt aRelayPref )
    {
    iRelayPref = aRelayPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetPeerReflexPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetPeerReflexPref( TInt aPeerReflexPref )
    {
    iPeerReflexPref = aPeerReflexPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetIPv4Pref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetIPv4Pref( TInt aIPv4Pref )
    {
    iIPv4Pref = aIPv4Pref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetIPv6Pref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetIPv6Pref( TInt aIPv6Pref )
    {
    iIPv6Pref = aIPv6Pref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetVpnPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetVpnPref( TInt aVpnPref )
    {
    iVpnPref = aVpnPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetUdpPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetUdpPref( TInt aUdpPref )
    {
    iUdpPref = aUdpPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetTcpPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetTcpPref( TInt aTcpPref )
    {
    iTcpPref = aTcpPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetTcpActivePref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetTcpActivePref( TInt aTcpActivePref )
    {
    iTcpActivePref = aTcpActivePref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetTcpPassivePref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetTcpPassivePref( TInt aTcpPassivePref )
    {
    iTcpPassivePref = aTcpPassivePref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::SetTcpSimultPref
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::SetTcpSimultPref( TInt aTcpSimultPref )
    {
    iTcpSimultPref = aTcpSimultPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddTurnSrvAddrL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddTurnSrvAddrL( const TDesC8& aTurnSrvAddr )
    {
    HBufC8* temp = aTurnSrvAddr.AllocLC();
    iTurnSrvAddrArray.AppendL( temp );
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddTurnSrvPortL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddTurnSrvPortL( TInt aTurnSrvPort )
    {
    iTurnSrvPortArray.AppendL( aTurnSrvPort );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddTurnUsernameL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddTurnUsernameL( const TDesC8& aTurnUsername )
    {
    HBufC8* temp = aTurnUsername.AllocLC();
    iTurnUsernameArray.AppendL( temp );
    CleanupStack::Pop( temp );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::AddTurnPasswordL
//
// ----------------------------------------------------------------------------
//
void CNATFWDomainEntry::AddTurnPasswordL( const TDesC8& aTurnPassword )
    {
    HBufC8* temp = aTurnPassword.AllocLC();
    iTurnPasswordArray.AppendL( temp );
    CleanupStack::Pop( temp );
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateStunSrvAddrL()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateStunSrvAddrL( TInt aIndex, const TDesC8& aAddr )
    {
    if ( aIndex < 0 || aIndex >= iStunSrvAddrArray.Count() )
        {
        return KErrArgument;
        }
    HBufC8* temp = aAddr.AllocLC();
    delete iStunSrvAddrArray[ aIndex ];
    iStunSrvAddrArray.Remove( aIndex );
    iStunSrvAddrArray.Compress();
    iStunSrvAddrArray.InsertL( temp, aIndex );
    CleanupStack::Pop( temp );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateStunSrvPort()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateStunSrvPort( TInt aIndex, TInt aPort )
    {
    if ( aIndex < 0 || aIndex >= iStunSrvPortArray.Count() )
        {
        return KErrArgument;
        }
    iStunSrvPortArray[aIndex] = aPort;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateStunUsernameL()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateStunUsernameL( TInt aIndex, 
    const TDesC8& aUsername )
    {
    if ( aIndex < 0 || aIndex >= iStunUsernameArray.Count() )
        {
        return KErrArgument;
        }
    HBufC8* temp = aUsername.AllocLC();
    delete iStunUsernameArray[ aIndex ];
    iStunUsernameArray.Remove( aIndex );
    iStunUsernameArray.Compress();
    iStunUsernameArray.InsertL( temp, aIndex );
    CleanupStack::Pop( temp );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateStunPasswordL()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateStunPasswordL( TInt aIndex,
    const TDesC8& aPassword )
    {
    if ( aIndex < 0 || aIndex >= iStunPasswordArray.Count() )
        {
        return KErrArgument;
        }
    HBufC8* temp = aPassword.AllocLC();
    delete iStunPasswordArray[ aIndex ];
    iStunPasswordArray.Remove( aIndex );
    iStunPasswordArray.Compress();
    iStunPasswordArray.InsertL( temp, aIndex );
    CleanupStack::Pop( temp );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateTurnSrvAddrL()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateTurnSrvAddrL( TInt aIndex, const TDesC8& aAddr )
    {
    if ( aIndex < 0 || aIndex >= iTurnSrvAddrArray.Count() )
        {
        return KErrArgument;
        }
    HBufC8* temp = aAddr.AllocLC();
    delete iTurnSrvAddrArray[ aIndex ];
    iTurnSrvAddrArray.Remove( aIndex );
    iTurnSrvAddrArray.Compress();
    iTurnSrvAddrArray.InsertL( temp, aIndex );
    CleanupStack::Pop( temp );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateTurnSrvPort()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateTurnSrvPort( TInt aIndex, TInt aPort )
    {
    if ( aIndex < 0 || aIndex >= iTurnSrvPortArray.Count() )
        {
        return KErrArgument;
        }
    iTurnSrvPortArray[aIndex] = aPort;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateTurnUsernameL()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateTurnUsernameL( TInt aIndex,
    const TDesC8& aUsername )
    {
    if ( aIndex < 0 || aIndex >= iTurnUsernameArray.Count() )
        {
        return KErrArgument;
        }
    HBufC8* temp = aUsername.AllocLC();
    delete iTurnUsernameArray[ aIndex ];
    iTurnUsernameArray.Remove( aIndex );
    iTurnUsernameArray.Compress();
    iTurnUsernameArray.InsertL( temp, aIndex );
    CleanupStack::Pop( temp );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UpdateTurnPasswordL()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UpdateTurnPasswordL( TInt aIndex,
    const TDesC8& aPassword )
    {
    if ( aIndex < 0 || aIndex >= iTurnPasswordArray.Count() )
        {
        return KErrArgument;
        }
    HBufC8* temp = aPassword.AllocLC();
    delete iTurnPasswordArray[ aIndex ];
    iTurnPasswordArray.Remove( aIndex );
    iTurnPasswordArray.Compress();
    iTurnPasswordArray.InsertL( temp, aIndex );
    CleanupStack::Pop( temp );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveStunSrvAddr()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveStunSrvAddr( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iStunSrvAddrArray.Count() )
        {
        return KErrArgument;
        }
    delete iStunSrvAddrArray[ aIndex ];
    iStunSrvAddrArray.Remove( aIndex );
    iStunSrvAddrArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveStunSrvPort()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveStunSrvPort( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iStunSrvPortArray.Count() )
        {
        return KErrArgument;
        }
    iStunSrvPortArray.Remove( aIndex );
    iStunSrvPortArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveStunUsername()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveStunUsername( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iStunUsernameArray.Count() )
        {
        return KErrArgument;
        }
    delete iStunUsernameArray[ aIndex ];
    iStunUsernameArray.Remove( aIndex );
    iStunUsernameArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveStunPassword()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveStunPassword( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iStunPasswordArray.Count() )
        {
        return KErrArgument;
        }
    delete iStunPasswordArray[ aIndex ];
    iStunPasswordArray.Remove( aIndex );
    iStunPasswordArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveTurnSrvAddr()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveTurnSrvAddr( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iTurnSrvAddrArray.Count() )
        {
        return KErrArgument;
        }
    delete iTurnSrvAddrArray[ aIndex ];
    iTurnSrvAddrArray.Remove( aIndex );
    iTurnSrvAddrArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveTurnSrvPort()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveTurnSrvPort( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iTurnSrvPortArray.Count() )
        {
        return KErrArgument;
        }
    iTurnSrvPortArray.Remove( aIndex );
    iTurnSrvPortArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveTurnUsername()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveTurnUsername( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iTurnUsernameArray.Count() )
        {
        return KErrArgument;
        }
    delete iTurnUsernameArray[ aIndex ];
    iTurnUsernameArray.Remove( aIndex );
    iTurnUsernameArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::RemoveTurnPassword()
//
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RemoveTurnPassword( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iTurnPasswordArray.Count() )
        {
        return KErrArgument;
        }
    delete iTurnPasswordArray[ aIndex ];
    iTurnPasswordArray.Remove( aIndex );
    iTurnPasswordArray.Compress();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetSTUNUsername
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::GetSTUNUsername() const
    {
    return *iSTUNUsername;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::GetSTUNPassword
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::GetSTUNPassword() const
    {
    return *iSTUNPassword;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::StunSrvAddrL
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::StunSrvAddrL( TInt aIndex ) const
    {
    if ( aIndex < iStunSrvAddrArray.Count() && aIndex >= 0 )
        {
        return *( iStunSrvAddrArray[aIndex] );
        }
    return *( KValueNotSet().AllocL() );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::StunSrvPort
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::StunSrvPort( TInt aIndex ) const
    {
    if ( aIndex < iStunSrvPortArray.Count() && aIndex >= 0 )
        {
        return iStunSrvPortArray[aIndex];
        }
    return KErrNotFound;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::StunUsernameL
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::StunUsernameL( TInt aIndex ) const
    {
    if ( aIndex < iStunUsernameArray.Count() && aIndex >= 0 )
        {
        return *( iStunUsernameArray[aIndex] );
        }
    return *( KValueNotSet().AllocL() );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::StunPasswordL
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::StunPasswordL( TInt aIndex ) const
    {
    if ( aIndex < iStunPasswordArray.Count() && aIndex >= 0 )
        {
        return *( iStunPasswordArray[aIndex] );
        }
    return *( KValueNotSet().AllocL() );
    }


// ----------------------------------------------------------------------------
// CNATFWDomainEntry::DisableStunSharedSecret
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::DisableStunSharedSecret() const
    {
    return iDisableStunSharedSecret;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::StartPortRange
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::StartPortRange() const
    {
    return iStartPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::EndPortRange
// 
// ---------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::EndPortRange() const
    {
    return iEndPortRange;
    }

// ---------------------------------------------------------------------------
// CNATFWDomainEntry::UsedNatProtocol
// 
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::UsedNatProtocol() const
    {
    return *iUsedNatProtocol;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::NatUtilities
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::NatUtilities() const
    {
    return *iNatUtilities;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::HostPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::HostPref() const
    {
    return iHostPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::ServerReflexPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::ServerReflexPref() const
    {
    return iServerReflexPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::RelayPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::RelayPref() const
    {
    return iRelayPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::PeerReflexPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::PeerReflexPref() const
    {
    return iPeerReflexPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::IPv4Pref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::IPv4Pref() const
    {
    return iIPv4Pref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::IPv6Pref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::IPv6Pref() const
    {
    return iIPv6Pref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::VpnPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::VpnPref() const
    {
    return iVpnPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::UdpPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::UdpPref() const
    {
    return iUdpPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TcpPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::TcpPref() const
    {
    return iTcpPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TcpActivePref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::TcpActivePref() const
    {
    return iTcpActivePref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TcpPassivePref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::TcpPassivePref() const
    {
    return iTcpPassivePref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TcpSimultPref
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::TcpSimultPref() const
    {
    return iTcpSimultPref;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TurnSrvAddrL
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::TurnSrvAddrL( TInt aIndex ) const
    {
    if ( aIndex < iTurnSrvAddrArray.Count() && aIndex >= 0 )
        {
        return *( iTurnSrvAddrArray[aIndex] );
        }
    return *( KValueNotSet().AllocL() );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TurnSrvPort
//
// ----------------------------------------------------------------------------
//
TInt CNATFWDomainEntry::TurnSrvPort( TInt aIndex ) const
    {
    if ( aIndex < iTurnSrvPortArray.Count() && aIndex >= 0 )
        {
        return iTurnSrvPortArray[aIndex];
        }
    return KErrNotFound;
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TurnUsernameL
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::TurnUsernameL( TInt aIndex ) const
    {
    if ( aIndex < iTurnUsernameArray.Count() && aIndex >= 0 )
        {
        return *( iTurnUsernameArray[aIndex] );
        }
    return *( KValueNotSet().AllocL() );
    }

// ----------------------------------------------------------------------------
// CNATFWDomainEntry::TurnPasswordL
//
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWDomainEntry::TurnPasswordL( TInt aIndex ) const
    {
    if ( aIndex < iTurnPasswordArray.Count() && aIndex >= 0 )
        {
        return *( iTurnPasswordArray[aIndex] );
        }
    return *( KValueNotSet().AllocL() );
    }

//  End of File  
