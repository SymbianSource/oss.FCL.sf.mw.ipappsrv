/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include "test_interface.h"
#include "mmcccodecinformation.h"
#include "mccteststubs.h"
#include "mmccinterface.h"
#include "testdefinitions.hrh"

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CTestInterfaceInterface::CTestInterfaceInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CTestInterface::CTestInterface( CConsoleBase& aConsole,
    const TMccNetSettings& aSettings, CMccInterface& aIF, CMccInterfaceStub& aIFStub )
    :
    iConsole( aConsole ), iNetsettings( aSettings ), iInterface( aIF ),
    iIfStub( aIFStub )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CTestInterface::ConstructL()
    {
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CTestInterface* CTestInterface::NewL( CConsoleBase& aConsole,
    const TMccNetSettings& aSettings, CMccInterface& aIF, CMccInterfaceStub& aIFStub )
    {
    CTestInterface* self 
        = new ( ELeave ) CTestInterface( aConsole, aSettings, aIF, aIFStub );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTestInterface::~CTestInterface
// Destructor.
// ---------------------------------------------------------------------------
//
CTestInterface::~CTestInterface()
    {
    }

// ---------------------------------------------------------------------------
// CTestInterface::DoRunInterfaceTestsL
// 
// ---------------------------------------------------------------------------
//
void CTestInterface::DoRunInterfaceTestsL()
    {
    iConsole.ClearScreen();
    iConsole.Printf( _L("\n*** INTERFACE TESTS ***\n") );
    
    TInt err( KErrNone );
    iConsole.Printf( _L("GET CAPABILITIES\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetCapabilitiesL") );
    __UHEAP_MARK;
    TRAP( err, TestGetCapabilitiesL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetCapabilitiesL, ERR: %d"), err );
    
    
    iConsole.Printf( _L("CREATE SINGLE SESSION\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSingleSessionCreationL") );
    __UHEAP_MARK;
    TRAP( err, TestSingleSessionCreationL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSingleSessionCreationL, ERR: %d"), err );


    iConsole.Printf( _L("CREATE SINGLE LINK\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSingleLinkCreationL") );
    __UHEAP_MARK;
    TRAP( err, TestSingleLinkCreationL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSingleLinkCreationL, ERR: %d"), err );


    iConsole.Printf( _L("CREATE SINGLE STREAM\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSingleStreamCreationL") );
    //__UHEAP_MARK;
    TRAP( err, TestSingleStreamCreationL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSingleStreamCreationL, ERR: %d"), err );

    
    iConsole.Printf( _L("PREPARE SINGLE STREAM\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSingleStreamPreparingL") );
    __UHEAP_MARK;
    iConsole.Printf( _L("Audio uplink\n") );
    TRAP( err, TestSingleStreamPreparingL( KMccLinkGeneral, KMccAudioUplinkStream,
        KUidMmfAudioInput, KMccRtpSinkUid, KAMRSdpName ) );
    __UHEAP_MARKEND;
    
    
    iConsole.Printf( _L("Audio downlink\n") );
    __UHEAP_MARK;
    TRAP( err, TestSingleStreamPreparingL( KMccLinkGeneral, KMccAudioDownlinkStream,
        KMccRtpSourceUid, KUidMmfAudioOutput, KAMRSdpName ) );
    __UHEAP_MARKEND;
    //TestSingleStreamPreparingL( KPCMUSdpName );
    //TestSingleStreamPreparingL( KPCMASdpName );
    //TestSingleStreamPreparingL( KILBCSdpName );
    //TestSingleStreamPreparingL( KG729SdpName );
    __UHEAP_MARK;
    iConsole.Printf( _L("Invalid source/sink 1\n") );
    // INVALID SOUCE/SINK, SHOULD NOT PASS TEST
    TRAP( err, TestSingleStreamPreparingL( KMccLinkGeneral, KMccAudioUplinkStream,
        KMccRtpSourceUid, KUidMmfAudioOutput, KAMRSdpName ) );
    __UHEAP_MARKEND;
    __UHEAP_MARK;
    iConsole.Printf( _L("Invalid source/sink 2\n") );
    // INVALID SOUCE/SINK, SHOULD NOT PASS TEST
    TRAP( err, TestSingleStreamPreparingL( KMccLinkGeneral, KMccAudioDownlinkStream,
        KUidMmfAudioInput, KMccRtpSinkUid, KAMRSdpName ) );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSingleStreamPreparingL, ERR: %d"), err );


    iConsole.Printf( _L("PLAY SINGLE STREAM\n") );
    iConsole.Printf( _L("Audio uplink\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSingleStreamPlayingL") );
    __UHEAP_MARK;
    TRAP( err, TestSingleStreamPlayingL( KMccLinkGeneral, KMccAudioUplinkStream,
        KUidMmfAudioInput, KMccRtpSinkUid, KAMRSdpName ) );
    __UHEAP_MARKEND;
    __UHEAP_MARK;
    iConsole.Printf( _L("Audio downlink\n") );
    TRAP( err, TestSingleStreamPlayingL( KMccLinkGeneral, KMccAudioDownlinkStream,
        KMccRtpSourceUid, KUidMmfAudioOutput, KAMRSdpName ) );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSingleStreamPlayingL, ERR: %d"), err );
    

    iConsole.Printf( _L("PAUSE SINGLE STREAM\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSingleStreamPausingL") );
    __UHEAP_MARK;
    iConsole.Printf( _L("Audio uplink\n") );
    TRAP( err, TestSingleStreamPausingL( KMccLinkGeneral, KMccAudioUplinkStream,
        KUidMmfAudioInput, KMccRtpSinkUid, KAMRSdpName ) );
    __UHEAP_MARKEND;
    __UHEAP_MARK;
    iConsole.Printf( _L("Audio downlink\n") );
    TRAP( err, TestSingleStreamPausingL( KMccLinkGeneral, KMccAudioDownlinkStream,
        KMccRtpSourceUid, KUidMmfAudioOutput, KAMRSdpName ) );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSingleStreamPausingL, ERR: %d"), err );
    
    iConsole.Printf( _L("MUTE AUDIO\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestAudioMuteL") );
    //__UHEAP_MARK;
    TRAP( err, TestAudioMuteL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestAudioMuteL, ERR: %d"), err );
    
    iConsole.Printf( _L("SET CODEC\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSetCodecL") );
    __UHEAP_MARK;
    TRAP( err, TestSetCodecL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSetCodecL, ERR: %d"), err );
    

    iConsole.Printf( _L("GET CODEC\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetCodecL") );
    //__UHEAP_MARK;
    TRAP( err, TestGetCodecL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetCodecL, ERR: %d"), err );


    iConsole.Printf( _L("GET SSRC\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestGetSSRCL") );
    __UHEAP_MARK;
    TRAP( err, TestGetSSRCL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestGetSSRCL, ERR: %d"), err );

    
    iConsole.Printf( _L("INACTIVITY TIMER\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestInactivityTimerL") );
    __UHEAP_MARK;
    TRAP( err, TestInactivityTimerL() );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestInactivityTimerL, ERR: %d"), err );
    
    
    iConsole.Printf( _L("FAST SHUTDOWNS\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestFastShutdownsL") );
    //__UHEAP_MARK;
    TRAP( err, TestFastShutdownsL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestFastShutdownsL, ERR: %d"), err );
    
    
    iConsole.Printf( _L("CREATE MULTI STREAM/LINK/SESSION\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestMultiStreamLinkSessionL") );
    //__UHEAP_MARK;
    TRAP( err, TestMultiStreamLinkSessionL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestMultiStreamLinkSessionL, ERR: %d"), err );
    
    
    iConsole.Printf( _L("MULTI STREAM PREPARE\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestMultiStreamPreparingL") );
    //__UHEAP_MARK;
    TRAP( err, TestMultiStreamPreparingL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestMultiStreamPreparingL, ERR: %d"), err );
    

    iConsole.Printf( _L("RESOURCE REUSE\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestResourceReuseL") );
    //__UHEAP_MARK;
    TRAP( err, TestResourceReuseL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestResourceReuseL, ERR: %d"), err );
    
#ifndef __WINSCW__
    iConsole.Printf( _L("BASIC AUDIO STREAMING\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestAudioStreamingL") );
    //__UHEAP_MARK;

    TRAP( err, TestAudioStreamingL( KAMRSdpName ) );
    RDebug::Print( _L("AMR: %d"), err );

    TRAP( err, TestAudioStreamingL( KILBCSdpName ) );
    RDebug::Print( _L("iLBC: %d"), err );
    TRAP( err, TestAudioStreamingL( KPCMUSdpName ) );
    RDebug::Print( _L("PCMU: %d"), err );
    TRAP( err, TestAudioStreamingL( KPCMASdpName ) );
    RDebug::Print( _L("PCMA: %d"), err );
    TRAP( err, TestAudioStreamingL( KG729SdpName ) );
    RDebug::Print( _L("G729: %d"), err );

    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestAudioStreamingL, ERR: %d"), err );
    
    iConsole.Printf( _L("UP/DOWNLINK PAUSE\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestMultiStreamPausingL") );
    //__UHEAP_MARK;
    TRAP( err, TestMultiStreamPausingL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestMultiStreamPausingL, ERR: %d"), err );

    
    iConsole.Printf( _L("AMR-NB FEC\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestAmrFecL") );
    //__UHEAP_MARK;
    TRAP( err, TestAmrFecL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestAmrFecL, ERR: %d"), err );

    
    iConsole.Printf( _L("2198 REDUNDANCY\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestRedundancyL") );
    //__UHEAP_MARK;
    
    TRAP( err, TestRedundancyL( KAMRSdpName ) );
    RDebug::Print( _L("AMR RED: %d"), err );

    TRAP( err, TestRedundancyL( KPCMASdpName ) );
    RDebug::Print( _L("PCMU RED: %d"), err );
    
    TRAP( err, TestRedundancyL( KPCMUSdpName ) );
    RDebug::Print( _L("PCMU RED: %d"), err );
    
    TRAP( err, TestRedundancyL( KILBCSdpName ) );
    RDebug::Print( _L("iLBC RED: %d"), err );
    
    TRAP( err, TestRedundancyL( KG729SdpName ) );
    RDebug::Print( _L("G729: %d"), err );
    
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestRedundancyL, ERR: %d"), err );
    
    iConsole.Printf( _L("AUDIO ROUTING\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestAudioRoutingL") );
    //__UHEAP_MARK;
    //TRAP( err, TestAudioRoutingL() );
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestAudioRoutingL, ERR: %d"), err );
    
#endif
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::TestGetCapabilitiesL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestGetCapabilitiesL() const
    {
    RDebug::Print( _L("CTestInterface::TestGetCapabilitiesL") );

    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface.GetCapabilities( codecArray ) );
    
    if ( 0 == codecArray.Count() )
        {
        RDebug::Print( _L("CTestInterface::TestGetCapabilitiesL - KErrUnknown") );
        User::Leave( KErrUnknown );
        }
    
    CleanupStack::PopAndDestroy( &codecArray );
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::TestSingleSessionCreationL
// Test explicit/implicit session creation and destruction.
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSingleSessionCreationL()
    {
    RDebug::Print( _L("CTestInterface::TestSingleSessionCreationL") );

    TUint32 sessionId1;
    User::LeaveIfError( iInterface.CreateSession( sessionId1 ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId1 ) );
    
    TUint32 sessionId2;
    User::LeaveIfError( iInterface.CreateSession( sessionId2 ) );
    if ( KErrNone == iInterface.CloseSession( 0 ) )
        {
        RDebug::Print( _L("CTestInterface::TestSingleSessionCreationL - KErrUnknown") );
        User::Leave( KErrUnknown );
        }
    
    User::LeaveIfError( iInterface.CloseSession( sessionId2 ) );
    }


// ---------------------------------------------------------------------------
// CTestInterface::TestSingleLinkCreationL
// 
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSingleLinkCreationL()
    {
    RDebug::Print( _L("CTestInterface::TestSingleLinkCreationL") );
    
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    
    RDebug::Print( _L("CTestInterface::TestSingleLinkCreationL KMccLinkGeneral") );
    TUint32 linkId1;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId1,
            iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId1, 0, KMccLinkCreated );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId1 ) );
    
    RDebug::Print( _L( "CTestInterface::TestSingleLinkCreationL KMccLinkLocal" ) );
    TUint32 linkId2;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkLocal, linkId2,
            iNetsettings ) );
    
    iIfStub.WaitForEvent( sessionId, linkId2, 0, KMccLinkCreated );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId2 ) );

    TInt err
        = iInterface.CreateLink( sessionId + 10, KMccLinkLocal, linkId2, iNetsettings );
    
    if ( KErrNotFound != err )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    TUint32 linkId3;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId3,
            iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId3, 0, KMccLinkCreated );
    
    if ( KErrNone == iInterface.CloseLink( 0, 0 ) )
        {
        RDebug::Print( _L("CTestInterface::TestSingleLinkCreationL - KErrUnknown 1") );
        User::Leave( KErrUnknown );
        }
    
    if ( KErrNone == iInterface.CloseLink( sessionId, 0 ) )
        {
        RDebug::Print( _L("CTestInterface::TestSingleLinkCreationL - KErrUnknown 2") );
        User::Leave( KErrUnknown );
        }
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId3 ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::TestSingleStreamCreationL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSingleStreamCreationL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    
    RDebug::Print( _L( "CTestInterface::TestSingleStreamCreationL KMccLinkGeneral" ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId,
            iNetsettings ) );
    
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );

    // AUDIO UPLINK STREAM CREATION
    RDebug::Print( _L("CTestInterface::TestSingleStreamCreationL ADD RSC AUDIO UL") );
    TUint32 rtpSinkId;
    TUint32 audioSourceId;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, rtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, audioSourceId ) );
    
    RDebug::Print( _L("CTestInterface::TestSingleStreamCreationL CREATE AUDIO UL") );
    TUint32 streamId1;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId1, 
        KMccAudioUplinkStream, *codec ) );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId1 ) );
    iInterface.RemoveDataSource( sessionId, linkId, streamId1, audioSourceId );
    iInterface.RemoveDataSink( sessionId, linkId, streamId1, rtpSinkId );
    
    // AUDIO DOWNLINK STREAM CREATION
    RDebug::Print( _L("CTestInterface::TestSingleStreamCreationL ADD RSC AUDIO DL") );
    TUint32 audioSinkId;
    TUint32 rtpSourceId;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, audioSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, rtpSourceId ) );
    
    RDebug::Print( _L("CTestInterface::TestSingleStreamCreationL CREATE AUDIO DL") );
    TUint32 streamId2;
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId2, 
        KMccAudioDownlinkStream, *codec ) );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId2 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId2, audioSinkId );
    iInterface.RemoveDataSource( sessionId, linkId, streamId2, rtpSourceId );
    
    // AUDIO LOCAL STREAM CREATION
    
    // AUDIO DOWNLINK STANDBY STREAM CREATION
    
    // VIDEO UPLINK STREAM CREATION
    
    // VIDEO DOWNLINK STREAM CREATION
    
    // VIDEO LOCAL STREAM CREATION
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }


