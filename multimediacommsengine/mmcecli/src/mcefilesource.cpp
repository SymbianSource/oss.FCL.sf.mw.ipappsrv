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





#include "mcefilesource.h"
#include "mcecomfilesource.h"
#include "mceitcsender.h"
#include "mcestreamobserver.h"
#include "mcevideostream.h"
#include "mceaudiostream.h"
#include "mcemanager.h"
#include "mce.h"
#include "mcesession.h"
#include "mceserial.h"
#include "utf.h"
#include "mceevents.h"
#include "mceclilogs.h"

#define _FLAT_DATA static_cast<CMceComFileSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSource* CMceFileSource::NewL(
            CMceManager& aManager,
            const TFileName& aFileName )
    {
    CMceFileSource* self = CMceFileSource::NewLC( aManager, aFileName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSource* CMceFileSource::NewLC(
            CMceManager& aManager,
            const TFileName& aFileName )
    {
    CMceFileSource* self = new (ELeave) CMceFileSource();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager, aFileName );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSource::~CMceFileSource()
    {
    if ( iManager && iFlatData )
        {
        TRAP_IGNORE( DoCancelTranscodeL() );
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSource::EnableL()
    {
    MCECLI_DEBUG("CMceFileSource::EnableL, Entry");
    
    CMceMediaSource::DoEnableL();
    
    MCECLI_DEBUG("CMceFileSource::EnableL, Exit");
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSource::DisableL()
    { 
    MCECLI_DEBUG("CMceFileSource::DisableL, Entry");
    
    CMceMediaSource::DoDisableL();
    
    MCECLI_DEBUG("CMceFileSource::DisableL, Exit");
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CMceFileSource::MediaElementCountL( TMceMediaType aType ) const
    {
    if ( aType == KMceAudio )
        {
        return FLAT_DATA( iFileInfo.iAudioElementCount );
        }
    else if ( aType == KMceVideo )
        {
        return FLAT_DATA( iFileInfo.iVideoElementCount );
        }
   
    return 0;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMceFileSource::SetCurrentMediaElementL( 
            TMceMediaType aType, 
            TInt aIndex )
    {
    __ASSERT_ALWAYS( aIndex >= 0 &&
                     aIndex < MediaElementCountL( aType ), 
                     User::Leave( KErrArgument ) );
                     
    if ( aType == KMceAudio )
        {
        FLAT_DATA( iCurrentAudioElement ) = aIndex;
        }
    else if ( aType == KMceVideo )
        {
        FLAT_DATA( iCurrentVideoElement ) = aIndex;
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//      
EXPORT_C TInt CMceFileSource::CurrentMediaElement( TMceMediaType aType )
    {
    if ( aType == KMceAudio )
        {
        return FLAT_DATA( iCurrentAudioElement );
        }
    else if ( aType == KMceVideo )
        {
        return FLAT_DATA( iCurrentVideoElement );
        }
    
    return 0;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//      
EXPORT_C void CMceFileSource::SetPositionL( 
            const TTimeIntervalMicroSeconds& aPosition )
    {
    MCECLI_DEBUG("CMceFileSource::SetPositionL, Entry");
    
    __ASSERT_ALWAYS( aPosition <= DurationL() &&
                     aPosition >= TTimeIntervalMicroSeconds( 0 ), 
                     User::Leave( KErrArgument ) );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
                
        TMceIds ids;
    	iStream->Session()->PrepareForITC( ids );
    	ids.iMediaID  = iStream->Id();
    	ids.iSourceID = Id();
    	
    	TMceItcArgTime position( aPosition );
    	
    	iStream->Session()->ITCSender().WriteL( ids, EMceItcSetFilePosition, position );
        
        FLAT_DATA( iPosition ) = position();	
        }
    else
        {
        FLAT_DATA( iPosition ) = aPosition;
        MCECLI_DEBUG("CMceDisplaySink::SetPositionL, done locally");
        }
    MCECLI_DEBUG("CMceFileSource::SetPositionL, Exit");
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//          
EXPORT_C TTimeIntervalMicroSeconds CMceFileSource::PositionL() const
    {
    MCECLI_DEBUG("CMceFileSource::PositionL, Entry");
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        
        TPckgBuf<TTimeIntervalMicroSeconds> pckg( FLAT_DATA( iPosition ) );
        
        TMceIds ids;
    	iStream->Session()->PrepareForITC( ids );
    	ids.iMediaID  = iStream->Id();
    	ids.iSourceID = Id();
    	
    	TMceItcArgTime position;
    	
    	iStream->Session()->ITCSender().ReadL( ids, EMceItcFilePosition, position );
        
        FLAT_DATA( iPosition ) = position();	
        }
    else
        {
        MCECLI_DEBUG("CMceDisplaySink::PositionL, done locally");
        }
        
    MCECLI_DEBUG("CMceFileSource::PositionL, Exit");
    return FLAT_DATA( iPosition );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//          
EXPORT_C TTimeIntervalMicroSeconds CMceFileSource::DurationL() const
    {
    return FLAT_DATA( iFileInfo.iDuration );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::SetFastForwardL( TBool aUseFFWD )
    {
    MCECLI_DEBUG("CMceFileSource::SetFastForwardL, Entry");
    
    __ASSERT_ALWAYS( this->MediaElementCountL( KMceVideo ) > 0, 
                     User::Leave( KErrNotSupported ) );

    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
                         
        TMceIds ids;
    	iStream->Session()->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
    	ids.iSourceID  = Id();
    	ids.iState     = aUseFFWD; 
    	
    	iStream->Session()->ITCSender().SendL( ids, EMceItcSetFastForward );
    	
    	FLAT_DATA( iFastForward ) = static_cast<TBool>( ids.iState );
        }
    else
        {
        MCECLI_DEBUG("CMceDisplaySink::SetFastForwardL, done locally");
        FLAT_DATA( iFastForward ) = aUseFFWD;
        }
    
    if ( FLAT_DATA( iFastForward ) )
        {
        FLAT_DATA( iFastRewind ) = EFalse ;
        }

    MCECLI_DEBUG("CMceFileSource::SetFastForwardL, Exit");
        
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::SetFastRewindL( TBool aUseFRWD )
    {
    MCECLI_DEBUG("CMceFileSource::SetFastRewindL, Entry");
    
    __ASSERT_ALWAYS( this->MediaElementCountL( KMceVideo ) > 0, 
                     User::Leave( KErrNotSupported ) );

    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
                         
        TMceIds ids;
    	iStream->Session()->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
    	ids.iSourceID  = Id();
    	ids.iState     = aUseFRWD; 
    	
    	iStream->Session()->ITCSender().SendL( ids, EMceItcSetFastRewind );
    	
    	FLAT_DATA( iFastRewind ) = static_cast<TBool>( ids.iState );
        }
    else
        {
        MCECLI_DEBUG("CMceDisplaySink::SetFastRewindL, done locally");
        FLAT_DATA( iFastRewind ) = aUseFRWD;
        }
        
    if ( FLAT_DATA( iFastRewind ) )
        {
        FLAT_DATA( iFastForward ) = EFalse ;
        }
    MCECLI_DEBUG("CMceFileSource::SetFastRewindL, Exit");
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::UpdateL ( const TFileName& aFileName )
    {
    MCECLI_DEBUG("CMceFileSource::UpdateL, Entry");
    MCECLI_DEBUG_SVALUE("file", aFileName );
    
    FLAT_DATA( iFileInfo.iFileName ) = aFileName;
    
    __ASSERT_ALWAYS( iManager, User::Leave( KErrNotReady ) );
    
    MCECLI_DEBUG("CMceFileSource::UpdateL, querying file info");
    GetFileInfoL( EFalse );
                        
    SynchronizeWithStreamL();
    
    // If any of the streams need transcoding, filesource needs to be paused
    TBool transcodingRequired = EFalse;
    TInt index = 0;
    while( index < iParents.Count() && !transcodingRequired )
        {
        transcodingRequired = 
         ( iParents[ index++ ]->State() == CMceMediaStream::ETranscodingRequired );
        }
        
    if ( transcodingRequired )
        {
        MCECLI_DEBUG("CMceFileSource::UpdateL, transcoding required");
        if ( IsEnabled() )
            {
            // If enabled, filesource has to be disabled
            MCECLI_DEBUG("CMceFileSource::UpdateL, disabling file source");
            DisableL();
            }
        else
            {
            // If already disabled, stream state change can be notified immediately
            Updated();
            }
        }
    else
        {
        SetFileInfoL();
        }

    MCECLI_DEBUG("CMceFileSource::UpdateL, Exit");
        
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::TranscodeL( const TFileName& aFileName )
    {
    MCECLI_DEBUG("CMceDisplaySink::TranscodeL, Entry");
    MCECLI_DEBUG_SVALUE("file", aFileName );
    
    __ASSERT_ALWAYS( iManager, User::Leave( KErrNotReady ) );
    
    FLAT_DATA( iTranscodeInfo.iFileName ) = aFileName;
	FLAT_DATA( iTranscodeInfo.iQuality ) = 1;
	FLAT_DATA( iTranscodeInfo.iError ) = KErrNone;
	FLAT_DATA( iTranscodeInfo.iProgress ) = 0;
	
	if ( FLAT_DATA( iID ) == KMceMediaIdNotAssigned )
	    {
	    FLAT_DATA( iID ) = iManager->NextMediaId();
	    }

	TUint sessionid = iManager->TranscodeL( *this );
    
	FLAT_DATA( iTranscodeInfo.iTranscodeSessionId ) = sessionid;
	
    UpdateState( CMceMediaStream::ETranscoding );
        
    MCECLI_DEBUG("CMceDisplaySink::TranscodeL, Exit");
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::CancelTranscodeL()
    {
    MCECLI_DEBUG("CMceDisplaySink::CancelTranscodeL, Entry");
    
    DoCancelTranscodeL();
 
    SynchronizeWithStreamL(); 
        
    MCECLI_DEBUG("CMceDisplaySink::CancelTranscodeL, Exit");
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void CMceFileSource::UpdateState( CMceMediaStream::TState aState )
    {
    TInt index = 0;
    while( index < iParents.Count() )
        {
        iParents[ index++ ]->SetState( aState );
        }
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void CMceFileSource::SynchronizeWithStreamL()
    {
    TInt index = 0;
    while( index < iParents.Count() )
        {
        iParents[ index++ ]->SynchronizeWithFileL( *this );
        }
    }
    
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C TInt CMceFileSource::TranscodingProgressL() const
    {
    __ASSERT_ALWAYS( iManager, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iParents.Count() > 0, User::Leave( KErrNotReady ) );
    
    return FLAT_DATA( iTranscodeInfo.iProgress );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceFileSource::InitializeL( CMceManager* aManager )
    {
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    iManager = aManager;
    
    if ( !FLAT_DATA( iInitialInfoRetrieved ) )
        {
        MCECLI_DEBUG("CMceFileSource::InitializeL, querying file info");
        GetFileInfoL();
        
        FLAT_DATA( iInitialInfoRetrieved ) = ETrue;

        }

        
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC8& CMceFileSource::SupportedVideo()
    {
    return FLAT_DATA( iFileInfo.iVideoCodec );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC8& CMceFileSource::SupportedAudio()
    {
    return FLAT_DATA( iFileInfo.iAudioCodec );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TMceFileInfo& CMceFileSource::FileInfo()
    {
    return FLAT_DATA( iFileInfo );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceFileSource::StreamAddedL( CMceMediaStream& aParent )
    {
    CMceMediaSource::StreamAddedL( aParent );
    
    if ( iManager ) //not intialized
        {
        SynchronizeWithStreamL();
        }
    
    }
    
    
    
// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceFileSource* CMceFileSource::NewL()
    {
    CMceFileSource* self = CMceFileSource::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceFileSource* CMceFileSource::NewLC()
    {
    CMceFileSource* self = new (ELeave) CMceFileSource();
    CleanupStack::PushL( self );
    self->ConstructL( NULL, KNullDesC() );
    return self;
    }    
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
CMceFileSource::CMceFileSource()
    {    
    }


// -----------------------------------------------------------------------------
// CMceFileSource::Updated
// -----------------------------------------------------------------------------
//
void CMceFileSource::Updated()
    {
    MMceStreamObserver* observer = iManager->MediaObserver();
    
    if ( observer && iParents.Count() > 0 )
        {
        observer->StreamStateChanged( *iParents[ 0 ], *this );
        }
    }


// -----------------------------------------------------------------------------
// CMceFileSource::EventReceivedL
// -----------------------------------------------------------------------------
//
TInt CMceFileSource::EventReceivedL( TMceEvent& aEvent )
    {
    TInt status = CMceMediaSource::EventReceivedL( aEvent );
    if ( status != KMceEventNotConsumed )
        {
        return status;
        }
    
    if ( aEvent.Id().IsSourceId() )
        {
        if ( aEvent.Action() == EMceItcTranscodingCompleted  )
            {
            // If we leave here, client cannot interpret the failure
            TRAPD( err, TranscodingCompletedL( aEvent ) )
            if ( err )
                {
                // Client will see that transcoding failed as file info is not
                // updated and stream will stay in transcoding needed state.
                MCECLI_DEBUG("CMceFileSource::EventReceivedL, transcoding failed");
                const TInt KMceTranscodingCompletedPercentage = 100;
                FLAT_DATA( iTranscodeInfo.iProgress ) = KMceTranscodingCompletedPercentage;
                }

            SynchronizeWithStreamL();
            
            }
        status = HandleEvent( aEvent );

        }
    else
        {
        status = KMceEventNotConsumed;
        }
    
    return status;    
    
    }

// -----------------------------------------------------------------------------
// CMceFileSource::GetFileInfoL
// -----------------------------------------------------------------------------
//
void CMceFileSource::GetFileInfoL( TBool aUseSession )
    {
    TMceIds ids;
    TMceItcArgTFileInfo fileInfoBuf( FLAT_DATA( iFileInfo ) );
    
    if ( iStream && iStream->Id().IsAssigned() && 
         Id().IsAssigned() && aUseSession )
        {
        
        iStream->Session()->PrepareForITC( ids );
        ids.iMediaID  = iStream->Id();
        ids.iSourceID = Id();
        iStream->Session()->ITCSender().ReadL( ids, EMceItcFileInfo, fileInfoBuf );
        }
    else
        {
        ids.iAppUID = iManager->AppUid().iUid;
        _FLAT_DATA->SenderL( 
            iManager->ServerSession() ).ReadL( ids, EMceItcFileInfo, fileInfoBuf );
        }
    
    FLAT_DATA( iFileInfo ) = fileInfoBuf();

    User::LeaveIfError( FLAT_DATA( iFileInfo.iAccessRights ) == TMceFileInfo::EZero ?
                        KErrAccessDenied : KErrNone );
    
    }

// -----------------------------------------------------------------------------
// CMceFileSource::SetFileInfoL
// -----------------------------------------------------------------------------
//
void CMceFileSource::SetFileInfoL()
    {
    
    if ( iStream && iStream->Id().IsAssigned() && 
         Id().IsAssigned() )
        {
        TMceIds ids;
        iStream->Session()->PrepareForITC( ids );
        ids.iMediaID  = iStream->Id();
        ids.iSourceID = Id();
        	
        TMceItcArgTFileInfo fileInfo( FLAT_DATA( iFileInfo ) );
        	
        iStream->Session()->ITCSender().WriteL( ids, EMceItcSetFileInfo, fileInfo );
        }
    }

// -----------------------------------------------------------------------------
// CMceFileSource::HandleEvent
// -----------------------------------------------------------------------------
//
TInt CMceFileSource::HandleEvent( TMceEvent& aEvent )
    {
    MCECLI_DEBUG("CMceFileSource::HandleEvent, Entry");
    TInt status = KMceEventNotConsumed;

    if ( aEvent.Action() == EMceItcTranscodingCompleted ) 
        {
        MCECLI_DEBUG("CMceFileSource::HandleEvent, transcoding completed");
        iManager->TranscodeCompleted( *this );
        status = KMceEventConsumed;
        Updated();
        }
    else if ( aEvent.Action() == EMceItcTranscodingInProgress ) 
        {
        MCECLI_DEBUG("CMceFileSource::HandleEvent, transcoding in progress");
        FLAT_DATA( iTranscodeInfo.iProgress ) = aEvent.ActionData();
        status = KMceEventConsumed;
        Updated();
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceFileSource::HandleEvent, not consumed");
        }
        
    MCECLI_DEBUG_DVALUE("CMceFileSource::HandleEvent, Exit. status", status );
    return status;
    }


// -----------------------------------------------------------------------------
// CMceFileSource::ConstructL
// -----------------------------------------------------------------------------
//  
void CMceFileSource::ConstructL( CMceManager* aManager, 
                                 const TFileName& aFileName )
    {
    CMceComFileSource* source = CMceComFileSource::NewLC();
    CMceMediaSource::ConstructL( source );
    CleanupStack::Pop( source );

    FLAT_DATA( iFileInfo.iFileName ) = aFileName;
    FLAT_DATA( iFileInfo.iDirection ) = TMceFileInfo::ERead;
    
    if ( aManager )
        {        
        InitializeL( aManager );
        }
   
    }

// -----------------------------------------------------------------------------
// CMceFileSource::Manager
// -----------------------------------------------------------------------------
//
CMceManager* CMceFileSource::Manager()
    {
    return iManager;
    }

// -----------------------------------------------------------------------------
// CMceFileSource::TranscodingCompletedL
// -----------------------------------------------------------------------------
//
void CMceFileSource::TranscodingCompletedL( TMceEvent& aEvent )
    {
    // Update file to be the transcoded file only if transcoding
    // completed succesfully
    User::LeaveIfError( aEvent.Id().iStatus );
    
    FLAT_DATA( iFileInfo.iFileName ) = FLAT_DATA( iTranscodeInfo.iFileName );
        
    MCECLI_DEBUG("CMceFileSource::TranscodingCompletedL, querying file info");
    // Get new info outside session
    GetFileInfoL( EFalse );
    
    MCECLI_DEBUG("CMceFileSource::TranscodingCompletedL, setting file info");
    // Set it to the session
    SetFileInfoL();
    }

// -----------------------------------------------------------------------------
// CMceFileSource::DoCancelTranscodeL
// -----------------------------------------------------------------------------
//
void CMceFileSource::DoCancelTranscodeL()
    {
    MCECLI_DEBUG("CMceDisplaySink::DoCancelTranscodeL, Entry");
    
    __ASSERT_ALWAYS( iManager, User::Leave( KErrNotReady ) );
    
    iManager->CancelTranscodeL( *this,
                 FLAT_DATA( iTranscodeInfo.iTranscodeSessionId ) );
                 
    MCECLI_DEBUG("CMceDisplaySink::DoCancelTranscodeL, Exit");
    }
    
// End of File

