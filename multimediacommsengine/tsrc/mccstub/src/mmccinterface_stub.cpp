/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Mcc Interface
*
*/


// INCLUDE FILES
#include "MmccInterface_stub.h"
#include "CMccController_stub.h"
#include <MmccNetworkSettings.h>
#include <mmccevents.h>
#include "MmccCodecAMR.h"
#include "MmccCodecDTMF.h"
#include <MmccCodecInformation.h>
#include <MmccCodecInformationFactory.h>
#include "MmccInterfaceLogs.h"
#include <ECam.h>


// These have to be same as in rtpdef.h, but can not include rtpdef.h here,
// since that results compilation errors for redefinition of TRtpPeerStat and
// TTimeStamps
//typedef TUint32 TRtpId;
//const TRtpId KNullId = 0xffffffff;



const TUid KImplementMccController  = { 0 };

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CMccInterface::CMccInterface
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMccInterface::CMccInterface() 
    {
    }

// ---------------------------------------------------------------------------
// CMccInterface::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMccInterface::ConstructL( MMccCtrlObserver& aObserver )
    {
	__INTERFACE( "MccInterface::ConstructL" )
    TInt err = KErrNone;
   
    // Open Controller
    err = OpenController( KImplementMccController );
    if ( err )
        {
        User::Leave( err );
        }


	// No need to ask CMccControllerStub here, since OpenController() created
	// it when necessary.
    iController->iObserver = &aObserver;

    //iMccEvent = CMccEvent::NewL( iController, aObserver );
    //iMccEvent->RequestEventNotification();

	CapabilitiesL();
	__INTERFACE( "MccInterface::ConstructL, exit" )
    }

