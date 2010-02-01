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
#include <e32math.h>
#include "mccfilesourcetypebase.h"
#include "mccfilesourcelogs.h"
#include "mccfilesourceimpl.h"

// CONSTANTS
const TInt KRandMax = 0x7fff;
const TUint32 KMccTimingCorrectionIntervalMicrosecs = 800000;
const TUint32 KMccTimingCorrectionToleranceMillisecs = 400;


// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::~CMccFileSourceTypeBase
// -----------------------------------------------------------------------------
//
CMccFileSourceTypeBase::~CMccFileSourceTypeBase()
	{
    delete iPeriodicRunner;
    iFrames.ResetAndDestroy();
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::SetFileHandle
// -----------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::SetFileHandle( MP4Handle aMP4Handle )
	{
	iMP4Handle = aMP4Handle;
	}

// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::FillBufferL( 
    CMMFBuffer* aBuffer, 
    MDataSink* aConsumer )
	{
	iConsumer = aConsumer;
	iConsumerBuffer = aBuffer;
	}
	

// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::StopTimer
// -----------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::StopTimer()
	{
	iPeriodicRunner->Stop();
	SetPaused( ETrue );
	}

// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::Cleanup
// -----------------------------------------------------------------------------
//	
void CMccFileSourceTypeBase::Cleanup()
    {
    iConsumer = NULL;
    iConsumerBuffer = NULL;
    }
    
// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::SetPositionL
// -----------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::SetPositionL( TUint32 aPosition, TBool aResuming )
	{
	TUint32 oldPosition = iPosition;
	
	__FILESOURCE_CONTROLL_INT2( "CMccFileSourceTypeBase::SetPositionL, old pos:", 
	                            oldPosition," proposed pos:", aPosition )
	
	// If position is tried to be set over the end, end position is used.
	aPosition = Min( aPosition, Duration() );
	__FILESOURCE_CONTROLL_INT1( "CMccFileSourceTypeBase::SetPositionL, sanitized pos:", 
                                aPosition )
                                
    if ( iMP4Handle == NULL || IsPaused() )
        {
        iPosition = aPosition;
        iPositionChangedWhilePaused = 
            iPositionChangedWhilePaused ? iPositionChangedWhilePaused :
                                          ( oldPosition != iPosition );
        }
    else if ( !aResuming || ( aResuming && iPositionChangedWhilePaused ) )
        {
        DoSetPositionL( aPosition );
        iPositionChangedWhilePaused = EFalse;
        }
    else
        {
        // NOP
        }
        
    __FILESOURCE_CONTROLL_INT2( "CMccFileSourceTypeBase::SetPositionL, new pos:", 
                                iPosition, " resuming:", aResuming )
    
    // Read interval value is used for preventing two subsequent frames
    // having the same timestamp. 
    if ( oldPosition > iPosition )
        {
        // If rewinded, have to add time to timestamps
        iPositionModifier += ( oldPosition - iPosition + iReadInterval );
        }
    else if ( oldPosition < iPosition )
        {
        // If forwarded, have to subtract time from timestamps
        TInt64 difference = ( iPosition - oldPosition );
        TInt64 difference2 = ( difference - iReadInterval );
        iPositionModifier -= ( ( difference2 > 0 ) ? difference2 : difference );
        }
    else
        {
        // NOP
        }
        
    __FILESOURCE_CONTROLL_INT1( "CMccFileSourceTypeBase::SetPositionL, pos modifier:", 
                                iPositionModifier )
	}
	
// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::ResetTimeL
// -----------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::ResetTimeL()
	{
	SetPositionL(0);
	
	iPositionModifier = 0;
	}

// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::ParseUpdateStreamDescription
// -----------------------------------------------------------------------------
//	
void CMccFileSourceTypeBase::ParseUpdateStreamDescription( MP4Handle aMP4Handle )
	{
	MP4Err error;
	error = MP4ParseRequestStreamDescription( aMP4Handle,
        ( mp4_u32* ) &iStreamSize, ( mp4_u32* ) &iStreamAverageBitRate );
       
    if (error != MP4_OK)
	    {
	    __FILESOURCE_CONTROLL_INT1("CMccFileSourceTypeBase, \
MP4ParseRequestStreamDescription error=",error)	
	    }
	}

// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::StreamAverageBitRate
// -----------------------------------------------------------------------------
//	
TUint32 CMccFileSourceTypeBase::StreamAverageBitRate()
	{
	return iStreamAverageBitRate;
	}

// -----------------------------------------------------------------------------
// CMccFileSourceTypeBase::SetStartTime
// -----------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::SetStartTime( )
	{
	__FILESOURCE_CONTROLL( "CMccFileSourceTypeBase::SetStartTime" )
	
	iStartTime.HomeTime(); // Set start time to current time
	iPreviousCorrection = iStartTime;	
	}

// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::IsPaused
// ---------------------------------------------------------------------------
//	
TBool CMccFileSourceTypeBase::IsPaused() const
    {
    return iIsPaused;
    }

// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::MediaType
// ---------------------------------------------------------------------------
//
TMediaId CMccFileSourceTypeBase::MediaType() const
    {
    return iMediaType;
    }
    
// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::Random
// ---------------------------------------------------------------------------
//
TUint16 CMccFileSourceTypeBase::Random() const
    {
    TTime tmp_time;
    tmp_time.HomeTime();
    TInt64 seed = tmp_time.Int64();
    TInt32 rand = Math::Rand( seed );
    return static_cast<TUint16>( Abs( rand ) % KRandMax );
    }

// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::SetPaused
// ---------------------------------------------------------------------------
//   
void CMccFileSourceTypeBase::SetPaused( TBool aIsPaused )
    {
    __FILESOURCE_CONTROLL_INT2( 
   	        "CMccFileSourceTypeBase::SetPaused, iIsPaused:", 
   	        iIsPaused, " aIsPaused:", aIsPaused )
   	        
    if ( aIsPaused && !iIsPaused )
        {
        iPauseStarted.HomeTime();
        }
    else if ( iIsPaused && !aIsPaused )
        {
        iFrames.ResetAndDestroy();
        
        iPreviousCorrection.HomeTime(); 
        
        iPauseStopped.HomeTime();
        TTimeIntervalMicroSeconds differenceTemp = 
    	    iPauseStopped.MicroSecondsFrom( iPauseStarted );
    	    
    	iPositionModifier += ( differenceTemp.Int64() / KMccMicroToMilliConst );
        }
    iIsPaused = aIsPaused;
    }
    
// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::CMccFileSourceTypeBase
// ---------------------------------------------------------------------------
//
CMccFileSourceTypeBase::CMccFileSourceTypeBase( 
    CMccFileSourceImpl& aSource,
    TMediaId aMediaType ) : 
    iSource( aSource ),
    iMediaType( aMediaType )
    {
    iSequenceNum = Random();
    }

// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::ConstructL
// ---------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::ConstructL()
	{
	iPeriodicRunner = CMccPeriodicRunner::NewL();
	}

// ---------------------------------------------------------------------------
// CMccFileSourceTypeBase::DoTimingCorrection
// ---------------------------------------------------------------------------
//
void CMccFileSourceTypeBase::DoTimingCorrection( TUint32 aCurrentPosition )
    {
    TTimeIntervalMicroSeconds elapsedTime;
    TTime currentTime;
    currentTime.HomeTime();
    
    elapsedTime = currentTime.MicroSecondsFrom( iPreviousCorrection );
    if ( elapsedTime >= 
         TTimeIntervalMicroSeconds( KMccTimingCorrectionIntervalMicrosecs ) )
        {
        elapsedTime = currentTime.MicroSecondsFrom( iStartTime );
    
        TUint32 elapsedTimeMs = elapsedTime.Int64() / KMccMicroToMilliConst;
   	    
   	    elapsedTimeMs -= iPositionModifier;
   	    
   	    __FILESOURCE_CONTROLL_INT2( 
   	        "CMccFileSourceTypeBase::DoTimingCorrection, position (msecs):", 
   	        aCurrentPosition, " played (msecs):", elapsedTimeMs )
   	      
   	    if ( ( elapsedTimeMs + 
   	           KMccTimingCorrectionToleranceMillisecs ) < aCurrentPosition )
   	        {
   	        // Have read too fast, slow down if possible 
   	        // (only one notch slowing is allowed)
   	        if ( iCorrectionInterval <= iTimerInterval )
   	            {
   	            __FILESOURCE_CONTROLL( 
   	                "CMccFileSourceTypeBase::DoTimingCorrection, slow down" )
   	            
   	            const TReal KMccCorrectionModifier = 1.3;
   	            iCorrectionInterval = (TUint32) (iTimerInterval * KMccCorrectionModifier);
   	            }
   	        }
   	    else if ( ( elapsedTimeMs - 
   	                KMccTimingCorrectionToleranceMillisecs ) > aCurrentPosition )
   	        {
   	        // Have read too slow, speed up if possible
   	        // (only one notch speeding is allowed)
   	        if ( iCorrectionInterval >= iTimerInterval )
   	            {
   	            __FILESOURCE_CONTROLL( 
   	                "CMccFileSourceTypeBase::DoTimingCorrection, speed up" )
   	            
   	            const TReal KMccCorrectionModifier = 0.7;
   	            iCorrectionInterval = (TUint32) (iTimerInterval * KMccCorrectionModifier);
   	            }
   	        }
   	    else
   	        {
   	        // No changes needed / normalized due interval changing       
   	        iCorrectionInterval = iTimerInterval;
   	        }
   	    
   	    if ( iPeriodicRunner->Interval() != iCorrectionInterval )
   	        {
   	        __FILESOURCE_CONTROLL_INT1( 
   	            "CMccFileSourceTypeBase::DoTimingCorrection, start timer:", 
                 iCorrectionInterval )
                                
   	        iPeriodicRunner->Start( iCorrectionInterval, TickCallBack() );
   	        }
   	        
   	    iPreviousCorrection = currentTime;
        }
    }
    
// End of file

