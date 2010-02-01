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
* Description: 
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
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::AddDataSinkL
// Adds a datasink to the datapath
// -----------------------------------------------------------------------------
void CMccDataPathBase::AddDataSinkL( MDataSink* aSink )
    {
	__SUBCONTROLLER( "CMccDataPathBase::AddDataSinkL" )
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

	}

// -----------------------------------------------------------------------------
// CMccDataPathBase::CreateCompleteCallBackL
// a CCompleteCallback object is created
// -----------------------------------------------------------------------------
void CMccDataPathBase::CreateCompleteCallBackL()
	{
	}

// -----------------------------------------------------------------------------
// CMccDataPathBase::SignalDataPathCompleteL
// a signal the datapath complete with a error code
// -----------------------------------------------------------------------------
void CMccDataPathBase::SignalDataPathCompleteL( TInt aErrorCode )
	{
	}
        

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetSinkDataTypeCode
// From MDataSink
// -----------------------------------------------------------------------------
TInt CMccDataPathBase::SetSinkDataTypeCode( TFourCC aSinkFourCC, TMediaId aMediaId )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetSourceDataTypeCode
// From MDataSource
// -----------------------------------------------------------------------------
TInt CMccDataPathBase::SetSourceDataTypeCode( TFourCC aSourceFourCC, TMediaId aMediaId )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SetState
// Sets the current datapath state
// -----------------------------------------------------------------------------
void CMccDataPathBase::SetStateL( const TDataPathState& aState )
    {

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
        TFourCC dummy( KFourCCNULL );
        return dummy;
    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::SourceDataTypeCode
// From MDataSource
// -----------------------------------------------------------------------------
TFourCC CMccDataPathBase::SourceDataTypeCode( TMediaId aMediaId )
    {
        TFourCC dummy( KFourCCNULL );
        return dummy;
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

    }

// -----------------------------------------------------------------------------
// CMccDataPathBase::RunL
// From CActive
// -----------------------------------------------------------------------------
void CMccDataPathBase::RunL()
    {

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
