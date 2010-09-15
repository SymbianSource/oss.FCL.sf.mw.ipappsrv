/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "UT_CMccFileSink.h"
#include "MmccInterfaceDef.h"
#include "MccInternalCodecs.h"
#include "mccunittestmacros.h"
#include "mcctesteventhandler.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>

//  INTERNAL INCLUDES
#include "MccFileSink.h"
#include "Mccdldatapath.h"
#include "mccresourcepool.h"
#include "CamC3GPDataSinkImpl_stub.h"
#include "mccresourcepool_STUB.h"

// Prime may leave also with KErrGeneral in out of memory situation
#define DO_SINK_PRIME_FOR_USER( user ) { \
User::LeaveIfError( iFileSink->SinkThreadLogon( *user ) ); \
iFileSink->SetCurrentUser( user ); \
TMccCodecInfo codecInfo; \
codecInfo.iSdpName = KAVCSdpName; \
codecInfo.iFourCC.Set(KMccFourCCIdAVC); \
iFileSink->SetVideoCodecL( codecInfo ); \
TRAPD( primeErr, iFileSink->SinkPrimeL() ); \
if ( primeErr == KErrGeneral || primeErr == KErrNoMemory ) \
    {\
    User::Leave( KErrNoMemory );\
    }\
else\
    {\
    EUNIT_ASSERT_EQUALS( primeErr, KErrNone );\
    } \
}

#define DO_SINK_PRIME \
DO_SINK_PRIME_FOR_USER( iEventHandler )

#define ASSERT_EVENT_FOR_USER( user, eventType ) \
EUNIT_ASSERT( user->iLastEvent.iEventType == eventType ); \
user->iLastEvent = TMccEvent(); \

#define ASSERT_EVENT( eventType ) ASSERT_EVENT_FOR_USER( iEventHandler, eventType )


#define ASSERT_LAST_BUFFER_TYPE( type ) \
EUNIT_ASSERT( iFileSink->iFileComposer && \
iFileSink->iFileComposer->SetAverageVideoBitRate( KMccTestGetLastBufferType ) == type );


