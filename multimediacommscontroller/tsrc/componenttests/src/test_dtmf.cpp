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
#include "test_dtmf.h"
#include "mccteststubs.h"
#include "mmccinterface.h"
#include "mmcccodecinformation.h"
#include "testdefinitions.hrh"

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CTest::CTest
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CTestDtmf::CTestDtmf( CConsoleBase& aConsole, 
        const TMccNetSettings& aNetsettings )
    : iConsole( aConsole ), iNetsettings( aNetsettings )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestDtmf::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CTestDtmf::ConstructL()
    {
    }
    
// ---------------------------------------------------------------------------
// CTestDtmf::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CTestDtmf* CTestDtmf::NewL( CConsoleBase& aConsole, 
        const TMccNetSettings& aNetsettings )
    {
    CTestDtmf* self = new ( ELeave ) CTestDtmf( aConsole, aNetsettings );
    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTestDtmf::~CTestDtmf
// Destructor.
// ---------------------------------------------------------------------------
//
CTestDtmf::~CTestDtmf()
    {
    }

// ---------------------------------------------------------------------------
// CTestDtmf::
// ---------------------------------------------------------------------------
//
void CTestDtmf::DoRunDtmfTestsL()
    {
    iConsole.Printf( _L("\n** DTMF TESTS **\n") );
    iConsole.Printf( _L("Stream creation\n") );
    
    RDebug::Print( _L("Mcc_COM_TEST_START: TestStreamCreationL - INBAND") );
    __UHEAP_MARK;
    TestStreamCreationL( EInbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestStreamCreationL - INBAND") );
    
    RDebug::Print( _L("Mcc_COM_TEST_START: TestStreamCreationL - OUTBAND") );
    __UHEAP_MARK;
    TestStreamCreationL( EOutbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestStreamCreationL - OUTBAND") );        

    iConsole.Printf( _L("Stream ctrl\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestStreamControlL - INBAND") );
    __UHEAP_MARK;
    TestStreamControlL( EInbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestStreamControlL - INBAND") );
    
    RDebug::Print( _L("Mcc_COM_TEST_START: TestStreamControlL - OUTBAND") );
    __UHEAP_MARK;
    TestStreamControlL( EOutbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestStreamControlL - OUTBAND") );
    
    iConsole.Printf( _L("DTMF digit sending - INBAND\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestManualDtmfSendingL - INBAND") );
    __UHEAP_MARK;
    TestManualDtmfSendingL( EInbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestManualDtmfSendingL - INBAND") );

    iConsole.Printf( _L("DTMF digit sending - OUTBAND\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestManualDtmfSendingL - OUTBAND") );
    __UHEAP_MARK;
    TestManualDtmfSendingL( EOutbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestManualDtmfSendingL - OUTBAND") );
    
    iConsole.Printf( _L("DTMF string sending - INBAND\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestAutomaticDtmfSendingL - INBAND") );
    __UHEAP_MARK;
    TestAutomaticDtmfSendingL( EInbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestAutomaticDtmfSendingL - INBAND") );
    
    iConsole.Printf( _L("DTMF STRING sending - OUTBAND\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestAutomaticDtmfSendingL - OUTBAND") );
    __UHEAP_MARK;
    TestAutomaticDtmfSendingL( EOutbandSignal );
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestAutomaticDtmfSendingL - OUTBAND") );
    }

// ---------------------------------------------------------------------------
// CTestDtmf::TestStreamCreationL
// ---------------------------------------------------------------------------
//
void CTestDtmf::TestStreamCreationL( TDtmfSignalType aSigType )
    {
    TMccNetSettings tempSettings = iNetsettings;
    
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );
    
    TUint32 sessionId;
    User::LeaveIfError( interface->CreateSession( sessionId ) );
    
    TUint32 linkId;
    TInt linkType( KMccLinkGeneral );
    User::LeaveIfError( interface->CreateLink( sessionId, linkType, linkId,
        tempSettings ) );
    stub->WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    CMccCodecInformation* cInfo 
        = FetchCodecByMimeSubtypeNameL( *interface, KTxtDtmf );
    CleanupStack::PushL( cInfo );
    
    if ( aSigType == EInbandSignal )
        {
        cInfo->SetCodecMode( EDTMFModeInband );
        }
    else
        {
        cInfo->SetCodecMode( EDTMFModeEvent );
        }
        
    TUint32 streamId( 0 );
    TUint32 endpointId1( 0 ), endpointId2( 0 );
    User::LeaveIfError(
        interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, endpointId1 ) );
    User::LeaveIfError(
        interface->AddDataSource( KMccRtpSourceUid, KNullDesC8, endpointId2 ) );
    
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, streamId,
        KMccDtmfStream, *cInfo ) );
    
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, streamId ) );
    //interface->RemoveDataSink( endpointId1 );
    //interface->RemoveDataSource( endpointId1 );
    User::LeaveIfError( interface->CloseLink( sessionId, linkId ) );
    User::LeaveIfError( interface->CloseSession( sessionId ) );
    
    CleanupStack::PopAndDestroy( cInfo );
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );    
    }


// ---------------------------------------------------------------------------
// CTestDtmf::TestStreamControlL
// ---------------------------------------------------------------------------
//
void CTestDtmf::TestStreamControlL( TDtmfSignalType aSigType )
    {
    TMccNetSettings tempSettings = iNetsettings;
    
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );
    
    TUint32 sessionId;
    User::LeaveIfError( interface->CreateSession( sessionId ) );
    
    TUint32 linkId;
    TInt linkType( KMccLinkGeneral );
    User::LeaveIfError( interface->CreateLink( sessionId, linkType, linkId,
        tempSettings ) );
    stub->WaitForEvent( sessionId, linkId, 0, KMccLinkCreated );
    
    CMccCodecInformation* cInfo 
        = FetchCodecByMimeSubtypeNameL( *interface, KTxtDtmf );
    CleanupStack::PushL( cInfo );
    
    if ( aSigType == EInbandSignal )
        {
        cInfo->SetCodecMode( EDTMFModeInband );
        }
    else
        {
        cInfo->SetCodecMode( EDTMFModeEvent );
        }
    
    TUint32 streamId( 0 );
    TUint32 sinkId( 0 ), sourceId( 0 );
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, sinkId );
    interface->AddDataSource( KMccRtpSourceUid, KNullDesC8, sourceId );
    
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, streamId,
        KMccDtmfStream, *cInfo ) );
    
    // TEST CORRECT CONTROL PATHS
    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, streamId ) );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamPrepared );
   
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, streamId ) );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamStarted );
    
    User::LeaveIfError ( interface->PauseStream( sessionId, linkId, streamId ) );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamPaused );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamPaused );
    
    User::LeaveIfError ( interface->ResumeStream( sessionId, linkId, streamId ) );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamResumed );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamResumed );
    
    User::LeaveIfError ( interface->StopStream( sessionId, linkId, streamId ) );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamStopped );
    stub->WaitForEvent( sessionId, linkId, streamId, KMccStreamStopped );
    
    // TBD: TEST INVALID CONTROL PATHS
    
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, streamId ) );
    User::LeaveIfError( interface->CloseLink( sessionId, linkId ) );
    User::LeaveIfError( interface->CloseSession( sessionId ) );
    
    CleanupStack::PopAndDestroy( cInfo );
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }

