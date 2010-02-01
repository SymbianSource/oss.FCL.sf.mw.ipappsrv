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
* Description:    Formatter class for decoding of RTP payload format
*                for AMR ( wide band ) audio codecs.
*
*/




#ifndef AMRPAYLOADENCODEROA_H
#define AMRPAYLOADENCODEROA_H

// INCLUDES
#include <e32std.h>
#include "amrpayloadencoder.h" 
#include "amrframe.h"

// CLASS DECLARATION

/**
* The Amr payload encoder for octet aligned ( OA ) mode.
*
* @lib      Mccamrplformat.dll
* @since    Series 60 3.1
*/
class CAmrPayloadEncoderOA : public CAmrPayloadEncoder
    {
    
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CAmrPayloadEncoderOA* NewL( TBool aIsNb );

    /**
    * Destructor.
    */
    ~CAmrPayloadEncoderOA();

public: // Functions

    /**
    * Encode a received Amr frame into the payload buffer.
    * @since    Series 60 3.1
    * @param    aChannel - [input] Channel number that the frame belongs 
    *                        to
    * @param    aFrameType - [input] Frame type
    * @param    aFrameQualityInd - [input] Frame quality indicator bit; 
    *           1 if frame OK; 0 if frame damaged
    * @param    aFrameData - [input] Frame data; its length depends on
    *                         frame type
    * @return   ETrue if payload formatting is completed; otherwise EFalse
    */
    virtual TBool EncodeFrame( TInt aChannel, TAmrFrameType aFrameType,
        TUint8 aFrameQualityInd, const TDes8& aFrameData );

public: // Functions from CAmrPayloadEncoder
        
    /**
    * Set number of audio channels.
    * @since    Series 60 3.1
    * @param    aChannelCount - [input] Number of audio channels
    * @return   KErrNone if successful; system wide error code otherwise
    */
    virtual TInt SetChannelCount( TInt aChannelCount );
        
    /**
    * Set number of Amr frame blocks included in one RTP packet.
    * Each frame block contains frames for all audio channels.
    * @since    Series 60 3.1
    * @param    aFrameblockCount - [input] Number of Amr frame blocks
    *           included in one RTP packet
    * @return   KErrNone if successful; system wide error code otherwise
    */
    virtual TInt SetFrameBlockCount( TInt aFrameblockCount );

protected:

    /**
    * Constructor.
    */
    CAmrPayloadEncoderOA( TBool aIsNb );
    
private:

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

    /**
    * Initialize frame encoder ( CStreamEncoder ).
    * Information to initialize are obtained from iPayload, calculation 
    * using iChannelCount and iFrameBlockCount. Initialization should be 
    * done whenever a payload is finished.
    * @since    Series 60 3.1
    */
    virtual void InitializeFrameEncoder();
        
    };



#endif  // AMRPAYLOADENCODEROA_H
