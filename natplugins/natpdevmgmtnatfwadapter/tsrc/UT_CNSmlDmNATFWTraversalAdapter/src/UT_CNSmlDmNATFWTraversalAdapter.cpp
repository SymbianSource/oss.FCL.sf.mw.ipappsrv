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
#include "UT_CNSmlDmNATFWTraversalAdapter.h"
 
//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>

//  INTERNAL INCLUDES
#include "nsmldmnatfwtraversaladapter.h"
#include "natfwdomainentry.h"
#include "natfwiapentry.h"
#include "centralrepository.h"
#include "UT_StateQuery.h"
#include "SmlDmCallback_stub.h"

#include <unsafprotocolscrkeys.h>
#include "natfwdefaults.h" // Default values.

// Reference: OMA_Device_Management_DDF_for_NATFW_v1_0.doc
_LIT8( KNSmlDMNATFWDDFVersion, "1.0" );

// Domain specific settings
_LIT8( KDomain, "domain.example.com" );
_LIT8( KSTUNSrvAddr, "stunserver.example.com" );
_LIT8( KSTUNSrvPort, "88" );
_LIT8( KDomainNATRefreshTCP, "10" );
_LIT8( KDomainNATRefreshUDP, "20" );
_LIT8( KEnableCRLFRefresh, "True" );
_LIT8( KEnableCRLFRefresh2, "False" );
_LIT8( KSTUNSharedSecret, "True" );
_LIT8( KSTUNSharedSecret2, "False" );
_LIT8( KSTUNUsername, "tapeworm" );
_LIT8( KSTUNPassword, "cabin8" );
_LIT8( KAddSTUNUsername, "joppe" );
_LIT8( KAddSTUNPassword, "pl0p" );
_LIT8( KStartPort, "87" );
_LIT8( KEndPort, "5000" );
_LIT8( KNATProtocol, "nokia.ice,nokia.stun" );
// AdditionalSTUNServer settings
_LIT8( KSTUNSrvAddr2, "stunsrv2.example.com" );
_LIT8( KSTUNSrvPort2, "1069" );
// ICE settings
//_LIT8( KNATUtilities, "nokia.stun,nokia.turn" );
//_LIT8( KHostPreference, "125" );
//_LIT8( KServerReflexivePreference, "105" );
//_LIT8( KRelayPreference, "90" );
//_LIT8( KPeerReflexivePreference, "10" );
//_LIT8( KIPv4Preference, "8" );
//_LIT8( KIPv6Preference, "6" );
//_LIT8( KVPNPreference, "10" );
//_LIT8( KUDPPreference, "14" );
//_LIT8( KTCPPreference, "7" );
//_LIT8( KTCPActivePreference, "4" );
//_LIT8( KTCPPassivePreference, "1" );
//_LIT8( KTCPSimultaneousPreference, "5" );
// TURNServer settings
//_LIT8( KTURNSrvAddr, "turnsvr.example.com" );
//_LIT8( KTURNSrvPort, "2053" );
//_LIT8( KTURNUsername, "kaaleppi" );
//_LIT8( KTURNPassword, "turn6baa" );

// IAP specific settings
_LIT8( KPrefConRef, "AP/APId001" );
_LIT8( KPrefConRef2, "AP/APId999" );
_LIT8( KIAPNATRefreshTCP, "60" ); 
_LIT8( KIAPNATRefreshUDP, "70" ); 
_LIT8( KSTUNRetransmit,   "80" );


#define DBG_PRINTTI(p) RDebug::Print(_L(p))
#define DBG_PRINTTI_2(p,i) RDebug::Print(_L(p),i)


// CONSTRUCTION
UT_CNSmlDmNATFWTraversalAdapter* UT_CNSmlDmNATFWTraversalAdapter::NewL()
    {
    UT_CNSmlDmNATFWTraversalAdapter* self = UT_CNSmlDmNATFWTraversalAdapter::NewLC(); 
    CleanupStack::Pop( self );

    return self;
    }

