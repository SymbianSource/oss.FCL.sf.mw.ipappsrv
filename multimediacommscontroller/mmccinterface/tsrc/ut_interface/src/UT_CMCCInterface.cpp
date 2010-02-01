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
#include "UT_CMccInterface.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>

#include "mccunittestmacros.h"

// CONSTANTS
const TUid KImplementMccController  = { KImplUidMccController };
#ifndef __WINSCW__
 
const TInt KTestIapId = 1;

#else

const TInt KTestIapId = 6;

#endif

//  INTERNAL INCLUDES

// CONSTRUCTION
UT_CMccInterface* UT_CMccInterface::NewL()
    {
    UT_CMccInterface* self = UT_CMccInterface::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccInterface* UT_CMccInterface::NewLC()
    {
    UT_CMccInterface* self = new( ELeave ) UT_CMccInterface();
    CleanupStack::PushL( self );

    self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccInterface::~UT_CMccInterface()
    {
    }

// Default constructor
UT_CMccInterface::UT_CMccInterface()
    {
    iNetsettings.iRemoteAddress = KInetAddrLoop;
    iNetsettings.iLocalRtcpPort = 5000;
    iNetsettings.iRemoteRtcpPort = 5001;
    iNetsettings.iIapId = KTestIapId;
    }

// Second phase construct
void UT_CMccInterface::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  INHERITED FUNCTIONS

/**
* Callback function to receive media signalling
* @param aSignal Received media signal
* @return void
*/
void UT_CMccInterface::MccEventReceived( const TMccEvent& /*aEvent*/ )
    {
    }


/**
* Callback function to receive stream started event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaStarted( TUint32 aSessionId,
                            TUint32 aLinkId,     
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamStarted -- Stream Started SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamStarted -- Stream Started LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamStarted -- Stream Started StreamId  = %d\n"), aStreamId );
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream stopped event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaStopped( TUint32 aSessionId,
                            TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamStopped -- Stream Stopped SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamStopped -- Stream Stopped LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamStopped -- Stream Stopped StreamId  = %d\n"), aStreamId );
   
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream paused event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaPaused( TUint32 aSessionId,
                           TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamPaused -- Stream Paused SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamPaused -- Stream Paused LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamPaused -- Stream Paused StreamId  = %d\n"), aStreamId );
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream resumed event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaResumed( TUint32 aSessionId,
                            TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamResumed -- Stream Resumed SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamResumed -- Stream Resumed LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamResumed -- Stream Resumed StreamId  = %d\n"), aStreamId );
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream prepared event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaPrepared( TUint32 aSessionId,
                             TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamPrepared -- Stream Prepared SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamPrepared -- Stream Prepared LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamPrepared -- Stream Prepated StreamId  = %d\n"), aStreamId );
    
  //  CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream inactivity event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaInactive( TUint32 aSessionId,
                             TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamInactive -- Stream Prepared SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamInactive -- Stream Prepared LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamInactive -- Stream Prepated StreamId  = %d\n"), aStreamId );
    
//    CActiveScheduler::Stop();
    }

/**
* Callback function to receive stream activity event.
* @param aSessionId Session ID
* @param aLinkId Link ID
* @param aStreamId Stream ID
*/
void UT_CMccInterface::MccMediaActive( TUint32 aSessionId,
                             TUint32 aLinkId,
                            TUint32 aStreamId,
                            TUint32 /*aSinkSourceId*/ )
    {
    RDebug::Print( _L("\n/********************* CALLBACK FUNCTION UT_CMccInterface *****************************/") );
    RDebug::Print( _L("UT_CMccInterface::StreamActive -- Stream Prepared SessionId = %d"), aSessionId );
    RDebug::Print( _L("UT_CMccInterface::StreamActive -- Stream Prepared LinkId    = %d"), aLinkId );
    RDebug::Print( _L("UT_CMccInterface::StreamActive -- Stream Prepated StreamId  = %d\n"), aStreamId ); 
    
 //   CActiveScheduler::Stop();
    }

/**
* Callback function to receive error event.
* @param aError Error code
* @return None
*/
void UT_CMccInterface::MccCtrlError( TInt /*aError*/ )
    {
    }

/**
* Callback function to receive error event.
* @param aError Error code
* @param aSessionId Session ID
* @param aLinkId Link ID
*/
void UT_CMccInterface::MccCtrlError( TInt /*aError*/,
                           TUint32 /*aSessionId*/,
                           TUint32 /*aLinkId*/,
                            TUint32 /*aStreamId*/,
                            TUint32 /*aSinkSourceId*/ )
    {
    }


// SETUP / TEARDOWN
void UT_CMccInterface::SetupL()
    {
    iInterface = CMccInterface::NewL( *this );
    } 

void UT_CMccInterface::Teardown()
    {
    delete iInterface;
    iInterface = NULL;
    }

// Create interface, session
void UT_CMccInterface::Setup2L(  )
    {
    iInterface = CMccInterface::NewL( *this );
           
    // Create session
    TInt error = iInterface->CreateSession( iSessionId );
   
    if ( ( error != KErrNone ) )
        {
        User::Leave( error );
        }
    } 

void UT_CMccInterface::Teardown2(  )
    {
    if( iInterface )
        {
        // Close session
        iInterface->CloseSession( iSessionId );
        
        delete iInterface;
        iInterface = NULL;
        }
    }


// TEST FUNCTIONS

//Create and close session
void UT_CMccInterface::UT_CMccInterface_CreateSessionL()
    {
    TInt error = iInterface->CreateSession( iSessionId );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( iSessionId >= 0 );
    
    TUint32 sessionId = 0;
    error = iInterface->CreateSession( sessionId );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( sessionId >= 0 );
    
    iInterface->CloseSession( iSessionId );
    iInterface->CloseSession( sessionId );
    }

void UT_CMccInterface::UT_CMccInterface_CreateLinkL(  )
    {
    TInt linkType = KMccLinkGeneral;
    
    //Wrong session number
    TInt error = iInterface->CreateLink( (iSessionId + 1),
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
                                          
                                         
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );                                        
    
    
    //Correct
    error = iInterface->CreateLink( iSessionId,
                                    linkType,
                                    iUpLinkId,
                                    iNetsettings );
                                    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( iUpLinkId >= 0 );
    

    if( error == KErrNone )
        {
        iInterface->CloseLink( iSessionId, iUpLinkId );    
        }
    }


void UT_CMccInterface::UT_CMccInterface_CreateStreamL(  )
    {
    //Create Uplink and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );

    
    
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
    
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
       
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

    //Missing sink
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );
    
    //Create stream with wrong session id
	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
    error = iInterface->CreateStream( ( iSessionId + 1 ), iUpLinkId, streamId, streamType, *codecArray[0] );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
        
    //Correct
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    if ( error == KErrNone )
        {
        iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
        }
    
    CleanupStack::Pop(); // codecarray
    codecArray.ResetAndDestroy();
    codecArray.Close();
        
    iInterface->CloseLink( iSessionId, iUpLinkId );

    }

void UT_CMccInterface::UT_CMccInterface_PrepareStreamL(  )
    {
    //Create Uplink, start and prepare the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    User::LeaveIfError( iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings ) );
    //EUNIT_ASSERT_EQUALS( error, KErrNone )
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
    
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
       
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    
    //Add fmtp string to the codec
    _LIT8( KTxtFmtp, "a=fmtp:mode=30" );
    codecArray[0]->SetFmtpAttrL( KTxtFmtp );
    
    //Correct
	User::LeaveIfError( iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId ) );
	User::LeaveIfError( iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId ) );        
    User::LeaveIfError( iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] ) );
    
    //EUNIT_ASSERT_EQUALS( error, KErrNone )
    //Prepare the stream
    //Correct
    User::LeaveIfError( iInterface->PrepareStream( iSessionId, iUpLinkId, streamId ) );
    
    //EUNIT_ASSERT_EQUALS( error, KErrNone )
    iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
    
    CleanupStack::Pop(); // codecarray
    codecArray.ResetAndDestroy();
    codecArray.Close();
    }