// ---------------------------------------------------------------------------
// CTestDtmf::TestManualDtmfSendingL
// ---------------------------------------------------------------------------
//    
void CTestDtmf::TestManualDtmfSendingL( TDtmfSignalType aSigType )
    {
    TMccNetSettings tempSettings = iNetsettings;
    
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );
    
    TUint32 sessionId;
    User::LeaveIfError( interface->CreateSession( sessionId ) );
    
    TUint32 linkId;
    TInt linkType( KMccLinkGeneral );
    User::LeaveIfError( interface->CreateLink( sessionId, 
        linkType, linkId, tempSettings ) );  
    
    CMccCodecInformation* cInfo 
        = FetchCodecByMimeSubtypeNameL( *interface, KAMRSdpName );            
    CleanupStack::PushL( cInfo );
    
    CMccCodecInformation* cInfo2 
        = FetchCodecByMimeSubtypeNameL( *interface, KTelephoneEvent );
    CleanupStack::PushL( cInfo2 );
    
    if ( aSigType == EInbandSignal )
        {
        cInfo2->SetCodecMode( EDTMFModeInband );
        }
    else
        {
        cInfo2->SetCodecMode( EDTMFModeEvent );
        }
        
    TUint32 speakerSinkId( 0 );
    TUint32 rtpSourceId( 0 );
    TUint32 dlStreamId( 0 );
    User::LeaveIfError(
        interface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, speakerSinkId ) );
    User::LeaveIfError(
        interface->AddDataSource( KMccRtpSourceUid, KNullDesC8, rtpSourceId ) );
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, dlStreamId,
        KMccAudioDownlinkStream, *cInfo ) );
        
    TUint32 rtpSinkId( 0 );
    TUint32 micSourceId( 0 );
    TUint32 ulStreamId( 0 );
    User::LeaveIfError(
        interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, rtpSinkId ) );
    User::LeaveIfError( 
        interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId ) );
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, ulStreamId,
        KMccAudioUplinkStream, *cInfo ) );

    TUint32 dtmfStreamId( 0 );
    User::LeaveIfError(
        interface->Reuse( sessionId, linkId, dlStreamId, rtpSourceId ) );
    User::LeaveIfError(
        interface->Reuse( sessionId, linkId, ulStreamId, rtpSinkId ) );
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, 
        dtmfStreamId, KMccDtmfStream, *cInfo2 ) );
    
    User::LeaveIfError( interface->SetRemoteAddress( sessionId, linkId, 
    iNetsettings.iRemoteAddress ) );

    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, dlStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    
    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, ulStreamId ) );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    
    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, dtmfStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamPrepared );
    
