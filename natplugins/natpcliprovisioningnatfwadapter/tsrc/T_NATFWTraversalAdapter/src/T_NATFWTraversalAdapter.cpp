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
* Description:
*
*/


//  CLASS HEADER
#include "T_NATFWTraversalAdapter.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <f32file.h>

// Central Repository keys for NAT-FW Traversal settings
#include <unsafprotocolscrkeys.h> // VoIP rel 2.1

#include "centralrepository.h"
#include "CWPEngine.h"
#include "BADESCA.H"
#include "natfwdomainspecificitem.h"
#include "natfwiapspecificitem.h"

#include "wpnatfwdebug.h"

#include "natfwdefaults.h"
#include "stunserveritem.h"
#include "turnserveritem.h"
#include "wpnatfwitem.h"


_LIT8( KDomain,                "domain3.com" );
_LIT8( KStunSrvAddr,           "stunserver.domain.com" );

const TInt KStunSrvPort         = 3478;
const TInt KCrlfRefresh         = 1;
const TInt KDomainNATRefreshTcp = 10;
const TInt KDomainNATRefreshUdp = 20;
const TInt KNATIAPRefreshTcp    = 30;
const TInt KNATIAPRefreshUdp    = 40;
const TInt KSTUNRetransmit      = 100;

_LIT8( KStunSrvAddr2,          "stun3.domain.com" );
const TInt KStunSrvPort2     = 4808;
const TInt KStunSrvPort3     = 4818;
const TInt KStartPort        = 49153;
const TInt KEndPort          = 49205;
_LIT8( KUsedNATProtocol,      "nokia.ice,nokia.stun" );

// ICESpecific
_LIT8( KUsedNatUtilities,     "nokia.stun,nokia.turn" );
const TInt KHostPref         = 127;
const TInt KServerRefPref    = 99;
const TInt KRelayPref        = 1;
const TInt KPeerRefPref      = 109;
const TInt KIPv4Pref         = 60;
const TInt KIPv6Pref         = 40;
const TInt KVPNPref          = 20;
const TInt KUDPPref          = 2;
const TInt KTCPPref          = 1;
const TInt KTCPActivePref    = 59;
const TInt KTCPPassivePref   = 39;
const TInt KTCPSimultPref    = 19;

// TURNSpecific
_LIT8( KTurnSrvAddr,          "turn1.domain.com" );
_LIT8( KTurnSrvAddr2,         "turn2.domain.com" );
_LIT8( KTurnSrvAddr3,         "turn3.domain.com" );
const TInt KTurnSrvPort      = 3579;
const TInt KTurnSrvPort2     = 3580;
const TInt KTurnSrvPort3     = 3581;
_LIT8( KTurnUsername,         "kaaleppi" );
_LIT8( KTurnUsername3,        "pyylevi" );
_LIT8( KTurnPassword,         "turn6baa" );
_LIT8( KTurnPassword3,        "hababoo" );
_LIT8( KNwStunUsername,       "nwstunuser" );
_LIT8( KNwStunPassword,       "nwstunpw" );
_LIT8( KAddStunUsername,      "addstunuser" );
_LIT8( KAddStunPassword,      "addstunpw" );


// Constants for testing updating domain specific settings:
_LIT8( KUpdateStunSrvAddr,          "stunniserveri.stunnidomaini.com" );
const TInt KUpdateStunSrvPort      = 3479;
const TInt KUpdateNATRefreshTcp    = 12;
const TInt KUpdateNATRefreshUdp    = 25;
const TInt KUpdateCrlfRefresh      = 0;
_LIT8( KUpdateNwStunUsername,       "JuhoKusti" );
_LIT8( KUpdateNwStunPassword,       "Paasikivi" );
const TInt KUpdateStartPort        = 49202;
const TInt KUpdateEndPort          = 49219;
_LIT8( KUpdatedNATProtocol,         "nokia.ice" );
_LIT8( KUpdateAddStunSrvAddr,       "stunniserveri2.stunnidomaini.com" );
const TInt KUpdateStunSrvPort2     = 4810;
_LIT8( KUpdateAddStunUsername,      "KaarloJuho" );
_LIT8( KUpdateAddStunPassword,      "Stahlberg" );

// ICESpecific
_LIT8( KUpdateUsedNatUtilities,     "nokia.turn,nokia.stun" );
const TInt KUpdateHostPref         = 124;
const TInt KUpdateServerRefPref    = 101;
const TInt KUpdateRelayPref        = 3;
const TInt KUpdatePeerRefPref      = 111;
const TInt KUpdateIPv4Pref         = 65;
const TInt KUpdateIPv6Pref         = 45;
const TInt KUpdateVPNPref          = 25;
const TInt KUpdateUDPPref          = 3;
const TInt KUpdateTCPPref          = 2;
const TInt KUpdateTCPActivePref    = 61;
const TInt KUpdateTCPPassivePref   = 41;
const TInt KUpdateTCPSimultPref    = 21;