void UT_CMccInterface::UT_CMccInterface_StartStreamL(  )
    {
    //Create Uplink, Start, Pause, Resume and Stop the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
            
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
       
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

    /*
    If the active object has an observer interface, then the correct 
    completion and its status can be verified using a technique that 
    involves calling CActiveScheduler::Start() just after the 
    active object activation, and then CActiveScheduler::Stop() in 
    the observing method.

    The scheduler start call will cause a new event loop to be started, 
    so that the current event loop is halted. In other words, execution 
    in the test case method is halted at the scheduler start call. 
    When the asynchronous request is completed, the observer method will 
    be called in the newer scheduler loop. When CActiveScheduler::Stop() 
    is called in the observing method, the newest loop is terminated, 
    and execution continues in the test case method.
    */
	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );        
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    //Prepare the stream
    //Correct
    error = iInterface->PrepareStream( iSessionId, iUpLinkId, streamId );
//	CActiveScheduler::Start();
         
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    //Start the stream
    error = iInterface->StartStream( iSessionId, iUpLinkId, streamId );
//    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    
    //Pause the stream
    error = iInterface->PauseStream( iSessionId, iUpLinkId, streamId );
//    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( error, KErrNone );
   
    //Resume the stream
    error = iInterface->ResumeStream( iSessionId, iUpLinkId, streamId );
//    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( error, KErrNone );
   
    //Stop the stream
    error = iInterface->StopStream( iSessionId, iUpLinkId, streamId );
//    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
        
    CleanupStack::Pop(); // codecarray
    codecArray.ResetAndDestroy();
    codecArray.Close();
        
    iInterface->CloseLink( iSessionId, iUpLinkId );
 
    }

void UT_CMccInterface::UT_CMccInterface_StartStream2L(  )
    {
    //Create Downlink, Start, Pause, Resume and Stop the stream
    TInt linkType = KMccLinkGeneral;    
    TInt streamType = KMccAudioDownlinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                     linkType,
                                     iDownLinkId,
                                     iNetsettings );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
    
    RPointerArray<CMccCodecInformation> codecArray2;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray2 );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray2 ) );
       
    if( !codecArray2.Count() )
        {
        User::Leave( KErrNotFound );
        }

        //Correct
	iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMccRtpSource, KNullDesC8, endpointId );        
    error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray2[0] );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    // Prepare the stream
    error = iInterface->PrepareStream( iSessionId, iDownLinkId, streamId );
 //   CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( error, KErrNone );

	//Start the stream
	error = iInterface->StartStream( iSessionId, iDownLinkId, streamId );
//	CActiveScheduler::Start();
	EUNIT_ASSERT_EQUALS( error, KErrNone );

	//Pause the stream
	error = iInterface->PauseStream( iSessionId, iDownLinkId, streamId );
//	CActiveScheduler::Start();
    
	EUNIT_ASSERT_EQUALS( error, KErrNone );

	//Resume the stream
	error = iInterface->ResumeStream( iSessionId, iDownLinkId, streamId );
  //  CActiveScheduler::Start();
	EUNIT_ASSERT_EQUALS( error, KErrNone );

	//Stop the stream
	error = iInterface->StopStream( iSessionId, iDownLinkId, streamId );
//	CActiveScheduler::Start();
	EUNIT_ASSERT_EQUALS( error, KErrNone );
   
    iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
        
    CleanupStack::Pop(); // codecarray
    codecArray2.ResetAndDestroy();
    codecArray2.Close();
    iInterface->CloseLink( iSessionId, iDownLinkId );  
    }

void UT_CMccInterface::UT_CMccInterface_StartInactivityTimerL(  )
    {
    //Create Uplink, create, prepare and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );

	EUNIT_ASSERT_EQUALS( error, KErrNone );

    
    TUint32 streamId = 0;
    TUint32 endpointId = 0;    
    
    RPointerArray<CMccCodecInformation> codecArray;
    //CleanupClosePushL( codecArray );
    
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );

    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

    // Create stream with codec info
	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );        
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
        
	EUNIT_ASSERT_EQUALS( error, KErrNone );
    // Prepare the stream
    error = iInterface->PrepareStream( iSessionId, iUpLinkId, streamId );
    
        
   
  //  CActiveScheduler::Start();
            
	EUNIT_ASSERT_EQUALS( error, KErrNone );
    //Start the stream
    error = iInterface->StartStream( iSessionId, iUpLinkId, streamId );
 //   CActiveScheduler::Start();
    
    if( error == KErrNone )
        {
        //Start inactivity timer - Not supported by uplink
        error = iInterface->StartInactivityTimer( iSessionId, iUpLinkId, streamId, 50 );
//        EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
        
        //Stop inactivity timer - Not supported by uplink
        error = iInterface->StopInactivityTimer( iSessionId, iUpLinkId, streamId);
  //      EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
        }

	iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );

    //CleanupStack::Pop(); // codecarray
    //codecArray.ResetAndDestroy();
    //codecArray.Close();
    CleanupStack::PopAndDestroy(); // codecArray
    iInterface->CloseLink( iSessionId, iDownLinkId );
    
    }

void UT_CMccInterface::UT_CMccInterface_StartInactivityTimer2L(  )
    {
    //Create Downlink, create, prepare and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioDownlinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                    linkType,
                                    iDownLinkId,
                                    iNetsettings );
    
	EUNIT_ASSERT_EQUALS( error, KErrNone );
    TUint32 endpointId = 0;
    TUint32 streamId = 0;
    
    RPointerArray<CMccCodecInformation> codecArray2;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray2 );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray2 ) );
       
    if( !codecArray2.Count() )
        {
        User::Leave( KErrNotFound );
        }

    // Create stream with codec info
	iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMccRtpSource, KNullDesC8, endpointId );        
    error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray2[0] );
        
	EUNIT_ASSERT_EQUALS( error, KErrNone );
    // Prepare the stream
    error = iInterface->PrepareStream( iSessionId, iDownLinkId, streamId );
    
    //Wait for prepare stream event
//    CActiveScheduler::Start();
    
	EUNIT_ASSERT_EQUALS( error, KErrNone );
    //Start the stream
    error = iInterface->StartStream( iSessionId, iDownLinkId, streamId );
    
    //Wait for start stream event
//    CActiveScheduler::Start();
        
	EUNIT_ASSERT_EQUALS( error, KErrNone );
    //Start inactivity timer - Not supported by uplink
    error = iInterface->StartInactivityTimer( iSessionId, iDownLinkId, streamId, 50 );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    //Wait for inactivity event
//    CActiveScheduler::Start();
    
    //Start inactivity timer - Not supported by uplink
    error = iInterface->StartInactivityTimer( iSessionId, iDownLinkId, streamId, 50 );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    //Stop inactivity timer - Not supported by uplink
    error = iInterface->StopInactivityTimer( iSessionId, iDownLinkId, streamId );
    EUNIT_ASSERT_EQUALS( error, KErrNone);
            
    iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
    iInterface->CloseLink( iSessionId, iUpLinkId );    
    CleanupStack::Pop(); // codecarray
    codecArray2.ResetAndDestroy();
    codecArray2.Close();
    }