/*
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, dlStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, ulStreamId ) );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
*/    
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, dtmfStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamStarted );
    
    TMccDtmfEventData dtmfSignal;
    dtmfSignal.iDtmfEventType = KMccDtmfSigStartTone;
    _LIT8( KDtmfDigit, "0" );
    dtmfSignal.iDtmfString.Copy( KDtmfDigit );
    TMccDtmfEventDataPackage signalPkg1( dtmfSignal );
    
    TMccEvent event;
    event.iSessionId = sessionId;
    event.iLinkId = linkId;
    event.iStreamId = dtmfStreamId;
    event.iEventCategory = KMccEventCategoryDtmf;
    event.iEventType = KMccDtmfControl;
    event.iEventData.Copy( signalPkg1 );
        
    interface->SendMediaSignalL( event );
    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfManualStart );
    
    if ( EOutbandSignal == aSigType )
        {
        stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccActivityEvent );
        }
    
    iConsole.Printf( _L("Press a key to stop digit send\n") );
    iConsole.Getch();
    
    dtmfSignal.iDtmfEventType = KMccDtmfSigStopTone;
    signalPkg1 = dtmfSignal;
    event.iEventData.Copy( signalPkg1 );
    interface->SendMediaSignalL( event );
    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfManualStop );
    
    iConsole.Printf( _L("Press a key to delete streams\n") );
    iConsole.Getch();
    
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, dlStreamId ) );
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, ulStreamId ) );
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, dtmfStreamId ) );
    User::LeaveIfError( interface->CloseLink( sessionId, linkId ) );
    User::LeaveIfError( interface->CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( cInfo2 );
    CleanupStack::PopAndDestroy( cInfo );
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }

