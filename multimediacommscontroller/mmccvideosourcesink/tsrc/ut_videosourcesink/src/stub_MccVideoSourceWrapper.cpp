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
#include "MccVideoSourceWrapper.h"
#include "MccVideoSourceSinkLogs.h"

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
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::OpenL
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::OpenL(
   MDataSource* aAudioSource, 
   TInt aCameraHandle,
   const TDesC8& aVideoMimeType,
   TFourCC aAudioType )
	{
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::PlayL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::PlayL()
    {
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::PauseL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::PauseL()
    {
	}

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::ResumeL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::ResumeL()
    {
    }
    
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::StopL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::StopL()
    {
    }

TUint CMccVideoSourceWrapper::GetVideoBitrateL()
    {
	return 128000;
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::SetVideoBitrateL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::SetVideoBitrateL( TUint /*aVideoBitRate*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::State()
// -----------------------------------------------------------------------------
//
CCMRMediaRecorder::TRecorderState CMccVideoSourceWrapper::State()
    {
    
#ifdef MediaRecorderStateNone
    return CCMRMediaRecorder::EStateNone;
#endif

#ifdef MediaRecorderStateReadyToRecord
    return CCMRMediaRecorder::EStateReadyToRecord;
#endif

#ifdef MediaRecorderStatePause
    return CCMRMediaRecorder::EStateReadyToRecordPause;
#endif

    return CCMRMediaRecorder::EStateNone;
    
    }
// -----------------------------------------------------------------------------
// CMccVideoSourceWrapper::WriteBufferL()
// -----------------------------------------------------------------------------
//
void CMccVideoSourceWrapper::WriteBufferL( CCMRMediaBuffer* aBuffer )
	{
    iObserver.WriteBufferL( aBuffer );
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
// CMccVideoSourceWrapper::SetVideoFrameRateL()
// -----------------------------------------------------------------------------
//		
void CMccVideoSourceWrapper::SetVideoFrameRateL( TReal32 /*aFramesPerSecond*/ )
    {
    __V_SOURCESINK_CONTROLL( "CMccVideoSourceWrapper::SetVideoFrameRateL" )
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
    __V_SOURCESINK_CONTROLL_INT1( "CMccVideoSourceWrapper, media recorder state:",
                                  iMediaRecorder->State() )    
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
    iObserver2.MmroFatalError( aError );
	}