// ---------------------------------------------------------------------------
// CMccInterface::NewL()
// Static constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CMccInterface* CMccInterface::NewL( MMccCtrlObserver& aObserver )
    {
    CMccInterface* self = new(ELeave) CMccInterface();
    CleanupStack::PushL( self );
    self->ConstructL( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccInterface::~CMccInterface()
// Destructor
// ---------------------------------------------------------------------------
//
CMccInterface::~CMccInterface()
    {
	__INTERFACE( "MccInterface::~CMccInterface" )

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		ctrl->Close();
		}

    iCodecInformation.ResetAndDestroy();
    iCodecInformation.Close();
	__INTERFACE( "MccInterface::~CMccInterface, exit" )
    }

// -----------------------------------------------------------------------------
// CMccInterface::CapabilitiesL()
// Get supported codecs from controller.
// -----------------------------------------------------------------------------
//
void CMccInterface::CapabilitiesL()
    {
	__INTERFACE( "MccInterface::CapabilitiesL" )
    RArray<TFourCC> codecs;     
    CleanupClosePushL( codecs );       
    CMccCodecInformationFactory* codecFactory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( codecFactory );
    
    User::LeaveIfError( GetSupportedCodecs( codecs ) );

    TFourCC fourCCValue;
    
	__INTERFACE_INT1( "MccInterface::CapabilitiesL, count of found codecs:", codecs.Count() )
    for ( TInt k = 0; k < codecs.Count(); k++ )
        {
        fourCCValue = codecs[k];
        
        CMccCodecInformation* codec = codecFactory->CreateCodecInformationL( fourCCValue );
        if ( codec )
            {
            CleanupStack::PushL( codec );
			__INTERFACE_STR8( "MccInterface::CapabilitiesL, appending:", codec->SdpName() )
            iCodecInformation.AppendL( codec );            
            CleanupStack::Pop( codec );
            }
        }
    
    
    CleanupStack::PopAndDestroy( codecFactory );
    CleanupStack::PopAndDestroy( &codecs );
    
	__INTERFACE( "MccInterface::CapabilitiesL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccInterface::GetSupportedCodecs()
// Get supported codecs.
// -----------------------------------------------------------------------------
//
TInt CMccInterface::GetSupportedCodecs( RArray<TFourCC>& aCodecs )
    {
	__INTERFACE( "MccInterface::GetSupportedCodecs" )
	

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNone );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccGetSupportedCodecs );
		}
	else
		{
		return KErrNotFound;
		}

/*    
    TMccSession session;
    
    TInt count = session.iFourCCArray.Count();
    for ( TInt i = 0; i < count; i++)
        {
        session.iFourCCArray[i] = NULL;
        }

    TMccSessionPckg package( session );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetSupportedCodecs,
                                         package,
                                         KNullDesC8,
                                         package );

    if ( !err ) 
        {
        count =  package().iFourCCArray.Count();
		__INTERFACE_INT1( "MccInterface::GetSupportedCodecs, found", count )
        for( TInt i = 0; i < count; i++ )
            {
            if ( package().iFourCCArray[i] != NULL )
                {
                aCodecs.Append( package().iFourCCArray[i] );
                }
            }
        }
  */
  
    if ( !err )
        {
        err = aCodecs.Append( KMccFourCCIdAMRNB );
        if ( !err )
            {
            err = aCodecs.Append( KMccFourCCIdH263 );
            }
        }
    
    
	__INTERFACE_INT1( "MccInterface::GetSupportedCodecs, exit with ", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CMccInterface::OpenController()
// Opens Mcc Controller
// -----------------------------------------------------------------------------
//
TInt CMccInterface::OpenController( const TUid& /*aControllerUid*/ )
    {
	__INTERFACE( "MccInterface::OpenController" )
    
    TRAPD( error, CMccControllerStub::OpenL() );
    iController = CMccControllerStub::Stub();
    
    if ( iController && !error )
        {
        error = iController->iLeaveError; 
        }
        
    /*
    TMMFPrioritySettings settings;
    settings.iPriority = EPriorityNormal;

    error = iController.Open( aControllerUid, settings );
    if ( error )
        {
        iController.Close();
        }
    */
	__INTERFACE_INT1( "MccInterface::OpenController, exit with", error)
    return error;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SetRemoteAddress
// Set remote IP address and port number for a given RTP session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::SetRemoteAddress(  TUint32 aSessionId, TUint32 aLinkId,
                                               const TInetAddr& /*aRemoteAddr*/, 
                                               TUint /*aRemoteRtcpPort*/ )
    {
	__INTERFACE( "MccInterface::SetRemoteAddress" )
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccSetRemoteAddress,
									  aSessionId,
									  aLinkId );
		}
    /*
    TMccAddress addr;
    addr.iSessionID = aSessionId;
    addr.iLinkID = aLinkId;
    addr.iAddress = aRemoteAddr;
    TMccAddressPckg package( addr );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetRemoteAddress,
                                         package,
                                         KNullDesC8 );
    */
	__INTERFACE_INT1( "MccInterface::SetRemoteAddress, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::CreateSession
// Creates a new RTP session with a remote participant.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CreateSession( TUint32& aSessionId )
    {
	__INTERFACE( "MccInterface::CreateSession" )
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt result( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		result = ctrl->ControllerCalled( CMccControllerStub::EMccCreateSession );
		aSessionId = ctrl->NextSessionId();
		__INTERFACE_INT1( "MccInterface::CreateSession, sesid ", aSessionId )
		}
	
	/*
    TMccCreateSession pktSession;
    TMccCreateSessionPckg package( pktSession );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccCreateSession,
                                                 package,
                                                 KNullDesC8,
                                                 package );

    if ( result == KErrNone )
        {
        aSessionId = package().iSessionID;
        }
    */
	__INTERFACE_INT1( "MccInterface::CreateSession, exit with ", result )
    return result;
    }

// ---------------------------------------------------------------------------
// CMccInterface::CreateLink
// Creates a new Mcc link with a remote participant.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CreateLink( TUint32 aSessionId,
                                         TInt aLinkType,
                                         TUint32& aLinkId,
                                         TMccNetSettings& /*aNetSettings*/ )
    {    
	__INTERFACE( "MccInterface::CreateLink")
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt result( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		result = ctrl->ControllerCalled( CMccControllerStub::EMccCreateLink,
				                  		aSessionId,
				                  		KMceNotAssigned,
				                  		KMceNotAssigned,
				                  		KMceNotAssigned,
				                  		aLinkType );
		aLinkId = ctrl->NextLinkId();
		__INTERFACE_INT1( "MccInterface::CreateLink, lid ", aLinkId )
		}

    /*	
    TMccCreateLink pktLink;
    pktLink.iSessionID = aSessionId;
    pktLink.iLinkType = aLinkType;
    pktLink.iIapId = aNetSettings.iIapId;
    pktLink.iAddress = aNetSettings.iRemoteAddress;
    pktLink.iPort = aNetSettings.iLocalPort;
    pktLink.iMediaSignaling = aNetSettings.iMediaSignalling;

    TMccCreateLinkPckg package( pktLink );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccCreateLink,
                                                 package,
                                                 KNullDesC8,
                                                 package );

    if ( result == KErrNone )
        {
        aNetSettings.iLocalPort = package().iPort;
        aLinkId = package().iLinkID;
        }*/

	__INTERFACE_INT1( "MccInterface::CreateLink, exit with ", result )
    return result;
    }

// ---------------------------------------------------------------------------
// CMccInterface::CloseSession
// Closes RTP session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CloseSession( TUint32 aSessionId )
    {
	__INTERFACE_INT1( "MccInterface::CloseSession, sesid ", aSessionId )

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccCloseSession,
									  aSessionId );
		}