// ---------------------------------------------------------------------------
// CTestInterface::TestSingleStreamPreparingL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSingleStreamPreparingL( TInt aLinkType,
    TInt aStreamType, TUid aDataSource, TUid aDataSink, const TDesC8& aCodec )
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, aLinkType, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    // STREAM PREPARING, IMPLICIT ENDPOINT PREPARE
    RDebug::Print( _L( "CTestInterface::TestSingleStreamPreparingL AUDIO UL, CODEC: %S" ), &aCodec );
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( aDataSink, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( aDataSource, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId1;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, aCodec );
    CleanupStack::PushL( codec );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId1, 
        aStreamType, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.SetRemoteAddress( sessionId, linkId, 
        iNetsettings.iRemoteAddress, iNetsettings.iRemoteRtcpPort ) );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );

    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId1 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId1, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId1, dataSourceId1 );

    // STREAM PREPARING, EXPLICIT ENDPOINT PREPARE
    TUint32 streamId2;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( aDataSink, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( aDataSource, KNullDesC8, dataSourceId2 ) );
    
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId2, 
        aStreamType, *codec ) );
    
    // Network resources must be prepared first
    TUint32 prepareId1 = ( KMccRtpSinkUid == aDataSink ) ? dataSinkId2 : dataSourceId2;
    TUint32 prepareId2 = ( prepareId1 == dataSinkId2 ) ? dataSourceId2 : dataSinkId2;
    User::LeaveIfError( 
        iInterface.PrepareStream( sessionId, linkId, streamId2, prepareId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, prepareId1, KMccStreamPrepared );
    
    User::LeaveIfError( 
        iInterface.PrepareStream( sessionId, linkId, streamId2, prepareId2 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, prepareId2, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId2 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId2, dataSinkId2 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId2, dataSourceId2 );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    

// ---------------------------------------------------------------------------
// CTestInterface::TestSingleStreamPlayingL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSingleStreamPlayingL( TInt aLinkType,
    TInt aStreamType, TUid aDataSource, TUid aDataSink, const TDesC8& aCodec )
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, aLinkType, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
            
    RDebug::Print( _L("CTestInterface::TestSingleStreamPlayingL: %S"), &aCodec );
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( aDataSink, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( aDataSource, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId1;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, aCodec );
    CleanupStack::PushL( codec );
    
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId1, 
        aStreamType, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );

    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop (1/4)\n" ) );
    iConsole.Getch();
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );

    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId1 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId1, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId1, dataSourceId1 );
    // STREAM DESTRUCTION FROM PLAYING STATE
