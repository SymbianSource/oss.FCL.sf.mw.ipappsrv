/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides functionality for DTMF decoding and playing.
*
*/




#ifndef __CDTMFPAYLOADFORMATREAD_H__
#define __CDTMFPAYLOADFORMATREAD_H__

//  INCLUDES
#include <e32base.h>
#include <mdaaudiotoneplayer.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "dtmfpayloadformatdefs.h"
#include "mccinternalcodecs.h"

// FORWARD DECLARATIONS
class MDataSource;
class CDTMFPayloadDecoder;
class TDTMFEventPayloadInfo;
class TDTMFTonePayloadInfo;


// CLASS DECLARATION

/**
 *  Handles DTMF reading from RTP payload and playing out DTMFs.
 *  Supported DTMF payload format is telephone-event payload.
 *
 *  @lib mmccdtmfplformat.dll
 *  @since Series 60 3.2
 */
class CDTMFPayloadFormatRead : public CPayloadFormatRead, 
                               public MPayloadFormatRead, 
                               public MMdaAudioToneObserver
    {
    public: // Constructors and destructor
        
        /**
         * Two-phased constructor.
         */
        static CDTMFPayloadFormatRead* NewL( MDataSource* aSource );
        
        /**
         * Destructor.
         */
        virtual ~CDTMFPayloadFormatRead();

    public: // New functions
        
        /**
         * Sets payload format to be used in DTMF decoding.
         *
         * @since   Series 60 3.2
         * @param   aPayloadFormat  Payload format assumed in RTP payload
         * @return  System wide error code
         */ 
        virtual TInt SetPayloadFormat( TDTMFPayloadFormat aPayloadFormat );
        
        /**
         * Indicates the RTP data source has filled the buffer.
         *
         * @since   Series 60 3.2
         * @param   aBuffer        The filled buffer
         * @param   aRtpHeader     RTP header relating to passed buffer
         */
        virtual void DataBufferFilledL( CMMFBuffer* aBuffer, 
                                    const TRtpRecvHeader& aRtpHeader );

    public: // Functions from CMMFFormatDecode

        /**
         * From CMMFFormatDecode. Tests whether a source
         * buffer can be created.
         *
         * @since   Series 60 3.2
         * @return  ETrue if a source buffer can be
         *          created. EFalse otherwise.
         */
        TBool CanCreateSourceBuffer();

        /**
         * From CMMFFormatDecode. Not applicable for DTMF payload formatter.
         *
         * @since   Series 60 3.2
         * @param   aMediaType  The UID of the media type
         * @return  The number of streams
         */
        TUint Streams( TUid aMediaType ) const;

        /**
         * From CMMFFormatDecode. Not applicable for DTMF payload format read.
         *
         * @since   Series 60 3.2
         * @param   aMediaType  The media type ID.
         * @return  The time interval for one frame, in microseconds.
         */
        TTimeIntervalMicroSeconds FrameTimeInterval( 
            TMediaId aMediaType ) const;

        /**
         * From CMMFFormatDecode. Not applicable for DTMF payload format read.
         *
         * @since   Series 60 3.2
         * @param   aMediaType  Media type ID.
         * @return  Duration of the sink clip.
         */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;

        /**
         * From CMMFFormatDecode. Not used, because there is no need for data
         * exchange with DataPath. DTMF receiving and processing cycle is
         * managed by SourcePrimeL(), SourcePlayL(), SourcePauseL(),
         * SourceStopL() -methods.
         *
         * @since   Series 60 3.2
         * @param   aBuffer
         * @param   aConsumer
         * @param   aMediaId
         */
        void FillBufferL( CMMFBuffer* aBuffer,
                          MDataSink* aConsumer, 
                          TMediaId aMediaId );

        /**
         * From CMMFFormatDecode. Not used.
         *
         * @since   Series 60 3.2
         * @param   aMediaId
         * @param   aReference
         * @return  NULL or KErrNotSupported
         */
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, 
                                         TBool& aReference );

        /**
         * From CMMFFormatDecode. Gets the FourCC code for the 
         * specified media ID.
         *
         * @since   Series 60 3.2
         * @param   aMediaId     Media ID
         * @return  The source data type code.
         */
        TFourCC SourceDataTypeCode( TMediaId aMediaId );
        
        /**
         * Gets the data type code for the sink specified by the media ID.
         * Format decode may act in role of MDataSink.
         *
         * @since   Series 60 3.2
         * @param   aMediaId    Media ID
         * @return  The data sink type code
         */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );        
        
        /**
         * Sets the source's priority settings.
         *
         * @since   Series 60 3.2
         * @param   aPrioritySettings   Source priority settings.
         */        
        void SetSourcePrioritySettings( 
            const TMMFPrioritySettings &aPrioritySettings );

    public:    

        /**
         * Configures payload format with format spesific parameters.
         * Leaves with KErrArgument if parameters are invalid.
         *
         * @since   Series 60 3.2
         * @param   aConfigParams   Configuration parameters
         */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams );
        
    public: // From MDataSource
    
        /**
         * From MDataSource. Primes the source.
         *
         * @since   Series 60 3.2
         */
        void SourcePrimeL();
        
        /**
         * From MDataSource. Plays the source.
         *
         * @since   Series 60 3.2
         */        
        void SourcePlayL();
        
        /**
         * From MDataSource. Pauses the source.
         *
         * @since   Series 60 3.2
         */   
        void SourcePauseL();
        
        /**
         * From MDataSource. Stops the source.
         *
         * @since   Series 60 3.2
         */
        void SourceStopL();
        
        /**
         * From MDataSource. Logs on the source thread.
         *
         * @since   Series 60 3.2
         * @param   aEventHandler   Used to send events back to the data path
         */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
        
        /**
         * From MDataSource. Logs off from the source thread.
         *
         * @since   Series 60 3.2
         */
        void SourceThreadLogoff( );
        
    public: // MPayloadFormatRead
        
        /**
         * Send decoded Events or Tones to further processing.
         * Called by the state machine.
         *
         * @since   Series 60 3.2
         */   
        void SendDataToSinkL();
        
        /**
         * Fill the sink buffer by decoding the RTP payload into
         * DTMF events or Tones. Called by the state machine.
         *
         * @since   Series 60 3.2
         */    
        void FillSinkBufferL();
        
        /**
         * Request the RTP source to fill the source buffer with RTP payload.
         * Source buffer is decided by the RTP source.
         * Called by the state machine.
         *
         * @since   Series 60 3.2
         */    
        void FillSourceBufferL();

        /**
         * From MPayloadFormatRead. Error notification if one of the
         * statechange callbacks leaved.
         *
         * @since   Series 60 3.2
         * @param   aError     Leave error code
         * @param   aState     State that caused the leave
         * @return  TInt       System wide error code from error handling
         */
        TInt HandleError( TInt aError, TFormatDecodeState aState );

    public: // From MMdaAudioToneObserver
    
        /**
         * From MMdaAudioToneObserver
         */
        void MatoPrepareComplete( TInt aError );
        
        /**
         * From MMdaAudioToneObserver
         */        
        void MatoPlayComplete( TInt aError );

    private:

        /**
         * C++ default constructor.
         */
        CDTMFPayloadFormatRead();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL( MDataSource* aSource );
        
        /**
         * Handle and play events using DevSound API.
         *
         * @since   Series 60 3.2
         */
        void HandleDTMFEventsL();
        
        /**
         * Handle and play tones using DevSound API.
         *
         * @since   Series 60 3.2
         */
        void HandleDTMFTones();
        
        /**
         * Creates a buffer used in data transfer between format read and
         * its datasource.
         *
         * @since    Series 60 3.2 
         * @return   void
         */
        void CreateClipBufferL();
        
        /**
         * Compares received RTP header to last received when marker bits are
         * set. Function returns ETrue if headers match so that only sequence
         * number is incremented in aRtpheader.
         *
         * @since S60 v3.2
         * @param aRtpheader RTP header to compare.
         * @return TBool ETrue if headers match.
         */
        TBool CompareFirstPacketRtpHeaders( const TRtpRecvHeader& aRtpheader );

    private: // Data
        
        /**
         * Pointer to format decoding state machine
         * Own.
         */        
        CFormatDecodeStateMachine* iStateMachine;