/*    
    TMccSession sessID;
    sessID.iSessionID = aSessionId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccCloseSession,
                                         package,
                                         KNullDesC8 );
*/
                                         
	__INTERFACE_INT1( "MccInterface::CloseSession, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::CloseLink
// Closes Mcc link.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CloseLink( TUint32 aSessionId, TUint32 aLinkId )
    {
	__INTERFACE_INT1( "MccInterface::CloseLink, lid ", aLinkId )
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccCloseLink,
									  aSessionId,
									  aLinkId );
		}

    /*
    TMccGenericMessage message;
    message.iSessionID = aSessionId;
    message.iLinkID = aLinkId;
    TMccGenericMsgBuffer package( message );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccCloseLink,
                                         package,
                                         KNullDesC8 );
    */                                         
	__INTERFACE_INT1( "MccInterface::CloseLink, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::CreateStream
// Creates a stream to a RTP session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CreateStream( TUint32 aSessionId, 
                                           TUint32 aLinkId,
                                           TUint32& aStreamId,
                                           TInt aStreamType,
                                           CMccCodecInformation& aCodecInformation )
    {
	__INTERFACE( "MccInterface::CreateStream" )

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccCreateStream,
									  aSessionId, 
                                      aLinkId,
                                      KMceNotAssigned,
                                      KMceNotAssigned, 
                                      KMceNotAssigned,
                                      aStreamType );
		aStreamId = ctrl->NextStreamId();
		__INTERFACE_INT1( "MccInterface::CreateStream, sid ", aStreamId )
		}
    
    /*
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    TMccCreateStreamParam message;
    message.iPayloadType = aCodecInformation->PayloadType();
    message.iFourCC = aCodecInformation->FourCC();
    message.iStreamType = aStreamType;
    message.iPrioritySettings.iPriority = aCodecInformation->Priority();
    message.iPrioritySettings.iPref =
    	TMdaPriorityPreference( aCodecInformation->PriorityPreference() );
    TMccCreateStreamParamPckg messagePckg( message );
     
    TInt err = iController.CustomCommandSync( iMessageDest, EMccCreateStream,
                                         msgBuffer, messagePckg,
                                         msgBuffer );

	if( err != KErrNone )
	    {
        return err;
	    }

    aStreamId = msgBuffer().iStreamID;
    */
    
    err = this->SetCodec( aSessionId, aLinkId, aStreamId, aCodecInformation );
	    
	__INTERFACE_INT1( "MccInterface::CreateStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::DeleteStream
// Deletes media stream in a given session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::DeleteStream( TUint32 aSessionId, 
                                           TUint32 aLinkId, 
				                           TUint32 aStreamId )
    {
	__INTERFACE_INT1( "MccInterface::DeleteStream, sid ", aStreamId )
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccDeleteStream,
									  aSessionId, 
                                      aLinkId,
                                      aStreamId,
                                      0 );
		}

    /*
    TMccSession sessID;
    sessID.iSessionID = aSessionId;
    sessID.iLinkID = aLinkId;
    sessID.iStreamID = aStreamId;
    sessID.iEndpointID = aSinkSourceId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccDeleteStream,
                                         package,
                                         KNullDesC8 );
    */
    
	__INTERFACE_INT1( "MccInterface::DeleteStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::StartInactivityTimer
// Starts inactivity timer for a stream in a given session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::StartInactivityTimer( TUint32 aSessionId,
                                                   TUint32 aLinkId,
                                                   TUint32 aStreamId,
                                                   TUint32 /*aTimeoutTime*/,
                                                   TUint32 aEndpointId )
    {
	__INTERFACE( "MccInterface::StartInactivityTimer" )
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccInactivityTimerStart, 
                                      aSessionId,
                                      aLinkId,
                                      aStreamId,
                                      aEndpointId );
		}

    /*	
    TMccInactivityTimer inactivityTimer;
    inactivityTimer.iSessionID = aSessionId;
    inactivityTimer.iLinkID = aLinkId;
    inactivityTimer.iStreamID = aStreamId;
    inactivityTimer.iTimeoutTime = aTimeoutTime;
    TMccInactivityTimerPckg package( inactivityTimer );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccInactivityTimerStart,
                                              package,
                                              KNullDesC8 );
	*/
    
	__INTERFACE_INT1( "MccInterface::StartInactivityTimer, exit with ", err )
    return err;
    } 

