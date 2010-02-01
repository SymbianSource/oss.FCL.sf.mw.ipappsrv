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
* Description:    Controller
*
*/




// INCLUDE FILES
#include <mmf/common/mmfbase.h>
#include <ecam.h>
#include <mmf/server/sounddevice.h>

#include "mmccinterface.h"
#include "mmcccodecinformation.h"

#include "mcccontroller.h"
#include "mcccamerahandler.h"
#include "mcculdlclient.h"
#include "mccuids.hrh"
#include "mccresourcepool.h"
#include "mccresourceitem.h"
#include "mccqoscontroller.h"
#include "mcccontrollerlogs.h"
#include "mccasynclinkcreator.h"
#include "mmcccryptocontext.h"
#include "mccdatasink.h"
#include "mccdatasource.h"
#include "mccrtpdatasink.h"

const TInt KNumValue2 = 2;

_LIT( KMccControllerName, "mmcccontroller" );

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

// -----------------------------------------------------------------------------
// CMccController::CMccController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccController::CMccController()
    {
    }

// -----------------------------------------------------------------------------
// CMccController::~CMccController
// Destructor
// -----------------------------------------------------------------------------
//
CMccController::~CMccController()
    {
	__CONTROLLER( "CMccController::~CMccController" )
    
    CancelMmfMessage();

    // delete isessionarray
    iSessionArray.ResetAndDestroy();
    iSessionArray.Close();
    
    iCreatorArray.ResetAndDestroy();
    iCreatorArray.Close();
    
    // delete eventbuffer and contents of it
    iEventBuf.ResetAndDestroy();
    iEventBuf.Close();
    
    delete iQosController;
    
    delete iResourcePool;
    
    if ( iTranscoder )
        {
        iTranscoder->UnregisterEventObserver();
        
        /* Commented off since causes crash
        REComSession::DestroyedImplementation( iTranscoderKey );
        */
        delete iTranscoder;
        iTranscoder = NULL;   
        }
    
    iDtmfPayloadTypes.Close();
       
    #ifdef FTD_ENABLED
    iStreamStatsQueue.Close();
    iCodecStatsQueue.Close();
    iJBufferStatsQueue.Close();
    #endif
	__CONTROLLER( "CMccController::~CMccController, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccController* CMccController::NewL()
    {
    CMccController* self = new ( ELeave ) CMccController;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------------------------
// CMccController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccController::ConstructL()
    {
	__CONTROLLER( "CMccController::ConstructL" )
	User::LeaveIfError( User::RenameThread( KMccControllerName ) );

    CMMFAudioPlayDeviceCustomCommandParser* audPlayDevParser
        = CMMFAudioPlayDeviceCustomCommandParser::NewL( *this );
    CleanupStack::PushL( audPlayDevParser );
    AddCustomCommandParserL( *audPlayDevParser );
    CleanupStack::Pop( audPlayDevParser );

    CMMFAudioRecordDeviceCustomCommandParser* audRecDevParser
        = CMMFAudioRecordDeviceCustomCommandParser::NewL( *this );
    CleanupStack::PushL( audRecDevParser );
    AddCustomCommandParserL( *audRecDevParser );
    CleanupStack::Pop( audRecDevParser );
    
    iResourcePool = CMccResourcePool::NewL();
    
    iQosController = CMccQosController::NewL( *this, *iResourcePool );
    
    __CONTROLLER( "CMccController::ConstructL, creating devsound" )
    
    CMMFDevSound* devSound = CMMFDevSound::NewL();
    iMaxVolume = devSound->MaxVolume();
    __CONTROLLER_INT1( "CMccController::ConstructL, max vol:", iMaxVolume )
    iMaxGain = devSound->MaxGain();
    __CONTROLLER_INT1( "CMccController::ConstructL, max gain:", iMaxGain )
    delete devSound;
    iVolume = iMaxVolume / KNumValue2;
    iGain = iMaxGain / KNumValue2;

    #ifdef FTD_ENABLED
    // Message queues are created here and opened by components,
    // which will produce messages
    User::LeaveIfError( iStreamStatsQueue.CreateGlobal( KMccStreamStats,
            KNumOfStreamStatsSlots, EOwnerProcess ) );
    
    User::LeaveIfError( iCodecStatsQueue.CreateGlobal( KMccCodecStats,
            KNumOfCodecStatsSlots, EOwnerProcess ) );
    
    User::LeaveIfError( iJBufferStatsQueue.CreateGlobal( KMccJBufferStats,
            KNumOfJBufferStatsSlots, EOwnerProcess ) );
    #endif
	__CONTROLLER( "CMccController::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetSourceIdL
// -----------------------------------------------------------------------------
//
void CMccController::GetSourceIdL( TMMFMessage& aMessage )
    {
    MDataSource* dataSource = iResourcePool->SourceCandidate();
    __ASSERT_ALWAYS( dataSource, User::Leave( KErrNotFound ) );
    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );
    pckg().iEndpointID = MCC_ENDPOINT_ID( dataSource );
    aMessage.WriteDataToClientL( pckg );
    }

// -----------------------------------------------------------------------------
// CMccController::GetSinkIdL
// -----------------------------------------------------------------------------
//
void CMccController::GetSinkIdL( TMMFMessage& aMessage )
    {
    MDataSink* dataSink = iResourcePool->SinkCandidate();
    __ASSERT_ALWAYS( dataSink, User::Leave( KErrNotFound ) );
    
    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );
    pckg().iEndpointID = MCC_ENDPOINT_ID( dataSink );
    aMessage.WriteDataToClientL( pckg );
    }

// -----------------------------------------------------------------------------
// CMccController::CreateSessionL
// Creates new UlDlClient session
// -----------------------------------------------------------------------------
//
void CMccController::CreateSessionL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::CreateSessionL" )
    TMccCreateSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    // Mcc ULDL Client -> NewL
    CMccUlDlClient* ulDlClient = 
        CMccUlDlClient::NewL( this, iResourcePool, iSessionIndex );
    CleanupStack::PushL( ulDlClient );
    iSessionArray.AppendL( ulDlClient );
    CleanupStack::Pop( ulDlClient );

	__CONTROLLER_INT1( "CMccController::CreateSessionL, session id", iSessionIndex )
    pckg().iSessionID = iSessionIndex;
    iSessionIndex++;
    
    aMessage.WriteDataToClientL( pckg );
	__CONTROLLER( "CMccController::CreateSessionL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::CloseSessionL
// Closes UlDlClient
// -----------------------------------------------------------------------------
//
void CMccController::CloseSessionL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::CloseSessionL" )
    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    
	TInt lastIndex = iCreatorArray.Count() - 1;
	
	for ( TInt i = lastIndex; i >= 0; i-- )
	    {
   		if ( iCreatorArray[i]->GetSessionId() == pckg().iSessionID )
			{
			delete iCreatorArray[i];
			iCreatorArray.Remove( i );
			}
	    }

    delete iSessionArray[session];
    iSessionArray.Remove( session );
	__CONTROLLER( "CMccController::CloseSessionL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::CreateLinkL
// Creates new Ul/Dl Client
// -----------------------------------------------------------------------------
//
void CMccController::CreateLinkL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::CreateLinkL" )
    TMccCreateLinkPckg pckg;
    aMessage.ReadData1FromClientL( pckg );
    TInt session = FindSessionL( pckg().iSessionID );
    
    CMccAsyncLinkCreator* creator =
    CMccAsyncLinkCreator::NewLC( *this, iSessionArray[session] );

    creator->StartLinkCreationL( aMessage );
    iCreatorArray.AppendL( creator );
    
    CleanupStack::Pop( creator );
    }

// -----------------------------------------------------------------------------
// CMccController::CloseLinkL
// Closes a Ul/Dl Client
// -----------------------------------------------------------------------------
//
void CMccController::CloseLinkL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::CloseLinkL" )
    TMccGenericMsgBuffer pckg;
    aMessage.ReadData1FromClientL( pckg );
    
    TInt session = FindSessionL( pckg().iSessionID );
    iSessionArray[session]->CloseLinkL( pckg().iLinkID );
    // Remove DTMF payload associated with the closed link.
    TInt index = iDtmfPayloadTypes.Find( pckg().iLinkID, 
        TMccKeyValuePair<TUint32, TUint8>::CompareKey );
    if ( KErrNotFound != index )
        {
        __CONTROLLER_INT1( "CMccController::CloseLinkL, REMOVING DTMF PT: ",
             iDtmfPayloadTypes[index].Value() );
        
        iDtmfPayloadTypes.Remove( index );
        }
    
	__CONTROLLER( "CMccController::CloseLinkL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::StartStreamL
// Starts stream
// -----------------------------------------------------------------------------
//
void CMccController::StartStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::StartStreamL" )
    TMccStreamPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    iResourcePool->SetEnableRtcpL( pckg().iStreamID, pckg().iEnableRTCP );
    
    iSessionArray[session]->PlayL( pckg().iLinkID, pckg().iStreamID, pckg().iEndpointID,
                                         pckg().iStreamPaused,
                                         pckg().iEnableRTCP );
	__CONTROLLER( "CMccController::StartStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::StopStreamL
// Stops stream
// -----------------------------------------------------------------------------
//
void CMccController::StopStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::StopStreamL" )
    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    iSessionArray[session]->StopL( pckg().iLinkID, pckg().iStreamID,
    	pckg().iEndpointID );
	__CONTROLLER( "CMccController::StopStreamL, exit" )
    }


// -----------------------------------------------------------------------------
// CMccController::StartInactivityTimerL
// Starts inactivity timer for a stream in a given session.
// -----------------------------------------------------------------------------
//
void CMccController::StartInactivityTimerL( TMMFMessage& aMessage )
	{
	__CONTROLLER( "CMccController::StartInactivityTimerL" )
	TMccInactivityTimerPckg buffer;
    aMessage.ReadData1FromClientL( buffer );
   
    TInt session = FindSessionL( buffer().iSessionID );
    iSessionArray[session]->StartInactivityTimerL( buffer().iLinkID,
                                                   buffer().iStreamID,
                                                   buffer().iTimeoutTime );
	__CONTROLLER( "CMccController::StartInactivityTimerL, exit" )
	}	

// -----------------------------------------------------------------------------
// CMccController::StopInactivityTimerL
// Stops inactivity timer for a stream in a given session.
// -----------------------------------------------------------------------------
//
void CMccController::StopInactivityTimerL( TMMFMessage& aMessage )
	{
	__CONTROLLER( "CMccController::StopInactivityTimerL" )
	TMccGenericMsgBuffer buffer;
    aMessage.ReadData1FromClientL( buffer );
   
    TInt session = FindSessionL( buffer().iSessionID );
    iSessionArray[session]->StopInactivityTimerL( buffer().iLinkID,
                                                  buffer().iStreamID );
	__CONTROLLER( "CMccController::StopInactivityTimerL, exit" )
	}	

// -----------------------------------------------------------------------------
// CMccController::RequestEventNotificationL
// Requests Event notification
// -----------------------------------------------------------------------------
//
void CMccController::RequestEventNotificationL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::RequestEventNotificationL" )
    
    CancelMmfMessage();

    TMccEventPackage pckg;
    aMessage.ReadData1FromClientL( pckg );
    
    iMessage = new ( ELeave ) TMMFMessage( aMessage );
    
    SendEvent();

	__CONTROLLER( "CMccController::RequestEventNotificationL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SendEvent
// Calls SendEventL()
// -----------------------------------------------------------------------------
//
void CMccController::SendEvent()
    {
	__CONTROLLER( "CMccController::SendEvent" )
    TRAPD( err, SendEventL() );
    if ( err )
        {
        __CONTROLLER_INT1( "CMccController::SendEventL, leaved with err:", err )
        }
    }

// -----------------------------------------------------------------------------
// CMccController::SendEventL
// Sends event to clientside
// -----------------------------------------------------------------------------
//
void CMccController::SendEventL()
    {
    if ( iMessage && iEventBuf.Count() )
        {
        TMccEvent* event = iEventBuf[ 0 ];
        
        TMccEventPackage pckg( *event );
        iMessage->WriteDataToClientL( pckg );

        // 2) Complete the iMessage.
        iMessage->Complete( KErrNone ); 
        delete iMessage;
        iMessage = 0;
        
        iEventBuf.Remove( 0 );
        delete event;        
        }
    }
    

// -----------------------------------------------------------------------------
// CMccController::AddDataSinkL
// Adds data sink
// -----------------------------------------------------------------------------
//
void CMccController::AddDataSinkL( MDataSink& aSink )
    {
	__CONTROLLER( "CMccController::AddDataSinkL" )
	__CONTROLLER_INT1( "CMccController sink", aSink.DataSinkType().iUid )
    
    iResourcePool->ReserveSinkL( &aSink, MCC_ENDPOINT_ID( &aSink ) );
    }

// -----------------------------------------------------------------------------
// CMccController::AddDataSourceL
// Adds data source
// -----------------------------------------------------------------------------
//
void CMccController::AddDataSourceL( MDataSource& aSource )
    {
	__CONTROLLER( "CMccController::AddDataSourceL" )
	__CONTROLLER_INT1( "CMccController source", aSource.DataSourceType().iUid )
	
    iResourcePool->ReserveSourceL( &aSource, MCC_ENDPOINT_ID( &aSource ) );
    }

// -----------------------------------------------------------------------------
// CMccController::RemoveDataSourceL
// -----------------------------------------------------------------------------
//
void CMccController::RemoveDataSourceL( MDataSource& aDataSource )
    {
    RemoveEndpointL( MCC_ENDPOINT_ID( &aDataSource ) ); 
    }

// -----------------------------------------------------------------------------
// CMccController::RemoveDataSinkL
// -----------------------------------------------------------------------------
//
void CMccController::RemoveDataSinkL( MDataSink& aDataSink )
    {
    RemoveEndpointL( MCC_ENDPOINT_ID( &aDataSink ) ); 
    }

// -----------------------------------------------------------------------------
// CMccController::ResetL
// Not used
// -----------------------------------------------------------------------------
//
void CMccController::ResetL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::PlayL
// Not used
// -----------------------------------------------------------------------------
//
void CMccController::PlayL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::StopL
// Not used
// -----------------------------------------------------------------------------
//
void CMccController::StopL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::PrimeL
// Not used
// -----------------------------------------------------------------------------
//
void CMccController::PrimeL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::PauseL
// Not used
// -----------------------------------------------------------------------------
//
void CMccController::PauseL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::PositionL
// Not supported
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccController::PositionL() const
    {
    User::Leave( KErrNotSupported );
    // Make CW happy, lint #527
    TTimeIntervalMicroSeconds pos( static_cast<TUint64>( 0 ) );
    return pos;
    }

// -----------------------------------------------------------------------------
// CMccController::SetPositionL
// Position setting not supported
// -----------------------------------------------------------------------------
//
void CMccController::SetPositionL( const TTimeIntervalMicroSeconds& /*aPosition*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::DurationL
// Duration not supported
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMccController::DurationL() const
    {
    User::Leave( KErrNotSupported );
    // Make CW happy, lint #527
    return TTimeIntervalMicroSeconds( static_cast<TUint64>( 0 ) );
    }

// -----------------------------------------------------------------------------
// CMccController::SetPrioritySettings
// Sets priority settings
// -----------------------------------------------------------------------------
//
void CMccController::SetPrioritySettings( const TMMFPrioritySettings& aPrioritySettings )
    {
	__CONTROLLER( "CMccController::SetPrioritySettings" )
    iPrioritySettings = aPrioritySettings;
    }

// -----------------------------------------------------------------------------
// CMccController::SetPriorityL
// Set link priority
// -----------------------------------------------------------------------------
//
void CMccController::SetPriorityL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SetPriorityL" )
    TMccStreamPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TMMFPrioritySettings settings;
    settings.iPref = EMdaPriorityPreferenceNone;
    settings.iState = EMMFStateIdle;
    settings.iPriority = pckg().iPriority;

    TInt session = FindSessionL( pckg().iSessionID );
    iSessionArray[session]->SetPriorityL( pckg().iLinkID, pckg().iStreamID,
                                          settings );
	__CONTROLLER( "CMccController::SetPriorityL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetNumberOfMetaDataEntriesL
// -----------------------------------------------------------------------------
//
void CMccController::GetNumberOfMetaDataEntriesL( TInt& /*aNumberOfEntries*/ )
    {
    User::Leave( KErrNotSupported ); 
    }

// -----------------------------------------------------------------------------
// CMccController::GetMetaDataEntryL
// -----------------------------------------------------------------------------
//
CMMFMetaDataEntry* CMccController::GetMetaDataEntryL( TInt /*aIndex*/ )
    {
    User::Leave( KErrNotSupported );
    // Make CW happy, lint #527    
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccController::CreateStreamL
// Creates stream
// -----------------------------------------------------------------------------
//
void CMccController::CreateStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::CreateStreamL" )
	
	MDataSource* dataSource = iResourcePool->SourceCandidate();
	MDataSink* dataSink = iResourcePool->SinkCandidate();
    __ASSERT_ALWAYS( dataSource, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( dataSink, User::Leave( KErrNotReady ) );
    
    TMccGenericMsgBuffer localBuffer;
    aMessage.ReadData1FromClientL( localBuffer );

    TInt session = FindSessionL( localBuffer().iSessionID );
    
    // Read stream creation parameters from message
    TMccCreateStreamParamPckg paramPckg;
    aMessage.ReadData2FromClientL( paramPckg );
   
   	TUint32 dataSourceUid = dataSource->DataSourceType().iUid;
   
    if ( MCC_INTERNAL_ENDPOINT( dataSourceUid ) )
	   {  	
	   CMccDataSource* mccDataSource = static_cast<CMccDataSource*>( dataSource );
	   UpdateEndpointVolumeSettings( *mccDataSource );
	   }
	   
	TUint32 dataSinkUid = dataSink->DataSinkType().iUid;

	if ( MCC_INTERNAL_ENDPOINT( dataSinkUid ) )
	   {  
	   CMccDataSink* mccDataSink = static_cast<CMccDataSink*>( dataSink );
	   UpdateEndpointVolumeSettings( *mccDataSink );	
	   }

    localBuffer().iStreamID = iSessionArray[session]->GenerateStreamId();
 
    // TBD! IapId has to be fetched from link 
    
    // Standby should be enabled only for audio downlink
    TBool isStandBy( paramPckg().iStandByTimer != 0 &&
                     dataSink->DataSinkType() == KUidMmfAudioOutput &&
                     dataSource->DataSourceType() == KMccRtpSourceUid );
                     
    // TMccGenericMessage's iMessageFunction contains the stream type in the
    // case of stream creation, modify stream type if standby timer is enabled
    // and allowed (usage rights are checked in resource pool).
    TInt streamType = isStandBy ?
         KMccAudioDownlinkStandbyStream : paramPckg().iStreamType;
                     
    TMccResourceParams resourceParams( localBuffer().iSessionID, 
                                       localBuffer().iLinkID,
                                       localBuffer().iStreamID,
                                       0,
                                       isStandBy,
                                       streamType );
                                        
    iResourcePool->ReserveResourcesL( resourceParams );
    
    iSessionArray[session]->OpenL( localBuffer().iLinkID,
                                   localBuffer().iStreamID,
                                   streamType,
                                   paramPckg().iFourCC,
                                   dataSource,
                                   dataSink,
                                   paramPckg().iPrioritySettings );
  
    iSessionArray[session]->SetVolumeL( iVolume );
    
    // Need to write the stream ID back to client side
    aMessage.WriteDataToClientL( localBuffer );

	__CONTROLLER( "CMccController::CreateStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::DeleteStreamL
// Deletes stream
// -----------------------------------------------------------------------------
//
void CMccController::DeleteStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::DeleteStreamL" )
    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    iSessionArray[session]->CloseL( pckg().iLinkID, pckg().iStreamID );
    
    iResourcePool->FreeResources( pckg().iStreamID );
    
	__CONTROLLER( "CMccController::DeleteStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::PrepareStreamL
// Prepares stream
// -----------------------------------------------------------------------------
//
void CMccController::PrepareStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::PrepareStreamL" )
    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    iSessionArray[session]->PrepareL( pckg().iLinkID, pckg().iStreamID,
        	pckg().iEndpointID );
	__CONTROLLER( "CMccController::PrepareStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::PauseStreamL
// Pauses stream
// -----------------------------------------------------------------------------
//
void CMccController::PauseStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::PauseStreamL" )
    TMccStreamPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    iResourcePool->SetEnableRtcpL( pckg().iStreamID, pckg().iEnableRTCP );
    iSessionArray[session]->PauseL( pckg().iLinkID, pckg().iStreamID,
    	pckg().iEndpointID, pckg().iEnableRTCP );
	__CONTROLLER( "CMccController::PauseStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::ResumeStreamL
// Resumes stream
// -----------------------------------------------------------------------------
//
void CMccController::ResumeStreamL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::ResumeStreamL" )
    TMccStreamPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    iResourcePool->SetEnableRtcpL( pckg().iStreamID, pckg().iEnableRTCP );
    iSessionArray[session]->ResumeL( pckg().iLinkID, pckg().iStreamID,
        pckg().iEndpointID, pckg().iEnableRTCP );
	__CONTROLLER( "CMccController::ResumeStreamL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::ResumeStreamL
// Resumes stream
// -----------------------------------------------------------------------------
//
void CMccController::StreamsExistsL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::StreamsExistsL" )
    TMccStreamPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    TBool ret = iSessionArray[session]->StreamsExistsL( pckg().iLinkID );
    if ( !ret )
        {
        User::Leave( KErrNotFound );
        }
	__CONTROLLER( "CMccController::StreamsExistsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::CustomCommand
// Controller custom commands
// -----------------------------------------------------------------------------
//
void CMccController::CustomCommand( TMMFMessage& aMessage )
    {
    if( KMccUidInterface != aMessage.Destination().InterfaceId() )
        {
        // This message was not intended for us
        aMessage.Complete( KErrNotSupported );
        return;
        }
    else
        {
        // Interface ID matches ours, lets do the custom command
        TRAPD( err, CustomCommandL( aMessage ) )

        if( KErrNone != err )
            {
            // Complete the message as it leaved with error. This will probably
            // leave the client.
            aMessage.Complete( err );
            }
        else
            {
            // ERequestEventNotification is asynchronous and will be 
            // completed later when events start pouring in
            if( ERequestEventNotification != aMessage.Function() )
                {
                aMessage.Complete( err );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CMccController::CustomCommandL
// Controller custom commands
// -----------------------------------------------------------------------------
//
void CMccController::CustomCommandL( TMMFMessage& aMessage )
    {
    switch( aMessage.Function() )
        {
		case EMccClose:
			break;
			
        case EMccGetSinkId:
            GetSinkIdL( aMessage );
            break;
        case EMccGetSourceId:
            GetSourceIdL( aMessage );
            break;
                        
        // Session
        case EMccCreateSession:
            CreateSessionL( aMessage );
            break;
        case EMccSetRemoteAddress:
            SetRemoteAddressL( aMessage );
            break;
        case EMccSetRemoteRtcpAddr:
        	SetRemoteRtcpAddrL( aMessage );
        	break;
        case EMccCloseSession:
            CloseSessionL( aMessage );
            break;

        // Link
        case EMccCreateLink:
            CreateLinkL( aMessage );
            break;
        case EMccCloseLink:
            CloseLinkL( aMessage );
            break;

        // Stream
        case EMccCreateStream:
            CreateStreamL( aMessage );
            break;
        case EMccPrepareStream:
            PrepareStreamL( aMessage );
            break;
        case EMccStartStream:
            StartStreamL( aMessage );
            break;
        case EMccPauseStream:
            PauseStreamL( aMessage );
            break;
        case EMccResumeStream:
            ResumeStreamL( aMessage );
            break;
        case EMccStopStream:
            StopStreamL( aMessage );
            break;
        case EMccDeleteStream:
            DeleteStreamL( aMessage );
            break;
        case EMccStreamSettings:
            StreamSettingsL( aMessage );
            break;
        case EMccStreamsExists:
            StreamsExistsL( aMessage );
            break;
        
        // Inactivity Timer
        case EMccInactivityTimerStart:
        	StartInactivityTimerL( aMessage );
        	break;
        case EMccInactivityTimerStop:
        	StopInactivityTimerL( aMessage );
        	break;
        
        // RTCP reports and non-RTCP data
        case EMccSendRtcpRR:
            SendRTCPReceiverReportL( aMessage );
            break;
        case EMccSendRtcpSR:
            SendRTCPSenderReportL( aMessage );
            break;
        case EMccSendRtcpData:
            SendRTCPDataL( aMessage );
            break;
            
        // Codec
        case EMccGetSupportedCodecs:
            GetSupportedCodecsL( aMessage );
            break;
        case EMccGetCodec:
            GetCodecL( aMessage );
            break;
        case EMccGetFmtpAttr:
            GetFmtpAttrL( aMessage );
            break;
        case EMccSetCodec:
            SetCodecL( aMessage );
            break;
        case EMccGetSupportedBitrates:
            GetSupportedBitratesL( aMessage );
            break;
            
        // SSRC
        case EMccGetSSRC:
            GetSSRCL( aMessage );
            break;
        case EMccSetAudioPriority:
            SetPriorityL( aMessage );
            break;

        // Camera
        case EMccEnableViewFinder:
            EnableViewFinderL( aMessage );
            break;
        case EMccDisableViewFinder:
            DisableViewFinderL( aMessage );
            break;
        case EMccGetViewFinderSettings:
            ViewFinderSettingsL( aMessage );
            break;
        case EMccGetCamInfo:
            GetCamInfoL( aMessage );        
            break;
        case EMccGetNumOfCams:
            GetNumOfCamsL( aMessage );        
            break;
        case EMccSetCamValue1:
            SetCamValue1L( aMessage );
            break;
        case EMccGetCamValue1:
            GetCamValue1L( aMessage );
            break;
        case EMccSetCamValue2:
            SetCamValue2L( aMessage );
            break;
        case EMccGetCamValue2:
            GetCamValue2L( aMessage );
            break;
            
        // Audio routing
        case EMccSetAudioRoute:
            SetAudioRouteL( aMessage );
            break;
        case EMccGetAudioRoute:
            GetAudioRouteL( aMessage );
            break;

        // Endpoint handling
        case EMccReuse:
            ReuseL( aMessage );
            break;
        case EMccGetReferenceCount:
            ReferenceCountL( aMessage );
            break;      
        case EMccSetEndpointParameter:
            SetEndpointParameterL( aMessage );
            break;
        case EMccGetEndpointParameter:
            GetEndpointParameterL( aMessage );
            break;
        case EMccUpdateDataSink:
        case EMccUpdateDataSource:
            UpdateEndpointL( aMessage );
            break;
            
        // transcode file
        case EMccTranscodeFile:
            TranscodeFileL( aMessage );
            break;
        case EMccCancelTranscodeFile:
            CancelTranscodeFileL( aMessage );
            break;
            
        // Generic    
        case ERequestEventNotification:
            RequestEventNotificationL( aMessage );
            break;
        case EMccSendMediaSignal:
            SendMediaSignalL( aMessage );
            break;
        case EMccCancel:
            CancelMmfMessage();        
            break;
            
        // Secure    
        case EMccBindContextIntoStream:
            BindContextIntoStreamL( aMessage );
            break;
        case EMccRemoveContext:
            RemoveContextL( aMessage );
            break;    
        default:
            User::Leave( KErrNotSupported );
            break;
        }   // end of switch
    }

// -----------------------------------------------------------------------------
// CMccController::GetSupportedCodecsL
// Puts supported codecs into fourcc array
// -----------------------------------------------------------------------------
//
void CMccController::GetSupportedCodecsL( TMMFMessage& aMessage ) const
    {
    __CONTROLLER( "CMccController::GetSupportedCodecsL" )

    TMccSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );
    TFixedArray<TFourCC, KMccFourCCArraySize>& codecs = pckg().iFourCCArray;
    TInt index( 0 );
    
    // Audio codecs
    CMMFDevSound* devSound = CMMFDevSound::NewL();
    CleanupStack::PushL( devSound );
    
    TMMFPrioritySettings dummy;
    RArray< TFourCC > outCodecs;
    devSound->GetSupportedOutputDataTypesL( outCodecs, dummy );
    CleanupClosePushL( outCodecs );
    
    RArray< TFourCC > inCodecs;
    devSound->GetSupportedInputDataTypesL( inCodecs, dummy );
    CleanupClosePushL( inCodecs );
    
    // Support dynamically checked from CMMFDevSound in HW
    AddCodec( KMccFourCCIdAMRWB, outCodecs, inCodecs, codecs, index++ );
    AddCodec( KMccFourCCIdAMRNB, outCodecs, inCodecs, codecs, index++ );
    AddCodec( KMccFourCCIdG711, outCodecs, inCodecs, codecs, index++ );
    AddCodec( KMccFourCCIdILBC, outCodecs, inCodecs, codecs, index++ );
    AddCodec( KMccFourCCIdG729, outCodecs, inCodecs, codecs, index++ );
    // Not in CMMFDevSound's list
    codecs[ index++ ] = TFourCC( KMccFourCCIdDTMF ); 
    codecs[ index++ ] = TFourCC( KMccFourCCIdRed );
    codecs[ index++ ] = TFourCC( KMccFourCCIdCN );
 
    CleanupStack::PopAndDestroy( &inCodecs );
    CleanupStack::PopAndDestroy( &outCodecs );
    CleanupStack::PopAndDestroy( devSound );
    
    // Video codecs
    codecs[ index++ ] = TFourCC( KMccFourCCIdAVC );
    codecs[ index ] = TFourCC( KMccFourCCIdH263 );

    aMessage.WriteDataToClientL( pckg );

    __CONTROLLER( "CMccController::GetSupportedCodecsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetCodecL
// Gets current codec
// -----------------------------------------------------------------------------
//
void CMccController::GetCodecL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetCodecL" )
    TMccGenericMsgBuffer pckg;
	TMccCodecInfo codecInfo;
    aMessage.ReadData1FromClientL( pckg );

    if( pckg().iSessionID == 0 && pckg().iLinkID == 0)
    	{
    	TUid targetUid;
    	targetUid.iUid = (TUint) pckg().iStreamID;
    	CMccUlDlClient::GetCodecDefaultsL( targetUid, pckg().iParam1, codecInfo);
    	}
    else
    	{
	    TInt session = FindSessionL( pckg().iSessionID );
	    iSessionArray[session]->GetCodecL( pckg().iLinkID,
	                                       pckg().iStreamID,
	                                       codecInfo );    	
    	}		    	
    TMccCodecInfoBuffer writeBuffer( codecInfo );
    aMessage.WriteDataToClientL( writeBuffer );    
	__CONTROLLER( "CMccController::GetCodecL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetFmtpAttrL
// Gets current codec
// -----------------------------------------------------------------------------
//
void CMccController::GetFmtpAttrL( TMMFMessage& /*aMessage*/ )
    {
	__CONTROLLER( "CMccController::GetFmtpAttrL" )
	User::Leave( KErrNotSupported );
	__CONTROLLER( "CMccController::GetFmtpAttrL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SetCodecL
// Sets codec.
// -----------------------------------------------------------------------------
//
void CMccController::SetCodecL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SetCodecL" )
    TMccCodecInfoBuffer readBuffer;
    aMessage.ReadData1FromClientL( readBuffer );
    
    TInt session = FindSessionL( readBuffer().iSessionID );
    iSessionArray[session]->SetCodecInformationL( readBuffer().iLinkID,
                                                  readBuffer().iStreamID,
                                                  readBuffer(),
                                                  KNullDesC8 );
    aMessage.WriteDataToClientL( readBuffer );
    
    if ( 0 == readBuffer().iSdpName.Compare( KTelephoneEvent ) )
        {
        TInt index = iDtmfPayloadTypes.Find( readBuffer().iLinkID, 
            TMccKeyValuePair<TUint32, TUint8>::CompareKey );
        if ( KErrNotFound != index )
            {
            // Remove DTMF payload type for audio muting functionality
            __CONTROLLER_INT1( "CMccController::SetCodecL, REMOVE INDEX:",
                index );
            iDtmfPayloadTypes.Remove( index );
            }
            // Save DTMF payload type for audio muting functionality during
            // DTMF sending.
            __CONTROLLER_INT1( "CMccController::SetCodecL, SAVING DTMF PT:",
                readBuffer().iPayloadType );
            TMccKeyValuePair< TUint32, TUint8 > keyValuePair( 
                readBuffer().iLinkID, readBuffer().iPayloadType );
            iDtmfPayloadTypes.AppendL( keyValuePair );
        }
    
	__CONTROLLER( "CMccController::SetCodecL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetSupportedBitratesL
// Gets the supported bitrates from the codec of the specified stream
// -----------------------------------------------------------------------------
//
void CMccController::GetSupportedBitratesL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetSupportedBitratesL" )
    TMccGenericMsgBuffer pckg;
    aMessage.ReadData1FromClientL( pckg );
    TInt session = FindSessionL( pckg().iSessionID );
    
    RArray<TUint> bitrateArray;    
    iSessionArray[session]->GetSupportedBitratesL( pckg().iLinkID,
                                                   pckg().iStreamID,
                                                   bitrateArray );

    // Move the bitrates to a package descriptor
    TMccBitrates bitrates;
    for ( TInt i = 0; ( i < bitrateArray.Count() ) && i < KMaxBitrates; i++ )
        {
        bitrates.iBitrates[i] = bitrateArray[i];
        }
    bitrateArray.Close();
    
    TMccBitratesPckg writeBuffer( bitrates );
    aMessage.WriteDataToClientL( writeBuffer );
	__CONTROLLER( "CMccController::GetSupportedBitratesL, exit" )
    }


// -----------------------------------------------------------------------------
// CMccController::GetSSRCL
// Gets the syncronization source from the specified stream
// -----------------------------------------------------------------------------
//
void CMccController::GetSSRCL ( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetSSRCL" )
    TMccGenericMsgBuffer pckg;
    aMessage.ReadData1FromClientL( pckg );
    TInt session = FindSessionL( pckg().iSessionID );
    
    TMccSSRC ssrc;
    iSessionArray[session]->GetSSRCL( pckg().iLinkID,
                                      pckg().iStreamID,
                                      ssrc.iSSRC );
                                     
    TMccSSRCPckg ssrcPckg(ssrc);
    aMessage.WriteDataToClientL( ssrcPckg );
	__CONTROLLER( "CMccController::GetSSRCL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccController::SetRemoteAddressL
// Sets remote address
// -----------------------------------------------------------------------------
//
void CMccController::SetRemoteAddressL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SetRemoteAddressL" )
    TMccAddressPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    
    iSessionArray[session]->SetRemoteAddressL( pckg().iAddress,
                                               pckg().iLinkID );
	__CONTROLLER( "CMccController::SetRemoteAddressL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SetRemoteRtcpAddrL
// Sets remote address
// -----------------------------------------------------------------------------
//
void CMccController::SetRemoteRtcpAddrL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SetRemoteRtcpAddrL" )
    TMccAddressPckg pckg;
    aMessage.ReadData1FromClientL( pckg );

    TInt session = FindSessionL( pckg().iSessionID );
    
    iSessionArray[session]->SetRemoteRtcpAddrL( pckg().iAddress,
                                               pckg().iLinkID );
	__CONTROLLER( "CMccController::SetRemoteAddrL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SendMccEventToClient
// Enqueues MccEvent to eventbuffer and sends it to client
// -----------------------------------------------------------------------------
//
TInt CMccController::SendMccEventToClient( TMccEvent& aEvent )
    {
	__CONTROLLER( "CMccController::SendMccEventToClient" )
	__CONTROLLER_INT1( "CMccController event category", aEvent.iEventCategory )
	__CONTROLLER_INT1( "CMccController event type", aEvent.iEventType )
   
    TInt err( KErrNone );
    TBool discardEvent( EFalse );
    TBool streamControl( EFalse );
    TMccEvent* additionalControlEvent( NULL );
    
    // Show event to the QosController
    discardEvent = iQosController->EventReceived( aEvent );
    
    if ( !discardEvent )
        {
        if ( aEvent.iEventCategory == KMccEventCategoryStream ) 
    	    {
    	    if ( aEvent.iEventType == KMccStreamMMFEvent &&
                 aEvent.iErrorCode == KErrDied )
                {
                // Resource not available, disable the endpoint
                
                __CONTROLLER( "CMccController::SendMccEventToClient, \
MMF resource not available" )
                
                discardEvent = ETrue;
                streamControl = ETrue;
                
                // Modify event to be "resource not available" event
                aEvent.iEventCategory = KMccEventCategoryStream;
                aEvent.iEventType = KMccResourceNotAvailable;
                StoreAndSendEvent( aEvent );
                
                // Modify event to be "stream control" event
                aEvent.iEventType = KMccStreamPaused;
                }
            else
                {
                TBool effectiveStandby( EFalse );
        	   	err = iResourcePool->EventReceived( aEvent, 
        	   	                                    discardEvent, 
        	   	                                    effectiveStandby,
        	   	                                    &additionalControlEvent );
        	   	streamControl = effectiveStandby;
                }
    	    }
        else if ( aEvent.iEventCategory == KMccEventCategoryStreamControl )
            {
            // Filter stream control event over here
            discardEvent = ETrue;
            streamControl = ETrue;
            }
        else if ( KMccEventCategoryDtmf == aEvent.iEventCategory &&
                  KMccDtmfControl == aEvent.iEventType )
            {
            // Unmute RTP sink regarding audio payloads when DTMF sending is
            // stopped. Audio can be sent between key presses.
            const TMccDtmfEventData& dtmfEvent = 
                (*reinterpret_cast<const TMccDtmfEventDataPackage*>( 
                &aEvent.iEventData ) ) ();
            
            if ( KMccDtmfManualStop == dtmfEvent.iDtmfEventType ||
                 KMccDtmfManualAbort == dtmfEvent.iDtmfEventType ||
                 KMccDtmfSequenceStop == dtmfEvent.iDtmfEventType ||
                 KMccDtmfSequenceAbort == dtmfEvent.iDtmfEventType ||
                 KMccDtmfStopInDtmfString == dtmfEvent.iDtmfEventType )
                {
                MDataSource* dataSource;
                MDataSink* dataSink;
                err = iResourcePool->FindResource( aEvent.iLinkId, 
                    aEvent.iEndpointId, KMccRtpSinkUid, &dataSource, 
                    &dataSink );
                
                if ( KErrNone == err )
                    {
                    CMccRtpDataSink* rtpSink 
                        = static_cast<CMccRtpDataSink*>( dataSink );
                    if ( rtpSink )
                        {
                        rtpSink->Mute( EFalse, KPayloadTypeUndefined );
                        }
                    }
                else
                    {
                    __CONTROLLER_INT1( 
                        "CMccController::SendMccEventToClient DTMF ERR", err )
                    }
                }
            }
        else
            {
            }
        }
	    
    if ( !err && !discardEvent )
	    {
	    err = StoreAndSendEvent( aEvent );
	    }
    
    if ( !err && streamControl )
	    {
	    TRAP( err, StreamActionL( aEvent, additionalControlEvent ) );
	    if ( err )
	        {
	        // Modify event to contain error info
	        aEvent.iEventCategory = KMccEventCategoryStream;
	        aEvent.iEventType = KMccStreamError;
	        aEvent.iEventData.Zero();
	        aEvent.iErrorCode = err;
	        StoreAndSendEvent( aEvent );
	        }
	    }
	    
	delete additionalControlEvent;
	   
    return err;
    }

// -----------------------------------------------------------------------------
// CMccController::PauseByEndPointIdL
// -----------------------------------------------------------------------------
//
void CMccController::PauseByEndPointIdL( 
    const TMccEvent& aEvent, 
    TUint32 aEndPointId )
	{
	TInt session = FindSessionL( aEvent.iSessionId );
    
    iSessionArray[session]->PauseL( 
            aEvent.iLinkId, 
     		aEvent.iStreamId, 
     		aEndPointId, 
     		iResourcePool->EnableRtcp( aEvent.iStreamId ) );	
	}

// -----------------------------------------------------------------------------
// CMccController::ResumeByEndPointIdL
// -----------------------------------------------------------------------------
//
void CMccController::ResumeByEndPointIdL( 
    const TMccEvent& aEvent, 
    TUint32 aEndPointId )
	{
	TInt session = FindSessionL( aEvent.iSessionId );
    
    iSessionArray[session]->ResumeL( 
            aEvent.iLinkId, 
     		aEvent.iStreamId, 
     		aEndPointId, 
     		iResourcePool->EnableRtcp( aEvent.iStreamId ) );	
	}

// -----------------------------------------------------------------------------
// CMccController::UpdateEndpointVolumeSettings
// -----------------------------------------------------------------------------
//
TInt CMccController::UpdateEndpointVolumeSettings( MMccSourceSink& aEndpoint )
    {
    // Some endpoints do not support volume setting, that's ok
    TRAPD( err, aEndpoint.SetParameterL( KMccSpeakerMaxVolume, 
                                         TPckgBuf<TInt>( iMaxVolume ) ) );
    if ( !err )
        {
        TRAP( err, aEndpoint.SetParameterL( KMccSpeakerVolume, 
                                            TPckgBuf<TInt>( iVolume ) ) );
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CMccController::StateChange
// -----------------------------------------------------------------------------
//
void CMccController::StateChange( TInt /*aState*/, TUint32 /*aLinkId*/ )
	{		
	__CONTROLLER( "CMccController::SendMccEventToClient ignoring" )
	}
	
// -----------------------------------------------------------------------------
// CMccController::ErrorOccured
// -----------------------------------------------------------------------------
//
void CMccController::ErrorOccured( 
    TInt aError, 
    TUint32 aSessionId, 
    TUint32 aLinkId,
	TUint32 aStreamId, 
	TUint32 aEndpointId )
	{	
	__CONTROLLER_INT1( "CMccController::ErrorOccured", aError )
	__CONTROLLER_INT1( "CMccController sessionid",  aSessionId )
	__CONTROLLER_INT1( "CMccController linkid",  aLinkId )
	__CONTROLLER_INT1( "CMccController streamId",  aStreamId )
	__CONTROLLER_INT1( "CMccController endpointId",  aEndpointId )
	
    if ( KErrNone != aError )
        {
        TMccEvent event( aSessionId, aLinkId, aStreamId, aEndpointId,
            KMccEventCategoryStream, KMccStreamError, aError, KNullDesC8 );
        
        SendMccEventToClient( event );            
        }
        
	__CONTROLLER( "CMccController::ErrorOccured, exit" )
	}
    
// -----------------------------------------------------------------------------
// CMccController::StreamSettingsL
// Sets stream settings
// -----------------------------------------------------------------------------
//
void CMccController::StreamSettingsL( TMMFMessage& aMessage )
    {
    aMessage.ReadData1FromClientL( iStreamPckg );
    // Check the session ID already at this stage to avoid a panic from the MMF
    TInt dummy = FindSessionL( iStreamPckg().iSessionID );
    dummy = dummy;
    }

// -----------------------------------------------------------------------------
// CMccController::SendRTCPReceiverReport
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
//
void CMccController::SendRTCPReceiverReportL( TMMFMessage& aMessage ) 
    {
	__CONTROLLER( "CMccController::SendRTCPReceiverReportL" )
    TMccGenericMsgBuffer buffer;
    aMessage.ReadData1FromClientL( buffer );
   
    TInt session = FindSessionL( buffer().iSessionID );
    iSessionArray[session]->SendRTCPReceiverReportL( buffer().iLinkID,
                                                     buffer().iStreamID );
	__CONTROLLER( "CMccController::SendRTCPReceiverReportL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccController::SendRTCPSenderReport
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
//
void CMccController::SendRTCPSenderReportL( TMMFMessage& aMessage ) 
    {
	__CONTROLLER( "CMccController::SendRTCPSenderReportL" )
    TMccGenericMsgBuffer buffer;
    aMessage.ReadData1FromClientL( buffer );
    TInt session = FindSessionL( buffer().iSessionID );
    iSessionArray[session]->SendRTCPSenderReportL( buffer().iLinkID,
                                                   buffer().iStreamID );
	__CONTROLLER( "CMccController::SendRTCPSenderReportL, exit" )
    }    

// -----------------------------------------------------------------------------
// CMccController::SendRTCPDataL
// Sends non-RTCP data
// -----------------------------------------------------------------------------
//
void CMccController::SendRTCPDataL( TMMFMessage& aMessage ) 
    {
	__CONTROLLER( "CMccController::SendRTCPDataL" )
    TMccGenericMsgBuffer buffer;
    aMessage.ReadData1FromClientL( buffer );
    TInt session = FindSessionL( buffer().iSessionID );

    // Copy the data locally        
    HBufC8* dataBuf( NULL ); 
    dataBuf = HBufC8::NewLC( aMessage.SizeOfData2FromClient() );
    TPtr8 ptr( dataBuf->Des() ); 
    aMessage.ReadData2FromClientL( ptr );
    iSessionArray[session]->SendRTCPDataL( buffer().iLinkID,
                                           buffer().iStreamID,
                                           *dataBuf );
    CleanupStack::PopAndDestroy( dataBuf );
	__CONTROLLER( "CMccController::SendRTCPDataL, exit" )
    }    

// -----------------------------------------------------------------------------
// CMccController::MapdGetVolumeL
// Gets downlink volume. 
// -----------------------------------------------------------------------------
//
void CMccController::MapdGetVolumeL( TInt& aVolume )
    {
    aVolume = iVolume;
    }

// -----------------------------------------------------------------------------
// CMccController::MapdGetMaxVolumeL
// Gets downlink stream Max volume. Uses stream settings from iStreamPckg.
// -----------------------------------------------------------------------------
//
void CMccController::MapdGetMaxVolumeL( TInt& aMaxVolume )
    {
    aMaxVolume = iMaxVolume;
    }

// -----------------------------------------------------------------------------
// CMccController::MapdSetVolumeL
// Sets volume to all sessions
// -----------------------------------------------------------------------------
//
void CMccController::MapdSetVolumeL( TInt aVolume )
    {
    if ( aVolume >= 0 && aVolume <= iMaxVolume ) 
        {
        iVolume = aVolume;

        for ( TInt i=0; i < iSessionArray.Count(); i++ )
            {
            iSessionArray[i]->SetVolumeL( aVolume );
            }
        }
    else 
        {
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// CMccController::MapdGetBalanceL
// Gets play balance. Uses stream settings from iStreamPckg.
// -----------------------------------------------------------------------------
//
void CMccController::MapdGetBalanceL( TInt& aBalance )
    {
    TInt left( 0 );
    TInt right( 0 );
    TInt session = FindSessionL( iStreamPckg().iSessionID );
    iSessionArray[session]->GetPlayBalanceL( iStreamPckg().iLinkID,
        iStreamPckg().iStreamID,
        left, right );

    // The right + left balances are <= 100 (%)
    // Here, the balances are converted to one value between -100 and 100 (L-R)
    aBalance = ( KNumValue2 * right ) - KMaxBalance;
    }

// -----------------------------------------------------------------------------
// CMccController::MapdSetBalanceL
// Sets play balance. Uses stream settings from iStreamPckg.
// -----------------------------------------------------------------------------
//
void CMccController::MapdSetBalanceL( TInt aBalance )
    {
    if( aBalance < KMMFBalanceMaxLeft || aBalance > KMMFBalanceMaxRight )
        {
        User::Leave( KErrArgument );
        }

    // The right + left balances should be <= 100 (%)
    TInt right = ( KMaxBalance + aBalance ) / KNumValue2;
    TInt left = KMaxBalance - right;

    TInt session = FindSessionL( iStreamPckg().iSessionID );
    iSessionArray[session]->SetPlayBalanceL( iStreamPckg().iLinkID,
                                             iStreamPckg().iStreamID,
                                             left, right );
    }

// -----------------------------------------------------------------------------
// CMccController::MapdSetVolumeRampL
// -----------------------------------------------------------------------------
//
void CMccController::MapdSetVolumeRampL( const TTimeIntervalMicroSeconds& /*aRampDuration*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccController::MardSetGainL
// Sets uplink gain to all sessions.
// -----------------------------------------------------------------------------
//
void CMccController::MardSetGainL( TInt aGain )
    {
    iGain = aGain;

    for ( TInt i=0; i < iSessionArray.Count(); i++ )
        {
        iSessionArray[i]->SetGainL(aGain);
        }
    }

// -----------------------------------------------------------------------------
// CMccController::MardGetMaxGainL
// Gets uplink stream Max gain. Uses stream settings from iStreamPckg.
// -----------------------------------------------------------------------------
//
void CMccController::MardGetMaxGainL( TInt& aMaxGain )
    {
    aMaxGain = iMaxGain;
    }

// -----------------------------------------------------------------------------
// CMccController::MardGetGainL
// Gets uplink gain.
// -----------------------------------------------------------------------------
//
void CMccController::MardGetGainL( TInt& aGain )
    {
    aGain = iGain;
    }

// -----------------------------------------------------------------------------
// CMccController::MardSetBalanceL
// Sets record balance. Uses stream settings from iStreamPckg.
// -----------------------------------------------------------------------------
//
void CMccController::MardSetBalanceL( TInt aRBalance )
    {
    if( aRBalance < KMMFBalanceMaxLeft || aRBalance > KMMFBalanceMaxRight )
        {
        User::Leave( KErrArgument );
        }

    // The right + left balances should be <= 100%
    TInt right = ( KMaxBalance + aRBalance ) / KNumValue2;
    TInt left = KMaxBalance - right;

    TInt session = FindSessionL( iStreamPckg().iSessionID );
    iSessionArray[session]->SetRecordBalanceL( iStreamPckg().iLinkID,
                                               iStreamPckg().iStreamID, 
                                               left, right );
    }

// -----------------------------------------------------------------------------
// CMccController::MardGetBalanceL
// Gets record balance. Uses stream settings from iStreamPckg.
// -----------------------------------------------------------------------------
//
void CMccController::MardGetBalanceL( TInt& aRecBalance )
    {   
    TInt left( 0 );
    TInt right( 0 );
    TInt session = FindSessionL( iStreamPckg().iSessionID );
    iSessionArray[session]->GetRecordBalanceL( iStreamPckg().iLinkID,
        iStreamPckg().iStreamID, left, right );

    // The right + left balances are <= 100 (%)
    // Here, the balances are converted to one value between -100 and 100 (L-R)
    aRecBalance = ( KNumValue2 * right ) - KMaxBalance;
    }

// -----------------------------------------------------------------------------
// CMccController::FindSessionL
// Finds sessionindex for sessionarray
// -----------------------------------------------------------------------------
//
TInt CMccController::FindSessionL( TUint32 aSessionID )
    {
	__CONTROLLER_INT1( "CMccController::FindSessionL, session id", aSessionID )
    TInt sessionCount = iSessionArray.Count();
    TInt err = KErrNotFound;
    TInt sessionID = 0;

    for ( TInt i = 0; i < sessionCount; i++ )
        {
        if ( iSessionArray[i]->GetSessionId() == aSessionID )
            {
            sessionID = i;
            err = KErrNone;
            }
        }
    
    if ( err == KErrNotFound )
        {
        User::Leave( err );
        }

    return sessionID;
    }

// -----------------------------------------------------------------------------
// CMccController::SendMediaSignalL
// Sends media signal to stream
// -----------------------------------------------------------------------------
//
void CMccController::SendMediaSignalL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SendMediaSignalL" )
    
    // Get the media event package, find session and send the event.
    TMccEventPackage eventPkg;
    aMessage.ReadData1FromClientL( eventPkg );
    
    TInt session = FindSessionL( eventPkg().iSessionId );
    
    if ( KMccEventCategoryDtmf == eventPkg().iEventCategory &&
         KMccDtmfControl == eventPkg().iEventType )
        {
        const TMccDtmfEventData& dtmfEvent = 
            (*reinterpret_cast<const TMccDtmfEventDataPackage*>( 
                &eventPkg().iEventData ) ) ();
        
        if ( KMccDtmfSigStartTone == dtmfEvent.iDtmfEventType ||
             KMccDtmfSigSendString == dtmfEvent.iDtmfEventType ||
             KMccDtmfSigContinueSending == dtmfEvent.iDtmfEventType )
            {
            // Simultaneous audio and outband DTMF sending confuses some
            // servers and thus audio sending must be disabled until DTMF
            // sending is completed.
            
            // DTMF and audio MUST share same SSRC. RTP-sink corresponds SSRC,
            // so  DTMF and audio use same RTP sink. Video streams MUST have
            // own RTP-sink instance because of different data source.
            MDataSource* dataSource;
            MDataSink* dataSink;
            iResourcePool->FindResource( eventPkg().iLinkId, 
                eventPkg().iEndpointId, KMccRtpSinkUid, &dataSource, 
                &dataSink );
            CMccRtpDataSink* rtpSink 
                = static_cast<CMccRtpDataSink*>( dataSink );
            
            // Find DTMF payload type used with the RTP-session(MCC link) and
            // mute other payloads
            TInt index = iDtmfPayloadTypes.Find( eventPkg().iLinkId, 
                TMccKeyValuePair<TUint32, TUint8>::CompareKey );
            if ( KErrNotFound != index && NULL != rtpSink )
                {
                rtpSink->Mute( ETrue, iDtmfPayloadTypes[index].Value() );
                }
            else
                {
                // If DTMF payload type finding fails, try one's best and send 
                // DTMF simultaneously with audio.
                __CONTROLLER_INT1( "CMccController::SendMediaSignalL, ERR", 
                    index )
                }
            }
        }
    
    iSessionArray[session]->SendMediaSignalL( eventPkg() );
	__CONTROLLER( "CMccController::SendMediaSignalL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccController::EnableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccController::EnableViewFinderL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::EnableViewFinderL" )

    TMccVideoSinkSettingBuf localBuffer;
    aMessage.ReadData1FromClientL( localBuffer );
    iResourcePool->EnableViewFinderL( localBuffer() );
	
	__CONTROLLER( "CMccController::EnableViewFinderL, exit" )
    }    

// -----------------------------------------------------------------------------
// CMccController::DisableViewFinderL
// -----------------------------------------------------------------------------
//
void CMccController::DisableViewFinderL( TMMFMessage& /*aMessage*/ )
    {
	__CONTROLLER( "CMccController::DisableViewFinderL" )

    iResourcePool->DisableViewFinderL();

	__CONTROLLER( "CMccController::DisableViewFinderL, exit" )
    }    

// -----------------------------------------------------------------------------
// CMccController::ViewFinderSettingsL
// -----------------------------------------------------------------------------
//   
void CMccController::ViewFinderSettingsL( TMMFMessage& aMessage )
    {
   	__CONTROLLER( "CMccController::ViewFinderSettingsL" )

    TMccGenericMsgBuffer packBuf;
    aMessage.ReadData1FromClientL( packBuf );
    TMccVideoSinkSettingBuf localBuffer;
    aMessage.ReadData2FromClientL( localBuffer );
    iResourcePool->CameraHandlerL().GetViewFinderSettingsL( localBuffer() );
    aMessage.WriteDataToClientL( localBuffer );

	__CONTROLLER( "CMccController::ViewFinderSettingsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetCamInfoL
// Puts supported codecs into fourcc array
// -----------------------------------------------------------------------------
//
void CMccController::GetCamInfoL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetCamInfoL" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );
        
    TCameraInfo camInfo;
    TPckgBuf<TCameraInfo> camBuffer( camInfo );
    
    CMccCameraHandler* cameraHandler = 0;
    
    // Disabling a PC-lint warning about passing an expression to TRAP_IGNORE macro
    /*lint -e665*/
    TRAP_IGNORE( cameraHandler = &iResourcePool->CameraHandlerL( EFalse, package().iParam2 ) )
    /*lint +e665*/
    
    if ( !cameraHandler )
        {
        // Create temporary camera handler
        cameraHandler = iResourcePool->CreateNewCameraHandlerL( package().iParam2 );
        CleanupStack::PushL( cameraHandler );
        cameraHandler->GetCamInfoL( camBuffer(), ETrue );
        CleanupStack::PopAndDestroy( cameraHandler );
        }
    else
        {
        cameraHandler->GetCamInfoL( camBuffer(), ETrue );
        }

    aMessage.WriteDataToClientL( camBuffer );
	__CONTROLLER( "CMccController::GetCamInfoL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetNumOfCamsL
// -----------------------------------------------------------------------------
//
void CMccController::GetNumOfCamsL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetNumOfCamsL" )
    TPckgBuf<TInt> buffer;
    aMessage.ReadData1FromClientL( buffer );

    buffer() = CMccCameraHandler::GetNumOfCamerasL();    

    aMessage.WriteDataToClientL( buffer );
	__CONTROLLER( "CMccController::GetNumOfCamsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetCamValue1L
// -----------------------------------------------------------------------------
//
void CMccController::GetCamValue1L( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetCamValue1L" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );
    
    iResourcePool->CameraHandlerL().GetValueL( package().iMessageFunction,  
                                               package().iParam4 );    

    aMessage.WriteDataToClientL( package );
	__CONTROLLER( "CMccController::GetCamValue1L, exit" ) 
    }

// -----------------------------------------------------------------------------
// CMccController::SetCamValue1L
// -----------------------------------------------------------------------------
//
void CMccController::SetCamValue1L( TMMFMessage& aMessage )
    {
  
	__CONTROLLER( "CMccController::SetCamValue1L" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );
    iResourcePool->CameraHandlerL().SetValueL( package().iMessageFunction, 
                                               package().iParam4 );    

	__CONTROLLER( "CMccController::SetCamValue1L, exit" )
	
    }

// -----------------------------------------------------------------------------
// CMccController::GetCamValue2L
// -----------------------------------------------------------------------------
//
void CMccController::GetCamValue2L( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetCamValue2L" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );

    iResourcePool->CameraHandlerL().GetValueL( package().iMessageFunction, 
                                               package().iParam3 );  

    aMessage.WriteDataToClientL( package );
	__CONTROLLER( "CMccController::GetCamValue2L, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SetCamValue2L
// -----------------------------------------------------------------------------
//
void CMccController::SetCamValue2L( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SetCamValue2L" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );

    iResourcePool->CameraHandlerL().SetValueL( package().iMessageFunction, 
                                               package().iParam3 );    
	__CONTROLLER( "CMccController::SetCamValue2L, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SetAudioRouteL
// -----------------------------------------------------------------------------
//
void CMccController::SetAudioRouteL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::SetAudioRouteL" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );

    TInt session = FindSessionL( package().iSessionID );
    
    iSessionArray[ session ]->SetAudioRouteL( package().iLinkID,
                                              package().iStreamID,
                                              package().iParam2 );
      
	__CONTROLLER( "CMccController::SetAudioRouteL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccController::GetAudioRouteL
// -----------------------------------------------------------------------------
//
void CMccController::GetAudioRouteL( TMMFMessage& aMessage )
    {
	__CONTROLLER( "CMccController::GetAudioRouteL" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );
    
    TInt session = FindSessionL( package().iSessionID );
    
    iSessionArray[ session ]->GetAudioRouteL( package().iLinkID,
                                              package().iStreamID,
                                              package().iParam2 );
      
	aMessage.WriteDataToClientL( package );
	__CONTROLLER( "CMccController::GetAudioRouteL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::SetEndpointParameterL
// -----------------------------------------------------------------------------
//      
void CMccController::SetEndpointParameterL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::SetEndpointParameterL" )
    
	__ASSERT_ALWAYS( iResourcePool, User::Leave( KErrNotReady ) );
    
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );

    MDataSource* source = 0;
    MDataSink* sink = 0;
    TInt err = iResourcePool->FindResource( 0,
                                            package().iEndpointID, 
                                            KNullUid, 
                                            &source, 
                                            &sink );
        
    switch( package().iParam2 )
        {
       	case KMccFileInfo:
	    case KMccFileName:
	    case KMccFileDuration:
	    case KMccFileVideoFrameRate:
        case KMccFileVideoBitRate:
        case KMccFileVideoFrameSize:
        case KMccFileAudioBitRate:
        case KMccFilePosition:
        case KMccFileAudioFourCC:
        case KMccFileVideoFourCC:
            {
            __CONTROLLER( "CMccController::SetEndpointParameterL, file" )
            
            User::LeaveIfError( err );
            __ASSERT_ALWAYS( source, User::Leave( KErrNotFound ) );
            CMccDataSource* dataSource = static_cast<CMccDataSource*>( source );
            TMccFileSourceSettingBuf fileSetting;
            aMessage.ReadData2FromClientL( fileSetting );
            dataSource->SetParameterL( package().iParam2, fileSetting );
            break;
            }
        case KMccFileFastForward:
            {
            __CONTROLLER( "CMccController::SetEndpointParameterL, file" )
            
            User::LeaveIfError( err );
            __ASSERT_ALWAYS( source, User::Leave( KErrNotFound ) );
            CMccDataSource* dataSource = static_cast<CMccDataSource*>( source );
            TPckgBuf<TInt> fastforwardSettingPak;
            aMessage.ReadData2FromClientL( fastforwardSettingPak );
            dataSource->SetParameterL( package().iParam2, fastforwardSettingPak );
            break;
            }
        case KMccDisplayLocalVideo:
            {
            __CONTROLLER( "CMccController::SetEndpointParameterL, display" )
            
            User::LeaveIfError( err );
            __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );
            CMccDataSink* dataSink = static_cast<CMccDataSink*>( sink );
            TMccVideoSinkSettingBuf displaySetting;
            aMessage.ReadData2FromClientL( displaySetting );
            dataSink->SetParameterL( package().iParam2, displaySetting );
            break;
            }
        case KMccRtpCName:
            {
            __CONTROLLER( "CMccController::SetEndpointParameterL, cname" )
            
            // Can be set even if endpoint is not yet there
            HBufC8* cname = HBufC8::NewLC( aMessage.SizeOfData2FromClient() );
            TPtr8 ptrCName( cname->Des() );
            aMessage.ReadData2FromClientL( ptrCName ); 
            TInt session = FindSessionL( package().iSessionID );
            iSessionArray[ session ]->SetParameterL( package().iParam2,
                                                     package().iLinkID,
                                                     package().iStreamID,
                                                     package().iEndpointID,
                                                     ptrCName );
            CleanupStack::PopAndDestroy( cname );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
        
    __CONTROLLER( "CMccController::SetEndpointParameterL,  exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::GetEndpointParameterL
// -----------------------------------------------------------------------------
//
void CMccController::GetEndpointParameterL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::GetEndpointParameterL" )
   
   	__ASSERT_ALWAYS( iResourcePool, User::Leave( KErrNotReady ) );
        
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );

    MDataSource* source = 0;
    MDataSink* sink = 0;
    iResourcePool->FindResource( 0,
                                 package().iEndpointID, 
                                 KNullUid, 
                                 &source, 
                                 &sink );      
    switch( package().iParam2 )
        {
       	case KMccFileInfo:
	    case KMccFileName:
	    case KMccFileDuration:
	    case KMccFileVideoFrameRate:
        case KMccFileVideoBitRate:
        case KMccFileVideoFrameSize:
        case KMccFileAudioBitRate:
        case KMccFilePosition:
        case KMccFileAudioFourCC:
        case KMccFileVideoFourCC:
            {
            __CONTROLLER( "CMccController::GetEndpointParameterL, file" )
            
            TMccFileSourceSettingBuf fileSettingPak;
            aMessage.ReadData2FromClientL( fileSettingPak );
            
            if ( !source )
                {
                // Static file info retrieval (i.e. filesource plugin
                // is not loaded)
                //
                __ASSERT_ALWAYS( package().iParam2 == KMccFileInfo, 
                                 User::Leave( KErrNotReady ) );
                
                TMccCodecInfo codecInfo;
            	CMccUlDlClient::GetCodecDefaultsL( KMccFileSourceUid, 
            	                                   fileSettingPak,
            	                                   codecInfo );                        
                }
            else
                {
                CMccDataSource* dataSource = static_cast<CMccDataSource*>( source );
                
                dataSource->GetParameterL( package().iParam2, fileSettingPak );
                }
            
            aMessage.WriteDataToClientL( fileSettingPak );
            break;
            }
        case KMccFileFastForward:
            {
            __CONTROLLER( "CMccController::GetEndpointParameterL, file" )
            
            __ASSERT_ALWAYS( source, User::Leave( KErrNotReady ) );
            CMccDataSource* dataSource = static_cast<CMccDataSource*>( source );
            TPckgBuf<TInt> fastforwardSettingPak;
            aMessage.ReadData2FromClientL( fastforwardSettingPak );
            dataSource->GetParameterL( package().iParam2, fastforwardSettingPak );
            aMessage.WriteDataToClientL( fastforwardSettingPak ); 
            break;
            }
        case KMccDisplayLocalVideo:
            {
            __CONTROLLER( "CMccController::GetEndpointParameterL, display" )
            
            __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );
            CMccDataSink* dataSink = static_cast<CMccDataSink*>( sink );
            TMccVideoSinkSettingBuf displaySetting;
            aMessage.ReadData2FromClientL( displaySetting );
            dataSink->GetParameterL( package().iParam2, displaySetting );
            aMessage.WriteDataToClientL( displaySetting );
            break;
            }
        case KMccConfigKey:
            {
            __CONTROLLER( "CMccController::GetEndpointParameterL, config" )
            
            __ASSERT_ALWAYS( source, User::Leave( KErrNotFound ) );
            CMccDataSource* dataSource = static_cast<CMccDataSource*>( source );
            TBuf8<KMaxConfigKeyLen> configKey;
            aMessage.ReadData2FromClientL( configKey );
            dataSource->GetParameterL( package().iParam2, configKey );
            aMessage.WriteDataToClientL( configKey );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }

    __CONTROLLER( "CMccController::GetEndpointParameterL,  exit" )        
    }

// -----------------------------------------------------------------------------
// CMccController::UpdateEndpointL
// -----------------------------------------------------------------------------
//    
void CMccController::UpdateEndpointL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::UpdateEndpointL" )
  
   	__ASSERT_ALWAYS( iResourcePool, User::Leave( KErrNotReady ) );
        
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );
    
    HBufC8* updateVal = HBufC8::NewLC( aMessage.SizeOfData2FromClient() );
    TPtr8 ptrUpdateVal( updateVal->Des() );
    aMessage.ReadData2FromClientL( ptrUpdateVal );

    iResourcePool->UpdateResourceL( package().iEndpointID, *updateVal );   
    
    CleanupStack::PopAndDestroy( updateVal );
    }

// -----------------------------------------------------------------------------
// CMccController::ReuseL
// -----------------------------------------------------------------------------
//   
void CMccController::ReuseL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::ReuseL" )
    TMccStreamPckg package;
    aMessage.ReadData1FromClientL( package );
    
    iResourcePool->ReuseL( package().iEndpointID );
    
    __CONTROLLER( "CMccController::ReuseL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccController::ReferenceCountL
// -----------------------------------------------------------------------------
//   
void CMccController::ReferenceCountL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::ReferenceCountL" )
    TMccGenericMsgBuffer package;
    aMessage.ReadData1FromClientL( package );
    
    iResourcePool->ReferenceCountL( package().iEndpointID, package().iParam4 );
    
    aMessage.WriteDataToClientL( package );
    
    __CONTROLLER( "CMccController::ReferenceCountL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccController::RemoveEndpointL
// -----------------------------------------------------------------------------
//   
void CMccController::RemoveEndpointL( TUint32 aEndpointId )
    {
    __CONTROLLER( "CMccController::RemoveEndpointL" )
    
    RArray<TMccResourceParams> affectedStreams;
    CleanupClosePushL( affectedStreams );
    iResourcePool->PrepareEndpointRemovalL( aEndpointId, affectedStreams );
    
    // Stop all affected streams completely (i.e. ones using the endpoint)
    for ( TInt i = 0; i < affectedStreams.Count(); i++ )
        {
        TInt session = FindSessionL( affectedStreams[ i ].iSessionId );
    
        iSessionArray[ session ]->StopL( affectedStreams[ i ].iLinkId, 
     		                             affectedStreams[ i ].iStreamId, 
     	                                 0 );
     	
     	// Stream cannot be used anymore                               
     	iSessionArray[ session ]->UnuseL( affectedStreams[ i ].iLinkId, 
     		                              affectedStreams[ i ].iStreamId );
        }

    CleanupStack::PopAndDestroy( &affectedStreams );
    
    // Now it should be safe to finally remove the endpoint
    iResourcePool->RemoveEndpointL( aEndpointId );
    
    __CONTROLLER( "CMccController::RemoveEndpointL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccController::TranscodeFileL
// -----------------------------------------------------------------------------
//    
void CMccController::TranscodeFileL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::TranscodeFileL" )
   
    TMccTranscodeFileMsgBuffer fileMsgPkg;
    aMessage.ReadData1FromClientL( fileMsgPkg );
    
	TMccCreateSessionPckg sessionMsgPkg;
    
    if ( !iTranscoder )
        {
        iTranscoder = reinterpret_cast< CMccTranscoder* >(
            REComSession::CreateImplementationL( 
                KUidMccTranscoder, iTranscoderKey ) );
        }
    if ( !iTranscoder )
        {
        User::Leave( KErrNotFound );   
        }
 
    TUint32 sessionId = 0;
    iTranscoder->RegisterEventObserver( *this );

    iTranscoder->TranscodeFileL( sessionId, fileMsgPkg );
  
    sessionMsgPkg().iSessionID = sessionId;
    aMessage.WriteDataToClientL( sessionMsgPkg );
    
	__CONTROLLER( "CMccController::TranscodeFileL, exit" )
    }
        
// -----------------------------------------------------------------------------
// CMccController::CancelTranscodeFileL
// -----------------------------------------------------------------------------
//        
void CMccController::CancelTranscodeFileL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::CancelTranscodeFileL" )

    TMccCreateSessionPckg pckg;
    aMessage.ReadData1FromClientL( pckg );
    
    if ( iTranscoder )
        {
        iTranscoder->CancelTranscodeFileL( pckg().iSessionID );
        }
        
	__CONTROLLER( "CMccController::CancelTranscodeFileL, exit" )
    }
        
// CMccController::RateAdaptationRequest
// -----------------------------------------------------------------------------
//    
TInt CMccController::RateAdaptationRequest( 
    const TMccEvent& /*aInputData*/, 
    TMccEvent& /*aOutputData*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccController::RateAdaptationAlert
// -----------------------------------------------------------------------------
//    
TInt CMccController::RateAdaptationAlert( 
    const TMccEvent& /*aAdaptationData*/,
    TAlertType /*aAlertType*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccController::StoreAndSendEvent
// -----------------------------------------------------------------------------
//
TInt CMccController::StoreAndSendEvent( const TMccEvent& aEvent )
    {
    TRAPD( err, StoreEventL( aEvent ) );
    if ( !err )
        {
        this->SendEvent(); 
        }
    return err;	   
    }
    
// -----------------------------------------------------------------------------
// CMccController::StoreEventL
// -----------------------------------------------------------------------------
//
void CMccController::StoreEventL( const TMccEvent& aEvent )
    {
    // RArray cannot be used since TMccEvent size exceeds 640 bytes
    TMccEvent* event = new ( ELeave ) TMccEvent( aEvent );
    CleanupStack::PushL( event );
    iEventBuf.AppendL( event );
    CleanupStack::Pop( event );
    }

// -----------------------------------------------------------------------------
// CMccController::StreamActionL
// -----------------------------------------------------------------------------
//
void CMccController::StreamActionL( 
    const TMccEvent& aEvent, 
    const TMccEvent* aAdditionalEvent )
    {
    __CONTROLLER( "CMccController::StreamActionL" )
    
    if ( aAdditionalEvent )
        {
        __CONTROLLER( "CMccController::StreamActionL, handling additional event first" )
        
        StreamActionL( *aAdditionalEvent, NULL );
        }
    
    switch ( aEvent.iEventType )
        {
        case KMccStandbyInactivityEvent:
        case KMccStreamPaused:
            {
            PauseByEndPointIdL( aEvent, aEvent.iEndpointId );
            break;
            }
        case KMccStandbyActivityEvent:
        case KMccStreamResumed:
            {
            ResumeByEndPointIdL( aEvent, aEvent.iEndpointId );
            break;
            }
        default:
            {
            break;
            }
        }
        
    __CONTROLLER( "CMccController::StreamActionL, exit" )
    }
			
// -----------------------------------------------------------------------------
// CMccController::CancelMmfMessage
// -----------------------------------------------------------------------------
//  
void CMccController::CancelMmfMessage()
    {
    if ( iMessage )
        {
        iMessage->Complete( KErrCancel ); 
        delete iMessage;
        iMessage = 0;
        }  
    }

// -----------------------------------------------------------------------------
// CMccController::CreationProcessEnd
// Called by link creator when link creation process has ended
// -----------------------------------------------------------------------------
//
void CMccController::CreationProcessEnd( const TMccCreateLinkPckg& aClientData, 
    CMccAsyncLinkCreator* aCreator, TInt aError )
    {
    __CONTROLLER( "CMccController::CreationProcessEnd IN" )
    
        
    TMccNetSettings netSettings;
    netSettings.iLocalAddress = aClientData().iLocalAddress;
    netSettings.iLocalRtcpPort =  aClientData().iLocalRtcpAddress.Port();
    netSettings.iMediaQosValue = aClientData().iIpTOS;
    TMccNetSettingsPackage netSettingsPkg = netSettings;
    
    // Send event to client to notify that a new link is now created
    TMccEvent event;
    event.iSessionId = aClientData().iSessionID;
    event.iLinkId = aClientData().iLinkID;
    event.iEventType = KMccLinkCreated;
    event.iEventData = netSettingsPkg;
    event.iErrorCode = aError;
    
    if ( NULL != aCreator )
    	{
        TInt index = iCreatorArray.Find( aCreator );

        if( KErrNotFound != index )
            {
            __CONTROLLER( "CMccController::CreationProcessEnd FOUND delete" )
            iCreatorArray.Remove( index );
            }
			
		delete aCreator;
		}

    SendMccEventToClient( event );
    }

// -----------------------------------------------------------------------------
// CMccController::BindContextIntoStreamL
// -----------------------------------------------------------------------------
//  
void CMccController::BindContextIntoStreamL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::BindContextIntoStreamL" )
    
    TMccCryptoContextPckg cryptoContextPckg;
    aMessage.ReadData1FromClientL( cryptoContextPckg );

    TMccSecurityDestinationBuffer secDestBuf;
    aMessage.ReadData2FromClientL( secDestBuf );
        
    TInt session = FindSessionL( secDestBuf().iSessionID );
    
    iSessionArray[ session ]->BindContextIntoStreamL( secDestBuf().iLinkID, 
     		                                          secDestBuf().iStreamID,
     		                                          secDestBuf().iEndpointID,
     	                                              cryptoContextPckg() );
     	                                              
	__CONTROLLER( "CMccController::BindContextIntoStreamL, exit" ) 
    }              

// -----------------------------------------------------------------------------
// CMccController::RemoveContextL
// -----------------------------------------------------------------------------
//  
void CMccController::RemoveContextL( TMMFMessage& aMessage )
    {
    __CONTROLLER( "CMccController::RemoveContextL" )
    TMccSecurityDestinationBuffer secDestBuf;
    aMessage.ReadData2FromClientL( secDestBuf );
        
    TInt session = FindSessionL( secDestBuf().iSessionID );
    
    iSessionArray[ session ]->RemoveContextL( secDestBuf().iLinkID, 
     		                                  secDestBuf().iStreamID,
     		                                  secDestBuf().iEndpointID );
     	                                              
	__CONTROLLER( "CMccController::RemoveContextL, exit" ) 
    }      

// -----------------------------------------------------------------------------
// CMccController::MccTranscoderEventReceived
// -----------------------------------------------------------------------------
//  
void CMccController::MccTranscoderEventReceived( TMccEvent& aEvent )
	{
	SendMccEventToClient( aEvent );
	}

// -----------------------------------------------------------------------------
// CMccController::AddCodec
// -----------------------------------------------------------------------------
//
void CMccController::AddCodec(
    const TUint32& aCodec,
    const RArray< TFourCC >& aSupportedOutCodecs,
    const RArray< TFourCC >& aSupportedInCodecs,
    TFixedArray<TFourCC, KMccFourCCArraySize>& aCodecs,
    TInt aIndex ) const
    {
    TFourCC codec( aCodec );
    #if ( defined __WINSCW__ ) || ( defined __WINS__ )
        // In emulator, CMMFDevSound does not return the same list as in HW
        aCodecs[ aIndex ] = codec;
        aSupportedOutCodecs; // silence compiler warning
        aSupportedInCodecs; // silence compiler warning
    #else
        if ( aSupportedOutCodecs.Find( codec ) >= 0 &&
             aSupportedInCodecs.Find( codec ) >= 0 )
            {
            aCodecs[ aIndex ] = codec;
            }
    #endif
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