void UT_CMccInterface::UT_CMccInterface_SendRTCPReceiverReportL(  )
    {
   //Create Uplink, create, prepare and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iDownLinkId,
                                         iNetsettings );
                                         
                                        
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray< CMccCodecInformation > codecArray;
        CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
        iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId );
	    iInterface->AddDataSource( KUidMccRtpSource, KNullDesC8, endpointId ); 
        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            // Prepare the stream
            error = iInterface->PrepareStream( iSessionId, iDownLinkId, streamId );
            
                
            
            if( error == KErrNone )
                {
                //Start the stream
                error = iInterface->StartStream( iSessionId, iDownLinkId, streamId, EFalse, ETrue );
                
                
                if( error == KErrNone )
                    {
 
                    //Send rtcp receiver report
                    TMccEvent event( iSessionId, 
                                     iDownLinkId, 
                                     streamId, 
                                     0, 
                                     KMccEventCategoryRtcp, 
                                     KMccRtcpControl, 
                                     KErrNone, 
                                     KNullDesC8 );
                    {   
                    TMccRtcpEventData eventData;
                    eventData.iRtcpPacketType = KRtcpRrPacket;
                    TMccRtcpEventDataPackage eventDataPackage( eventData );
                    event.iEventData.Copy( eventDataPackage );  
                    }
                                
                    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->SendMediaSignalL( event ) );
                    
                    //Send rtcp sender report
                    {   
                    TMccRtcpEventData eventData;
                    eventData.iRtcpPacketType = KRtcpSrPacket;
                    TMccRtcpEventDataPackage eventDataPackage( eventData );
                    event.iEventData.Copy( eventDataPackage );  
                    }
                    
                    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->SendMediaSignalL( event ) );
                    }
                }
            
            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop(); // codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    
    }

void UT_CMccInterface::UT_CMccInterface_SetRemoteAddressL(  )
    {
    TInt linkType = KMccLinkGeneral;
    
    //Correct
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );

    
    if( error == KErrNone )
        {
        //Change address
        error = iInterface->SetRemoteAddress( iSessionId, 
                                              iUpLinkId, 
                                              iNetsettings.iRemoteAddress );
        
        EUNIT_ASSERT_EQUALS( error, KErrNone );
        
        
        error = iInterface->SetRemoteAddress( iSessionId, 
                                              iUpLinkId, 
                                              iNetsettings.iRemoteAddress,
                                              5440 );
        
        EUNIT_ASSERT_EQUALS( error, KErrNone );
        
        
        iInterface->CloseLink( iSessionId, iUpLinkId );    
        }
    }

void UT_CMccInterface::UT_CMccInterface_SetPriorityL(  )
    {
    //Create Downlink, start and prepare the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioDownlinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iDownLinkId,
                                         iNetsettings );
                                         
    
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray<CMccCodecInformation> codecArray;
        CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            // TBD! Test updatedatasource/sink over here
            
            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop();// codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    }

void UT_CMccInterface::UT_CMccInterface_VolumeL(  )
    {
    TInt volume;
    TPckgBuf<TInt> volPak( volume );
    
    TInt error = iInterface->GetParameter( 0, 0, 0, 0, KMccSpeakerVolume, volPak );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( volPak() >= 0 );
    }

void UT_CMccInterface::UT_CMccInterface_MaxVolumeL(  )
    {
    //Create Downlink, start and prepare the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioDownlinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iDownLinkId,
                                         iNetsettings );
                                         
     
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray<CMccCodecInformation> codecArray;
        CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            TInt volume;
            TPckgBuf<TInt> volPak( volume );
    
            iInterface->GetParameter( (iSessionId + 1), iDownLinkId, streamId, 0, KMccSpeakerMaxVolume, volPak );

            //EUNIT_ASSERT_EQUALS( error, KErrNotFound );
           
            iInterface->GetParameter( iSessionId, iDownLinkId, streamId, 0, KMccSpeakerMaxVolume, volPak );
            
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            EUNIT_ASSERT( volPak() >= 0 );

            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop(); // codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    }

void UT_CMccInterface::UT_CMccInterface_SetVolumeL(  )
    {
    TInt volume( 5 );
    TPckgBuf<TInt> volPak( volume );
    
    TInt error = iInterface->SetParameter( 0, 0, 0, 0, KMccSpeakerVolume, volPak );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_BalanceL(  )
    {
    //Create Uplink and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    TMccDevice deviceType = EMccAudioRecord;

    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
                                         
     
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        
        RPointerArray<CMccCodecInformation> codecArray;
        CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            //Get balance for EMccAudioUplink
            TInt balance;
            TPckgBuf<TInt> balancePak( balance );
    
            TInt error = iInterface->GetParameter( (iSessionId + 1), iUpLinkId, streamId, 0, KMccMicBalance, balancePak );
            //EUNIT_ASSERT_EQUALS( error, KErrNotFound );
            
            error = iInterface->GetParameter( iSessionId, iUpLinkId, streamId, 0, KMccMicBalance, balancePak );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            
            // TBD: Not supported at the moment
            // Balance should be somewhere between -100 and 100
            //EUNIT_ASSERT( balancePak() >= -100 );
            //EUNIT_ASSERT( balancePak() <= 100 );
            
            iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
            }
        
        CleanupStack::Pop(); // codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
        
    iInterface->CloseLink( iSessionId, iUpLinkId );
        
        
    //Create Downlink and start the stream
    linkType = KMccLinkGeneral;
    streamType = KMccAudioDownlinkStream;
    deviceType = EMccAudioPlay;
    
    error = iInterface->CreateLink( iSessionId,
                                     linkType,
                                     iDownLinkId,
                                     iNetsettings );
                                     
      
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray<CMccCodecInformation> codecArray;
        CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            //Get balance for EMccAudioDownlink
            
            TInt balance;
            TPckgBuf<TInt> balancePak( balance );
    
            TInt error = iInterface->GetParameter( iSessionId, iDownLinkId, streamId, 0, KMccMicBalance, balancePak );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            // TBD: Not supported at the moment
            // Balance should be somewhere between -100 and 100
            //EUNIT_ASSERT( balancePak() >= -100 );
            //EUNIT_ASSERT( balancePak() <= 100 );
            
/* DTMF NOT SUPPORTED
            //Change linktype to DTMF
            linkType = EMccDTMFLink;
            error = iInterface->Balance(iSessionId, iDownLinkId, streamId, balance, deviceType );
            EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
*/            
            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop(); // codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    
    iInterface->CloseLink( iSessionId, iDownLinkId );
    }

void UT_CMccInterface::UT_CMccInterface_SetBalanceL(  )
    {
    //Create Uplink and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    TMccDevice deviceType = EMccAudioRecord;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
                                         
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;

        RPointerArray< CMccCodecInformation > codecArray;
        CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            //Set balance for EMccAudioUplink
            TInt balance = 5;
            TPckgBuf<TInt> balancePak( balance );
    
            TInt error = iInterface->SetParameter( (iSessionId + 1), iUpLinkId, streamId, 0, KMccMicBalance, balancePak );
            //EUNIT_ASSERT_EQUALS( error, KErrNotFound );
            
            error = iInterface->SetParameter( iSessionId, iUpLinkId, streamId, 0, KMccMicBalance, balancePak );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            
            iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
            }
        
        CleanupStack::Pop();// codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
        
    iInterface->CloseLink( iSessionId, iUpLinkId );
        
        
    //Create Downlink and start the stream
    linkType = KMccLinkGeneral;
    streamType = KMccAudioDownlinkStream;
    deviceType = EMccAudioPlay;
    
    error = iInterface->CreateLink( iSessionId,
                                     linkType,
                                     iDownLinkId,
                                     iNetsettings );
                                     
    
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray< CMccCodecInformation > codecArray;
        CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            //Set balance for EMccAudioDownlink
            TInt balance = 5;
            TPckgBuf< TInt > balancePak( balance );
            TInt error = iInterface->SetParameter( (iSessionId + 1), iDownLinkId, streamId, 0, KMccMicBalance, balancePak );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            
/* DTMF NOT SUPPORTED 
            //Change linktype to DTMF
            linkType = EMccDTMFLink;
            error = iInterface->SetBalance(iSessionId, iDownLinkId, streamId, balance, deviceType );
            EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
*/            
            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop(); // codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    
    iInterface->CloseLink( iSessionId, iDownLinkId );
    }