// TURNSpecific
_LIT8( KUpdateTurnSrvAddr,          "turniserveri.domain.com" );
const TInt KUpdateTurnSrvPort      = 3581;
_LIT8( KUpdateTurnUsername,         "UrhoKaleva" );
_LIT8( KUpdateTurnPassword,         "Kekkonen" );
_LIT8( KUpdateTurnSrvAddr2,         "turniserveri2.domain.com" );
const TInt KUpdateTurnSrvPort2     = 3583;
_LIT8( KUpdateTurnUsername2,        "LauriKristian" );
_LIT8( KUpdateTurnPassword2,        "Relander" );

// CONSTRUCTION
T_NATFWTraversalAdapter* T_NATFWTraversalAdapter::NewL()
    {
    T_NATFWTraversalAdapter* self = T_NATFWTraversalAdapter::NewLC();
    CleanupStack::Pop( self );

    return self;
    }

T_NATFWTraversalAdapter* T_NATFWTraversalAdapter::NewLC()
    {
    T_NATFWTraversalAdapter* self = new( ELeave ) T_NATFWTraversalAdapter();
    CleanupStack::PushL( self );

    self->ConstructL();
 
    return self;
    }

// Destructor (virtual by CBase)
T_NATFWTraversalAdapter::~T_NATFWTraversalAdapter()
    {
    iFs.Close();
    }

// Default constructor
T_NATFWTraversalAdapter::T_NATFWTraversalAdapter()
    : iProvData( NULL, 0 )
    {
    }

// Second phase construct
void T_NATFWTraversalAdapter::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    User::LeaveIfError( iFs.Connect() );
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void T_NATFWTraversalAdapter::SetupL(  )
    {
    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols );

    iNATFWDomainSpecificItem = CNATFWDomainSpecificItem::NewL();
    iNATFWIAPSpecificItem = CNATFWIAPSpecificItem::NewL();


    //////////////////////////////
    ////////// TMTEST ////////////
    //////////////////////////////
    /*_LIT8( KTesti, "101kakka,3,2,1" );
    TLex8 leksaaja( KTesti() );
    TInt arvoitus( KErrNotFound );
    TInt virhe = leksaaja.Val( arvoitus );
    DBG_PRINT_2( "Timo testaa: virhe = %d", virhe );
    DBG_PRINT_2( "Timo testaa: arvoitus = %d", arvoitus );*/
    //////////////////////////////
    //////// TMTEST END //////////
    //////////////////////////////


    // Load stored data data if exist
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    TInt err = rep->FindL( KUNSAFProtocolsDomainMask, 
        KUNSAFProtocolsFieldTypeMask, keys ); 
    if ( keys.Count() )
        {
        GetDomainSettingsL( keys[0], *iNATFWDomainSpecificItem );
        }
    err = rep->FindL( KUNSAFProtocolsIAPIdMask, 
        KUNSAFProtocolsFieldTypeMask, keys ); 
    
    if ( keys.Count() )
        {
        GetIAPSettingsL( keys[0], *iNATFWIAPSpecificItem );
        }

    CleanupStack::PopAndDestroy( &keys );
    CleanupStack::PopAndDestroy( rep );
    }

void T_NATFWTraversalAdapter::Teardown(  )
    {
    if ( iProvisioningDoc )
        {
        delete iProvisioningDoc;
        iProvisioningDoc = NULL;
        }
    if ( iNATFWIAPSpecificItem )
        {
        delete iNATFWIAPSpecificItem;
        iNATFWIAPSpecificItem = NULL;
        }
    if ( iNATFWDomainSpecificItem )
        {
        delete iNATFWDomainSpecificItem;
        iNATFWDomainSpecificItem = NULL;  
        }
    }

void T_NATFWTraversalAdapter::StoreL(  )
    {
    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols );
    rep->Reset();
    
    _LIT( KFileName, "c:\\data\\NATFW_AP_BOOTSTRAP_2.wbxml" );
    
    EUNIT_ASSERT_NO_LEAVE( SaveDocumentL( KFileName() ) );

    CleanupStack::PopAndDestroy( rep );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetDomain(  )
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetDomain() == KDomain );
    }
        
void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetSTUNAddr(  )
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNAddr() == KStunSrvAddr );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetSTUNPort(  )
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNPort() == KStunSrvPort );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetNATRefreshTCP(  )
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetNATRefreshTCP() == 
        KDomainNATRefreshTcp );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetNATRefreshUDP(  )
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetNATRefreshUDP() == 
        KDomainNATRefreshUdp );
    }
    
void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetEnableCRLFRefresh(  )
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetEnableCRLFRefresh() == 
        KCrlfRefresh );
    }

void T_NATFWTraversalAdapter::NATFWIAPSpecificItem_GetIntervalUDP(  )
    {
    EUNIT_ASSERT( iNATFWIAPSpecificItem->GetIntervalUDP() == 
        KNATIAPRefreshUdp );
    }
    