/*
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( aDataSink, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( aDataSource, KNullDesC8, dataSourceId2 ) );
    
    TUint32 streamId2;
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId2, 
        aStreamType, *codec ) );
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId2 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, streamId2 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop (2/4)\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId2 ) );
*/

    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    
// ---------------------------------------------------------------------------
// CTestInterface::TestSingleStreamPausingL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSingleStreamPausingL( TInt aLinkType,
    TInt aStreamType, TUid aDataSource, TUid aDataSink, const TDesC8& aCodec )
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, aLinkType, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
        
    RDebug::Print( _L("CTestInterface::TestSingleStreamPausingL: %S"), &aCodec );
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( aDataSink, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( aDataSource, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId1;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, aCodec );
    CleanupStack::PushL( codec );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId1, 
        aStreamType, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStarted );

    iConsole.Printf( _L( "Press a key to pause\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.PauseStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPaused );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPaused );

    iConsole.Printf( _L( "Press a key to resume\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.ResumeStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamResumed );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamResumed );

    iConsole.Printf( _L( "Press a key to stop\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );

    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId1 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId1, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId1, dataSourceId1 );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }

// ---------------------------------------------------------------------------
// CTestInterface::TestAudioMuteL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestAudioMuteL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
        
    // Create link
    TUint32 linkId;
    User::LeaveIfError( iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
        
    User::LeaveIfError( iInterface.SetRemoteAddress( sessionId, linkId, iNetsettings.iRemoteAddress ) );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError( iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError( iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    codec->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, upStream, 
        KMccAudioUplinkStream, *codec ) );

    // Create downstream
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError( iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError( iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    codec->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, downStream,
        KMccAudioDownlinkStream, *codec ) );
    
    // Prepare and start up and downstream       
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
        
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
        
    // Pause upstream
    iConsole.Printf( _L( "Press a key to mute audio\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.PauseStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPaused );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPaused );
    
    // Resume upstream
    iConsole.Printf( _L( "Press a key to unmute audio\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.ResumeStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamResumed );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamResumed );
    
    iConsole.Printf( _L( "Press a key to stop\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
        
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, upStream ) );
    iInterface.RemoveDataSink( sessionId, linkId, upStream, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, upStream, dataSourceId1 );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, downStream ) );
    iInterface.RemoveDataSink( sessionId, linkId, downStream, dataSinkId2 );
    iInterface.RemoveDataSource( sessionId, linkId, downStream, dataSourceId2 );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }

// ---------------------------------------------------------------------------
// CTestInterface::TestSetCodecL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestSetCodecL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId1;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId1, 
        KMccAudioUplinkStream, *codec ) );
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStarted );
    
    // CHANGING CODEC SHOULD CAUSE AN ERROR
    /*
    CMccCodecInformation* codec2
        = FetchCodecByMimeSubtypeNameL( iInterface, KPCMUSdpName );
    CleanupStack::PushL( codec2 );
    TInt err = iInterface.SetCodec( sessionId, linkId, streamId1, *codec2 );
    if ( KErrArgument != err )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    */
    // TBD: TEST CHANGING OF CODEC PARAMETERS
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId1 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId1, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId1, dataSourceId1 );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    //CleanupStack::PopAndDestroy( codec2 );
    CleanupStack::PopAndDestroy( codec );
    }
    
    
// ---------------------------------------------------------------------------
// CTestInterface::TestGetCodecL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestGetCodecL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId, 
        KMccAudioUplinkStream, *codec ) );
    
    CMccCodecInformation* gotCodec 
        = iInterface.CodecL( sessionId, linkId, streamId );
    CleanupStack::PushL( gotCodec );
    
    if ( gotCodec && gotCodec->FourCC() != codec->FourCC() )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId, dataSourceId1 );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( gotCodec );
    CleanupStack::PopAndDestroy( codec );
    }


