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
* Description:    Encoder capable to form redundant RTP payload.
*
*/




#ifndef MCCREDENCODER_H
#define MCCREDENCODER_H

//  INCLUDES
#include    "mccredencdecbase.h"

/**
*  Redundancy RTP payload format encoder.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class CMccRedEncoder : public CMccRedEncDecBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccRedEncoder* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccRedEncoder();

    public: // New functions
        
        /**
        * Encode all encodings set to the RTP payload.
        * @since    Series 60 3.0
        * @param    aCurTimeStamp       Time stamp of the current RTP packet
        * @return   void
        */
        void EncodePayloadL( TUint aCurTimeStamp );
 
    private:    // Functions from base classes
        
        /**
        * From CMccRedEncDecBase. Initializes encoder.
        */
        void DoInitializeL( TInt aRedBlockCount, TInt aMaxPayloadSize,
            TInt aNumOfEncodings );

    private:

        /**
        * C++ default constructor.
        */
        CMccRedEncoder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Construct redundancy headers.
        * @since    Series 60 3.0
        * @param    aCurTimeStamp       Current time stamp
        * @return   None
        */
        void ConstructRedHeaders( TUint aCurTimeStamp );

    private:    // Data
         
         // Contains latest payloads constructed
         CMMFDataBuffer* iRedPayload;
         
         // Indicates whether redundant packets to send exist
         TBool iRedPackets;
         
         // Contains redundancy headers being encoded
         HBufC8* iRedHeaders;
         
         // Contains time stamps of the latest payloads constructed
         RArray<TUint> iRedTimeStamps;

    };

#endif      // MCCREDENCODER_H   
            
// End of File