void T_NATFWTraversalAdapter::NATFWIAPSpecificItem_GetIntervalTCP(  )
    {
    EUNIT_ASSERT( iNATFWIAPSpecificItem->GetIntervalTCP() == 
        KNATIAPRefreshTcp );
    }

void T_NATFWTraversalAdapter::NATFWIAPSpecificItem_GetSTUNRetransmitTimer(  )
    {
    EUNIT_ASSERT( iNATFWIAPSpecificItem->GetSTUNRetransmitTimer() == 
        KSTUNRetransmit );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetSTUNUsername()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNUsername() == 
        KNwStunUsername );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_GetSTUNPassword()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNPassword() == 
        KNwStunPassword );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_StunSrvAddrL()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunSrvAddr() == 
        KNullDesC8 );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[1]->StunSrvAddr() == 
        KStunSrvAddr2 );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_StunSrvPort()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunSrvPort() == 
        KStunSrvPort2 );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[1]->StunSrvPort() == 
        KStunSrvPort3 );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_StunSrvUsernameL()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunUsername() ==
        KAddStunUsername );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_StunSrvPasswordL()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunPassword() ==
        KAddStunPassword );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_DisableStunSharedSecret()
    {
    // STUN shared secret is set on, i.e. it is not disabled.
    EUNIT_ASSERT( iNATFWDomainSpecificItem->DisableStunSharedSecret() == 
        TInt( EFalse ) );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_StartPortRange()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->StartPortRange() == KStartPort );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_EndPortRange()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->EndPortRange() == KEndPort );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_UsedNatProtocol()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->UsedNatProtocol() == 
        KUsedNATProtocol );
    }

// ICE specific settings start.
void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_NatUtilities()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->NatUtilities() == 
        KUsedNatUtilities );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_HostPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->HostPref() == KHostPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_ServerRefPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->ServerRefPref() == 
        KServerRefPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_RelayPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->RelayPref() == KRelayPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_PeerRefPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->PeerRefPref() == KPeerRefPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_IPv4Pref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->IPv4Pref() == KIPv4Pref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_IPv6Pref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->IPv6Pref() == KIPv6Pref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_VpnPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->VpnPref() == KVPNPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_UdpPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->UdpPref() == KUDPPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TcpPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpPref() == KTCPPref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TcpActivePref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpActivePref() == 
        KTCPActivePref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TcpPassivePref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpPassivePref() == 
        KTCPPassivePref );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TcpSimultPref()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpSimultPref() == 
        KTCPSimultPref );
    }
// ICE specific settings end.
// TURN specific settings start.
void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TurnSrvAddrL()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnSrvAddr() 
        == KTurnSrvAddr );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnSrvAddr() 
        == KTurnSrvAddr2 );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[2]->TurnSrvAddr() 
        == KTurnSrvAddr3 );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TurnSrvPort()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnSrvPort() 
        == KTurnSrvPort );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnSrvPort() 
        == KTurnSrvPort2 );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[2]->TurnSrvPort() 
        == KTurnSrvPort3 );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TurnUsernameL()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnUsername() 
        == KTurnUsername );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnUsername() 
        == KNullDesC8 );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[2]->TurnUsername() 
        == KTurnUsername3 );
    }

void T_NATFWTraversalAdapter::NATFWDomainSpecificItem_TurnPasswordL()
    {
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnPassword() 
        == KTurnPassword );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnPassword() 
        == KNullDesC8 );
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[2]->TurnPassword() 
        == KTurnPassword3 );
    }
// TURN specific settings end.

void T_NATFWTraversalAdapter::SaveDocumentL( const TDesC16& aFilename )
    {
    User::LeaveIfError( iFile.Open( iFs, aFilename, EFileShareReadersOnly ) );

    TInt fileSize ( 0 );
    iFile.Size( fileSize );

    delete iProvisioningDoc;
    iProvisioningDoc = NULL;
    iProvisioningDoc = HBufC8::NewL( fileSize );
    iProvData.Set( iProvisioningDoc->Des() );
    
	User::LeaveIfError( iFile.Read( iProvData ) );
	iFile.Close();

    // The ProvisioningEngine 
    //__UHEAP_MARK;
    CWPEngine* engine = CWPEngine::NewL();
    CleanupStack::PushL( engine );
    engine->ImportDocumentL( iProvData );
    engine->PopulateL();

    // Save OMA message
    for ( TInt i = 0; i < engine->ItemCount(); i++ )
        {
        engine->SaveL( i );
        }
    CleanupStack::PopAndDestroy( engine );
    //__UHEAP_MARKEND;
    }
    
