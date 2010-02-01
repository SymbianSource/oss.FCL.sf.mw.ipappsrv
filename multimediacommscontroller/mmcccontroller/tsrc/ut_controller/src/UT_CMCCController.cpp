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
#include "UT_CMccController.h"
#include "Mmccnetworksettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/CEunitAllocTestCaseDecorator.h>


//  INTERNAL INCLUDES
#include "mccuids.hrh"
#include "MccController.h"
#include "Mccinternalcodecs.h"
#include "MmccEvents.h"

// Controller UID
const TUid KImplementMccController  = { KImplUidMccController };

const TUid KUidMccFileSink		= { 0x102747D7 };
const TUid KUidMccFileSource    = { 0x102747D5 };
const TUid KUidMccRtpSource   	= { 0x1020740B };
const TUid KUidMccRtpSink   	= { 0x1020740C };
const TUid KUidMccVideoSource   = { 0x102747D6 };
const TUid KUidMccVideoSink   	= { 0x102747D8 };


// Interface Implementation UIDs
//const TUid KRtpSourceUID            = { KImplUidRtpDataSource };
//const TUid KRtpSinkUID              = { KImplUidRtpDataSink   };

// CONSTRUCTION
UT_CMccController* UT_CMccController::NewL()
    {
    UT_CMccController* self = UT_CMccController::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMccController* UT_CMccController::NewLC()
    {
    UT_CMccController* self = new( ELeave ) UT_CMccController();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccController::~UT_CMccController()
    {
    }

// Default constructor
UT_CMccController::UT_CMccController(): 
                    iMessageDest( KMccControllerUidInterface ),
                    iAudioPlayDeviceCustomCommands( iController ),
                    iAudioRecordDeviceCustomCommands( iController )
    {
    }

// Second phase construct
void UT_CMccController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccController::SetupL(  )
    {

    TInt error = KErrNone;
    TMMFPrioritySettings settings;
    settings.iPriority = EPriorityNormal;
    
    error = iController.Open( KImplementMccController, settings );
    
    if ( error )
        {
        iController.Close();
        User::Leave( error );
        }
    } 

void UT_CMccController::Teardown(  )
    {
    delete iControllerPtr;
    iController.Close();
    }


//Open controller, create session and link
void UT_CMccController::Setup2L(  )
    {
    TInt error = KErrNone;
    TMMFPrioritySettings settings;
    settings.iPriority = EPriorityNormal;

    error = iController.Open( KImplementMccController, settings );
    if ( error )
        {
        iController.Close();
        User::Leave( error );
        }

    //Create session
    TMccCreateSession pktSession;
    TMccCreateSessionPckg package( pktSession );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccCreateSession,
                                                 package,
                                                 KNullDesC8,
                                                 package );
    iSessionId = package().iSessionID;
    
    //Create link                                               
    if( result == KErrNone )
        {
        TInetAddr localAddr;
        localAddr.SetAddress(KInetAddrAny);
        
        TMccCreateLink pktLink;
        pktLink.iSessionID = iSessionId;
        pktLink.iLinkType = KMccLinkGeneral;
        pktLink.iIapId = 0;
        pktLink.iLocalAddress = localAddr;
        pktLink.iLocalAddress.SetPort( 5000 );

        TMccCreateLinkPckg package2( pktLink );

        result = iController.CustomCommandSync( iMessageDest,
                                                     EMccCreateLink,
                                                     package2,
                                                     KNullDesC8,
                                                     package2 );
        
        iLinkId = package2().iLinkID;
        
        RDebug::Print( _L("Create Link, result = %d"), result );
        }
    } 

//Close link, session and finally controller
void UT_CMccController::Teardown2(  )
    {
    //Close link
    TInt err = KErrNone;
    TMccGenericMessage message;
    message.iSessionID = iSessionId;
    message.iLinkID = iLinkId;
    TMccGenericMsgBuffer package( message );
        
    TInt result = iController.CustomCommandSync( iMessageDest,
                                             EMccCloseLink,
                                             package,
                                             KNullDesC8 );
        
    RDebug::Print( _L("Close Link, result = %d"), result );
        
    //Close session
    TMccSession sessID;
    sessID.iSessionID = iSessionId;
    TMccSessionPckg package1( sessID );

    result = iController.CustomCommandSync( iMessageDest,
                                         EMccCloseSession,
                                         package1,
                                         KNullDesC8 );
        
    RDebug::Print( _L("Close Session, result = %d"), result );
    
    //Close controller 
    iController.Close();
    
    iLinkId    = -1;
    iSessionId = -1;
    }
 


//Test Create and Close session
void UT_CMccController::UT_CMccController_CreateSessionLL(  )
    {
    //Create session
    TMccCreateSession pktSession;
    TMccCreateSessionPckg package( pktSession );

    User::LeaveIfError(
        iController.CustomCommandSync( iMessageDest,
                                       EMccCreateSession,
                                       package,
                                       KNullDesC8,
                                       package ) );
    EUNIT_ASSERT_EQUALS( package().iSessionID, 0 );
    
    //Close session
    TMccSession sessID;
    sessID.iSessionID = package().iSessionID;
    TMccSessionPckg package2( sessID );

    User::LeaveIfError(
        iController.CustomCommandSync( iMessageDest,
                                       EMccCloseSession,
                                       package2,
                                       KNullDesC8 ) );
    }



//Test: Close Session -- Implicit Close Link

void UT_CMccController::UT_CMccController_CloseSessionLL(  )
    {
    //Test1: Create Session / Create Link / Close Session 
    //Ensure links do not need to be closed explicitly, Close Session would 
    //close the link.
    
    // Create Session:
    TMccCreateSession pktSession;
    TMccCreateSessionPckg package( pktSession );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccCreateSession,
                                                 package,
                                                 KNullDesC8,
                                                 package );
    // Create Links:
    if( result == KErrNone )
        {
        TInetAddr localAddr;
        localAddr.SetAddress(KInetAddrAny);
        
        TMccCreateLink pktLink;
        pktLink.iSessionID = package().iSessionID;
        pktLink.iLinkType = KMccLinkGeneral;
        pktLink.iIapId = 0;
        pktLink.iLocalAddress = localAddr;
        pktLink.iLocalAddress.SetPort( 5000 );

        TMccCreateLinkPckg package2( pktLink );

        result = iController.CustomCommandSync( iMessageDest,
                                                     EMccCreateLink,
                                                     package2,
                                                     KNullDesC8,
                                                     package2 );
        EUNIT_ASSERT_EQUALS( result, KErrNone );
        
	// Close Session:
		TMccSession sessID;
		sessID.iSessionID = package().iSessionID;
		TMccSessionPckg package4( sessID );

		result = iController.CustomCommandSync( iMessageDest,
		                                 EMccCloseSession,
		                                 package4,
		                                 KNullDesC8 );

		RDebug::Print( _L("Close Session, result = %d"), result );
		EUNIT_ASSERT_EQUALS( result, KErrNone );
        }
    }