// ---------------------------------------------------------------------------
// CTestInterface::TestGetSSRCL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestGetSSRCL()
	{
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    // UPLINK STREAM
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId1;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId1, 
        KMccAudioUplinkStream, *codec ) );
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamPrepared );

    TUint32 ssrc( 0 );
    User::LeaveIfError(
        iInterface.GetSSRC( sessionId, linkId, streamId1, dataSinkId1, ssrc ) );
    if ( 0 == ssrc )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    // DOWNLINK STREAM
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId2 ) );

    TUint32 streamId2;
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId2, 
        KMccAudioUplinkStream, *codec ) );
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, streamId2 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamPrepared );
        
    User::LeaveIfError(
        iInterface.GetSSRC ( sessionId, linkId, streamId2, dataSinkId1, ssrc ) );
    if ( 0 == ssrc )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, streamId1 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, streamId1, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, streamId2 ) );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, streamId2, KMccStreamStopped );    
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId1 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId1, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId1, dataSourceId1 );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId2 ) );
    iInterface.RemoveDataSink( sessionId, linkId, streamId2, dataSinkId1 );
    iInterface.RemoveDataSource( sessionId, linkId, streamId2, dataSourceId1 );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
	}
	
   
// ---------------------------------------------------------------------------
// CTestInterface::TestInactivityTimerL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestInactivityTimerL()
    {
    const TUint KLongInactivityTimeout( 1000 ); // 1 second

    TUint32 session;
    User::LeaveIfError( iInterface.CreateSession( session ) );

    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    
    // Create downlink and stream
    TUint32 linkId, downStream;
    User::LeaveIfError( 
        iInterface.CreateLink( session, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( session, linkId, 0, KMccLinkCreated );
    
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId1 ) );
    User::LeaveIfError( iInterface.CreateStream( session, linkId, downStream,
                                                 KMccAudioDownlinkStream, *codec ) );

    // Start, wait for inactivity event and stop timer
    User::LeaveIfError( iInterface.StartInactivityTimer( session, linkId, downStream,
                        KLongInactivityTimeout, dataSourceId1 ) );
    iIfStub.WaitForEvent( session, linkId, downStream, KMccInactivityEvent );
    
    /*
    // Timer is automatically stopped after sending inactivity event
    User::LeaveIfError( iInterface.StopInactivityTimer( session, linkId, downStream,
        dataSourceId1 ) );
    */
    
    // TRY TO USE INVALID TIMEOUT TIME
    TInt err = iInterface.StartInactivityTimer( session, linkId, downStream,
        0, dataSourceId1 );
    if ( KErrArgument != err )
        {
        RDebug::Print( _L("CTestInterface::TestInactivityTimerL ERR: %d" ),
            err );
        RDebug::Print( _L("Timer should not work with timeout 0") );
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    User::LeaveIfError( iInterface.DeleteStream( session, linkId, downStream ) );
    iInterface.RemoveDataSink( session, linkId, downStream, dataSinkId1 );
    iInterface.RemoveDataSource( session, linkId, downStream, dataSourceId1 );
                            
    // TRY TO USE TIMER WITH UL-STREAM
    TUint32 upStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId2 ) );
    User::LeaveIfError( iInterface.CreateStream( session, linkId, upStream,
        KMccAudioUplinkStream, *codec ) );
    
    err = iInterface.StartInactivityTimer( session, linkId, upStream, dataSinkId2,
        KLongInactivityTimeout );
    if ( KErrNone == err )
        {
        RDebug::Print( _L("Timer should not work on ul-stream") );
        User::Leave( KErrTotalLossOfPrecision );
        }

    User::LeaveIfError( iInterface.DeleteStream( session, linkId, upStream ) );
    iInterface.RemoveDataSink( session, linkId, upStream, dataSinkId2 );
    iInterface.RemoveDataSource( session, linkId, upStream, dataSourceId2 );
    
    User::LeaveIfError( iInterface.CloseLink( session, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( session ) );
    CleanupStack::PopAndDestroy( codec );
    }


