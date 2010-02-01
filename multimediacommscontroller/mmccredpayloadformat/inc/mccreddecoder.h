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
* Description:    Decoder capable to handle redundancy format RTP payload.
*
*/




#ifndef MCCREDDECODER_H
#define MCCREDDECODER_H

//  INCLUDES
#include    "mccredencdecbase.h"

/**
*  Redundancy RTP payload format decoder.
*
*  @lib mmccredplformat.dll
*  @since Series 60 3.0
*/
class CMccRedDecoder : public CMccRedEncDecBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccRedDecoder* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CMccRedDecoder();

    public: // New functions
        
        /**
        * Decode encoding(s) from RTP payload given.
        * @since    Series 60 3.0
        * @param    aCurTimeStamp   Timestamp of the RTP packet under decoding
        * @return   Number of data blocks decoded or system wide error code.
        */
        TInt DecodePayload( TUint aCurTimeStamp );

    private:    // Functions from base classes
        
        /**
        * From CMccRedEncDecBase. Initializes decoder.
        */
        void DoInitializeL( TInt aRedBlockCount, TInt aMaxPayloadSize,
            TInt aNumOfEncodings );
            
    private:

        class TRedHeaderInfo
            {
            public:
                TUint8 iBlockPT;
                TUint iTimeStamp;
                TUint iBlockLength;
                TUint8* iBlockIndex;
            };
            
        /**
        * C++ default constructor.
        */
        CMccRedDecoder();

        /**
        * Get info about audio data blocks in a RTP payload buffer
        */
        TInt GetBlockInfos( RArray<TRedHeaderInfo>& aBlockInfos,
                            const TDesC8& aRTPPayload );

    private:    // Data
         
         // Time stamp from the RTP header
         TUint iCurTimeStamp;
         
         // Contains primary data blocks timestamp from previous RTP payload
         TInt iLatestTimeStamp;
         
         // Used to carry block indexes, block lengths etc.
         RArray<TRedHeaderInfo> iRedHeaderInfo;

    };

#endif      // MCCREDDECODER_H   
            
// End of File