// CONSTRUCTION
UT_CMccFileSink* UT_CMccFileSink::NewL()
    {
    UT_CMccFileSink* self = UT_CMccFileSink::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccFileSink* UT_CMccFileSink::NewLC()
    {
    UT_CMccFileSink* self = new( ELeave ) UT_CMccFileSink();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccFileSink::~UT_CMccFileSink()
    {
    }

// Default constructor
UT_CMccFileSink::UT_CMccFileSink()
    {
    }

// Second phase construct
void UT_CMccFileSink::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccFileSink::SetupL(  )
    {
    iEventHandler = CMccTestEventHandler::NewL();
    iEventHandler2 = NULL;
    iBuffer = CMMFDescriptorBuffer::NewL( 10 );
    iBuffer->Data().Copy(_L("aaaaaaaaaa"));
    TMediaId media = KUidMediaTypeVideo;
    MAsyncEventHandler* eventHandler = NULL;
    CMccResourcePool* iMccResource = NULL;
    iDLDataPathStub = 
    	CMccDlDataPath::NewL( eventHandler, iMccResource, media );
    
    TFileName fileName;
    fileName = _L("c:\\fs.3gp");
    TPckgBuf<TFileName> initParam( fileName );
    
    iFileSink = static_cast<CMccFileSink*>( CMccFileSink::NewSinkL( KMccFileSinkUid, initParam ) );
    iFileSink->ConstructSinkL( initParam );
    
    iResourceStub = CMccResourcePoolStub::NewL();
    }

void UT_CMccFileSink::Teardown(  )
    {
    delete iFileSink;
    delete iDLDataPathStub;
    delete iBuffer;
    delete iEventHandler;
    iEventHandler = NULL;
    delete iEventHandler2;
    iEventHandler2 = NULL;
    delete iResourceStub;
    iResourceStub = NULL;
    }

void UT_CMccFileSink::UT_CMccFileSink_NewSinkLL(  )
    {
    //NOP
    }

void UT_CMccFileSink::UT_CMccFileSink_ConstructSinkLL(  )
    {
    //NOP
    }

void UT_CMccFileSink::UT_CMccFileSink_CMccFileSinkL(  )
    {
    //NOP
    }

void UT_CMccFileSink::UT_CMccFileSink_SetFileNameLL(  )
    {
#define KFileSinkName TFileName(_L("c:\\fs3.3gp"));  
    TFileName fileName = KFileSinkName;   
    iFileSink->SetFileNameL( fileName );
    DO_SINK_PRIME
    iFileSink->SinkPlayL();
    
    TMediaId media = KUidMediaTypeAudio;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    media = KUidMediaTypeVideo;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );

    iFileSink->SinkStopL();
    }

void UT_CMccFileSink::UT_CMccFileSink_RecordTimeAvailableLL(  )
    {
    DO_SINK_PRIME
    iFileSink->SinkPlayL();
    TTimeIntervalMicroSeconds remainingTime;
    iFileSink->RecordTimeAvailableL( remainingTime );
    
    iFileSink->SinkStopL();
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkDataTypeCodeL(  )
    {
	TMediaId mediaId;
    EUNIT_ASSERT(iFileSink->SinkDataTypeCode( mediaId ) == TFourCC(KMMFFourCCCodeNULL) );
                        
    mediaId.iMediaType = KUidMediaTypeVideo;
    EUNIT_ASSERT(iFileSink->SinkDataTypeCode( mediaId ) == TFourCC(KMccFourCCIdH263) );
     
    mediaId.iMediaType = KUidMediaTypeAudio;
    EUNIT_ASSERT(iFileSink->SinkDataTypeCode( mediaId ) == TFourCC(KMMFFourCCCodeNULL) );            
    }

void UT_CMccFileSink::UT_CMccFileSink_SetSinkDataTypeCodeL(  )
    {
    TMediaId mediaId( KUidMediaTypeVideo );
    TFourCC codec1( KMccFourCCIdH263 );
    EUNIT_ASSERT_EQUALS(
    	iFileSink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNone );
    mediaId.iMediaType = KUidMediaTypeAudio;
    codec1.Set(KMccFourCCIdAMRNB);

    EUNIT_ASSERT_EQUALS(
    	iFileSink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNotSupported );
    
    mediaId.iMediaType = KUidMediaTypeMidi;
    
    EUNIT_ASSERT_EQUALS(
    	iFileSink->SetSinkDataTypeCode(codec1, mediaId),
        KErrNotSupported );
    }

void UT_CMccFileSink::UT_CMccFileSink_BufferEmptiedLL(  )
    {
    CMMFBuffer* buffer = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE(iFileSink->BufferEmptiedL(buffer), KErrNotSupported);
    }

void UT_CMccFileSink::UT_CMccFileSink_CanCreateSinkBufferL(  )
    {
    EUNIT_ASSERT(!iFileSink->CanCreateSinkBuffer() );
    }

void UT_CMccFileSink::UT_CMccFileSink_CreateSinkBufferLL(  )
    {
	TMediaId mediaId;
    TBool bufRef = EFalse;
    mediaId.iMediaType = KUidMediaTypeMidi;
	EUNIT_ASSERT_SPECIFIC_LEAVE(
		iFileSink->CreateSinkBufferL(mediaId,bufRef), KErrNotSupported );
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkThreadLogonL(  )
    {
    MAsyncEventHandler* aHandler = NULL;
	iFileSink->SinkThreadLogon( *aHandler );
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkThreadLogoffL(  )
    {
	iFileSink->SinkThreadLogoff();
	delete iEventHandler;
	iEventHandler = NULL;
	
	// One user logoff
	iEventHandler = AddUserL();
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 1 );
	iFileSink->SetCurrentUser( iEventHandler );
	iFileSink->SinkThreadLogoff();
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 0 );
	
	// Multiple codecs, primary logoff
	delete iEventHandler;
	iEventHandler = NULL;
	iEventHandler = AddUserL();
	iEventHandler2 = AddUserL( ETrue );
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 2 );
	iFileSink->SetCurrentUser( iEventHandler );
	iFileSink->SinkThreadLogoff();
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 1 );
	delete iEventHandler;
	iEventHandler = NULL;
	
	// Multiple codecs, primary logoff while playing
	iEventHandler = AddUserL();
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 2 );
	DO_SINK_PRIME_FOR_USER( iEventHandler2 )
	iFileSink->SinkPlayL();
	iFileSink->SinkThreadLogoff();
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 1 );
	if ( iFileSink->iCurrentState != CMccFileSink::ERecording )
	    {
	    // Some operations inside TRAP_IGNORE of SinkThreadLogoff
	    // were not executed yet due memory leave
	    User::Leave( KErrNoMemory );
	    }
	EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::ERecording );
	delete iEventHandler2;
	iEventHandler2 = NULL;
	
	// Multiple codecs, primary logoff while paused, active user information
	// gets uninitialized so that next user can possibly start using the sink
	iEventHandler2 = AddUserL( ETrue );
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 2 );
	iFileSink->SetCurrentUser( iEventHandler );
    iFileSink->SinkPauseL();
   	iFileSink->SinkThreadLogoff();
	EUNIT_ASSERT_EQUALS( iFileSink->iUsers.Count(), 1 );
	EUNIT_ASSERT_EQUALS( iFileSink->iActiveUserIndex, KErrNotFound );
	if ( iFileSink->iCurrentState != CMccFileSink::EPaused )
	    {
	    // Some operations inside TRAP_IGNORE of SinkThreadLogoff
	    // were not executed yet due memory leave
	    User::Leave( KErrNoMemory );
	    }
	EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EPaused );
	iFileSink->SetCurrentUser( iEventHandler2 );
	iFileSink->SinkThreadLogoff();
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkPrimeLL(  )
    {
	DO_SINK_PRIME
	EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EReady );
	ASSERT_EVENT( KMccStreamPrepared )
	
	// Priming twice is allowed
	iFileSink->SinkPrimeL();
	EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EReady );
	ASSERT_EVENT( KMccStreamPrepared )
	iFileSink->SinkStopL();
	
	// Test situation where memory is full at beginning
	iFileSink->iFileComposer->SetSizeLimit( KMccFileSinkTestMemoryFullAtBeginning );
	iFileSink->SinkPrimeL();
	EUNIT_ASSERT( iFileSink->iNotifySizeLimitReached );
	
	iFileSink->SinkPlayL();
	
	TMediaId media( KUidMediaTypeVideo );
    iFileSink->EmptyBufferL( iBuffer, iDLDataPathStub, media );
    EUNIT_ASSERT( !iFileSink->iNotifySizeLimitReached );
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkPlayLL(  )
    {
    DO_SINK_PRIME
    iFileSink->SinkPlayL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::ERecording );
    ASSERT_EVENT( KMccStreamStarted )
    
    // Starting twice is allowed
    iFileSink->SinkPlayL(); 
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::ERecording );
    ASSERT_EVENT( KMccStreamStarted )
    
    // Resume
    iFileSink->SinkPauseL();
    iFileSink->SinkPlayL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::ERecording );
    ASSERT_EVENT( KMccStreamResumed )
    
    // Starting when stopped is not allowed
    iFileSink->SinkStopL();
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSink->SinkPlayL(), KErrNotReady );
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkPauseLL(  )
    {
    DO_SINK_PRIME
    iFileSink->SinkPlayL();
    iFileSink->SinkPauseL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EPaused );
    ASSERT_EVENT( KMccStreamPaused )
    
    // Pausing twice is allowed
    iFileSink->SinkPauseL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EPaused );
    ASSERT_EVENT( KMccStreamPaused )
    
    iFileSink->SinkPlayL();
    iFileSink->SinkPauseL();
    iFileSink->SinkStopL();
    
    // Pausing when stopped is not allowed
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iFileSink->SinkPauseL(), KErrNotReady );
    }

