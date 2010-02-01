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
#include "test_async_features.h"
#include "mccteststubs.h"
#include "mmccinterface.h"
#include "mmcccodecinformation.h"
#include "testdefinitions.hrh"


// ======== MEMBER FUNCTIONS =================================================

// ---------------------------------------------------------------------------
// CTestAsyncFeat::CTestAsyncFeat
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CTestAsyncFeat::CTestAsyncFeat( CConsoleBase& aConsole,
     const TMccNetSettings& aNetsettings, CMccInterface& aIF,
     CMccInterfaceStub& aIFStub )
    : iInterface( aIF ), iConsole( aConsole ), iNetsettings( aNetsettings ),
    iIfStub( aIFStub )
    {
    }

// ---------------------------------------------------------------------------
// CTestAsyncFeat::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CTestAsyncFeat::ConstructL(  )
    {
    }
    
// ---------------------------------------------------------------------------
// CTestAsyncFeat::NewL
// Static constructor.
// ---------------------------------------------------------------------------
//   
CTestAsyncFeat* CTestAsyncFeat::NewL( CConsoleBase& aConsole,
        const TMccNetSettings& aNetsettings, CMccInterface& aIF,
        CMccInterfaceStub& aIFStub )
    {
    CTestAsyncFeat* self 
        = new ( ELeave ) CTestAsyncFeat( aConsole, aNetsettings, aIF, aIFStub );
    CleanupStack::PushL( self );

    self->ConstructL();

    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CTestAsyncFeat::~CTestAsyncFeat
// Destructor.
// ---------------------------------------------------------------------------
//
CTestAsyncFeat::~CTestAsyncFeat()
    {  
    delete iCodec;
    }

// ---------------------------------------------------------------------------
// CTestAsyncFeat::DoRunAsyncFeatureTestsL
// ---------------------------------------------------------------------------
//
void CTestAsyncFeat::DoRunAsyncFeatureTestsL()
    {
    iConsole.Printf( _L("REQUEST MEDIA QUALITY\n") );
    RDebug::Print( _L("Mcc_COM_TEST_START: TestMediaQualityRequestL") );
    //__UHEAP_MARK;
    TestMediaQualityRequestL();
    //__UHEAP_MARKEND;
    RDebug::Print( _L("Mcc_COM_TEST_STOP: TestMediaQualityRequestL") );
    }

// ---------------------------------------------------------------------------
// CTestAsyncFeat::TestMediaQualityRequestL()
// private
// ---------------------------------------------------------------------------
//
void CTestAsyncFeat::TestMediaQualityRequestL()
    {
    RDebug::Print( _L("CTestAsyncFeat::TestMediaQualityRequestL") );
    
    User::LeaveIfError( iInterface.CreateSession( iSessionId ) );
    
    RDebug::Print( _L("CTestAsyncFeat::TestMediaQualityRequestL KMccLinkGeneral") );
    User::LeaveIfError(
        iInterface.CreateLink( iSessionId, KMccLinkGeneral, iLinkId,
            iNetsettings ) );
    
    iIfStub.WaitForEvent( iSessionId, iLinkId, 0, KMccLinkCreated );

//    User::LeaveIfError( iInterface.SetRemoteAddress( iSessionId, iLinkId, iNetsettings.iRemoteAddress ) );
    
    iCodec = FetchCodecByMimeSubtypeNameL( iInterface, KAMRSdpName );
    
    // AUDIO DOWNLINK STREAM CREATION
    RDebug::Print( _L("CTestAsyncFeat::TestMediaQualityRequestL ADD RSC AUDIO DL") );

    User::LeaveIfError(
        iInterface.AddDataSink( KUidMmfAudioOutput, KNullDesC8, iAudioSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KMccRtpSourceUid, KNullDesC8, iRtpSourceId ) );

    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, iStreamId1, 
        KMccAudioDownlinkStream, *iCodec ) );
        
    User::LeaveIfError( iInterface.PrepareStream( iSessionId, iLinkId, iStreamId1 ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId1, KMccStreamPrepared );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId1, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( iSessionId, iLinkId, iStreamId1 ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId1, KMccStreamStarted );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId1, KMccStreamStarted );
    
    // AUDIO UPLINK STREAM CREATION
    RDebug::Print( _L("CTestAsyncFeat::TestMediaQualityRequestL CREATE AUDIO UL") );
    
    User::LeaveIfError(
        iInterface.AddDataSink( KMccRtpSinkUid, KNullDesC8, iRtpSinkId ) );
    User::LeaveIfError(
        iInterface.AddDataSource( KUidMmfAudioInput, KNullDesC8, iAudioSourceId ) );

    User::LeaveIfError( iInterface.CreateStream( iSessionId, iLinkId, iStreamId2, 
        KMccAudioUplinkStream, *iCodec ) );
        
    User::LeaveIfError( iInterface.PrepareStream( iSessionId, iLinkId, iStreamId2 ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId2, KMccStreamPrepared );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId2, KMccStreamPrepared );
    
    User::LeaveIfError( iInterface.StartStream( iSessionId, iLinkId, iStreamId2 ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId2, KMccStreamStarted );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId2, KMccStreamStarted );
    
    iConfig.iJitterLevel = 1;
    iConfig.iPacketLoss = 1;
    iConfig.iReportType = EMccJitterReport;
    iConfig.iReportIntervalType = EMccQualityTimeBased;
    iConfig.iReportInterval = 100;
    iConfig.iSessionId = iSessionId;
    iConfig.iEndpointId = iRtpSourceId;
    
    iConsole.Printf( _L( "Media quality observing: Start\n" ) );
    User::LeaveIfError( iInterface.MediaQualityRequest( iConfig ) );

    iConsole.Printf( _L( "Press a key to stop \n" ) );
    iConsole.Getch();

    User::LeaveIfError( iInterface.CancelMediaQualityRequest( iConfig ) );

    User::LeaveIfError( iInterface.StopStream( iSessionId, iLinkId, iStreamId1 ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId1, KMccStreamStopped );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId1, KMccStreamStopped );

    User::LeaveIfError( iInterface.StopStream( iSessionId, iLinkId, iStreamId2 ) );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId2, KMccStreamStopped );
    iIfStub.WaitForEvent( iSessionId, iLinkId, iStreamId2, KMccStreamStopped );
    
    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, iStreamId1 ) );
    User::LeaveIfError( iInterface.DeleteStream( iSessionId, iLinkId, iStreamId2 ) );
    
    //iInterface.RemoveDataSink( iSessionId, iLinkId1, iStreamId1, iAudioSinkId );
    //iInterface.RemoveDataSource( iSessionId, iLinkId1, iStreamId1, iRtpSourceId );
    
    User::LeaveIfError( iInterface.CloseLink( iSessionId, iLinkId ) );
    User::LeaveIfError( iInterface.CloseSession( iSessionId ) );
    }

// ---------------------------------------------------------------------------
// CTestAsyncFeat::FetchCodecByMimeSubtypeNameL
// Private
// ---------------------------------------------------------------------------
//
CMccCodecInformation* CTestAsyncFeat::FetchCodecByMimeSubtypeNameL( 
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
 

// End of File
