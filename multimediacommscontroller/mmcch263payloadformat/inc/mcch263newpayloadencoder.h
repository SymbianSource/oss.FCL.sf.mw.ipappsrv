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



#ifndef MCCH263NEWPAYLOADENCODER_H
#define MCCH263NEWPAYLOADENCODER_H

// ============================ INCLUDES =======================================

#include <e32base.h>
#include <e32std.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "mccrtpdatasink.h"
#include "mcch263payloadencoder.h"

// ============================ CLASS DECLARATIONS =============================

class CMccH263NewPayloadEncoder : public CMccH263PayloadEncoder
{
	public:
    
	    /**
	    * First stage consturctor
	    */
	    static CMccH263NewPayloadEncoder* NewL();
	    
	    /**
	    * Write a buffer to sink
	    * @since SWIS Lite
	    * @param CCMRMediaBuffer* aBuffer The buffer to write
	    * @param aDesBuffers, buffers array contains encoded data
	    * @return void
	    */
	    void EncodeL( CMMFBuffer* aBuffer, 
	                  RPointerArray<CBufferContainer>& aDesBuffers );
	    
	    /**
	    */
	    void SetPayloadType( TUint8 aPayloadType );

	    /**
	    */
	    virtual ~CMccH263NewPayloadEncoder();
	    
	private:
	    
	    /**
	    * Write header byte to RTP packet as in RFC 2190
	    * @since SWIS Lite
	    * @param TInt byte
	    * @return TUint8 header byte
	    */
	    TUint8 WriteHeaderData( TInt Byte );
	    
	    void CreateNewBufferL( 
                    	CMMFBuffer* aMMFBuffer,  
                    	RPointerArray<CBufferContainer>& aDesBuffers,
                    	const TDesC8& aPacketData,
                    	TInt aLength,
                    	TInt aMarker,
                    	TInt aHeaderDataByteOne,
                    	TInt aHeaderDataByteTwo );
	
	private:
	
		TRtpSendHeader iSendHeader;//RTP send header
		TTimeIntervalMicroSeconds iTimeStamp;
		TUint16 iSequenceNumIncrementer;
	    
	private:
	
		#ifdef EUNIT_TEST
        	friend class UT_CMccH263NewPayloadEncoder;
   		#endif
		
};


#endif
