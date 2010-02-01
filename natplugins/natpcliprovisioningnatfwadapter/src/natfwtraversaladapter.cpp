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
* Description:  Receives and stores NAT/FW Traversal settings internally.
*
*/
 

// INCLUDE FILES
#include <e32base.h>
#include <CWPAdapter.h>
#include <CWPCharacteristic.h>
#include <CWPParameter.h>
#include <wpnatfwtraversaladapter.rsg>
#include <WPAdapterUtil.h> // Adapter utils
#include <utf.h> // Unicode conversion
#include <cmmanagerext.h>
#include <cmdestinationext.h>
#include <cmconnectionmethoddef.h>
#include "natfwdomainspecificitem.h"
#include "natfwiapspecificitem.h"
#include "natfwtraversaladapter.h"
#include "wpnatfwitem.h"
#include "wpnatfwdebug.h"
#include "natfwdefaults.h"
#include "stunserveritem.h"
#include "turnserveritem.h"

// Central Repository keys for NAT-FW Traversal settings
#include <unsafprotocolscrkeys.h>
#include "centralrepository.h"

// LOCAL CONSTANTS
// Application related constants
_LIT( KNATFWTraversalAdapterName, "WPNATFWTraversalAdapter");
_LIT( KNATFWAppID,                "w902E" ); // OMA CP AC document for NATFW.
_LIT8( KNATFWAppID8,              "w902E" );
// Network (domain) specific settings.
_LIT( KNetwork,                   "NW" ); 
_LIT( KAdditionalStun,            "ADDITIONALSTUN" );
_LIT( KStunsrvaddr,               "STUNSRVADDR" );
_LIT( KStunsrvport,               "STUNSRVPORT" );
_LIT( KNatRefreshTcp,             "NATREFRESHTCP" );
_LIT( KNatRefreshUdp,             "NATREFRESHUDP" );
_LIT( KCrlfRefresh,               "CRLFREFRESH" );
// Access point specific settings.
_LIT( KNatIap,                    "NATIAP" );
_LIT( KToNapId,                   "TONAPID" );
_LIT( KRefreshTcp,                "REFRESHTCP" );
_LIT( KRefreshUdp,                "REFRESHUDP" );
_LIT( KStunRetransmit,            "STUNRETRANSMIT" );

// Network (domain) specific settings.
_LIT( KStunSharedSecret,          "SHAREDSECRET" );
_LIT( KPortPoolStart,             "SPRANGE" );
_LIT( KPortPoolEnd,               "EPRANGE" );
// ICE specific settings.
_LIT( KIce,                       "ICE" );
_LIT( KNatUtilities,              "NATUTILITIES" );
_LIT( KHostPref,                  "HOSTPREF" );
_LIT( KServerReflexivePref,       "SERVERREFPREF" );
_LIT( KRelayPref,                 "RELAYPREF" );
_LIT( KPeerReflexivePref,         "PEERREFPREF" );
_LIT( KIPv4Pref,                  "IPV4PREF" );
_LIT( KIPv6Pref,                  "IPV6PREF" );
_LIT( KVPNPref,                   "VPNPREF" );
_LIT( KUDPPref,                   "UDPPREF" );
_LIT( KTCPPref,                   "TCPPREF" );
_LIT( KTCPActivePref,             "TCPACTIVEPREF" );
_LIT( KTCPPassivePref,            "TCPPASSIVEPREF" );
_LIT( KTCPSimultaneousOpenPref,   "TCPSIMULTPREF" );
// TURN specific settings.
_LIT( KTurnServer,                "TURNSERVER" );
_LIT( KTurnSrvAddr,               "TURNSRVADDR" );
_LIT( KTurnSrvPort,               "TURNSRVPORT" );

