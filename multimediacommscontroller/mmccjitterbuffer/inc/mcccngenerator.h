/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/




#ifndef MCCCNGENERATOR_H
#define MCCCNGENERATOR_H

#include <e32base.h>
#include <mmf/common/mmfutilities.h>

// FORWARD DECLARATIONS
class CMMFDevSound;
class CErrorConcealmentIntfc;

/**
* MCC Comfort noise generator
*
* @lib MMccJitterBuffer.lib
* @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccCnGenerator ) : public CBase
    {
    public:  // Constructors and destructor 
        
        /**
        * Two phased constructor
        */
        static CMccCnGenerator* NewL( const TFourCC& aAudioType, 
                CMMFDevSound& aDevSound );

        /**
        * Destructor
        */
        virtual ~CMccCnGenerator();

    public: // New functions

        /**
        * Generates an SID packet to given buffer
        * @since Series 60 3.0
        * @param aPayload Buffer to be filled with SID data
        * @param aRequestSize Amount of data to be generated to buffer.
        * @return void
        */
        void GenerateSidPacket( TDes8& aPayload, TInt aRequestSize );
        
        /**
        * Jitterbuffer has noted that frames were missing, so error concealment
        * needs to be done to some frames. This function calls the error 
        * concealment interface's error concealment function.
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void ConcealErrorForNextFrame() const;
        
        /**
        * Returns the current DTX status
        * @since Series 60 3.0
        * @param None
        * @return TBool ETrue if DTX period ongoing
        */
        inline TBool DtxPeriodStatus() const
            {
            return ( iDtxPeriodStarted );
            }
        
        /**
        * Decide whether we are in DTX period or in normal speech period
        * @since Series 60 3.0
        * @param aData Buffer containing the data
        * @return void
        */
        void DoDtxDecision( const TDesC8& aData );
        
        /**
        * Check if the given databuffer is a SID frame for current codec
        * @since Series 60 3.0
        * @param aData Buffer containing the data
        * @return TBool ETrue if the aBuffer is a SID for current codec
        */
        TBool IsSidBuffer( const TDesC8& aData ) const;

    private: // New functions

        /**
        * Generates an AMR CN packet into aPayload
        * @since Series 60 3.0
        * @param aPayload
        * @return void
        */
        void GenerateAmrNoDataPacket( TDes8& aPayload ) const;
        
        /**
        * Generates an VoIP NO_DATA frame into aPayload
        * @since Series 60 3.0
        * @param aPayload Buffer to be filled with VoIP NO_DATA information
        * @param aRequestSize Amount of data to be generated to buffer.
        * @return void
        */
        void GenerateVoIPNoDataPacket( TDes8& aPayload, TInt aRequestSize ) const;

    private: //Constructors

        /**
        * C++ default constructor.
        */
        CMccCnGenerator( const TFourCC& aAudioType, CMMFDevSound& aDevSound );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
           
    private:    // Data
                
        // Audio type
        TFourCC iAudioType;
        
        // Sound device for Error Concealment IF creation
        CMMFDevSound& iDevSound;
        
        // Error concealment interface
        CErrorConcealmentIntfc* iErrorConcealer;
        
        // Generated CN frames
        TInt iGeneratedCnFrames;
        
        // DTX period started
        TBool iDtxPeriodStarted;
      
    private:    // Friend classes
    
    // Friend class for EUNIT tests
    #ifdef TEST_EUNIT
        friend class UT_CMccCnGenerator;
    #endif
    };

#endif  // MCCCNGENERATOR_H
