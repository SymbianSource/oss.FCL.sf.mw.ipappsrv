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
#include "MccFileSink.h"
#include "MmccInterfaceDef.h"
#include "mmcccodecinformation.h"
#include "mccfilesinklogs.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"

// CONSTANTS

const TInt KMccMaxNumTimestamps = 5;

const TInt KMccTimestampDifferenceMultiplier = 10;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccFileSink::NewSinkL
// -----------------------------------------------------------------------------
//
MDataSink* CMccFileSink::NewSinkL( TUid /*aImplementationUid*/, 
                                   const TDesC8& /*aInitData*/ )
    {
    CMccFileSink* self = new ( ELeave ) CMccFileSink();
    return static_cast<MDataSink*>( self );
    }

// -----------------------------------------------------------------------------
// CMccFileSink::ConstructSinkL
// -----------------------------------------------------------------------------
//
void CMccFileSink::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccFileSink::CMccFileSink
// -----------------------------------------------------------------------------
//
CMccFileSink::CMccFileSink() : 
    CMccDataSink( KMccFileSinkUid )
    {
    iVideoCodec.iFourCC = TFourCC( KMccFourCCIdH263 );
    iAudioFourCC = TFourCC( KMccFourCCIdAMRNB );
    }
        
// -----------------------------------------------------------------------------
// CMccFileSink::~CMccFileSink
// -----------------------------------------------------------------------------
//
CMccFileSink::~CMccFileSink()
    {
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetCurrentUser
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetCurrentUser( MAsyncEventHandler* /*aEventHandler*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccFileSink::SinkPrimeL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkPrimeL()
	{		
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SinkPlayL()
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkPlayL()
    {
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SinkPauseL()
// 
// Pauses streaming by cancelling timers
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkPauseL()
    {
	}

// -----------------------------------------------------------------------------
// CMccFileSink::SinkStopL()
// 
// Stops streaming
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkStopL()
    {		
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SinkDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccFileSink::SinkDataTypeCode( TMediaId aMediaId )
	{
    if ( KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iVideoCodec.iFourCC;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType  )
        {
        return iAudioFourCC;
        }
    else
        {
        return TFourCC( KMMFFourCCCodeNULL );
        }
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SetSinkDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccFileSink::SetSinkDataTypeCode( TFourCC aCodec, 
                            TMediaId aMediaId )
	{
    TInt retVal = KErrNone;
    if ( KUidMediaTypeVideo == aMediaId.iMediaType &&
    	aCodec == iVideoCodec.iFourCC )
        {
        retVal = KErrNone;
        }
    else if ( KUidMediaTypeAudio == aMediaId.iMediaType &&
    	aCodec == iAudioFourCC )
        {
        retVal = KErrNone;
        }
    else
        {
        retVal = KErrNotSupported;
        }

	return retVal;
	}	

// -----------------------------------------------------------------------------
// CMccFileSink::BufferEmptiedL
// -----------------------------------------------------------------------------
//
void CMccFileSink::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::CanCreateSinkBuffer
// -----------------------------------------------------------------------------
//
TBool CMccFileSink::CanCreateSinkBuffer()
	{
	return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccFileSink::CreateSinkBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccFileSink::CreateSinkBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	return NULL;
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SinkThreadLogon
// -----------------------------------------------------------------------------
//
TInt CMccFileSink::SinkThreadLogon( MAsyncEventHandler& /*aEventHandler*/ )
	{
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SinkThreadLogoff
// -----------------------------------------------------------------------------
//
void CMccFileSink::SinkThreadLogoff()
	{	
	}

// -----------------------------------------------------------------------------
// CMccFileSink::EmptyBufferL
// -----------------------------------------------------------------------------
//
void CMccFileSink::EmptyBufferL( CMMFBuffer* /*aBuffer*/,
                  MDataSource* /*aProvider*/,
                  TMediaId /*aMediaId*/ )
	{
	}	

// -----------------------------------------------------------------------------
// CMccFileSink::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccFileSink::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
	{
	}	                  

// -----------------------------------------------------------------------------
// CMccFileSink::MfcoDiskFullL
// -----------------------------------------------------------------------------
//
void CMccFileSink::MfcoDiskFullL()
	{
	}	                  

// -----------------------------------------------------------------------------
// CMccFileSink::MfcoSizeLimitReachedL
// -----------------------------------------------------------------------------
//
void CMccFileSink::MfcoSizeLimitReachedL()
	{
	}	                  

// -----------------------------------------------------------------------------
// CMccFileSink::SetVideoCodecL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetVideoCodecL( const TMccCodecInfo& aVideoCodec )
    {
    iVideoCodec.iFourCC = aVideoCodec.iFourCC;
    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetAudioCodecL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetAudioCodecL( const TMccCodecInfo& aAudioCodec )
    {
    iAudioFourCC = aAudioCodec.iFourCC;
    }
    
// -----------------------------------------------------------------------------
// CMccFileSink::RecordTimeAvailableL
// -----------------------------------------------------------------------------
//
void CMccFileSink::RecordTimeAvailableL( TTimeIntervalMicroSeconds& /*aTime*/ )
	{ 
	}

// -----------------------------------------------------------------------------
// CMccFileSink::SetFileNameL
// -----------------------------------------------------------------------------
//
void CMccFileSink::SetFileNameL( const TFileName /*aFileName*/ )
	{
	}
	
// -----------------------------------------------------------------------------
// CMccFileSink::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccFileSink::SendStreamEventToClient( 
    TMccEventType /*aEventType*/, 
    TInt /*aError*/,
    TBool /*aToAllClients*/ )
	{
	}
 
// -----------------------------------------------------------------------------
// CMccFileSink::TimeToPlayL
// -----------------------------------------------------------------------------
//	   	
TTimeIntervalMicroSeconds CMccFileSink::TimeToPlayL( 
    TTimeIntervalMicroSeconds aCurrentTimestamp )
    {
    return aCurrentTimestamp;
    }

// -----------------------------------------------------------------------------
// CMccFileSink::ResetTimers
// -----------------------------------------------------------------------------
//	  
void CMccFileSink::ResetTimers()
    {

    }

// -----------------------------------------------------------------------------
// CMccFileSink::SetPausedDuration
// -----------------------------------------------------------------------------
//	
void CMccFileSink::SetPausedDuration( TTime /*aT1*/, TTime /*aT2*/ )
    {
    }


#ifndef EKA2
EXPORT_C TInt E32Dll( TDllReason )
    {
    return KErrNone;
    }
#endif