// ---------------------------------------------------------------------------
// T_NATFWTraversalAdapter::GetDomainSettingsL
// Gets one domain settings entry
// ---------------------------------------------------------------------------
//
void T_NATFWTraversalAdapter::GetDomainSettingsL( TUint32 aDomainKeyId,
    CNATFWDomainSpecificItem& aItem )
    {
    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
    
    TUint32 key( KErrNone );
    // select domain 
    key = ( KUNSAFProtocolsDomainMask^( aDomainKeyId ) );
    key |= KUNSAFProtocolsDomainTableMask;
    // set key id
    aItem.SetDomainKey( aDomainKeyId );
    
    // set domain
    TInt actualLength = 0;
    TBuf8<1> tmp;
    rep->Get( KUNSAFProtocolsDomainMask|key, tmp, actualLength );
    
    if ( actualLength > 0 )
        {
        HBufC8* domainName = HBufC8::NewLC( actualLength );
        TPtr8 ptr( domainName->Des() );
        TInt err = rep->Get( KUNSAFProtocolsDomainMask|key, ptr ); 
        if( KErrNone == err )
            {
            aItem.SetDomainL( domainName->Des() );
            }
        else
            {
            aItem.SetDomainL( KNullDesC8() );
            }
        CleanupStack::PopAndDestroy( domainName );
        actualLength = 0;
        }

    // set STUN server address
    rep->Get( KUNSAFProtocolsSTUNServerMask|key, tmp, actualLength );
    
    if ( actualLength > 0 )
        {
        HBufC8* stunServerAddr = HBufC8::NewLC( actualLength );
        TPtr8 ptr ( stunServerAddr->Des() );
        TInt err = rep->Get( KUNSAFProtocolsSTUNServerMask|key, ptr );
        if( err == KErrNone ) 
            {
            aItem.SetSTUNServerAddrL( stunServerAddr->Des() );
            }
        else
            {
            aItem.SetSTUNServerAddrL( KNullDesC8() );
            }
        CleanupStack::PopAndDestroy( stunServerAddr );
        }
    
    // STUN server port
    TInt tmpValue( 0 );
    rep->Get( KUNSAFProtocolsSTUNServerPortMask|key, tmpValue );
    aItem.SetSTUNServerPort( tmpValue );

    // STUN server username
    rep->Get( KUNSAFProtocolsSTUNUsernameMask|key, tmp, actualLength );
    
    if ( actualLength > 0 )
        {
        HBufC8* stunUsername = HBufC8::NewLC( actualLength );
        TPtr8 ptr ( stunUsername->Des() );
        TInt err = rep->Get( KUNSAFProtocolsSTUNUsernameMask|key, ptr );
        if( err == KErrNone ) 
            {
            aItem.SetSTUNUsernameL( stunUsername->Des() );
            }
        else
            {
            aItem.SetSTUNUsernameL( KNullDesC8() );
            }
        CleanupStack::PopAndDestroy( stunUsername );
        }

    // STUN server password
    rep->Get( KUNSAFProtocolsSTUNPasswordMask|key, tmp, actualLength );
    
    if ( actualLength > 0 )
        {
        HBufC8* stunPassword = HBufC8::NewLC( actualLength );
        TPtr8 ptr ( stunPassword->Des() );
        TInt err = rep->Get( KUNSAFProtocolsSTUNPasswordMask|key, ptr );
        if( err == KErrNone ) 
            {
            aItem.SetSTUNPasswordL( stunPassword->Des() );
            }
        else
            {
            aItem.SetSTUNPasswordL( KNullDesC8() );
            }
        CleanupStack::PopAndDestroy( stunPassword );
        }

    // NAT refresh interval for UDP
    tmpValue = KErrNotFound;
    rep->Get( KUNSAFProtocolsDomainIntervalUDPMask|key, tmpValue );
    aItem.SetNATRefreshUDP( tmpValue );
    
    // NAT refresh interval for TCP
    tmpValue = KErrNotFound;
    rep->Get( KUNSAFProtocolsDomainIntervalTCPMask|key, tmpValue );
    aItem.SetNATRefreshTCP( tmpValue );
    
    // CRLF-based NAT binding refresh 
    tmpValue = KErrNotFound;
    rep->Get( KUNSAFProtocolsDomainEnableCRLFRefresh|key, tmpValue );
    aItem.SetEnableCRLFRefresh( tmpValue );

    TUint32 tempKey = aDomainKeyId|KUNSAFProtocolsFieldTypeMask;
    tempKey ^= KUNSAFProtocolsFieldTypeMask;
    
    RArray<TUint32> stunKeys;
    CleanupClosePushL( stunKeys ); // CS:2
    rep->FindL( tempKey|KUNSAFProtocolsSTUNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, stunKeys );
    TInt stunKeyCount = stunKeys.Count();

    // STUN server items start from CenRep table index 1.
    for ( TInt counter = 1; counter < stunKeyCount; counter++ )
        {
        CWPStunServerItem* stunItem = CWPStunServerItem::NewLC(); // CS:3

        // Remove STUN server address mask from key.
        TUint32 tmpKey = KUNSAFProtocolsSTUNAddressMask^stunKeys[counter];

        // STUNSrvAddr
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNAddressMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* stunSrvAddr = HBufC8::NewLC( actualLength ); // CS:4
            TPtr8 ptr( stunSrvAddr->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsSTUNAddressMask, 
                ptr );
            if ( KErrNone == err )
                {
                stunItem->SetStunSrvAddrL( stunSrvAddr->Des() );
                }
            else
                {
                stunItem->SetStunSrvAddrL( KNullDesC8 );
                }
            CleanupStack::PopAndDestroy( stunSrvAddr ); // CS:3
            }
        else if ( 0 == actualLength )
            {
            stunItem->SetStunSrvAddrL( KNullDesC8 );
            }

        // STUNSrvPort
        TInt tmpValue( KErrNotFound );
        rep->Get( tmpKey|KUNSAFProtocolsSTUNPortMask, tmpValue );
        stunItem->SetStunSrvPort( tmpValue );

        // STUNSrvUsername
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNUsernameMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* stunUsername = HBufC8::NewLC( actualLength ); // CS:4
            TPtr8 ptr( stunUsername->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsSTUNUsernameMask, 
                ptr );
            if ( KErrNone == err )
                {
                stunItem->SetStunUsernameL( stunUsername->Des() );
                }
            else
                {
                stunItem->SetStunUsernameL( KNullDesC8 );
                }
            CleanupStack::PopAndDestroy( stunUsername ); // CS:3
            }
        else if ( 0 == actualLength )
            {
            stunItem->SetStunUsernameL( KNullDesC8 );
            }

        // STUNSrvPassword
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsSTUNPasswordMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* stunPassword = HBufC8::NewLC( actualLength ); // CS:4
            TPtr8 ptr( stunPassword->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsSTUNPasswordMask, 
                ptr );
            if ( KErrNone == err )
                {
                stunItem->SetStunPasswordL( stunPassword->Des() );
                }
            else
                {
                stunItem->SetStunPasswordL( KNullDesC8 );
                }
            CleanupStack::PopAndDestroy( stunPassword ); // CS:3
            }
        else if ( 0 == actualLength )
            {
            stunItem->SetStunPasswordL( KNullDesC8 );
            }

        aItem.AddStunServerItemL( stunItem ); // Ownership transfer.

        CleanupStack::Pop( stunItem ); // CS:2
        }
    stunKeys.Close();
    CleanupStack::PopAndDestroy( &stunKeys ); // CS:1

    // STUNSharedSecret
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsDomainSharedSecretNotSupported, 
        tmpValue );
    aItem.SetDisableStunSharedSecret( tmpValue );

    // PortPoolStartPort
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsPortPoolStartPortMask, tmpValue );
    aItem.SetStartPortRange( tmpValue );

    // PortPoolEndPort
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsPortPoolEndPortMask, tmpValue );
    aItem.SetEndPortRange( tmpValue );

    // UsedNATProtocol
    actualLength = 0;
    rep->Get( key|KUNSAFProtocolsUsedNATProtocolMask, tmp, actualLength );
    if ( actualLength > 0 )
        {
        HBufC8* natProtocol = HBufC8::NewLC( actualLength ); // CS:4
        TPtr8 ptr( natProtocol->Des() );
        TInt err = rep->Get( key|KUNSAFProtocolsUsedNATProtocolMask, 
            ptr );
        if ( KErrNone == err )
            {
            aItem.SetNatProtocolL( natProtocol->Des() );
            }
        else
            {
            aItem.SetNatProtocolL( KNullDesC8 );
            }
        CleanupStack::PopAndDestroy( natProtocol ); // CS:3
        }
    else if ( 0 == actualLength )
        {
        aItem.SetNatProtocolL( KNullDesC8 );
        }

    // ICESpecific/NATUtilities
    actualLength = 0;
    rep->Get( key|KUNSAFProtocolsNATUtilities, tmp, actualLength );
    if ( actualLength > 0 )
        {
        HBufC8* natUtils = HBufC8::NewLC( actualLength ); // CS:4
        TPtr8 ptr( natUtils->Des() );
        TInt err = rep->Get( key|KUNSAFProtocolsNATUtilities, ptr );
        if ( KErrNone == err )
            {
            aItem.SetNatUtilitiesL( natUtils->Des() );
            }
        else
            {
            aItem.SetNatUtilitiesL( KNullDesC8 );
            }
        CleanupStack::PopAndDestroy( natUtils ); // CS:3
        }
    else if ( 0 == actualLength )
        {
        aItem.SetNatUtilitiesL( KNullDesC8 );
        }

    // ICESpecific/HostPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsHostPref, tmpValue );
    aItem.SetHostPref( tmpValue );

    // ICESpecific/ServerRefPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsServerRefPref, tmpValue );
    aItem.SetServerRefPref( tmpValue );

    // ICESpecific/RelayPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsRelayPref, tmpValue );
    aItem.SetRelayPref( tmpValue );

    // ICESpecific/PeerRefPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsPeerRefPref, tmpValue );
    aItem.SetPeerRefPref( tmpValue );

    // ICESpecific/IPv4Pref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsIPV4Pref, tmpValue );
    aItem.SetIPv4Pref( tmpValue );

    // ICESpecific/IPv6Pref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsIPV6Pref, tmpValue );
    aItem.SetIPv6Pref( tmpValue );

    // ICESpecific/VpnPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsVPNPref, tmpValue );
    aItem.SetVpnPref( tmpValue );

    // ICESpecific/UdpPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsUDPPref, tmpValue );
    aItem.SetUdpPref( tmpValue );

    // ICESpecific/TcpPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPPref, tmpValue );
    aItem.SetTcpPref( tmpValue );

    // ICESpecific/TcpActivePref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPActivePref, tmpValue );
    aItem.SetTcpActivePref( tmpValue );

    // ICESpecific/TcpPassivePref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPPassivePref, tmpValue );
    aItem.SetTcpPassivePref( tmpValue );

    // ICESpecific/TCPSimultPref
    tmpValue = KErrNotFound;
    rep->Get( key|KUNSAFProtocolsTCPSimultPref, tmpValue );
    aItem.SetTcpSimultPref( tmpValue );

    // TURN server
    tempKey = aDomainKeyId|KUNSAFProtocolsFieldTypeMask;
    tempKey ^= KUNSAFProtocolsFieldTypeMask;
    
    RArray<TUint32> turnKeys;
    CleanupClosePushL( turnKeys ); // CS:2
    rep->FindL( tempKey|KUNSAFProtocolsTURNAddressMask, 
        KUNSAFProtocolsSubTableFieldTypeMask, turnKeys );
    TInt turnKeyCount = turnKeys.Count();
    for ( TInt counter = 0; counter < turnKeyCount; counter++ )
        {
        CWPTurnServerItem* turnItem = CWPTurnServerItem::NewLC(); // CS:3

        // Remove TURN server address mask from key.
        TUint32 tmpKey = KUNSAFProtocolsTURNAddressMask^turnKeys[counter];

        // TURNSrvAddr
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsTURNAddressMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* turnSrvAddr = HBufC8::NewLC( actualLength ); // CS:4
            TPtr8 ptr( turnSrvAddr->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsTURNAddressMask, 
                ptr );
            if ( KErrNone == err )
                {
                turnItem->SetTurnSrvAddrL( turnSrvAddr->Des() );
                }
            else
                {
                turnItem->SetTurnSrvAddrL( KNullDesC8 );
                }
            CleanupStack::PopAndDestroy( turnSrvAddr ); // CS:3
            }
        else if ( 0 == actualLength )
            {
            turnItem->SetTurnSrvAddrL( KNullDesC8 );
            }

        // TURNSrvPort
        TInt tmpValue( KErrNotFound );
        rep->Get( tmpKey|KUNSAFProtocolsTURNPortMask, tmpValue );
        turnItem->SetTurnSrvPort( tmpValue );

        // TURNUsername
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsTURNUsernameMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* turnUsername = HBufC8::NewLC( actualLength ); // CS:4
            TPtr8 ptr( turnUsername->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsTURNUsernameMask, 
                ptr );
            if ( KErrNone == err )
                {
                turnItem->SetTurnUsernameL( turnUsername->Des() );
                }
            else
                {
                turnItem->SetTurnUsernameL( KNullDesC8 );
                }
            CleanupStack::PopAndDestroy( turnUsername ); // CS:3
            }
        else
            {
            turnItem->SetTurnUsernameL( KNullDesC8 );
            }

        // TURNPassword
        actualLength = 0;
        rep->Get( tmpKey|KUNSAFProtocolsTURNPasswordMask, tmp, actualLength );
        if ( actualLength > 0 )
            {
            HBufC8* turnPassword = HBufC8::NewLC( actualLength ); // CS:4
            TPtr8 ptr( turnPassword->Des() );
            TInt err = rep->Get( tmpKey|KUNSAFProtocolsTURNPasswordMask, 
                ptr );
            if ( KErrNone == err )
                {
                turnItem->SetTurnPasswordL( turnPassword->Des() );
                }
            else
                {
                turnItem->SetTurnPasswordL( KNullDesC8 );
                }
            CleanupStack::PopAndDestroy( turnPassword ); // CS:3
            }
        else
            {
            turnItem->SetTurnPasswordL( KNullDesC8 );
            }
        aItem.AddTurnServerItemL( turnItem ); // Ownership transfer.
        CleanupStack::Pop( turnItem ); // CS:2
        }

    turnKeys.Close();
    CleanupStack::PopAndDestroy( &turnKeys ); // CS:1
    CleanupStack::PopAndDestroy( rep ); // CS:0
    }