void UT_CMccInterface::UT_CMccInterface_GetGainL(  )
    {
    TInt gain;
    TPckgBuf<TInt> gainPak( gain );
    
    TInt error = iInterface->GetParameter( 0, 0, 0, 0, KMccMicGain, gainPak );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( gainPak() >= 0 );
    }

void UT_CMccInterface::UT_CMccInterface_MaxGainL(  )
    {
    //Create Downlink, start and prepare the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioDownlinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iDownLinkId,
                                         iNetsettings );
                                         
     
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray<CMccCodecInformation> codecArray;
        CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info
		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        
        if ( error == KErrNone )
            {
            TInt gain;
            TPckgBuf<TInt> gainPak( gain );
            
            TInt error = iInterface->GetParameter( (iSessionId + 1) , iDownLinkId, streamId, 0, KMccMicMaxGain, gainPak );
            //EUNIT_ASSERT_EQUALS( error, KErrNotFound );
            
            error = iInterface->GetParameter( iSessionId , iDownLinkId, streamId, 0, KMccMicMaxGain, gainPak );
            EUNIT_ASSERT_EQUALS( error, KErrNone );
            EUNIT_ASSERT( gainPak() >= 0 );

            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop();// codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    
    
    }

void UT_CMccInterface::UT_CMccInterface_SetGainL(  )
    {
    TInt gain = 5;
    TPckgBuf<TInt> gainPak( gain );
    
    TInt error = iInterface->SetParameter( 0, 0, 0, 0, KMccMicGain, gainPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
 
    }

void UT_CMccInterface::UT_CMccInterface_GetCodecLL(  )
    {
    //Create Downlink and start the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioDownlinkStream;

    TInt error = iInterface->CreateLink( iSessionId,
                                     linkType,
                                     iDownLinkId,
                                     iNetsettings );
                                     
    
    if( error == KErrNone )
        {
        TUint32 streamId = 0;
        TUint32 endpointId = 0;
        
        RPointerArray< CMccCodecInformation > codecArray;
        CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
        User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
           
        if( !codecArray.Count() )
            {
            User::Leave( KErrNotFound );
            }

        // Create stream with codec info

		iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId );
		iInterface->AddDataSource( KUidMccRtpSource, KNullDesC8, endpointId );        

        error = iInterface->CreateStream( iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] );
        //EUNIT_ASSERT_NO_LEAVE( iInterface->SetCodec( iSessionId, iDownLinkId, streamId, *codecArray[0] ) );
        User::LeaveIfError( iInterface->SetCodec( iSessionId, iDownLinkId, streamId, *codecArray[0] ) );
        
        if ( error == KErrNone )
            {
            //Get codec information
            CMccCodecInformation* info = NULL; 
            
            MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(info = iInterface->CodecL( (iSessionId + 1), iDownLinkId, streamId ), KErrNotFound);
            MCC_EUNIT_ASSERT_SPECIFIC_LEAVE(info = iInterface->CodecL( iSessionId, iDownLinkId, streamId ), KErrNotFound);

            iInterface->DeleteStream( iSessionId, iDownLinkId, streamId );
            }
        
        CleanupStack::Pop(); // codecarray
        codecArray.ResetAndDestroy();
        codecArray.Close();
        }
    
    iInterface->CloseLink( iSessionId, iDownLinkId );
    }