void UT_CMccFileSink::UT_CMccFileSink_SinkStopLL(  )
    { 
    iFileSink->SinkStopL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EStopped );
    
    // Stopping twice is allowed
    iFileSink->SinkStopL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EStopped );
    
    // Stopping does not have effect when multiple users exist
    DO_SINK_PRIME
    
   	iEventHandler2 = AddUserL();
   	iFileSink->SinkStopL();
   	EUNIT_ASSERT( iFileSink->iCurrentState != CMccFileSink::EStopped );
    ASSERT_EVENT_FOR_USER( iEventHandler2, KMccStreamStopped )
    iFileSink->SinkThreadLogoff();
    delete iEventHandler2;
    iEventHandler2 = NULL;
    iFileSink->SetCurrentUser( iEventHandler );
    iFileSink->SinkStopL();
    EUNIT_ASSERT( iFileSink->iCurrentState == CMccFileSink::EStopped );
    ASSERT_EVENT( KMccStreamStopped )
    }

void UT_CMccFileSink::UT_CMccFileSink_EmptyBufferLL(  )
    {
    TMediaId media;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    DO_SINK_PRIME
    iFileSink->SinkPlayL();
    EUNIT_ASSERT_SPECIFIC_LEAVE(iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media ), KErrNotSupported );

    media = KUidMediaTypeAudio;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    
    // First normal AVC bytestream, buffer ignored as dec spec info not yet given
    media = KUidMediaTypeVideo;
    iDLDataPathStub->SetSourceDataTypeCode( TFourCC( KMccFourCCIdAVC ), media );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( 0 );   

    // Then AVC dec spec info
    iBuffer->Data()[ 0 ] = 0x00;
    iBuffer->Data()[ 1 ] = 0x00;
    iBuffer->Data()[ 2 ] = 0x00;
    iBuffer->Data()[ 3 ] = 0x01;
    iBuffer->Data()[ 4 ] = 0x07;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo );
    
    // Normal bytestream is passed to file writer as dec spec info has been written
    iBuffer->Data()[ 4 ] = 0x11;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264Bytestream );  
    
    // Dec spec info is not written anymore as it has been already written once 
    iBuffer->Data()[ 4 ] = 0x07;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( 0 );
    
    // H263 in use
    //
    iFileSink->SinkStopL();
    iFileSink->SinkThreadLogoff();
    iEventHandler2 = AddUserL();
    iFileSink->SinkPrimeL();
    iFileSink->SinkPlayL();
    iDLDataPathStub->SetSourceDataTypeCode( TFourCC( KMccFourCCIdH263 ), media );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH263 );
    iFileSink->SinkThreadLogoff();
    delete iEventHandler2;
    iEventHandler2 = NULL;
    }