//Test create and close link
void UT_CMccController::UT_CMccController_CreateLinkLL(  )
    {
    //Create session
    TMccCreateSession pktSession;
    TMccCreateSessionPckg package( pktSession );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccCreateSession,
                                                 package,
                                                 KNullDesC8,
                                                 package );
    
    RDebug::Print( _L("Create Session, result = %d"), result );
    
    //Create link                                               
    if( result == KErrNone )
        {
        TInetAddr localAddr;
        localAddr.SetAddress(KInetAddrAny);
        
        TMccCreateLink pktLink;
        pktLink.iSessionID = package().iSessionID;
        pktLink.iLinkType = KMccLinkGeneral;
        pktLink.iIapId = 0;
        pktLink.iLocalAddress = localAddr;
        pktLink.iLocalAddress.SetPort( 5000 );

        TMccCreateLinkPckg package2( pktLink );

        result = iController.CustomCommandSync( iMessageDest,
                                                     EMccCreateLink,
                                                     package2,
                                                     KNullDesC8,
                                                     package2 );
        
        RDebug::Print( _L("Create Link, result = %d"), result );
        EUNIT_ASSERT_EQUALS( result, KErrNone );
        
        //Close link
        TInt err = KErrNone;
        TMccGenericMessage message;
        message.iSessionID = package().iSessionID;
        message.iLinkID = package2().iLinkID;
        TMccGenericMsgBuffer package3( message );

        result = iController.CustomCommandSync( iMessageDest,
                                             EMccCloseLink,
                                             package3,
                                             KNullDesC8 );
        
        RDebug::Print( _L("Close Link, result = %d"), result );
        EUNIT_ASSERT_EQUALS( result, KErrNone );   
        
        //Close session
        TMccSession sessID;
        sessID.iSessionID = package().iSessionID;
        TMccSessionPckg package4( sessID );

        result = iController.CustomCommandSync( iMessageDest,
                                         EMccCloseSession,
                                         package4,
                                         KNullDesC8 );
        
        RDebug::Print( _L("Close Session, result = %d"), result );
        EUNIT_ASSERT_EQUALS( result, KErrNone );                                       
        }
    
    
    }

void UT_CMccController::UT_CMccController_CloseLinkLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }


void UT_CMccController::UT_CMccController_StartStreamLL(  )
    {
    
    UT_CMccController_CreateStreamLL();
    
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = iSessionId;
    params.iLinkID = iLinkId;
    params.iStreamID = iStreamId;

    params.iStreamPaused = EFalse;//aPaused;
    params.iEnableRTCP = EFalse;//aEnableRTCP;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccStartStream,
                                         package,
                                         KNullDesC8 );

    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_StopStreamLL(  )
    {
    UT_CMccController_CreateStreamLL();
    
    TInt err = KErrNone;
    TMccSession sessID;
    sessID.iSessionID = iSessionId;
    sessID.iLinkID = iLinkId;
    sessID.iStreamID = iStreamId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccStopStream,
                                         package,
                                         KNullDesC8 );
    
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_StartInactivityTimerLL(  )
    {
    TMccInactivityTimer inactivityTimer;
    inactivityTimer.iSessionID = iSessionId;
    inactivityTimer.iLinkID = iLinkId;
    inactivityTimer.iStreamID = 1;
    inactivityTimer.iTimeoutTime = 10;
    TMccInactivityTimerPckg package( inactivityTimer );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccInactivityTimerStart,
                                              package,
                                              KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_StopInactivityTimerLL(  )
    {
    TMccGenericMessage genMessage;
    genMessage.iSessionID = iSessionId;
    genMessage.iLinkID = iLinkId;
    genMessage.iStreamID = 1;  
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccInactivityTimerStop,
                                              msgBuffer,
                                              KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_RequestEventNotificationLL(  )
    {
    TMccEvent event;
    TMccEventPackage package( event );
    
    EUNIT_ASSERT_NO_LEAVE( iController.CustomCommandAsync( 
                                        iMessageDest,
                                        ERequestEventNotification,
                                        KNullDesC8,
                                        KNullDesC8,
                                        package,
                                        iStatus          ) 
                         );
    //Check if possible to send an event and after that wait for
    //iStatus to change value, User::WaitForAnyRequest( iStatus );
    //read package. 
    User::WaitForRequest( iStatus );
    
    
    EUNIT_ASSERT(package().iEventType == KMccInactivityEvent )      
    }

void UT_CMccController::UT_CMccController_CreateStreamLL(  )
    {
    TMccGenericMessage genMessage;
    genMessage.iSessionID = iSessionId;
    genMessage.iLinkID = iLinkId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );

   
    /* Stream creation parameters */
    TMccCreateStreamParam message;
    message.iPayloadType = 0;
    TMccCreateStreamParamPckg messagePckg( message );
     
    /* Call the controller */
    TInt err = iController.CustomCommandSync( iMessageDest, EMccCreateStream,
                                         msgBuffer, messagePckg,
                                         msgBuffer );

    EUNIT_ASSERT_EQUALS( err, KErrNotReady );
     
    err = iController.AddDataSink( KUidMccRtpSink, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    TMccSession sessID;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetSinkId,
                                         package,
                                         KNullDesC8,
                                         package );
    EUNIT_ASSERT_EQUALS( err, KErrNone );	
	iSinkEndpointId = package().iEndpointID;
    	
    err = iController.AddDataSource( KUidMccRtpSource, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetSourceId,
                                         package,
                                         KNullDesC8,
                                         package );
	
    EUNIT_ASSERT_EQUALS( err, KErrNone );
	iSourceEndpointId = package().iEndpointID;

    err = iController.CustomCommandSync( iMessageDest, EMccCreateStream,
                                         msgBuffer, messagePckg,
                                         msgBuffer );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    iStreamId = msgBuffer().iStreamID;
    }

void UT_CMccController::UT_CMccController_DeleteStreamLL(  )
    {
    TMccSession sessID;
    sessID.iSessionID = iSessionId;
    sessID.iLinkID = iLinkId;
    sessID.iStreamID = iStreamId;
    TMccSessionPckg package( sessID );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccDeleteStream,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_PrepareStreamLL(  )
    {
    UT_CMccController_CreateStreamLL();
    
    TMccSession sessID;
    sessID.iSessionID = iSessionId;
    sessID.iLinkID = iLinkId;
    sessID.iStreamID = iStreamId;
    TMccSessionPckg package( sessID );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccPrepareStream,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_PauseStreamLL(  )
    {
    UT_CMccController_CreateStreamLL();
    
    TMccStream params;
    params.iSessionID = iSessionId;
    params.iLinkID = iLinkId;
    params.iStreamID = iStreamId;
    params.iEnableRTCP = ETrue;
    TMccStreamPckg package( params );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccPauseStream,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_ResumeStreamLL(  )
    {
    UT_CMccController_CreateStreamLL();
    
    TMccStream params;
    params.iSessionID = iSessionId;
    params.iLinkID = iLinkId;
    params.iStreamID = iStreamId;
    params.iEnableRTCP = EFalse;
    TMccStreamPckg package( params );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccResumeStream,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_GetSupportedCodecsLL(  )
    {
    TMccSession session;
    TMccSessionPckg package( session );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetSupportedCodecs,
                                         package,
                                         KNullDesC8,
                                         package );
 
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_GetCodecLL(  )
    {
    
    TMccGenericMessage msg;
    msg.iSessionID = iSessionId = 0;
    msg.iLinkID = iLinkId = 0;
    msg.iStreamID = 1;
    TMccGenericMsgBuffer msgBuffer( msg );
    
    TMccCodecInfo codecInfo;
    TMccCodecInfoBuffer codecBuffer( codecInfo );
    
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetCodec, msgBuffer,
                                         KNullDesC8, codecBuffer );
    
  
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    msg.iSessionID = iSessionId = 1;
    msg.iLinkID = iLinkId = 0;
    msgBuffer = msg;
    
    err = iController.CustomCommandSync( iMessageDest, EMccGetCodec, msgBuffer,
                                         KNullDesC8, codecBuffer );
    
  
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    }

void UT_CMccController::UT_CMccController_GetFmtpAttrLL(  )
    {
    TMccGenericMessage msg;
    msg.iSessionID = iSessionId;
    msg.iLinkID = iLinkId;
    msg.iStreamID = 1;
    TMccGenericMsgBuffer msgBuffer( msg );
    
    HBufC8* fmtpBuf = HBufC8::NewLC( 100 );
    TPtr8 ptr( fmtpBuf->Des() );  

    TPckgBuf<TUint32> fmtpLength;
    fmtpLength() = 10;
       
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetFmtpAttr, msgBuffer,
                                         fmtpLength, ptr );
        
    CleanupStack::PopAndDestroy( fmtpBuf );
  
    EUNIT_ASSERT_EQUALS( err, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_SetCodecLL(  )
    {
    TInt err = KErrNone;
        
    TMccCodecInfo codecInfo;
    codecInfo.iSessionID = iSessionId;
    codecInfo.iLinkID    = iLinkId;
    codecInfo.iStreamID  = iStreamId;
    codecInfo.iSdpName   = KTelephoneEvent;
    codecInfo.iPayloadType = 98;
    TMccCodecInfoBuffer infoBuffer( codecInfo );
    
    err = iController.CustomCommandSync( iMessageDest,
                                             EMccSetCodec,
                                             infoBuffer,
                                             KNullDesC8,
                                             infoBuffer );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( 98, infoBuffer().iPayloadType);
    
    codecInfo.iPayloadType = 101;
    TMccCodecInfoBuffer infoBuffer2( codecInfo );

    err = iController.CustomCommandSync( iMessageDest,
                                             EMccSetCodec,
                                             infoBuffer2,
                                             KNullDesC8,
                                             infoBuffer );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( 101, infoBuffer().iPayloadType);

    codecInfo.iSdpName   = KPCMUSdpName;
    codecInfo.iPayloadType = 98;
    TMccCodecInfoBuffer infoBuffer3( codecInfo );

    err = iController.CustomCommandSync( iMessageDest,
                                             EMccSetCodec,
                                             infoBuffer3,
                                             KNullDesC8,
                                             infoBuffer );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    EUNIT_ASSERT_EQUALS( 98, infoBuffer().iPayloadType);
    
    }

void UT_CMccController::UT_CMccController_GetSupportedBitratesLL(  )
    {
    TMccGenericMessage message;
    message.iSessionID = iSessionId;
    message.iLinkID = iLinkId;
    message.iStreamID = 1;
    TMccGenericMsgBuffer messagePckg( message );

    TMccBitrates bitrates;
    TMccBitratesPckg bitratePckg( bitrates );

    TInt err = iController.CustomCommandSync( iMessageDest, 
                                              EMccGetSupportedBitrates,
                                              messagePckg, KNullDesC8,
                                              bitratePckg );

    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }
void UT_CMccController::UT_CMccController_GetSSRCL()
    {
    TMccGenericMessage message;
    message.iSessionID = iSessionId;
    message.iLinkID = iLinkId;
    message.iStreamID = 1;
    TMccGenericMsgBuffer messagePckg( message );

    TMccSSRC ssrc;
    TMccSSRCPckg ssrcPckg(ssrc);
    TInt err = iController.CustomCommandSync( iMessageDest, 
                                              EMccGetSSRC,
                                              messagePckg, KNullDesC8,
                                              ssrcPckg );

    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    }

void UT_CMccController::UT_CMccController_SetPriorityLL(  )
    {
    TMccStream stream;
    stream.iSessionID = iSessionId;
    stream.iLinkID = iLinkId;
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetAudioPriority,
                                         package,
                                         KNullDesC8 );

    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_StreamSettingsLL(  )
    {
    TMccStream stream;
    stream.iSessionID = iSessionId;
    stream.iLinkID    = iLinkId;
    stream.iStreamID  = 1;
    
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccStreamSettings,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_SendRTCPReceiverReportLL(  )
    {
    TMccGenericMessage genMessage;
    genMessage.iSessionID = iSessionId;
    genMessage.iLinkID = iLinkId;
    genMessage.iStreamID = 1;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSendRtcpRR,
                                         msgBuffer,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_SendRTCPSenderReportLL(  )
    {
    TMccGenericMessage genMessage;
    genMessage.iSessionID = iSessionId;
    genMessage.iLinkID = iLinkId;
    genMessage.iStreamID = 1;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSendRtcpSR,
                                         msgBuffer,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_SendRTCPDataLL()
    {
    TMccGenericMessage genMessage;
    genMessage.iSessionID = iSessionId;
    genMessage.iLinkID = iLinkId;
    genMessage.iStreamID = 1;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );

    TBuf8<5> data;
    data.Format( _L8( "foo42" ) );
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSendRtcpData,
                                         msgBuffer,
                                         data );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }

void UT_CMccController::UT_CMccController_SetRemoteAddressLL(  )
    {
    TMccAddress addr;
    addr.iSessionID = iSessionId;
    addr.iLinkID = iLinkId;
    TMccAddressPckg package( addr );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetRemoteAddress,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }
    

void UT_CMccController::UT_CMccController_SetRemoteRtcpAddrLL(  )
    {
    TMccAddress addr;
    addr.iSessionID = iSessionId;
    addr.iLinkID = iLinkId;
    TMccAddressPckg package( addr );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetRemoteRtcpAddr,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }    

void UT_CMccController::UT_CMccController_SendMediaSignalLL(  )
    {
    UT_CMccController_CreateStreamLL();
    UT_CMccController_SetCodecLL();
    
    TMccRtcpEventData rtcpData;
    rtcpData.iRtcpPacketData.Copy( _L8("foofoo") );
    rtcpData.iRtcpPacketType = KRtcpAnyPacket;
    rtcpData.iSsrc = 2;
    
    TMccRtcpEventDataPackage rtcpDataPackage( rtcpData );
    
    // Create a media signal and send it
    TMccEvent event( iSessionId, iLinkId, iStreamId, iSinkEndpointId, 
        KMccEventCategoryRtcp, KMccRtcpReceived, KErrNone, rtcpDataPackage );
    TMccEventPackage package( event );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
        EMccSendMediaSignal, package, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    // Test DTMF sending
    TMccCodecInfo codecInfo;
    codecInfo.iSessionID = iSessionId;
    codecInfo.iLinkID    = iLinkId;
    codecInfo.iStreamID  = iStreamId;
    codecInfo.iSdpName.Copy( KTelephoneEvent );
    TMccCodecInfoBuffer infoBuffer( codecInfo );
    
    err = iController.CustomCommandSync( iMessageDest,
        EMccSetCodec, infoBuffer, KNullDesC8, infoBuffer );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    TMccDtmfEventData dtmfData;
    dtmfData.iDtmfEventType = KMccDtmfSigStartTone;
    dtmfData.iDtmfString.Append( '1' );
    TMccDtmfEventDataPackage dtmfDataPackage( dtmfData );
    
    TMccEvent dtmfEvent( iSessionId, iLinkId, iStreamId, iSinkEndpointId,
        KMccEventCategoryDtmf, KMccDtmfControl, KErrNone, dtmfDataPackage );
    TMccEventPackage dtmfPackage( dtmfEvent );
    
    err = iController.CustomCommandSync( iMessageDest,
        EMccSendMediaSignal, dtmfPackage, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    TMccDtmfEventData dtmfData2;
    dtmfData2.iDtmfEventType = KMccDtmfSequenceStart;
    _LIT8( KTxtTestDtmfStringWith100Chars, "1234567890123456782012345678301234567840123456785012345678601234567870123456788012345678901234567100" );
    dtmfData2.iDtmfString.Copy( KTxtTestDtmfStringWith100Chars() );
    TMccDtmfEventDataPackage dtmfDataPackage2( dtmfData2 );
    
    TMccEvent dtmfEvent2( iSessionId, iLinkId, iStreamId, iSinkEndpointId,
        KMccEventCategoryDtmf, KMccDtmfControl, KErrNone, dtmfDataPackage2 );
    TMccEventPackage dtmfPackage2( dtmfEvent2 );
    
    err = iController.CustomCommandSync( iMessageDest,
        EMccSendMediaSignal, dtmfPackage2, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    UT_CMccController_DeleteStreamLL();
    }

void UT_CMccController::UT_CMccController_MapdGetVolumeLL(  )
    {
    TInt volume = -1;
    iAudioPlayDeviceCustomCommands.GetVolume( volume );
    
    EUNIT_ASSERT( volume >= 0 );
    }

void UT_CMccController::UT_CMccController_MapdGetMaxVolumeLL(  )
    {
    TInt volume = -1;
    iAudioPlayDeviceCustomCommands.GetMaxVolume( volume );
    
    EUNIT_ASSERT( volume >= 0 );
    }

void UT_CMccController::UT_CMccController_MapdSetVolumeLL(  )
    {
    TInt volume = 5;
    TInt error;
    
    error = iAudioPlayDeviceCustomCommands.SetVolume( volume );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iAudioPlayDeviceCustomCommands.GetVolume( volume );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    EUNIT_ASSERT( volume == 5 );
    
    error = iAudioPlayDeviceCustomCommands.SetVolume( -1 );
    EUNIT_ASSERT_EQUALS( error, KErrArgument );
    
    TInt maxVolume = 0;
    iAudioPlayDeviceCustomCommands.GetMaxVolume( maxVolume );
    
    error = iAudioPlayDeviceCustomCommands.SetVolume( maxVolume + 1 );
    EUNIT_ASSERT_EQUALS( error, KErrArgument );
    }

void UT_CMccController::UT_CMccController_MapdGetBalanceLL(  )
    {
    // Prepare the controller for a stream settings commands
    TMccStream stream;
    stream.iSessionID = iSessionId;
    stream.iLinkID    = iLinkId;
    stream.iStreamID  = 0;
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccStreamSettings,
                                              package,
                                              KNullDesC8 );

    TInt balance = -1;
    iAudioPlayDeviceCustomCommands.GetBalance( balance );
    
    // Balance should be between -100 and 100
    EUNIT_ASSERT( balance >= -100 );
    EUNIT_ASSERT( balance <= 100 );
    }

void UT_CMccController::UT_CMccController_StreamExistsL()
    {   
    TMccStream stream;
    stream.iSessionID = iSessionId;
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccStreamsExists,
                                         package,
                                         KNullDesC8,
                                         package );
                                         
    EUNIT_ASSERT( err == KErrNotFound );       
    }

void UT_CMccController::UT_CMccController_MapdSetBalanceLL(  )
    {
    // Prepare the controller for a stream settings commands
    TMccStream stream;
    stream.iSessionID = iSessionId;
    stream.iLinkID    = iLinkId;
    stream.iStreamID  = 0;
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccStreamSettings,
                                              package,
                                              KNullDesC8 );

    User::LeaveIfError( err );

    TInt error, test;
    iAudioPlayDeviceCustomCommands.GetBalance( test );
    
    TInt balance = 50;
    error = iAudioPlayDeviceCustomCommands.SetBalance( balance );
    EUNIT_ASSERT_EQUALS( error, KErrNone );

    // Balance is not stored in the stub version of subthreads    
    /*TInt newBalance( 0 );
    error = iAudioPlayDeviceCustomCommands.GetBalance( newBalance );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    RDebug::Print( _L( "UT_CMccController_MapdSetBalanceLL balance: %d, newBalance: %d" ),
                   balance, newBalance );
    EUNIT_ASSERT( newBalance == balance );*/
    
    // Try border values
    error = iAudioPlayDeviceCustomCommands.SetBalance( KMMFBalanceMaxLeft - 1 );
    EUNIT_ASSERT_EQUALS( error, KErrArgument );
    
    error = iAudioPlayDeviceCustomCommands.SetBalance( KMMFBalanceMaxRight + 1 );
    EUNIT_ASSERT_EQUALS( error, KErrArgument );
    
    
    error = iAudioPlayDeviceCustomCommands.SetBalance( KMMFBalanceCenter + 1 );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    
    error = iAudioPlayDeviceCustomCommands.SetBalance( KMMFBalanceCenter - 1 );
    EUNIT_ASSERT_EQUALS( error, KErrNone );
    }

void UT_CMccController::UT_CMccController_MapdSetVolumeRampLL(  )
    {
    EUNIT_ASSERT_NO_LEAVE( iAudioPlayDeviceCustomCommands.SetVolumeRamp( TTimeIntervalMicroSeconds(0)) );
    }

void UT_CMccController::UT_CMccController_MardSetGainLL(  )
    {
    TInt gain = 5;
    iAudioRecordDeviceCustomCommands.SetGain( gain );
    iAudioRecordDeviceCustomCommands.GetGain( gain );
    EUNIT_ASSERT( gain == 5 );
    }

void UT_CMccController::UT_CMccController_MardGetMaxGainLL(  )
    {
    TInt gain;
    iAudioRecordDeviceCustomCommands.GetMaxGain( gain );
    EUNIT_ASSERT( gain >= 0 );
    }

void UT_CMccController::UT_CMccController_MardGetGainLL(  )
    {
    TInt gain;
    iAudioRecordDeviceCustomCommands.GetGain( gain );
    EUNIT_ASSERT( gain >= 0 );
    }

void UT_CMccController::UT_CMccController_MardSetBalanceLL(  )
    {
    // Prepare the controller for a stream settings commands
    TMccStream stream;
    stream.iSessionID = iSessionId;
    stream.iLinkID    = iLinkId;
    stream.iStreamID  = 0;
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccStreamSettings,
                                              package,
                                              KNullDesC8 );

    TInt balance = 5;
    iAudioRecordDeviceCustomCommands.SetBalance( balance );
    TInt newBalance( 0 );
    // Balance is not stored in the stub version of Subthreads
    /*iAudioRecordDeviceCustomCommands.GetBalance( newBalance );
    EUNIT_ASSERT( newBalance == balance );*/
    }

void UT_CMccController::UT_CMccController_MardGetBalanceLL(  )
    {
    // Prepare the controller for a stream settings commands
    TMccStream stream;
    stream.iSessionID = iSessionId;
    stream.iLinkID    = iLinkId;
    stream.iStreamID  = 0;
    TMccStreamPckg package( stream );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccStreamSettings,
                                              package,
                                              KNullDesC8 );

    TInt balance;
    iAudioRecordDeviceCustomCommands.GetBalance( balance );
    EUNIT_ASSERT( balance >= -100 );
    EUNIT_ASSERT( balance <= 100 );
    }
    
void UT_CMccController::UT_CMccController_AddDataSinkL(  )
    {
    TMMFMessageDestination messageDestination;

    TInt err = iController.AddDataSink( KUidMmfAudioOutput, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    err = iController.AddDataSink( KMccRtpSinkUid, KNullDesC8, messageDestination );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    err = iController.RemoveDataSink( messageDestination );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    }

void UT_CMccController::UT_CMccController_AddDataSourceL(  )
    {
    TMMFMessageDestination messageDestination;
    
    TInt err = iController.AddDataSource( KUidMmfAudioInput, KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    err = iController.AddDataSource( KMccRtpSourceUid, KNullDesC8, messageDestination );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    err = iController.RemoveDataSource( messageDestination );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    }

void UT_CMccController::UT_CMccController_EnableViewFinderL()
    {
    TMccVideoSinkSetting localsettings;
    TMccVideoSinkSettingBuf localbuf(localsettings);
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccEnableViewFinder,
                                              localbuf,
                                              KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    }
    
void UT_CMccController::UT_CMccController_DisableViewFinderL()
    {
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccDisableViewFinder,
                                         KNullDesC8,
                                         KNullDesC8 ); 
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
    }
  
void UT_CMccController::UT_CMccController_GetViewFinderSettingsL()
    {
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    
    TMccVideoSinkSetting videosetting;
    TMccVideoSinkSettingBuf videosettingbuf(videosetting);
   
    TMccGenericMessage message;
    message.iParam2 = 2;
    TMccGenericMsgBuffer package( message );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetViewFinderSettings,
                                              package,
                                              videosettingbuf,
                                              videosettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );   

    }

void UT_CMccController::UT_CMccController_GetCamInfoL()
    {
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    TMccGenericMessage msg;
    msg.iParam2 = 1;
    TMccGenericMsgBuffer msgBuffer( msg );
    
    TCameraInfo camInfo;
    TPckgBuf<TCameraInfo> camBuffer( camInfo );
    
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetCamInfo, msgBuffer,
                                         KNullDesC8, camBuffer );
    
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
    }


void UT_CMccController::UT_CMccController_GetNumOfCamsL()
    {  
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    
    TMccGenericMessage msg;
    TMccGenericMsgBuffer msgBuffer( msg );
    TPckgBuf<TInt> buffer;    
        
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetNumOfCams, msgBuffer,
                                         KNullDesC8, buffer );
    
    EUNIT_ASSERT_EQUALS( err, KErrNone );   

    }
    
void UT_CMccController::UT_CMccController_GetCamValue1L()
    {
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    
    TMccGenericMessage msg;
    msg.iMessageFunction = 0;
    TMccGenericMsgBuffer package( msg );
        
    TInt err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    
    EUNIT_ASSERT_EQUALS( err, KErrNone );   

    msg.iMessageFunction = 1;
    package = msg;
    err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
        
    msg.iMessageFunction = 2;
    package = msg;
    err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
    
    msg.iMessageFunction = 3;
    package = msg;
    err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
    
    msg.iMessageFunction = 4;
    package = msg;
    err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
    
    msg.iMessageFunction = 5;
    package = msg;
    err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
    }
void UT_CMccController::UT_CMccController_SetCamValue1L()
    {
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    
    TMccGenericMessage message;
    message.iMessageFunction = 0;
    message.iParam4 = 0;
    TMccGenericMsgBuffer package( message );
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);    
    EUNIT_ASSERT_EQUALS( err, KErrNone );   
       
    message.iMessageFunction = 1;
    message.iParam4 = 1;  
    package =  message;    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);    
    EUNIT_ASSERT_EQUALS( err, KErrNone );    
    
    message.iMessageFunction = 2;
    message.iParam4 = 2;  
    package =  message;    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);    
    EUNIT_ASSERT_EQUALS( err, KErrNone );  
    
    message.iMessageFunction = 3;
    message.iParam4 = 3;  
    package =  message;    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);    
    EUNIT_ASSERT_EQUALS( err, KErrNone );    
    
    message.iMessageFunction = 4;
    message.iParam4 = 4;  
    package =  message;    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);    
    EUNIT_ASSERT_EQUALS( err, KErrNone );    
    
    message.iMessageFunction = 5;
    message.iParam4 = 5;  
    package =  message;    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);    
    EUNIT_ASSERT_EQUALS( err, KErrNone );                                  
    }
