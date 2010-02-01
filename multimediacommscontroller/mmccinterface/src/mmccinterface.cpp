/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include <ecom/ecom.h>
#include <mmf/common/mmfbase.h>
#include <ecam.h>

#include "mmccinterface.h"
#include "mmcctranscoder.h"

#include "mccuids.hrh"
#include "mccinternalcodecs.h"
#include "mmccnetworksettings.h"
#include "mccdef.h"       
#include "mmccevent.h"
#include "mmcccodecamr.h"
#include "mmcccodecdtmf.h"
#include "mmcccodecinformation.h"
#include "mmcccodecinformationfactory.h"
#include "mmccevents.h"
#include "mmccinterfacelogs.h"
#include "mmcccodech263.h"

const TUid KImplementMccController  = { KImplUidMccController };

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
CMccInterface::CMccInterface() : 
    iMessageDest( KMccControllerUidInterface ),
    iAudioPlayDeviceCustomCommands( iController ),
    iAudioRecordDeviceCustomCommands( iController )
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
   
    // Open Controller
    OpenControllerL( KImplementMccController );

    iMccEvent = CMccEvent::NewL( iController, aObserver );
    iMccEvent->RequestEventNotification();

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
    delete iMccEvent;
    
    iController.Close();
    iCodecInformation.ResetAndDestroy();
    iCodecInformation.Close();
    
    iEndpointInfos.Reset();
    iEndpointInfos.Close();
    
    REComSession::FinalClose();
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
    CMccCodecInformationFactory* codecFactory = CMccCodecInformationFactory::NewL();
    CleanupStack::PushL( codecFactory );
    
    GetSupportedCodecs( codecs );
    
    TFourCC fourccValue;
    
    TInt codecCount = codecs.Count();
    __INTERFACE_INT1( "MccInterface::CapabilitiesL, count of found codecs:", codecCount )
    for ( TInt k = 0; k < codecCount; k++ )
        {
        fourccValue = codecs[k];
        
        CMccCodecInformation* codec = codecFactory->CreateCodecInformationL( fourccValue );
        if ( codec )
            {
            CleanupStack::PushL( codec );
            __INTERFACE_STR8( "MccInterface::CapabilitiesL, appending:", codec->SdpName() )
            iCodecInformation.AppendL( codec );            
            CleanupStack::Pop( codec );
            }
        
        if ( fourccValue == KMccFourCCIdG711 )
            {
            codec = codecFactory->CreateCodecInformationL( fourccValue );
            CleanupStack::PushL( codec );
            if ( 0 == codec->SdpName().CompareF( KPCMUSdpName ) )
                {
                codec->SetSdpName( KPCMASdpName );
                }
            else
                {
                codec->SetSdpName( KPCMUSdpName );
                }
            
            __INTERFACE_STR8( "MccInterface::CapabilitiesL, appending:", codec->SdpName() )
            iCodecInformation.AppendL( codec );
            CleanupStack::Pop( codec );
            }
            
        if ( fourccValue == KMccFourCCIdH263 )
            {
            codec = codecFactory->CreateCodecInformationL( fourccValue );
            CleanupStack::PushL( codec );
            if ( 0 == codec->SdpName().CompareF( KH2632000SdpName ) )
                {
                // Changes also sdpname
                codec->SetCodecMode( KH263FormatModeOld );
                }
            else
                {
                // Changes also sdpname
                codec->SetCodecMode( KH263FormatModeNew );
                }
            
            __INTERFACE_STR8( "MccInterface::CapabilitiesL, appending:", codec->SdpName() )
            iCodecInformation.AppendL( codec );
            CleanupStack::Pop( codec );
            }
        }
    
    codecs.Close();
    CleanupStack::PopAndDestroy( codecFactory );
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
    TInt err = KErrNone;
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
    
    __INTERFACE_INT1( "MccInterface::GetSupportedCodecs, exit with ", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CMccInterface::OpenControllerL()
// Opens Mcc Controller
// -----------------------------------------------------------------------------
//
void CMccInterface::OpenControllerL( const TUid& aControllerUid )
    {
    __INTERFACE( "MccInterface::OpenControllerL" )
    TInt error = KErrNone;
    TMMFPrioritySettings settings;
    settings.iPriority = EPriorityNormal;
    
    CMMFControllerImplementationInformation* implInfo = 
        CMMFControllerImplementationInformation::NewL( aControllerUid );
        
    CleanupStack::PushL( implInfo );

    __INTERFACE_INT1( "MccInterface::OpenController, heapsize:", 
                      implInfo->HeapSpaceRequired() )
    
    error = iController.Open( *implInfo, settings );
    if ( error )
        {
        iController.Close();
        __INTERFACE_INT1( "MccInterface::OpenController, exit with", error)
        User::Leave( error );
        }
        
    CleanupStack::PopAndDestroy( implInfo );

    __INTERFACE_INT1( "MccInterface::OpenController, exit with", error)
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::SetRemoteAddress
// Set remote IP address and port number for a given RTP session.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::SetRemoteAddress( TUint32 aSessionId, 
                                               TUint32 aLinkId,
                                               const TInetAddr& aRemoteAddr,
                                               TUint aRemoteRtcpPort )
    {
    __INTERFACE( "MccInterface::SetRemoteAddress" )
    TInt err = KErrNone;
    TMccAddress addr;
    addr.iSessionID = aSessionId;
    addr.iLinkID = aLinkId;
    addr.iAddress = aRemoteAddr;
    TMccAddressPckg package( addr );
	if ( aRemoteRtcpPort == 0 )
		{
		err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetRemoteAddress,
                                         package,
                                         KNullDesC8 );

		}
	if ( aRemoteRtcpPort != 0)
		{
		 __INTERFACE_INT1( "RtcpPort = ", aRemoteRtcpPort )
   
   		err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetRemoteRtcpAddr,
                                         package,
                                         KNullDesC8 );
		}
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
                                         TMccNetSettings& aNetSettings )
    {    
    __INTERFACE( "MccInterface::CreateLink")
    
    TMccCreateLink pktLink;
    pktLink.iSessionID = aSessionId;
    pktLink.iLinkType = aLinkType;
    pktLink.iIapId = aNetSettings.iIapId;
    pktLink.iRemoteAddress = aNetSettings.iRemoteAddress;
    pktLink.iLocalAddress.SetPort( aNetSettings.iLocalAddress.Port() );
    pktLink.iIpTOS = aNetSettings.iMediaQosValue;
    TMccCreateLinkPckg package( pktLink );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccCreateLink,
                                                 package,
                                                 KNullDesC8,
                                                 package );

    if ( KErrNone == result )
        {
        aLinkId = package().iLinkID;
        }

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
    __INTERFACE( "MccInterface::CloseSession" )
    TInt err = KErrNone;
    TMccSession sessID;
    sessID.iSessionID = aSessionId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccCloseSession,
                                         package,
                                         KNullDesC8 );
                                         
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
    __INTERFACE( "MccInterface::CloseLink" )
    
    TInt err = KErrNone;
    TMccGenericMessage message;
    message.iSessionID = aSessionId;
    message.iLinkID = aLinkId;
    TMccGenericMsgBuffer package( message );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccCloseLink,
                                         package,
                                         KNullDesC8 );
                                         
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

    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    TMccCreateStreamParam message;
    message.iPayloadType = aCodecInformation.PayloadType();
    message.iFourCC = aCodecInformation.FourCC();
    message.iStreamType = aStreamType;
    message.iPrioritySettings.iPriority = aCodecInformation.Priority();
    message.iPrioritySettings.iPref =
    TMdaPriorityPreference( aCodecInformation.PriorityPreference() );
    message.iStandByTimer = aCodecInformation.JitterBufInactivityTimeOut();
    TMccCreateStreamParamPckg messagePckg( message );
     
    /* Call the controller */
    TInt err = iController.CustomCommandSync( iMessageDest, EMccCreateStream,
                                         msgBuffer, messagePckg,
                                         msgBuffer );

    if( err != KErrNone )
        {
        return err;
        }

    aStreamId = msgBuffer().iStreamID;
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
    __INTERFACE( "MccInterface::DeleteStream" )
    TInt err = KErrNone;
    TMccSession sessID;
    sessID.iSessionID = aSessionId;
    sessID.iLinkID = aLinkId;
    sessID.iStreamID = aStreamId;
    TMccSessionPckg package( sessID );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccDeleteStream,
                                         package,
                                         KNullDesC8 );

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
                                                   TUint32 aTimeoutTime,
                                                   TUint32 /*aEndpointId*/ )
    {
    __INTERFACE( "MccInterface::StartInactivityTimer" )
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
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
    genMessage.iEndpointID = aEndpointId;
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccInactivityTimerStop,
                                              msgBuffer,
                                              KNullDesC8 );

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
    __INTERFACE( "MccInterface::PrepareStream" )
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
    __INTERFACE( "MccInterface::StartStream" )
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
    __INTERFACE( "MccInterface::PauseStream" )
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
    __INTERFACE( "MccInterface::ResumeStream" )
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
    __INTERFACE( "MccInterface::StopStream" )
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


    __INTERFACE_INT1( "MccInterface::StopStream, exit with ", err )
    return err;
    }

