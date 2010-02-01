/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    rtp media clock
*
*/




#include <e32std.h>
#include <e32math.h>
#include "mccrtpmediaclock.h"
#include "mccsubcontrollerlogs.h"

// LOCAL CONSTANTS

// Helper for calculating next timestamp with sampling rate.
const TUint KKiloHzDivider( 1000 );

// Idle time limit when no timestamps have been asked from media clock. If
// idle time is larger than this, then next timestamp is calculated from
// actual time between two timestamp requests.
const TInt KIdleTimeStampLimitSecs = 1;

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// default C++ constructor.
// ---------------------------------------------------------------------------
//
CMccRtpMediaClock::CMccRtpMediaClock() : iSamplingFreq( K8Khz ),
    iHwFrametime( K20ms )
    {
    
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CMccRtpMediaClock::ConstructL()
    {
    // Generate random initial rtp timestamp which will be mapped to initial
    // time. Then take note of the construction time of mediaclock.
    iPreviousTimestamp.HomeTime();
    TInt64 seed = iPreviousTimestamp.Int64();
    iCurRtpTS = Abs( Math::Rand( seed ) );
    iPreviousTimestamp.UniversalTime();
    }

// ---------------------------------------------------------------------------
// Static constructor.
// ---------------------------------------------------------------------------
//
CMccRtpMediaClock* CMccRtpMediaClock::NewL()
    {
    CMccRtpMediaClock* self = new( ELeave ) CMccRtpMediaClock();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMccRtpMediaClock::~CMccRtpMediaClock()
    {
    iRegister.Close();
    }

// ---------------------------------------------------------------------------
// Each media clock client must register themselves once. If sampling rate or
// frametime of the client is changed, client must register again.
// ---------------------------------------------------------------------------
//
TUint32 CMccRtpMediaClock::RegisterMediaFormat( TUint aSamplingFreq, 
                                                TInt aHwFrametime )
    {
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::RegisterMediaFormat aSamplingFreq: ", aSamplingFreq )
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::RegisterMediaFormat aHwFrametime: ", aHwFrametime )
    
    // create unique key for client
    TUint32 key = Math::Random();
    
    // create record of registered clients
    TMccRegInfo reg_info;
    reg_info.iKey = key;
    reg_info.iSamplingFreq = aSamplingFreq;
    reg_info.iHwFrametime = aHwFrametime;
    iRegister.Append( reg_info );
    
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::RegisterMediaFormat key created: ", key )
       
    return key;
    }

// ---------------------------------------------------------------------------
// Get new timestamp for registered media format
// ---------------------------------------------------------------------------
//
TInt CMccRtpMediaClock::GetTimeStamp( TUint32 aKey, TUint32& aTimeStamp )
    {
    __SUBCONTROLLER( "CMccRtpMediaClock::GetTimeStamp" );
    
    TInt cell( KErrNotFound );
    
    if ( aKey != iUsedKey )
        {
        __SUBCONTROLLER_INT1( "---CMccRtpMediaClock::GetTimeStamp key changed: ", aKey )
        
        TInt err = CheckKey( aKey, cell );
        if ( err )
            {
            // if not registered, -1 is returned
            return err;
            }
        }
    
    // increment is done with previous sampling rate and hw time
    aTimeStamp = TimeStampIncrement();
    
    if ( KErrNotFound != cell )
        {
        // update current parameters
        iUsedKey = aKey;
        iSamplingFreq = iRegister[cell].iSamplingFreq;
        iHwFrametime = iRegister[cell].iHwFrametime;
        
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::GetTimeStamp cell: ", cell )
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::GetTimeStamp iRegister.iKey: ", iRegister[cell].iKey )
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::GetTimeStamp iRegister.iSamplingFreq: ", iRegister[cell].iSamplingFreq )
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::GetTimeStamp iRegister.iHwFrametime: ", iRegister[cell].iHwFrametime )
        }
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Returns the latest used rtp timestamp.
// For 'initial keep alive' and 'during call keep alive'.
// ---------------------------------------------------------------------------
//
void CMccRtpMediaClock::GetLatestConsumedTS( TUint32& aTimeStamp )
    {
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::GetLatestConsumedTS: ", iCurRtpTS );
    
    aTimeStamp = iCurRtpTS;
    }

// ---------------------------------------------------------------------------
// Private method
// Designed to take care of rtp time stamp calculation
// ---------------------------------------------------------------------------
//
TUint32 CMccRtpMediaClock::TimeStampIncrement()
    {
    __SUBCONTROLLER ("CMccRtpMediaClock::TimeStampIncrement" )  
    
    TTime current;
    current.UniversalTime();
    
    TTimeIntervalSeconds seconds;
    current.SecondsFrom( iPreviousTimestamp, seconds );
    
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::TimeStampIncrement secs: ", seconds.Int() )
    
    // Note: We cannot rely that this function is called with monotonic rate.
    // Due to the fact that audio adaptation may not work with monotonic rate,
    // we must not use TTime and other variants to calculate RTP timestamps,
    // but increment the timestamp each time this function is called.
    // Thus we rely on the fact that caller knows best if the RTP mediaclock
    // timestamp should be incremented.
    // Note that if there is at least two second delay between the calls to
    // this function, then we take into account the actual time between the
    // calls. Currently we use two second limit between the calls.
    const TTimeIntervalSeconds diff( KIdleTimeStampLimitSecs );
    if ( diff < seconds )
        {
        iTimeBasedIncrement = ETrue;
        
        iCurRtpTS += TUint( seconds.Int() * iSamplingFreq );
        
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::TimeStampIncrement timebased incr: ",
            seconds.Int() * iSamplingFreq )
        }
    else
        {
        iTimeBasedIncrement = EFalse;
        
        const TUint khzPart = iSamplingFreq / KKiloHzDivider;
        iCurRtpTS += TUint( iHwFrametime * khzPart );
        
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::TimeStampIncrement callbased incr: ",
            iHwFrametime * khzPart )
        }
    
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::TimeStampIncrement iCurRtpTS: ",
        iCurRtpTS )
    
    // Get the current time to make the calculation when called next.
    iPreviousTimestamp.UniversalTime();
    
    return iCurRtpTS;
    }