void UT_CMccFileSink::UT_CMccFileSink_EmptyBufferL2L(  )
    {
    // Test file writing when keyframe info is checked from resource pool
    //
    iFileSink->SetResources( iResourceStub );
    
    DO_SINK_PRIME
    iFileSink->SinkPlayL();
    TMediaId media;
    media = KUidMediaTypeVideo;
    
    // First normal AVC bytestream, buffer ignored as dec spec info not yet given
    iBuffer->SetTimeToPlay( 10000 );
    iDLDataPathStub->SetSourceDataTypeCode( TFourCC( KMccFourCCIdAVC ), media );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( 0 );   

    // Then AVC dec spec info
    iBuffer->Data()[ 0 ] = 0x00;
    iBuffer->Data()[ 1 ] = 0x00;
    iBuffer->Data()[ 2 ] = 0x00;
    iBuffer->Data()[ 3 ] = 0x01;
    iBuffer->Data()[ 4 ] = 0x07;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo );
    
    // Normal bytestream is not passed to file writer as key frame has not been writte
    iBuffer->Data()[ 4 ] = 0x11;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( 0 );  
    
    // Key frame is written to file
    iBuffer->SetTimeToPlay( 11000 );
    iResourceStub->StoreKeyFrameInfoL( 0, *iBuffer );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264Bytestream );  
    
    // Normal bytestream is passed to file writer as dec spec info and key frame has been written
    iBuffer->SetTimeToPlay( 14000 );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264Bytestream );  
    
    // Key frame is again needed after pause/resume
    iFileSink->SinkPauseL();
    iFileSink->SinkPlayL();
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( 0 );  
    iBuffer->SetTimeToPlay( 15000 );
    iResourceStub->StoreKeyFrameInfoL( 0, *iBuffer );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264Bytestream );  
    iBuffer->SetTimeToPlay( 16000 );
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( CCMRMediaBuffer::EVideoH264Bytestream );  
       
    // Dec spec info is not written anymore as it has been already written once 
    iBuffer->Data()[ 4 ] = 0x07;
    iFileSink->EmptyBufferL(iBuffer, iDLDataPathStub, media );
    ASSERT_LAST_BUFFER_TYPE( 0 );  // Still previous buf
    }