void UT_CMccInterface::UT_CMccInterface_SetCodecL(  )
    {
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    TInt streamType2 = KMccAudioDownlinkStream;
    
    // Get codec info from interface
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    
    // Try to set each codec
    TInt count = codecArray.Count();

	/* NOTE, 0 NOT K ALWAYS USED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    for( TInt k = 0; k < count; k++ )
        {
        RDebug::Print( _L("TODO UT_CMccInterface_SetCodecL: %d,"), k );

		
		//h2634 & dtmf
		if ( k != 1  && k != 6 )
		    {
		    TUint32 endpointId( 0 );
            TUint32 streamId( 0 );
            User::LeaveIfError( iInterface->CreateLink( iSessionId, linkType, iUpLinkId, iNetsettings ) );
		
    		iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
    		iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );        
            EUNIT_ASSERT_NO_LEAVE( iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[k] ) );
            EUNIT_ASSERT_NO_LEAVE( iInterface->SetCodec( iSessionId, iUpLinkId, streamId, *codecArray[k] ) );
            
            // Prepare the stream
            User::LeaveIfError( iInterface->PrepareStream( iSessionId, iUpLinkId, streamId ) );
          
            codecArray[0]->SetMaxPTime( 120 );
            EUNIT_ASSERT_NO_LEAVE( iInterface->SetCodec( iSessionId, iUpLinkId, streamId, *codecArray[k] ) );
            EUNIT_ASSERT_NO_LEAVE( iInterface->DeleteStream( iSessionId, iUpLinkId, streamId ) );
            
       
            EUNIT_ASSERT_NO_LEAVE( iInterface->AddDataSink( KUidMmfAudioOutput, KNullDesC8, endpointId ) );
    	    EUNIT_ASSERT_NO_LEAVE( iInterface->AddDataSource( KUidMccRtpSource, KNullDesC8, endpointId ) );        

            EUNIT_ASSERT_NO_LEAVE( iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType2, *codecArray[k] ) );
            EUNIT_ASSERT_NO_LEAVE( iInterface->SetCodec( iSessionId, iUpLinkId, streamId, *codecArray[k] ) );
            
            codecArray[0]->SetMaxPTime( 140 );
            // Prepare the stream
            
            EUNIT_ASSERT_NO_LEAVE( iInterface->PrepareStream( iSessionId, iUpLinkId, streamId ) );
            EUNIT_ASSERT_NO_LEAVE( iInterface->SetCodec( iSessionId, iUpLinkId, streamId, *codecArray[k] ) );
            
            EUNIT_ASSERT_NO_LEAVE( iInterface->DeleteStream( iSessionId, iUpLinkId, streamId ) );
         
            
            EUNIT_ASSERT_NO_LEAVE( iInterface->CloseLink( iSessionId, iUpLinkId ) );
		    }


        }
    CleanupStack::Pop(); // codecArray
    codecArray.ResetAndDestroy();
    codecArray.Close();
    }

void UT_CMccInterface::UT_CMccInterface_SendMediaSignalLL(  )
    {
        TInt linkType = KMccLinkGeneral;    
    TInt streamType = KMccAudioUplinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                     linkType,
                                     iUpLinkId,
                                     iNetsettings );
                                     
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    

    
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
    TUint32 endpointId2 = 0;
    
    RPointerArray<CMccCodecInformation> codecArray2;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray2 );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray2 ) );
       
    if( !codecArray2.Count() )
        {
        User::Leave( KErrNotFound );
        }

        //Correct
	User::LeaveIfError( iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId2 ) );
	User::LeaveIfError( iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId ) );       
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray2[0] );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    // Prepare the stream
    error = iInterface->PrepareStream( iSessionId, iUpLinkId, streamId );
    
        

                    //Start the stream
    error = iInterface->StartStream( iSessionId, iUpLinkId, streamId, EFalse, ETrue );
    

 //   CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( error, KErrNone );

//	CActiveScheduler::Start();
	EUNIT_ASSERT_EQUALS( error, KErrNone );
	
	
    TMccEvent event;
    
    event.iSessionId = iSessionId;
    event.iLinkId = iUpLinkId;
    event.iStreamId = streamId;
    event.iEndpointId = endpointId2;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpControl;

    TBuf8<5> buf8;
    buf8.Format( _L8( "foo42" ) );
        {   
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpAnyPacket;
    eventData.iRtcpPacketData.Copy( buf8 );
    TMccRtcpEventDataPackage eventDataPackage( eventData );
    event.iEventData.Copy( eventDataPackage );  
    }

    
    EUNIT_ASSERT_NO_LEAVE( iInterface->SendMediaSignalL( event ) );
   
    event.iSessionId = 5;
    event.iEventData.Copy( _L8("foofooo") );
    EUNIT_ASSERT_LEAVE( iInterface->SendMediaSignalL( event ) );
    

    // Clean up
    EUNIT_ASSERT_NO_LEAVE( iInterface->DeleteStream( iSessionId, iUpLinkId, streamId ) );
    EUNIT_ASSERT_NO_LEAVE( iInterface->CloseLink( iSessionId, iUpLinkId ) );
    CleanupStack::Pop(); // codecArray
    codecArray2.ResetAndDestroy();
    codecArray2.Close();   
    }

void UT_CMccInterface::UT_CMccInterface_VersionL(  )
    {
    }


void UT_CMccInterface::UT_CMccInterface_TestEventsL(  )
    {
    TInt error = KErrNone;
    TMMFPrioritySettings settings;
    settings.iPriority = EPriorityNormal;
    
    RMMFController controller;
    
    User::LeaveIfError( controller.Open( KImplementMccController, settings ) );
    CleanupClosePushL( controller );

    CMccEvent* mccEvent = CMccEvent::NewL( controller, *this );  
    CleanupStack::PushL( mccEvent );
    
    TMccEvent event;
    event.iEventType = KMccRtcpReceived;
    event.iErrorCode = KErrNone;

    mccEvent->iPackage = event;
    mccEvent->Cancel();
    mccEvent->iStatus = KErrNone;
    mccEvent->RunL();
    
    event.iEventType = KMccStreamError;
    mccEvent->iPackage = event;
    mccEvent->Cancel();
    mccEvent->iStatus = KErrNone;
    mccEvent->RunL();
    
    event.iEventType = KMccStreamStarted;
    mccEvent->iPackage = event;
    mccEvent->Cancel();
    mccEvent->iStatus = KErrNone;
    mccEvent->RunL();
    
    event.iEventType = KMccInactivityEvent;
    mccEvent->iPackage = event;
    mccEvent->Cancel();
    mccEvent->iStatus = KErrNone;
    mccEvent->RunL();
    
    event.iEventType = KMccActivityEvent;
    mccEvent->iPackage = event;
    mccEvent->Cancel();
    mccEvent->iStatus = KErrNone;
    mccEvent->RunL();
    
    mccEvent->Cancel();
    mccEvent->iStatus = KErrNoMemory;
    EUNIT_ASSERT_SPECIFIC_LEAVE( mccEvent->RunL(), KErrNoMemory );
    
    CleanupStack::PopAndDestroy( mccEvent );
    
    CleanupStack::PopAndDestroy(); // controller 
    }
    
    
void UT_CMccInterface::UT_CMccInterface_SendRTCPAnyDataL(  )
	{
    // Get codec info from interface
    RPointerArray< CMccCodecInformation > codecArray;
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

    // Create link and stream
	TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
	
    TUint32 streamId( 0 );
    TUint32 endpointId = 0;
    TUint32 endpointId2 = 0;
    RArray<TUint> dummy;
    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->CreateLink(
                               iSessionId, linkType, iUpLinkId, iNetsettings ) );

    
	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId2 );
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
                               
    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->CreateStream(
                               iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] ) );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->PrepareStream( iSessionId, iUpLinkId, streamId ) );
    //Start the stream
    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->StartStream( iSessionId, iUpLinkId, streamId, EFalse, ETrue ) );
                

    // Run the interesting functions
    
    TBuf8<1> buf8;
    buf8.Format( _L8( "f" ) );
    
    TMccEvent event( iSessionId, 
                        iUpLinkId, 
                        streamId, 
                        endpointId2, 
                        KMccEventCategoryRtcp, 
                        KMccRtcpControl, 
                        KErrNone, 
                        KNullDesC8 );
    {   
    TMccRtcpEventData eventData;
    eventData.iRtcpPacketType = KRtcpAnyPacket;
    eventData.iRtcpPacketData.Copy( buf8 );
    TMccRtcpEventDataPackage eventDataPackage( eventData );
    event.iEventData.Copy( eventDataPackage );  
    }

    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->SendMediaSignalL( event ) );


    // Clean up
    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->DeleteStream( iSessionId, iUpLinkId, streamId ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iInterface->CloseLink( iSessionId, iUpLinkId ) );
    CleanupStack::Pop(); // codecArray
    codecArray.ResetAndDestroy();
    codecArray.Close();
    }

void UT_CMccInterface::UT_CMccInterface_GetSSRCL(  )
	{
	TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;

    // Get codec info from interface
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
    
    TUint32 streamId( 0 );
    TUint32 endpointId = 0;
    
    User::LeaveIfError( iInterface->CreateLink( iSessionId, linkType, iUpLinkId, iNetsettings ) );

    
	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );

    User::LeaveIfError( iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] ) );
    User::LeaveIfError( iInterface->PrepareStream( iSessionId, iUpLinkId, streamId ) );
          
    
    TUint32 ssrc = 0;
    User::LeaveIfError( iInterface->GetSSRC( iSessionId, iUpLinkId, streamId, endpointId, ssrc ) );
    EUNIT_ASSERT( ssrc >= 0 );
    
    User::LeaveIfError( iInterface->DeleteStream( iSessionId, iUpLinkId, streamId ) );
    User::LeaveIfError( iInterface->CloseLink( iSessionId, iUpLinkId ) );
    
    CleanupStack::Pop(); // codecArray
    codecArray.ResetAndDestroy();
    codecArray.Close();
	}

void UT_CMccInterface::UT_CMccInterface_CreateFmtpFromCodecL(  )
	{
    // Get codec info from interface
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

    // Create link and stream
	TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    
    TUint32 streamId( 0 );
    TUint32 endpointId = 0;

    RArray<TUint> dummy;
    EUNIT_ASSERT_NO_LEAVE( iInterface->CreateLink(
                               iSessionId, linkType, iUpLinkId, iNetsettings ) );

    
	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );

    EUNIT_ASSERT_NO_LEAVE( iInterface->CreateStream(
                               iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] ) );

    // Clean up
    EUNIT_ASSERT_NO_LEAVE( iInterface->DeleteStream( iSessionId, iUpLinkId, streamId ) );
    EUNIT_ASSERT_NO_LEAVE( iInterface->CloseLink( iSessionId, iUpLinkId ) );
    CleanupStack::Pop(); // codecArray
    codecArray.ResetAndDestroy();
    codecArray.Close();
    }

void UT_CMccInterface::UT_CMccInterface_TestCameraConfigsL(  )
    {
    TInt count;
    TPckgBuf<TInt> countPak( count );
    TInt error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamCount, countPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT_EQUALS( countPak(), 1 );

    TMccVideoInfo2 info;
    info.iCameraIndex = 0;
    TMccVideoInfoBuf2 infoPak( info );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamInfo, infoPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    TInt para;
    TPckgBuf<TInt> paraPak( para );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamZoomFactor, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT_EQUALS( paraPak(), 5 );
    
    // TBD: brightness not in TCameraInfo
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamBrightness, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT_EQUALS( paraPak(), 5 );

    error = iInterface->GetParameter( 0, 0, 0, 0, 999, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrArgument );

    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamZoomFactor, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    // TBD: brightness not in TCameraInfo
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamBrightness, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, 999, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrArgument );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamDigitZoomFactor, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT_EQUALS( paraPak(), 5 );

    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamContrast, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT_EQUALS( paraPak(), 5 );

    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamWhiteBalance, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );   
    EUNIT_ASSERT_EQUALS( paraPak(), 5 );
 
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamDigitZoomFactor, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamDigitZoomFactor, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamContrast, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamContrast, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamWhiteBalance, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamWhiteBalance, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamExposure, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamExposure, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccSpeakerBalance, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccSpeakerBalance, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
   
    TMccVideoSinkSettingBuf videoSinkSetting;
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccCamLocalVideo, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccCamLocalVideo, paraPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    TMccFileSourceSettingBuf filesourceSetting;
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileInfo, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileInfo, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileName, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileName, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileDuration, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileDuration, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileVideoFrameRate, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileVideoFrameRate, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileVideoBitRate, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileVideoBitRate, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileVideoFrameSize, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileVideoFrameSize, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileAudioBitRate, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound ); 
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileAudioBitRate, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady ); 
       
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFilePosition, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );  
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFilePosition, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );    
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileAudioFourCC, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );    
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileAudioFourCC, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );    
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileVideoFourCC, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );   
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileVideoFourCC, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );    
    
    TPckgBuf<TInt> fastforwardSettingPak;
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccFileFastForward, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );   
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccFileFastForward, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotReady );    
   
    TMccVideoSinkSettingBuf displaySetting;
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccDisplayLocalVideo, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );  
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccDisplayLocalVideo, filesourceSetting );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );  
    
    error = iInterface->SetParameter( 0, 0, 0, 0, 50, displaySetting );
    EUNIT_ASSERT_EQUALS( error, KErrArgument ); 
    error = iInterface->GetParameter( 0, 0, 0, 0, 50, displaySetting );
    EUNIT_ASSERT_EQUALS( error, KErrArgument ); 
    
    error = iInterface->SetParameter( 0, 0, 0, 0, KMccRtpCName, displaySetting );
    EUNIT_ASSERT_EQUALS( error, KErrNone ); 
    error = iInterface->GetParameter( 0, 0, 0, 0, KMccRtpCName, displaySetting );
    EUNIT_ASSERT_EQUALS( error, KErrArgument ); 
    
    }
    
void UT_CMccInterface::UT_CMccInterface_TestFileConfigsL()
    {
    //Create Uplink, Start, Pause, Resume and Stop the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
                                         
                                             
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
            
    RPointerArray< CMccCodecInformation > codecArray;
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
       
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	TFileName fileName;
    fileName = _L("C:\\test.3gp");
    TPckgBuf<TFileName> initParam( fileName );
	
	iInterface->AddDataSource( KUidMccFileSource, initParam, endpointId );        
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    //Prepare the stream
    //Correct
    error = iInterface->PrepareStream( iSessionId, iUpLinkId, streamId );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    
        
    
    //Start the stream
    error = iInterface->StartStream( iSessionId, iUpLinkId, streamId );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    
    TMccFileSourceSetting fileSourceSetting;
    TMccFileSourceSettingBuf fileSettingPak( fileSourceSetting );
    error = iInterface->GetParameter( 0, 0, 0, endpointId, KMccFileName, fileSettingPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    fileSourceSetting = fileSettingPak();
 /*   error = iInterface->GetParameter(0,0,0,endpointId, KMccFileVideoFrameRate, fileSettingPak );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    fileSourceSetting = fileSettingPak();

    error = iInterface->GetParameter(0,0,0,endpointId, KMccFilePosition, fileSettingPak );
    */
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    fileSourceSetting = fileSettingPak();
    
    TMccFileSourceSetting fileSourceSetting2;

    fileSourceSetting2.iDuration = 30;
    fileSourceSetting2.iPosition = 10;
    fileSourceSetting2.iVideoFrameRate = 12.0;
    fileSourceSetting2.iVideoBitRate = 20;
    //fileSourceSetting.iVideoFrameSize = ;
    fileSourceSetting2.iAudioBitRate = 30;
    TMccFileSourceSettingBuf fileSettingPak2( fileSourceSetting2 );

  
    error = iInterface->SetParameter(0,0,0,endpointId, KMccFilePosition, fileSettingPak2 );
//    error = iInterface->SetParameter(0,0,0,endpointId, KMccFileVideoFrameRate, fileSettingPak );

    //fileSourceSetting.iVideoFrameSize
    //Stop the stream
    error = iInterface->StopStream( iSessionId, iUpLinkId, streamId );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
        
    CleanupStack::Pop(); // codecarray
    codecArray.ResetAndDestroy();
    codecArray.Close();
        
    iInterface->CloseLink( iSessionId, iUpLinkId );
    }

