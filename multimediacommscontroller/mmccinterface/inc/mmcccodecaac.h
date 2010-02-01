/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef MMCCCODECAAC_H
#define MMCCCODECAAC_H

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*/
NONSHARABLE_CLASS( CMccCodecAAC ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccCodecAAC* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecAAC();

    public: // New functions

    public: // Functions from base classes

    public: // From CMccCodecInformation
    
        TBool RequireSignalling( const CMccCodecInformation& aCandidate ) const; 
        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDefaultsL();
        
        /**
        * From CMccCodecInformation
        */
        CMccCodecInformation* CloneDetailedL();
        
       
        TInt EnableVAD( TBool aEnableVAD );
     
    

       
        TInt SetBitrate( TUint aBitrate );
        
        
        TInt SetSamplingFreq( TUint32 aSamplingFreq );

        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since    Series 60 3.0
        * @param    aPacketrate        New packetrate
        * @return   KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
        */
        TInt SetPTime( TUint aPTime );
        
        /**
        * From CMccCodecInformation. Sets the packet rate.
        * @since    Series 60 3.0
        * @param    aPacketrate        New packetrate
        * @return   KErrNotSupported if codec doesn't support packetrate value issued; otherwise KErrNone.
        */
        TInt SetMaxPTime( TUint aMaxPTime );

        /**
        * From CMccCodecInformation. Sets the sdp name.
        * @since    Series 60 3.0
        * @param aRtpId - [input] RTP session ID
        * @return One of the standard system-wide error codes.
        */
        TInt SetSdpName( const TDesC8& aSdpName );
        
        /**
        * From CMccCodecInformation. Sets the payload type.
        * @since    Series 60 3.0
        * @param    aRtpId - [input] RTP session ID
        * @return   One of the standard system-wide error codes.
        */
        TInt SetPayloadType( TUint8 aPayloadType );

        /**
        * From CMccCodecInformation. Sets the codec mode.
        * @since    Series 60 3.0
        * @param    aMode    Codec mode ( e.g. ULAW/ALAW, Bandwidth efficient / octet aligned )
        * @return   KErrNotSupported if codec doesnt' support codec mode value issued; otherwise KErrNone.
        */
        TInt SetCodecMode( TCodecMode aCodecMode );
        
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CMccCodecAAC();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public:     // Data
    
    protected:  // Data

    private:    // Data

    public:     // Friend classes

    protected:  // Friend classes
    
    private:    // Friend classes

    #ifdef TEST_EUNIT
        friend class UT_CMccCodecAAC;
    #endif
    };

#endif      // MMCCCODECAAC_H   
            
// End of File