// ---------------------------------------------------------------------------
// CTestDtmf::TestAutomaticDtmfSendingL
// ---------------------------------------------------------------------------
//    
void CTestDtmf::TestAutomaticDtmfSendingL( TDtmfSignalType aSigType )
    {
    TMccNetSettings tempSettings = iNetsettings;
    
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );
    
    TUint32 sessionId;
    User::LeaveIfError( interface->CreateSession( sessionId ) );

    TUint32 linkId;
    TInt linkType( KMccLinkGeneral );
    User::LeaveIfError( interface->CreateLink( sessionId, 
        linkType, linkId, tempSettings ) );  


    CMccCodecInformation* cInfo = FetchCodecByMimeSubtypeNameL( *interface, 
                                                                KAMRSdpName );            
    CleanupStack::PushL( cInfo );
    
    CMccCodecInformation* cInfo2 = FetchCodecByMimeSubtypeNameL( *interface, 
                                                                KTelephoneEvent );
    CleanupStack::PushL( cInfo2 );
    
    if ( aSigType == EInbandSignal )
        {
        cInfo2->SetCodecMode( EDTMFModeInband );
        }
    else
        {
        cInfo2->SetCodecMode( EDTMFModeEvent );
        }
        
    TUint32 speakerSinkId( 0 );
    TUint32 rtpSourceId( 0 );
    
    TUint32 dlStreamId( 0 );
    User::LeaveIfError(
        interface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, speakerSinkId ) );
    User::LeaveIfError(
        interface->AddDataSource( KMccRtpSourceUid, KNullDesC8, rtpSourceId ) );
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, dlStreamId,
        KMccAudioDownlinkStream, *cInfo ) );
        
    TUint32 rtpSinkId( 0 );
    TUint32 micSourceId( 0 );
    TUint32 ulStreamId( 0 );
    User::LeaveIfError(
        interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, rtpSinkId ) );
    User::LeaveIfError( 
        interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, micSourceId ) );
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, ulStreamId,
        KMccAudioUplinkStream, *cInfo ) );

    TUint32 dtmfStreamId( 0 );
    User::LeaveIfError(
        interface->Reuse( sessionId, linkId, dlStreamId, rtpSourceId ) );
    User::LeaveIfError(
        interface->Reuse( sessionId, linkId, ulStreamId, rtpSinkId ) );
    User::LeaveIfError( interface->CreateStream( sessionId, linkId, 
        dtmfStreamId, KMccDtmfStream, *cInfo2 ) );
    
    
    User::LeaveIfError( interface->SetRemoteAddress( sessionId, linkId, 
    iNetsettings.iRemoteAddress ) );


    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, dlStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamPrepared );
    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, ulStreamId ) );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamPrepared );
    User::LeaveIfError ( interface->PrepareStream( sessionId, linkId, dtmfStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamPrepared );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamPrepared );
    
