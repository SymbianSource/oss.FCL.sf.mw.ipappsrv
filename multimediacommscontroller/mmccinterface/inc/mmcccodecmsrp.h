/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class is used for configuring G729 codec.
*
*/





#ifndef MMCCCODECMSRP_H
#define MMCCCODECMSRP_H

//  INCLUDES
#include "mmcccodecinformation.h"

// CONSTANTS
const TUint8 KPayloadType = 999;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Container class for the codec information.
*
*/
NONSHARABLE_CLASS( CMccCodecMsrp ) : public CMccCodecInformation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccCodecMsrp* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccCodecMsrp();

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
        
    private:

        /**
        * C++ default constructor.
        */
        CMccCodecMsrp();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    };

#endif         
            
// End of File
