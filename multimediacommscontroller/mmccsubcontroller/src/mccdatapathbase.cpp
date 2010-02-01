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
* Description:    Datapath baseclass
*
*/




// INCLUDE FILES
#include <mmf/common/mmfutilities.h>
#include "mccdatapathbase.h"
#include "mcccompletecallback.h"
#include "mccsubcontrollerlogs.h"
#include "mccinternaldef.h"

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
// CMccDataPathBase::AddDataSourceL
// Adds a datasource to the datapath
// -----------------------------------------------------------------------------
void CMccDataPathBase::AddDataSourceL( MDataSource* aSource )
    {
	__SUBCONTROLLER( "CMccDataPathBase::AddDataSourceL" )
    __ASSERT_ALWAYS( aSource, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( EStopped == iState, User::Leave( KErrNotReady ) );
        
    if ( !iDataSink ) 
        {
		__SUBCONTROLLER( "CMccDataPathBase::AddDataSourceL, added" )
        iDataSource = aSource; 
        }
    else 
        {
		__SUBCONTROLLER( "CMccDataPathBase::AddDataSourceL, creating path" )
        this->CreateDataPathL( aSource, iDataSink );
        iDataSource = aSource;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::AddDataSinkL
// Adds a datasink to the datapath
// -----------------------------------------------------------------------------
void CMccDataPathBase::AddDataSinkL( MDataSink* aSink )
    {
	__SUBCONTROLLER( "CMccDataPathBase::AddDataSinkL" )
    __ASSERT_ALWAYS( aSink, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( EStopped == iState, User::Leave( KErrNotReady ) );
    
    if ( !iDataSource ) 
        {
		__SUBCONTROLLER( "CMccDataPathBase::AddDataSinkL, added" )
        iDataSink = aSink; 
        }
    else 
        {
		__SUBCONTROLLER( "CMccDataPathBase::AddDataSinkL, creating path" )
        this->CreateDataPathL( iDataSource, aSink );
        iDataSink = aSink;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetAssociatedSink
// -----------------------------------------------------------------------------    
void CMccDataPathBase::SetAssociatedSink( MDataSink* aAssociatedSink )
    {
    __SUBCONTROLLER_INT1( "CMccDataPathBase::SetAssociatedSinkId, id:", 
                          MCC_ENDPOINT_ID( aAssociatedSink ) )
    
    iAssociatedSink = aAssociatedSink;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetAssociatedSource
// -----------------------------------------------------------------------------        
void CMccDataPathBase::SetAssociatedSource( MDataSource* aAssociatedSource )
    {
    __SUBCONTROLLER_INT1( "CMccDataPathBase::SetAssociatedSourceId,id:", 
                          MCC_ENDPOINT_ID( aAssociatedSource ) )
                          
    iAssociatedSource = aAssociatedSource;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::ChangeDataPathTransferState
// Changes the transfer state of the datapath
// ----------------------------------------------------------------------------- 
TBool CMccDataPathBase::ChangeDataPathTransferState( 
    TTransferState aTransferState, 
    TBool aFailureFallback )
    {
    // On failure fallback situation, keep on asking data
    if ( !aFailureFallback &&
       ( iTransferState == EWaitSource && aTransferState == ENeedSourceData ) )
        {
        // Do not allow fillbuffer request if such request is already active
        return EFalse;
        }
           
    iTransferState = aTransferState;
    
    switch( iState )
        {
        case EStreaming:
            if ( ( EWaitSink != iTransferState ) && 
                 ( EWaitSource != iTransferState ) &&
                 ( ECanceled != iTransferState ) )
                {
                // If not waiting for something to happen, activate
                TRequestStatus* stat = &iStatus;
                this->Cancel(); 
                User::RequestComplete( stat, KErrNone );
                this->SetActive();
                }
            break;
        default:
            // Not a supported state for transfer state
            break;
        }
        
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::CreateDataPathL
// Datapath creating
// -----------------------------------------------------------------------------
void CMccDataPathBase::CreateDataPathL( MDataSource* aSource, MDataSink* aSink )
    { 
	__SUBCONTROLLER( "CMccDataPathBase::CreateDataPathL" )
    iDataPathCreated = EFalse;
    
    if ( aSource && aSink ) 
        { 
        TFourCC sourceFourCC = aSource->SourceDataTypeCode( iMediaId );
        TFourCC sinkFourCC = aSink->SinkDataTypeCode( iMediaId );
		__SUBCONTROLLER_INT1( "CMccDataPathBase source", aSource->DataSourceType().iUid )
		__SUBCONTROLLER_INT1( "CMccDataPathBase sourceFourCC", sourceFourCC.FourCC() )
		__SUBCONTROLLER_INT1( "CMccDataPathBase sink", aSink->DataSinkType().iUid )
		__SUBCONTROLLER_INT1( "CMccDataPathBase sinkFourCC", sinkFourCC.FourCC() )
        
        if ( ( sourceFourCC != sinkFourCC ) && 
             ( KMMFFourCCCodeNULL != sourceFourCC.FourCC() ) && 
             ( KMMFFourCCCodeNULL != sinkFourCC.FourCC() ) )
            {
			__SUBCONTROLLER( "CMccDataPathBase::CreateDataPathL, not compatible, ignored" )			
            iDataPathCreated = ETrue;
            } 
        else
            { 
			__SUBCONTROLLER( "CMccDataPathBase::CreateDataPathL, created" )
            iDataPathCreated = ETrue;
            }

        User::LeaveIfError( this->SetSinkDataTypeCode( sourceFourCC, iMediaId ) );
        User::LeaveIfError( this->SetSourceDataTypeCode( sourceFourCC, iMediaId ) );
        }
    else
        {
		__SUBCONTROLLER( "CMccDataPathBase::CreateDataPathL, KErrNotReady" )
        User::Leave( KErrNotReady );
        }
	__SUBCONTROLLER( "CMccDataPathBase::CreateDataPathL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::~CMccDataPathBase
// Destructor
// -----------------------------------------------------------------------------
CMccDataPathBase::~CMccDataPathBase()
    {
    delete iCompleteCallback;
    Cancel();

    
    iDataSink = NULL;
    iDataSource = NULL;
    iSourceBuffer = NULL;  
    iSinkBuffer = NULL;
    iEventHandler = NULL;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::ConstructSinkL
// From MDataSink
// -----------------------------------------------------------------------------
void CMccDataPathBase::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::ConstructSourceL
// From MDataSource
// -----------------------------------------------------------------------------
void CMccDataPathBase::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::CanCreateSinkBuffer
// From MDataSink
// -----------------------------------------------------------------------------
TBool CMccDataPathBase::CanCreateSinkBuffer()
    {
    // CMccDataPathBase can't create buffers
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::CanCreateSourceBuffer
// From MDataSource
// -----------------------------------------------------------------------------
TBool CMccDataPathBase::CanCreateSourceBuffer()
    {
    // CMccDataPathBase can't create buffers
    return EFalse; 
    }

// -----------------------------------------------------------------------------
// CMccUlDataPath::CreateSinkBufferL
// From MDataSink
// -----------------------------------------------------------------------------
CMMFBuffer* CMccDataPathBase::CreateSinkBufferL( TMediaId /*aMediaId*/, 
                                                         TBool& /*aReference*/ )
    {
    // CMccDataPathBase can't create buffers
    User::Leave( KErrNotSupported );
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::CreateSourceBufferL
// From MDataSource
// -----------------------------------------------------------------------------
CMMFBuffer* CMccDataPathBase::CreateSourceBufferL( TMediaId /*aMediaId*/, 
                                                         TBool& /*aReference*/ )
    {
    // CMccDataPathBase can't create buffers
    User::Leave( KErrNotSupported );
    return NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// CMccDataPathBase::FillBufferL
// From MDataSource
// -----------------------------------------------------------------------------
void CMccDataPathBase::FillBufferL( CMMFBuffer* /*aBuffer*/, 
        MDataSink* /*aConsumer*/, TMediaId /*aMediaId*/ )
    {
    // Not implemented
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::IsBufferSupportedL
// Checks if the given buffer is supported
// -----------------------------------------------------------------------------
void CMccDataPathBase::IsBufferSupportedL( CMMFBuffer* aBuffer )
    {
   	__ASSERT_ALWAYS ( aBuffer, User::Leave(KErrArgument) );

    TUid type = aBuffer->Type();

    if( KUidMmfDataBuffer == type )
        {
        return;
        }
    else if( KUidMmfTransferBuffer == type )
        {
        return;
        }
    else if( KUidMmfAudioBuffer == type )
        {
        return;
        }
    else if( KUidMmfPtrBuffer == type )
        {
        return;
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::CopyBuffer
// copy data and settings from one buffer to another
// -----------------------------------------------------------------------------
void CMccDataPathBase::CopyBuffer( CMMFBuffer* aDesBuffer,
                                   CMMFBuffer* aOrigBuffer)
	{
	// No need to copy if parameters point to the same buffer.
	if ( ( aDesBuffer && aOrigBuffer ) && ( aDesBuffer != aOrigBuffer ) )
	    {
    	// copy buffer data
    	CMMFDataBuffer* desBuffer(
    	    static_cast<CMMFDataBuffer*>( aDesBuffer ) );
        
        desBuffer->Data().Copy(
            static_cast<CMMFDataBuffer*>( aOrigBuffer )->Data() );	
        
        // copy buffer setting
        aDesBuffer->SetFrameNumber( aOrigBuffer->FrameNumber() );
        aDesBuffer->SetLastBuffer( aOrigBuffer->LastBuffer() );
        aDesBuffer->SetPosition( aOrigBuffer->Position() );
        aDesBuffer->SetStatus( aOrigBuffer->Status() );
        aDesBuffer->SetTimeToPlay( aOrigBuffer->TimeToPlay() );
	    }
	else
	    {
	    __SUBCONTROLLER("CMccDataPathBase::CopyBuffer, null buffer")	
	    }
	}

// -----------------------------------------------------------------------------
// CMccDataPathBase::CreateCompleteCallBackL
// a CCompleteCallback object is created
// -----------------------------------------------------------------------------
void CMccDataPathBase::CreateCompleteCallBackL()
	{
    __ASSERT_ALWAYS( iDataSink, User::Leave(KErrArgument) );

	if ( iCompleteCallback )
        {
        delete iCompleteCallback;
        iCompleteCallback = NULL;
        }
        
    TBool waitForSink = IsMmfEndpoint( iDataSink );
    iCompleteCallback = new ( ELeave ) CCompleteCallback( *iEventHandler, waitForSink );	
	}

// -----------------------------------------------------------------------------
// CMccDataPathBase::SignalDataPathCompleteL
// a signal the datapath complete with a error code
// -----------------------------------------------------------------------------
void CMccDataPathBase::SignalDataPathCompleteL( TInt aErrorCode )
	{
	__ASSERT_ALWAYS( iCompleteCallback, User::Leave( KErrArgument ) );
    iCompleteCallback->SignalDataPathComplete( aErrorCode );	
	}
        

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetSinkDataTypeCode
// From MDataSink
// -----------------------------------------------------------------------------
TInt CMccDataPathBase::SetSinkDataTypeCode( TFourCC aSinkFourCC, TMediaId aMediaId )
    {
    if( iMediaId.iMediaType == aMediaId.iMediaType )
        {
        iSinkFourCC = aSinkFourCC;
        if ( iAssociatedSink )
            {
            iAssociatedSink->SetSinkDataTypeCode( aSinkFourCC, aMediaId );
            }
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetSourceDataTypeCode
// From MDataSource
// -----------------------------------------------------------------------------
TInt CMccDataPathBase::SetSourceDataTypeCode( TFourCC aSourceFourCC, TMediaId aMediaId )
    {
    if( iMediaId.iMediaType == aMediaId.iMediaType )
        {
        iSourceFourCC = aSourceFourCC;
        if ( iAssociatedSource )
            {
            iAssociatedSource->SetSourceDataTypeCode( aSourceFourCC, aMediaId );
            }
        return KErrNone;
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetState
// Sets the current datapath state
// -----------------------------------------------------------------------------
void CMccDataPathBase::SetStateL( const TDataPathState& aState )
    {
    /*
    * States are EStopped, EPrimed, EStreaming
    * Following transitions are valid:
    * STOP -> EPrimed, EStopped
    * PRIMED -> EStreaming, EStopped
    * STREAMING -> EStopped, EPaused
    * PAUSED -> EStreaming, EStopped
    * ANY -> EStopped
    */
    
    switch( iState )
        {
        case EStopped:
            if( EPrimed == aState || EStopped == aState )
                {
                iState = aState;
                }
            else
                {
                User::Leave( KErrArgument );
                }
            break;
        case EPrimed:
            if( EStopped == aState || EStreaming == aState )
                {
                iState = aState;
                }
            else
                {
                User::Leave( KErrArgument );
                }
            break;
        case EStreaming:
            if( EStopped == aState || EPaused == aState )
                {
                iState = aState;
                }
            else
                {
                User::Leave( KErrArgument );
                }
            break;
        case EPaused:
            if( EStreaming == aState || EStopped == aState )
                {
                iState = aState;
                }
            else
                {
                User::Leave( KErrArgument );
                }
            break;
        default:
            User::Leave( KErrUnknown );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::EmptyBufferL
// From MDataSink
// -----------------------------------------------------------------------------
void CMccDataPathBase::EmptyBufferL( CMMFBuffer* /* aBuffer */, MDataSource* 
                                          /*aSupplier*/, TMediaId /*aMediaId*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SinkDataTypeCode
// From MDataSink
// -----------------------------------------------------------------------------
TFourCC CMccDataPathBase::SinkDataTypeCode( TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType ||
        KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iSinkFourCC;
        }
    else
        {
        TFourCC dummy( KFourCCNULL );
        return dummy;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SourceDataTypeCode
// From MDataSource
// -----------------------------------------------------------------------------
TFourCC CMccDataPathBase::SourceDataTypeCode( TMediaId aMediaId )
    {
    if( KUidMediaTypeAudio == aMediaId.iMediaType || 
        KUidMediaTypeVideo == aMediaId.iMediaType )
        {
        return iSourceFourCC;
        }
    else
        {
        TFourCC dummy( KFourCCNULL );
        return dummy;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::State
// Returns the current datapath state
// -----------------------------------------------------------------------------
TDataPathState CMccDataPathBase::State() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::TransferState
// Returns the current transferstate
// -----------------------------------------------------------------------------
TTransferState CMccDataPathBase::TransferState() const
    {
    return iTransferState;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::ControlSource
// -----------------------------------------------------------------------------  
TBool CMccDataPathBase::ControlSource( TUint32 aEndpointId ) const
    {
    return ( MCC_CONTROL_ALL_ENDPOINTS( aEndpointId ) || 
             aEndpointId == MCC_ENDPOINT_ID( iAssociatedSource ) );
    }
 
// -----------------------------------------------------------------------------
// CMccDataPathBase::ControlSink
// -----------------------------------------------------------------------------    
TBool CMccDataPathBase::ControlSink( TUint32 aEndpointId ) const
    {
    return ( MCC_CONTROL_ALL_ENDPOINTS( aEndpointId ) || 
             aEndpointId == MCC_ENDPOINT_ID( iAssociatedSink ) );
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::IsMmfEndpoint
// -----------------------------------------------------------------------------    
TBool CMccDataPathBase::IsMmfEndpoint( MDataSink* aSink ) const
    {
    return ( aSink && aSink->DataSinkType() == KUidMmfAudioOutput );
    }
    
// -----------------------------------------------------------------------------
// CMccDataPathBase::IsMmfEndpoint
// -----------------------------------------------------------------------------    
TBool CMccDataPathBase::IsMmfEndpoint( MDataSource* aSource ) const
    {
    return ( aSource && aSource->DataSourceType() == KUidMmfAudioInput );
    }
    
// -----------------------------------------------------------------------------
// CMccDataPathBase::DoCancel
// From CActive
// -----------------------------------------------------------------------------
void CMccDataPathBase::DoCancel()
    {
    // Nothing to do
    }
    
// -----------------------------------------------------------------------------
// CMccDataPathBase::EndOfData
// End of data
// -----------------------------------------------------------------------------
void CMccDataPathBase::EndOfData()
    {
    TRAPD( err, DoEndOfDataL() );

    if ( err )
        {
        __SUBCONTROLLER_INT1("CMccDataPathBase::EndOfData(), err=", err)
        if ( iEventHandler )
	        {
	        iEventHandler->SendEventToClient( 
                TMMFEvent( KMMFEventCategoryPlaybackComplete, err )  );
	        }
        else
	        {
	        __SUBCONTROLLER("CMccDataPathBase::EndOfData(),iEventHandler=NULL")	
	        }
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::RunL
// From CActive
// -----------------------------------------------------------------------------
void CMccDataPathBase::RunL()
    {
    switch ( State() )
        {
        case EStopped:
            break;
        case EPrimed:
            break;
        case EPaused:
            break;
        case EStreaming:
            switch ( TransferState() )
                {
                case EWaitSink:
                case EWaitSource:
                    break;
                case ENeedSourceData:
                    FillSourceBufferL();
                    break;
                case ESendDataToSink:
                    EmptySinkBufferL();
                    break;
                case EEndOfData:
                    EndOfData();
                    break;
                default:
                    User::Leave( KErrArgument );
                    break;
                }
            break;
        default:
            User::Leave( KErrArgument );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::RunError
// From CActive
// -----------------------------------------------------------------------------
TInt CMccDataPathBase::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
