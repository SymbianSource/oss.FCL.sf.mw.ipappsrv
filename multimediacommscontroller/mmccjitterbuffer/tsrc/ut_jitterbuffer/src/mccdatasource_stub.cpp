/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    MCC data source stub for unit tests
*
*/




// INCLUDE FILES
#include "mccdatasource_stub.h"


// -----------------------------------------------------------------------------
// CMccDataSourceStub::CMccDataSourceStub
// -----------------------------------------------------------------------------
//
CMccDataSourceStub::CMccDataSourceStub( TUid aType ) : MDataSource( aType )
    {
    }
 
 // -----------------------------------------------------------------------------
// CMccDataSourceStub::~CMccDataSourceStub
// -----------------------------------------------------------------------------
//
CMccDataSourceStub::~CMccDataSourceStub()
    {
    delete iBuffer;
    }
    
// -----------------------------------------------------------------------------
// CMccDataSourceStub::ConstructSourceL.
// -----------------------------------------------------------------------------
//
void CMccDataSourceStub::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {
    ReserveSomeMemoryForOomSimulationL();
    }    

// -----------------------------------------------------------------------------
// CMccDataSourceStub::SourceDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccDataSourceStub::SourceDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC();
    }   
    
// -----------------------------------------------------------------------------
// CMccDataSourceStub::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccDataSourceStub::FillBufferL( 
    CMMFBuffer* /*aBuffer*/, 
    MDataSink* /*aConsumer*/, 
    TMediaId /*aMediaId*/ )
    {    
    ReserveSomeMemoryForOomSimulationL();
    }

// -----------------------------------------------------------------------------
// CMccDataSourceStub::BufferEmptiedL
// -----------------------------------------------------------------------------
//
void CMccDataSourceStub::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
    {
    ReserveSomeMemoryForOomSimulationL();
    }

// -----------------------------------------------------------------------------
// CMccDataSourceStub::CanCreateSourceBuffer
// -----------------------------------------------------------------------------
//
TBool CMccDataSourceStub::CanCreateSourceBuffer()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccDataSourceStub::CreateSourceBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccDataSourceStub::CreateSourceBufferL( 
    TMediaId /*aMediaId*/,
    TBool& aReference )
    {
    CMMFBuffer* tmp = CMMFDataBuffer::NewL( 100 );
    delete iBuffer;
    iBuffer = tmp;
    aReference = ETrue;
    return iBuffer;
    }    
     
// -----------------------------------------------------------------------------
// CMccDataSourceStub::SetSourceDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccDataSourceStub::SetSourceDataTypeCode(
    TFourCC /*aSourceFourCC*/,
    TMediaId /*aMediaId*/)
    {
    return KErrNone;
    }     
     
// -----------------------------------------------------------------------------
// CMccDataSourceStub::ReserveSomeMemoryForOomSimulationL
// -----------------------------------------------------------------------------
// 
void CMccDataSourceStub::ReserveSomeMemoryForOomSimulationL()
    {
    delete HBufC::NewL( 100 );
    }

//  End of File  