void UT_CMccFileSink::UT_CMccFileSink_BufferFilledLL(  )
    {
    CMMFBuffer* buffer = NULL;
    
    iFileSink->BufferFilledL(buffer);
    }

void UT_CMccFileSink::UT_CMccFileSink_MfcoDiskFullLL(  )
    {
    DO_SINK_PRIME
    iFileSink->MfcoDiskFullL();
    EUNIT_ASSERT( iFileSink->iSizeLimitReached );
    }

void UT_CMccFileSink::UT_CMccFileSink_MfcoSizeLimitReachedLL(  )
    {
    iFileSink->MfcoSizeLimitReachedL();
    EUNIT_ASSERT( iFileSink->iSizeLimitReached );
    }
void UT_CMccFileSink::UT_CMccFileSink_TimeToPlayL()
    {
    const TInt KMccMaxNumTimestamps = 5;
    const TInt KMccTimestampDifferenceMultiplier = 10;
    TInt originalTime( 1000 );
    TInt timeIncrement( 1000 );
    
    // Ask so long that average timestamp difference can be calculated
    for ( TInt i = 0; i < ( KMccTimestampDifferenceMultiplier + 2 ); i++ )
        {
        TTimeIntervalMicroSeconds timeToPlay = 
            iFileSink->TimeToPlayL( TTimeIntervalMicroSeconds( originalTime ) );
        
        EUNIT_ASSERT_EQUALS( timeToPlay.Int64(), originalTime );
            
        originalTime += timeIncrement; 
        }
    
    EUNIT_ASSERT_EQUALS( iFileSink->iTimestamps.Count(), KMccMaxNumTimestamps );
    
    // New timestamp is not enough little to cause timing correction
    TTimeIntervalMicroSeconds decreaseTime( originalTime - timeIncrement ); 
    TTimeIntervalMicroSeconds timeToPlay = iFileSink->TimeToPlayL( decreaseTime );
    
    EUNIT_ASSERT_EQUALS( (TInt)timeToPlay.Int64(), (TInt)decreaseTime.Int64() );
    EUNIT_ASSERT_EQUALS( iFileSink->iAddToTimestamp, 0 );
    
    // New timestamp is enough little to cause timing correction
    TInt difference( timeIncrement * ( KMccTimestampDifferenceMultiplier + 2 ) );
    TTimeIntervalMicroSeconds decreaseTime2( originalTime - difference ); 
    TTimeIntervalMicroSeconds timeToPlay2 = iFileSink->TimeToPlayL( decreaseTime2 );
    
    EUNIT_ASSERT_EQUALS( (TInt)timeToPlay2.Int64(), (TInt)( timeToPlay.Int64() + timeIncrement ) );
    EUNIT_ASSERT_EQUALS( (TInt)iFileSink->iAddToTimestamp, difference );
    EUNIT_ASSERT_EQUALS( iFileSink->iTimestamps.Count(), 0 );
    
    // Pausing causes timing correction
    iFileSink->iAddToTimestamp = 0;
    iFileSink->iPausedDuration = 2000;
    TTimeIntervalMicroSeconds timeToPlay3( timeToPlay2.Int64() + timeIncrement );
    TTimeIntervalMicroSeconds timeToPlay4 = iFileSink->TimeToPlayL( timeToPlay3 );
    EUNIT_ASSERT_EQUALS( (TInt)timeToPlay4.Int64(), 
                         (TInt)( timeToPlay3.Int64() - iFileSink->iPausedDuration.Int64() ) );
    
    // Has been paused but timing correction is not possible
    iFileSink->iTimestamps.Reset();
    iFileSink->iPreviousTimestamp = 0;
    iFileSink->iAddToTimestamp = 0;
    iFileSink->iPausedDuration = 100000;
    TTimeIntervalMicroSeconds timeToPlay5( 5000 );
    TTimeIntervalMicroSeconds timeToPlay6 = iFileSink->TimeToPlayL( timeToPlay5 );
    EUNIT_ASSERT_EQUALS( (TInt)timeToPlay6.Int64(), (TInt)timeToPlay5.Int64() );
    }

