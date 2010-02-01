/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a uplink stream for symmetric subthread
*
*/



#ifndef MCCSYMULSTREAM_H
#define MCCSYMULSTREAM_H

//  INCLUDES
#include "mccsymstreambase.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CMMFFormatEncode;
class MMccResources;

// CLASS DECLARATION

/**
*  Mcc Symmetric subthread uplink stream
*
*  @lib Mccsubthreads.lib
*  @since Series 60 3.0
*/
#ifdef EKA2
NONSHARABLE_CLASS( CMccSymUlStream ) : public CMccSymStreamBase
#else
class CMccSymUlStream : public CMccSymStreamBase
#endif
    {
    public:  // Constructors and destructor
        
        /**
        * Static constructor
        */
        static CMccSymUlStream* NewLC( TUint32 aMccStreamId, 
                                       MAsyncEventHandler* aEventhandler, 
                                       MMccResources* aMccResources,
                                       CMccRtpManager* aManager, 
                                       TFourCC aFourCC,
                                       TInt aStreamType,
                                       CMccRtpMediaClock& aClock );
        
        /**
        * Destructor.
        */
        virtual ~CMccSymUlStream();

    public: // New functions

    public: // Functions from base classes

        /**
        * From CMccSymStreamBase
        */
        void SetPrioritySettingsL( const TMMFPrioritySettings& aPriority );
        
        /**
        * From CMccSymStreamBase
        */
        void AddSinkAndSourceL( MDataSink* aDatasink, 
                MDataSource* aDatasource );
                
        /**
        * From CMccSymStreamBase
        */      
        void LoadCodecL( const TMccCodecInfo& aCodecInfo,
                         const TDesC8& aFmtp );
        
        void UpdateCodecInformationL( TMccCodecInfo& aCodecInfo );
        /**
        * From CMccSymStreamBase
        */
        void PrimeL( const TUint32 aEndpointId );
        
        /**
        * From CMccSymStreamBase
        */
        void PlayL( const TUint32 aEndpointId,
        			TBool aStreamPaused, 
                    TBool aEnableRtcp );
        
        /**
        * From CMccSymStreamBase
        */
        void PauseL( const TUint32 aEndpointId,
        			 TBool aEnableRtcp );
        
        /**
        * From CMccSymStreamBase
        */
        void ResumeL( const TUint32 aEndpointId,
        			  TBool aEnableRtcp );
        
        /**
        * From CMccSymStreamBase
        */
        void StopL( const TUint32 aEndpointId );

        /**
        * From CMccSymStreamBase
        */
        void SetBalanceL( TInt aLeftbal, TInt aRightbal );
        
        /**
        * From CMccSymStreamBase
        */
        void GetBalanceL( TInt& aLeftbal, TInt& aRightbal );
        
        /**
        * From CMccSymStreamBase
        */
        void SendMediaSignalL( const TMccEvent& aEvent );
        
        /**
        * From CMccSymStreamBase
        */
        void ResetCountersL();

        /**
        * Sends RTCP sender report.
        * @param none
        * @return One of the standard system-wide error codes.
        */
        void SendRTCPSenderReportL();

        /**
        * Sends Non-RTCP data to uplink if possible.
        * @param aData - [input] Data to send
        * @return One of the standard system-wide error codes.
        */
        void SendRTCPDataL( const TDesC8& aData );
        
        void GetSSRCL( TUint32& aSSRCValue );
        
        TBool IsUplink();
        
        void StandByL();
        
        /**
         * Bind context into stream..
         * @since Series 60 3.2
         * @param aEndpointId The endpoint id
         * @param aContextParams Crypto context
         * @return void
         */	    
        void BindContextIntoStreamL( TUint32 aEndpointId,
                                     const TMccCryptoContext& aContextParams );

    private:
        
        /**
        * C++ default constructor.
        */
        CMccSymUlStream( TUint32 aMccStreamId, 
                         MAsyncEventHandler* aEventhandler,
                         MMccResources* aMccResources,
                         CMccRtpManager* aManager, 
                         TFourCC aFourCC,
                         TInt aStreamType,
                         CMccRtpMediaClock& aClock );
        
        /**
        * 2nd phase constructor
        */
        void ConstructL();

    protected: // Functions from base classes
    
        MDataSink* MultiplexerSink();

        /**
         * From CMccSymStreamBase
         * @since Series 60 3.0
         * @param Desired bitrate
         */
        void DoChangeBitrateL( TInt aBitrate );

    protected:  // New functions

    private:    // New functions
    
        /**
        * Creates the payload format encoder
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void CreatePayloadFormatEncoderL();

    public:     // Data

    protected:  // Data

    private:    // Data
    
        // Payload format encoder
        CMMFFormatEncode* iFormatEncode;
        
        // Payload format encoder for redundancy format
        CMMFFormatEncode* iRedFormatEncode;
        
        TFourCC	iFourCC;

    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

        #ifdef TEST_EUNIT
        
        friend class UT_CMccSymUlStream;
        
        #endif

    };

#endif      // MCCSYMULSTREAM_H

// End of File
