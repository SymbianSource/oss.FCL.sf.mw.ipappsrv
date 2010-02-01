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



#ifndef MCCDATASINKSTUB_H
#define MCCDATASINKSTUB_H

//  INCLUDES
#include <mmf/server/mmfaudiooutput.h>

// CLASS DECLARATION
/**
*  Unit test stub for MDataSink
*/
class CMccDataSinkStub : public CBase, public MMMFAudioOutput
    {
    public:  // Constructors and destructor

        CMccDataSinkStub();
        ~CMccDataSinkStub();         

    public: // From MDataSink

        TFourCC SinkDataTypeCode(TMediaId aMediaId);

        void EmptyBufferL(
            CMMFBuffer* aBuffer, 
            MDataSource* aSupplier, 
            TMediaId aMediaId);

        void BufferFilledL(CMMFBuffer* aBuffer);

        TBool CanCreateSinkBuffer();

        CMMFBuffer* CreateSinkBufferL(
            TMediaId aMediaId, 
            TBool &aReference);

        void ConstructSinkL( const TDesC8& aInitData );

        TInt SetSinkDataTypeCode(TFourCC aSinkFourCC, TMediaId aMediaId);

    public: // From MMMFAudioOutput
    
    	CMMFDevSound& SoundDevice();

        TInt BytesPlayed();
        
        void HWEmptyBufferL(CMMFBuffer* aBuffer, MDataSource* aSupplier);
	
    private: // New functions
    
        void ReserveSomeMemoryForOomSimulationL();
        
    private: // Data
        
        CMMFDevSound* iDevSound;
        CMMFBuffer* iBuffer;	
    };

#endif // MCCDATASINKSTUB_H

// End of File 
