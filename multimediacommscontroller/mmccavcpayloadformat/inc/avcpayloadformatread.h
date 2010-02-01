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
* Description:    The depayloadization part of the AVC payloadization/depayloadization plugin
*
*/




#ifndef AVCPAYLOADFORMATREAD_H
#define AVCPAYLOADFORMATREAD_H

//  INCLUDES
#include <E32Base.h>
#include <e32std.h>
#include <mmf/server/mmfdatabuffer.h>
#include <mmf/common/mmfutilities.h>
#include <ecom/ImplementationProxy.h>
#include "rtpheader.h"
#include "rfc3984decode.h"
#include "formatstatemachine.h"


// classes forward decleration

//class CRFC3984Decode;
class TRtpRecvHeader;

// CLASS DECLARATION

#define KAvcMaxRxPacketSize 10000


/**
*  The CAvcPayloadFormatRead class implements the MMF format plugin for decoding of RTP payload
*  format for AVC codec.  Refer to IETF RFC3984 for RTP Payload format for AVC codec.
*  This class receives AVC payload data from an RTP packet, depayloadizes it and passes it to the jitter buffer
*
*  @lib ?library
*  @since Series 60 3.0
*/ 
class CAvcPayloadFormatRead : public CPayloadFormatRead, public MPayloadFormatRead
{

  public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aSource    Source of the data for the payload format plugin
        */
        static   CAvcPayloadFormatRead* NewL ( MDataSource* aSource );

        /**
        * Destructor.
        */
        virtual  ~CAvcPayloadFormatRead ( );

        virtual void InitializeL(  );
        
        void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer, 
                                         TMediaId aMediaId );
       

        virtual void CancelDlRequest( );

        void SendDataToSinkL( );

     
      
        void FillSourceBufferL( );
        
       virtual void SetPayloadType( TUint8 aPayloadType );
       
       virtual TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
       virtual void SourceThreadLogoff( );
       virtual void NegotiateSourceL( MDataSink& aDataSink );
       
       virtual void BufferFilledL( CMMFBuffer* aBuffer, 
                                           const TRtpRecvHeader& aRtpHeader );
       
       TFourCC SourceDataTypeCode( TMediaId aMediaId );
       TInt SetSourceDataTypeCode( TFourCC aSourceFourCC, 
                                                   TMediaId aMediaId );

	   void FillSinkBufferL( );
	   
	   virtual CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, TBool &aReference );
	   
	   
	   void SourcePrimeL( );
	   void SourcePlayL();
	   void SourcePauseL();
	   void SourceStopL();
	   
	   void UseRTPInfoExtension();
	   
	  
	// from CPayloadFormatRead
	
	virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams );  
    
    
   // from CMMFFormatDecode
   
    /**
        * Return the frame time interval for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Time interval in micro seconds
        */
        virtual TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;

        /**  *** NOT SUPPORTED ***
        * Return the clip duration for the given media
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           Clip duration in micro seconds
        */
        virtual  TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;
        
        virtual  TUint Streams(TUid aMediaType) const;
        
   
   
   	/**
        * From CMMFFormatDecode
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );
        
   // From MDataSink
   
   protected:
   
     TFourCC iFourCC;  
   
   private:
   
   CAvcPayloadFormatRead ( );
   void ConstructL(MDataSource* aSource);
   CMMFBuffer* CreateSourceBufferNowL( TMediaId aMediaId, TBool &aReference );
   
   CRFC3984Decode * iDecoder;  	// depayloadization class pointer
 					 	// contains H264
   MDataSource* iRtpDataSource; // data source
   MDataSource * iCurDataSource; // currently pointed data source, same as RTP Data Source
   
   TBool iBufferToReadExists;
   TBool iFillRequested;
   
   // Data sink who requested "FillBuffer".
   MDataSink* iSink;
   
   // RTP Header
		TRtpRecvHeader iRecvHeader;
   
   TMediaId iMediaId;
   
   // Format decoding state machine
   CFormatDecodeStateMachine* iStateMachine;  
   
   // Count of depayloadized NAL units
   TInt iNalCount;
   TInt iNalSent;
   
   // DG: Use to provide several packets at a time to the jitter buffer
   TInt iCounter;
   

   CMMFDataBuffer * iPayloadBuffer;  	// Sink Buffer -- data comes into this buffer, is depacketized 
   CMMFDataBuffer * iFrameBuffer; 		// Source Buffer -- data is put in this buffer after depacketization and given to data path
   
   // Payload types to expect including PTs for redundancy encodings
		RArray<TUint8> iPayloadTypes;
	
	TBool iUsingJitterBufferExtension;		// indicates if using jitter buffer extension structure
  
   
};

#endif 