void UT_CMccController::UT_CMccController_GetCamValue2L()
    {    
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    
    TMccGenericMessage message;
    message.iMessageFunction = 0;
    TMccGenericMsgBuffer package( message );
        
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetCamValue2,
                                         package,
                                         KNullDesC8,
                                         package );
                                         
    EUNIT_ASSERT_EQUALS( err, KErrNotSupported );                                  

    }
void UT_CMccController::UT_CMccController_SetCamValue2L()
    { 
    // Testing emulator implementation of camera hanlder 
    // (dummy impl, no need to assert any return values)
    //
    
    TMccGenericMessage message;
    message.iMessageFunction = 0;
    message.iParam3 = 0;
    TMccGenericMsgBuffer package( message );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue2,
                                         package,
                                         KNullDesC8 );
                 
    EUNIT_ASSERT_EQUALS( err, KErrNotSupported );                                  
    
    }
void UT_CMccController::UT_CMccController_SetAudioRouteL()
    {

    TMccGenericMessage params;
    params.iSessionID = 0;
    params.iLinkID = 0;
    params.iStreamID = 0;
    params.iParam2 = 0;
    TMccGenericMsgBuffer package( params );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetAudioRoute,
                                         package,
                                         KNullDesC8 );
                                
    EUNIT_ASSERT_EQUALS( err, KErrNone );                                  

    }