// ---------------------------------------------------------------------------
// CTestInterface::TestMultiStreamLinkSessionL
// Tests stream creation to multiple links and sessions.
// ---------------------------------------------------------------------------
//
void CTestInterface::TestMultiStreamLinkSessionL()
    {
    RArray<TUint32> sessIds;
    CleanupClosePushL( sessIds );
    RArray<TUint32> linkIds;
    CleanupClosePushL( linkIds );
    RArray<TUint32> strmIds;
    CleanupClosePushL( strmIds );
    RArray<TUint32> sourceIds;
    CleanupClosePushL( sourceIds );
    RArray<TUint32> sinkIds;
    CleanupClosePushL( sinkIds );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    for ( TInt sessCnt = 0; sessCnt < 3; sessCnt++ )
        {
        TUint32 sessId;
        User::LeaveIfError( iInterface.CreateSession( sessId ) );
        sessIds.Append( sessId );
        
        for ( TInt linkCnt = 0; linkCnt < 3; linkCnt++ )
            {
            TUint32 linkId;
            User::LeaveIfError(
                iInterface.CreateLink( 
                sessIds[sessCnt], KMccLinkGeneral, linkId, iNetsettings ) );
            
            iIfStub.WaitForEvent( sessId, linkId, 0, KMccLinkCreated );
            linkIds.Append( linkId );
            
            // UL-STREAM
            TUint32 dataSinkId;
            TUint32 dataSourceId;
            User::LeaveIfError(
                iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId ) );
            User::LeaveIfError(
                iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId ) );
            sourceIds.Append( dataSourceId );
            sinkIds.Append( dataSinkId );
            
            TUint32 ulStreamId;
            User::LeaveIfError( 
                iInterface.CreateStream( sessIds[sessCnt], linkIds[sessCnt*3+linkCnt], ulStreamId, 
                KMccAudioUplinkStream, *codec ) );
            strmIds.Append( ulStreamId );
            
            // DL-STREAM
            User::LeaveIfError(
                iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId ) );
            User::LeaveIfError( 
                iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId ) );
            sourceIds.Append( dataSourceId );
            sinkIds.Append( dataSinkId );
            
            TUint32 dlStreamId;
            User::LeaveIfError( 
                iInterface.CreateStream( sessIds[sessCnt], linkIds[sessCnt*3+linkCnt], dlStreamId, 
                KMccAudioDownlinkStream, *codec ) );
            strmIds.Append( dlStreamId );
            }
        }
    
    for ( TInt ind = 0; ind < sessIds.Count(); ind++ )
        {
        // FAST SHUTDOWN
        User::LeaveIfError( iInterface.CloseSession( sessIds[ind] ) );
        }
    
    // TBD: REMOVE DATASINK&SOURCE
    CleanupStack::PopAndDestroy( codec );
    CleanupStack::PopAndDestroy( &sinkIds );
    CleanupStack::PopAndDestroy( &sourceIds );
    CleanupStack::PopAndDestroy( &strmIds );
    CleanupStack::PopAndDestroy( &linkIds );
    CleanupStack::PopAndDestroy( &sessIds );
    }
    
    