// -----------------------------------------------------------------------------
// CMccInterfaceImpl::GetCodecL()
// Gets current codec used in a given stream.
// -----------------------------------------------------------------------------
//
EXPORT_C CMccCodecInformation* CMccInterface::CodecL( TUint32 aSessionId,
                                                      TUint32 aLinkId,
                                                      TUint32 aStreamId )
    {
    __INTERFACE( "MccInterface::GetCodecL" )    
    TMccGenericMessage msg;
    msg.iSessionID = aSessionId;
    msg.iLinkID = aLinkId;
    msg.iStreamID = aStreamId;
    TMccGenericMsgBuffer msgBuffer( msg );
    
    TMccCodecInfo codecInfo;
    TMccCodecInfoBuffer codecBuffer( codecInfo );
    
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetCodec, msgBuffer,
                                         KNullDesC8, codecBuffer );
    
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
        
    // Now we need to find the correspoding codec from iCodecArray & clone it.
    // Then we need to set the parameters returned from controller which are in codecInfo
    // to that cloned codec.
    TInt count = iCodecInformation.Count();
    TInt index = KErrNotFound;
    TInt counter = 0;
    
    // If fourcc match, we've got the right one.
    TFourCC fourcc;
    while ( counter < count )
        {
        fourcc = iCodecInformation[counter]->FourCC();
        if( fourcc == codecBuffer().iFourCC )
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
    retCodec->SetValues( codecBuffer() );
    retCodec->CreateFmtpAttrListL();
            
    __INTERFACE( "MccInterface::GetCodecL, exit" )    
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
    TMccCodecInfo codecInfo;
    
    aCodecInformation.GetValues( codecInfo );         
    codecInfo.iSessionID = aSessionId;
    codecInfo.iLinkID    = aLinkId;
    codecInfo.iStreamID  = aStreamId;
    TMccCodecInfoBuffer infoBuffer( codecInfo );
    
    __INTERFACE_INT1( "MccInterface SessionID", codecInfo.iSessionID )     
    __INTERFACE_INT1( "MccInterface LinkID", codecInfo.iLinkID )     
    __INTERFACE_INT1( "MccInterface StreamID", codecInfo.iStreamID )     
    __INTERFACE_INT1( "MccInterface FourCC", codecInfo.iFourCC.FourCC() )      
    __INTERFACE_INT1( "MccInterface AlgoUsed", codecInfo.iAlgoUsed )      
    __INTERFACE_INT1( "MccInterface Bitrate", codecInfo.iBitrate )      
    __INTERFACE_INT1( "MccInterface CodecMode", codecInfo.iCodecMode )      
    __INTERFACE_INT1( "MccInterface EnableDTX", codecInfo.iEnableDTX )      
    __INTERFACE_INT1( "MccInterface MaxPtime", codecInfo.iMaxPtime )      
    __INTERFACE_INT1( "MccInterface Ptime", codecInfo.iPtime )      
    __INTERFACE_INT1( "MccInterface PayloadType", codecInfo.iPayloadType )      
    __INTERFACE_INT1( "MccInterface RedundantPayload", 
                      codecInfo.iRedundantPayload )      
    __INTERFACE_INT1( "MccInterface RedundancyCount", codecInfo.iRedundancyCount )      
    __INTERFACE_INT1( "MccInterface FrameSize", codecInfo.iFrameSize )      
    __INTERFACE_INT1( "MccInterface HwFrameTime", codecInfo.iHwFrameTime )      
    __INTERFACE_INT1( "MccInterface BitrateMask", codecInfo.iBitrateMask )      
    __INTERFACE_INT1( "MccInterface AverageBitrate", codecInfo.iAverageBitrate )      
    __INTERFACE_INT1( "MccInterface Framerate", (TInt) codecInfo.iFramerate )
    __INTERFACE_INT1( "MccInterface iComfortNoiseGenerationPt", 
                      codecInfo.iComfortNoiseGenerationPt )      
    __INTERFACE_INT1( "MccInterface VideoWidth", codecInfo.iVideoWidth )     
    __INTERFACE_INT1( "MccInterface VideoHeight", codecInfo.iVideoHeight )     
    __INTERFACE_INT1( "MccInterface JitterBufInactivityTimeOut", 
                      codecInfo.iJitterBufInactivityTimeOut )     
    __INTERFACE_INT1( "MccInterface JitterBufThreshold", 
                      codecInfo.iJitterBufThreshold )     
    __INTERFACE_INT1( "MccInterface JitterBufBufferLength", 
                      codecInfo.iJitterBufBufferLength )     
    __INTERFACE_INT1( "MccInterface JitterBufPlayToneTimeout", 
                      codecInfo.iJitterBufPlayToneTimeout )     
    __INTERFACE_INT1( "MccInterface JitterBufPlayToneFrequency", 
                      codecInfo.iJitterBufPlayToneFrequency )     
    __INTERFACE_INT1( "MccInterface JitterBufPlayToneDuration", 
                      codecInfo.iJitterBufPlayToneDuration )     
    __INTERFACE_INT1( "MccInterface Priority", codecInfo.iPriority.iPriority )     
    __INTERFACE_INT1( "MccInterface Preference", codecInfo.iPriority.iPref )
    __INTERFACE_INT1( "MccInterface iModeChangePeriod", 
                      codecInfo.iModeChangePeriod )     
    __INTERFACE_INT1( "MccInterface iNeighbor", codecInfo.iNeighbor )        
    __INTERFACE_INT1( "MccInterface Keepalive interval", 
                      codecInfo.iKeepaliveInterval )
    __INTERFACE_INT1( "MccInterface Keepalive payloadtype", 
                      codecInfo.iKeepalivePT )   
    __INTERFACE_INT1( "MccInterface Keepalive data len", 
                      codecInfo.iKeepaliveData.Length() )   
    __INTERFACE_INT1( "MccInterface MaxRed", codecInfo.iMaxRed )  
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCodec,
                                         infoBuffer,
                                         KNullDesC8,
                                         infoBuffer );
    
    // Return payloadtype allocated
    aCodecInformation.SetPayloadType( infoBuffer().iPayloadType );
    
    __INTERFACE_INT1( "MccInterface Final PayloadType",
        aCodecInformation.PayloadType() )
    __INTERFACE_INT1( "MccInterface::SetCodec err",
        err )
    
    return err;    
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
// CMccInterface::SendMediaSignalL
// Sends media signalling to uplink if possible
// ---------------------------------------------------------------------------
//
EXPORT_C void CMccInterface::SendMediaSignalL( const TMccEvent& aEvent )
    {
    __INTERFACE( "MccInterface::SendMediaSignalL" )      
    TMccEventPackage pack( aEvent );
    TInt err = iController.CustomCommandSync( iMessageDest, 
                                              EMccSendMediaSignal,
                                              pack, KNullDesC8 );
    User::LeaveIfError( err );
    }