// ================= MEMBER FUNCTIONS ======================

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::CNATFWTraversalAdapter
//
// ---------------------------------------------------------------------------
//
CNATFWTraversalAdapter::CNATFWTraversalAdapter() : CWPAdapter()
    {
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::ConstructL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::ConstructL()
    {
    DBG_PRINT( "CNATFWTraversalAdapter::ConstructL - begin" );
    // read default title and name from resources.
    TFileName fileName;
    Dll::FileName( fileName );
    iDefaultName = WPAdapterUtil::ReadHBufCL( fileName, 
        KNATFWTraversalAdapterName, 
        R_QTN_SM_DEFAULT_NAME_NATFW );
    iTitle = WPAdapterUtil::ReadHBufCL( fileName, 
        KNATFWTraversalAdapterName, 
        R_QTN_SM_HEAD_NATFW );
    
    iCharacteristicNw             = EFalse;
    iCharacteristicIce            = EFalse;
    iCharacteristicTurn           = EFalse;
    iCharacteristicNatIap         = EFalse;
    iCharacteristicTurnServer     = EFalse;
    iCharacteristicAdditionalStun = EFalse;

    DBG_PRINT( "CNATFWTraversalAdapter::ConstructL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//
CNATFWTraversalAdapter* CNATFWTraversalAdapter::NewL()
    {
    CNATFWTraversalAdapter* self = new ( ELeave ) CNATFWTraversalAdapter;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::~CNATFWTraversalAdapter
//
// ---------------------------------------------------------------------------
//
CNATFWTraversalAdapter::~CNATFWTraversalAdapter()
    {
    DBG_PRINT( "CNATFWTraversalAdapter::~CNATFWTraversalAdapter - begin" );
    delete iDefaultName;
    delete iTitle;
    iDatas.ResetAndDestroy();
    iDatas.Close();
    DBG_PRINT( "CNATFWTraversalAdapter::~CNATFWTraversalAdapter - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::ContextExtension
//
// ---------------------------------------------------------------------------
//
TInt CNATFWTraversalAdapter::ContextExtension( 
    MWPContextExtension*& aExtension )
    {
    aExtension = this;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::VisitLinkL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::VisitLinkL( CWPCharacteristic& aLink )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::VisitLinkL - begin" );
    if ( ( KWPNapDef == aLink.Type() ) && iCurrentIapItem 
        && !iCurrentIapItem->NapDef() )
        {
        iCurrentIapItem->SetNapDef( &aLink );
        }
    DBG_PRINT( "CNATFWTraversalAdapter::VisitLinkL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SaveDataL
//
// ---------------------------------------------------------------------------
//
const TDesC8& CNATFWTraversalAdapter::SaveDataL( TInt aIndex ) const
    {
    return iDatas[aIndex]->SaveData();
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::DeleteL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::DeleteL( const TDesC8& aSaveData )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::DeleteL - begin" );
    // Central Repository for NAT-FW Traversal settings.
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols );
    TInt step( 0 );

    // Delete all keys that are stored.
    while ( step < aSaveData.Length() )
        {
        TUint32 key = DesToTUint( aSaveData.Mid( step, KMaxCharsInTUint32 ) );
        key &= KUNSAFProtocolsTableMask;
        TUint32 errorKey;
        rep->Delete( key, KUNSAFProtocolsTableMask, errorKey );
        step += KMaxCharsInTUint32;
        }
    
    CleanupStack::PopAndDestroy( rep );
    DBG_PRINT( "CNATFWTraversalAdapter::DeleteL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::Uid
//
// ---------------------------------------------------------------------------
//
TUint32 CNATFWTraversalAdapter::Uid() const
    {
    return iDtor_ID_Key.iUid; 
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::DetailsL
//
// ---------------------------------------------------------------------------
//
TInt CNATFWTraversalAdapter::DetailsL( 
    TInt /*aItem*/, MWPPairVisitor& /*aVisitor */ )
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::ItemCount
//
// ---------------------------------------------------------------------------
//
TInt CNATFWTraversalAdapter::ItemCount() const
    {
    return iDatas.Count();
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SummaryTitle
//
// ---------------------------------------------------------------------------
//
const TDesC16& CNATFWTraversalAdapter::SummaryTitle( TInt /*aIndex*/ ) const
    {
    return *iTitle;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SummaryText
//
// ---------------------------------------------------------------------------
//
const TDesC16& CNATFWTraversalAdapter::SummaryText( TInt aIndex ) const
    {
    // If there is no value for NAME parameter the default value is set here.
    if ( 0 == iDatas[aIndex]->Name().Length() )
        {
        TRAP_IGNORE( iDatas[aIndex]->SetNameL( iDefaultName->Des() ) );
        }
    return iDatas[aIndex]->Name();
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SaveL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::SaveL( TInt aIndex )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::SaveL - begin" );
    CWPNATFWItem* cNATFWItem = iDatas[aIndex];
    const TInt iapItemCount( iDatas[aIndex]->IAPItems().Count() );
    for ( TInt counter = 0; counter < iapItemCount; counter++ )
        {
        // Set access point ID to current IAP item (access point is always
        // stored before NAT/FW settings).
        CNATFWIAPSpecificItem* cIapItem = iDatas[aIndex]->IAPItems()[counter];
        if ( cIapItem->NapDef() )
            {
            // Get WAP ID.
            TPckgBuf<TUint32> uid;
            uid.Copy( cIapItem->NapDef()->Data( 0 ) );
            
            TUint32 wapId( uid() );
            // Get corresponding IAP ID.
            TUint32 iapId = IapIdFromWapIdL( wapId );
            cIapItem->SetIAPId( iapId );
            }// if 
        cIapItem = NULL;    
        }
    cNATFWItem->StoreL();
    cNATFWItem = NULL;
    DBG_PRINT( "CNATFWTraversalAdapter::SaveL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::CanSetAsDefault
//
// ---------------------------------------------------------------------------
//
TBool CNATFWTraversalAdapter::CanSetAsDefault( TInt /*aIndex*/ ) const
    {
    // NATFW profile cannot be set as default via OMA CP.
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SetAsDefaultL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::SetAsDefaultL( TInt /*aIndex*/ )
    {
    // NATFW profile cannot be set as default via OMA CP.
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::VisitL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::VisitL( CWPCharacteristic& aCharacteristic )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::VisitL( characteristic ) - begin" );

    switch( aCharacteristic.Type() )
        {
        // Core NATFW settings.
        case KWPApplication:
            {

            iCurrentNATFWItem = CWPNATFWItem::NewL();
            aCharacteristic.AcceptL( *this );

            // If NATFW settings, append the item into array.
            if ( KNATFWAppID() == iAppID ) 
                {
                User::LeaveIfError( iDatas.Append( iCurrentNATFWItem ) );
                }
            else // Else the settings are no longer of type NATFW.
                {
                delete iCurrentNATFWItem;
                iCurrentNATFWItem             = NULL;
                iCharacteristicNw             = EFalse;
                iCharacteristicIce            = EFalse;
                iCharacteristicTurn           = EFalse;
                iCharacteristicNatIap         = EFalse;
                iCharacteristicTurnServer     = EFalse;
                iCharacteristicAdditionalStun = EFalse;
                }
            break;
            }

        case KWPNamedCharacteristic:
            {
            // Domain specific settings.
            if ( aCharacteristic.Name().Compare( KNetwork() ) == 0 )
                {
                iCurrentDomainItem = CNATFWDomainSpecificItem::NewL();
                iCharacteristicNw = ETrue;
                iCurrentDomainItem->SetDefaultValuesL();
                aCharacteristic.AcceptL( *this ); 
                // Ownership transfer:
                iCurrentNATFWItem->AddDomainSpecificL( iCurrentDomainItem );
                }
            // IAP specific settings.
            else if ( aCharacteristic.Name().Compare( KNatIap() ) == 0 )
                {
                iCurrentIapItem = CNATFWIAPSpecificItem::NewL();
                iCharacteristicNatIap = ETrue;

                // Set defaults for IAP specific item.
                iCurrentIapItem->SetSTUNRetransmitTimer(
                    KDefaultSTUNRetransmitTimer );
                // Default values for UDP and TCP refresh intervals are set in
                // NATFWItem if there are no values for either Domain or IAP
                // specific items.
                iCurrentIapItem->SetIntervalUDP( KNotSet );
                iCurrentIapItem->SetIntervalTCP( KNotSet );
                aCharacteristic.AcceptL( *this );
                // Ownership transfer:
                iCurrentNATFWItem->AddIAPSpecificL( iCurrentIapItem );
                }

            // ICE specific settings.
            else if ( 0 == aCharacteristic.Name().Compare( KIce() ) )
                {
                if ( !iCurrentDomainItem )
                    {
                    break;
                    }
                else
                    {
                    iCharacteristicIce = ETrue;
                    aCharacteristic.AcceptL( *this );
                    }
                }
            // TURNSERVER specific settings.
            else if ( 0 == aCharacteristic.Name().Compare( KTurnServer() ) )
                {
                if ( !iCurrentDomainItem )
                    {
                    break;
                    }
                else
                    {
                    iCurrentTurnServer = CWPTurnServerItem::NewL();
                    iCharacteristicTurnServer = ETrue;
                    aCharacteristic.AcceptL( *this );
                    // Ownership transfer:
                    iCurrentDomainItem->AddTurnServerItemL( 
                        iCurrentTurnServer );
                    }
                }
            // ADDITIONALSTUN specific settings.
            else if ( 0 == aCharacteristic.Name().Compare( 
                KAdditionalStun() ) )
                {
                if ( !iCurrentDomainItem )
                    {
                    break;
                    }
                else
                    {
                    iCurrentStunServer = CWPStunServerItem::NewL();
                    iCharacteristicAdditionalStun = ETrue;
                    aCharacteristic.AcceptL( *this );
                    // Ownership transfer:
                    iCurrentDomainItem->AddStunServerItemL( 
                        iCurrentStunServer );
                    }
                }
            break; 
            }
        default:
            break;
        }
    DBG_PRINT( "CNATFWTraversalAdapter::VisitL( characteristic ) - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::VisitL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::VisitL( CWPParameter& aParameter )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::VisitL( parameter ) - begin" );

    // tmpValue holds the value converted from Unicode to UTF8.
    HBufC8* tmpValue = HBufC8::NewLC( aParameter.Value().Length() ); // CS:1
    TPtr8 ptrTmpValue( tmpValue->Des() );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( ptrTmpValue, 
        aParameter.Value() );

    switch( aParameter.ID() )
        {
        case EWPParameterAppID:
            {
	        iAppID.Set( aParameter.Value() );
            break;
            }
        case EWPParameterDomain:
            {
            if ( iCurrentDomainItem )
                {
                iCurrentDomainItem->SetDomainL( tmpValue->Des() );
                }
            break;
            }    
        // Here at case 0 are handled the NAT/FW parameters that are
        // extensions to OMA Client Provisioning parameter set.
        case 0:
            
            // NW/STUNSRVADDR
            if ( 0 == aParameter.Name().Compare( KStunsrvaddr ) )
                {
                if ( iCharacteristicNw )
                    {
                    // Domain STUN server settings.
                    if ( !iCharacteristicAdditionalStun )
                        {
                        iCurrentDomainItem->SetSTUNServerAddrL( 
                            tmpValue->Des() );
                        }
                    // Additional STUN server settings.
                    else
                        {
                        iCurrentStunServer->SetStunSrvAddrL(
                            tmpValue->Des() );
                        }
                    }
                }

            // NW/STUNSRVPORT
            else if ( 0 == aParameter.Name().Compare( KStunsrvport ) )
                {
                if ( iCharacteristicNw )
                    {
                    // Domain STUN server settings.
                    if ( !iCharacteristicAdditionalStun )
                        {
                        iCurrentDomainItem->SetSTUNServerPort( 
                            DesToInt( tmpValue->Des() ) );
                        }
                    // Additional STUN server settings.
                    else
                        {
                        iCurrentStunServer->SetStunSrvPort( DesToInt( 
                            tmpValue->Des() ) );
                        }
                    }
                }

            // NW/NATREFRESHTCP
            else if ( 0 == aParameter.Name().Compare( KNatRefreshTcp ) )
                {
                if ( iCharacteristicNw ) 
                    {
                    iCurrentDomainItem->SetNATRefreshTCP( DesToInt( 
                        tmpValue->Des() ) );
                    }
                }

            // NW/NATREFRESHUDP
            else if ( 0 == aParameter.Name().Compare( KNatRefreshUdp ) )
                {
                if ( iCharacteristicNw )
                    {
                    iCurrentDomainItem->SetNATRefreshUDP( DesToInt( 
                        tmpValue->Des() ) );
                    }
                }

            // NW/CRLFREFRESH
            else if ( 0 == aParameter.Name().Compare( KCrlfRefresh ) )
                {
                if ( iCharacteristicNw )
                    {
                    iCurrentDomainItem->SetEnableCRLFRefresh( ETrue );
                    }
                }
            // VoIP rel 2.2 parameters

            // NW/SHAREDSECRET
            else if ( 0 == aParameter.Name().Compare( KStunSharedSecret )
                && iCharacteristicNw )
                {
                // The CentRep key is for not using STUN shared secret so we
                // must check the provisioned value for using STUN shared
                // secret and reverse it.
                if ( TInt( ETrue ) == DesToInt( tmpValue->Des() ) )
                    {
                    iCurrentDomainItem->SetDisableStunSharedSecret( EFalse );
                    }
                else if ( TInt( EFalse ) == DesToInt( tmpValue->Des() ) )
                    {
                    iCurrentDomainItem->SetDisableStunSharedSecret( ETrue );
                    }
                }
            // NW/SPRANGE
            else if ( 0 == aParameter.Name().Compare( KPortPoolStart ) 
                && iCharacteristicNw )
                {
                iCurrentDomainItem->SetStartPortRange( DesToInt( 
                    tmpValue->Des() ) );
                }
            // NW/EPRANGE
            else if ( 0 == aParameter.Name().Compare( KPortPoolEnd ) 
                && iCharacteristicNw )
                {
                    iCurrentDomainItem->SetEndPortRange( DesToInt( 
                        tmpValue->Des() ) );
                }
            // NATIAP/TONAPID
            else if ( 0 == aParameter.Name().Compare( KToNapId ) )
                {
                if ( iCharacteristicNatIap )
                    {
                    iCurrentIapItem->SetNapDefNameL( tmpValue->Des() );
                    }
                }
            // NATIAP/REFRESHTCP
            else if ( 0 == aParameter.Name().Compare( KRefreshTcp ) )
                {
                if ( iCharacteristicNatIap )
                    {
                    iCurrentIapItem->SetIntervalTCP( DesToInt( 
                        tmpValue->Des() ) );
                    }
                }
            // NATIAP/REFRESHUDP
            else if ( 0 == aParameter.Name().Compare( KRefreshUdp ) )
                {
                if ( iCharacteristicNatIap )
                    {
                    iCurrentIapItem->SetIntervalUDP( DesToInt(
                        tmpValue->Des() ) );
                    }
                }
            // NATIAP/STUNRETRANSMIT
            else if ( 0 == aParameter.Name().Compare( KStunRetransmit ) )
                {
                if ( iCharacteristicNatIap )
                    {
                    iCurrentIapItem->SetSTUNRetransmitTimer( 
                        DesToInt( tmpValue->Des() ) );
                    }
                }

            // ICE/NATUTILITIES
            else if ( 0 == aParameter.Name().Compare( 
                KNatUtilities ) && iCharacteristicIce )
                {
                iCurrentDomainItem->SetNatUtilitiesL( tmpValue->Des() );
                }
            // ICE/HOSTPREF
            else if ( 0 == aParameter.Name().Compare( KHostPref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetHostPref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/SERVERREFPREF
            else if ( 0 == aParameter.Name().Compare( KServerReflexivePref ) 
                && iCharacteristicIce )
                {
                iCurrentDomainItem->SetServerRefPref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/RELAYPREF
            else if ( 0 == aParameter.Name().Compare( KRelayPref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetRelayPref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/PEERREFPREF
            else if ( 0 == aParameter.Name().Compare( KPeerReflexivePref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetPeerRefPref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/IPV4PREF
            else if ( 0 == aParameter.Name().Compare( KIPv4Pref ) && 
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetIPv4Pref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/IPV6PREF
            else if ( 0 == aParameter.Name().Compare( KIPv6Pref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetIPv6Pref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/VPNPREF
            else if ( 0 == aParameter.Name().Compare( KVPNPref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetVpnPref( DesToInt( tmpValue->Des() ) );
                }
            // ICE/UDPPREF
            else if ( 0 == aParameter.Name().Compare( KUDPPref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetUdpPref( DesToInt( tmpValue->Des() ) );
                }
            // ICE/TCPPREF
            else if ( 0 == aParameter.Name().Compare( KTCPPref ) &&
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetTcpPref( DesToInt( tmpValue->Des() ) );
                }
            // ICE/TCPACTIVEPREF
            else if ( 0 == aParameter.Name().Compare( KTCPActivePref ) && 
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetTcpActivePref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/TCPPASSIVEPREF
            else if ( 0 == aParameter.Name().Compare( KTCPPassivePref ) && 
                iCharacteristicIce )
                {
                iCurrentDomainItem->SetTcpPassivePref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // ICE/TCPSIMULTPREF
            else if ( 0 == aParameter.Name().Compare( 
                KTCPSimultaneousOpenPref ) && iCharacteristicIce )
                {
                iCurrentDomainItem->SetTcpSimultPref( DesToInt( 
                    tmpValue->Des() ) );
                }
            // TURNSERVER/TURNSRVADDR
            else if ( 0 == aParameter.Name().Compare( KTurnSrvAddr ) &&
                iCharacteristicTurnServer )
                {
                iCurrentTurnServer->SetTurnSrvAddrL( tmpValue->Des() );
                }
            // TURNSERVER/TURNSRVPORT
            else if ( 0 == aParameter.Name().Compare( KTurnSrvPort ) &&
                iCharacteristicTurnServer )
                {
                iCurrentTurnServer->SetTurnSrvPort( DesToInt(
                    tmpValue->Des() ) );
                }
            break; // case 0

        case EWPParameterName: // APPLICATION/NAME
            iCurrentNATFWItem->SetNameL( aParameter.Value() );
            break;

        case EWPParameterToNapID: // NATIAP/TO-NAPID
            {
            if ( iCurrentIapItem )
                {
                iCurrentIapItem->SetNapDefNameL( tmpValue->Des() );
                }
            break;
            }

        case EWPParameterAppRef: // APPLICATION/APPREF
            {
            if ( 0 == iCurrentNATFWItem->AppRef().Length() )
                {
                iCurrentNATFWItem->SetAppRefL( tmpValue->Des() );
                }
            break;
            }

        case EWPParameterAProtocol: // NW/APROTOCOL
            {
            if ( iCharacteristicNw )
                {
                iCurrentDomainItem->SetNatProtocolL( tmpValue->Des() );
                }
            break;
            }

        case EWPParameterAAuthName: // TURNSERVER/AAUTHNAME
            {
            if ( iCharacteristicTurnServer )
                {
                iCurrentTurnServer->SetTurnUsernameL( tmpValue->Des() );
                }
            else if ( iCharacteristicAdditionalStun )
                {
                iCurrentStunServer->SetStunUsernameL( tmpValue->Des() );
                }
            else if ( iCharacteristicNw )
                {
                iCurrentDomainItem->SetSTUNUsernameL( tmpValue->Des() );
                }
            break;
            }

        case EWPParameterAAuthSecret: // TURNSERVER/AAUTHSECRET
            {
            if ( iCharacteristicTurnServer )
                {
                iCurrentTurnServer->SetTurnPasswordL( tmpValue->Des() );
                }
            else if ( iCharacteristicAdditionalStun )
                {
                iCurrentStunServer->SetStunPasswordL( tmpValue->Des() );
                }
            else if ( iCharacteristicNw )
                {
                iCurrentDomainItem->SetSTUNPasswordL( tmpValue->Des() );
                }
            break;
            }

		default:
			break;
        }// switch
    CleanupStack::PopAndDestroy( tmpValue );

    DBG_PRINT( "CNATFWTraversalAdapter::VisitL( parameter ) - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::GetSavingInfoL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::GetSavingInfoL( TInt aIndex,
    RPointerArray<HBufC8> &aSavingInfo )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::GetSavingInfoL - begin" );

    if(iDatas[aIndex]->SavedDomainId().Length() > 0 )
        {
        // APPID into place [0].
        aSavingInfo.AppendL( KNATFWAppID8().AllocL() );

        // APPREF into place [1].
        aSavingInfo.AppendL( iDatas[aIndex]->AppRef().AllocL() );

        // Domain specific settings key ID into place [2].
        // This is the information that is stored in VoIPAdapter.
        aSavingInfo.AppendL( iDatas[aIndex]->SavedDomainId().AllocL() );
        }

    DBG_PRINT( "CNATFWTraversalAdapter::GetSavingInfoL - end" );
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SettingsSavedL
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::SettingsSavedL ( 
    const TDesC8& /*aAppIdOfSavingItem */, const TDesC8& /*aAppRef*/,
    const TDesC8& /*aStorageIdValue*/ )
    {
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::SavingFinalizedL()
//
// ---------------------------------------------------------------------------
//
void CNATFWTraversalAdapter::SavingFinalizedL()
    {
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::DesToInt
// Converts a 8 bit descriptor to TInt.
// ---------------------------------------------------------------------------
//
TInt CNATFWTraversalAdapter::DesToInt( 
    const TDesC8& aDes )
    {
    TLex8 lex( aDes );
    TInt value = 0;
    lex.Val( value );
    return value;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::DesToTUint
// Converts a 8 bit descriptor to TUint.
// ---------------------------------------------------------------------------
//
TUint CNATFWTraversalAdapter::DesToTUint( 
    const TDesC8& aDes )
    {
    TLex8 lex( aDes );
    TInt value = 0;
    lex.Val( value );
    return value;
    }

// ---------------------------------------------------------------------------
// CNATFWTraversalAdapter::IapIdFromWapIdL
// Returns IAP ID from WAP ID.
// ---------------------------------------------------------------------------
//
TUint32 CNATFWTraversalAdapter::IapIdFromWapIdL( TUint32 aWapId )
    {
    DBG_PRINT( "CNATFWTraversalAdapter::IapIdFromWapIdL - begin" );
    RCmManagerExt cmManager;
    cmManager.OpenLC(); // CS:1
    RArray<TUint32> iapIds;

    TUint32 iapId( 0 );

    // First get all free IAP ID's.
    cmManager.ConnectionMethodL( iapIds );
    CleanupClosePushL( iapIds );  // CS:2

    // Then get IAP ID's from all destinations.
    RArray<TUint32> destIds;
    cmManager.AllDestinationsL( destIds );
    CleanupClosePushL( destIds );  // CS:3
    
    TInt destCount = destIds.Count();
    for ( TInt destIndex = 0; destIndex < destCount; destIndex++ )
        {
        RCmDestinationExt dest = cmManager.DestinationL( 
            destIds[destIndex] );
        
        CleanupClosePushL( dest ); // CS:4
        TInt cmCount = dest.ConnectionMethodCount();
        for ( TInt cmIndex = 0; cmIndex < cmCount; cmIndex++ )
            {
            TUint32 apId = dest.ConnectionMethodL( 
                cmIndex ).GetIntAttributeL( CMManager::ECmIapId );
            iapIds.AppendL( apId );
            }
        CleanupStack::PopAndDestroy( &dest ); // CS:3
        }
    // Finally, go through all connection methods and find correct IAP ID.
    const TInt cmCount = iapIds.Count();
    for ( TInt counter = 0; counter < cmCount; counter++ )
        {
        TUint32 id = cmManager.GetConnectionMethodInfoIntL( 
            iapIds[counter], CMManager::ECmWapId );

        if ( id == aWapId )
            {
            iapId = iapIds[counter];
            // No need to go through rest of IAPs.
            break;
            }
        }
    
    // PopAndDestroy destIds, iapIds, cmManager.
    CleanupStack::PopAndDestroy( 3, &cmManager ); // CS:0

    DBG_PRINT( "CNATFWTraversalAdapter::IapIdFromWapIdL - end" );
    return iapId;
    }

//  End of File
