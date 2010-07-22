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



        
// INCLUDE FILES
#include <e32def.h>
#include <CCMRMediaRecorder.h>

#include "mccvideosourcewrapper.h"
#include "mccvideosourcesinklogs.h"

#define VIDEO_SOURCE_ASSERT_READINESS \
__ASSERT_ALWAYS( iMediaRecorder && !iFatalErrorOccured, User::Leave( KErrNotReady ) );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::NewL
// -----------------------------------------------------------------------------
//
CMccVideoSourceWrapper* CMccVideoSourceWrapper::NewL(
    MCMRMediaSink& aObserver, 
    MCMRMediaRecorderObserver& aObserver2 )
    {
    CMccVideoSourceWrapper* self = new ( ELeave ) CMccVideoSourceWrapper(
        aObserver, aObserver2 );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::ConstructL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::ConstructL()
    {
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::CMccVideoSourceWrapper
// -----------------------------------------------------------------------------
//
CMccVideoSourceWrapper::CMccVideoSourceWrapper(
    MCMRMediaSink& aObserver, 
    MCMRMediaRecorderObserver& aObserver2 )
    : iObserver( aObserver ), iObserver2( aObserver2 )
    {
    }
        
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::~CMccVideoSourceWrapper 
// -----------------------------------------------------------------------------
//
CMccVideoSourceWrapper::~CMccVideoSourceWrapper()
    {
    if ( iMediaRecorder && 
         !iFatalErrorOccured &&
         iMediaRecorder->State() == CCMRMediaRecorder::EStateRecording )
        {
        TRAP_IGNORE( StopL() )
        }
	delete iMediaRecorder;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::OpenL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::OpenL(
    MDataSource* aAudioSource, 
    TInt aCameraHandle,
    const TDesC8& aVideoMimeType,
    TFourCC aAudioType,
    TUid aEncoderUid,
    TBool aForceEncapsulationType )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::OpenL" )	
    
    __ASSERT_ALWAYS( !iMediaRecorder, User::Leave( KErrAlreadyExists ) );
    iMediaRecorder = CCMRMediaRecorder::NewL();	
    
    iFatalErrorOccured = EFalse;

	iMediaRecorder->OpenL( this, aAudioSource, this, aCameraHandle, 
		aVideoMimeType, aAudioType ) ;
	iMediaRecorder->SetAudioEnabledL( EFalse );
	
	if ( iMediaRecorder->State() == CCMRMediaRecorder::EStateOpen )
		{
		if ( aEncoderUid != KNullUid )
		    {
    		__V_SOURCESINK_CONTROLL_INT1( "SetPreferredVideoEncoderL, uid:", 
    		                              aEncoderUid.iUid )
            iMediaRecorder->SetPreferredVideoEncoderL( aEncoderUid );
		    }
		if ( aForceEncapsulationType )
            {
            __V_SOURCESINK_CONTROLL( "SetPreferredVideoEncapsulationL" )
            iMediaRecorder->SetPreferredVideoEncapsulationL( EDuElementaryStream );
            }
        
        __V_SOURCESINK_CONTROLL( "SetVideoCodingOptionsL" )        
		//set mediarecorder to insert GOB:s
		TCCMRVideoCodingOptions CodingOptions; 
	    CodingOptions.iSyncIntervalInPicture = 1;  
		CodingOptions.iMinRandomAccessPeriodInSeconds = KIPULRandomAccessPeriod;
		iMediaRecorder->SetVideoCodingOptionsL( CodingOptions );
		
		__V_SOURCESINK_CONTROLL( "PrepareL" )   
		iMediaRecorder->PrepareL();		
    	__V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper, \
preparing recorder success" )
		}
	else
		{
    	__V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper, \
opening recorder success, waiting" )
		}	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::OpenL, exit" )		
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::PlayL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::PlayL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SourcePlayL" )
    
    VIDEO_SOURCE_ASSERT_READINESS
	iMediaRecorder->RecordL();
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SourcePlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::ResumeL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::ResumeL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::ResumeL" )
    
    VIDEO_SOURCE_ASSERT_READINESS
	iMediaRecorder->ResumeL();
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::ResumeL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::PauseL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::PauseL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SourcePauseL")
    
    VIDEO_SOURCE_ASSERT_READINESS
	iMediaRecorder->PauseL();
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SourcePauseL, exit")
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::StopL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::StopL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SourceStopL" )
    
    if ( iMediaRecorder && !iFatalErrorOccured )
        {
	    iMediaRecorder->StopL();
        }
	
	// Delete the recorder in order to free camera resources
	delete iMediaRecorder;
	iMediaRecorder = 0;
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SourceStopL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::GetVideoBitrate()
// -----------------------------------------------------------------------------
//
TUint CMccVideoSourceWrapper::GetVideoBitrateL()
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoBitrateL" )
    
    VIDEO_SOURCE_ASSERT_READINESS
	return iMediaRecorder->VideoBitRateL();
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetVideoBitrateL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::SetVideoBitrateL( TUint aVideoBitRate )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceWrapper::SetVideoBitrateL", aVideoBitRate )
    
    VIDEO_SOURCE_ASSERT_READINESS
	iMediaRecorder->SetVideoBitRateL( aVideoBitRate );
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoBitrateL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetVideoFrameRateL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::SetVideoFrameRateL( TReal32 aFramesPerSecond )
    {
    __V_SOURCESINK_CONTROLL_REAL( "CMccVideoSourceWrapper::SetVideoFrameRateL", 
                                  aFramesPerSecond )
    
    VIDEO_SOURCE_ASSERT_READINESS
	
	if ( aFramesPerSecond > 0 )
	    {
	    iMediaRecorder->SetVideoFrameRateL( aFramesPerSecond );
	    }
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoFrameRateL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetVideoRateControlOptionsL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::SetVideoRateControlOptionsL( const TRateControlOptions& aOptions )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoRateControlOptionsL")
    
    VIDEO_SOURCE_ASSERT_READINESS
	iMediaRecorder->SetVideoRateControlOptionsL( aOptions );
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoRateControlOptionsL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::GetVideoRateControlOptionsL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::GetVideoRateControlOptionsL( TRateControlOptions& aOptions )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::GetVideoRateControlOptionsL")
    
    VIDEO_SOURCE_ASSERT_READINESS
	iMediaRecorder->GetVideoRateControlOptionsL( aOptions );
	
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::GetVideoRateControlOptionsL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetVideoBitrateL()
// -----------------------------------------------------------------------------
//
CCMRMediaRecorder::TRecorderState CMccVideoSourceWrapper::State()
    {
    if ( iMediaRecorder && !iFatalErrorOccured )
        {        
        return iMediaRecorder->State();
        }
    else
        {
        return CCMRMediaRecorder::EStateNone;
        }
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::WriteBufferL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::WriteBufferL( CCMRMediaBuffer* aBuffer )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::WriteBufferL" )
    iObserver.WriteBufferL( aBuffer );
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::WriteBufferL, exit" )
	}
	
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetVideoFrameSize()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceWrapper::SetVideoFrameSize( TSize aSize )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoFrameSize" )
    return iObserver.SetVideoFrameSize( aSize );
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetAverageVideoBitRate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceWrapper::SetAverageVideoBitRate( TInt aBitRate )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetAverageVideoBitRate" )				
    return iObserver.SetAverageVideoBitRate( aBitRate );
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetMaxVideoBitRate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceWrapper::SetMaxVideoBitRate( TInt aBitRate )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetMaxVideoBitRate" )				
    return iObserver.SetMaxVideoBitRate( aBitRate );
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetAverageAudioBitRate()
// -----------------------------------------------------------------------------
//
TInt CMccVideoSourceWrapper::SetAverageAudioBitRate( TInt aBitRate )
	{
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetAverageAudioBitRate" )
    return iObserver.SetAverageAudioBitRate( aBitRate );
	}
		
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::MmroPrepareComplete()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::MmroPrepareComplete( TInt aError )    
	{
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceWrapper::MmroPrepareComplete=",
                                  aError )
    iObserver2.MmroPrepareComplete( aError );

	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::MmroStateChange()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::MmroStateChange( TInt aState, TInt aError )
	{
    __V_SOURCESINK_CONTROLL_INT2( "CMccVideoSourceWrapper::MmroStateChange, \
aState=", aState, "aError=", aError )

    if ( iMediaRecorder )
        {
        __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceWrapper, media recorder state:",
                                      iMediaRecorder->State() ) 
        }
        
    iObserver2.MmroStateChange( aState, aError );
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::MmroTemporaryError()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::MmroTemporaryError( TInt aError )    
	{
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceWrapper::MmroTemporaryError=", 
                                  aError )
    iObserver2.MmroTemporaryError( aError );
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::MmroFatalError()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::MmroFatalError( TInt aError )
	{	
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceWrapper::MmroFatalError=", aError )

    // Remember fatal error
    iFatalErrorOccured = ETrue;

    iObserver2.MmroFatalError( aError );
	}



