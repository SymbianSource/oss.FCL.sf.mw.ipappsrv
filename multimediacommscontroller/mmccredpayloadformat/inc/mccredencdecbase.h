/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Base functionality common for redundancy RTP payload encoder 
*                and decoder.
*
*/



#ifndef MCCREDENCDECBASE_H
#define MCCREDENCDECBASE_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt  KMaxNumOfEncodings  =   3;
const TInt  KPayloadNotDefined  =   128;

// DATA TYPES
enum TMccRedEncoding
    {
    EMccPrimaryEncoding     =   0,
    EMccSecondaryEncoding,
    EMccTertiaryEncoding,
    EMccRTPPayload
    };

// FORWARD DECLARATIONS
class CMMFDataBuffer;

// CLASS DECLARATION

/**
*  Base class for redundancy RTP payload format encoder/decoder.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class CMccRedEncDecBase : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CMccRedEncDecBase();

    public: // New functions
        
        /**
        * Set encoding, which is wanted to encode/decode.
        * @since    Series 60 3.0
        * @param    aEncoding       Identifies, which kind of encoding is set
        * @param    aEncodingBuf    Buffer holding encoding
        * @return   void
        */
        virtual void SetEncodingBlockL( TMccRedEncoding aEncoding,
                                        TDesC8& aEncodingBuf );

        /**
        * Get encoding specified with parameter.
        * @since    Series 60 3.0
        * @param    aEncoding       Identifies, which kind of encoding is get
        * @param    aToBuffer       Buffer where to place data
        * @return   void
        */                                     
        virtual void GetEncodingBlockL( TMccRedEncoding aEncoding, 
                                        TDes8& aToBuffer );
        
        /**
        * Initialize after enc/dec parameters are changed or stream is
        * started again after pause.
        *
        * @since    Series 60 3.0
        * @param    aRedCount           Number of redundant blocks to handle
        * @param    aMaxPayloadSize     Maximum size of the pure RTP payload
        * @param    aNumOfEncodings     Number of encodings to handle
        * @return   void
        */
        virtual void InitializeL( TInt aRedCount, TInt aMaxPayloadSize,
            TInt aNumOfEncodings );
                      
        /**
        * Set payload types to wait for.
        * @since    Series 60 3.0
        * @param    aPayloadTypes       Payload types
        * @return   System wide error code
        */
        virtual TInt SetPayloadTypes( RArray<TUint>& aPayloadTypes );

    protected:  // New functions
        
        /**
        * C++ default constructor.
        */
        CMccRedEncDecBase();

    private:
        
        virtual void DoInitializeL( TInt aRedCount, TInt aMaxPayloadSize,
            TInt aNumOfEncodings ) = 0;

    protected:  // Data
    
        // Number of redundant data blocks included in one RTP payload
        TInt iRedBlockCount;
        
        // Maximum size of the RTP payload in bytes to handle
        TInt iMaxPayloadSize;
        
        // Buffer where to handle whole RTP payload including redundancy
        CMMFDataBuffer* iRTPPayload;

        // Payload types for all encodings
        TFixedArray< TUint8, KMaxNumOfEncodings > iPayloadTypes;
        
        // Encodings, which are included in RTP payload
        TFixedArray< CMMFDataBuffer*, KMaxNumOfEncodings > iEncodings;

    };

#endif      // MCCREDENCDECBASE_H   
            
// End of File