// ---------------------------------------------------------------------------
// T_NATFWTraversalAdapter::GetIAPSettingsL
// Gets one IAP settings entry
// ---------------------------------------------------------------------------
//
void T_NATFWTraversalAdapter::GetIAPSettingsL( TUint32 aIAPKey, 
    CNATFWIAPSpecificItem& aItem )
    {
    // Central Repository for NAT-FW Traversal settings
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols );
    
    TUint32 key( KErrNone );
    // select domain 
    key = ( KUNSAFProtocolsIAPIdMask^( aIAPKey ) );
    key |= KUNSAFProtocolsIAPTableMask;
    // set key id
    aItem.SetIAPKey( aIAPKey );
    TInt tmp( 0 );
    rep->Get( KUNSAFProtocolsIAPIdMask|key, tmp);
    aItem.SetIAPId( tmp ) ;
    tmp = KErrNotFound;
    rep->Get( KUNSAFProtocolsIntervalUDPMask|key, tmp );
    aItem.SetIntervalUDP( tmp ) ;
    tmp = KErrNotFound;
    rep->Get( KUNSAFProtocolsIntervalTCPMask|key, tmp );
    aItem.SetIntervalTCP( tmp ) ;
    tmp = KErrNotFound;
    rep->Get( KUNSAFProtocolsSTUNRetransmitTimerMask|key, tmp );
    aItem.SetSTUNRetransmitTimer( tmp );
    CleanupStack::PopAndDestroy( rep );
    }