UT_CNSmlDmNATFWTraversalAdapter* UT_CNSmlDmNATFWTraversalAdapter::NewLC()
    {
    UT_CNSmlDmNATFWTraversalAdapter* self = new( ELeave ) UT_CNSmlDmNATFWTraversalAdapter();
    CleanupStack::PushL( self );

    self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CNSmlDmNATFWTraversalAdapter::~UT_CNSmlDmNATFWTraversalAdapter()
    {
    }

// Default constructor
UT_CNSmlDmNATFWTraversalAdapter::UT_CNSmlDmNATFWTraversalAdapter()
    {
    }

// Second phase construct
void UT_CNSmlDmNATFWTraversalAdapter::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CNSmlDmNATFWTraversalAdapter::SetupL(  )
    {
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
    TInt err = rep->Reset();
    CleanupStack::PopAndDestroy( rep );  // CS:0
    
    TStates* states = new ( ELeave ) TStates;
    Dll::SetTls( static_cast<TAny*>( states ) );
    
    iDmCallback = CSmlDmCallback_stub::NewL();
    iNSmlDmNATFWTraversalAdapter = CNSmlDmNATFWTraversalAdapter::NewL( iDmCallback );

    iAllocDecorator = CEUnitTestCaseDecorator::ActiveTestCaseDecorator( 
            KEUnitAllocTestCaseDecoratorName ) ? ETrue : EFalse;
    }

void  UT_CNSmlDmNATFWTraversalAdapter::Setup2L(  )
    {
    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols ); // CS:1
    TInt err = rep->Reset();
    CleanupStack::PopAndDestroy( rep );  // CS:0
    
    TStates* states = new ( ELeave ) TStates;
    Dll::SetTls( static_cast<TAny*>( states ) );
    
    iDmCallback = CSmlDmCallback_stub::NewL();
    iNSmlDmNATFWTraversalAdapter = CNSmlDmNATFWTraversalAdapter::NewL( iDmCallback );
    
    // Setting the NATFW settings:
    
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test3"), KNullDesC8, 0 );
    iLuid1 = iDmCallback->iLUID->Des().AllocL();
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/Domain"), *iLuid1, KDomain, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSrvAddr"), *iLuid1, KSTUNSrvAddr, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSrvPort"), *iLuid1, KSTUNSrvPort, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNUsername"), *iLuid1, KSTUNUsername, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNPassword"), *iLuid1, KSTUNPassword, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/NATRefreshTCP"), *iLuid1, KDomainNATRefreshTCP, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/NATRefreshUDP"), *iLuid1, KDomainNATRefreshUDP, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/EnableCRLFRefresh"), *iLuid1, KEnableCRLFRefresh, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSharedSecret"), *iLuid1, KSTUNSharedSecret, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/PortPoolStartPort"), *iLuid1, KStartPort, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/PortPoolEndPort"), *iLuid1, KEndPort, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/UsedNATProtocol"), *iLuid1, KNATProtocol, KNullDesC8, 0 );    
    
    
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi"), *iLuid1, 0 );
    iLuid2 = iDmCallback->iLUID->Des().AllocL();
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvAddr"), *iLuid2, KSTUNSrvAddr2, KNullDesC8, 0 );    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvPort"), *iLuid2, KSTUNSrvPort2, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNUsername"), *iLuid2, KAddSTUNUsername, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNPassword"), *iLuid2, KAddSTUNPassword, KNullDesC8, 0 );
    
    
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/IAPSpecific/IAPTest3"), KNullDesC8, 0 );
    iLuid3 = iDmCallback->iLUID->Des().AllocL();

    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/PrefConRef"), *iLuid3, KPrefConRef, KNullDesC8, 0 ); 
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/NATRefreshTCP"), *iLuid3, KIAPNATRefreshTCP, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/NATRefreshUDP"), *iLuid3, KIAPNATRefreshUDP, KNullDesC8, 0 );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/STUNRetransmit"), *iLuid3, KSTUNRetransmit, KNullDesC8, 0 );

    iAllocDecorator = CEUnitTestCaseDecorator::ActiveTestCaseDecorator( 
            KEUnitAllocTestCaseDecoratorName ) ? ETrue : EFalse;
    }