/*
// ---------------------------------------------------------------------------
// CMccInterface::GetCodecDefaultsL
// ---------------------------------------------------------------------------
//
EXPORT_C CMccCodecInformation* CMccInterface::GetCodecDefaultsL(
    const TUid aType, TInt aStreamType, const TDesC8& aParam )
    {
    __INTERFACE( "MccInterface::GetCodecDefaultsL" )      
    TMccCodecInfo codecInfo;
    
    TMccGenericMessage msg;
    msg.iSessionID = 0;
    msg.iLinkID = 0;
    msg.iStreamID = aType.iUid;
    msg.iParam1 = aParam;
    TMccGenericMsgBuffer msgBuffer( msg );

    if (aStreamType == EMccAudioUplinkStream ||
        aStreamType == EMccAudioDownlinkStream ||
        aStreamType == KMccAudioLocalStream )
        {
        codecInfo.iType = KUidMediaTypeAudio;
        }
    else
        {
        codecInfo.iType = KUidMediaTypeVideo;        
        }    
        
    TMccCodecInfoBuffer codecBuffer( codecInfo );
    
    User::LeaveIfError(iController.CustomCommandSync( iMessageDest, EMccGetCodec, msgBuffer,
                                         KNullDesC8, codecBuffer ) );
    
    // Now we need to find the correspoding codec from iCodecArray & clone it.
    // Then we need to set the parameters returned from controller which are in codecInfo
    // to that cloned codec.
    TInt count = iCodecInformation.Count();
    TInt index = KErrNotFound;
    TInt counter = 0;
    
    // If fourcc match, we've got the right one.
    TFourCC fourcc;
    while ( counter < count )
        {
        fourcc = iCodecInformation[counter]->FourCC();
        if( fourcc == codecBuffer().iFourCC )
            {
            index = counter;
            counter = count;
            }        
        counter++;
        }
    
    // See if we've found the correct codec...
    if( index == KErrNotFound )
        {
        User::Leave( KErrNotFound );
        }

    CMccCodecInformation* retCodec = iCodecInformation[index]->CloneDetailedL();
    retCodec->SetValues( codecBuffer() );
    retCodec->CreateFmtpAttrListL();                
    return retCodec;    
    }
*/

// ---------------------------------------------------------------------------
// CMccInterface::GetDefaultCapabilities
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::GetCapabilities( const TUid /*aType*/, TInt /*aStreamType*/,
    const TDesC8& /*aParam*/, RPointerArray<CMccCodecInformation>& /*aCapabilities*/ )
    {
    return KErrNotSupported;
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
    TMccGenericMessage message;
    message.iSessionID = aSessionId;
    message.iLinkID    = aLinkId;
    message.iStreamID  = aStreamId;
    message.iEndpointID  = aEndpointId;
    TMccGenericMsgBuffer messagePckg( message );
    
    // payload contains SSRC info
    TMccSSRC     ssrc;
    TMccSSRCPckg ssrcPckg ( ssrc );
    
    TInt err = iController.CustomCommandSync( iMessageDest, 
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
    return err;
    }
        
// ---------------------------------------------------------------------------
// CMccInterface::AddDataSink
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::AddDataSink( const TUid aSinkType, 
    const TDesC8& aParam, TUint32& aSinkSourceId )
    {
    __INTERFACE_INT1( "MccInterface::AddDataSink, type", aSinkType.iUid )
    __INTERFACE_INT1( "MccInterface::AddDataSink, initdata len:", aParam.Length() )
        
    TInt err = KErrNone;
    TMMFMessageDestination handleInfo;
    err = iController.AddDataSink( aSinkType, aParam, handleInfo );
    if ( err == KErrNone )
        {
        TMccSession sessID;
        TMccSessionPckg package( sessID );

        err = iController.CustomCommandSync( iMessageDest,
                                             EMccGetSinkId,
                                             package,
                                             KNullDesC8,
                                             package );
        
        if ( !err )
            {
            aSinkSourceId = package().iEndpointID;
            
            StoreEndpointInfo( aSinkSourceId, handleInfo );
            }
        }

    __INTERFACE_INT1( "MccInterface::AddDataSink, exit with", err)      
    return err;        
    }

// ---------------------------------------------------------------------------
// CMccInterface::AddDataSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::AddDataSource( const TUid aSourceType,
    const TDesC8& aParam, TUint32& aSinkSourceId )
    {
    __INTERFACE_INT1( "MccInterface::AddDataSource, type", aSourceType.iUid )
    __INTERFACE_INT1( "MccInterface::AddDataSource, initdata len:", aParam.Length() )
    
    TInt err = KErrNone;
    TMMFMessageDestination handleInfo;
    err = iController.AddDataSource( aSourceType, aParam, handleInfo );        
    if ( err == KErrNone )
        {
        TMccSession sessID;
        TMccSessionPckg package( sessID );

        err = iController.CustomCommandSync( iMessageDest,
                                             EMccGetSourceId,
                                             package,
                                             KNullDesC8,
                                             package );
        
        if ( !err )
            {
            aSinkSourceId = package().iEndpointID;
            
            StoreEndpointInfo( aSinkSourceId, handleInfo );
            }
        }

    __INTERFACE_INT1( "MccInterface::AddDataSource, exit with", err)      
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
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    TMccGenericMsgBuffer messagePckg( genMessage );

    TInt result = iController.CustomCommandSync( iMessageDest,
                                                 EMccStreamsExists,
                                                 messagePckg,
                                                 KNullDesC8 );

    TBool ret = EFalse;
    if ( result == KErrNone )
        {
        ret = ETrue;
        }

    __INTERFACE_INT1( "MccInterface::StreamsExists, exit with ", ret )
    return ret;
    }