// ---------------------------------------------------------------------------
// CTestInterface::TestFastShutdownsL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestFastShutdownsL()
    {
    RDebug::Print( _L("CTestInterface::TestFastShutdownsL") );
    
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    
    TUint32 linkId1;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId1,
            iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId1, 0, KMccLinkCreated );
    
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId1 ) );
        
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    // Add data sink and source
    TUint32 endpointId1;
    TUint32 endpointId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, endpointId2 ) );
    
    // Create downlink and stream 1
    TUint32 downLink1, downStream1;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, downLink1, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, downLink1, 0, KMccLinkCreated );
    
    User::LeaveIfError( iInterface.CreateStream( sessionId, downLink1, downStream1,
        KMccAudioDownlinkStream, *codec ) );

    // Create uplink and stream 1
    TUint32 upLink1, upStream1;
    User::LeaveIfError( 
        iInterface.CreateLink( sessionId, KMccLinkGeneral, upLink1, iNetsettings ) );
    
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, endpointId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId2 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, upLink1, upStream1,
                                                 KMccAudioUplinkStream, *codec ) );

    // Create downlink and stream 2
    TUint32 downLink2, downStream2;
    User::LeaveIfError( iInterface.CreateLink( sessionId, KMccLinkGeneral, downLink2, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, downLink2, 0, KMccLinkCreated );
    
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, endpointId2 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, downLink2, downStream2,
                                                 KMccAudioDownlinkStream, *codec ) );

    // Create uplink and stream 2
    TUint32 upLink2, upStream2;
    User::LeaveIfError( iInterface.CreateLink( sessionId, KMccLinkGeneral, upLink2, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, upLink2, 0, KMccLinkCreated );
    
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, endpointId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId2 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, upLink2, upStream2,
                                                 KMccAudioUplinkStream, *codec ) );

    User::LeaveIfError( iInterface.PrepareStream( sessionId, upLink1, upStream1 ) );
    iIfStub.WaitForEvent( sessionId, upLink1, upStream1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, upLink1, upStream1, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, upLink2, upStream2 ) );
    iIfStub.WaitForEvent( sessionId, upLink2, upStream2, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, upLink2, upStream2, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, downLink1, downStream1 ) );
    iIfStub.WaitForEvent( sessionId, downLink1, downStream1, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, downLink1, downStream1, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, downLink2, downStream2 ) );
    iIfStub.WaitForEvent( sessionId, downLink2, downStream2, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, downLink2, downStream2, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, upLink1, upStream1 ) );
    iIfStub.WaitForEvent( sessionId, upLink1, upStream1, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, upLink1, upStream1, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop\n" ) );
    iConsole.Getch();
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    

// ---------------------------------------------------------------------------
// CTestInterface::TestResourceReuseL
// 
// ---------------------------------------------------------------------------
//
void CTestInterface::TestResourceReuseL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    TUint32 ulStreamId;
    TUint32 rtpSinkId;
    TUint32 micSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, rtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId1 ) );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, ulStreamId, 
        KMccAudioUplinkStream, *codec ) );
    
    TUint32 dlStreamId;
    TUint32 speakerSinkId1;
    TUint32 rtpSourceId;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, speakerSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, rtpSourceId ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, dlStreamId, 
        KMccAudioUplinkStream, *codec ) );
        
    TUint32 ulStreamId2;
    TUint32 micSourceId2;
    User::LeaveIfError(
        iInterface.Reuse( sessionId, linkId, ulStreamId, rtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId2 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, ulStreamId2, 
        KMccAudioUplinkStream, *codec ) );
    
    TUint32 dlStreamId2;
    TUint32 speakerSinkId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, speakerSinkId2 ) );
    User::LeaveIfError(
        iInterface.Reuse( sessionId, linkId, dlStreamId, rtpSourceId ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, dlStreamId2, 
        KMccAudioDownlinkStream, *codec ) );
    
    User::LeaveIfError( 
        iInterface.PrepareStream( sessionId, linkId, dlStreamId, rtpSourceId ) );
    iIfStub.WaitForEvent( 
        sessionId, linkId, dlStreamId, rtpSourceId, KMccStreamPrepared );
    
    User::LeaveIfError( 
        iInterface.PrepareStream( sessionId, linkId, dlStreamId2, rtpSourceId ) );
    iIfStub.WaitForEvent( 
        sessionId, linkId, dlStreamId2, rtpSourceId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    

// ---------------------------------------------------------------------------
// CTestInterface::TestMultiStreamPreparingL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestMultiStreamPreparingL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    // CREATE UL-STREAM
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, upStream,
                                                 KMccAudioUplinkStream, *codec ) );

    // CREATE DL-STREAM
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, downStream,
                                                 KMccAudioDownlinkStream, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    
    // CREATE SECOND LINK
    TUint32 linkId2;
    User::LeaveIfError( 
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId2, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId2, 0, KMccLinkCreated );
    
    // CREATE SECOND UL-STREAM
    TUint32 upStream2;
    TUint32 dataSinkId3;
    TUint32 dataSourceId3;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId3 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId3 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId2, upStream2,
                                                 KMccAudioUplinkStream, *codec ) );

    // CREATE SECOND DL-STREAM
    TUint32 downStream2;
    TUint32 dataSinkId4;
    TUint32 dataSourceId4;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId4 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId4 ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId2, downStream2,
                                                 KMccAudioDownlinkStream, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId2, upStream2 ) );
    iIfStub.WaitForEvent( sessionId, linkId2, upStream2, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId2, upStream2, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId2, downStream2 ) );
    iIfStub.WaitForEvent( sessionId, linkId2, downStream2, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId2, downStream2, KMccStreamPrepared );
        
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, upStream ) );
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, downStream ) );
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId2, upStream2 ) );
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId2, downStream2 ) );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId2 ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    
    