void UT_CMccInterface::UT_CMccInterface_TestResourceAddAndRemovalL()
    {
    //Create Uplink, Start, Pause, Resume and Stop the stream
    TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioUplinkStream;
    
    TInt error = iInterface->CreateLink( iSessionId,
                                         linkType,
                                         iUpLinkId,
                                         iNetsettings );
                                         
            
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    TUint32 streamId = 0;
    TUint32 endpointId = 0;
            
    RPointerArray< CMccCodecInformation > codecArray;
    CleanupResetAndDestroy< RPointerArray< CMccCodecInformation > >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
       
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }

	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	TUint32 filesourceEndpointId = 0;
    TFileName fileName;
    fileName = _L("C:\\test.3gp");
    TPckgBuf<TFileName> initParam( fileName );
	
	iInterface->AddDataSource( KUidMccFileSource, initParam, filesourceEndpointId );        
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId, streamType, *codecArray[0] );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    //Prepare the stream
    //Correct
    error = iInterface->PrepareStream( iSessionId, iUpLinkId, streamId );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    
        
    
    //Start the stream
    error = iInterface->StartStream( iSessionId, iUpLinkId, streamId );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    

    //Create second stream (reuse source), Start, Pause, Resume and Stop the stream
   	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	iInterface->Reuse( iSessionId, iUpLinkId, streamId, filesourceEndpointId ); 
	TUint32 streamId2 = 0;       
    error = iInterface->CreateStream( iSessionId, iUpLinkId, streamId2, streamType, *codecArray[0] );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    //Prepare the stream
    //Correct
    error = iInterface->PrepareStream( iSessionId, iUpLinkId, streamId2 );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    
    //Start the stream
    error = iInterface->StartStream( iSessionId, iUpLinkId, streamId2 );
    EUNIT_ASSERT_EQUALS( error, KErrNone );

    
    // Remove filesource
    TUint refCount = 0;
    error = iInterface->ReferenceCount( iSessionId, iUpLinkId, streamId2, filesourceEndpointId, refCount );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    EUNIT_ASSERT_EQUALS( refCount, 2 )
    
    error = iInterface->RemoveDataSource( iSessionId, iUpLinkId, streamId2, filesourceEndpointId );
    EUNIT_ASSERT_EQUALS( error, KErrNone )
    
    error = iInterface->RemoveDataSink( iSessionId, iUpLinkId, streamId2, filesourceEndpointId );
    EUNIT_ASSERT_EQUALS( error, KErrNotFound );
    
    // Delete streams
    iInterface->DeleteStream( iSessionId, iUpLinkId, streamId2 );
    iInterface->DeleteStream( iSessionId, iUpLinkId, streamId );
        
    CleanupStack::Pop(); // codecarray
    codecArray.ResetAndDestroy();
    codecArray.Close();
        
    iInterface->CloseLink( iSessionId, iUpLinkId );
    }
    
