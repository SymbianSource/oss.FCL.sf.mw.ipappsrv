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
* Description:    Downlink (playback) datapath
*
*/




// INCLUDE FILES
#include <mmf/common/mmfcontroller.h>
#include "mccdldatapath.h"
#include "MccSubcontrollerLogs.h"

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
// CMccDlDataPath::CMccDlDataPath
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CMccDlDataPath::CMccDlDataPath( 
    MAsyncEventHandler* aEventHandler, 
    MMccResources* aMccResources,
    TMediaId aMediaId ) : 
        CMccDataPathBase( aEventHandler, aMccResources, aMediaId )
    {

    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CMccDlDataPath::ConstructL()
    {
	__SUBCONTROLLER( "CMccDlDataPath::ConstructL" )
	__SUBCONTROLLER( "CMccDlDataPath::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::NewL
// Static constructor.
// -----------------------------------------------------------------------------
CMccDlDataPath* CMccDlDataPath::NewL( 
	MAsyncEventHandler* aEventHandler, 
    MMccResources* aMccResources,
 	TMediaId aMediaId )
    {
   // __ASSERT_ALWAYS ( aEventHandler, User::Leave(KErrArgument) );
    
    CMccDlDataPath* self = new ( ELeave ) CMccDlDataPath( 
    	aEventHandler,aMccResources, aMediaId );        
    CleanupStack::PushL( self );   
    self->ConstructL();   
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::~CMccDlDataPath
// Destructor

// -----------------------------------------------------------------------------  
CMccDlDataPath::~CMccDlDataPath()
    {
	__SUBCONTROLLER( "CMccDlDataPath::~CMccDlDataPath" )
	__SUBCONTROLLER( "CMccDlDataPath::~CMccDlDataPath, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PrimeL
// Primes the datapath
// ----------------------------------------------------------------------------- 
void CMccDlDataPath::PrimeL( TUint32 /*aEndpointId*/ )
    {
	__SUBCONTROLLER( "CMccDlDataPath::PrimeL" )
 /*
    if ( iDataPathCreated && ( EStopped == State() ) )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PrimeL, priming" )
        iDataSource->NegotiateSourceL( *iDataSink );

        TBool bufferReference;
        iSourceBuffer = iDataSource->CreateSourceBufferL( iMediaId, bufferReference);
        __ASSERT_ALWAYS ( iSourceBuffer, User::Leave(KErrGeneral) );
        __ASSERT_ALWAYS ( bufferReference, User::Leave(KErrGeneral) );

        iDataSource->SourcePrimeL(); 
        iDataSink->SinkPrimeL();

        SetStateL( EPrimed );

  		CreateCompleteCallBackL();
        }
    else if( EPrimed == State() )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PrimeL, already primed" )
        }
    else
        {
		__SUBCONTROLLER( "CMccDlDataPath::PrimeL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccDlDataPath::PrimeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PauseL
// Pauses the downlink streaming
// -----------------------------------------------------------------------------
void CMccDlDataPath::PauseL( TUint32 /*ndpointId*/ )
    {
	__SUBCONTROLLER( "CMccDlDataPath::PauseL" )
	/*
    if ( ( iDataPathCreated ) && ( EPlaying == State() )  )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PauseL, pausing" )
        iDataSource->SourcePauseL(); 
        iDataSink->SinkPauseL();     
        SetStateL( EPaused );
        iSourceBuffer = NULL;
        Cancel();
        }
    else if( EPrimed == State() )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PauseL, already paused" )
        }
    else
        {
		__SUBCONTROLLER( "CMccDlDataPath::PauseL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccDlDataPath::PauseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PlayL
// Starts the downlink playing
// -----------------------------------------------------------------------------
void CMccDlDataPath::PlayL( TUint32 /*ndpointId*/ )
    {
	__SUBCONTROLLER( "CMccDlDataPath::PlayL" )
	/*
    if ( iDataPathCreated && ( EPrimed == State() ) )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PlayL, playing" )
        SetStateL( EPlaying );
        iDataPathCompletedErrorCode = KErrNone;
        iDataSource->SourcePlayL(); 
        iDataSink->SinkPlayL(); 
        iSourceBuffer->SetStatus( EAvailable );
        ChangeDataPathTransferState( ENeedSourceData );
        }
    else if( EPlaying == State() )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PlayL, already playing" )
        }
    else
        {
		__SUBCONTROLLER( "CMccDlDataPath::PlayL, KErrNotReady" )
        User::Leave( KErrNotReady  );
        }
        */
	__SUBCONTROLLER( "CMccDlDataPath::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::BufferEmptiedL
// From MDataSource
// ----------------------------------------------------------------------------- 
void CMccDlDataPath::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    /*
    // This is the buffer returned by DevSound when it has emptied it
    // Check the buffer that it is not NULL and it is supported.
    IsBufferSupportedL( aBuffer );
    if( EPlaying == State() )
        {
      	__ASSERT_ALWAYS ( aBuffer == iSourceBuffer, User::Leave(KErrArgument) );
	    
	    iSourceBuffer->SetStatus( EAvailable );
	    
        ChangeDataPathTransferState( ENeedSourceData );
        }
    else
        {
        // We're not streaming currently, can leave
        User::Leave( KErrNotReady );
        }
        */
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::BufferFilledL
// From MDataSink
// -----------------------------------------------------------------------------
void CMccDlDataPath::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::FillSourceBufferL
// Fill the source buffer
// -----------------------------------------------------------------------------
void CMccDlDataPath::FillSourceBufferL()
    {
  /*
  
  */
    
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::EmptySinkBufferL
// Empty the sink buffer
// -----------------------------------------------------------------------------
void CMccDlDataPath::EmptySinkBufferL()
    {
    /*
    ChangeDataPathTransferState( EWaitSink );  

    iSourceBuffer->SetStatus( EFull );
    TRAPD( error, iDataSink->EmptyBufferL( iSourceBuffer, this, iMediaId ) );

    if ( KErrEof == error || KErrOverflow == error || KErrUnderflow == error )
        {
        iDataPathCompletedErrorCode = error;
        ChangeDataPathTransferState( EEndOfData );
        }
    else
        {
        User::LeaveIfError( error );
        }
        */
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::DoEndOfDataL
// Worker function for data ending
// -----------------------------------------------------------------------------
void CMccDlDataPath::DoEndOfDataL()
    {
    /*
    if ( KUidMmfAudioOutput != iDataSink->DataSinkType() )
            {
            if ( KErrNone == iDataPathCompletedErrorCode )
                {
                iDataPathCompletedErrorCode = KErrCompletion;
                }
            }
    
    SignalDataPathCompleteL(iDataPathCompletedErrorCode);

    SetStateL( EStopped );
    */
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::Stop
// Stop the datapath
// -----------------------------------------------------------------------------
void CMccDlDataPath::StopL( TUint32 /*ndpointId*/ )
    {
	__SUBCONTROLLER( "CMccDlDataPath::PlayL" )
	/*
    if( ( iDataPathCreated )  && ( EStopped != State() )  )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PlayL, stopping" )
        iDataSource->SourceStopL();
        iDataSink->SinkStopL();
        iDataPathCompletedErrorCode = KErrNone;
        iSourceBuffer = NULL;
        SetStateL( EStopped );
        Cancel();
        }
    else if( EStopped == State() )
        {
		__SUBCONTROLLER( "CMccDlDataPath::PlayL, already stopped" )
        }
    else
        {
		__SUBCONTROLLER( "CMccDlDataPath::PlayL, KErrNotReady" )
        User::Leave( KErrNotReady  );
        }
        */
	__SUBCONTROLLER( "CMccDlDataPath::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::NegotiateL
// Negotiate the source and sink
// -----------------------------------------------------------------------------
void CMccDlDataPath::NegotiateL( MDataSource& /*aDataSource*/ )
    {
	__SUBCONTROLLER( "CMccDlDataPath::NegotiateL" )
	/*
    if( iDataSink )
        {
		__SUBCONTROLLER( "CMccDlDataPath::NegotiateL, negotiating" )
        ChangeDataPathTransferState( ENegotiate );
        iDataSource = &aDataSource;
        iDataSink->NegotiateL( *iDataSource );
        }
    else
        {
		__SUBCONTROLLER( "CMccDlDataPath::NegotiateL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccDlDataPath::NegotiateL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::ResumeL
// Resumes pause audio streaming
// -----------------------------------------------------------------------------
void CMccDlDataPath::ResumeL( TUint32 /*ndpointId*/ )
    {
	__SUBCONTROLLER( "CMccDlDataPath::ResumeL" )
	/*
    if ( iDataPathCreated && EPaused == State() )
        {
		__SUBCONTROLLER( "CMccDlDataPath::ResumeL, playing" )
        SetStateL( EPlaying );
        iDataPathCompletedErrorCode = KErrNone;
        TBool bufferReference;
        iSourceBuffer = iDataSource->CreateSourceBufferL( iMediaId, bufferReference);
        __ASSERT_ALWAYS ( iSourceBuffer, User::Leave(KErrGeneral) );
        __ASSERT_ALWAYS ( bufferReference, User::Leave(KErrGeneral) );
        iDataSource->SourcePlayL();
        iDataSink->SinkPlayL(); 
        iSourceBuffer->SetStatus( EAvailable );
        ChangeDataPathTransferState( ENeedSourceData );
        }
    else if( EPlaying == State() )
        {
		__SUBCONTROLLER( "CMccDlDataPath::ResumeL, already playing" )
        }
    else
        {
		__SUBCONTROLLER( "CMccDlDataPath::ResumeL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccDlDataPath::ResumeL, exit" )
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