// *** Relating to decoding functionality  *** //
        
        /**
         * Pointer to payload decoder
         * Own.
         */        
        CDTMFPayloadDecoder* iPayloadDecoder;
        
        /**
         * Pointer to buffer to hold payload got from data source
         */
        CMMFDataBuffer* iSourceBuffer;
        
        // Indicates whether payload read has ownership of source buffer
        TBool iSourceBufOwnership;
        
        /**
         * Indicates do we have payload buffer from data source
         */        
        TBool iBufferToReadExists;
        
        /**
         * Array containing Event Payload field values
         */        
        RArray<TDTMFEventPayloadInfo> iEventPayloadInfo;
        
        /**
         * Array containing Tone Payload field values
         */        
        RArray<TDTMFTonePayloadInfo> iTonePayloadInfo;
        
        /**
         * Current event ongoing. Needed for final packet loss detection.
         */
        TChar iCurrentEvent;
        
        /**
         * Info about is M(arker) bit set in a RTP packet
         */        
        TBool iFirstPacket;
        
        /**
         * Time stamp of previous event
         */        
        TTimeIntervalMicroSeconds32 iTimeStampOfPrevEvent;
        
        /**
         * Payload format (Event, Tone) used in RTP packets
         */        
        TDTMFPayloadFormat iPayloadFormat;
                
// *** Relating to DTMF playback *** //

        /**
         * Pointer to tone player for local/inband DTMF playing
         * Own.
         */        
        CMdaAudioToneUtility* iTonePlayer;

        /**
         * Parameters relating to RTP data encoding
         */        
        TMccCodecInfo iCInfo;
        
        /**
         * Receive header from previous packet.
         */
        TRtpRecvHeader iReceivedHeader;

    private: // Friend classes
        
        #ifdef TEST_EUNIT
            friend class UT_CDTMFPayloadFormatRead;
        #endif

    };

#endif      // __CDTMFPAYLOADFORMATREAD_H__

// End of File