// ---------------------------------------------------------------------------
// T_NATFWTraversalAdapter::StoreAgainL
// Saves settings again to test update methods.
// ---------------------------------------------------------------------------
//
void T_NATFWTraversalAdapter::StoreAgainL(  )
    {
    _LIT( KFileName, "c:\\data\\NATFW_AP_BOOTSTRAP_3.wbxml" );
    EUNIT_ASSERT_NO_LEAVE( SaveDocumentL( KFileName() ) );    
    }

// ---------------------------------------------------------------------------
// T_NATFWTraversalAdapter::CheckUpdatedSettingsL
// Saves settings again to test update methods.
// ---------------------------------------------------------------------------
//
void T_NATFWTraversalAdapter::CheckUpdatedSettingsL(  )
    {

    // ==============================
    // Domain specific settings.
    // ==============================
    //
    // Domain
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetDomain() == KDomain );

    // STUN server address
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNAddr() == 
        KUpdateStunSrvAddr );

    // STUN server port
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNPort() == 
        KUpdateStunSrvPort );

    // TCP refresh interval
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetNATRefreshTCP() == 
        KUpdateNATRefreshTcp );

    // UDP refresh interval
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetNATRefreshUDP() == 
        KUpdateNATRefreshUdp );

    // CRLF refresh
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetEnableCRLFRefresh() == 
        KUpdateCrlfRefresh );

    // STUN server username
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNUsername() == 
        KUpdateNwStunUsername );

    // STUN server password
    EUNIT_ASSERT( iNATFWDomainSpecificItem->GetSTUNPassword() == 
        KUpdateNwStunPassword );

    // Additional STUN server address
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunSrvAddr() == 
        KUpdateAddStunSrvAddr );

    // Additional STUN server port
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunSrvPort() == 
        KUpdateStunSrvPort2 );

    // Additional STUN server username
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunUsername() ==
        KUpdateAddStunUsername );

    // Additional STUN server password
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iStunServers[0]->StunPassword() ==
        KUpdateAddStunPassword );

    // Disable STUN shared secret
    EUNIT_ASSERT( iNATFWDomainSpecificItem->DisableStunSharedSecret() == 
        TInt( ETrue ) );

    // Start port
    EUNIT_ASSERT( iNATFWDomainSpecificItem->StartPortRange() == 
        KUpdateStartPort );

    // End port
    EUNIT_ASSERT( iNATFWDomainSpecificItem->EndPortRange() == 
        KUpdateEndPort );

    // Used NAT protocol
    EUNIT_ASSERT( iNATFWDomainSpecificItem->UsedNatProtocol() == 
        KUpdatedNATProtocol );

    // ==============================
    // ICE specific settings.
    // ==============================
    //
    // NAT utilities
    EUNIT_ASSERT( iNATFWDomainSpecificItem->NatUtilities() == 
        KUpdateUsedNatUtilities );

    // Host preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->HostPref() == KUpdateHostPref );

    // Server-reflexive preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->ServerRefPref() == 
        KUpdateServerRefPref );

    // Relay-preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->RelayPref() == KUpdateRelayPref );

    // Peer-reflexive preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->PeerRefPref() == 
        KUpdatePeerRefPref );

    // IPv4 preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->IPv4Pref() == KUpdateIPv4Pref );

    // IPv6 preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->IPv6Pref() == KUpdateIPv6Pref );

    // VPN preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->VpnPref() == KUpdateVPNPref );

    // UDP preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->UdpPref() == KUpdateUDPPref );

    // TCP preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpPref() == KUpdateTCPPref );

    // TCP-active preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpActivePref() == 
        KUpdateTCPActivePref );

    // TCP-passive preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpPassivePref() == 
        KUpdateTCPPassivePref );

    // TCP-simultaneous-open preference
    EUNIT_ASSERT( iNATFWDomainSpecificItem->TcpSimultPref() == 
        KUpdateTCPSimultPref );

    // ==============================
    // TURN specific settings.
    // ==============================
    //
    // TURN server address
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnSrvAddr() 
        == KUpdateTurnSrvAddr );

    // TURN server port
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnSrvPort() 
        == KUpdateTurnSrvPort );

    // TURN server username
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnUsername() 
        == KUpdateTurnUsername );

    // TURN server password
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[0]->TurnPassword() 
        == KUpdateTurnPassword );

    // TURN server #2 address
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnSrvAddr() 
        == KUpdateTurnSrvAddr2 );

    // TURN server #2 port
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnSrvPort() 
        == KUpdateTurnSrvPort2 );

    // TURN server #2 password
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnPassword() 
        == KUpdateTurnPassword2 );

    // TURN server #2 username
    EUNIT_ASSERT( iNATFWDomainSpecificItem->iTurnServers[1]->TurnUsername() 
        == KUpdateTurnUsername2 );

    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    T_NATFWTraversalAdapter,
    "NAT/FW Traversal adapter unit&module tests.",
    "MODULE" )