// ---------------------------------------------------------------------------
// CMccInterface::StopInactivityTimer
// Stops inactivity timer for a stream in a given session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::StopInactivityTimer( TUint32 aSessionId,
                                                  TUint32 aLinkId,
                                                  TUint32 aStreamId,
                                                  TUint32 aEndpointId )
    {
	__INTERFACE( "MccInterface::StopInactivityTimer" )

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccInactivityTimerStop, 
                                      aSessionId,
                                      aLinkId,
                                      aStreamId,
                                      aEndpointId );
		}

    /*	
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccInactivityTimerStop,
                                              msgBuffer,
                                              KNullDesC8 );
	*/

	__INTERFACE_INT1( "MccInterface::StopInactivityTimer, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::PrepareStream
// Prepares a stream so that it is ready to be started.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::PrepareStream( TUint32 aSessionId, 
                                            TUint32 aLinkId, 
				                            TUint32 aStreamId,
				                            TUint32 aSinkSourceId )
    {
	__INTERFACE_INT1( "MccInterface::PrepareStream, sid", aStreamId )

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerCalled( CMccControllerStub::EMccPrepareStream,
									  aSessionId, 
                                      aLinkId,
                                      aStreamId,
                                      aSinkSourceId );
		}

    /*	
    TInt err = KErrNone;
    TMccSession sessID;
    sessID.iSessionID = aSessionId;
    sessID.iLinkID = aLinkId;
    sessID.iStreamID = aStreamId;
    sessID.iEndpointID = aSinkSourceId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccPrepareStream,
                                         package,
                                         KNullDesC8 );
    */
    
	__INTERFACE_INT1( "MccInterface::PrepareStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::StartStream
// Starts to send/receive media stream in a given session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::StartStream( TUint32 aSessionId, 
                                          TUint32 aLinkId, 
				                          TUint32 aStreamId,
				                          TUint32 aSinkSourceId,
				                          TBool aPaused,
				                          TBool aEnableRTCP )
    {
	__INTERFACE_INT1( "MccInterface::StartStream, sid", aStreamId )

	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerStreamCtrCalled( CMccControllerStub::EMccStartStream, 
                                               aSessionId,
                                               aLinkId,
                                               aStreamId,
                                               aSinkSourceId,
                                               aPaused,
                                               aEnableRTCP );
		}

    /*	
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aSinkSourceId;
    params.iStreamPaused = aPaused;
    params.iEnableRTCP = aEnableRTCP;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccStartStream,
                                         package,
                                         KNullDesC8 );
    */

	__INTERFACE_INT1( "MccInterface::StartStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::PauseStream
// Pauses media stream in a given session. No RTP packets are send/received anymore.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::PauseStream( TUint32 aSessionId, 
                                          TUint32 aLinkId, 
				                          TUint32 aStreamId,
				                          TUint32 aSinkSourceId,
				                          TBool aEnableRTCP )
    {
	__INTERFACE_INT1( "MccInterface::PauseStream, sid", aStreamId )
	
	// Always ask controller stub, since if there are multiple CMccInterface
	// instances, and one of them closes controller, the other CMccInterface
	// instances wouldn't know it.
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerStreamCtrCalled( CMccControllerStub::EMccPauseStream, 
                                               aSessionId,
                                               aLinkId,
                                               aStreamId,
                                               aSinkSourceId,
                                               EFalse,
                                               aEnableRTCP );
		}

	/*
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aSinkSourceId;
    params.iEnableRTCP = aEnableRTCP;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccPauseStream,
                                         package,
                                         KNullDesC8 );
    */
                                         
	__INTERFACE_INT1( "MccInterface::PauseStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::ResumeStream
// Resumes paused media stream in a given session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::ResumeStream( TUint32 aSessionId, 
                                           TUint32 aLinkId, 
				                           TUint32 aStreamId,
				                           TUint32 aSinkSourceId,
				                           TBool aEnableRTCP )
    {
	__INTERFACE_INT1( "MccInterface::ResumeStream, sid", aStreamId )

	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerStreamCtrCalled( CMccControllerStub::EMccResumeStream, 
                                               aSessionId,
                                               aLinkId,
                                               aStreamId,
                                               aSinkSourceId,
                                               EFalse,
                                               aEnableRTCP );
		}

    /*	
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aSinkSourceId;
    params.iEnableRTCP = aEnableRTCP;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccResumeStream,
                                         package,
                                         KNullDesC8 );
    */
	__INTERFACE_INT1( "MccInterface::ResumeStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::StopStream
// Stops sending/receiving media stream in a given session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::StopStream( TUint32 aSessionId, 
                                         TUint32 aLinkId, 
				                         TUint32 aStreamId,
				                         TUint32 aSinkSourceId )
    {
	__INTERFACE_INT1( "MccInterface::StopStream, sid", aStreamId )
	
	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerStreamCtrCalled( CMccControllerStub::EMccStopStream,
                                               aSessionId,
                                               aLinkId,
                                               aStreamId,
                                               aSinkSourceId );
		}

    /*    	
    TInt err = KErrNone;
    TMccSession sessID;
    sessID.iSessionID = aSessionId;
    sessID.iLinkID = aLinkId;
    sessID.iStreamID = aStreamId;
    sessID.iEndpointID = aSinkSourceId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccStopStream,
                                         package,
                                         KNullDesC8 );

    */
    
	__INTERFACE_INT1( "MccInterface::StopStream, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::TranscodeFile
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::TranscodeFile( 
    const TDesC8& /*aSourceFile*/, 
    const TDesC8& /*aDestFile*/,
	TUint32 /*aQuality*/, 
	TUint32& aSessionId,  
	const CMccCodecInformation* /*aVideoCodec*/,
	const CMccCodecInformation* /*aAudioCodec*/ )
    {
    __INTERFACE( "MccInterface::TranscodeFile" )
    aSessionId = 1;

	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerStreamCtrCalled( CMccControllerStub::EMccTranscodeFile,
                                               aSessionId,
                                               KMceNotAssigned,
                                               KMceNotAssigned,
                                               KMceNotAssigned );
		}
                                          
	__INTERFACE_INT1( "MccInterface::TranscodeFile, exit with ", err )
	
	return err;
    }
  
// ---------------------------------------------------------------------------
// CMccInterface::CancelTranscodeFile
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CancelTranscodeFile( TUint32 aSessionId )
    {
    __INTERFACE( "MccInterface::CancelTranscodeFile" )

	TInt err( KErrNotFound );
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( ctrl )
		{
		err = ctrl->ControllerStreamCtrCalled( CMccControllerStub::EMccCancelTranscodeFile, 
                                               aSessionId,
                                               KMceNotAssigned,
                                               KMceNotAssigned,
                                               KMceNotAssigned );
		}

  	__INTERFACE_INT1( "MccInterface::CancelTranscodeFile, exit with ", err )                                     
  	
  	return err;
    }