void UT_CMccInterface::UT_CMccInterface_TestAudioRoutingL()
    {
    // Test situation where session does not yet exist
    TInt err = iInterface->SetParameter( 6, 0, 0, 0, KMccSpeakerRouting, TPckgBuf<TInt>( KMccAudioToPrivate ) );
    //EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    
    TUint32 audioRoute( 0 );
    TPckgBuf<TInt> audioRoutePak( audioRoute );
    err = iInterface->GetParameter( 6, 0, 0, 0, KMccSpeakerRouting, audioRoutePak );
    //EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    
    // Get codec info from interface
    RPointerArray<CMccCodecInformation> codecArray;
    CleanupResetAndDestroy< RPointerArray<CMccCodecInformation> >::PushL( codecArray );
    User::LeaveIfError( iInterface->GetCapabilities( codecArray ) );
    if( !codecArray.Count() )
        {
        User::Leave( KErrNotFound );
        }
        
    // Create link and stream
	TInt linkType = KMccLinkGeneral;
    TInt streamType = KMccAudioDownlinkStream;
    
    TUint32 streamId( 0 );
    TUint32 endpointId = 0;

    RArray<TUint> dummy;
    EUNIT_ASSERT_NO_LEAVE( iInterface->CreateLink(
                               iSessionId, linkType, iDownLinkId, iNetsettings ) );

	iInterface->AddDataSink( KUidMccRtpSink, KNullDesC8, endpointId );
	iInterface->AddDataSource( KUidMmfAudioInput, KNullDesC8, endpointId );
    EUNIT_ASSERT_NO_LEAVE( iInterface->CreateStream(
                               iSessionId, iDownLinkId, streamId, streamType, *codecArray[0] ) );


    // Test situation where appropriate session, link and stream exist
    err = iInterface->SetParameter( iSessionId, iDownLinkId, streamId, 0, KMccSpeakerRouting, TPckgBuf<TInt>( KMccAudioToPrivate ) );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    err = iInterface->GetParameter( iSessionId, iDownLinkId, streamId, 0, KMccSpeakerRouting, audioRoutePak );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    // Clean up
    EUNIT_ASSERT_NO_LEAVE( iInterface->DeleteStream( iSessionId, iDownLinkId, streamId ) );
    EUNIT_ASSERT_NO_LEAVE( iInterface->CloseLink( iSessionId, iDownLinkId ) );
    CleanupStack::Pop(); // codecArray
    codecArray.ResetAndDestroy();
    codecArray.Close();
    }

void UT_CMccInterface::UT_CMccInterface_AddBundleL()
    {
    TUint32 bundleid = 0;
    TUint32 seddionId = 0;
    TMccStreamBundle bundle;
    EUNIT_ASSERT( iInterface->AddBundle(seddionId, bundle ,bundleid) == KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_RemoveBundle()
    {
    EUNIT_ASSERT( iInterface->RemoveBundle( 0,0 ) == KErrNone );  
    }

void UT_CMccInterface::UT_CMccInterface_GetBundles()
    { 
	RArray<TMccStreamBundle> bundles;
	CleanupClosePushL( bundles );
    EUNIT_ASSERT( iInterface->GetBundles(0, bundles) == KErrNone );  
    EUNIT_ASSERT( bundles.Count() == 0 );
    CleanupStack::PopAndDestroy( );// bundles
    }

void UT_CMccInterface::UT_CMccInterface_GetSupportedBitrates()
    {
    RArray<TUint> bitrates;
    CleanupClosePushL( bitrates );
    EUNIT_ASSERT( iInterface->GetSupportedBitrates(iSessionId, 0, 0, bitrates) == KErrNone );
    CleanupStack::PopAndDestroy(); // bitrates
    }

void UT_CMccInterface::UT_CMccInterface_SendRTCPReceiverReport()
    {
    EUNIT_ASSERT( iInterface->SendRTCPReceiverReport(iSessionId, 0,0) == KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_SendRTCPSenderReport()
    {
    EUNIT_ASSERT( iInterface->SendRTCPSenderReport(iSessionId, 0,0) == KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_SendRTCPAnyData()
    {
    EUNIT_ASSERT( iInterface->SendRTCPAnyData(iSessionId, 0, 0, _L8("anydata")) == KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_EnableViewFinder()
    {
    TMccVideoSinkSettingBuf videoSink;
    EUNIT_ASSERT( iInterface->EnableViewFinder(videoSink) == KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_DisableViewFinder()
    {
    EUNIT_ASSERT( iInterface->DisableViewFinder() == KErrNone );  
    }

void UT_CMccInterface::UT_CMccInterface_GetViewFinderSettings()
    {
    TMccVideoSinkSettingBuf videoSink;

    EUNIT_ASSERT( iInterface->GetViewFinderSettings( 0,0,videoSink ) == KErrNone );   
    }
void UT_CMccInterface::UT_CMccInterface_GetCameraParameter()
    {
    TInt cameraPara;
    
    TInt error = iInterface->GetCameraParameter( 4, cameraPara );
    
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( cameraPara == 5 );
    
    TReal val;
    
    error = iInterface->GetCameraParameter( 4, val );
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }


void UT_CMccInterface::UT_CMccInterface_SetCameraParameter()
    {
    TInt error = iInterface->SetCameraParameter( 4, 4 );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    TReal val = 0.0;
    iInterface->SetCameraParameter( 4, val );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    }

void UT_CMccInterface::UT_CMccInterface_Balance()
    {
    TInt balance;
    EUNIT_ASSERT( iInterface->Balance(iSessionId,0,0, balance, EMccAudioPlay) == KErrNone );   
    EUNIT_ASSERT( iInterface->Balance(iSessionId,0,0, balance, EMccAudioRecord) == KErrNone );   
    }

void UT_CMccInterface::UT_CMccInterface_SetBalance()
    {
    EUNIT_ASSERT( iInterface->SetBalance(iSessionId,0,0, 20, EMccAudioPlay) == KErrNone );   
    EUNIT_ASSERT( iInterface->SetBalance(iSessionId,0,0, 20, EMccAudioRecord) == KErrNone );   
    }

void UT_CMccInterface::UT_CMccInterface_StreamsExists()
    {
    EUNIT_ASSERT_EQUALS(iInterface->StreamsExists( iSessionId,0 ) , KErrNone ); 
    }
void UT_CMccInterface::UT_CMccInterface_UpdateDataSink()
    {
    TMccStreamPckg param;
    EUNIT_ASSERT_EQUALS(iInterface->UpdateDataSink( 0, 0, 0, 0, param ), KErrNotFound);  
    }
void UT_CMccInterface::UT_CMccInterface_UpdateDataSource()
    {
    TMccStreamPckg param;
    EUNIT_ASSERT_EQUALS(iInterface->UpdateDataSource( 0, 0, 0, 0, param ), KErrNotFound);     
    }
    
void UT_CMccInterface::UT_CMccInterface_MediaQualityRequest()
    {
    TMccMediaQualityConfig configs; 
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrArgument );
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 10;
    configs.iSessionId = 1;
 
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrArgument );
    
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrArgument );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrArgument );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 1;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 1;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 1;
    EUNIT_ASSERT( iInterface->MediaQualityRequest( configs ) == KErrNotFound );
    
    }
    
void UT_CMccInterface::UT_CMccInterface_CancelMediaQualityRequest()
    {
    TMccMediaQualityConfig configs; 
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrArgument );
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 10;
    configs.iSessionId = 1;
    
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrNotFound );  
    
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrArgument );
    
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrArgument );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 0;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrArgument );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 10;
    configs.iReportInterval = 1;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 10;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 1;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrNotFound );
    
    configs.iJitterLevel= 0;
    configs.iPacketLoss = 0;
    configs.iReportInterval = 1;
    EUNIT_ASSERT( iInterface->CancelMediaQualityRequest( configs ) == KErrNotFound );
    
    }
    
//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccInterface,
    "CMccInterface test",
    "UNIT" )

EUNIT_TEST(
    "CreateSession - test ",
    "CMccInterface",
    "CreateSession",
    "FUNCTIONALITY",
    SetupL, UT_CMccInterface_CreateSessionL, Teardown )

EUNIT_TEST(
    "CreateLink - test ",
    "CMccInterface",
    "CreateLink",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_CreateLinkL, Teardown2 )

EUNIT_TEST(
    "CreateStream - test ",
    "CMccInterface",
    "CreateStream",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_CreateStreamL, Teardown2 )

