/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class definition for the AMR/RTP payload format plugin
*
*/




#ifndef AMRPAYLOADFORMATWRITE_H
#define AMRPAYLOADFORMATWRITE_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "amrpayloadformatter.h"
#include "mccinternalcodecs.h"


// FORWARD DECLARATIONS
class CAmrPayloadEncoder;
class CAmrPayloadDecoder;
class MMccRtpDataSink;

/**
*  The CAmrPayloadFormatWrite class implements the MMF format plugin for encoding of RTP payload
*  format for AMR codec.  Refer to IETF RFC3267.
*  This class receives one or more AMR frames and converts them into payload data
*  of an RTP packet.
*
*  @lib Mccamrplformat.dll
*  @since Series 60 3.0
*/ 
class CAmrPayloadFormatWrite : public CPayloadFormatWrite, public MPayloadFormatWrite
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        *
        * @param aSink    Datasink for the payload format write
        */
        static   CAmrPayloadFormatWrite* NewL ( MDataSink* aSink );

        /**
        * Destructor.
        */
        virtual  ~CAmrPayloadFormatWrite ( );

    public:  // New functions

        /**
        * Cancel any outstanding requests. 
        * @since Series-60 2.1
        */
        virtual void CancelUlRequest( );

        /**
        * Empty the source buffer by formatting the AMR frames into RTP payload.
        * Source buffer is given in "iBuffer".
        * Called by the state machine.
        * @since Series-60 2.1
        */
        void EmptySourceBufferL( );

        /**
        * Hanlde the event that source buffer has been emptied.
        * Source buffer is given in "iBuffer".
        * Called by the state machine.
        * @since Series-60 2.1
        */
        void SourceBufferEmptiedL( );
        
         /**
          * Checks if codec mode change request is valid.
          * @since    Series 60 3.0 
          * @param    Desired bitrate
          * @return   TBool
          */
        virtual TBool IsBitrateChangeValid( TInt aBitRate ); 

    public: // From CMMFFormatEncode
  
        /**
        * Funtion to return time interval for the complete frame
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           time interval in micro seconds
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;

        /**   *** NOT SUPPORTED ***
        * Funtion to return the clip duration
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        * @return           clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId /*aMediaType*/ ) const;

        /**
        * Accessor funtion to return the number of channels
        *
        * @return   Number of channels
        */
        TUint NumChannels( );

        /**
        * Accessor funtion to return the sampling rate
        *
        *
        * @return   Sampling rate
        */
        TUint SampleRate( );

        /**
        * Set the packetizationrate to the given value.
        *
        * @param aPacketizationRate  - [input] Number of AMR frame blocks per RTP packet
        *
        * @return  error code KErrNotSupported if invalid packetizationrate, else return KErrNone
        */
        TInt SetSampleRate( TUint aSampleRate );

        
    public: // From MDataSink

        /**
        * Create a sink buffer for the given media
        *
        * @param aMediaId   contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        * @param aReference value of False is returned to caller if sink buffer is created
        *
        * @return           Pointer to sink buffer created
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, 
                                       TBool &aReference ); 

        /**
        * Return the sink four CC code for the given media
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return            FourCC code for the mediaId
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
        * Set the sink data type to the given four CC code for the given media
        *
        * @param aSinkFourCC      fourCC code
        * @param aMediaId         contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return                 error code KErrNotSupported if invalid media ID, else return KErrNone
        */
        TInt SetSinkDataTypeCode( TFourCC aSinkFourCC, TMediaId aMediaId );

        /**
        * Log in to the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log on procedures.
        *
        * @param aEventHandler    address of event handler
        * @return                 error code returned by source clip
        */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * Log out of the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log off procedures.
        *
        *
        * @return None
        */
        void SinkThreadLogoff( );

        /**
        * Empty the given source buffer.
        *
        * @param aBuffer    data buffer containing encoded AMR frames
        * @param aSupplier  data source pointer
        * @param aMediaId   contains the media type KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, 
                           MDataSource* aSupplier,
                           TMediaId aMediaId );

        /**
        * Called after the data buffer is written. Update the number of bytes written
        * and the current write position for the next write operation. 
        *
        * @param    aBuffer       data buffer emptied
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );

        /**
        * From MDataSink Primes the source.
        * @since Series 60 3.0
        */
        void SinkPrimeL( );

        /**
        * From MDataSink Plays the source.
        * @since Series 60 3.0
        */
        void SinkPlayL( );
        
        /**
        * From MDataSink Pauses the source.
        * @since Series 60 3.0
        */
        void SinkPauseL( );
        
        /**
        * From MDataSink Stops the source.
        * @since Series 60 3.0
        */
        void SinkStopL( );
        
    public:    

        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams,
                                              CMccRtpMediaClock& aClock );        
 
    private:

        /**
        * C++ default constructor.
        */
        CAmrPayloadFormatWrite ( );

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aSink    Datasink for the payload format write
        */
        void ConstructL( MDataSink* aSink );

    private: // New functions

        /**
        * Packetize the AMR frames received from AMR codec and deliver the packets.
        * @since 
        * @return Returns true if current talk spurt is finished.
        */
        TBool ProcessFramesL( );

        /**
        * Prepare packet header and deliver the packet to the datasink.
        * @since 
        * @param aPayload Payload to deliver to the datasink.
        */        
        void DeliverPacketL( CMMFDataBuffer& aPayload );

        /**
        * Create a sink buffer of the given size.
        *
        * @param   aSize     size of sink buffer to create
        * @return  Pointer to sink buffer created
        */
        CMMFDataBuffer* CreateSinkBufferOfSizeL( TUint aSize ); 
        
        /**
         * Update payload format with format spesific parameters.
         * Leaves with KErrArgument if parameters are invalid.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Configuration parameters
         * @return   void
         */
        void UpdateConfigurationL( const TMccCodecInfo& aCodecInfo ); 

    private:    // Data

		// True if this AMR narrow band write, False if wideband
		TBool iIsNb;

        // fourCC
        TFourCC iFourCC;

        // Format encoding state machine
        CFormatEncodeStateMachine* iStateMachine;
        
        // Pointer to source data buffer, passed from data path
        CMMFDataBuffer* iSourceBuffer;

        // Sampling rate from clip header
        TInt iSamplingRate;

        // Number of channels from clip header
        TUint iChannels;

        // Payload encoder used to construct RTP AMR payload in bandwidth efficient mode
        CAmrPayloadEncoder* iPayloadEncoder;

        // Current encoded channel number
        TUint iCurrentChannel;

        // Flag to indicate whether to set MARK field to be 1
        // ( the first packet in a talk spurt should have MARK field set )
        TBool iFirstPacketFinished;

        // Packetization Rate
        TInt iFramesPerPacket;
        
        // Sink buffer to hold AMR/RTP payload
        CMMFDataBuffer* iSinkBuffer; 

        // RTP header used for sending RTP packet
        TRtpSendHeader iRtpSendHeader;

        // current media type
        TMediaId iMediaId;

        TBool iNeedToFlush;
        
        // Counter for mode change period purpose
        TUint32 iFramesEncoded;
        
        TBool iFirstCmrHandled;
        
        // Parameters relating to RTP data encoding
        TMccCodecInfo iCInfo;
        
        // Data sink capable to receive RTP data
        MMccRtpDataSink* iRtpDataSink;
        
        TBool iIsRtpSink;

        // Mode-set
        RArray<TUint32> iModes;
        
        // Key for Rtp media clock
        TUint32 iKey;
        
        /**
         * Rtp media clock instance
         * Not own.
         */
        CMccRtpMediaClock* iRtpMediaClock;
        
    private:    // Friend classes
    
    #ifdef TEST_EUNIT
        friend class UT_CAmrPayloadFormatWrite;
    #endif
    
    };

#endif      //AMRPAYLOADFORMATWRITE_H