void UT_CMccFileSink::UT_CMccFileSink_SetPausedDurationL()
    {
    TTime t1(0);
    TTime t2(100);
    iFileSink->SetPausedDuration( t1, t2 );
    }

void UT_CMccFileSink::UT_CMccFileSink_UpdateActiveUserL()
    {
    delete iEventHandler;
    iEventHandler = NULL;
    iEventHandler = AddUserL();
	iEventHandler2 = AddUserL( ETrue );
	
    // Provide data type is different as currently is in use which
    // causes that filewriter needs to be reopened for the new user
    iFileSink->SetCurrentUser( iEventHandler );
    iFileSink->iActiveUserIndex = 0;
    TMediaId mediaId;
    mediaId.iMediaType = KUidMediaTypeVideo;
    iDLDataPathStub->SetSourceDataTypeCode( TFourCC( KMccFourCCIdAVC ), mediaId );

    iFileSink->UpdateActiveUserL( mediaId, *iDLDataPathStub );
    EUNIT_ASSERT_EQUALS( iFileSink->iActiveUserIndex, 1 );
    EUNIT_ASSERT( iFileSink->iAsyncEventHandler == iEventHandler2 );
    }

void UT_CMccFileSink::UT_CMccFileSink_CalculateAverageTimestampDifferenceL()
	{
	const TInt KMccMaxNumTimestamps = 5;
	const TInt KMccTimestampDifferenceMultiplier = 10;
	TInt originalTime(1000);
	TInt timeIncrement(1000);
	
	iFileSink->iCurrAvgTimestampDifference = 0;

	// Ask so long that average timestamp difference can be calculated
	for (TInt i = 0; i < (KMccTimestampDifferenceMultiplier + 2); i++)
		{
		TTimeIntervalMicroSeconds timeToPlay = iFileSink->TimeToPlayL(
				TTimeIntervalMicroSeconds(originalTime));

		EUNIT_ASSERT_EQUALS( timeToPlay.Int64(), originalTime );

		originalTime += timeIncrement;
		}

	EUNIT_ASSERT_EQUALS( iFileSink->iTimestamps.Count(), KMccMaxNumTimestamps );
	EUNIT_ASSERT_GREATER( iFileSink->iCurrAvgTimestampDifference, 0 );
	
	iFileSink->iTimestamps.Reset();
	iFileSink->iPreviousTimestamp = 500;
	iFileSink->TimeToPlayL( TTimeIntervalMicroSeconds( originalTime ) );
	EUNIT_ASSERT_GREATER( iFileSink->iTimestamps.Count(), 0 );
	
	originalTime = 1000;
	iFileSink->iAddToTimestamp = 0;
	iFileSink->iPreviousTimestamp = 1010;
	iFileSink->TimeToPlayL( TTimeIntervalMicroSeconds( originalTime ) );
	EUNIT_ASSERT_GREATER( iFileSink->iAddToTimestamp, 0 );
	}
    
// HELPERS

