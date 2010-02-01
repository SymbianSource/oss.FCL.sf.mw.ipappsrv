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
* Description:    Formatter classes for decoding of RTP payload format
*                for AMR ( wide band ) audio codecs.
*
*/




#ifndef AMRPAYLOADDECODEROA_H
#define AMRPAYLOADDECODEROA_H

// INCLUDES
#include <e32std.h>
#include "amrpayloaddecoder.h"
#include "amrpayloadheaderoa.h"
#include "amrtocentryoa.h"
#include "amrframe.h"


/**
* The Amr payload decoder for octet aligned ( OA ) mode.
*
* @lib Mccamrplformat.dll
* @since Series 60 3.1
*/
class CAmrPayloadDecoderOA : public CAmrPayloadDecoder
    {
    
public:  // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CAmrPayloadDecoderOA* NewL( TBool aIsNb );

    /**
    * Destructor.
    */
    ~CAmrPayloadDecoderOA();

public: // Functions

    /**
    * Decode payload data received in the payload buffer. If AMR FEC is 
    * used and redundant frames are discarded, time stamp is increased to
    * correspond start of remaining frames.
    * @since    Series 60 3.1
    * @param    aTimeStamp Time stamp from RTP header
    * @param    aTimeStampInc Unit for time stamp incrementing
    * @return   TInt - Number of AMR frames decoded
    */
    virtual TInt DecodePayload( TUint32& aTimeStamp, TUint32 aTimeStampInc );

private:

    /**
    * Constructor.
    */
    CAmrPayloadDecoderOA( TBool aIsNb );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

protected:
   
    /**
    * Calculate frames' starting Byte and Bit positions in a given payload
    * buffer and update `iFrames' array with the frames' positions.
    * @since    Series 60 3.1
    * @param    aBuffer - [input] Payload buffer.
    * @param    aFrameCount - [input] Number of frames included in the
    *           payload buffer.
    */
    virtual void CalculateFramePosition( const TUint8* aBuffer,
        TInt aFrameCount );
               
              
    };

#endif  // AMRPAYLOADDECODEROA_H