void UT_CNSmlDmNATFWTraversalAdapter::Teardown(  )
    {
    delete iLuid1;
    iLuid1 = NULL;
    
    delete iLuid2;
    iLuid2 = NULL;
    
    delete iLuid3;
    iLuid3 = NULL;
    
    delete iNSmlDmNATFWTraversalAdapter;
    iNSmlDmNATFWTraversalAdapter = NULL;
    
    delete iDmCallback;
    iDmCallback = NULL;
    
    TStates* states = static_cast< TStates* > ( Dll::Tls() );
    states->Clean();
    delete states;
    states = NULL;
    Dll::FreeTls();
    }

inline void UT_CNSmlDmNATFWTraversalAdapter::DmTaPrint( TInt aStatus )
    {
    if ( !iAllocDecorator )
        {
        _LIT(KStatusFormat, "%d");
        EUNIT_PRINT( KStatusFormat, aStatus );
        }    
    }

inline void UT_CNSmlDmNATFWTraversalAdapter::DmTaPrint( const TDesC8& aText )
    {
    if ( !iAllocDecorator )
        {
        EUNIT_PRINT( aText );
        }
    }

    
void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_DDFVersionLL(  )
    {
    CBufBase *version = CBufFlat::NewL( 3 );
    CleanupStack::PushL( version );
    iNSmlDmNATFWTraversalAdapter->DDFVersionL( *version );
    HBufC8* resultHBufC = HBufC8::NewLC( version->Size() );
    resultHBufC->Des().Format( version->Ptr( 0 ) );

    EUNIT_ASSERT_EQUALS( KNSmlDMNATFWDDFVersion(), resultHBufC->Des() );
    EUNIT_PRINT( resultHBufC->Des() );
    CleanupStack::PopAndDestroy( resultHBufC );
    CleanupStack::PopAndDestroy( version );
    }
    

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_AddNodeObjectLL(  )
    {
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test1"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    iLuid1 = iDmCallback->iLUID->Des().AllocL();
    
    // Add second profile

    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test2"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    iLuid2 = iDmCallback->iLUID->Des().AllocL();
    
    // Test already existing profile
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test1"), *iLuid1, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EAlreadyExists == iDmCallback->iStatus );

    // Add AdditionalSTUNServer settings

    // Trying first without parent LUID
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test1/AdditionalSTUNServer/Testaus"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::ENotFound == iDmCallback->iStatus );
    
    // Then with correct LUID
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test1/AdditionalSTUNServer/Testaus"), *iLuid1, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );

    // Add an AdditionalSTUNServer under second DomainSpecific node.
    
    // First try with wrong parent LUID
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test2/AdditionalSTUNServer/Testaus"), _L8("12345678"), 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::ENotFound == iDmCallback->iStatus );
    
    // Then with correct one
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test2/AdditionalSTUNServer/Testaus"), *iLuid2, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    // Try to add another AdditionalSTUNServer under second DomainSpesific node.
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test2/AdditionalSTUNServer/Testausta"), *iLuid2, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    // Try to add third AdditionalSTUNServer under second DomainSpesific node.
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test2/AdditionalSTUNServer/TestaustaTaas"), *iLuid2, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );

/*    
    // ICE settings
    iStatus = 0;
    iDmModule->AddObjectL( _L8( "./NATFW/DomainSpecific/Test1/ICESpecific/NATUtilities" ), KNATUtilities, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/HostPreference"), KHostPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/ServerReflexivePreference"), KServerReflexivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/RelayPreference"), KRelayPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/PeerReflexivePreference"), KPeerReflexivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/IPv4Preference"), KIPv4Preference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/IPv6Preference"), KIPv6Preference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/VPNPreference"), KVPNPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/UDPPreference"), KUDPPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPPreference"), KTCPPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPActivePreference"), KTCPActivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPPassivePreference"), KTCPPassivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPSimultaneousPreference"), KTCPSimultaneousPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );


    // TURN settings

    // Add a TURNServer node
    iStatus = 0;
    iDmModule->AddObjectL( _L8( "./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1" ), KNullDesC8, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNSrvAddr"), KTURNSrvAddr, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNSrvPort"), KTURNSrvPort, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNUsername"), KTURNUsername, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNPassword"), KTURNPassword, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );
*/
    
    // IAPSpecific
    // test fetching when prefconref not yet set, link to AP not set yet.
    
    // Add IAP specific
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/IAPSpecific/IAPTest1"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    TPtr8 luid3( iDmCallback->iLUID->Des() );

    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/IAPSpecific/IAPTest2"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    // Test already existing profile
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/IAPSpecific/IAPTest1"), luid3, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EAlreadyExists == iDmCallback->iStatus );
    
    // Test fetching non existing AP reference
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest1/PrefConRef"), luid3, KNullDesC8, 0, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::ENotFound == iDmCallback->iStatus );
    }
 