// -----------------------------------------------------------------------------
// CMccInterfaceImpl::CodecL()
// Gets current codec used in a given stream.
// -----------------------------------------------------------------------------
//
EXPORT_C CMccCodecInformation* CMccInterface::CodecL( TUint32 aSessionId,
                                                         TUint32 aLinkId,
                                                         TUint32 aStreamId )
    {
	__INTERFACE( "MccInterface::CodecL" )  

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	__ASSERT_ALWAYS( ctrl != NULL, User::Leave( KErrNotFound ) );
	ctrl->ControllerCalled( CMccControllerStub::EMccGetCodec, 
                            aSessionId,
                            aLinkId,
                            aStreamId );
    CMccControllerStub::TCodecInfo codecBuffer;
    codecBuffer.iSessionID = aSessionId;
    codecBuffer.iLinkID = aLinkId;
    codecBuffer.iStreamID = aStreamId;
    
    ctrl->GetCodec( codecBuffer );
	
/*	
    TMccGenericMessage msg;
    msg.iSessionID = aSessionId;
    msg.iLinkID = aLinkId;
    msg.iStreamID = aStreamId;
    TMccGenericMsgBuffer msgBuffer( msg );
    
    TMccCodecInfo codecInfo;
    TMccCodecInfoBuffer codecBuffer( codecInfo );
    
    TInt err = ctrl->CustomCommandSync( iMessageDest, EMccGetCodec, msgBuffer,
                                         KNullDesC8, codecBuffer );
    
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
  */
        
    // Now we need to find the correspoding codec from iCodecArray & clone it.
    // Then we need to set the parameters returned from controller which are in codecInfo
    // to that cloned codec.
    TInt index = 0;
    TInt count = iCodecInformation.Count();
    TInt counter = 0;
    
    // If fourCC match, we've got the right one.
    TFourCC fourCC;
    while ( counter < count )
        {
        fourCC = iCodecInformation[counter]->FourCC();
        if( fourCC == codecBuffer.iFourCC )
            {
            index = counter;
            counter = count;
            }
        
        counter++;
        }
    
    if( index == KErrNotFound )
        {
        User::Leave( KErrNotFound );
        }

    CMccCodecInformation* retCodec = iCodecInformation[index]->CloneDetailedL();
    CleanupStack::PushL( retCodec );
    retCodec->SetBitrate( codecBuffer.iBitrate );
    retCodec->SetPTime( codecBuffer.iPtime );
    retCodec->SetMaxPTime( codecBuffer.iMaxPtime );
    retCodec->SetPayloadType( codecBuffer.iPayloadType );
    retCodec->SetCodecMode( TCodecMode( codecBuffer.iCodecMode ) );
    retCodec->EnableVAD( codecBuffer.iEnableDTX );
    retCodec->SetJitterBufInactivityTimeOut( codecBuffer.iJitterBufInactivityTimeOut );
    retCodec->SetJitterBufThreshold( codecBuffer.iJitterBufThreshold );
    retCodec->SetJitterBufBufferLength( codecBuffer.iJitterBufBufferLength );
    retCodec->SetAllowedBitrates( codecBuffer.iBitrateMask );
    retCodec->SetMaxBitrate( codecBuffer.iMaxBitrate );
    retCodec->SetAverageBitrate( codecBuffer.iAverageBitrate );
  	retCodec->SetFramerate( codecBuffer.iFramerate );
    retCodec->SetFrameWidth( codecBuffer.iVideoWidth );
    retCodec->SetFrameHeight( codecBuffer.iVideoHeight );
    
    retCodec->CreateFmtpAttrListL();
    CleanupStack::Pop( retCodec );
    
	__INTERFACE( "MccInterface::CodecL, exit" )    
    return retCodec;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SetCodec
// Sets codec to be used in a given stream.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::SetCodec( TUint32 aSessionId, 
                                       TUint32 aLinkId, 
                                       TUint32 aStreamId, 
                                       CMccCodecInformation& aCodecInformation
                                       )
    {
	__INTERFACE( "MccInterface::SetCodec" )

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
	TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccSetCodec, 
                            		   aSessionId,
                            		   aLinkId,
                            		   aStreamId );
    if ( &aCodecInformation )
        {
        CMccControllerStub::TCodecInfo codecInfo;
        
        codecInfo.iFourCC           = aCodecInformation.FourCC();
        codecInfo.iAlgoUsed         = aCodecInformation.Algo();
        codecInfo.iBitrate          = aCodecInformation.Bitrate();
        codecInfo.iCodecMode        = aCodecInformation.CodecMode();
        codecInfo.iEnableDTX        = aCodecInformation.VAD();
        codecInfo.iMaxPtime         = aCodecInformation.MaxPTime();
        codecInfo.iPtime            = aCodecInformation.PTime();
        codecInfo.iPayloadType      = aCodecInformation.PayloadType();
        codecInfo.iRedundantPayload = aCodecInformation.RedundancyPT();
        codecInfo.iRedundancyCount  = static_cast<TUint8>( aCodecInformation.RedCount() );
        codecInfo.iFrameSize        = static_cast<TUint8>( aCodecInformation.FrameSize() );
        codecInfo.iHwFrameTime      = static_cast<TUint8>( aCodecInformation.FrameTime() );
        codecInfo.iBitrateMask      = aCodecInformation.AllowedBitrates();
		codecInfo.iMaxBitrate		= aCodecInformation.MaxBitrate();
		codecInfo.iAverageBitrate	= aCodecInformation.AverageBitrate();
		codecInfo.iFramerate		= aCodecInformation.Framerate();
		codecInfo.iVideoWidth		= aCodecInformation.FrameWidth();
		codecInfo.iVideoHeight		= aCodecInformation.FrameHeight();
        
        //jitterbuffer settings
        codecInfo.iJitterBufInactivityTimeOut = aCodecInformation.JitterBufInactivityTimeOut();
        codecInfo.iJitterBufThreshold          = aCodecInformation.JitterBufThreshold();
        codecInfo.iJitterBufBufferLength      = aCodecInformation.JitterBufBufferLength();
        //playtone settings
        
        //codecInfo.iJitterBufPlayToneTimeout = aCodecInformation->JitterBufPlayToneTimeOut();
        //codecInfo.iJitterBufPlayToneFrequency = aCodecInformation->JitterBufPlayToneFrequency();
        //codecInfo.iJitterBufPlayToneDuration  = aCodecInformation->JitterBufPlayToneDuration();
        
        codecInfo.iSessionID = aSessionId;
        codecInfo.iLinkID    = aLinkId;
        codecInfo.iStreamID  = aStreamId;
        ctrl->SetCodec( codecInfo );
        
        /*
        TMccCodecInfoBuffer infoBuffer( codecInfo );
        
        TInt err = iController.CustomCommandSync( iMessageDest,
                                             EMccSetCodec,
                                             infoBuffer,
                                             KNullDesC8,
                                             infoBuffer );
        
        // Return payloadtype allocated
    	aCodecInformation->SetPayloadType( infoBuffer().iPayloadType );
        */
        return err;    
        }
    else
        {
        return KErrArgument;
        }   
    }
  