// ---------------------------------------------------------------------------
// CMccInterface::UpdateDataSink
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::UpdateDataSink( TUint32 aSessionId, TUint32 aLinkId, 
                             TUint32 aStreamId, TUint32 aEndpointId,
                             const TDesC8& aParam )
    {
    __INTERFACE( "MccInterface::UpdateDataSink" )
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
    }
        
// ---------------------------------------------------------------------------
// CMccInterface::UpdateDataSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::UpdateDataSource( TUint32 aSessionId, TUint32 aLinkId, 
                           TUint32 aStreamId, TUint32 aEndpointId,
                           const TDesC8& aParam )
    {
    __INTERFACE( "MccInterface::UpdateDataSource" )
    TInt err = KErrNone;
    TMccStream params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iEndpointID = aEndpointId;
    TMccStreamPckg package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccUpdateDataSource,
                                         package,
                                         aParam );
                                         
    __INTERFACE_INT1( "MccInterface::UpdateDataSource, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::RemoveDataSink
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CMccInterface::RemoveDataSink( 
    TUint32 /*aSessionId*/, 
    TUint32 /*aLinkId*/, 
    TUint32 /*aStreamId*/, 
    TUint32 aEndpointId )
    {
    TInt err = KErrNone;
    
    TMMFMessageDestination sourceHandleInfo = FindEndpointInfo( aEndpointId, err );
    if ( !err )
        {
        err = iController.RemoveDataSink( sourceHandleInfo );
        RemoveEndpointInfo( aEndpointId );
        }
        
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::RemoveDataSource
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::RemoveDataSource( 
    TUint32 /*aSessionId*/, 
    TUint32 /*aLinkId*/, 
    TUint32 /*aStreamId*/, 
    TUint32 aEndpointId )
    {
    TInt err = KErrNone;
    
    TMMFMessageDestination sinkHandleInfo = FindEndpointInfo( aEndpointId, err );
    if ( !err )
        {
        err = iController.RemoveDataSource( sinkHandleInfo );
        RemoveEndpointInfo( aEndpointId );
        }
        
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
    }

// ---------------------------------------------------------------------------
// CMccInterface::ReferenceCount
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CMccInterface::ReferenceCount( 
    TUint32 aSessionId, 
    TUint32 aLinkId, 
    TUint32 aStreamId, 
    TUint32 aEndpointId,
    TUint& aCount )
    {
    __INTERFACE( "MccInterface::ReferenceCount" )
    
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
    genMessage.iEndpointID = aEndpointId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
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
    
    TInt retVal( KErrNone );
    
    switch( aParam )
        {
        case KMccCamZoomFactor:
        case KMccCamDigitZoomFactor:
        case KMccCamContrast:
        case KMccCamBrightness:
        case KMccCamWhiteBalance:
        case KMccCamExposure:
            {
            TPckgBuf<TInt> buffer;
            retVal = GetCameraParameter( aParam, buffer() );
            aVal = buffer;        
            break;
            }   
        case KMccCamInfo:
            {
            TMccVideoInfoBuf2 buffer;
            buffer.Copy( aVal );
            
            TRAP( retVal, buffer().iInfo = 
                GetCameraInfoL( buffer().iCameraIndex ) );
            aVal = buffer;        
            break;                
            }
        case KMccCamCount:
            {
            TPckgBuf<TInt> buffer;
            TRAP( retVal, buffer() = GetNumOfCamerasL() );
            aVal = buffer;        
            break;                
            }
	    case KMccCamLocalVideo:
		    {
		    retVal = GetViewFinderSettings( aParam, aEndpointId, aVal );
		    break;
		    }
        case KMccMicGain:
            {
            TPckgBuf<TInt> buffer;
            retVal = GetGain( buffer() );
            aVal = buffer;        
            break;                
            }
        case KMccMicMaxGain:
            {
            TPckgBuf<TInt> buffer;
            retVal = MaxGain( aSessionId, aLinkId, aStreamId, buffer() );
            aVal = buffer;        
            break;                
            }
        case KMccMicBalance:
            {
            break;
            }
        case KMccSpeakerVolume:
            {
            TPckgBuf<TInt> buffer;
            retVal = Volume( buffer() );
            aVal = buffer;        
            break;                
            }
        case KMccSpeakerMaxVolume:
            {
            TPckgBuf<TInt> buffer;
            retVal = MaxVolume( aSessionId, aLinkId, aStreamId, buffer() );
            aVal = buffer;        
            break;                
            }
        case KMccSpeakerBalance:
            {
            break;
            }
        case KMccSpeakerRouting:
            {
            TPckgBuf<TUint32> buffer;
            retVal = AudioRouting( aSessionId, aLinkId, aStreamId, buffer() );
            aVal = buffer;                
            break;
            }
        case KMccFileInfo:
        case KMccFileName:
        case KMccFileDuration:
        case KMccFilePosition:
        case KMccFileVideoFrameRate:
        case KMccFileVideoBitRate:
        case KMccFileVideoFrameSize:
        case KMccFileAudioBitRate:
        case KMccFileAudioFourCC:
        case KMccFileVideoFourCC:
        case KMccFileFastForward:
		case KMccDisplayLocalVideo:
        case KMccConfigKey:
            {
    	    retVal = GetEndpointParameter( aParam, 
    	                                   aSessionId,
    	                                   aLinkId,
    	                                   aStreamId,
    	                                   aEndpointId, 
    	                                   aVal );
            break; 
            }
        default:
            {
            retVal = KErrArgument;
            break;
            }
        }
    __INTERFACE( "MccInterface::GetParameter, exit" )
    return retVal;    
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::SetParameter
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::SetParameter( TUint32 aSessionId, TUint32 aLinkId, 
    TUint32 aStreamId, TUint32 aEndpointId,
    TUint32 aParam, const TDesC8& aVal )
    {
    TInt retVal( KErrNone );

    switch( aParam )
        {
        case KMccCamZoomFactor:
        case KMccCamDigitZoomFactor:
        case KMccCamContrast:
        case KMccCamBrightness:
        case KMccCamWhiteBalance:
        case KMccCamExposure:
            {
            TPckgBuf<TInt> buffer;
            buffer.Copy( aVal );
            retVal = SetCameraParameter( aParam, buffer() );
            break;    
            }
        case KMccCamLocalVideo:
            {
            if ( aVal.Length() )
                {
                retVal = EnableViewFinder( aVal );                
                }
            else    
                {
                retVal = DisableViewFinder();                
                }
            break;
            }
        case KMccMicGain:
            {
            TPckgBuf<TInt> buffer;
            buffer.Copy( aVal );        
            retVal = SetGain( buffer() );
            break;                
            }
        case KMccMicBalance:
        break;
        case KMccSpeakerVolume:
            {
            TPckgBuf<TInt> buffer;
            buffer.Copy( aVal );        
            retVal = SetVolume( buffer() );
            break;
            }
        case KMccSpeakerBalance:
        break;
        case KMccSpeakerRouting:
            {
            TPckgBuf<TInt> buffer;
            buffer.Copy( aVal );        
            retVal = SetAudioRouting( aSessionId, aLinkId, aStreamId, buffer() );
            break;
            }
        case KMccFileInfo:
        case KMccFileName:
        case KMccFileDuration:
        case KMccFilePosition:
        case KMccFileVideoFrameRate:
        case KMccFileVideoBitRate:
        case KMccFileVideoFrameSize:
        case KMccFileAudioBitRate:
        case KMccFileAudioFourCC:
        case KMccFileVideoFourCC:
        case KMccFileFastForward:
	    case KMccDisplayLocalVideo:
		case KMccRtpCName:
        case KMccConfigKey:
    	    {
    	    retVal = SetEndpointParameter( aParam, 
    	                                   aSessionId,
    	                                   aLinkId, 
    	                                   aStreamId, 
    	                                   aEndpointId, 
    	                                   aVal );
            break; 
            }
        default:
            {
            retVal = KErrArgument;   
            break;
            }        
        }
    return retVal;    
    }

// ---------------------------------------------------------------------------
// CMccInterface::AddBundle
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::AddBundle( TUint32 /*aSessionId*/, 
    TMccStreamBundle& /*aBundle*/, TUint32& /*aBundleId*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMccInterface::RemoveBundle
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::RemoveBundle( TUint32 /*aSessionId*/, TUint32 /*aBundleId*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMccInterface::GetBundles
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::GetBundles( TUint32 /*aSessionId*/,
    RArray<TMccStreamBundle>& /*aBundles*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMccInterface::TranscodeFile
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::TranscodeFile( 
    const TDesC8& aSourceFile, 
    const TDesC8& aDestFile,
    TUint32 aQuality, 
    TUint32& aSessionId,  
    const CMccCodecInformation* aVideoCodec,
    const CMccCodecInformation* aAudioCodec )
    {
    __INTERFACE( "MccInterface::TranscodeFile" )
    
    TMccCreateSession msg;
    TMccCreateSessionPckg msgPkg( msg );
        
    TMccTranscodeFileMsg fileMsg;
    TInt cnvRet = CnvUtfConverter::ConvertToUnicodeFromUtf8( fileMsg.iSourceFile, aSourceFile );
    TInt cnvRet2 = CnvUtfConverter::ConvertToUnicodeFromUtf8( fileMsg.iDesFile, aDestFile );
    
    if ( cnvRet != 0 || cnvRet2 != 0 )
        {
        __INTERFACE_INT2( "MccInterface::TranscodeFile, file name conversion error [src,dest]:", 
                          cnvRet, cnvRet2 )
        return KErrGeneral;
        }
    
    fileMsg.iQuality = aQuality;
    
    if ( aVideoCodec )
        {
        fileMsg.iVideoCodec.iBitrate = aVideoCodec->Bitrate();
        fileMsg.iVideoCodec.iFramerate = aVideoCodec->Framerate();
        fileMsg.iVideoCodec.iSamplingFreq = aVideoCodec->SamplingFreq();
        fileMsg.iVideoCodec.iVideoWidth = aVideoCodec->FrameWidth();
        fileMsg.iVideoCodec.iVideoHeight = aVideoCodec->FrameHeight();
        fileMsg.iVideoCodec.iMimeType = aVideoCodec->SdpName();
        TUid videoCodecFourCC;
        videoCodecFourCC.iUid = aVideoCodec->FourCC();
        fileMsg.iVideoCodec.iFourCC = videoCodecFourCC;
        }
    if ( aAudioCodec )
        {
        fileMsg.iAudioCodec.iBitrate = aAudioCodec->Bitrate();
        fileMsg.iAudioCodec.iFramerate = aAudioCodec->Framerate();
        fileMsg.iAudioCodec.iSamplingFreq = aAudioCodec->SamplingFreq();
        fileMsg.iAudioCodec.iVideoWidth = aAudioCodec->FrameWidth();
        fileMsg.iAudioCodec.iVideoHeight = aAudioCodec->FrameHeight();
        fileMsg.iAudioCodec.iMimeType = aAudioCodec->SdpName();
        TUid audioCodecFourCC;
        audioCodecFourCC.iUid = aAudioCodec->FourCC();
        fileMsg.iAudioCodec.iFourCC = audioCodecFourCC;
        }
        
    TMccTranscodeFileMsgBuffer fileMsgBuffer( fileMsg );
 
    TInt err = iController.CustomCommandSync( iMessageDest, 
                                              EMccTranscodeFile,
                                              fileMsgBuffer, 
                                              KNullDesC8,
                                              msgPkg );
    aSessionId = msgPkg().iSessionID;                                             
                                              
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

    TMccCreateSession msg;
    msg.iSessionID = aSessionId;
    TMccCreateSessionPckg msgPkg( msg );
      
    TInt err = iController.CustomCommandSync( iMessageDest, 
                                              EMccCancelTranscodeFile,
                                              msgPkg, 
                                              KNullDesC8);
                                              
      __INTERFACE_INT1( "MccInterface::CancelTranscodeFile, exit with ", err )                                     
      
      return err;
    }

// -----------------------------------------------------------------------------
// CMccInterface::MediaQualityRequest
// Request media quality reports
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::MediaQualityRequest( 
    const TMccMediaQualityConfig& aConfig )
    {
    __INTERFACE( "MccInterface::MediaQualityRequest in" )

    TInt error( KErrNone );

    if( KErrNone != aConfig.CheckConfig() )
        {
        #ifdef TRACE_MCP_INTERFACE
            RDebug::Print( _L("CMccInterface::MediaQualityRequest invalid config!") );
        #endif

        error = KErrArgument;
        }
    else
        {
        TMMFMessageDestination dest;
        dest = FindEndpointInfo( aConfig.iEndpointId, error );

        if ( error )
            {
            __INTERFACE_INT1( "MccInterface::MediaQualityRequest, endpoint not found ", error )
            }
        else
            {
            TMMFMessageDestinationPckg destPckg( dest );
            TMccMediaQualConfBuf confBuf( aConfig );

            error = iController.CustomCommandSync( destPckg,
                                           EStartMediaQualityObserving,
                                           confBuf,
                                           KNullDesC8 );
                                           
            }
        }

    __INTERFACE( "MccInterface::MediaQualityRequest out" )
    return error;
    }

// -----------------------------------------------------------------------------
// CMccInterface::CancelMediaQualityRequest
// Cancels an outstanding media quality request
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMccInterface::CancelMediaQualityRequest( 
        const TMccMediaQualityConfig& aConfig )
    {
    __INTERFACE( "MccInterface::CancelMediaQualityRequest in" )

    TInt error( KErrNone );

    if( KErrNone != aConfig.CheckConfig() )
        {
        #ifdef TRACE_MCP_INTERFACE
            RDebug::Print( _L("CMccInterface::MediaQualityRequest invalid config!") );
        #endif
        
        error = KErrArgument;
        }
    else
        {
        TMMFMessageDestination dest;
        dest = FindEndpointInfo( aConfig.iEndpointId, error );
    
        if ( error )
            {
            __INTERFACE_INT1( "MccInterface::MediaQualityRequest, endpoint not found ", error )
            }
        else
            {
            TMMFMessageDestinationPckg destPckg( dest );
            TMccMediaQualConfBuf confBuf( aConfig );
    
            error = iController.CustomCommandSync( destPckg, 
                                           ECancelMediaQualityObserving, 
                                           confBuf,
                                           KNullDesC8 );
            }
        }
        
    __INTERFACE( "MccInterface::CancelMediaQualityRequest out" )
    return error;
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
    TMccGenericMessage message;
    message.iSessionID = aSessionId;
    message.iLinkID = aLinkId;
    message.iStreamID = aStreamId;
    TMccGenericMsgBuffer messagePckg( message );

    TMccBitrates bitrates;
    TMccBitratesPckg bitratePckg( bitrates );

    TInt err = iController.CustomCommandSync( iMessageDest, 
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
    return err;
    }


// ---------------------------------------------------------------------------
// CMccInterface::SendRTCPReceiverReport
// Sends RTCP receiver report to uplink if possible.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SendRTCPReceiverReport( TUint32 aSessionId,
                                            TUint32 aLinkId,
                                            TUint32 aStreamId )
    {
    __INTERFACE( "MccInterface::SendRTCPReceiverReport" )
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSendRtcpRR,
                                         msgBuffer,
                                         KNullDesC8 );
    
    __INTERFACE_INT1( "MccInterface::SendRTCPReceiverReport, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SendRTCPSenderReport
// Sends RTCP sender report to uplink if possible.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SendRTCPSenderReport( TUint32 aSessionId,
                                          TUint32 aLinkId,
                                          TUint32 aStreamId )
    {
    __INTERFACE( "MccInterface::SendRTCPSenderReport" )
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSendRtcpSR,
                                         msgBuffer,
                                         KNullDesC8 );

    __INTERFACE_INT1( "MccInterface::SendRTCPSenderReport, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SendRTCPAnyData
// Sends Non-RTCP data to uplink if possible.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SendRTCPAnyData( TUint32 aSessionId,
                                              TUint32 aLinkId,
                                              TUint32 aStreamId,
                                              const TDesC8& aData )
    {
    __INTERFACE( "MccInterface::SendRTCPAnyData" )
    TMccGenericMessage genMessage;
    genMessage.iSessionID = aSessionId;
    genMessage.iLinkID = aLinkId;
    genMessage.iStreamID = aStreamId;
        
    TMccGenericMsgBuffer msgBuffer( genMessage );
    
    TInt err( KErrNone );
    
    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSendRtcpData,
                                         msgBuffer,
                                         aData );
                                         
    __INTERFACE_INT1( "MccInterface::SendRTCPAnyData, exit with ", err )
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::EnableViewFinder
// ---------------------------------------------------------------------------
//
TInt CMccInterface::EnableViewFinder( const TDesC8& aParam )
    {
    __INTERFACE( "MccInterface::EnableViewFinder" )
    return iController.CustomCommandSync( iMessageDest,
                                         EMccEnableViewFinder,
                                         aParam,
                                         KNullDesC8 );    
    }

// ---------------------------------------------------------------------------
// CMccInterface::DisableViewFinder
// ---------------------------------------------------------------------------
//
TInt CMccInterface::DisableViewFinder()
    {
    __INTERFACE( "MccInterface::DisableViewFinder" )
    return iController.CustomCommandSync( iMessageDest,
                                         EMccDisableViewFinder,
                                         KNullDesC8,
                                         KNullDesC8 );        
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::GetViewFinderSettings
// ---------------------------------------------------------------------------
//
TInt CMccInterface::GetViewFinderSettings( 
    TUint32 aParam, TUint32 aEndPointId, TDes8& aVal )
    {
    __INTERFACE( "MccInterface::GetViewFinderSettings")
    
    TMccGenericMessage message;
    message.iParam2 = aParam;
    message.iEndpointID = aEndPointId;
    TMccGenericMsgBuffer package( message );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetViewFinderSettings,
                                              package,
                                              aVal,
                                              aVal );
    
    __INTERFACE_INT1( "MccInterface::GetViewFinderSettings, exit with", err )
    return err;
    }
    
// -----------------------------------------------------------------------------
// CMccInterfaceImpl::GetCameraInfoL()
// Gets current codec used in a given stream.
// -----------------------------------------------------------------------------
//
TCameraInfo CMccInterface::GetCameraInfoL(TInt aDeviceIndex )
    {
    __INTERFACE( "MccInterface::GetCameraInfoL" )    
    TMccGenericMessage msg;
    msg.iParam2 = aDeviceIndex;
    TMccGenericMsgBuffer msgBuffer( msg );
    
    TCameraInfo camInfo;
    TPckgBuf<TCameraInfo> camBuffer( camInfo );
    
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetCamInfo, msgBuffer,
                                         KNullDesC8, camBuffer );
    
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
        
    __INTERFACE( "MccInterface::GetCameraInfoL, exit" )    
    return camBuffer();
    }


// -----------------------------------------------------------------------------
// CMccInterfaceImpl::GetNumOfCamerasL()
// Gets current codec used in a given stream.
// -----------------------------------------------------------------------------
//
TInt CMccInterface::GetNumOfCamerasL()
    {
    __INTERFACE( "MccInterface::GetNumOfCamerasL" )    
    TMccGenericMessage msg;
    TMccGenericMsgBuffer msgBuffer( msg );
    TPckgBuf<TInt> buffer;    
        
    TInt err = iController.CustomCommandSync( iMessageDest, EMccGetNumOfCams, msgBuffer,
                                         KNullDesC8, buffer );
    
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
        
    __INTERFACE( "MccInterface::GetNumOfCamerasL, exit" )    
    return buffer();
    }

// ---------------------------------------------------------------------------
// CMccInterface::GetSupportedBitrates
// Gets supported bit rates from the codec of the specified stream
// ---------------------------------------------------------------------------
//
TInt CMccInterface::GetCameraParameter(TUint32 aParam, TInt& aVal)
    {
    __INTERFACE( "MccInterface::GetCameraParameter (TUint32) ")

    TMccGenericMessage msg;
    msg.iMessageFunction = aParam;
    TMccGenericMsgBuffer package( msg );
        
    TInt err = iController.CustomCommandSync( 
        iMessageDest, EMccGetCamValue1, package, KNullDesC8, package );
    
    aVal = package().iParam4;
    
    __INTERFACE_INT1( "MccInterface::GetCameraParameter, exit with ", err )
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::GetSupportedBitrates
// Gets supported bit rates from the codec of the specified stream
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SetCameraParameter(TUint32 aParam, TInt aVal)
    {
    __INTERFACE( "MccInterface::SetCameraParameter (TUint32)")
    TMccGenericMessage message;
    message.iMessageFunction = aParam;
    message.iParam4 = aVal;
    TMccGenericMsgBuffer package( message );
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue1,
                                         package,
                                         KNullDesC8);
                                         
    __INTERFACE_INT1( "MccInterface::SetCameraParameter, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::GetSupportedBitrates
// Gets supported bit rates from the codec of the specified stream
// ---------------------------------------------------------------------------
//
TInt CMccInterface::GetCameraParameter(TUint32 aParam, TReal& aVal)
    {
    __INTERFACE( "MccInterface::GetCameraParameter (TReal)")

    TMccGenericMessage message;
    message.iMessageFunction = aParam;
    TMccGenericMsgBuffer package( message );
        
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccGetCamValue2,
                                         package,
                                         KNullDesC8,
                                         package );
                                         
    aVal = package().iParam3;

    __INTERFACE_INT1( "MccInterface::GetCameraParameter, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::GetSupportedBitrates
// Gets supported bit rates from the codec of the specified stream
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SetCameraParameter(TUint32 aParam, TReal aVal)
    {
    __INTERFACE( "MccInterface::SetCameraParameter (TReal)")
    TMccGenericMessage message;
    message.iMessageFunction = aParam;
    message.iParam3 = aVal;
    TMccGenericMsgBuffer package( message );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetCamValue2,
                                         package,
                                         KNullDesC8 );
                                         
    __INTERFACE_INT1( "MccInterface::SetCameraParameter, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SetEndpointParameter
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SetEndpointParameter( 
    TUint32 aParam, 
    TUint32 aSessionId,
    TUint32 aLinkId,
    TUint32 aStreamId,
    TUint32 aEndPointId,
    const TDesC8& aVal )
    {
    __INTERFACE( "MccInterface::SetEndpointParameter")
    TMccGenericMessage message;
    message.iParam2 = aParam;
    message.iSessionID = aSessionId;
    message.iLinkID = aLinkId;
    message.iStreamID = aStreamId;
    message.iEndpointID = aEndPointId;
    TMccGenericMsgBuffer package( message );                           
                                         
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccSetEndpointParameter,
                                              package,
                                              aVal );
                                         
    __INTERFACE_INT1( "MccInterface::SetEndpointParameter, exit with", err)
    return err;
    }
	
// ---------------------------------------------------------------------------
// CMccInterface::GetEndpointParameter
// ---------------------------------------------------------------------------
//
TInt CMccInterface::GetEndpointParameter( 
    TUint32 aParam, 
    TUint32 /*aSessionId*/,
    TUint32 /*aLinkId*/,
    TUint32 /*aStreamId*/,
    TUint32 aEndPointId, 
    TDes8& aVal )
    {
    __INTERFACE( "MccInterface::GetEndpointParameter")
    
    TMccGenericMessage message;
    message.iParam2 = aParam;
    message.iEndpointID = aEndPointId;
    TMccGenericMsgBuffer package( message );
    
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetEndpointParameter,
                                              package,
                                              aVal,
                                              aVal );
    
    __INTERFACE_INT1( "MccInterface::GetEndpointParameter, exit with", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SetAudioRouting
// ---------------------------------------------------------------------------
//    
TInt CMccInterface::SetAudioRouting( 
    TUint32 aSessionId, 
    TUint32 aLinkId, 
    TUint32 aStreamId,
    TUint32 aRoutingDestination )
    {
    __INTERFACE_INT1( "MccInterface::SetAudioRouting, routing to ", 
                      aRoutingDestination )
    
    TInt err = KErrNone;
    TMccGenericMessage params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    params.iParam2 = aRoutingDestination;
    TMccGenericMsgBuffer package( params );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccSetAudioRoute,
                                         package,
                                         KNullDesC8 );
                                         
    __INTERFACE_INT1( "MccInterface::SetAudioRouting, exit with ", err )
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::AudioRouting
// ---------------------------------------------------------------------------
//
TInt CMccInterface::AudioRouting( 
    TUint32 aSessionId, 
    TUint32 aLinkId, 
    TUint32 aStreamId, 
    TUint32& aRoutingDestination )
    {
    __INTERFACE( "MccInterface::AudioRouting")

    TMccGenericMessage params;
    params.iSessionID = aSessionId;
    params.iLinkID = aLinkId;
    params.iStreamID = aStreamId;
    TMccGenericMsgBuffer package( params );
        
    TInt err = iController.CustomCommandSync( iMessageDest,
                                              EMccGetAudioRoute,
                                              package,
                                              KNullDesC8,
                                              package );
                                         
    aRoutingDestination = package().iParam2;

    __INTERFACE_INT1( "MccInterface::AudioRouting, routing currently to ",
                      aRoutingDestination )   
    __INTERFACE_INT1( "MccInterface::AudioRouting, exit with ", err )
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMccInterface::Volume
// Gets current volume.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::Volume( TInt& aVolume ) const
    {
    __INTERFACE( "MccInterface::Volume" )
    TInt err = iAudioPlayDeviceCustomCommands.GetVolume( aVolume );
    __INTERFACE_INT1( "MccInterface::Volume, exit with" ,aVolume )    
    return err;
    }


// ---------------------------------------------------------------------------
// CMccInterface::MaxVolume
// Gets current maximum volume supported.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::MaxVolume( TUint32 /*aSessionId*/, 
                                        TUint32 /*aLinkId*/, 
                                        TUint32 /*aStreamId*/, 
                                        TInt& aMaxVolume )
    {
    __INTERFACE( "MccInterface::MaxVolume" )
    
    TInt err = iAudioPlayDeviceCustomCommands.GetMaxVolume( aMaxVolume );

    __INTERFACE_INT1( "MccInterface::MaxVolume, exit err = ", err )
    __INTERFACE_INT1( "MccInterface::MaxVolume, exit volume = ", aMaxVolume )
    return err;
    }


// ---------------------------------------------------------------------------
// CMccInterface::SetVolume
// Sets volume to all streams.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SetVolume( TInt aVolume )
    {
    __INTERFACE_INT1( "MccInterface::SetVolume", aVolume )
    return iAudioPlayDeviceCustomCommands.SetVolume( aVolume );
    }

// ---------------------------------------------------------------------------
// CMccInterface::Balance
// Gets current balance in a given stream
// ---------------------------------------------------------------------------
//
TInt CMccInterface::Balance( TUint32 aSessionId, 
                                      TUint32 aLinkId, 
                                      TUint32 aStreamId, 
                                      TInt& aBalance,
                                      TMccDevice aStreamType
                                     ) 
    {
    __INTERFACE( "MccInterface::Balance" )
    TInt err = KErrNone;
    
    TMccStream stream;
    stream.iSessionID = aSessionId;
    stream.iLinkID    = aLinkId;
    stream.iStreamID  = aStreamId;
    
    TMccStreamPckg package( stream );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccStreamSettings,
                                         package,
                                         KNullDesC8 );

    if ( KErrNone != err )
        {
        return err;
        }


    if ( aStreamType == EMccAudioPlay ) 
        {
        err = iAudioPlayDeviceCustomCommands.GetBalance( aBalance );
        }
    else if ( aStreamType == EMccAudioRecord )
        {
        err = iAudioRecordDeviceCustomCommands.GetBalance( aBalance );
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::SetBalance
// Sets balance in a given stream.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SetBalance( TUint32 aSessionId, 
                                         TUint32 aLinkId, 
                                         TUint32 aStreamId, 
                                         TInt aBalance,
                                         TMccDevice aStreamType
                                         )
    {
    __INTERFACE( "MccInterface::SetBalance" )    
    TInt err = KErrNone;
    
    TMccStream stream;
    stream.iSessionID = aSessionId;
    stream.iLinkID    = aLinkId;
    stream.iStreamID  = aStreamId;
    
    TMccStreamPckg package( stream );

    err = iController.CustomCommandSync( iMessageDest,
                                         EMccStreamSettings,
                                         package,
                                         KNullDesC8 );

    if ( KErrNone != err )
        {
        return err;
        }

    if ( aStreamType == EMccAudioPlay ) 
        {
        return iAudioPlayDeviceCustomCommands.SetBalance(aBalance);
        }
    else if ( aStreamType == EMccAudioRecord ) 
        { 
        return iAudioRecordDeviceCustomCommands.SetBalance(aBalance);
        }
    else 
        {
        return KErrNotSupported;
        }                                        

    }


// ---------------------------------------------------------------------------
// CMccInterface::GetGain
// Gets current gain used in recording.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::GetGain( TInt& aGain ) const
    {
    __INTERFACE( "MccInterface::GetGain" )    
    return iAudioRecordDeviceCustomCommands.GetGain( aGain );
    }


// ---------------------------------------------------------------------------
// CMccInterface::SetGain
// Sets uplink/record gain to all ongoing sessions.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::SetGain( TInt aGain )
    {
    __INTERFACE( "MccInterface::SetGain" )    
    return iAudioRecordDeviceCustomCommands.SetGain( aGain );
    }


// ---------------------------------------------------------------------------
// CMccInterface::MaxGain
// Gets current maximum gain supported.
// ---------------------------------------------------------------------------
//
TInt CMccInterface::MaxGain( TUint32 /*aSessionId*/, 
                                      TUint32 /*aLinkId*/, 
                                      TUint32 /*aStreamId*/, 
                                      TInt& aMaxGain ) 
    {
    __INTERFACE( "MccInterface::MaxGain" )    
    
    TInt err = iAudioRecordDeviceCustomCommands.GetMaxGain( aMaxGain );
    
    __INTERFACE_INT1( "MccInterface::MaxGain, exit err = ", err )
    __INTERFACE_INT1( "MccInterface::MaxGain, exit gain = ", aMaxGain )
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMccInterface::StoreEndpointInfo
// ---------------------------------------------------------------------------
//
TInt CMccInterface::StoreEndpointInfo( 
    TUint32 aEndpointId, 
    const TMMFMessageDestination& aHandleInfo )
    {
    TMccEndpointInfo info;
    info.iEndpointId = aEndpointId;
    info.iHandleInfo = aHandleInfo;
    return iEndpointInfos.Append( info );
    }

// ---------------------------------------------------------------------------
// CMccInterface::FindEndpointInfo
// ---------------------------------------------------------------------------
//    
TMMFMessageDestination CMccInterface::FindEndpointInfo( 
    TUint32 aEndpointId, TInt& aError )
    {
    // Have to do in ackward way because TMMFMessageDestination does not
    // have setters
    TMccEndpointInfo info;
    info.iEndpointId = aEndpointId;
    
    TIdentityRelation<TMccEndpointInfo> comparison( EndpointInfoMatch );
    TInt index = iEndpointInfos.Find( info, comparison );
    if ( index != KErrNotFound )
        {
        aError = KErrNone;
        return TMMFMessageDestination( 
                iEndpointInfos[ index ].iHandleInfo.InterfaceId(),
                iEndpointInfos[ index ].iHandleInfo.DestinationHandle() );
        }
    else
        {
        aError = KErrNotFound;
        }
    return TMMFMessageDestination();
    }

// ---------------------------------------------------------------------------
// CMccInterface::RemoveEndpointInfo
// ---------------------------------------------------------------------------
//                           
TInt CMccInterface::RemoveEndpointInfo( TUint32 aEndpointId )
    {
    TMccEndpointInfo info;
    info.iEndpointId = aEndpointId;
    
    TIdentityRelation<TMccEndpointInfo> comparison( EndpointInfoMatch );
    TInt index = iEndpointInfos.Find( info, comparison );
    if ( index != KErrNotFound )
        {
        iEndpointInfos.Remove( index );
        return KErrNone;
        }
        
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CMccInterface::EndpointInfoMatch
// ---------------------------------------------------------------------------
//   
TBool CMccInterface::EndpointInfoMatch( 
    const TMccEndpointInfo& aInfo1, 
    const TMccEndpointInfo& aInfo2 )
    {
    return ( aInfo1.iEndpointId == aInfo2.iEndpointId );
    }

// ---------------------------------------------------------------------------
// CMccInterface::GetController
// ---------------------------------------------------------------------------
// 
void CMccInterface::GetController( RMMFController& aController )
    {
    aController = iController;
    }
    
//  End of File