void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectLL(  )
    {
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test3"), KNullDesC8, 0 );
    TPtr8 luid4( iDmCallback->iLUID->Des() );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/Domain"), luid4, KDomain, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );

    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSrvAddr"), luid4, KSTUNSrvAddr, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSrvPort"), luid4, KSTUNSrvPort, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNUsername"), luid4, KSTUNUsername, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNPassword"), luid4, KSTUNPassword, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/NATRefreshTCP"), luid4, KDomainNATRefreshTCP, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/NATRefreshUDP"), luid4, KDomainNATRefreshUDP, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/EnableCRLFRefresh"), luid4, KEnableCRLFRefresh, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSharedSecret"), luid4, KSTUNSharedSecret, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/PortPoolStartPort"), luid4, KStartPort, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/PortPoolEndPort"), luid4, KEndPort, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/UsedNATProtocol"), luid4, KNATProtocol, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    // AdditionalSTUNServer
    
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi"), luid4, 0 );
    DmTaPrint( iDmCallback->iStatus );
    luid4.Set( iDmCallback->iLUID->Des() );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvAddr"), luid4, KSTUNSrvAddr2, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvPort"), luid4, KSTUNSrvPort2, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNUsername"), luid4, KAddSTUNUsername, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNPassword"), luid4, KAddSTUNPassword, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );

/*
    // ICESpecific
    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/NATUtilities"), KNATUtilities, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/HostPreference"), KHostPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/ServerReflexivePreference"), KServerReflexivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/RelayPreference"), KRelayPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/PeerReflexivePreference"), KPeerReflexivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/IPv4Preference"), KIPv4Preference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/IPv6Preference"), KIPv6Preference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/VPNPreference"), KVPNPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/UDPPreference"), KUDPPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPPreference"), KTCPPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPActivePreference"), KTCPActivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPPassivePreference"), KTCPPassivePreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPSimultaneousPreference"), KTCPSimultaneousPreference, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    // TURNSpecific
    // Update the TURNServer node
    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNSrvAddr"), KTURNSrvAddr, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNSrvPort"), KTURNSrvPort, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNUsername"), KTURNUsername, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->AddObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNPassword"), KTURNPassword, KNullDesC8, 0 );
    iDmModule->EndMessageL();
    DmTaPrint( iStatus );
    EUNIT_ASSERT( iStatus == 200 );
*/

    // IAPSpecific
    // test fetching non existing AP reference

    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/IAPSpecific/IAPTest"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    luid4.Set( iDmCallback->iLUID->Des() );

    iDmCallback->SetStatusL( 0, MSmlDmAdapter::EOk );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest/PrefConRef"), luid4, KPrefConRef2, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EInvalidObject == iDmCallback->iStatus );

    iDmCallback->SetStatusL( 0, MSmlDmAdapter::EOk );
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest/PrefConRef"), luid4, KPrefConRef, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest/NATRefreshTCP"), luid4, KIAPNATRefreshTCP, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest/NATRefreshUDP"), luid4, KIAPNATRefreshUDP, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest/STUNRetransmit"), luid4, KSTUNRetransmit, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    }
    
