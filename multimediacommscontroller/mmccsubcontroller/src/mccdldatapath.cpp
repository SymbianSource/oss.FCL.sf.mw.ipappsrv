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
* Description:    Downlink (playback) datapath
*
*/




// INCLUDE FILES
#include <mmf/common/mmfcontroller.h>
#include "mccdldatapath.h"
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
	__ASSERT_ALWAYS( iEventHandler, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iMccResources, User::Leave( KErrArgument ) );
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
    __ASSERT_ALWAYS ( aEventHandler, User::Leave(KErrArgument) );
    
    CMccDlDataPath* self = new ( ELeave ) CMccDlDataPath( aEventHandler,
                                                          aMccResources,
                                                          aMediaId );        
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
void CMccDlDataPath::PrimeL( TUint32 aEndpointId )
    {
    __SUBCONTROLLER( "CMccDlDataPath::PrimeL" )
    
    __ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );	
	__ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
    
    if ( !iSourceBuffer )
        {
        TBool bufferReference;
        iSourceBuffer = 
            iDataSource->CreateSourceBufferL( iMediaId, bufferReference );
        __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrGeneral ) );
        __ASSERT_ALWAYS( bufferReference, User::Leave( KErrGeneral ) );
        }
    
    if ( ControlSource( aEndpointId ) )
        {
        __SUBCONTROLLER( "CMccDlDataPath::PrimeL, control source" )
        
        SetStateL( EPrimed );
        
        iDataSource->NegotiateSourceL( *iDataSink );
        
        iDataSource->SourcePrimeL(); 
        }
    
    if ( ControlSink( aEndpointId ) )   
        {
        __SUBCONTROLLER( "CMccDlDataPath::PrimeL, control sink" )
        
        iDataSink->SinkPrimeL();
        }
        
    CreateCompleteCallBackL();

	__SUBCONTROLLER( "CMccDlDataPath::PrimeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PauseL
// Pauses the downlink streaming
// -----------------------------------------------------------------------------
void CMccDlDataPath::PauseL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccDlDataPath::PauseL" )
	
	__ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
	
	if ( ControlSource( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::PauseL, control source" )
	    
	    SetStateL( EPaused );
	    
	    iDataSource->SourcePauseL(); 
	    }
	
	if ( ControlSink( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::PauseL, control sink" )
	    
	    if ( IsMmfEndpoint( iAssociatedSink ) )
	        {
	        __SUBCONTROLLER( "CMccDlDataPath::PauseL, stop mmf sink" )
	        
	        // Resource has to be freed
	        iDataSink->SinkStopL();
	        }
	    else
	        { 
	        iDataSink->SinkPauseL(); 
	        }
	    }
	    
	if ( State() == EPaused )
	    {
	    iSourceBuffer = NULL;
    	
    	// Cancel the whole path    
    	Cancel();
    	
    	ChangeDataPathTransferState( ECanceled );
	    }

	__SUBCONTROLLER( "CMccDlDataPath::PauseL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::PlayL
// Starts the downlink playing
// -----------------------------------------------------------------------------
void CMccDlDataPath::PlayL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccDlDataPath::PlayL" )
	
	__ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iDataSource, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrGeneral ) );
    __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrGeneral ) );

    iDataPathCompletedErrorCode = KErrNone;
    
    if ( ControlSource( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::PlayL, control source" )
	    
	    SetStateL( EStreaming );
      
        iDataSource->SourcePlayL(); 
	    }

    if ( ControlSink( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::PlayL, control sink" )
	    
        iDataSink->SinkPlayL(); 
	    }
	    
	if ( State() == EStreaming && ChangeDataPathTransferState( ENeedSourceData ) )
	    {   	
    	ActivateSourceBuffer();
	    }

	__SUBCONTROLLER( "CMccDlDataPath::PlayL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::BufferEmptiedL
// From MDataSource
// ----------------------------------------------------------------------------- 
void CMccDlDataPath::BufferEmptiedL( CMMFBuffer* aBuffer )
    {
    // This is the buffer returned by DevSound when it has emptied it
    // Check the buffer that it is not NULL and it is supported.
    IsBufferSupportedL( aBuffer );
    if( EStreaming == State() )
        {
      	__ASSERT_ALWAYS( aBuffer == iSourceBuffer, User::Leave( KErrArgument ) );
      	
	    iSourceBuffer->SetStatus( EAvailable );
	    
        ChangeDataPathTransferState( ENeedSourceData );
        }
    else
        {
        // We're not streaming currently, can leave
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::BufferFilledL
// From MDataSink
// -----------------------------------------------------------------------------
void CMccDlDataPath::BufferFilledL( CMMFBuffer* aBuffer )
    {
    if( EStreaming == State() )
        {
        // There is a leave if aBuffer is NULL
        // the ownership of the aBuffer is in source, so source
        // must not give a NULL aBuffer
        IsBufferSupportedL( aBuffer );
 
	    if ( !aBuffer->BufferSize() /*|| aBuffer->LastBuffer()*/  )
            {
            __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrArgument ) );

            // Just in-case we are terminating on BufferSize == 0 or play window
            iSourceBuffer->SetStatus( EAvailable );  
            ChangeDataPathTransferState( ENeedSourceData, ETrue );
            }
        else
	        {
		    if( iSourceBuffer != aBuffer )
	            {
	            // Buffer has been changed by the source
	            iSourceBuffer = aBuffer;
            	}
            
            ChangeDataPathTransferState( ESendDataToSink );	
	        }
        }
    else
        {
        // We're not streaming so we can leave
        User::Leave( KErrNotReady );
        }
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::FillSourceBufferL
// Fill the source buffer
// -----------------------------------------------------------------------------
void CMccDlDataPath::FillSourceBufferL()
    {
    __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrNotReady ) );
    
    iSourceBuffer->SetStatus( EBeingFilled );
    iSourceBuffer->SetLastBuffer( EFalse );
    ChangeDataPathTransferState( EWaitSource );
    iDataSource->FillBufferL( iSourceBuffer, this, iMediaId );
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::EmptySinkBufferL
// Empty the sink buffer
// -----------------------------------------------------------------------------
void CMccDlDataPath::EmptySinkBufferL()
    {
    __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrNotReady ) );
    
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
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::DoEndOfDataL
// Worker function for data ending
// -----------------------------------------------------------------------------
void CMccDlDataPath::DoEndOfDataL()
    {
    if ( KUidMmfAudioOutput != iDataSink->DataSinkType() )
        {
        if ( KErrNone == iDataPathCompletedErrorCode )
            {
            iDataPathCompletedErrorCode = KErrCompletion;
            }
        }
    
    SignalDataPathCompleteL(iDataPathCompletedErrorCode);

    SetStateL( EStopped );
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::Stop
// Stop the datapath
// -----------------------------------------------------------------------------
void CMccDlDataPath::StopL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccDlDataPath::StopL" )
	
	__ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );

    iDataPathCompletedErrorCode = KErrNone;
    	    
    if ( ControlSource( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::StopL, control source" )
	    
	    SetStateL( EStopped );
	    
        iDataSource->SourceStopL();
	    }
	    
    if ( ControlSink( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::StopL, control sink" )
	    
        iDataSink->SinkStopL();
	    }
	    
	if ( State() == EStopped )
	    {     	    
	    iSourceBuffer = NULL;
	
	    // Stop the whole path    
	    Cancel();
	    
	    ChangeDataPathTransferState( ECanceled );
	    }

	__SUBCONTROLLER( "CMccDlDataPath::StopL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::NegotiateL
// Negotiate the source and sink
// -----------------------------------------------------------------------------
void CMccDlDataPath::NegotiateL( MDataSource& aDataSource )
    {
    __SUBCONTROLLER( "CMccDlDataPath::NegotiateL" )
    
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( &aDataSource, User::Leave( KErrArgument ) );
    
    // NegotiateSourceL will do a re-entry so we need to have iDataSource set
    // for that call to succeed.
    iDataSource = &aDataSource;
    iDataSource->NegotiateSourceL( *iDataSink );
    iDataSource = NULL;
    this->AddDataSourceL( &aDataSource );
    
    __SUBCONTROLLER( "CMccDlDataPath::NegotiateL exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::LoadL
// -----------------------------------------------------------------------------
void CMccDlDataPath::LoadL( MDataSource& aDataSource )
    {
    __SUBCONTROLLER( "CMccDlDataPath::LoadL" )
    
    __ASSERT_ALWAYS( iDataSink, User::Leave( KErrNotReady ) );
    iDataSink->NegotiateL( aDataSource );
    
    __SUBCONTROLLER( "CMccDlDataPath::LoadL exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::ResumeL
// Resumes pause audio streaming
// -----------------------------------------------------------------------------
void CMccDlDataPath::ResumeL( TUint32 aEndpointId )
    {
	__SUBCONTROLLER( "CMccDlDataPath::ResumeL" )
    
    __ASSERT_ALWAYS( iDataPathCreated, User::Leave( KErrNotReady ) );

    iDataPathCompletedErrorCode = KErrNone;

    if ( ControlSource( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::ResumeL, control source" )
	    
	    SetStateL( EStreaming );
	    
        iDataSource->SourcePlayL();
	    }
	
	if ( ControlSink( aEndpointId ) )
	    {
	    __SUBCONTROLLER( "CMccDlDataPath::ResumeL, control sink" )   
	    
	    if ( IsMmfEndpoint( iAssociatedSink ) )
	        {
	        __SUBCONTROLLER( "CMccDlDataPath::ResumeL, prime mmf sink" )
	        
	        // Resource was freed at pause, initialize it again
	        iDataSink->SinkPrimeL();
	        }
	     
        iDataSink->SinkPlayL(); 
	    }
	    
	if ( State() == EStreaming && ChangeDataPathTransferState( ENeedSourceData ) )
	    {     	    	    
	    iSourceBuffer = NULL;
        TBool bufferReference;
        iSourceBuffer = 
            iDataSource->CreateSourceBufferL( iMediaId, bufferReference );
        __ASSERT_ALWAYS( iSourceBuffer, User::Leave( KErrGeneral ) );
        __ASSERT_ALWAYS( bufferReference, User::Leave( KErrGeneral ) );
        	 
	    ActivateSourceBuffer();
	    }

	__SUBCONTROLLER( "CMccDlDataPath::ResumeL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDlDataPath::ActivateSourceBuffer
// Sets source buffer available
// -----------------------------------------------------------------------------    
void CMccDlDataPath::ActivateSourceBuffer()
    {
    if ( iSourceBuffer )
        {
        iSourceBuffer->SetStatus( EAvailable );
        }
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