// ---------------------------------------------------------------------------
// Private method
// Check used key if it is registered
// ---------------------------------------------------------------------------
//
TInt CMccRtpMediaClock::CheckKey( TUint32 aKey, TInt& aCell )
    {
    __SUBCONTROLLER( "CMccRtpMediaClock::CheckKey" )
    
    const TInt count( iRegister.Count() );
    
    for( TInt i = 0; i < count; i++ )
        {
        if( iRegister[i].iKey == aKey )
            {
            __SUBCONTROLLER_INT1( "CMccRtpMediaClock::CheckKey return key cell: ", i )
            
            aCell = i;
            return KErrNone;
            }
        }
    
    __SUBCONTROLLER( "CMccRtpMediaClock::CheckKey return error KErrNotFound" )
    
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// Removes a media format registration if found.
// ---------------------------------------------------------------------------
//
void CMccRtpMediaClock::UnregisterMediaFormat( TUint32 aKey )
    {
    __SUBCONTROLLER_INT1( "CMccRtpMediaClock::UnregisterMediaFormat aKey: ", aKey )
            
    TInt cell = KErrNotFound;
    TInt err = CheckKey( aKey, cell );
    if ( KErrNone == err )
        {
        __SUBCONTROLLER_INT1( "CMccRtpMediaClock::UnregisterMediaFormat remove cell: ",
            cell )
        
        iRegister.Remove( cell );
        }
    }

// ---------------------------------------------------------------------------
// Checks whether 'timebased' or 'callback' increment has been used
// ---------------------------------------------------------------------------
//
TBool CMccRtpMediaClock::TimeBasedIncrement() const
   	{
   	__SUBCONTROLLER_INT1( "CMccRtpMediaClock::TimeBasedIncrement: ", 
   	    iTimeBasedIncrement )
   	return iTimeBasedIncrement;
   	}


// End of file