// ---------------------------------------------------------------------------
// CMccInterface::GetCapabilities
// Get Capabilities
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::GetCapabilities(
                  RPointerArray<CMccCodecInformation>& aCodecInformation ) const
	{
	__INTERFACE( "MccInterface::GetCapabilities" )      
    TInt count = iCodecInformation.Count();
    aCodecInformation.ResetAndDestroy();
    
    if( count )
        {
        CMccCodecInformation* tempCodec = NULL;
        TInt err( KErrNone );
        
    	for ( TInt i = 0; i < count; i++ )
    	    {
    	    // FIX ME for leavescan, easiest way to do this is to introduce an API
    	    // break so we can do harm on offending clients.
    	    TRAP( err, (tempCodec = iCodecInformation[i]->CloneDefaultsL()) );
    	    
    	    if( err != KErrNone )
    	        {
    	        aCodecInformation.ResetAndDestroy();
    	        return err;
    	        }
    	        
    		err = aCodecInformation.Append( tempCodec );
    		
    		if (err != KErrNone )
    			{
    			delete tempCodec;
    			}
    	    }

    	return err;
        }
    else
        {
        return KErrNotReady;
        }
    }

// ---------------------------------------------------------------------------
// CMccInterface::SendMediaSignallingL
// Sends media signalling to uplink if possible
// ---------------------------------------------------------------------------
//
EXPORT_C void CMccInterface::SendMediaSignalL( const TMccEvent& aEvent )
    {
	__INTERFACE( "MccInterface::SendMediaSignalL" ) 

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	__ASSERT_ALWAYS( ctrl != NULL, User::Leave( KErrNotFound ) );
    ctrl->ControllerSignal( aEvent.iSessionId,
                            aEvent.iLinkId,
                            aEvent.iStreamId,
                            aEvent.iEndpointId,
                            aEvent.iEventType );
	
    if( KNullId == aEvent.iSessionId )
        {
        User::Leave( KErrArgument );
        }
    else
        {
        // Need to pass a pointer as TMccSignal does not have a public
        // constructor and it is sort of an "abstract base class"
        //TMccSignal* tmp = const_cast<TMccSignal*>( &aSignal );
        /*        
        TMccSignalPtrPkg sigPkg( tmp );
        TInt err = ctrl->CustomCommandSync( iMessageDest, 
                                                  EMccSendMediaSignal,
                                                  sigPkg, KNullDesC8 );
        */
        User::LeaveIfError( ctrl->iLeaveError );
        }
    }

// ---------------------------------------------------------------------------
// CMccInterface::LinkExists
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CMccInterface::LinkExists( TUint32 /*aSessionId*/,
	TUint32& /*aLinkId*/, const TMccNetSettings& /*aNetSettings*/ )
	{
	__INTERFACE( "MccInterface::LinkExists" )      
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CMccInterface::GetSSRC
// Get Synchronization source
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::GetSSRC( TUint32 aSessionId,
                                 		TUint32 aLinkId,
                                 		TUint32 aStreamId,
                                 		TUint32 aEndpointId,
                                 		TUint32& aSSRC )
	{
	__INTERFACE( "MccInterface::GetSSRC" )

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccGetSSRC,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId,
                                       aEndpointId );
	aSSRC = 200;
	/*
    TMccGenericMessage message;
    message.iSessionID = aSessionId;
    message.iLinkID    = aLinkId;
    message.iStreamID  = aStreamId;
    TMccGenericMsgBuffer messagePckg( message );
    
    // payload contains SSRC info
	TMccSSRC     ssrc;
	TMccSSRCPckg ssrcPckg ( ssrc );
	
    TInt err = ctrl->CustomCommandSync( iMessageDest, 
                                              EMccGetSSRC,
                                              messagePckg, 
                                              KNullDesC8,
                                              ssrcPckg 
                                             );

    if ( err == KErrNone )
        {
        aSSRC = ssrcPckg().iSSRC;// ssrc.iSSRC; 
        }
        
	__INTERFACE_INT1( "MccInterface::GetSSRC, exit with", err)      
	*/
    return err;
	}
		