EUNIT_TEST(
    "Store and load stored items",
    "CNATFWTraversalAdapter",
    "StoreL",
    "FUNCTIONALITY",
    SetupL, StoreL, Teardown)

// Domain specific
EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetDomain",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetDomain, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetSTUNAddr",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetSTUNAddr, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetSTUNPort",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetSTUNPort, Teardown)
    
EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetNATRefreshTCP",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetNATRefreshTCP, Teardown)
    
EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetNATRefreshUDP",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetNATRefreshUDP, Teardown)
    
EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetEnableCRLFRefresh",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetEnableCRLFRefresh, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetSTUNUsername",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetSTUNUsername, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "GetSTUNPassword",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_GetSTUNPassword, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "StunSrvAddrL",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_StunSrvAddrL, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "StunSrvPort",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_StunSrvPort, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "StunSrvUsernameL",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_StunSrvUsernameL, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "StunSrvPasswordL",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_StunSrvPasswordL, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "DisableStunSharedSecret",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_DisableStunSharedSecret, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "StartPortRange",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_StartPortRange, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "EndPortRange",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_EndPortRange, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "UsedNatProtocol",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_UsedNatProtocol, Teardown)

// ICE specific
EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "NatUtilities",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_NatUtilities, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "HostPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_HostPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "ServerRefPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_ServerRefPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "RelayPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_RelayPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "PeerRefPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_PeerRefPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "IPv4Pref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_IPv4Pref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "IPv6Pref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_IPv6Pref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "VpnPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_VpnPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "UdpPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_UdpPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TcpPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TcpPref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TcpActivePref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TcpActivePref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TcpPassivePref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TcpPassivePref, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TCPSimultPref",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TcpSimultPref, Teardown)

