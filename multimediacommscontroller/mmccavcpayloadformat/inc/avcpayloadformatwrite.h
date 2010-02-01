/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    The payloadization part of the AVC payloadization/depayloadization plugin
*
*/




#ifndef AVCPAYLOADFORMATWRITE_H
#define AVCPAYLOADFORMATWRITE_H

// include files
#include<e32base.h>
#include<e32std.h>
#include<mmf/server/mmfdatabuffer.h>
#include<mmf/common/mmfutilities.h>

#include "rtpheader.h"
#include "mccinternalcodecs.h"
#include "formatstatemachine.h"
#include "rfc3984encode.h"
#include "avcrtpstruct.h"

// Forward declarations

class CRFC3984Encode;
class TRtpRecvHeader;




// CAvcPayloadFormatWrite class declaration. 
// The class receives one avc frame and payloadizes it into
// one ore more RTP packets and gives the packets to the RTP
// layer

class CAvcPayloadFormatWrite : public CPayloadFormatWrite, public MPayloadFormatWrite
    {
	public: 
	
		static CAvcPayloadFormatWrite * NewL( MDataSink * aSink);
		
		virtual ~CAvcPayloadFormatWrite();
		
		// from MPayloadFormatWrite
		void EmptySourceBufferL( );
		void SourceBufferEmptiedL( );
		virtual void SetPayloadType( TUint8 aPayloadType);
		void CancelUlRequest();
		
		// from CMMFFormatEncode (currently for audio, so none of these
		// is implemented)
		
		virtual  TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;
		virtual  TTimeIntervalMicroSeconds Duration( TMediaId /*aMediaType*/ ) const;
	
		// from MDataSink (one of the above public class is derived from it)
		virtual CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, 
                                               TBool &aReference ); 
        virtual TFourCC SinkDataTypeCode( TMediaId aMediaId );
        virtual TInt SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                              TMediaId aMediaId ); 
        virtual TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );
        virtual void SinkThreadLogoff( );
        virtual void EmptyBufferL( CMMFBuffer* aBuffer, 
                                       MDataSource* aSupplier,
                                       TMediaId aMediaId );
        virtual void BufferEmptiedL( CMMFBuffer* aBuffer );
        virtual void SinkPrimeL();
        virtual void SinkPlayL( );
        virtual void SinkPauseL( );
        virtual void SinkStopL( );
    
		/**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        * @param    aConfigParams       Configuration parameters
        * @param    aClock
        */
        void ConfigurePayloadFormatL( const TDesC8& aConfigParams, 
                                      CMccRtpMediaClock& aClock );
        
	private:
	
		CAvcPayloadFormatWrite ( );
		void ConstructL( MDataSink* aSink);
		TBool ProcessFramesL();
		CMMFDataBuffer* CreateSinkBufferOfSizeL( TUint aSize ); 
    
    private:
  
      	CFormatEncodeStateMachine * iStateMachine;
      	CMMFDataBuffer * iSourceBuffer;
      	CRFC3984Encode * iEncoder; 					// payloadization class pointer
      	CMMFDataBuffer * iSinkBuffer;			// sink buffer 
      	TRtpSendHeader iRtpSendHeader;   	// RTP Header	
      	TTimeIntervalMicroSeconds iFrameTimeInterval;
      	TMediaId iMediaId;
      	
      	// Parameters relating to RTP data encoding
        TMccCodecInfo iCInfo;
        
        // Pointer to current sink (redundancy or rtp data sink)
        MDataSink* iCurDataSink;
        
        TBool iIsRtpSink;
        
        TUint iSeqNumIncrementer;
	
	    #if defined( TEST_EUNIT ) && defined ( __WINSCW__ )
        friend class UT_CAvcPayloadFormatWrite;
        #endif
    };

#endif