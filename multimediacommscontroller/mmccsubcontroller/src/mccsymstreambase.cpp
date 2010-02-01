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
* Description:    Mcc Symmetric subthread stream baseclass
*
*/



// INCLUDE FILES
#include <mmf/server/sounddevice.h>
#include "mccsymstreambase.h"
#include "mccdatapathbase.h"
#include "mccinternaldef.h"
#include "mccsubcontrollerlogs.h"
#include "mccsymsubthreadclient.h"
#include "mccinternalevents.h"
#include "mmccevents.h"
#include "mccresources.h"
#include "mccmultiplexer.h"
#include "mccrtpdatasink.h"
#include "mccrtpdatasource.h"
#include "mmccinterface.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSymStreamBase::CMccSymStreamBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccSymStreamBase::CMccSymStreamBase( 
    TUint32 aMccStreamId, 
    MAsyncEventHandler* aEventhandler, 
    MMccResources* aMccResources,
    CMccRtpManager* aManager, 
    TInt aStreamType ) : 
    iDatapath( NULL ), 
    iEventhandler( aEventhandler ), 
    iMccResources( aMccResources ),
    iDatasink( NULL ), 
    iDatasource( NULL ), 
    iFmtpAttr( NULL ), 
    iRtpmanager( aManager ),
    iSoundDevice( NULL ), 
    iMccStreamId( aMccStreamId ), 
    iType( aStreamType )
    {
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::~CMccSymStreamBase
// Destructor
// -----------------------------------------------------------------------------
//
CMccSymStreamBase::~CMccSymStreamBase()
    {
    HandleSpecialSinksAndSources();
    
    if( iDatasink )
        {
        iDatasink->SinkThreadLogoff();
        iDatasink = 0;
        }
        
    if( iDatapath )
        {
        delete iDatapath;
        iDatapath = 0;
        }
            
    if ( iFmtpAttr )
        {
        delete iFmtpAttr;
        iFmtpAttr = 0;
        }

	iControlledEndpoints.Reset();
	iUncontrolledEndpoints.Reset();
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SendEventToClient
// -----------------------------------------------------------------------------
//    
TInt CMccSymStreamBase::SendEventToClient( const TMMFEvent& aEvent )
	{
	TInt err( KErrNone );

	// Do in a bit ackward way in order to save stack memory
	TMccEvent* mccEventPtr = 0;
	if ( IS_MCC_EVENT( aEvent ) )
	    {
	    mccEventPtr = 
	        reinterpret_cast<const TMccInternalEvent&>( aEvent ).iMccEvent;                                     
	    }
	
	if ( mccEventPtr )
	    {	 
	    __SUBCONTROLLER( "CMccSymStreamBase::SendEventToClient #1" )
	       
	    mccEventPtr->iStreamId = iMccStreamId;
    	err = iEventhandler->SendEventToClient( aEvent );	
	    }
	else
	    {
	    __SUBCONTROLLER( "CMccSymStreamBase::SendEventToClient #2" )
	    
	    // Fill stream id for all MMF events and try to fill endpointId as well
	    
	    TUint32 endpointId = ( iDatasink->DataSinkType() == KUidMmfAudioOutput ) ?
	         MCC_ENDPOINT_ID( iDatasink ) : 0;
        endpointId = ( iDatasource->DataSourceType() == KUidMmfAudioInput ) ?
            MCC_ENDPOINT_ID( iDatasource ) : endpointId;
    
	    err = SendStreamEventToClient( KMccStreamMMFEvent, endpointId, aEvent.iErrorCode );
	    }
    
	__SUBCONTROLLER( "CMccSymStreamBase::SendEventToClient, exit" )
    return err;
	}

// -----------------------------------------------------------------------------
// CMccSymStreamBase::MultiplexerSink
// -----------------------------------------------------------------------------
//
MDataSink* CMccSymStreamBase::MultiplexerSink()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SetStateL
// Sets the new state of the stream
// -----------------------------------------------------------------------------
//
TBool CMccSymStreamBase::SetStateL( TStreamState aState, TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccSymStreamBase::SetStateL" )
    
    TBool stateOK = EFalse;
    
    TBool controlNetworkResources = ETrue;
     
    MMccResources::TResourceState resourceState = MMccResources::EConstructed;
    
    switch( aState )
        {
        case EStateNone:
            __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, EStateNone" )
            if( EStateStopped == iState )
                {
                resourceState = MMccResources::EConstructed;
                stateOK = ETrue;
                }
            break;
        case EStatePrepared:
            __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, EStatePrepared" )
            resourceState = MMccResources::EPrepared;
            stateOK = ETrue;
            break;
        case EStateStreaming:
            __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, EStateStreaming" )
            resourceState = MMccResources::EStarted;
            stateOK = ETrue;
            break;
        case EStatePaused:
            __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, EStatePaused" )
            resourceState = MMccResources::EResourcePaused;
            stateOK = ETrue;
            break;
        case EStateStopped:
            __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, EStateStopped" )
            resourceState = MMccResources::EResourceStopped;
            stateOK = ETrue;
            break;
        default:
            stateOK = EFalse;
            break;
        }
    
    __ASSERT_ALWAYS( stateOK, User::Leave( KErrArgument ) );

    if ( resourceState != MMccResources::EConstructed )
        {
        __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, set resource state" )
        
        iControlledEndpoints.Reset();
        iUncontrolledEndpoints.Reset();
        
        iMccResources->SetResourceStateL( iMccStreamId,
                                          aEndpointId, 
                                          resourceState, 
                                          controlNetworkResources,
                                          iControlledEndpoints,
                                          iUncontrolledEndpoints );
        
        // Update multiplexer
        HandleMultiplexerL();
        
        // Update sink
        HandleSpecialSinksAndSources();
        }
    
    // Stream state follows network resource state or in case of
    // local stream any change affect to the stream state 
    if ( controlNetworkResources || LocalStream() )
        {
        iState = aState;
        }
    
    __SUBCONTROLLER( "CMccSymStreamBase::SetStateL, exit" )
        
    return controlNetworkResources;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::CurrentState
// Returns the current state of the stream
// -----------------------------------------------------------------------------
//
CMccSymStreamBase::TStreamState CMccSymStreamBase::CurrentState()
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SetGainL
// Sets the current gain
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::SetGainL( TInt aGain )
    {
    if( iSoundDevice )
	    {
        iSoundDevice->SetGain( aGain );
        }
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SetVolumeL
// Sets the current volume
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::SetVolumeL( TInt aVolume )
    {
    if( iSoundDevice )
        {
        iSoundDevice->SetVolume( aVolume );
        }
    else if ( iDatasink && iDatasink->DataSinkType() == KMccVideoSinkUid )
        {
        CMccDataSink* sink = static_cast<CMccDataSink*>( iDatasink );
        sink->SetParameterL( KMccSpeakerVolume, TPckgBuf<TInt>( aVolume ) );
        }
    else
        {
        // NOP
        }
    }


// -----------------------------------------------------------------------------
// CMccSymStreamBase::SetVolumeL
// Sets the current volume
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::SetMMFPriority( 
                const TMMFPrioritySettings& aPriority )
	{
	__SUBCONTROLLER( "CMccSymStreamBase::SetPriorityToCodecsL entry " )
	//Check if the codec info priority is same otherwise don't do it
	if ( iPrioritySettings.iPref != aPriority.iPref ||
		 iPrioritySettings.iState != aPriority.iState ||
		 iPrioritySettings.iPriority != aPriority.iPriority )
		{
		__SUBCONTROLLER( " Different from privous setting " )
		TRAPD( err, SetPrioritySettingsL( aPriority ) );	
		
		__SUBCONTROLLER_INT1 ( " Setting priority preference to", aPriority.iPref );
		__SUBCONTROLLER_INT1 ( " Setting priority state to ", aPriority.iState );
		__SUBCONTROLLER_INT1 ( " Setting client priority to ", aPriority.iPriority );
		__SUBCONTROLLER_INT1 ( " SetPrioritySettingsL err =", err );
		
		if ( err == KErrNone )
			{
			iPrioritySettings = aPriority;
			}
		else
			{
			//Ignore error 
			__SUBCONTROLLER_INT1 ( " Sink or Source is not existed err =", err );
		
			err = KErrNone;
			}
		}
	__SUBCONTROLLER( "CMccSymStreamBase::SetPriorityToCodecsL exit " )
    }
// -----------------------------------------------------------------------------
// CMccSymStreamBase::MaxVolumeL
// Return the maximum volume
// -----------------------------------------------------------------------------
//
TInt CMccSymStreamBase::MaxVolumeL()
    {
    if( iSoundDevice )
        {
        return iSoundDevice->MaxVolume();
        }
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::MaxGainL
// Return the maximum gain
// -----------------------------------------------------------------------------
//
TInt CMccSymStreamBase::MaxGainL()
    {
    if( iSoundDevice )
        {
        return iSoundDevice->MaxGain();
        }
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::VolumeL
// Return the current volume
// -----------------------------------------------------------------------------
//
TInt CMccSymStreamBase::VolumeL()
    {
    if( iSoundDevice )
        {
        return iSoundDevice->Volume();
        }
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::GainL
// Return the current gain
// -----------------------------------------------------------------------------
//
TInt CMccSymStreamBase::GainL()
    {
    if( iSoundDevice )
        {
        return iSoundDevice->Gain();
        }
    return KErrNone;    
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SendMediaSignalL
// Default implementation of media signal sending
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::SendMediaSignalL( const TMccEvent& /*aEvent*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SendRTCPReceiverReportL
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::SendRTCPReceiverReportL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SendRTCPSenderReportL
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::SendRTCPSenderReportL()
    {
    User::Leave( KErrNotSupported );
    }
        
// -----------------------------------------------------------------------------
// CMccSymStreamBase::SendRTCPDataL
// Sends Non-RTCP data
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::SendRTCPDataL( const TDesC8& /*aData*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::StartInactivityTimerL
// Starts inactivity timer for a stream
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::StartInactivityTimerL( TUint32 /*aTimeoutTime*/ )
    {
    User::Leave( KErrNotSupported );
    }

      
// -----------------------------------------------------------------------------
// CMccSymStreamBase::StopInactivityTimerL
// Stops inactivity timer for a stream 
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::StopInactivityTimerL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::GetFmtpL
// Fetches the FMTP attribute
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::GetFmtpL( TDes8& aFmtp )
    {
    __ASSERT_ALWAYS ( iFmtpAttr, User::Leave(KErrArgument) );

	aFmtp.Copy( iFmtpAttr->Des() );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::GetCodecL
// Fetches codec info
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::GetCodecL( TMccCodecInfo& aCodec )
    {
    aCodec = iCodecInfo;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SetAudioRouteL
// -----------------------------------------------------------------------------
//    
void CMccSymStreamBase::SetAudioRouteL( TUint32 /*aRoutingDestination*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::GetAudioRouteL
// -----------------------------------------------------------------------------
//     
void CMccSymStreamBase::GetAudioRouteL( TUint32& /*aRoutingDestination*/ )
    {
    User::Leave( KErrNotSupported );
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::Type
// -----------------------------------------------------------------------------
//     
const TUid CMccSymStreamBase::Type()
    {
    TUid type = KUidMediaTypeAudio;
    
    if (iType == KMccVideoUplinkStream ||
        iType == KMccVideoDownlinkStream ||
        iType == KMccVideoLocalStream )
        {
        type = KUidMediaTypeVideo;
        }
        
    return type;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::LocalStream
// -----------------------------------------------------------------------------
//     
TBool CMccSymStreamBase::LocalStream() const
    {
    return ( iType == KMccVideoLocalStream || iType == KMccAudioLocalStream );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::StandbyStream
// -----------------------------------------------------------------------------
//     
TBool CMccSymStreamBase::StandbyStream() const
    {
    return ( iType == KMccAudioDownlinkStandbyStream );
    }
     
// -----------------------------------------------------------------------------
// CMccSymStreamBase::InformStreamStateChange
// -----------------------------------------------------------------------------
//   
void CMccSymStreamBase::InformStreamStateChange( 
    TMccEventType aEventType, TUint32 aEndpointId )
    {
    __SUBCONTROLLER_INT2( "CMccSymStreamBase::InformStreamStateChange, \
    aEventType=", aEventType, "aEndpointId=", aEndpointId )
    
    // Only state changes of mmf audio and uncontrolled endpoints (e.g. shared
    // ones which are already in state the client tried to set) are generated
    // at this level

    TBool isAudioOutput = ( iDatasink->DataSinkType() == KUidMmfAudioOutput );
    TBool isAudioInput = ( iDatasource->DataSourceType() == KUidMmfAudioInput );
    
    if ( iUncontrolledEndpoints.Count() > 0 )
        {
        __SUBCONTROLLER( "CMccSymStreamBase::InformStreamStateChange, \
        handle uncontrolled endpoints" )
        
        for ( TInt i = 0; i < iUncontrolledEndpoints.Count(); i++ )
            {  
            SendStreamEventToClient( aEventType, iUncontrolledEndpoints[ i ] );
            }
        }
    else if ( iCodecInfo.iType == KUidMediaTypeAudio &&
            ( isAudioOutput || isAudioInput ) )
	    {
	    __SUBCONTROLLER( "CMccSymStreamBase::InformStreamStateChange, \
        report state change" )
	    
        if ( MCC_CONTROL_ALL_ENDPOINTS( aEndpointId ) )
            {
            // Modify endpoint id
            aEndpointId = isAudioOutput ? MCC_ENDPOINT_ID( iDatasink ) :
                                          MCC_ENDPOINT_ID( iDatasource ); 
                                          
            SendStreamEventToClient( aEventType, aEndpointId );
            }
        else if ( ( aEndpointId == MCC_ENDPOINT_ID( iDatasink ) && isAudioOutput ) ||
                  ( aEndpointId == MCC_ENDPOINT_ID( iDatasource ) && isAudioInput ) )
            {
            // MMF endpoint is specificly controlled
            SendStreamEventToClient( aEventType, aEndpointId );
            }
        else
            {
            __SUBCONTROLLER("CMccSymStreamBase::InformStreamStateChange, not reported")
            }
	    }
	else
		{
		__SUBCONTROLLER("CMccSymStreamBase::InformStreamStateChange, not reported")	
		}	
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::HandleStandby
// If stream is paused and standby endpoint (speaker) is controlled, 
// standby is activated as we want that it will be resumed when receiving
// data for this stream. If stream is playing but standby endpoint control
// is non-effective, standby mechanism is re-activated in order to get the
// standby resource later into use (this scenario might occur if client has
// paused the speaker while standby occurs).
// -----------------------------------------------------------------------------
//    
void CMccSymStreamBase::HandleStandby( 
    TUint32 aEndpointId,
    TBool aStreamPaused )
    {  
    TUint32 standbyEndpointId( MCC_ENDPOINT_ID( iDatasink ) );
    TBool standbyEndpointControl( StandbyStream() && 
                                  ( aEndpointId == standbyEndpointId ||
                                    MCC_CONTROL_ALL_ENDPOINTS( aEndpointId ) ) );
    
    TBool isStandbyEndpointControlled( IsControlled( standbyEndpointId ) );
    
    if ( standbyEndpointControl && 
       ( ( aStreamPaused && isStandbyEndpointControlled ) || 
         ( !aStreamPaused && !isStandbyEndpointControlled ) ) )
        {
        CMccRtpDataSource* dSource = static_cast<CMccRtpDataSource*>( iDatasource );
        dSource->StandBy( EActivateStandby, iCodecInfo.iPayloadType );
        }
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SendStreamEventToClient
// -----------------------------------------------------------------------------
//     
TInt CMccSymStreamBase::SendStreamEventToClient( 
    TMccEventType aEventType, 
    TUint32 aEndpointId,
    TInt aError )
    {
    ClearStateChangeEvent();
    
    iStreamStateChangeEvent.iStreamId = iMccStreamId;
    iStreamStateChangeEvent.iEndpointId = aEndpointId;
    iStreamStateChangeEvent.iEventCategory = KMccEventCategoryStream;
    iStreamStateChangeEvent.iEventType = aEventType;
    iStreamStateChangeEvent.iErrorCode = aError;
   
    TMccInternalEvent internalEvent( Type(), 
                                     EMccInternalEventNone,
                                     iStreamStateChangeEvent );
                                     
    return iEventhandler->SendEventToClient( internalEvent );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::HandleMultiplexerL
// -----------------------------------------------------------------------------
//     
void CMccSymStreamBase::HandleMultiplexerL()
    {
    __SUBCONTROLLER( "CMccSymStreamBase::HandleMultiplexerL" )
        
    if ( iMultiplexer )
        {
        MDataSink* multiplexerSink = MultiplexerSink();
        if ( multiplexerSink )
            {
            iMultiplexer->SetCurrentUserL( *multiplexerSink );
            }
        }
        
    __SUBCONTROLLER( "CMccSymStreamBase::HandleMultiplexerL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::HandleSpecialSinksAndSources
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::HandleSpecialSinksAndSources()
    {
    if ( iDatasink && 
       ( iDatasink->DataSinkType() == KUidMccVideoSink || 
         iDatasink->DataSinkType() == KUidMccRtpSink ||
         iDatasink->DataSinkType() == KUidMccFileSink ) )
        {
        static_cast<CMccDataSink*>( iDatasink )->SetCurrentUser( this );
        }
        
    if ( iDatasource && iDatasource->DataSourceType() == KUidMccVideoSource )
        {
        static_cast<CMccDataSource*>( iDatasource )->SetCurrentUser( this );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::MultiplexerCleanup
// -----------------------------------------------------------------------------
//   
void CMccSymStreamBase::MultiplexerCleanup()
    {
    // If multiplexer exists, sourcethread logoff is routed through it to the
    // datasource
    if ( iMultiplexer )
        {
        iMultiplexer->MultiplexerSourceThreadLogoff( MultiplexerSink() );
        iMultiplexer = 0;
        }
    else if ( iDatasource )
        {
        iDatasource->SourceThreadLogoff();
        iDatasource = 0;
        }
    else
        {
        // NOP
        }
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::DoPrimeL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::DoPrimeL()
    {
    for ( TInt i = 0; i < iControlledEndpoints.Count(); i++ )
        { 
        iDatapath->PrimeL( iControlledEndpoints[ i ] );
        }   
    
    iMccResources->ResourceStateSetL();
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::DoPlayL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::DoPlayL()
    {
    for ( TInt i = 0; i < iControlledEndpoints.Count(); i++ )
        { 
        iDatapath->PlayL( iControlledEndpoints[ i ] );
        }  
        
    iMccResources->ResourceStateSetL();  
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::DoPauseL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::DoPauseL()
    {
    for ( TInt i = 0; i < iControlledEndpoints.Count(); i++ )
        { 
        iDatapath->PauseL( iControlledEndpoints[ i ] );
        }    
        
    iMccResources->ResourceStateSetL();
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::DoResumeL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::DoResumeL()
    {
    for ( TInt i = 0; i < iControlledEndpoints.Count(); i++ )
        { 
        iDatapath->ResumeL( iControlledEndpoints[ i ] );
        }    
        
    iMccResources->ResourceStateSetL();
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::DoStopL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::DoStopL()
    {
    for ( TInt i = 0; i < iControlledEndpoints.Count(); i++ )
        { 
        iDatapath->StopL( iControlledEndpoints[ i ] );
        }    
        
    iMccResources->ResourceStateSetL();    
    }
  
// -----------------------------------------------------------------------------
// CMccSymStreamBase::ParsePayloadTypesL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::ParsePayloadTypesL( RArray<TUint>& aPayloadTypes,
        const TMccCodecInfo& aCodecInfo ) const
    {
    __ASSERT_DEBUG( KMccPayloadTypeMax != aCodecInfo.iPayloadType,
        User::Panic( _L("PT PARSE, PT NOT SET"), KErrArgument ) );
    
    aPayloadTypes.AppendL( aCodecInfo.iPayloadType );
    
    if ( KMccPayloadTypeMax != aCodecInfo.iRedundantPayload )
        {
        aPayloadTypes.AppendL( aCodecInfo.iRedundantPayload );
        }
    
    
    if ( KPayloadTypeUndefined != aCodecInfo.iComfortNoiseGenerationPt &&
         aCodecInfo.iEnableDTX )
        {
        aPayloadTypes.AppendL( KCnPayloadType );
        aPayloadTypes.AppendL( KCnPayloadTypeReserved );
        }
    }

  
// -----------------------------------------------------------------------------
// CMccSymStreamBase::IsControllingMmfEndpoint
// -----------------------------------------------------------------------------
//   
TBool CMccSymStreamBase::IsControllingMmfEndpoint() const
    {
    TBool controlling( EFalse );
    for ( TInt i = 0; i < iControlledEndpoints.Count() && !controlling; i++ )
        { 
        TUint32 endpointId = iControlledEndpoints[ i ];
        controlling =
            MCC_CONTROL_ALL_ENDPOINTS( endpointId ) ||
            ( iDatasink->DataSinkType() == KUidMmfAudioOutput && 
              endpointId == MCC_ENDPOINT_ID( iDatasink ) ) ||
            ( iDatasource->DataSourceType() == KUidMmfAudioInput &&
              endpointId == MCC_ENDPOINT_ID( iDatasource ) );
            
        }
    return controlling;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::IsCodecOperationAllowed
// -----------------------------------------------------------------------------
//   
TBool CMccSymStreamBase::IsCodecOperationAllowed() const
    {
    TBool allowed( ETrue );
    
    if ( iDatasink->DataSinkType() == KUidMmfAudioOutput ||
         iDatasource->DataSourceType() == KUidMmfAudioInput )
        {
        allowed = IsControllingMmfEndpoint();
        }
        
    return allowed;
    
    }


// -----------------------------------------------------------------------------
// CMccSymStreamBase::CurrentCodecState
// -----------------------------------------------------------------------------
//   
CMccSymStreamBase::TCodecState CMccSymStreamBase::CurrentCodecState() const
    {
    return iCodecState;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::SetCodecState
// -----------------------------------------------------------------------------
//   
        
void CMccSymStreamBase::SetCodecState( TCodecState aState )
    {
    iCodecState = aState;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::UpdateCodecLoadingPermission
// -----------------------------------------------------------------------------
//     
void CMccSymStreamBase::UpdateCodecLoadingPermission( TBool aLoadingAllowed )
    {
    iCodecLoadingAllowed = aLoadingAllowed;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::CodecLoadingAllowed
// -----------------------------------------------------------------------------
//         
TBool CMccSymStreamBase::CodecLoadingAllowed() const
    {
    return iCodecLoadingAllowed;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::ChangeBitrate
// Traps the call to dynamic bitrate change function
// -----------------------------------------------------------------------------
//
TInt CMccSymStreamBase::ChangeBitrate( TInt aBitrate )
    {
    TRAPD( err, this->DoChangeBitrateL( aBitrate ) ); 
         
    // If the bitrate change is not supported, we may just suppress the error
    // as this function may have been called to e.g a dl stream.
    if( KErrNotSupported == err )
        {
        __SUBCONTROLLER( "CMccSymStreamBase::ChangeBitrate KErrNotSupported" );
        return KErrNone;
        }
    else
        {
        // Something else has happened, maybe bitrate was wrong, so report it
        // back
        __SUBCONTROLLER_INT1( "CMccSymStreamBase::ChangeBitrate ERROR:", err );
        return err;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::DoChangeBitrateL
// Downlink cannot support bitrate change
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::DoChangeBitrateL( TInt /*aBitrate*/ )
    {
    // This will be trapped inside ChangeBitrate() function
    __SUBCONTROLLER( "CMccSymStreamBase::DoChangeBitrateL KErrNotSupported" );
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::BindContextIntoStreamL
// -----------------------------------------------------------------------------
//
void CMccSymStreamBase::BindContextIntoStreamL( TUint32 /*aEndpointId*/,
                                                const TMccCryptoContext& /*aContextParams*/ )
    {              

    __SUBCONTROLLER( "CMccSymStreamBase::BindContextIntoStreamL KErrNotSupported" );
    User::Leave( KErrNotSupported );
    }
    
// -----------------------------------------------------------------------------
// CMccSymStreamBase::RemoveCryptoContext
// -----------------------------------------------------------------------------
//
TInt CMccSymStreamBase::RemoveContext( TUint32 aEndpointId )
    {
    if ( iDatasource && iDatasource->DataSourceType() == KMccRtpSourceUid &&
         ( aEndpointId == MCC_ENDPOINT_ID( iDatasource ) || 0 == aEndpointId ) )
	    {
	    static_cast<CMccRtpDataSource*>( iDatasource )->RemoveCryptoContext();
	    } 
	
    if ( iDatasink && iDatasink->DataSinkType() == KMccRtpSinkUid &&
	     ( aEndpointId == MCC_ENDPOINT_ID( iDatasink ) || 0 == aEndpointId ) )
	    {
	    static_cast<CMccRtpDataSink*>( iDatasink )->RemoveCryptoContext();
	    } 
	       
    return KErrNone;    
    }    

// -----------------------------------------------------------------------------
// CMccSymStreamBase::IsControlled
// -----------------------------------------------------------------------------
//   
TBool CMccSymStreamBase::IsControlled( TUint32 aEndpointId ) const
    {
    TBool controlled( EFalse );
    for ( TInt i = 0; i < iControlledEndpoints.Count() && !controlled; i++ )
        { 
        controlled = ( iControlledEndpoints[ i ] == aEndpointId );
        }
    return controlled;
    }

// -----------------------------------------------------------------------------
// CMccSymStreamBase::ClearStateChangeEvent
// -----------------------------------------------------------------------------
// 
void CMccSymStreamBase::ClearStateChangeEvent()
    {
    iStreamStateChangeEvent = TMccEvent();
    }
          
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