// TURN specific
EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TurnSrvPort",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TurnSrvPort, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TurnSrvPort",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TurnSrvPort, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TurnUsername",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TurnUsernameL, Teardown)

EUNIT_TEST(
    "Check stored domain specific items",
    "CNATFWDomainSpecificItem",
    "TurnPassword",
    "FUNCTIONALITY",
    SetupL, NATFWDomainSpecificItem_TurnPasswordL, Teardown)

// IAP specific
EUNIT_TEST(
    "Check stored iap specific items",
    "CNATFWIAPSpecificItem",
    "GetIntervalUDP",
    "FUNCTIONALITY",
    SetupL, NATFWIAPSpecificItem_GetIntervalUDP, Teardown)

EUNIT_TEST(
    "Check stored iap specific items",
    "CNATFWIAPSpecificItem",
    "GetIntervalTCP",
    "FUNCTIONALITY",
    SetupL, NATFWIAPSpecificItem_GetIntervalTCP, Teardown)
    
EUNIT_TEST(
    "Check stored iap specific items",
    "CNATFWIAPSpecificItem",
    "GetSTUNRetransmitTimer",
    "FUNCTIONALITY",
    SetupL, NATFWIAPSpecificItem_GetSTUNRetransmitTimer, Teardown)

EUNIT_TEST(
    "Store new settings using same domain",
    "CNATFWTraversalAdapter",
    "StoreL",
    "FUNCTIONALITY",
    SetupL, StoreAgainL, Teardown)

EUNIT_TEST(
    "Check updated settings",
    "CNATFWDomainSpecificItem",
    "All getter methods",
    "FUNCTIONALITY",
    SetupL, CheckUpdatedSettingsL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
