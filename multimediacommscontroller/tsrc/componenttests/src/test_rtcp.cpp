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
#include "test_rtcp.h"
#include "mccteststubs.h"
#include "mmccinterface.h"
#include "mmcccodecinformation.h"
#include "testdefinitions.hrh"

#if (defined __WINSCW__) || (defined __WINS__)
/** Length of APP packet name in bytes. Refer to RFC3550. */
const TUint KAppNameOctets = 4;
#endif

// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CTestRtcp::CTestRtcp
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CTestRtcp::CTestRtcp( CConsoleBase& aConsole,
    const TMccNetSettings& aNetsettings )
    : iConsole( aConsole ), iNetsettings( aNetsettings )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestRtcp::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CTestRtcp::ConstructL()
    {
    }
    
// ---------------------------------------------------------------------------
// CTestRtcp::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CTestRtcp* CTestRtcp::NewL( CConsoleBase& aConsole, 
    const TMccNetSettings& aNetsettings )
    {
    CTestRtcp* self = new ( ELeave ) CTestRtcp( aConsole, aNetsettings );
    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTestRtcp::~CTestRtcp
// Destructor.
// ---------------------------------------------------------------------------
//
CTestRtcp::~CTestRtcp()
    {
    }

// ---------------------------------------------------------------------------
// CTestRtcp::
// ---------------------------------------------------------------------------
//
void CTestRtcp::DoRunRtcpTestsL()
    {
    iConsole.Printf( _L("\n** RTCP tests **\n") );

    iConsole.Printf( _L("Send RTCP RR test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSendRTCPReceiverReportL") );
    __UHEAP_MARK;
//    TestSendRTCPReceiverReportL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSendRTCPReceiverReportL") );
    
    iConsole.Printf( _L("Send RTCP SR test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSendRTCPSenderReportL") );
    __UHEAP_MARK;
    TestSendRTCPSenderReportL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSendRTCPSenderReportL") );
    
    iConsole.Printf( _L("Send RTCP data test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSendRTCPDataL") );
    __UHEAP_MARK;
    TestSendRTCPDataL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSendRTCPDataL") );
    
    iConsole.Printf( _L("Send RTCP Sdes test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSetSdesL") );
    __UHEAP_MARK;
    TestSetSdesL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSetSdesL") );
    
    iConsole.Printf( _L("Send RTCP App test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSendAppL") );
    __UHEAP_MARK;
    TestSendAppL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSendAppL") );
    
    iConsole.Printf( _L("Send RTCP Bye test\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestSendByeL") );
    __UHEAP_MARK;
    TestSendByeL();
    __UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestSendByeL") );
    }

// ---------------------------------------------------------------------------
// CTestRtcp::TestSendRTCPReceiverReportL
// ---------------------------------------------------------------------------
//
void CTestRtcp::TestSendRTCPReceiverReportL()
    {
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );

    // Create session
    TUint32 session;
    User::LeaveIfError( interface->CreateSession( session ) );

    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( interface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    codecArray[0]->SetPriority( EMdaPriorityMax );
        
    // Create link
    TUint32 link;
    TInt linkType = KMccLinkGeneral;
    User::LeaveIfError( interface->CreateLink( session, linkType, link, iNetsettings ) );
    stub->WaitForEvent( session, link, 0, KMccLinkCreated );
    
    User::LeaveIfError( interface->SetRemoteAddress( session, link, iNetsettings.iRemoteAddress ) );
    
    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 );
    interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( interface->CreateStream( session, link, upStream,
                                                 KMccAudioUplinkStream, *codecArray[0] ) );
                                                 
    User::LeaveIfError( interface->PrepareStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    
    // Create downstream
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    interface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 );
    interface->AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( interface->CreateStream( session, link, downStream,
                                                 KMccAudioDownlinkStream, *codecArray[0] ) );
    
    User::LeaveIfError( interface->PrepareStream( session, link, downStream ) );
    stub->WaitForEvent( session, link, downStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, downStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, downStream ) );
    stub->WaitForEvent( session, link, downStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, downStream, KMccStreamStarted );
    
    // Wait for a bit so a few packets can be sent
    iConsole.Printf( _L( "Press any key to send RR\n" ) );
    iConsole.Getch();
    
    // Create the RTCP signal
    TMccRtcpEventData signal;
    signal.iRtcpPacketType = KRtcpRrPacket;
    TPckgBuf<TMccRtcpEventData> rtcpPkg( signal );

    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = session;
    event.iLinkId = link;
    event.iStreamId = downStream;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    event.iEventData.Append( rtcpPkg );
    
    // Send the report
    interface->SendMediaSignalL( event );

    // Clean up
    User::LeaveIfError( interface->StopStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    
    User::LeaveIfError( interface->StopStream( session, link, downStream ) );
    stub->WaitForEvent( session, link, downStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, downStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( interface->DeleteStream( session, link, upStream ) ); 
    User::LeaveIfError( interface->DeleteStream( session, link, downStream ) );   
  
    User::LeaveIfError( interface->CloseLink( session, link ) );
    User::LeaveIfError( interface->CloseSession( session ) );
    
    CleanupStack::Pop(); // Codec array
    codecArray.ResetAndDestroy();
    codecArray.Close();
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }

// ---------------------------------------------------------------------------
// CTestRtcp::TestSendRTCPSenderReportL
// ---------------------------------------------------------------------------
//
void CTestRtcp::TestSendRTCPSenderReportL()
    {
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );

    // Create session
    TUint32 session;
    User::LeaveIfError( interface->CreateSession( session ) );

    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( interface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    codecArray[0]->SetPriority( EMdaPriorityMax );
    
    // Create link
    TUint32 link;
    TInt linkType = KMccLinkGeneral;
    User::LeaveIfError( interface->CreateLink( session, linkType, link, iNetsettings ) );
    stub->WaitForEvent( session, link, 0, KMccLinkCreated );
    
    User::LeaveIfError( interface->SetRemoteAddress( session, link, iNetsettings.iRemoteAddress ) );
    
    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId1;
    TUint32 dataSourceId1;
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId1 );
    interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId1 );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( interface->CreateStream( session, link, upStream,
                                                 KMccAudioUplinkStream, *codecArray[0] ) );
                                                 
                                                 
    User::LeaveIfError( interface->PrepareStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    
    // Create downstream
    TUint32 downStream;
    TUint32 dataSinkId2;
    TUint32 dataSourceId2;
    interface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, dataSinkId2 );
    interface->AddDataSource( KMccRtpSourceUid, KNullDesC8, dataSourceId2 );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioDownlink );
    User::LeaveIfError( interface->CreateStream( session, link, downStream,
                                                 KMccAudioDownlinkStream, *codecArray[0] ) );
    
    User::LeaveIfError( interface->PrepareStream( session, link, downStream ) );
    stub->WaitForEvent( session, link, downStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, downStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, downStream ) );
    stub->WaitForEvent( session, link, downStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, downStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press any key to send SR\n" ) );
    iConsole.Getch();

    // Create the RTCP signal
    TMccRtcpEventData signal;
    signal.iRtcpPacketType = KRtcpSrPacket;
    TPckgBuf<TMccRtcpEventData> rtcpPkg( signal );

    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = session;
    //event.iLinkId = upLink;
    event.iLinkId = link;
    event.iStreamId = upStream;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    event.iEventData.Append( rtcpPkg );

    // Send the report
    interface->SendMediaSignalL( event );
    
    // Clean up
    User::LeaveIfError( interface->StopStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    
    User::LeaveIfError( interface->StopStream( session, link, downStream ) );
    stub->WaitForEvent( session, link, downStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, downStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( interface->DeleteStream( session, link, upStream ) ); 
    User::LeaveIfError( interface->DeleteStream( session, link, downStream ) );   
  
    User::LeaveIfError( interface->CloseLink( session, link ) );
    User::LeaveIfError( interface->CloseSession( session ) );
    
    CleanupStack::Pop(); // Codec array
    codecArray.ResetAndDestroy();
    codecArray.Close();
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }
    
// ---------------------------------------------------------------------------
// CTestRtcp::TestSendRTCPDataL
// ---------------------------------------------------------------------------
//
void CTestRtcp::TestSendRTCPDataL()
    {
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );

    // Create session
    TUint32 session;
    User::LeaveIfError( interface->CreateSession( session ) );

    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( interface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    codecArray[0]->SetPriority( EMdaPriorityMax );

    // Create link
    TUint32 link;
    TInt linkType = KMccLinkGeneral;
    User::LeaveIfError( interface->CreateLink( session, linkType, link, iNetsettings ) );
    stub->WaitForEvent( session, link, 0, KMccLinkCreated );
    
    User::LeaveIfError( interface->SetRemoteAddress( session, link, iNetsettings.iRemoteAddress ) );

    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId;
    TUint32 dataSourceId;    
    
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId );
    interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( interface->CreateStream( session, link, upStream,
                                                 KMccAudioUplinkStream, *codecArray[0] ) );
    
    User::LeaveIfError( interface->PrepareStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press any key to send data\n" ) );
    iConsole.Getch();
    
    TMccRtcpEventData signal;
    signal.iRtcpPacketType = KRtcpAnyPacket;
    
    // Non-RTCP data
    TBuf8<15> data;
    data.Format( _L8( "RTCP data test" ) );
    signal.iRtcpPacketData = data;
    
    TPckgBuf<TMccRtcpEventData> rtcpPkg( signal );

    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = session;
    event.iLinkId = link;
    event.iStreamId = upStream;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    event.iEventData.Append( rtcpPkg );
    
    // Send the signal
    interface->SendMediaSignalL( event );

    // Clean up
    User::LeaveIfError( interface->StopStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( interface->DeleteStream( session, link, upStream ) );   
    User::LeaveIfError( interface->CloseLink( session, link ) );
    User::LeaveIfError( interface->CloseSession( session ) );
    
    CleanupStack::Pop(); // Codec array
    codecArray.ResetAndDestroy();
    codecArray.Close();
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }
      
// ---------------------------------------------------------------------------
// CTestRtcp::TestSetSdesL
// ---------------------------------------------------------------------------
//     
void CTestRtcp::TestSetSdesL()
    {
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );

    // Create session
    TUint32 session;
    User::LeaveIfError( interface->CreateSession( session ) );
    
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( interface->GetCapabilities( codecArray ) );
    codecArray[0]->SetPriority( EMdaPriorityMax );

    // Create link
    TUint32 link;
    TInt linkType = KMccLinkGeneral;
    User::LeaveIfError( interface->CreateLink( session, linkType, link, iNetsettings ) );
    stub->WaitForEvent( session, link, 0, KMccLinkCreated );
    
    User::LeaveIfError( interface->SetRemoteAddress( session, link, iNetsettings.iRemoteAddress ) );

    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId;
    TUint32 dataSourceId;
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId );
    interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( interface->CreateStream( session, link, upStream,
                                                 KMccAudioUplinkStream, *codecArray[0] ) );
    
    User::LeaveIfError( interface->PrepareStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press any key to send Sdes\n" ) );
    iConsole.Getch();    
    
    TMccRtcpEventData signal;
    signal.iRtcpPacketType = KRtcpSdesPacket;
    
    TRtpSdesParams sdes;
    TPtrC8* ptr = &sdes.iCName;
    
    TBuf8<10> cname;
    cname.SetMax();
    cname.FillZ();  
    ptr->Set( cname );
    TPckgBuf<TRtpSdesParams> sdesPackage( sdes );
    
    // Finally copy sdes into beginning of the data
    signal.iRtcpPacketData.Insert( 0, sdesPackage );
    TPckgBuf<TMccRtcpEventData> rtcpPkg( signal );

    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = session;
    event.iLinkId = link;
    event.iStreamId = upStream;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    event.iEventData.Append( rtcpPkg );
    
    // Send the signal
    interface->SendMediaSignalL( event );
    
    // Clean up
    User::LeaveIfError( interface->StopStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( interface->DeleteStream( session, link, upStream ) );
    User::LeaveIfError( interface->CloseLink( session, link ) );
    User::LeaveIfError( interface->CloseSession( session ) );
    
    CleanupStack::Pop(); // Codec array
    codecArray.ResetAndDestroy();
    codecArray.Close();
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }

// ---------------------------------------------------------------------------
// CTestRtcp::TestSendAppL
// ---------------------------------------------------------------------------
//  
void CTestRtcp::TestSendAppL()
    { 
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );

    // Create session
    TUint32 session;
    User::LeaveIfError( interface->CreateSession( session ) );
    
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( interface->GetCapabilities( codecArray ) );
    codecArray[0]->SetPriority( EMdaPriorityMax );

    // Create link
    TUint32 link;
    TInt linkType = KMccLinkGeneral;
    User::LeaveIfError( interface->CreateLink( session, linkType, link, iNetsettings ) );
    stub->WaitForEvent( session, link, 0, KMccLinkCreated );
    
    User::LeaveIfError( interface->SetRemoteAddress( session, link, iNetsettings.iRemoteAddress ) );

    // Create upstream
    TUint32 upStream;
    TUint32 dataSinkId;
    TUint32 dataSourceId;
    
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId );
    interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( interface->CreateStream( session, link, upStream,
                                                 KMccAudioUplinkStream, *codecArray[0] ) );

    User::LeaveIfError( interface->PrepareStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press any key to send App\n" ) );
    iConsole.Getch();
    
    TMccRtcpEventData signal;
    signal.iRtcpPacketType = KRtcpAppPacket;
    
    // Has to be flagged temporarily away because somehow 
    // signal.iRtcpPacketData.Copy( appPkg ) operation
    // makes program to close in HW.
#if (defined __WINSCW__) || (defined __WINS__)
    TRtcpApp appPacket;
    appPacket.iSubType = 200;
    
    TBuf8<KAppNameOctets> tempName;
    for( TUint i=0; i < KAppNameOctets; i++ )
        {
        tempName.Append( appPacket.iName[i] );
        }
            
    TBuf8<KMaxRtcpAppData> tempData;
    for( TUint i=0; i < KMaxRtcpAppData; i++ )
        { 
        tempData.Append( appPacket.iAppData[i] );
        }
    
    TPckgBuf<TRtcpApp> appPkg( appPacket );  
    
    signal.iRtcpPacketData.Copy( appPkg );
#endif    
    TPckgBuf<TMccRtcpEventData> rtcpPkg( signal );

    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = session;
    event.iLinkId = link;
    event.iStreamId = upStream;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    event.iEventData.Append( rtcpPkg );
    
    // Send the signal
    interface->SendMediaSignalL( event );
    
    // Clean up
    User::LeaveIfError( interface->StopStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( interface->DeleteStream( session, link, upStream ) );   
    User::LeaveIfError( interface->CloseLink( session, link ) );
    User::LeaveIfError( interface->CloseSession( session ) );
    
    CleanupStack::Pop(); // Codec array
    codecArray.ResetAndDestroy();
    codecArray.Close();
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }

// ---------------------------------------------------------------------------
// CTestRtcp::TestSendByeL
// ---------------------------------------------------------------------------
//
void CTestRtcp::TestSendByeL()
    {
    CMccInterfaceStub* stub = CMccInterfaceStub::NewL();
    CleanupStack::PushL( stub );
    CMccInterface* interface = CMccInterface::NewL( *stub );
    CleanupStack::PushL( interface );

    // Create session
    TUint32 session;
    User::LeaveIfError( interface->CreateSession( session ) );
    
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( interface->GetCapabilities( codecArray ) );
    codecArray[0]->SetPriority( EMdaPriorityMax );

    // Create link
    TUint32 link;
    TInt linkType = KMccLinkGeneral;
    User::LeaveIfError( interface->CreateLink( session, linkType, link, iNetsettings ) );
    stub->WaitForEvent( session, link, 0, KMccLinkCreated );
    
    User::LeaveIfError( interface->SetRemoteAddress( session, link, iNetsettings.iRemoteAddress ) );

    // Create upstream    
    TUint32 upStream;
    TUint32 dataSinkId;
    TUint32 dataSourceId;
     
    interface->AddDataSink( KMccRtpSinkUid, KNullDesC8, dataSinkId );
    interface->AddDataSource( KUidMmfAudioInput, KNullDesC8, dataSourceId );
    
    codecArray[0]->SetPriorityPreference( KAudioPrefVoipAudioUplink );
    User::LeaveIfError( interface->CreateStream( session, link, upStream,
                                                 KMccAudioUplinkStream, *codecArray[0] ) );
                                                 
    User::LeaveIfError( interface->PrepareStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    stub->WaitForEvent( session, link, upStream, KMccStreamPrepared );
    
    User::LeaveIfError( interface->StartStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    stub->WaitForEvent( session, link, upStream, KMccStreamStarted );
    
    iConsole.Printf( _L( "Press any key to send Bye\n" ) );
    iConsole.Getch();    
    
    TMccRtcpEventData signal;
    signal.iRtcpPacketType = KRtcpByePacket;
    
    TBuf8<10> reason;
    reason.SetMax();
    reason.FillZ();  
    
    signal.iRtcpPacketData = reason;
    TPckgBuf<TMccRtcpEventData> rtcpPkg( signal );

    // Encapsulate the signal in event
    TMccEvent event;
    event.iSessionId = session;
    event.iLinkId = link;
    event.iStreamId = upStream;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;
    event.iEventData.Append( rtcpPkg );
    
    // Send the signal
    interface->SendMediaSignalL( event );
    
    // Clean up
    User::LeaveIfError( interface->StopStream( session, link, upStream ) );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    stub->WaitForEvent( session, link, upStream, KMccStreamStopped );
    
    // TBD: REMOVE DATASINK&SOURCE
    User::LeaveIfError( interface->DeleteStream( session, link, upStream ) );   
    User::LeaveIfError( interface->CloseLink( session, link ) );
    User::LeaveIfError( interface->CloseSession( session ) );
    
    CleanupStack::Pop(); // Codec array
    codecArray.ResetAndDestroy();
    codecArray.Close();
    CleanupStack::PopAndDestroy( interface );
    CleanupStack::PopAndDestroy( stub );
    }