EUNIT_TEST(
    "PrepareStream - test ",
    "CMccInterface",
    "PrepareStream",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_PrepareStreamL, Teardown2 )

EUNIT_TEST(
    "StartStream - test ",
    "CMccInterface",
    "StartStream",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_StartStreamL, Teardown2 )

EUNIT_TEST(
    "StartStream2 - test ",
    "CMccInterface",
    "StartStream2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_StartStream2L, Teardown2 )
    
EUNIT_TEST(
    "StartInactivityTimer - test ",
    "CMccInterface",
    "StartInactivityTimer",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_StartInactivityTimerL, Teardown2 )

EUNIT_TEST(
    "StartInactivityTimer2 - test ",
    "CMccInterface",
    "StartInactivityTimer2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_StartInactivityTimer2L, Teardown2 )

EUNIT_TEST(
    "SendRTCPReceiverReport - test ",
    "CMccInterface",
    "SendRTCPReceiverReport",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SendRTCPReceiverReportL, Teardown2 )

EUNIT_TEST(
    "SetRemoteAddress - test ",
    "CMccInterface",
    "SetRemoteAddress",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetRemoteAddressL, Teardown2 )

EUNIT_TEST(
    "SetPriority - test ",
    "CMccInterface",
    "SetPriority",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetPriorityL, Teardown2 )

EUNIT_TEST(
    "Volume - test ",
    "CMccInterface",
    "Volume",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_VolumeL, Teardown2 )

EUNIT_TEST(
    "MaxVolume - test ",
    "CMccInterface",
    "MaxVolume",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_MaxVolumeL, Teardown2 )

EUNIT_TEST(
    "SetVolume - test ",
    "CMccInterface",
    "SetVolume",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetVolumeL, Teardown2 )

EUNIT_TEST(
    "Balance - test ",
    "CMccInterface",
    "Balance",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_BalanceL, Teardown2 )

EUNIT_TEST(
    "SetBalance - test ",
    "CMccInterface",
    "SetBalance",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetBalanceL, Teardown2 )

EUNIT_TEST(
    "GetGain - test ",
    "CMccInterface",
    "GetGain",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetGainL, Teardown2 )

EUNIT_TEST(
    "MaxGain - test ",
    "CMccInterface",
    "MaxGain",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_MaxGainL, Teardown2 )

EUNIT_TEST(
    "SetGain - test ",
    "CMccInterface",
    "SetGain",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetGainL, Teardown2 )

EUNIT_TEST(
    "GetCodecL - test ",
    "CMccInterface",
    "GetCodecL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetCodecLL, Teardown2 )

EUNIT_TEST(
    "SetCodec - test ",
    "CMccInterface",
    "SetCodec",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetCodecL, Teardown2 )

EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccInterface",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SendMediaSignalLL, Teardown2 )

EUNIT_TEST(
    "Version - test ",
    "CMccInterface",
    "Version",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_VersionL, Teardown2 )

EUNIT_TEST(
    "TestEvents - test ",
    "CMccInterface",
    "TestEvents",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_TestEventsL, Teardown2 )

EUNIT_TEST(
    "SendRTCPAnyData - test ",
    "CMccInterface",
    "SendRTCPAnyData",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SendRTCPAnyDataL, Teardown2 )

EUNIT_TEST(
    "GetSSRC - test ",
    "CMccInterface",
    "GetSSRC",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetSSRCL, Teardown2 )
   
EUNIT_TEST(
    "CreateFmtpFromCodec - test ",
    "CMccInterface",
    "CreateFmtpFromCodec",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_CreateFmtpFromCodecL, Teardown2 )
   
EUNIT_TEST(
    "TestCameraConfigsL - test ",
    "CMccInterface",
    "TestCameraConfigsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_TestCameraConfigsL, Teardown2 )
   
EUNIT_TEST(
    "TestAudioRoutingL - test ",
    "CMccInterface",
    "TestAudioRoutingL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_TestAudioRoutingL, Teardown2 )   
    
EUNIT_TEST(
    "TestFileConfigsL - test ",
    "CMccInterface",
    "TestFileConfigsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_TestFileConfigsL, Teardown2 )   
       
EUNIT_TEST(
    "TestResourceAddAndRemovalL - test ",
    "CMccInterface",
    "TestResourceAddAndRemovalL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_TestResourceAddAndRemovalL, Teardown2 )   
    
EUNIT_TEST(
    "AddBundle - test ",
    "CMccInterface",
    "AddBundle",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_AddBundleL, Teardown2 )   
    
EUNIT_TEST(
    "RemoveBundle - test ",
    "CMccInterface",
    "RemoveBundle",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_RemoveBundle, Teardown2 )   
    
EUNIT_TEST(
    "GetBundles - test ",
    "CMccInterface",
    "GetBundles",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetBundles, Teardown2 )   
    
EUNIT_TEST(
    "GetSupportedBitrates - test ",
    "CMccInterface",
    "GetSupportedBitrates",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetSupportedBitrates, Teardown2 )   
    
EUNIT_TEST(
    "SendRTCPReceiverReport - test ",
    "CMccInterface",
    "SendRTCPReceiverReport",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SendRTCPReceiverReport, Teardown2 )   
    
EUNIT_TEST(
    "SendRTCPSenderReport - test ",
    "CMccInterface",
    "SendRTCPSenderReport",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SendRTCPSenderReport, Teardown2 )   
    
EUNIT_TEST(
    "SendRTCPAnyData - test 2",
    "CMccInterface",
    "SendRTCPAnyData",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SendRTCPAnyData, Teardown2 )   
    
EUNIT_TEST(
    "EnableViewFinder - test ",
    "CMccInterface",
    "EnableViewFinder",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_EnableViewFinder, Teardown2 )   
    
EUNIT_TEST(
    "DisableViewFinder - test ",
    "CMccInterface",
    "DisableViewFinder",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_DisableViewFinder, Teardown2 )   
    
EUNIT_TEST(
    "GetViewFinderSettings - test ",
    "CMccInterface",
    "GetViewFinderSettings",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetViewFinderSettings, Teardown2 )   
    
EUNIT_TEST(
    "GetCameraParameter - test ",
    "CMccInterface",
    "GetCameraParameter",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_GetCameraParameter, Teardown2 )   
    
EUNIT_TEST(
    "SetCameraParameter - test ",
    "CMccInterface",
    "SetCameraParameter",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetCameraParameter, Teardown2 )   
    
EUNIT_TEST(
    "SetCameraParameter - test ",
    "CMccInterface",
    "SetCameraParameter",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_Balance, Teardown2 )   
    
EUNIT_ALLOC_TEST(
    "Balance - alloc test ",
    "CMccInterface",
    "Balance",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_Balance, Teardown2 )   

EUNIT_TEST(
    "SetBalance - test ",
    "CMccInterface",
    "SetBalance",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_SetBalance, Teardown2 )   
    
EUNIT_TEST(
    "StreamsExists - test ",
    "CMccInterface",
    "StreamsExists",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_StreamsExists, Teardown2 )   
    
EUNIT_TEST(
    "UpdateDataSink - test ",
    "CMccInterface",
    "UpdateDataSink",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_UpdateDataSink, Teardown2 )   
    
EUNIT_TEST(
    "UpdateDataSource - test ",
    "CMccInterface",
    "UpdateDataSource",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_UpdateDataSource, Teardown2 )   
    
EUNIT_TEST(
    "MediaQualityRequest - test ",
    "CMccInterface",
    "MediaQualityRequest",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_MediaQualityRequest, Teardown2 )   
 
EUNIT_TEST(
    "CancelMediaQualityRequest - test ",
    "CMccInterface",
    "CancelMediaQualityRequest",
    "FUNCTIONALITY",
    Setup2L, UT_CMccInterface_CancelMediaQualityRequest, Teardown2 )   
    
EUNIT_END_TEST_TABLE

//  END OF FILE