void UT_CMccController::UT_CMccController_GetAudioRouteL()
    {
    TMccGenericMessage params;
    params.iSessionID = 0;
    params.iLinkID = 0;
    params.iStreamID = 0;
    TMccGenericMsgBuffer package( params );
        
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetAudioRoute,
                                              package,
                                              KNullDesC8,
                                              package );
                                         
    EUNIT_ASSERT_EQUALS( err, KErrNone );                                  
    }
void UT_CMccController::UT_CMccController_SetEndpointParameterL()
    {
    // Test situations where endpoint is not found
    TMccGenericMessage message;
    message.iParam2 = KMccFileInfo;
    message.iEndpointID = 0;
    TMccGenericMsgBuffer package( message );                           
    TMccFileSourceSettingBuf fileSettingbuf;
                  
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccSetEndpointParameter,
                                              package,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
    
    message.iParam2 = KMccFileFastForward;
    package = message;                                    
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccSetEndpointParameter,
                                              package,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
    
    message.iParam2 = KMccDisplayLocalVideo;
    package = message;                                 
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccSetEndpointParameter,
                                              package,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
       
    // Will add source and sink to resource pool
    UT_CMccController_CreateStreamLL();
    
    message.iParam2 = KMccFileInfo;
    message.iEndpointID = iSourceEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    
    ///////
    
    message.iParam2 = KMccFileName;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileDuration;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileVideoFrameRate;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileVideoBitRate;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileVideoFrameSize;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileAudioBitRate;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFilePosition;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileAudioFourCC;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    message.iParam2 = KMccFileVideoFourCC;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    message.iParam2 = KMccRtpCName;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         _L8("foofighter") );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    ///////
    
    message.iParam2 = KMccFileInfo;
    message.iEndpointID = iSinkEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
   
    message.iParam2 = KMccFileFastForward;
    message.iEndpointID = iSourceEndpointId;
    package = message;
    TPckgBuf<TInt> fastforwardSettingPak( 0 );
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fastforwardSettingPak );
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    message.iEndpointID = iSinkEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         fastforwardSettingPak );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
   
   
    message.iParam2 = KMccDisplayLocalVideo;
    message.iEndpointID = iSinkEndpointId;
    package = message;
    TMccVideoSinkSettingBuf videoSettingbuf;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         videoSettingbuf );
    
    EUNIT_ASSERT_EQUALS( err, KErrNone );
    
    message.iEndpointID = iSourceEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         videoSettingbuf );
    
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    
    message.iParam2 = 1000;
    message.iEndpointID = iSinkEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetEndpointParameter,
                                         package,
                                         videoSettingbuf );
                                         
    EUNIT_ASSERT_EQUALS( err, KErrArgument );    
                                                                                                                                        
    }