// ---------------------------------------------------------------------------
// CMccInterface::AddDataSink
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::AddDataSink( const TUid /*aSinkType*/, 
	const TDesC8& /*aParam*/, TUint32& aSinkSourceId )
	{
	__INTERFACE( "MccInterface::AddDataSink" )      

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}

    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccGetSinkId ); 
    aSinkSourceId = ctrl->NextEndpointId();

    /*
	
    TInt err = KErrNone;
    err = ctrl->AddDataSink( aSinkType, aParam );
    if ( err == KErrNone )
    	{
	    TMccSession sessID;
	    TMccSessionPckg package( sessID );

	    err = ctrl->CustomCommandSync( iMessageDest,
	                                         EMccGetSinkId,
	                                         package,
	                                         KNullDesC8,
	                                         package );
    	
    	aSinkSourceId = package().iEndpointID;
    	}

	__INTERFACE_INT1( "MccInterface::AddDataSink, exit with", err)      
	*/
    return err;    	
	}

// ---------------------------------------------------------------------------
// CMccInterface::AddDataSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::AddDataSource( const TUid /*aSourceType*/,
	const TDesC8& /*aParam*/, TUint32& aSinkSourceId )
	{
	__INTERFACE( "MccInterface::AddDataSource" )
	
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccGetSourceId );
    aSinkSourceId = ctrl->NextEndpointId();
     
	/*
    TInt err = KErrNone;
    err = ctrl->AddDataSource( aSourceType, aParam );		
    if ( err == KErrNone )
    	{
	    TMccSession sessID;
	    TMccSessionPckg package( sessID );

	    err = ctrl->CustomCommandSync( iMessageDest,
	                                         EMccGetSourceId,
	                                         package,
	                                         KNullDesC8,
	                                         package );
    	
    	aSinkSourceId = package().iEndpointID;
    	}

	__INTERFACE_INT1( "MccInterface::AddDataSource, exit with", err)      
	*/
    return err;    	
	}

// ---------------------------------------------------------------------------
// CMccInterface::GetSupportedBitrates
// Gets supported bit rates from the codec of the specified stream
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CMccInterface::StreamsExists( TUint32 aSessionId,
	TUint32 aLinkId )
	{
	__INTERFACE( "MccInterface::StreamsExists" )

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	return ctrl && ctrl->StreamsExists( aSessionId, aLinkId );
	}
	
// ---------------------------------------------------------------------------
// CMccInterface::GetSupportedBitrates
// Gets supported bit rates from the codec of the specified stream
// ---------------------------------------------------------------------------
//
TInt CMccInterface::GetSupportedBitrates( TUint32 aSessionId,
                                          TUint32 aLinkId,
                                          TUint32 aStreamId,
                                          RArray<TUint>& aBitrates )
    {
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccGetSupportedBitrates,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId );       
    TInt index = 0;
    aBitrates.Append( iCodecInformation[index]->Bitrate() );
    
    /*
    TMccGenericMessage message;
    message.iSessionID = aSessionId;
    message.iLinkID = aLinkId;
    message.iStreamID = aStreamId;
    TMccGenericMsgBuffer messagePckg( message );

    TMccBitrates bitrates;
    TMccBitratesPckg bitratePckg( bitrates );

    TInt err = ctrl->CustomCommandSync( iMessageDest, 
                                              EMccGetSupportedBitrates,
                                              messagePckg, KNullDesC8,
                                              bitratePckg );

    if ( err != KErrNone )
        {
        return err;
        }

    // The package should now contain the bitrates
    for ( TInt i = 0; i < KMaxBitrates; i++ )
        {
        if ( bitratePckg().iBitrates[i] != 0 )
            {
            aBitrates.Append( bitratePckg().iBitrates[i] );
            }
        }
    */        
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::UpdateDataSink
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::UpdateDataSink( TUint32 aSessionId, TUint32 aLinkId, 
                             TUint32 aStreamId, TUint32 aEndpointId,
                             const TDesC8& /*aParam*/ )
    {
	__INTERFACE( "MccInterface::UpdateDataSink" )

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccUpdateDataSink,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId,
                                       aEndpointId );
    return err;
	
	/*
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aEndpointId;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccUpdateDataSink,
                                         package,
                                         aParam );
                                         
	__INTERFACE_INT1( "MccInterface::UpdateDataSink, exit with ", err )
    return err;
    */    
    }