CMccTestEventHandler* UT_CMccFileSink::AddUserL( TBool aIsAvc )
    {
    CMccTestEventHandler* eventHandler = CMccTestEventHandler::NewL();
    CleanupStack::PushL( eventHandler );
    User::LeaveIfError( iFileSink->SinkThreadLogon( *eventHandler ) );
    iFileSink->SetCurrentUser( eventHandler );
    TMccCodecInfo codecInfo;
    if ( aIsAvc )
        {
        codecInfo.iSdpName = KAVCSdpName;
        codecInfo.iFourCC.Set(KMccFourCCIdAVC);
        }
    else
        {
        codecInfo.iSdpName = KH263SdpName;
        codecInfo.iFourCC.Set(KMccFourCCIdH263);
        }
    iFileSink->SetVideoCodecL( codecInfo );
    CleanupStack::Pop( eventHandler );
    return eventHandler;
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccFileSink,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "NewSinkL - test ",
    "CMccFileSink",
    "NewSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_NewSinkLL, Teardown)

EUNIT_TEST(
    "ConstructSinkL - test ",
    "CMccFileSink",
    "ConstructSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_ConstructSinkLL, Teardown)

EUNIT_TEST(
    "CMccFileSink - test ",
    "CMccFileSink",
    "CMccFileSink",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_CMccFileSinkL, Teardown)

EUNIT_TEST(
    "SetFileNameL - test ",
    "CMccFileSink",
    "SetFileNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SetFileNameLL, Teardown)

EUNIT_TEST(
    "RecordTimeAvailableL - test ",
    "CMccFileSink",
    "RecordTimeAvailableL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_RecordTimeAvailableLL, Teardown)

EUNIT_TEST(
    "SinkDataTypeCode - test ",
    "CMccFileSink",
    "SinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSinkDataTypeCode - test ",
    "CMccFileSink",
    "SetSinkDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SetSinkDataTypeCodeL, Teardown)

EUNIT_TEST(
    "BufferEmptiedL - test ",
    "CMccFileSink",
    "BufferEmptiedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_BufferEmptiedLL, Teardown)

EUNIT_TEST(
    "CanCreateSinkBuffer - test ",
    "CMccFileSink",
    "CanCreateSinkBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_CanCreateSinkBufferL, Teardown)

EUNIT_TEST(
    "CreateSinkBufferL - test ",
    "CMccFileSink",
    "CreateSinkBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_CreateSinkBufferLL, Teardown)

EUNIT_TEST(
    "SinkThreadLogon - test ",
    "CMccFileSink",
    "SinkThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkThreadLogonL, Teardown)

EUNIT_TEST(
    "SinkThreadLogoff - test ",
    "CMccFileSink",
    "SinkThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkThreadLogoffL, Teardown)

EUNIT_TEST(
    "SinkPrimeL - test ",
    "CMccFileSink",
    "SinkPrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkPrimeLL, Teardown)

EUNIT_TEST(
    "SinkPlayL - test ",
    "CMccFileSink",
    "SinkPlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkPlayLL, Teardown)

EUNIT_TEST(
    "SinkPauseL - test ",
    "CMccFileSink",
    "SinkPauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkPauseLL, Teardown)

EUNIT_TEST(
    "SinkStopL - test ",
    "CMccFileSink",
    "SinkStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SinkStopLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test ",
    "CMccFileSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_EmptyBufferLL, Teardown)

EUNIT_TEST(
    "EmptyBufferL - test 2 ",
    "CMccFileSink",
    "EmptyBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_EmptyBufferL2L, Teardown)
    
EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccFileSink",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_BufferFilledLL, Teardown)

EUNIT_TEST(
    "MfcoDiskFullL - test ",
    "CMccFileSink",
    "MfcoDiskFullL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_MfcoDiskFullLL, Teardown)

EUNIT_TEST(
    "MfcoSizeLimitReachedL - test ",
    "CMccFileSink",
    "MfcoSizeLimitReachedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_MfcoSizeLimitReachedLL, Teardown)

EUNIT_TEST(
    "TimeToPlay - test ",
    "CMccFileSink",
    "TimeToPlay",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_TimeToPlayL, Teardown)

EUNIT_TEST(
    "SetPausedDurationL - test ",
    "CMccFileSink",
    "SetPausedDurationL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_SetPausedDurationL, Teardown)

EUNIT_TEST(
    "UpdateActiveUserL - test ",
    "CMccFileSink",
    "UpdateActiveUserL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_UpdateActiveUserL, Teardown)    
    
EUNIT_TEST(
    "CalculateAverageTimestampDifferenceL - test ",
    "CMccFileSink",
    "CalculateAverageTimestampDifferenceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccFileSink_CalculateAverageTimestampDifferenceL, Teardown)
   
EUNIT_END_TEST_TABLE

//  END OF FILE