void UT_CMccController::UT_CMccController_GetEndpointParameterL()
    {
    TMccGenericMessage message;
    message.iParam2 = 0;
    message.iEndpointID = 0;
    TMccGenericMsgBuffer package( message );
    TMccFileSourceSettingBuf fileSettingbuf;
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrArgument ); 
    
    message.iParam2 = KMccFileInfo;
    message.iEndpointID = 0;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    
    
    message.iParam2 = KMccFileName;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNotReady ); 
    
    
    message.iParam2 = KMccFileFastForward;
    message.iEndpointID = 0;
    package = message;
    TPckgBuf<TInt> fastforwardSettingPak;

    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fastforwardSettingPak,
                                              fastforwardSettingPak );
    EUNIT_ASSERT_EQUALS( err, KErrNotReady ); 
    message.iParam2 = KMccDisplayLocalVideo;
    message.iEndpointID = 0;
    package = message;
    TMccVideoSinkSettingBuf displaySetting;

    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              displaySetting,
                                              displaySetting );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
    
     // Will add source and sink to resource pool
    UT_CMccController_CreateStreamLL();
    
    message.iParam2 = KMccFileInfo;
    message.iEndpointID = iSourceEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 

		//////
		
    message.iParam2 = KMccFileName;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileDuration;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileVideoFrameRate;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileVideoBitRate;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileVideoFrameSize;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileAudioBitRate;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFilePosition;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileAudioFourCC;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    message.iParam2 = KMccFileVideoFourCC;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fileSettingbuf,
                                              fileSettingbuf );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 

    //////
    
    message.iParam2 = KMccFileFastForward;
    message.iEndpointID = iSourceEndpointId;
    package = message;

    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              fastforwardSettingPak,
                                              fastforwardSettingPak );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    
    message.iParam2 = KMccDisplayLocalVideo;
    message.iEndpointID = iSinkEndpointId;
    package = message;

    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              displaySetting,
                                              displaySetting );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    
    message.iParam2 = 1000;
    package = message;

    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              displaySetting,
                                              displaySetting );
    EUNIT_ASSERT_EQUALS( err, KErrArgument ); 
    
    // Config key
    message.iParam2 = KMccConfigKey;
    message.iEndpointID = iSinkEndpointId;
    package = message;
    TBuf8<KMaxConfigKeyLen> confKey( KNullDesC8 );
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              confKey,
                                              confKey );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound ); 
    
    message.iParam2 = KMccConfigKey;
    message.iEndpointID = iSourceEndpointId;
    package = message;
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              confKey,
                                              confKey );
    EUNIT_ASSERT_EQUALS( err, KErrNone ); 
    }
void UT_CMccController::UT_CMccController_UpdateEndpointL()
    {
    TMccGenericMessage message;
    message.iEndpointID = 0;
    
    TMccGenericMsgBuffer package(message);
    TBuf8<5> buf(_L8("hell"));
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccUpdateDataSink,
                                              package,
                                              buf,
                                              buf );
                                              
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );                                  
                                        
    err = iController.CustomCommandSync( iMessageDest,
                                              EMccUpdateDataSource,
                                              package,
                                              buf,
                                              buf );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );                                  
                                             
    }
void UT_CMccController::UT_CMccController_ReferenceCountL()
    {
    TMccGenericMessage genMessage;
    genMessage.iSessionID = 0;
    genMessage.iLinkID = 0;
    genMessage.iStreamID = 0;
    genMessage.iEndpointID = 0;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetReferenceCount,
                                         msgBuffer,
                                         KNullDesC8,
                                         msgBuffer );
    
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );                                  

    }