// ---------------------------------------------------------------------------
// CMccInterface::UpdateDataSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::UpdateDataSource( TUint32 aSessionId, TUint32 aLinkId, 
                                               TUint32 aStreamId, TUint32 aEndpointId,
                                               const TDesC8& /*aParam*/ )
    {
	__INTERFACE( "MccInterface::UpdateDataSource" )
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccUpdateDataSource,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId,
                                       aEndpointId );
    return err;
	
	/*
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aEndpointId;
    TMccStreamPckg package( params );

    err = ctrl->CustomCommandSync( iMessageDest,
                                         EMccUpdateDataSource,
                                         package,
                                         aParam );
                                         
	__INTERFACE_INT1( "MccInterface::UpdateDataSource, exit with ", err )
    return err;
    */
    }

// ---------------------------------------------------------------------------
// CMccInterface::RemoveDataSink
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CMccInterface::RemoveDataSink( 
    TUint32 aSessionId, 
    TUint32 aLinkId, 
    TUint32 aStreamId, 
    TUint32 aEndpointId )
    {
    __INTERFACE( "MccInterface::RemoveDataSink" )

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccRemoveDataSink,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId,
                                       aEndpointId );
    
/*    
    TMMFMessageDestination sourceHandleInfo = FindEndpointInfo( aEndpointId, err );
    if ( !err )
        {
        err = ctrl->RemoveDataSink( sourceHandleInfo );
        RemoveEndpointInfo( aEndpointId );
        }
*/        
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::RemoveDataSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::RemoveDataSource( 
    TUint32 aSessionId, 
    TUint32 aLinkId, 
    TUint32 aStreamId, 
    TUint32 aEndpointId )
    {
    __INTERFACE( "MccInterface::RemoveDataSource" )

	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccRemoveDataSource,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId,
                                       aEndpointId );
    /*
    TMMFMessageDestination sinkHandleInfo = FindEndpointInfo( aEndpointId, err );
    if ( !err )
        {
        err = ctrl->RemoveDataSource( sinkHandleInfo );
        RemoveEndpointInfo( aEndpointId );
        }
    */    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::Reuse
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::Reuse( TUint32 aSessionId, TUint32 aLinkId, 
                             TUint32 aStreamId, TUint32 aEndpointId )
    {
	__INTERFACE( "MccInterface::Reuse" )
	
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->ControllerCalled( CMccControllerStub::EMccReuse,
                                       aSessionId,
                                       aLinkId,
                                       aStreamId,
                                       aEndpointId );
    return err;

	/*
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aEndpointId;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccReuse,
                                         package,
                                         KNullDesC8 );
                                         
	__INTERFACE_INT1( "MccInterface::Reuse, exit with ", err )
    return err;
    */    
    }

// ---------------------------------------------------------------------------
// CMccInterface::ReferenceCount
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CMccInterface::ReferenceCount( TUint32 aSessionId, TUint32 aLinkId, 
                              TUint32 /*aStreamId*/, TUint32 /*aEndpointId*/,
                              TUint& aCount )
    {
    __INTERFACE( "MccInterface::ReferenceCount" )
    
    CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    TInt err = ctrl->iLeaveError;
    
    if ( !ctrl->iRefCount )
        {
        CMccControllerStub::TLink link;
        link.iSessionID = aSessionId;
        link.iLinkID = aLinkId;
        CMccControllerStub::TLink& l = ctrl->GetLink( link );
        aCount = l.iStreams;
        }
    else
        {
        aCount = ctrl->iRefCount;
        }
    
    /*    
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
    genMessage.iEndpointID = aEndpointId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = ctrl->CustomCommandSync( iMessageDest,
                                         EMccGetReferenceCount,
                                         msgBuffer,
                                         KNullDesC8,
                                         msgBuffer );
    
    if ( !err )
        {
        aCount = msgBuffer().iParam4;
        __INTERFACE_INT1( "MccInterface::ReferenceCount, refcount:", aCount )
        }
        
    __INTERFACE_INT1( "MccInterface::ReferenceCount, exit with ", err )
    */
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::GetParameter
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::GetParameter( TUint32 aSessionId, TUint32 aLinkId, 
    TUint32 aStreamId, TUint32 aEndpointId,
    TUint32 aParam, TDes8& aVal )
	{
	__INTERFACE( "MccInterface::GetParameter" )
	
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
	
	if ( aParam == KMccConfigKey )
	    {
	    _LIT8( value, "configKey" );
	    aVal.Copy( value );
	    }
	
    return ctrl->ControllerStreamSettings( aSessionId,
    									   aLinkId,
    									   aStreamId,
    									   aEndpointId,
    									   aParam,
    									   EFalse );
	}

// ---------------------------------------------------------------------------
// CMccInterface::SetParameter
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::SetParameter( TUint32 aSessionId, TUint32 aLinkId, 
    TUint32 aStreamId, TUint32 aEndpointId,
    TUint32 aParam, const TDesC8& /*aVal*/ )
	{
	__INTERFACE( "MccInterface::SetParameter" )
	
	CMccControllerStub* ctrl = CMccControllerStub::Stub();
	if ( !ctrl )
		{
		return KErrNotFound;
		}
    return ctrl->ControllerStreamSettings( aSessionId,
    									   aLinkId,
    									   aStreamId,
    									   aEndpointId,
    									   aParam,
    									   ETrue );
	}

	
// ================= OTHER EXPORTED FUNCTIONS ==============

// -----------------------------------------------------------------------------
// E32Dll.
// DLL Entry point
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif


//  End of File
