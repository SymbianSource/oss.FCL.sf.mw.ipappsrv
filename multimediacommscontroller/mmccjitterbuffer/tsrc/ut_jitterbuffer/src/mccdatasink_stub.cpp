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
* Description:    MCC data sink stub for unit tests
*
*/




// INCLUDE FILES
#include "mccdatasink_stub.h"


// -----------------------------------------------------------------------------
// CMccDataSinkStub::CMccDataSinkStub
// -----------------------------------------------------------------------------
//
CMccDataSinkStub::CMccDataSinkStub()
    {
    }
 
 // -----------------------------------------------------------------------------
// CMccDataSinkStub::~CMccDataSinkStub
// -----------------------------------------------------------------------------
//
CMccDataSinkStub::~CMccDataSinkStub()
    {
    delete iBuffer;
    delete iDevSound;
    }
    
// -----------------------------------------------------------------------------
// CMccDataSinkStub::ConstructSinkL
// -----------------------------------------------------------------------------
//
void CMccDataSinkStub::ConstructSinkL( const TDesC8& /*aInitData*/ )
    {
    iDevSound = CMMFDevSound::NewL();
    }    

// -----------------------------------------------------------------------------
// CMccDataSinkStub::SinkDataTypeCode
// -----------------------------------------------------------------------------
//
TFourCC CMccDataSinkStub::SinkDataTypeCode( TMediaId /*aMediaId*/ )
    {
    return TFourCC();
    }   
    
// -----------------------------------------------------------------------------
// CMccDataSinkStub::FillBufferL
// -----------------------------------------------------------------------------
//
void CMccDataSinkStub::EmptyBufferL( 
    CMMFBuffer* /*aBuffer*/, 
    MDataSource* /*aSupplier*/, 
    TMediaId /*aMediaId*/ )
    {    
    ReserveSomeMemoryForOomSimulationL();
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccDataSinkStub::BufferFilledL( CMMFBuffer* /*aBuffer*/ )
    {
    ReserveSomeMemoryForOomSimulationL();
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::CanCreateSinkBuffer
// -----------------------------------------------------------------------------
//
TBool CMccDataSinkStub::CanCreateSinkBuffer()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::CreateSinkBufferL
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccDataSinkStub::CreateSinkBufferL( 
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
// CMccDataSinkStub::SetSinkDataTypeCode
// -----------------------------------------------------------------------------
//
TInt CMccDataSinkStub::SetSinkDataTypeCode(
    TFourCC /*aSinkFourCC*/,
    TMediaId /*aMediaId*/)
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::SoundDevice
// -----------------------------------------------------------------------------
//
CMMFDevSound& CMccDataSinkStub::SoundDevice()
    {
    return *iDevSound;
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::BytesPlayed
// -----------------------------------------------------------------------------
//
TInt CMccDataSinkStub::BytesPlayed()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::HWEmptyBufferL
// -----------------------------------------------------------------------------
//
void CMccDataSinkStub::HWEmptyBufferL(
    CMMFBuffer* /*aBuffer*/, 
    MDataSource* /*aSupplier*/)
    {
    ReserveSomeMemoryForOomSimulationL();
    }

// -----------------------------------------------------------------------------
// CMccDataSinkStub::ReserveSomeMemoryForOomSimulationL
// -----------------------------------------------------------------------------
// 
void CMccDataSinkStub::ReserveSomeMemoryForOomSimulationL()
    {
    delete HBufC::NewL( 100 );
    }

//  End of File  
