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




// INCLUDES
#include <e32debug.h>
#include "natfwconnectivityapidefs.h"
#include "natfwconnectivityapi.h"
#include <mmccinterface.h>
#include <mmcccodecinformation.h>
#include "mnatfwsocketmediaconnwrapper.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"

#include "mediateststubs.h"
#include "stunudpmediatest.h"
#include "testdefinitions.hrh"
#include "natfwtestconsolelogs.h"
#include <cnatfwsettingsapi.h>

const TInt KAudioPrefVoipAudioUplink           = 0x05220001;
const TInt KAudioPrefVoipAudioDownlink         = 0x05210001;
const TInt KDefaultQos = 46;
const TInt KRunningTime = 500000;

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CStunUdpMediaTestInterface::CStunUdpMediaTestInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CStunUdpMediaTest::CStunUdpMediaTest( CConsoleBase& aConsole,
    CNATFWConnectivityApi& aNat, CMediaTestStubs& aIFStub, TUint aIapId,
    TUint32 aDestAddr, TUint aDestPort ) :
    iConsole( aConsole ), iNat( aNat ), iIfStub( aIFStub ), iIapId( aIapId ),
    iDestAddr( aDestAddr ), iDestPort( aDestPort )
    {
    }
    
// ---------------------------------------------------------------------------
// CStunUdpMediaTest::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CStunUdpMediaTest::ConstructL( const TDesC8& aDomain )
    {
    iDomain = aDomain.AllocL();
    RDebug::Print( _L("CStunUdpMediaTest::ConstructL IAP: %d"), iIapId );
    }
    