/*   
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, dlStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStarted );
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, ulStreamId ) );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStarted );
*/    
    User::LeaveIfError ( interface->StartStream( sessionId, linkId, dtmfStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamStarted );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamStarted );
    
    TMccDtmfEventData dtmfSignal;
    dtmfSignal.iDtmfEventType = KMccDtmfSigSendString;
    dtmfSignal.iDtmfString.Copy( KTxtTestDtmfString1() );
    dtmfSignal.iContinue = ETrue;
    TMccDtmfEventDataPackage signalPkg1( dtmfSignal );

    // BASIC SENDING TEST WITH NORMAL DIGITS
    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = sessionId;
    event.iLinkId = linkId;
    event.iStreamId = dtmfStreamId;
    event.iEventCategory = KMccEventCategoryDtmf;
    event.iEventType = KMccDtmfControl;
    event.iEventData.Copy( signalPkg1 );
    
    interface->SendMediaSignalL( event );
    if ( EOutbandSignal == aSigType )
        {
        // Note: In loopback test activity event comes first due to delayed
        // control event sending.
        stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccActivityEvent );
        }
    
    for ( TInt i = 0; i < KTxtTestDtmfString1().Length(); i++ )
        {
        stub->WaitForEvent( 
            sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStart );
        stub->WaitForEvent( 
            sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStop );
        }
    
    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSendingComplete );
    
    iConsole.Printf( _L("Media signal sent 1, press to continue\n") );
    iConsole.Getch();
    
    // PAUSE CHARACTER HANDLING TEST
    dtmfSignal.iDtmfString.Copy( KTxtTestDtmfStringWithPauseChars() );
    TMccDtmfEventDataPackage signalPkg2( dtmfSignal );
    event.iEventData.Copy( signalPkg2 );
    interface->SendMediaSignalL( event );
    for ( TInt i = 0; i < 3; i++ )
        {
        stub->WaitForEvent( 
            sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStart );
        stub->WaitForEvent( 
            sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStop );
        }

    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSendingComplete );

    iConsole.Printf( _L("Media signal sent 2, press to continue\n") );
    iConsole.Getch();
    
    // STOP CHARACTER HANDLING TEST
    _LIT( KTxtTestDtmfStringWithStopChars, "w12" );
    dtmfSignal.iDtmfString.Copy( KTxtTestDtmfStringWithStopChars() );
    TMccDtmfEventDataPackage signalPkg3( dtmfSignal );
    event.iEventData.Copy( signalPkg3 );
    interface->SendMediaSignalL( event );
    
    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfStopInDtmfString );
    
    dtmfSignal.iDtmfEventType = KMccDtmfSigContinueSending;
    dtmfSignal.iContinue = ETrue;
    signalPkg3 = dtmfSignal;
    event.iEventData.Copy( signalPkg3 );
    interface->SendMediaSignalL( event );
    for ( TInt i = 0; i < 2; i++ )
        {
        stub->WaitForEvent( 
            sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStart );
        stub->WaitForEvent( 
            sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStop );
        }
    
    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSendingComplete );
    iConsole.Printf( _L("Media signal sent 3, press to continue\n") );
    iConsole.Getch();
    
    // CANCEL DTMF STRING SENDING -TEST
    dtmfSignal.iDtmfString.Copy( KTxtTestDtmfString1() );
    dtmfSignal.iDtmfEventType = KMccDtmfSigSendString;
    
    TMccDtmfEventDataPackage signalPkg4( dtmfSignal );
    event.iEventData.Copy( signalPkg4 );
    interface->SendMediaSignalL( event );
    stub->WaitForEvent( 
        sessionId, linkId, dtmfStreamId, KMccDtmfControl, KMccDtmfSequenceStart );
    
    dtmfSignal.iDtmfEventType = KMccDtmfSigCancelSending;
    signalPkg4 = dtmfSignal;
    event.iEventData.Copy( signalPkg4 );
    interface->SendMediaSignalL( event );
    
    iConsole.Printf( _L("Media signal sent 4, press to continue\n") );
    iConsole.Getch();
    
    // TBD: which event should be sent when string sending is cancelled?
    User::LeaveIfError ( interface->StopStream( sessionId, linkId, dlStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStopped );
    stub->WaitForEvent( sessionId, linkId, dlStreamId, KMccStreamStopped );
    
    User::LeaveIfError ( interface->StopStream( sessionId, linkId, ulStreamId ) );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStopped );
    stub->WaitForEvent( sessionId, linkId, ulStreamId, KMccStreamStopped );

    User::LeaveIfError ( interface->StopStream( sessionId, linkId, dtmfStreamId ) );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamStopped );
    stub->WaitForEvent( sessionId, linkId, dtmfStreamId, KMccStreamStopped );
    
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, dlStreamId ) );
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, ulStreamId ) );
    User::LeaveIfError( interface->DeleteStream( sessionId, linkId, dtmfStreamId ) );
    User::LeaveIfError( interface->CloseLink( sessionId, linkId ) );
    User::LeaveIfError( interface->CloseSession( sessionId ) );
    CleanupStack::PopAndDestroy( cInfo2 );
    CleanupStack::PopAndDestroy( cInfo );
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }    

// ---------------------------------------------------------------------------
// CTestDtmf::FetchCodecByMimeSubtypeNameL
// ---------------------------------------------------------------------------
//
CMccCodecInformation* CTestDtmf::FetchCodecByMimeSubtypeNameL( 
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
    
    User::LeaveIfNull( tmp );
    CleanupStack::PopAndDestroy( &codecArray );
    return tmp;
    }
