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
* Description:    Uplink (record) datapath
*
*/




// INCLUDE FILES
#include <mmf/common/mmfcontroller.h>
#include "mcculdatapath.h"
#include "mccsubcontrollerlogs.h"


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
// CMccUlDataPath::CMccUlDataPath
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
CMccUlDataPath::CMccUlDataPath( 
	MAsyncEventHandler* aEventHandler,   
  	MMccResources* aMccResources,
	TMediaId aMediaId  ) : 
        CMccDataPathBase( aEventHandler, aMccResources, aMediaId )
    {
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CMccUlDataPath::ConstructL()
    {
	__SUBCONTROLLER( "CMccUlDataPath::ConstructL" )
 //   __ASSERT_ALWAYS( iEventHandler, User::Leave( KErrArgument ) );
	__SUBCONTROLLER( "CMccUlDataPath::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::NewL
// Static constructor.
// -----------------------------------------------------------------------------
CMccUlDataPath* CMccUlDataPath::NewL( MAsyncEventHandler* aEventHandler, 
                                	  MMccResources* aMccResources,
                                	  TMediaId aMediaId )
    {
  //  __ASSERT_ALWAYS ( aEventHandler, User::Leave(KErrArgument) );
    CMccUlDataPath* self = new ( ELeave ) CMccUlDataPath( 
    	aEventHandler, aMccResources, aMediaId );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::~CMccUlDataPath
// Destructor
// ----------------------------------------------------------------------------- 
CMccUlDataPath::~CMccUlDataPath()
    {
	__SUBCONTROLLER( "CMccUlDataPath::~CMccUlDataPath" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::PrimeL
// Primes the datapath
// -----------------------------------------------------------------------------
void CMccUlDataPath::PrimeL( TUint32 /*aEndpointId*/ )
    {
	__SUBCONTROLLER( "CMccUlDataPath::PrimeL" )
  /*  if ( iDataPathCreated && ( EStopped == State() ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PrimeL, priming" )

        TBool bufferReference;
        iSinkBuffer = iDataSink->CreateSinkBufferL( iMediaId, bufferReference);
        __ASSERT_ALWAYS ( iSinkBuffer, User::Leave(KErrGeneral) );
        __ASSERT_ALWAYS ( bufferReference, User::Leave(KErrGeneral) );

        // Prime the source
        iDataSource->SourcePrimeL();
        // Prime the sink
        iDataSink->SinkPrimeL();
        
        // And we're primed...
        SetStateL( EPrimed );
        
        // Other allocations...
        CreateCompleteCallBackL();
        }
    else if( EPrimed == State() )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PrimeL, already primed" )
        }
    else
        {
        __SUBCONTROLLER( "CMccUlDataPath::PrimeL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccUlDataPath::PrimeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PauseL
// Pauses the uplink streaming
// -----------------------------------------------------------------------------
void CMccUlDataPath::PauseL( TUint32 /*ndpointId*/ )
    {
	__SUBCONTROLLER( "CMccUlDataPath::PauseL" )
 /*   if( ERecording == State() && ( iDataPathCreated ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PauseL, already pausing" )
        iDataSource->SourceStopL(); 
        iDataSink->SinkPauseL();     
          
        SetStateL( EPaused );
        iSinkBuffer = NULL;
        Cancel();
        }
    else if( EPrimed == State() )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PauseL, already paused" )
        }
    else
        {
        __SUBCONTROLLER( "CMccUlDataPath::PauseL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccUlDataPath::PauseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::PlayL
// Starts the uplink playing
// -----------------------------------------------------------------------------
void CMccUlDataPath::PlayL( TUint32 /*aEndpointId*/ )
    {
	__SUBCONTROLLER( "CMccUlDataPath::PlayL" )
  /*  if ( iDataPathCreated && ( EPrimed == State() ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PlayL, playing" )
        SetStateL( ERecording );

        iDataPathCompletedErrorCode = KErrNone;       
        iDataSource->SourcePlayL();
        iDataSink->SinkPlayL();

        iSinkBuffer->SetStatus( EAvailable );          
        ChangeDataPathTransferState( ENeedSourceData );
        }
    else if( ERecording == State() )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PlayL, already playing" )
        }
    else
        {
        __SUBCONTROLLER( "CMccUlDataPath::PlayL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccUlDataPath::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::BufferEmptiedL
// Buffer has been emptied callback
// -----------------------------------------------------------------------------
void CMccUlDataPath::BufferEmptiedL( CMMFBuffer* aBuffer )
    {
	__ASSERT_ALWAYS ( aBuffer, User::Leave(KErrArgument) );
	/*
    // This is the buffer emptied by payload format encoder
    // If it returns NULL buffer then something serious has happened
    // The NULL buffer is cheked in the IsBufferSupportedL() function
    IsBufferSupportedL( aBuffer );

    //Has the datapath stopped running, if so were not interested in any callbacks.
    if( ERecording == State() )
        {
        if ( iSinkBuffer != aBuffer ) //buffer has been updated
            {
            iSinkBuffer = aBuffer;
            }     

        iSinkBuffer->SetStatus( EAvailable );
                ChangeDataPathTransferState( ENeedSourceData );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
        */
    };

// -----------------------------------------------------------------------------
// CMccUlDataPath::BufferFilledL
// Databuffer filled callback
// -----------------------------------------------------------------------------
void CMccUlDataPath::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    /*
    //Has the datapath stopped running, if so were not interested in any callbacks.
    if( ERecording == State() )
        {
        if ( !aBuffer )
            {
            ChangeDataPathTransferState( EEndOfData );
            }
        else //sink buffer is with datapath, see if there is anything to send to sink
            {
	        IsBufferSupportedL( aBuffer );
	        aBuffer->SetStatus( EFull );

		    if( iSinkBuffer != aBuffer )
	        	{
				CopyBuffer( iSinkBuffer, aBuffer );
	        	}	
		    
		    if ( !aBuffer->BufferSize() || aBuffer->LastBuffer() )
		    	{
		        //ignore zero length buffer request for next buffer from AudioInput
		        iSinkBuffer->SetStatus( EAvailable );

		        ChangeDataPathTransferState( ENeedSourceData );
		    	}
		    else 
			    { 			    
          	    ChangeDataPathTransferState( ESendDataToSink );		
			    }
            }
        }
    else if( EPaused == State() )
        {
        }
    else
        {
        User::Leave( KErrNotReady );
        }
        */
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::FillSourceBufferL
// Fill the source buffer
// -----------------------------------------------------------------------------
void CMccUlDataPath::FillSourceBufferL()
    {
    /*
    iSinkBuffer->SetStatus( EBeingFilled );
    iSinkBuffer->SetLastBuffer( EFalse );

    // wait for BufferFilled callback from source. Do this here as some sources cause
    // re-entrancy into data path via BufferFilledL
    ChangeDataPathTransferState( EWaitSource );
    
	iDataSource->FillBufferL( iSinkBuffer, this, iMediaId );
	*/
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::EmptySinkBufferL
// Empty the sink buffer
// -----------------------------------------------------------------------------
void CMccUlDataPath::EmptySinkBufferL()
    {
    /*
    ChangeDataPathTransferState( EWaitSink );  

    iSinkBuffer->SetStatus( EFull );
    TRAPD( error, iDataSink->EmptyBufferL( iSinkBuffer, this, iMediaId ) );

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
// CMccUlDataPath::DoEndOfData
// Worker function for data ending
// -----------------------------------------------------------------------------
void CMccUlDataPath::DoEndOfDataL()
    {
    /*
    if ( KErrNone == iDataPathCompletedErrorCode )
        {
        iDataPathCompletedErrorCode = KErrCompletion;
        }

    SignalDataPathCompleteL( iDataPathCompletedErrorCode );

    SetStateL( EStopped );
    */
    };

// -----------------------------------------------------------------------------
// CMccUlDataPath::Stop
// Stop the datapath
// -----------------------------------------------------------------------------
void CMccUlDataPath::StopL( TUint32 /*ndpointId*/ )
    {   
	__SUBCONTROLLER( "CMccUlDataPath::StopL" )
  /*  if ( ( iDataPathCreated )  && ( EStopped != State() ) )
        {
		__SUBCONTROLLER( "CMccUlDataPath::StopL, stopping" )
        iDataSource->SourceStopL();
        iDataSink->SinkStopL();
        iDataPathCompletedErrorCode = KErrNone;    
        iSinkBuffer = NULL;
        SetStateL( EStopped );                         
        Cancel();
        }
    else if( EStopped == State() )
        {
		__SUBCONTROLLER( "CMccUlDataPath::StopL, already stopped" )
        }
    else
        {
		__SUBCONTROLLER( "CMccUlDataPath::StopL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccUlDataPath::StopL, exit" )
    }


// -----------------------------------------------------------------------------
// CMccUlDataPath::NegotiateL
// Negotiate the source and sink
// -----------------------------------------------------------------------------
void CMccUlDataPath::NegotiateL( MDataSink& /*aDataSink*/ )
    {
	__SUBCONTROLLER( "CMccUlDataPath::NegotiateL" )
   /* iDataSink = &aDataSink;
    iDataSource->NegotiateSourceL( *iDataSink );
	DoNegotiateL();    
	*/
	__SUBCONTROLLER( "CMccUlDataPath::NegotiateL, exit" )
    }
    

// -----------------------------------------------------------------------------
// CMccUlDataPath::ResumeL
// Resumes pause audio streaming
// -----------------------------------------------------------------------------
void CMccUlDataPath::ResumeL( TUint32 /*aEndpointId*/ )
    {
    
	__SUBCONTROLLER( "CMccUlDataPath::ResumeL" )
	/*
    if ( iDataPathCreated && ( EPaused == State() ) )
        {
		__SUBCONTROLLER( "CMccUlDataPath::ResumeL, playing" )
        iDataSource->SourcePrimeL();        
        SetStateL( ERecording );
        iDataPathCompletedErrorCode = KErrNone;
        TBool bufferReference;
        iSinkBuffer = iDataSink->CreateSinkBufferL( iMediaId, bufferReference);
        __ASSERT_ALWAYS ( iSinkBuffer, User::Leave(KErrGeneral) );
        __ASSERT_ALWAYS ( bufferReference, User::Leave(KErrGeneral) );
        
        iDataSource->SourcePlayL();
        iDataSink->SinkPlayL();

        iSinkBuffer->SetStatus( EAvailable );
        ChangeDataPathTransferState( ENeedSourceData );
        }
    else if( ERecording == State() )
        {
		__SUBCONTROLLER( "CMccUlDataPath::ResumeL, already playing" )
        }
    else
        {
		__SUBCONTROLLER( "CMccUlDataPath::ResumeL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
        */
	__SUBCONTROLLER( "CMccUlDataPath::ResumeL, exit" )
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
