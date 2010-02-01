/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
    __ASSERT_ALWAYS( iEventHandler, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iMccResources, User::Leave( KErrArgument ) );
	__SUBCONTROLLER( "CMccUlDataPath::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::NewL
// Static constructor.
// -----------------------------------------------------------------------------
CMccUlDataPath* CMccUlDataPath::NewL( 
    MAsyncEventHandler* aEventHandler, 
    MMccResources* aMccResources,
    TMediaId aMediaId )
    {
    __ASSERT_ALWAYS ( aEventHandler, User::Leave(KErrArgument) );
    CMccUlDataPath* self = 
        new ( ELeave ) CMccUlDataPath( aEventHandler, aMccResources, aMediaId );
    
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
void CMccUlDataPath::PrimeL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccUlDataPath::PrimeL" )
	
	__ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
    
    if ( !iSinkBuffer )
        {
        TBool bufferReference;
        iSinkBuffer = iDataSink->CreateSinkBufferL( iMediaId, bufferReference );
        __ASSERT_ALWAYS ( iSinkBuffer, User::Leave( KErrGeneral ) );
        __ASSERT_ALWAYS ( bufferReference, User::Leave( KErrGeneral ) );
        }
        
    if ( ControlSink( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PrimeL, control sink" )

        iDataSink->SinkPrimeL();
        }
        
    if ( ControlSource( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PrimeL, control source" )
        
        SetStateL( EPrimed );
        
        iDataSource->SourcePrimeL();
        }

    CreateCompleteCallBackL();

	__SUBCONTROLLER( "CMccUlDataPath::PrimeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PauseL
// Pauses the uplink streaming
// -----------------------------------------------------------------------------
void CMccUlDataPath::PauseL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccUlDataPath::PauseL" )
	
	__ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
        
    if ( ControlSink( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PauseL, control sink" )

        iDataSink->SinkPauseL();
        }
    
    if ( ControlSource( aEndpointId ) )
        {   
        __SUBCONTROLLER( "CMccUlDataPath::PauseL, control source" )
        
        SetStateL( EPaused );
        
        if ( IsMmfEndpoint( iAssociatedSource ) )
            {
            __SUBCONTROLLER( "CMccUlDataPath::PauseL, stop mmf source" )
            
            // Resource has to be freed
            iDataSource->SourceStopL(); 
            }
        else
            {
            iDataSource->SourcePauseL();
            }        
        } 
        
    if ( State() == EPaused )
        {                
        iSinkBuffer = NULL;
            
        // Pause the whole path       
        Cancel();
        
        ChangeDataPathTransferState( ECanceled );
        }

	__SUBCONTROLLER( "CMccUlDataPath::PauseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::PlayL
// Starts the uplink playing
// -----------------------------------------------------------------------------
void CMccUlDataPath::PlayL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccUlDataPath::PlayL" )
	
	__ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iSinkBuffer, User::Leave( KErrGeneral ) );
           
    iDataPathCompletedErrorCode = KErrNone; 
         
    if ( ControlSink( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PlayL, control sink" )

        iDataSink->SinkPlayL();
        }
    
    if ( ControlSource( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::PlayL, control source" )
        
        SetStateL( EStreaming );
        
        iDataSource->SourcePlayL();
        }
        
    if ( State() == EStreaming && ChangeDataPathTransferState( ENeedSourceData ) )
        {                
        ActivateSinkBuffer();
        }

	__SUBCONTROLLER( "CMccUlDataPath::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::BufferEmptiedL
// Buffer has been emptied callback
// -----------------------------------------------------------------------------
void CMccUlDataPath::BufferEmptiedL( CMMFBuffer* aBuffer )
    {
	__ASSERT_ALWAYS ( aBuffer, User::Leave(KErrArgument) );
	
    // This is the buffer emptied by payload format encoder
    // If it returns NULL buffer then something serious has happened
    // The NULL buffer is cheked in the IsBufferSupportedL() function
    IsBufferSupportedL( aBuffer );

    //Has the datapath stopped running, if so were not interested in any callbacks.
    if( EStreaming == State() )
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
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::BufferFilledL
// Databuffer filled callback
// -----------------------------------------------------------------------------
void CMccUlDataPath::BufferFilledL( CMMFBuffer* aBuffer )
    {
    // Has the datapath stopped running, if so were not interested in any
    // callbacks.
    if( EStreaming == State() )
        {
        if ( !aBuffer )
            {
            ChangeDataPathTransferState( EEndOfData );
            }
        else
            {
            // Sink buffer is with datapath, see if there is anything to send
            // to sink
	        IsBufferSupportedL( aBuffer );
	        aBuffer->SetStatus( EFull );
	        
	        if ( iSinkBuffer != aBuffer )
		        {
	            CMMFDataBuffer* desBuffer = static_cast<CMMFDataBuffer*>(iSinkBuffer);
		        if ( (TInt) aBuffer->BufferSize() > desBuffer->Data().MaxLength() )	
			        {
			        // if the buffer from source is too big, ignore it
			        // ask next one
                    __SUBCONTROLLER( "CMccUlDataPath::BufferFilledL, DATA IGNORED" )
                    __SUBCONTROLLER_INT1( "CMccUlDataPath::BufferFilledL, source", aBuffer->BufferSize() )
                    __SUBCONTROLLER_INT1( "CMccUlDataPath::BufferFilledL, dest", desBuffer->Data().MaxLength() )
			        iSinkBuffer->SetStatus( EAvailable );
			        aBuffer->SetStatus( EAvailable );
		            ChangeDataPathTransferState( ENeedSourceData );	
		            return;
			        }
			    else
				    {
				   	CopyBuffer( iSinkBuffer, aBuffer );
				    }
		        }
		    
		    if( !aBuffer->BufferSize() || aBuffer->LastBuffer() )
			    {
			    //ignore zero length buffer request for next buffer from AudioInput
		        iSinkBuffer->SetStatus( EAvailable );

		        ChangeDataPathTransferState( ENeedSourceData, ETrue );	
			    }
			else
				{
				ChangeDataPathTransferState( ESendDataToSink );	
				}
            }
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::FillSourceBufferL
// Fill the source buffer
// -----------------------------------------------------------------------------
void CMccUlDataPath::FillSourceBufferL()
    {
    __ASSERT_ALWAYS( iSinkBuffer, User::Leave( KErrNotReady ) );
    
    iSinkBuffer->SetStatus( EBeingFilled );
    iSinkBuffer->SetLastBuffer( EFalse );

    // wait for BufferFilled callback from source. Do this here as some sources cause
    // re-entrancy into data path via BufferFilledL
    ChangeDataPathTransferState( EWaitSource );
    
	iDataSource->FillBufferL( iSinkBuffer, this, iMediaId );
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::EmptySinkBufferL
// Empty the sink buffer
// -----------------------------------------------------------------------------
void CMccUlDataPath::EmptySinkBufferL()
    {
    __ASSERT_ALWAYS( iSinkBuffer, User::Leave( KErrNotReady ) );
    
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
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::DoEndOfData
// Worker function for data ending
// -----------------------------------------------------------------------------
void CMccUlDataPath::DoEndOfDataL()
    {
    if ( KErrNone == iDataPathCompletedErrorCode )
        {
        iDataPathCompletedErrorCode = KErrCompletion;
        }

    SignalDataPathCompleteL( iDataPathCompletedErrorCode );

    SetStateL( EStopped );
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::Stop
// Stop the datapath
// -----------------------------------------------------------------------------
void CMccUlDataPath::StopL( TUint32 aEndpointId )
    {   
	__SUBCONTROLLER( "CMccUlDataPath::StopL" )
    
    __ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
	__ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );

    iDataPathCompletedErrorCode = KErrNone;
            
    if ( ControlSink( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::StopL, control sink" )

        iDataSink->SinkStopL();                  
        }
    
    if ( ControlSource( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::StopL, control source" )
        
        SetStateL( EStopped ); 
        
        iDataSource->SourceStopL();  
        }
        
    if ( State() == EStopped )
        {
        iSinkBuffer = NULL;   
    
        // Stop the whole path    
        Cancel();
        
        ChangeDataPathTransferState( ECanceled );
        }
    
	__SUBCONTROLLER( "CMccUlDataPath::StopL, exit" )
    }


// -----------------------------------------------------------------------------
// CMccUlDataPath::NegotiateL
// Negotiate the source and sink
// -----------------------------------------------------------------------------
void CMccUlDataPath::NegotiateL( MDataSink& aDataSink )
    {
    __SUBCONTROLLER( "CMccUlDataPath::NegotiateL" )
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( &aDataSink, User::Leave( KErrArgument ) );
    	
    // NegotiateL call will lead to a re-entry so we need to have iDataSink set
    // for it to succeed.
    iDataSink = &aDataSink;
	iDataSink->NegotiateL( *iDataSource );
    iDataSink = NULL;
    this->AddDataSinkL( &aDataSink );
    
    __SUBCONTROLLER( "CMccUlDataPath::NegotiateL exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccUlDataPath::LoadL
// -----------------------------------------------------------------------------
void CMccUlDataPath::LoadL( MDataSink& aDataSink )
    {
	__SUBCONTROLLER( "CMccUlDataPath::LoadL" )
	
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrNotReady ) );
    iDataSource->NegotiateSourceL( aDataSink );
    
	__SUBCONTROLLER( "CMccUlDataPath::LoadL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::ResumeL
// Resumes pause audio streaming
// -----------------------------------------------------------------------------
void CMccUlDataPath::ResumeL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccUlDataPath::ResumeL" )
	
    __ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
	__ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
    
    iDataPathCompletedErrorCode = KErrNone;
                
    if ( ControlSink( aEndpointId ) )
        {       
        __SUBCONTROLLER( "CMccUlDataPath::ResumeL, control sink" )

        iDataSink->SinkPlayL();
        }
        
    if ( ControlSource( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccUlDataPath::ResumeL, control source" )
        
        SetStateL( EStreaming );
        
        if ( IsMmfEndpoint( iAssociatedSource ) )
            {
            __SUBCONTROLLER( "CMccUlDataPath::ResumeL, prime mmf source" )
            
            // Resource was freed at pause, initialize it again
            iDataSource->SourcePrimeL(); 
            }

        iDataSource->SourcePlayL();  
        }
        
    if ( State() == EStreaming && ChangeDataPathTransferState( ENeedSourceData ) )
        {
        iSinkBuffer = NULL;
        TBool bufferReference;
        iSinkBuffer = iDataSink->CreateSinkBufferL( iMediaId, bufferReference );
        __ASSERT_ALWAYS( iSinkBuffer, User::Leave( KErrGeneral ) );
        __ASSERT_ALWAYS( bufferReference, User::Leave( KErrGeneral ) );
        
        ActivateSinkBuffer();
        }
    
	__SUBCONTROLLER( "CMccUlDataPath::ResumeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::ActivateSinkBuffer
// Sets sink buffer available
// -----------------------------------------------------------------------------    
void CMccUlDataPath::ActivateSinkBuffer()
    {
    if ( iSinkBuffer )
        {
        iSinkBuffer->SetStatus( EAvailable );
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