void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectOtherLL(  )
    {
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/EnableCRLFRefresh"), *iLuid1, KEnableCRLFRefresh2, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSharedSecret"), *iLuid1, KSTUNSharedSecret2, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    }

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_FetchLeafObjectOtherLL(  )
    {
    TBuf8 <600> results;
    // Set the values that will be fetched
    CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectOtherLL();

    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/EnableCRLFRefresh"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KEnableCRLFRefresh2 );

    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSharedSecret"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSharedSecret2 );
    }

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_FetchLeafObjectLL(  )
    {
    TBuf8 <600> results;
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/Domain"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KDomain );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSrvAddr"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSrvAddr );
    
    // INT arvot antavat NOLLAN? 
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSrvPort"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSrvPort );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNUsername"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNUsername );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNPassword"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNPassword );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/NATRefreshTCP"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KDomainNATRefreshTCP );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/NATRefreshUDP"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KDomainNATRefreshUDP );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/EnableCRLFRefresh"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KEnableCRLFRefresh );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/STUNSharedSecret"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSharedSecret );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/PortPoolStartPort"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KStartPort );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/PortPoolEndPort"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KEndPort );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/UsedNATProtocol"), *iLuid1, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KNATProtocol );

    // AdditionalSTUNServer
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvAddr"), *iLuid2, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSrvAddr2 );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvPort"), *iLuid2, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSrvPort2 );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNUsername"), *iLuid2, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KAddSTUNUsername );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNPassword"), *iLuid2, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KAddSTUNPassword );

    // ICESpecific