void UT_CMccController::UT_CMccController_ResetL(  )
    {
    TInt error = iController.Reset();
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_PlayL(  )
    {
    TInt error = iController.Play();
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_StopL(  )
    {
    TInt error = iController.Stop();
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_PrimeL(  )
    {
    TInt error = iController.Prime();
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_PauseL(  )
    {
    TInt error = iController.Pause();
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_PositionL(  )
    {
    TTimeIntervalMicroSeconds position;
    TInt error = iController.GetPosition( position );
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_SetPositionL(  )
    {
    TInt error = iController.SetPosition( TTimeIntervalMicroSeconds(1) );
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_DurationL(   )
    {
    TTimeIntervalMicroSeconds time;
    
    TInt error = iController.GetDuration( time  ); 
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    
    TInt number;
    error = iController.GetNumberOfMetaDataEntries( number );
    
    EUNIT_ASSERT_EQUALS( error, KErrNotSupported );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iController.GetMetaDataEntryL( number ), KErrNotSupported );
    }

void UT_CMccController::UT_CMccController_RestartEventMonitorL()
    {
    TMccEvent event;
    event.iSessionId = iSessionId + 42;
    event.iLinkId = iLinkId;
    
    TMccEventPackage package( event );
    
    #if 0
    iController.CustomCommandAsync( iMessageDest,
                                    ERequestEventNotification,
                                    KNullDesC8,
                                    KNullDesC8,
                                    package,
                                    iStatus );

    event.iSessionId = iSessionId;
    event.iLinkId = iLinkId;
    
    TMccEventPckg package2( event );
    
    iController.CustomCommandAsync( iMessageDest,
                                    ERequestEventNotification,
                                    KNullDesC8,
                                    KNullDesC8,
                                    package,
                                    iStatus );
    #endif
    
    EUNIT_ASSERT_NO_LEAVE( iController.CustomCommandAsync( 
                                        iMessageDest,
                                        ERequestEventNotification,
                                        KNullDesC8,
                                        KNullDesC8,
                                        package,
                                        iStatus          ) 
                         );
    //Check if possible to send an event and after that wait for
    //iStatus to change value, User::WaitForAnyRequest( iStatus );
    //read package. 
    User::WaitForRequest( iStatus );
    
    }

void UT_CMccController::UT_CMccController_SendEventL()
    {/*
    TMccEvent event;
    EUNIT_ASSERT(iControllerPtr->StoreAndSendEvent(event) == KErrNone );   
    */
    }

void UT_CMccController::UT_CMccController_ReuseL()
    { 
    TMccStream params;
   
    params.iEndpointID = 1;
    TMccStreamPckg package( params );

    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccReuse,
                                         package,
                                         KNullDesC8 );
    EUNIT_ASSERT_EQUALS( err, KErrNotFound );
    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMccController,
    "MCCController Test",
    "UNIT" )

EUNIT_TEST(
    "CreateSessionL - test ",
    "CMccController",
    "CreateSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccController_CreateSessionLL, Teardown)

EUNIT_TEST(
    "CloseSessionL - test ",
    "CMccController",
    "CloseSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMccController_CloseSessionLL, Teardown)

EUNIT_TEST(
    "CreateLinkL - test ",
    "CMccController",
    "CreateLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccController_CreateLinkLL, Teardown)

EUNIT_TEST(
    "CloseLinkL - test ",
    "CMccController",
    "CloseLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccController_CloseLinkLL, Teardown)

EUNIT_TEST(
    "StartStreamL - test ",
    "CMccController",
    "StartStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StartStreamLL, Teardown2)

EUNIT_TEST(
    "StopStreamL - test ",
    "CMccController",
    "StopStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StopStreamLL, Teardown2)

EUNIT_TEST(
    "StartInactivityTimerL - test ",
    "CMccController",
    "StartInactivityTimerL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StartInactivityTimerLL, Teardown2)

EUNIT_TEST(
    "StopInactivityTimerL - test ",
    "CMccController",
    "StopInactivityTimerL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StopInactivityTimerLL, Teardown2)

EUNIT_TEST(
    "RequestEventNotificationL - test ",
    "CMccController",
    "RequestEventNotificationL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_RequestEventNotificationLL, Teardown2)


EUNIT_TEST(
    "SendEvent - test ",
    "CMccController",
    "SendEvent",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendEventL, Teardown2)
/*
EUNIT_TEST(
    "SendEventL - test ",
    "CMccController",
    "SendEventL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendEventLL, Teardown2)
*/

EUNIT_TEST(
    "CreateStreamL - test ",
    "CMccController",
    "CreateStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_CreateStreamLL, Teardown2)

EUNIT_TEST(
    "DeleteStreamL - test ",
    "CMccController",
    "DeleteStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_DeleteStreamLL, Teardown2)

EUNIT_TEST(
    "PrepareStreamL - test ",
    "CMccController",
    "PrepareStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_PrepareStreamLL, Teardown2)

EUNIT_TEST(
    "PauseStreamL - test ",
    "CMccController",
    "PauseStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_PauseStreamLL, Teardown2)

EUNIT_TEST(
    "ResumeStreamL - test ",
    "CMccController",
    "ResumeStreamL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_ResumeStreamLL, Teardown2)

EUNIT_TEST(
    "GetSupportedCodecsL - test ",
    "CMccController",
    "GetSupportedCodecsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetSupportedCodecsLL, Teardown2)

EUNIT_TEST(
    "GetCodecL - test ",
    "CMccController",
    "GetCodecL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetCodecLL, Teardown2)

EUNIT_TEST(
    "GetFmtpAttrL - test ",
    "CMccController",
    "GetFmtpAttrL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetFmtpAttrLL, Teardown2)

EUNIT_TEST(
    "SetCodecL - test ",
    "CMccController",
    "SetCodecL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetCodecLL, Teardown2)

EUNIT_TEST(
    "GetSupportedBitratesL - test ",
    "CMccController",
    "GetSupportedBitratesL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetSupportedBitratesLL, Teardown2)

EUNIT_TEST(
    "SetPriorityL - test ",
    "CMccController",
    "SetPriorityL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetPriorityLL, Teardown2)

EUNIT_TEST(
    "StreamSettingsL - test ",
    "CMccController",
    "StreamSettingsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StreamSettingsLL, Teardown2)

EUNIT_TEST(
    "SendRTCPReceiverReportL - test ",
    "CMccController",
    "SendRTCPReceiverReportL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendRTCPReceiverReportLL, Teardown2)

EUNIT_TEST(
    "SendRTCPSenderReportL - test ",
    "CMccController",
    "SendRTCPSenderReportL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendRTCPSenderReportLL, Teardown2)

EUNIT_TEST(
    "SendRTCPDataL - test ",
    "CMccController",
    "SendRTCPDataL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendRTCPDataLL, Teardown2)

EUNIT_TEST(
    "SetRemoteAddressL - test ",
    "CMccController",
    "SetRemoteAddressL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetRemoteAddressLL, Teardown2)

EUNIT_TEST(
    "SetRemoteRtcpAddressL - test ",
    "CMccController",
    "SetRemoteRtcpAddressL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetRemoteRtcpAddrLL, Teardown2)


EUNIT_TEST(
    "SendMediaSignalL - test ",
    "CMccController",
    "SendMediaSignalL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendMediaSignalLL, Teardown2)
/*

EUNIT_TEST(
    "FindSessionL - test ",
    "CMccController",
    "FindSessionL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_FindSessionLL, Teardown2)


EUNIT_TEST(
    "CustomCommandL - test ",
    "CMccController",
    "CustomCommandL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_CustomCommandLL, Teardown2)



EUNIT_TEST(
    "SendMccEventToClient - test ",
    "CMccController",
    "SendMccEventToClient",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendMccEventToClientL, Teardown2)

EUNIT_TEST(
    "SendMediaSignallingToClient - test ",
    "CMccController",
    "SendMediaSignallingToClient",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SendMediaSignallingToClientL, Teardown2)

EUNIT_TEST(
    "CustomCommand - test ",
    "CMccController",
    "CustomCommand",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_CustomCommandL, Teardown2)
*/
EUNIT_TEST(
    "MapdGetVolumeL - test ",
    "CMccController",
    "MapdGetVolumeL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MapdGetVolumeLL, Teardown2)

EUNIT_TEST(
    "MapdGetMaxVolumeL - test ",
    "CMccController",
    "MapdGetMaxVolumeL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MapdGetMaxVolumeLL, Teardown2)

EUNIT_TEST(
    "MapdSetVolumeL - test ",
    "CMccController",
    "MapdSetVolumeL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MapdSetVolumeLL, Teardown2)

EUNIT_TEST(
    "MapdGetBalanceL - test ",
    "CMccController",
    "MapdGetBalanceL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MapdGetBalanceLL, Teardown2)

EUNIT_TEST(
    "MapdSetBalanceL - test ",
    "CMccController",
    "MapdSetBalanceL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MapdSetBalanceLL, Teardown2)

EUNIT_TEST(
    "MapdSetVolumeRampL - test ",
    "CMccController",
    "MapdSetVolumeRampL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MapdSetVolumeRampLL, Teardown2)

EUNIT_TEST(
    "MardSetGainL - test ",
    "CMccController",
    "MardSetGainL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MardSetGainLL, Teardown2)

EUNIT_TEST(
    "MardGetMaxGainL - test ",
    "CMccController",
    "MardGetMaxGainL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MardGetMaxGainLL, Teardown2)

EUNIT_TEST(
    "MardGetGainL - test ",
    "CMccController",
    "MardGetGainL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MardGetGainLL, Teardown2)

EUNIT_TEST(
    "MardSetBalanceL - test ",
    "CMccController",
    "MardSetBalanceL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MardSetBalanceLL, Teardown2)

EUNIT_TEST(
    "MardGetBalanceL - test ",
    "CMccController",
    "MardGetBalanceL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_MardGetBalanceLL, Teardown2)


EUNIT_TEST(
    "AddDataSinkL - test ",
    "CMccController",
    "AddDataSinkL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_AddDataSinkL, Teardown2)

EUNIT_TEST(
    "AddDataSourceL - test ",
    "CMccController",
    "AddDataSourceL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_AddDataSourceL, Teardown2)

EUNIT_TEST(
    "ResetL - test ",
    "CMccController",
    "ResetL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_ResetL, Teardown2)
        
EUNIT_TEST(
    "PlayL - test ",
    "CMccController",
    "PlayL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_PlayL, Teardown2)

EUNIT_TEST(
    "StopL - test ",
    "CMccController",
    "StopL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StopL, Teardown2)
    
EUNIT_TEST(
    "PrimeL - test ",
    "CMccController",
    "PrimeL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_PrimeL, Teardown2)
         
EUNIT_TEST(
    "PauseL - test ",
    "CMccController",
    "PauseL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_PauseL, Teardown2)
    
EUNIT_TEST(
    "PositionL - test ",
    "CMccController",
    "PositionL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_PositionL, Teardown2)     
    
EUNIT_TEST(
    "SetPositionL - test ",
    "CMccController",
    "SetPositionL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetPositionL, Teardown2)

EUNIT_TEST(
    "DurationL - test ",
    "CMccController",
    "DurationL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_DurationL, Teardown2)     

EUNIT_TEST(
    "RestartEventMonitor - test ",
    "CMccController",
    "RestartEventMonitor",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_RestartEventMonitorL, Teardown2)
    
EUNIT_TEST(
    "StreamExistsL - test ",
    "CMccController",
    "StreamExistsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_StreamExistsL, Teardown2)
    
EUNIT_TEST(
    "GetSSRCL - test ",
    "CMccController",
    "GetSSRCL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetSSRCL, Teardown2)
   
      
EUNIT_TEST(
    "EnableViewFinderL - test ",
    "CMccController",
    "EnableViewFinderL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_EnableViewFinderL, Teardown2)
    
EUNIT_TEST(
    "DisableViewFinderL - test ",
    "CMccController",
    "DisableViewFinderL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_DisableViewFinderL, Teardown2)
    
EUNIT_TEST(
    "GetViewFinderSettingsL - test ",
    "CMccController",
    "GetViewFinderSettingsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetViewFinderSettingsL, Teardown2)
   
EUNIT_TEST(
    "GetCamInfoL - test ",
    "CMccController",
    "GetCamInfoL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetCamInfoL, Teardown2)
EUNIT_TEST(
    "GetNumOfCamsL - test ",
    "CMccController",
    "GetNumOfCamsL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetNumOfCamsL, Teardown2)
  
EUNIT_TEST(
    "GetCamValue1L - test ",
    "CMccController",
    "GetCamValue1L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetCamValue1L, Teardown2)
  
EUNIT_TEST(
    "SetCamValue1L - test ",
    "CMccController",
    "SetCamValue1L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetCamValue1L, Teardown2)
  
EUNIT_TEST(
    "GetCamValue2L - test ",
    "CMccController",
    "SetCamValue1L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetCamValue2L, Teardown2)
  
EUNIT_TEST(
    "SetCamValue2L - test ",
    "CMccController",
    "SetCamValue2L",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetCamValue2L, Teardown2)
  
EUNIT_TEST(
    "SetAudioRouteL - test ",
    "CMccController",
    "SetAudioRouteL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetAudioRouteL, Teardown2)
  
EUNIT_TEST(
    "GetAudioRouteL - test ",
    "CMccController",
    "GetAudioRouteL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetAudioRouteL, Teardown2)
  
EUNIT_TEST(
    "SetEndpointParameterL - test ",
    "CMccController",
    "SetEndpointParameterL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_SetEndpointParameterL, Teardown2)
  
EUNIT_TEST(
    "GetEndpointParameterL - test ",
    "CMccController",
    "GetEndpointParameterL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_GetEndpointParameterL, Teardown2)
  
EUNIT_TEST(
    "UpdateEndpointL - test ",
    "CMccController",
    "UpdateEndpointL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_UpdateEndpointL, Teardown2)
  

EUNIT_TEST(
    "ReferenceCountL - test ",
    "CMccController",
    "ReferenceCountL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_ReferenceCountL, Teardown2)
  
EUNIT_TEST(
    "ReuseL - test ",
    "CMccController",
    "ReuseL",
    "FUNCTIONALITY",
    Setup2L, UT_CMccController_ReuseL, Teardown2)
     
EUNIT_END_TEST_TABLE

//  END OF FILE