// ---------------------------------------------------------------------------
// CStunUdpMediaTest::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CStunUdpMediaTest* CStunUdpMediaTest::NewL( CConsoleBase& aConsole,
    CNATFWConnectivityApi& aNat, CMediaTestStubs& aIFStub,
    const TDesC8& aDomain, TUint aIapId, TUint32 aDestAddr, TUint aDestPort )
    {
    CStunUdpMediaTest* self 
        = new ( ELeave ) CStunUdpMediaTest( aConsole, aNat, aIFStub, aIapId,
            aDestAddr, aDestPort );

    CleanupStack::PushL( self );
    self->ConstructL( aDomain );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CStunUdpMediaTest::~CStunUdpMediaTest
// Destructor.
// ---------------------------------------------------------------------------
//
CStunUdpMediaTest::~CStunUdpMediaTest()
    {
    delete iMccInterface;
    delete iDomain;
    iNat.CloseStreamL( iSessionId, iStreamId );
    iNat.CloseSessionL( iSessionId );
    }


// ---------------------------------------------------------------------------
// CStunUdpMediaTest::PrintLocalAddressL
// ---------------------------------------------------------------------------
//
void CStunUdpMediaTest::PrintLocalAddressL()
    {
    RDebug::Print( _L( "CStunUdpMediaTest::PrintLocalAddressL" ) );
    
    iConsole.Printf( _L( "Local address resolving started...\n" ) );
    
    TInt pluginId;
    
    CNATFWNatSettingsApi* settings = CNATFWNatSettingsApi::NewLC( iDomain->Des() );
    
    iSessionId = iNat.CreateSessionL( iIapId, iDomain->Des() );
    iIfStub.StartActiveSchedulerL();
    
    // Load plug in
    iNat.LoadPluginL( iSessionId, settings->AvailableNatProtocols(), pluginId);
    iIfStub.StartActiveSchedulerL( 20000 );
    CleanupStack::PopAndDestroy( settings );
    
    // STREAM
    iStreamId = iNat.CreateStreamL( iSessionId, KProtocolInetUdp, KDefaultQos );
    
    // Get local IP
    iNat.FetchCandidateL( iSessionId, iStreamId, KAfInet );
    iIfStub.StartActiveSchedulerL();

    TInetAddr localAddr;
    TRAPD( err, localAddr = iIfStub.LocalCandidateL().TransportAddr() );
    if ( KErrNone != err )
        {
        iNat.CloseStreamL( iSessionId, iStreamId );
        iNat.CloseSessionL( iSessionId );
        User::Leave( err );
        }
    
    TBuf<40> buffer;
    localAddr.Output( buffer );
    _LIT( KPrintLocalAddFormat, "\nLOCAL ADDRESS: %S PORT: %d\n");
    
    RDebug::Print( KPrintLocalAddFormat, &buffer, localAddr.Port() );
    iConsole.Printf( KPrintLocalAddFormat, &buffer, localAddr.Port() );
    }


// ---------------------------------------------------------------------------
// CStunUdpMediaTest::GetLocalAddressL
// ---------------------------------------------------------------------------
//    
TInt CStunUdpMediaTest::GetLocalAddress( TInetAddr& localAddr )
    {
    RDebug::Print( _L( "CStunUdpMediaTest::GetLocalAddress" ) );
    
    CNATFWCandidate* localCandidate = NULL;
    
    TRAPD( err, localCandidate = &iIfStub.LocalCandidateL() )
    if ( KErrNone == err )
        {
        localAddr.SetAddress( localCandidate->TransportAddr().Address() );
        localAddr.SetPort( localCandidate->TransportAddr().Port() );
        return KErrNone;        
        }
    return err;
    }
    
    
// ---------------------------------------------------------------------------
// CStunUdpMediaTest::TestUdpAudioL
// ---------------------------------------------------------------------------
//
void CStunUdpMediaTest::TestUdpAudioStreamingL()
    {
    TUint sessionId( 0 );
    TUint streamId( 0 );
    TInt pluginId;
    
    // UDP SESSIO
    RDebug::Print( _L("CStunUdpMediaTest::TestUdpAudioL - start") );
     
    iSessionId = iNat.CreateSessionL( iIapId, iDomain->Des() );
    sessionId = iSessionId;
    iIfStub.StartActiveSchedulerL();
    
    // load plug-in
    CNATFWNatSettingsApi* settings = CNATFWNatSettingsApi::NewLC( iDomain->Des() );
    iNat.LoadPluginL( iSessionId, settings->AvailableNatProtocols(), pluginId );
    iIfStub.StartActiveSchedulerL( 20000 );
    CleanupStack::PopAndDestroy( settings );    
    
    // STREAM
    streamId = iNat.CreateStreamL( sessionId, KProtocolInetUdp, KDefaultQos );
    iStreamId = streamId;
    // Get local IP that receives media
    iNat.FetchCandidateL( sessionId, streamId, KAfInet );
    iIfStub.StartActiveSchedulerL();
    
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- LOCAL ADDRESS: ", iReceiveMediaAddress )

    // WRAPPER
    MNATFWSocketMediaConnWrapper& mediaWrapper =
        iNat.CreateWrapperL( sessionId, streamId );

    TInetAddr destinationAddress;
    mediaWrapper.OutgoingAddr( destinationAddress );

    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- Wrapper LOCAL ADDRESS: ", destinationAddress )

    // NET settings value
    iNetsettings.iRemoteAddress.SetAddress( destinationAddress.Address() );
    iNetsettings.iRemoteAddress.SetPort( destinationAddress.Port() );
    iNetsettings.iRemoteRtcpPort = KDefaultRtcpPort;

    iNetsettings.iLocalAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalAddress.SetPort( KDefaultRtpPort );

    iNetsettings.iLocalPublicAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalRtcpPort = KDefaultRtcpPort;
    iNetsettings.iLocalPublicRtcpPort = KDefaultRtcpPort;
    iNetsettings.iMediaQosValue = KDefaultMediaQos;
    iNetsettings.iIapId = iIapId;

    TUint32 sessionId1;
    iMccInterface = CMccInterface::NewL( iIfStub );     
    User::LeaveIfError( iMccInterface->CreateSession( sessionId1 ) );

    // Create MCC LINK
    TUint32 linkId;
    User::LeaveIfError( iMccInterface->CreateLink( sessionId1, KMccLinkGeneral, linkId, iNetsettings ) ); 

    CActiveScheduler::Start();

    iNetsettings.iRemoteAddress.SetAddress( destinationAddress.Address() );
    iNetsettings.iRemoteAddress.SetPort( destinationAddress.Port() );
 
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iRemoteAddress: ", iIfStub.NetSettings().iRemoteAddress )
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress )

    User::LeaveIfError( iMccInterface->SetRemoteAddress( sessionId1, linkId, iNetsettings.iRemoteAddress ) );
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress)


    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iMccInterface->GetCapabilities( codecArray ) );

    
    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError( iMccInterface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError( iMccInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( iMccInterface->CreateStream( sessionId1, linkId, upStream, 
        KMccAudioUplinkStream, *codecArray[0] ) );
    
    
    // Create downstream
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError( iMccInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError( iMccInterface->AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iMccInterface->CreateStream( sessionId1, linkId, downStream,
        KMccAudioDownlinkStream, *codecArray[0] ) );
    
    // Set destination address to wrapper
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iLocalAddress: ",
        iIfStub.NetSettings().iLocalAddress )
    mediaWrapper.SetIncomingAddrL( iIfStub.NetSettings().iLocalAddress);

    mediaWrapper.SetReceivingStateL( EStreamingStateActive );
    mediaWrapper.SetSendingStateL( EStreamingStateActive );  
    
    TInetAddr destAddr;
    if( iDestAddr == 0 )
        {
        destAddr.SetAddress( destinationAddress.Address() );
        destAddr.SetPort( 5000 );
        }
    else
        {
        destAddr.SetAddress( iDestAddr );
        destAddr.SetPort( iDestPort );
        }
    
    //iNat.SetReceivingStateL( iSessionId, KMediaConnection, EStreamingStateActive );
    //iNat.SetSendingStateL( iSessionId, KMediaConnection, destAddr, EStreamingStateActive );

    
     // Prepare and start up and downstream  
    User::LeaveIfError( iMccInterface->PrepareStream( sessionId1, linkId, downStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    
    User::LeaveIfError( iMccInterface->PrepareStream( sessionId1, linkId, upStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();   
    
    iNat.SetReceivingStateL( iIfStub.LocalCandidateL(), EStreamingStateActive );
    iIfStub.StartActiveSchedulerL( KRunningTime );
    
    iNat.SetSendingStateL( iIfStub.LocalCandidateL(),
        EStreamingStateActive, destAddr );
    iIfStub.StartActiveSchedulerL( KRunningTime );
    
    User::LeaveIfError( iMccInterface->StartStream( sessionId1, linkId, downStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    
    User::LeaveIfError( iMccInterface->StartStream( sessionId1, linkId, upStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
     
    CleanupStack::PopAndDestroy( &codecArray );
    }


// ---------------------------------------------------------------------------
// CStunUdpMediaTest::PrepareAudioStreamingL
// ---------------------------------------------------------------------------
//
void CStunUdpMediaTest::PrepareAudioStreamingL()
    {
    TUint sessionId( 0 );
    TUint streamId( 0 );
    
    // UDP SESSIO
    RDebug::Print( _L("CStunUdpMediaTest::PrepareAudioStreamingL - start") );
    
    //iSessionId = 
    iNat.CreateSessionL( 6, iDomain->Des() );
    //sessionId = iSessionId;
    CActiveScheduler::Start();
    
    iSessionId = iNat.CreateSessionL( iIapId, iDomain->Des() );
    sessionId = iSessionId;
    iIfStub.StartActiveSchedulerL();
       
    // STREAM
    streamId = iNat.CreateStreamL( sessionId, KProtocolInetUdp, KDefaultQos );
    iStreamId = streamId;
    // Get local IP that receives media
    iNat.FetchCandidateL( sessionId, streamId, KAfInet );
    iIfStub.StartActiveSchedulerL();
    
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- LOCAL ADDRESS: ", iReceiveMediaAddress )

    // WRAPPER
    MNATFWSocketMediaConnWrapper& mediaWrapper =
        iNat.CreateWrapperL( sessionId, streamId );

    TInetAddr destinationAddress;
    mediaWrapper.OutgoingAddr( destinationAddress );

    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- Wrapper LOCAL ADDRESS: ", destinationAddress )

    // NET settings value
    iNetsettings.iRemoteAddress.SetAddress( destinationAddress.Address() );
    iNetsettings.iRemoteAddress.SetPort( destinationAddress.Port() );
    iNetsettings.iRemoteRtcpPort = KDefaultRtcpPort;

    iNetsettings.iLocalAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalAddress.SetPort( KDefaultRtpPort );

    iNetsettings.iLocalPublicAddress.SetAddress( KInetAddrAny );
    iNetsettings.iLocalRtcpPort = KDefaultRtcpPort;
    iNetsettings.iLocalPublicRtcpPort = KDefaultRtcpPort;
    iNetsettings.iMediaQosValue = KDefaultMediaQos;
    iNetsettings.iIapId = iIapId;

    TUint32 sessionId1;
    iMccInterface = CMccInterface::NewL( iIfStub );     
    User::LeaveIfError( iMccInterface->CreateSession( sessionId1 ) );

    // Create MCC LINK
    TUint32 linkId;
    User::LeaveIfError( iMccInterface->CreateLink( sessionId1, KMccLinkGeneral, linkId, iNetsettings ) ); 

    CActiveScheduler::Start();

    iNetsettings.iRemoteAddress.SetAddress( destinationAddress.Address() );
    iNetsettings.iRemoteAddress.SetPort( destinationAddress.Port() );
 
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iRemoteAddress: ", iIfStub.NetSettings().iRemoteAddress )
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress )

    User::LeaveIfError( iMccInterface->SetRemoteAddress( sessionId1, linkId, iNetsettings.iRemoteAddress ) );
    __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress)


    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iMccInterface->GetCapabilities( codecArray ) );

    
    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError( iMccInterface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError( iMccInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( iMccInterface->CreateStream( sessionId1, linkId, upStream, 
        KMccAudioUplinkStream, *codecArray[0] ) );
    
    
    // Create downstream
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError( iMccInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError( iMccInterface->AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iMccInterface->CreateStream( sessionId1, linkId, downStream,
        KMccAudioDownlinkStream, *codecArray[0] ) );
    
    // Set destination address to wrapper
     __NATFWTESTCONSOLE_ADDRLOG( "CStunUdpMediaTest::TestUdpAudioL- iNetsettings.iLocalAddress: ", iIfStub.NetSettings().iLocalAddress )
    mediaWrapper.SetIncomingAddrL( iIfStub.NetSettings().iLocalAddress);

    TInetAddr destAddr;
    if( iDestAddr == 0 )
        {
        destAddr.SetAddress( destinationAddress.Address() );
        destAddr.SetPort( 5000 );
        }
    else
        {
        destAddr.SetAddress( iDestAddr );
        destAddr.SetPort( iDestPort );
        }
    
     // Prepare and start up and downstream  
    User::LeaveIfError( iMccInterface->PrepareStream( sessionId1, linkId, downStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    
    User::LeaveIfError( iMccInterface->PrepareStream( sessionId1, linkId, upStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();   

    User::LeaveIfError( iMccInterface->StartStream( sessionId1, linkId, downStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    
    User::LeaveIfError( iMccInterface->StartStream( sessionId1, linkId, upStream ) );
    CActiveScheduler::Start();
    CActiveScheduler::Start();
    /*
    iNat.SetReceivingStateL( iSessionId, streamId, EStreamingStateActive );
    iNat.SetSendingStateL( iSessionId, streamId, destAddr, EStreamingStateActive );
    */
    CleanupStack::PopAndDestroy( &codecArray );
    
    RDebug::Print( _L("CStunUdpMediaTest::PrepareAudioStreamingL - stop") );
    }