/*    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/NATUtilities"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KNATUtilities );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/HostPreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KHostPreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/ServerReflexivePreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KServerReflexivePreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/RelayPreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KRelayPreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/PeerReflexivePreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KPeerReflexivePreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/IPv4Preference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KIPv4Preference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/IPv6Preference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KIPv6Preference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/VPNPreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KVPNPreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/UDPPreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KUDPPreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPPreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTCPPreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPActivePreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTCPActivePreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPPassivePreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTCPPassivePreference );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific/TCPSimultaneousPreference"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTCPSimultaneousPreference );


    // TURNSpecific
    // TURNSpecific/TURNServer
    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNSrvAddr"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTURNSrvAddr );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNSrvPort"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTURNSrvPort );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNUsername"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTURNUsername );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1/TURNPassword"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( results == KTURNPassword );*/

    // IAPSpecific
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/PrefConRef"), *iLuid3, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KPrefConRef );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/NATRefreshTCP"), *iLuid3, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KIAPNATRefreshTCP );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/NATRefreshUDP"), *iLuid3, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KIAPNATRefreshUDP );
    
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/IAPSpecific/IAPTest3/STUNRetransmit"), *iLuid3, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNRetransmit );
    }

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_ChildURIListLL(  )
    {
    TInt result( 0 );
    TInt status( 0 );
    CArrayFix<TSmlDmMappingInfo>* array = new ( ELeave ) CArrayFixFlat<TSmlDmMappingInfo>( 1 );
    CleanupStack::PushL( array );
    
    iNSmlDmNATFWTraversalAdapter->ChildURIListL(
        _L8("NATFW/DomainSpecific/Test3"), *iLuid1, *array, result, status );
    
    DmTaPrint( iDmCallback->iStatus );    
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    DmTaPrint( iDmCallback->iResult );    
    EUNIT_ASSERT( _L8("Domain/STUNSrvAddr/STUNSrvPort/STUNUsername/STUNPassword/NATRefreshTCP/NATRefreshUDP/EnableCRLFRefresh/AdditionalSTUNServer/STUNSharedSecret/PortPoolStartPort/PortPoolEndPort/UsedNATProtocol/ICESpecific/TURNSpecific/") == iDmCallback->iResult );    

    // IAP Spesific settings:
    
    iNSmlDmNATFWTraversalAdapter->ChildURIListL(
        _L8("NATFW/IAPSpecific/IAPTest3"), *iLuid3, *array, result, status );
    
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    DmTaPrint( iDmCallback->iResult );
    EUNIT_ASSERT( _L8("PrefConRef/NATRefreshTCP/NATRefreshUDP/STUNRetransmit/") == iDmCallback->iResult );
    
    // Additional STUN srv settings:
    
    iNSmlDmNATFWTraversalAdapter->ChildURIListL(
        _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi"), *iLuid2, *array, result, status );
    
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    DmTaPrint( iDmCallback->iResult );
    EUNIT_ASSERT( _L8("STUNSrvAddr/STUNSrvPort/STUNUsername/STUNPassword/") == iDmCallback->iResult );
    
    // ICE Spesific:

/*    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/ICESpecific"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results );
    EUNIT_ASSERT( iStatus == 200 );

    // TURN Spesific

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( iStatus == 200 );

    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( iStatus == 200 );*/
     
    // Test not found profile
    iNSmlDmNATFWTraversalAdapter->ChildURIListL(
        _L8("NATFW/IAPSpecific/NOTFOUND"), KNullDesC8, *array, result, status );
    
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::ENotFound == iDmCallback->iStatus );
    
    // test non existing node
    iNSmlDmNATFWTraversalAdapter->ChildURIListL( _L8("NATFW/notexisting"), KNullDesC8, *array, result, status );
    
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::ENotFound == iDmCallback->iStatus );
    
    array->Reset();
    CleanupStack::PopAndDestroy( array );
    }

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_DeleteNodeObjectLL(  )
    {
/*    iStatus = 0;
    iDmModule->DeleteObjectL( _L8("./NATFW/DomainSpecific/Test1/TURNSpecific/TURNServer/TS1"), 0 );
    iDmModule->EndMessageL();
    status.Num( iStatus );
    EUNIT_PRINT( status  ); 
    EUNIT_ASSERT( iStatus == 200 );*/
    
    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( 
        _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi"), *iLuid2, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/IAPSpecific/IAPTest2"), KNullDesC8, 0 );
    TPtr8 tmp( iDmCallback->iLUID->Des() );
    
    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( _L8("NATFW/IAPSpecific/IAPTest3"), *iLuid3, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( _L8("NATFW/IAPSpecific/IAPTest2"), tmp, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    
    iNSmlDmNATFWTraversalAdapter->AddNodeObjectL( _L8("NATFW/DomainSpecific/Test"), KNullDesC8, 0 );
    tmp.Set( iDmCallback->iLUID->Des() );
    
    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( _L8("NATFW/DomainSpecific/Test3"), *iLuid1, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( _L8("NATFW/DomainSpecific/Test"), tmp, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );


    // test deleting non existing profile

    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( _L8("NATFW/DomainSpecific/NOTEXISTING"), KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EInvalidObject == iDmCallback->iStatus );
    }

    
void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_DesToIntL(  )
    {
    EUNIT_ASSERT_EQUALS( iNSmlDmNATFWTraversalAdapter->DesToInt( _L8( "0" ) ), 0 );
    EUNIT_ASSERT_EQUALS( iNSmlDmNATFWTraversalAdapter->DesToInt( _L8( "9" ) ), 9 );
    EUNIT_ASSERT_EQUALS( iNSmlDmNATFWTraversalAdapter->DesToInt( _L8( "-1" ) ), -1 );
    }

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_RemoveLastURISegL(  )
    {
    // Test empty uri
    EUNIT_ASSERT_EQUALS_DESC( iNSmlDmNATFWTraversalAdapter->RemoveLastURISeg( KNullDesC8), KNullDesC8, NULL );
    
    // Test real URI
    EUNIT_ASSERT_EQUALS_DESC( iNSmlDmNATFWTraversalAdapter->RemoveLastURISeg( _L8( "./SIP/OutboundProxy/Host" ) ), _L8("./SIP/OutboundProxy"), NULL );
    }
    
void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_NumOfURISegsL(  )
    {
    // Test empty URI
    EUNIT_ASSERT_EQUALS( iNSmlDmNATFWTraversalAdapter->NumOfURISegs( KNullDesC8), 1 );
    
    // Test real URI
    EUNIT_ASSERT_EQUALS( iNSmlDmNATFWTraversalAdapter->NumOfURISegs( _L8("./SIP/OutboundProxy/Host") ), 4 );
    
    // Test ridiculously big URI
    EUNIT_ASSERT_EQUALS( iNSmlDmNATFWTraversalAdapter->NumOfURISegs( _L8("./SIP/OutboundPrsdglsadflkashofhewfoihawoiufhsdoxy/Hdlgkhaslkjbogoersogerpogsjeposgrgpsoejpsojieojieojivoismmiovgpmvsmgpesgoiergopveoimmoipgoadgasvinhuwavnhuievineihvhniavivvwaiwahvgivgiohgemgsoimpgeomriojgdghslkdgjoiwehfopihnbjiohbifpabsdfipoihost") ), 4 );
    }  
    
void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_LastURISegL(  )
    {
    EUNIT_ASSERT_EQUALS_DESC( iNSmlDmNATFWTraversalAdapter->LastURISeg( KNullDesC8), KNullDesC8, NULL );
    EUNIT_ASSERT_EQUALS_DESC( iNSmlDmNATFWTraversalAdapter->LastURISeg( _L8("./SIP/OutboundProxy/Host") ), _L8("Host"), NULL );
    }
    

void UT_CNSmlDmNATFWTraversalAdapter::CNSmlDmNATFWTraversalAdapter_OtherFunctionsLL(  )
    {
    iNSmlDmNATFWTraversalAdapter->StartAtomicL();
    
    iNSmlDmNATFWTraversalAdapter->CommitAtomicL();
    
    iNSmlDmNATFWTraversalAdapter->RollbackAtomicL();
    
    TInt a = 1;
    
    TBool test = iNSmlDmNATFWTraversalAdapter->StreamingSupport( a );
    EUNIT_ASSERT( !test );
        
    TBuf16< 8 > status;
    TBuf8 <600> results;

    // Various AdditionalSTUNServer tests.

    // First check that there is value set in Setup2L
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvPort"), *iLuid2, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSrvPort2 );
    
    // Then update the value
    iNSmlDmNATFWTraversalAdapter->UpdateLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvPort"), *iLuid2, KSTUNSrvPort, KNullDesC8, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );
    
    // And check that value has been updated
    iNSmlDmNATFWTraversalAdapter->FetchLeafObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi/STUNSrvPort"), *iLuid2, KNullDesC8, 0, 0 );
    results.Copy( iDmCallback->iResult );
    EUNIT_ASSERT( results == KSTUNSrvPort );
    
    iNSmlDmNATFWTraversalAdapter->DeleteObjectL( _L8("NATFW/DomainSpecific/Test3/AdditionalSTUNServer/Testi"), *iLuid2, 0 );
    DmTaPrint( iDmCallback->iStatus );
    EUNIT_ASSERT( MSmlDmAdapter::EOk == iDmCallback->iStatus );

    // Fetch all remaining interior nodes before fetching leaf nodes under them.
/*    iStatus = 0;
    iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test2/AdditionalSTUNServer"), KNullDesC8, 0, 0 );
    results.Copy( iResults->Ptr( 0 ) );
    EUNIT_PRINT( results  );
    EUNIT_ASSERT( iStatus == 200 );*/
    // AdditionalSTUNServer tests end.

    // See whether 'use STUN', 'use TURN' and 'offer ICE' flags are set on.
/*    CRepository* rep = CRepository::NewLC( KCRUidUNSAFProtocols );
    RArray<TUint32> keys;
    CleanupClosePushL( keys );
    rep->FindEqL( KUNSAFProtocolsDomainMask, KUNSAFProtocolsFieldTypeMask,
        KDomain(), keys );

    DBG_PRINTTI_2( "TM: test01: number of keys: %d", keys.Count() );

    if ( keys.Count() > 0 )
        {
        iStatus = 0;
        iDmModule->FetchObjectL( _L8("./NATFW/DomainSpecific/Test1/UsedNATProtocol"), KNullDesC8, 0, 0 );
        results.Copy( iResults->Ptr( 0 ) );
        DBG_PRINTTI( "TM: UsedNATProtocol for Test1:" );
        EUNIT_PRINT( results  );

        TUint32 key = keys[0]|KUNSAFProtocolsDomainMask;
        key &= KUNSAFProtocolsKeyMask;

        DBG_PRINTTI_2( "TM: test01 base key: %d", key );

        // Change Used NAT Protocol settings and check flags...
        _LIT8( KNewNatProtocol, "nokia.stun" );
        iStatus = 0;
        iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/UsedNATProtocol"), KNewNatProtocol, KNullDesC8, 0 );
        iDmModule->EndMessageL();
        status.Num( iStatus );
        EUNIT_PRINT( status  ); 
        EUNIT_ASSERT( iStatus == 200 );

        // Change Used NAT Protocol settings again and check flags...
        _LIT8( KNewNatProtocol2, "nokia.turn,nokia.ice" );
        iStatus = 0;
        iDmModule->UpdateObjectL( _L8("./NATFW/DomainSpecific/Test1/UsedNATProtocol"), KNewNatProtocol2, KNullDesC8, 0 );
        iDmModule->EndMessageL();
        status.Num( iStatus );
        EUNIT_PRINT( status  ); 
        EUNIT_ASSERT( iStatus == 200 );

        }
    CleanupStack::PopAndDestroy( &keys );
    CleanupStack::PopAndDestroy( rep );*/
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CNSmlDmNATFWTraversalAdapter,
    "Test DM NATFW Traversal adapter functionality.",
    "UNIT" )

EUNIT_TEST(
    "DDFVersionL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "DDFVersionL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_DDFVersionLL, Teardown)

EUNIT_TEST(
    "AddNodeObjectL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "AddNodeObjectL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_AddNodeObjectLL, Teardown)

EUNIT_TEST(
    "UpdateLeafObjectL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "UpdateLeafObjectL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectLL, Teardown)

EUNIT_TEST(
    "FetchLeafObjectL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "FetchLeafObjectL",
    "FUNCTIONALITY",
    Setup2L, CNSmlDmNATFWTraversalAdapter_FetchLeafObjectLL, Teardown)

EUNIT_TEST(
    "UpdateLeafObjectOtherLL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "UpdateLeafObjectOtherL",
    "FUNCTIONALITY",
    Setup2L, CNSmlDmNATFWTraversalAdapter_UpdateLeafObjectOtherLL, Teardown)

EUNIT_TEST(
    "FetchLeafObjectOtherLL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "FetchLeafObjectOtherL",
    "FUNCTIONALITY",
    Setup2L, CNSmlDmNATFWTraversalAdapter_FetchLeafObjectOtherLL, Teardown)

EUNIT_TEST(
    "ChildURIListL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "ChildURIListL",
    "FUNCTIONALITY",
    Setup2L, CNSmlDmNATFWTraversalAdapter_ChildURIListLL, Teardown)

EUNIT_TEST(
    "Misc functions - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "Miscellanious",
    "FUNCTIONALITY",
    Setup2L, CNSmlDmNATFWTraversalAdapter_OtherFunctionsLL, Teardown)

EUNIT_TEST(
    "DeleteNodeObjectL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "DeleteNodeObjectL",
    "FUNCTIONALITY",
    Setup2L, CNSmlDmNATFWTraversalAdapter_DeleteNodeObjectLL, Teardown)

EUNIT_TEST(
    "DesToIntL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "DesToIntL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_DesToIntL, Teardown)

EUNIT_TEST(
    "RemoveLastURISegL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "RemoveLastURISegL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_RemoveLastURISegL, Teardown)
    
EUNIT_TEST(
    "NumOfURISegsL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "NumOfURISegsL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_NumOfURISegsL, Teardown)
    
EUNIT_TEST(
    "LastURISegL - test ",
    "CNSmlDmNATFWTraversalAdapter",
    "LastURISegL",
    "FUNCTIONALITY",
    SetupL, CNSmlDmNATFWTraversalAdapter_LastURISegL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