// ---------------------------------------------------------------------------
// CTestInterface::TestAudioStreamingL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestAudioStreamingL( const TDesC8& aCodec )
    {
    iNetsettings.iLocalAddress.SetPort( KDefaultRtpPort );
    
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    RDebug::Print( _L("CTestInterface::TestAudioStreaming: %S"), &aCodec );
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    
    TUint32 ulStreamId;
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, aCodec );
    CleanupStack::PushL( codec );
    
    // SET VOIP PRIORITIES FOR THE CODEC
    codec->SetPriority( EMdaPriorityMax );
    codec->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, ulStreamId, 
        KMccAudioUplinkStream, *codec ) );
    
    TUint32 dlStreamId;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    
    codec->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, dlStreamId, 
        KMccAudioDownlinkStream, *codec ) );
    
    // START UPLINK FIRST
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, ulStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.SetRemoteAddress( sessionId, linkId, 
        iNetsettings.iRemoteAddress, iNetsettings.iRemoteRtcpPort ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, dlStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, dlStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, ulStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop 1\n" ) );
    iConsole.Getch();
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, ulStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, dlStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStopped );
    
    // START DOWNLINK FIRST
    /*
    // PREPARING AFTER STOP DOES NOT WORK YET
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, ulStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, dlStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, ulStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, dlStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop 2\n" ) );
    iConsole.Getch();
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, ulStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, dlStreamId ) );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStopped );
    */
    // TBD: REMOVE DATASINK&SOURCE
    iInterface.DeleteStream( sessionId, linkId, ulStreamId );
    iInterface.DeleteStream( sessionId, linkId, dlStreamId );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    

// ---------------------------------------------------------------------------
// CTestInterface::TestMultiStreamPausingL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestMultiStreamPausingL()
    {
    iNetsettings.iLocalAddress.SetPort( KDefaultRtpPort );
    
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    
    codec->SetPriority( EMdaPriorityMax );
    codec->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, upStream, 
                                                 KMccAudioUplinkStream, *codec ) );
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 ) );
    
    codec->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, downStream, 
                                                 KMccAudioDownlinkStream, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
        
    User::LeaveIfError( iInterface.SetRemoteAddress( sessionId, linkId, 
        iNetsettings.iRemoteAddress, iNetsettings.iRemoteRtcpPort ) );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, upStream) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, downStream) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to pause\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.PauseStream( sessionId, linkId, upStream) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPaused );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPaused );
    
    User::LeaveIfError( iInterface.PauseStream( sessionId, linkId, downStream) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPaused );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPaused );
    
    iConsole.Printf( _L( "Press a key to resume\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.ResumeStream( sessionId, linkId, upStream) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamResumed );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamResumed );
    
    User::LeaveIfError( iInterface.ResumeStream( sessionId, linkId, downStream) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamResumed );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamResumed );
    
    iConsole.Printf( _L( "Press a key to stop\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, upStream) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, downStream) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, upStream) );
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, downStream) );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    

// ---------------------------------------------------------------------------
// CTestInterface::TestAmrFecL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestAmrFecL()
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    // Enable forward error correction
    User::LeaveIfError( codec->SetAlgo( EAmrFecUsed ) );
    User::LeaveIfError( codec->SetRedCount( 2 ) );
    
    // CREATE UL-SREAM
    TUint32 upStream;
    TUint32 rtpSinkId;
    TUint32 micSourceId;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, rtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, upStream,
                                                 KMccAudioUplinkStream, *codec ) );
    
    // CREATE DL-STREAM
    TUint32 downStream;
    TUint32 speakerSinkId;
    TUint32 rtpSourceId;
    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, speakerSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, rtpSourceId ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, downStream,
                                                 KMccAudioDownlinkStream, *codec ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.SetRemoteAddress( sessionId, linkId, 
        iNetsettings.iRemoteAddress, iNetsettings.iRemoteRtcpPort ) );
    
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    
    // Try the FEC algorithm
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, upStream ) );   
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, downStream ) );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }

// ---------------------------------------------------------------------------
// CTestInterface::TestRedundancyL
// ---------------------------------------------------------------------------
//
void CTestInterface::TestRedundancyL( const TDesC8& aCodec )
    {
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId,
            iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, aCodec );
    CleanupStack::PushL( codec );
    
    // SET VOIP PRIORITIES FOR THE CODEC
    codec->SetPriority( EMdaPriorityMax );
    codec->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    codec->SetAlgo( EGenRedUsed );
    codec->SetRedundancyPT( 100 );
    
    TInt err = codec->SetRedCount( 2 );
    if ( KErrArgument == err )
        {
        User::LeaveIfError( codec->SetRedCount( 1 ) );
        }
    else
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    TUint32 upStream;
    TUint32 rtpSinkId;
    TUint32 micSourceId;
    User::LeaveIfError( 
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, rtpSinkId ) );
    User::LeaveIfError( 
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId ) );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, upStream,
                                                 KMccAudioUplinkStream, *codec ) );

    TUint32 downStream;
    TUint32 speakerSinkId;
    TUint32 rtpSourceId;
    User::LeaveIfError( 
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, speakerSinkId ) );
    User::LeaveIfError( 
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, rtpSourceId ) );
    
    codec->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, downStream,
                                                 KMccAudioDownlinkStream, *codec ) );

    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.SetRemoteAddress( sessionId, linkId, 
        iNetsettings.iRemoteAddress, iNetsettings.iRemoteRtcpPort ) );
        
    User::LeaveIfError( iInterface.PrepareStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStarted );
    
    User::LeaveIfError( iInterface.StartStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press a key to stop\n" ) );
    iConsole.Getch();
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, upStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, upStream, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.StopStream( sessionId, linkId, downStream ) );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    iIfStub.WaitForEvent( sessionId, linkId, downStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, upStream ) );   
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, downStream ) );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }


// ---------------------------------------------------------------------------
// CTestInterface::TestAudioRoutingL
// TBD: Update to loopback test.
// ---------------------------------------------------------------------------
//
void CTestInterface::TestAudioRoutingL()
    {
    TInt err = iInterface.SetParameter( 6, 0, 0, 0, 
        KMccSpeakerRouting, TPckgBuf<TInt>( KMccAudioToPrivate ) );
    if ( KErrNotFound != err )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    TUint32 audioRoute( 0 );
    TPckgBuf<TInt> audioRoutePak( audioRoute );
    err = iInterface.GetParameter( 6, 0, 0, 0, 
        KMccSpeakerRouting, audioRoutePak );
    if ( KErrNotFound != err )
        {
        User::Leave( KErrTotalLossOfPrecision );
        }
    
    CMccCodecInformation* codec
        = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    CleanupStack::PushL( codec );
    
    TUint32 sessionId;
    User::LeaveIfError( iInterface.CreateSession( sessionId ) );
    TUint32 linkId;
    User::LeaveIfError(
        iInterface.CreateLink( sessionId, KMccLinkGeneral, linkId, iNetsettings ) );
    iIfStub.WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 ) );
    
    TUint32 streamId;
    User::LeaveIfError( iInterface.CreateStream( sessionId, linkId, streamId, 
        KMccAudioUplinkStream, *codec ) );
    
    err = iInterface.SetParameter( sessionId, linkId, streamId, 0,
        KMccSpeakerRouting, TPckgBuf<TInt>( KMccAudioToPrivate ) );
    
    User::LeaveIfError( iInterface.DeleteStream( sessionId, linkId, streamId ) );
    User::LeaveIfError( iInterface.CloseLink( sessionId, linkId ) );
    User::LeaveIfError( iInterface.CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( codec );
    }
    
    
// ---------------------------------------------------------------------------
// CTestInterface::FetchCodecByMimeSubtypeNameL
// ---------------------------------------------------------------------------
//
CMccCodecInformation* CTestInterface::FetchCodecByMimeSubtypeNameL( 
        const CMccInterface& aInterface, const TDesC8& aName ) const
    {
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( aInterface.GetCapabilities( codecArray ) );
    
    CMccCodecInformation* tmp = NULL;
    TInt ind( codecArray.Count() );
    
    while ( ( ind-- ) && ( NULL == tmp ) )
        {
        if ( codecArray[ind]->SdpName().Compare( aName ) == 0 )
            {
            tmp = codecArray[ind]->CloneDetailedL();
            }
        }
    
    if ( NULL == tmp )
        {
        User::Leave( KErrNotFound );
        }
    
    CleanupStack::PopAndDestroy( &codecArray );
    return tmp;
    }
