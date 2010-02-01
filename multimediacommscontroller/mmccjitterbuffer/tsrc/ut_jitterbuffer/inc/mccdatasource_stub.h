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



#ifndef MCCDATASOURCESTUB_H
#define MCCDATASOURCESTUB_H

//  INCLUDES
#include <mmf/server/mmfdatasource.h>

// CLASS DECLARATION
/**
*  Unit test stub for MDataSource
*/
class CMccDataSourceStub : public CBase, public MDataSource
    {
    public:  // Constructors and destructor

        CMccDataSourceStub( TUid aType );
        ~CMccDataSourceStub();         

    public: // From MDataSource

        TFourCC SourceDataTypeCode(TMediaId aMediaId);

        void FillBufferL(
            CMMFBuffer* aBuffer,
            MDataSink* aConsumer,
            TMediaId aMediaId);

        void BufferEmptiedL(CMMFBuffer* aBuffer);

        TBool CanCreateSourceBuffer();

        CMMFBuffer* CreateSourceBufferL(
            TMediaId aMediaId,
            TBool &aReference);

        void ConstructSourceL(const TDesC8& aInitData);
    
        TInt SetSourceDataTypeCode(TFourCC aSourceFourCC, TMediaId aMediaId);
    
    private: // New functions
    
        void ReserveSomeMemoryForOomSimulationL();
        
    private: // Data
        
        CMMFBuffer* iBuffer;
    };

#endif // MCCDATASOURCESTUB_H   
            
// End of File
