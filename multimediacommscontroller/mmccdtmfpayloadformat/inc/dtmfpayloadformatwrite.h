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
* Description:    Provides functionality for DTMF digit encoding and sending.
*
*/




#ifndef CDTMFPAYLOADFORMATWRITE_H
#define CDTMFPAYLOADFORMATWRITE_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfformat.h>
#include "rtpheader.h"
#include "mmccevents.h"
#include "dtmfpayloadformatdefs.h"
#include "mccinternalcodecs.h"
#include "dtmfencstatemachine.h"
#include "formatstatemachine.h"

// FORWARD DECLARATIONS
class CDTMFPayloadEncoder;
class MMccRtpDataSink;
class CMccRtpMediaClock;
class CDtmfHighResTimer;

// CLASS DECLARATION

/**
 * Handles DTMF encoding and passing to the RTP data sink.
 * Supported DTMF payload format is telephone-event payload.
 *
 * @lib mmccdtmfplformat.dll
 * @since Series 60 3.2
 */
class CDTMFPayloadFormatWrite : public CPayloadFormatWrite,
                                public MDtmfEncClient
    {
    public: // Constructors and destructor
        
        /**
         * Two-phased constructor.
         */
        static CDTMFPayloadFormatWrite* NewL( MDataSink* aSink );
        
        /**
         * Destructor.
         */
        virtual ~CDTMFPayloadFormatWrite();

    public: // New functions

        /**
         * Used to dispatch timer events from the timer to the class instance
         * being interested in receiving them.
         *
         * @since   Series 60 3.2
         * @param   aObjectPtr Pointer to object that wants 
         *          to receive timer events
         * @return  Non-zero value if current task is ongoing
         */
        static TInt HandleTimerEventL( TAny* aObjectPtr );
        
        /**
         * Start to packetize DTMF digit to the RTP packets and send them.
         *
         * @since   Series 60 3.2
         * @param   aTone           Digit user has pressed at UI
         */
        virtual void StartDTMFToneL( const TChar aTone );
        
        /**
         * Stop DTMF digit packetizing and sending.
         *
         * @since   Series 60 3.2
         */
        virtual void StopDTMFToneL();
        
        /**
         * Starts sending of passed DTMF string incrementally.
         *
         * @since   Series 60 3.2
         * @param   aTones          DTMF string to send
         */        
        virtual void SendDTMFTonesL( const TDesC8& aTones );
        
        /**
         * Start/Stop notify about stops ('w' char) in a DTMF string.
         *
         * @since   Series 60 3.2
         * @param   aFlag   ETrue to enable, EFalse to disable notify service
         */
        virtual void NotifyStopInDTMFString( TBool aFlag );
        
        /**
         * Continue DTMF string sending after stop in a string has been
         * encountered.
         *
         * @since   Series 60 3.2
         * @param   aContinue   Whether to continue string sending
         * @return  KErrNone
         */
        virtual TInt ContinueDTMFStringSending( TBool aContinue );
        
        /**
         * Cancel DTMF string sending.
         *
         * @since   Series 60 3.2
         */
        virtual void CancelDTMFStringSending();
        
        /**
         * Returns payload format used in DTMF encoding.
         *
         * @since   Series 60 3.2
         * @return  Payload format used in DTMF encoding
         */ 
        virtual TDTMFPayloadFormat PayloadFormat() const;
        
        /**
         * Sets payload format to be used in DTMF encoding.
         *
         * @since   Series 60 3.2
         * @param   aPayloadFormat Payload format to be used
         * @return  System wide error code
         */ 
        virtual TInt SetPayloadFormat( TDTMFPayloadFormat aPayloadFormat );
        
        /**
         * From MDtmfEncClient. Called when it's time to handle next digit
         * in sequence while sending DTMF string.
         *
         * @since   Series 60 3.2
         */
        void EncodeNextDigitL();
        
        /**
         * From MDtmfEncClient. Does actions belonging to tone on state.
         * Actions include DTMF event update packets sending.
         *
         * @since   Series 60 3.2
         * @param   aEntryToState  Indicates is this transition to state
         */
        void DoToneOnActionsL( TBool aEntryToState );
        
        /**
         * From MDtmfEncClient. Does actions belonging to tone off state.
         * Actions include DTMF event final packet sending and 
         * retransmissions. During DTMF string sending this state implements
         * wait activity before proceeding to send next digit in a string.
         *
         * @since   Series 60 3.2
         * @param   aEntryToState   Indicates is this transition to state
         * @return  void
         */
        void DoToneOffActionsL( TBool aEntryToState );
        
    public: // Functions from CMMFFormatEncode

        /**
         * From CMMFFormatEncode. Gets the time interval for one frame.
         *
         * @since   Series 60 3.2
         * @param   aMediaType      The media type ID
         * @return  The time interval for one frame, in microseconds.
         */
        TTimeIntervalMicroSeconds FrameTimeInterval(
            TMediaId aMediaType ) const;
        
        /**
         * From CMMFFormatEncode. Gets the duration of the sink clip for the
         * specified media ID.
         *
         * @since   Series 60 3.2
         * @param   aMediaType      The media type ID
         * @return  Duration of the sink clip
         */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;
        
        /**
         * From CMMFFormatEncode. Empty the given source buffer.
         *
         * @since   Series 60 3.2
         * @param   aBuffer    data buffer containing DTMF payload
         * @param   aSupplier  data source pointer
         * @param   aMediaId   contains the media type KUidMediaTypeAudio or
         *                     KUidMediaTypeVideo
         */
        void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier,
            TMediaId aMediaId );

        /**
         * From CMMFFormatEncode. Called by the clip to indicate data has been
         * processed.
         *
         * @since   Series 60 3.2
         * @param   aBuffer    The emptied buffer.
         */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
         * From CMMFFormatEncode. DTMF payload format write doesn't support
         * sink buffer creation.
         *
         * @since   Series 60 3.2
         * @return  EFalse
         */        
        TBool CanCreateSinkBuffer();
        
        /**
         * From CMMFFormatEncode. DTMF payload format write doesn't support
         * sink buffer creation.
         *
         * @since   Series 60 3.2
         * @param   aMediaId    contains the media type KUidMediaTypeAudio or
         *                      KUidMediaTypeVideo 
         * @param   aReference  value of False is returned to caller if sink 
         *                      buffer is created
         * @return  NULL
         */
        CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, TBool &aReference );
        
    public:     // Functions from MDataSink
        
        /**
         * From MDataSink. Passes prime transition to the RTP data sink.
         *
         * @since   Series 60 3.2
         */
        void SinkPrimeL();
        
        /**
         * From MDataSink. Passes play transition to the RTP data sink.
         *
         * @since   Series 60 3.2
         */
        void SinkPlayL();
        
        /**
         * From MDataSink. Passes pause transition to the RTP data sink.
         *
         * @since   Series 60 3.2
         */
        void SinkPauseL();
        
        /**
         * From MDataSink. Passes stop transition to the RTP data sink.
         *
         * @since   Series 60 3.2
         */
        void SinkStopL();
                                     
        /**
         * Log in to the sink thread - this funtion merely passes the
         * command to its sink clip data source object. The sink clip
         * object will handle the thread log on procedures.
         *
         * @since   Series 60 3.2
         * @param   aEventHandler   address of event handler
         * @return  error code returned by source clip
         */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
         * Log out of the sink thread - this funtion merely passes the command
         * to its sink clip data source object. The sink clip object will
         * handle the thread log off procedures.
         *
         * @since   Series 60 3.2
         */
        void SinkThreadLogoff();
        
        /**
         * Return the sink four CC code for the given media
         *
         * @since   Series 60 3.2
         * @param   aMediaId     contains the media type KUidMediaTypeAudio or
         *                       KUidMediaTypeVideo
         * @return  FourCC code for the mediaId
         */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
         * Set the sink data type to given four CC code for the given media
         *
         * @since   Series 60 3.2
         * @param   aSinkFourCC fourCC code
         * @param   aMediaId    contains the media type KUidMediaTypeAudio or
         *                      KUidMediaTypeVideo
         * @return  error code KErrNotSupported if invalid media ID, else
         *          return KErrNone
         */
        TInt SetSinkDataTypeCode( TFourCC aSinkFourCC, TMediaId aMediaId );
        
        /**
         * Sets the sink's priority settings.
         *
         * @since   Series 60 3.2
         * @param   aPrioritySettings    Sink priority settings.
         */
        void SetSinkPrioritySettings( const TMMFPrioritySettings& 
                                                        aPrioritySettings );

    public:    

        /**
         * Configures payload format with format spesific parameters and
         * sets RTP Media clock with which to synchronize DTMF sending.
         * Leaves with KErrArgument if parameters are invalid.
         *
         * @since   Series 60 3.2
         * @param   aConfigParams   Configuration parameters
         * @param   aRtpMediaClock  RTP media clock
         */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams,
                                              CMccRtpMediaClock& aClock );

    protected:

        /**
         * Called when specified time interval has elapsed.
         *
         * @since   Series 60 3.2
         * @return  Non-zero value if current task is ongoing
         */
        TInt DoHandleTimerEventL();

    private:

        /**
         * C++ default constructor.
         */
        CDTMFPayloadFormatWrite();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL( MDataSink* aSink );
    
        /**
         * Prepare packet header and deliver the packet to the datasink.
         *
         * @since   Series 60 3.2
         * @param   aPayload    Payload to deliver to the datasink.
         * @param   aMarkerBit  Marker bit on/off status
         */ 
        void DeliverPacketL( CMMFDataBuffer& aPayload, TBool aMarkerBit );
       
        /**
         * Check that digits are valid DTMF.
         *
         * @since   Series 60 3.2
         * @param   aDigits     DTMF string to be checked
         * @return  ETrue if valid, otherwise EFalse
         */
        TBool IsValidDigits( const TDesC8& aDigits ) const;
        
        /**
         * Takes care of handling 'p' and 'w' chars in a DTMF string.
         *
         * @since   Series 60 3.2
         * @param   aChar       Control char to handle
         */        
        void HandleControlChar( const TChar& aChar );
        
        /**
         * Signaling about events to the client.
         *
         * @since   Series 60 3.2
         * @param   aEventType  Type of the event to send to a client
         */ 
        void SendEventSignalToClient( TMccDtmfEventType aEventType ) const;
        
        /**
         * Checks that digit is valid DTMF.
         *
         * @since   Series 60 3.2
         * @param   aDigit      Digit to check
         * @return  ETrue if valid, otherwise EFalse
         */
        TBool IsValidDigit( const TChar& aDigit ) const;
        
    private: // Data
        
        class TSendBufferElement
            {
        
        public:
        
            TSendBufferElement( const TChar& aDtmfTone, TUint32 aStartTime )
                :
                iTone( aDtmfTone ),
                iStartTimeInMs( aStartTime ),
                iStopTimeInMs( 0 )
                {
                
                }
            
            const TChar& Tone() const
                {
                return iTone;
                }
            
            TUint32 StartTime() const
                {
                return iStartTimeInMs;
                }

            TUint32 StopTime() const
                {
                return iStopTimeInMs;
                }
            
            void SetStopTime( TUint32 aStopTime )
                {
                iStopTimeInMs = aStopTime;
                }
            
            TUint Duration() const
                {
                if ( iStopTimeInMs < iStartTimeInMs )
                    {
                    // wrap around situation
                    const TUint32 KMaxUintValue( 0xFFFFFFFF );
                    return ( ( KMaxUintValue - iStartTimeInMs ) + iStopTimeInMs );
                    }
                else
                    {
                    return ( iStopTimeInMs - iStartTimeInMs );
                    }
                }
        
        private:
        
            TChar iTone;
            TUint32 iStartTimeInMs;
            TUint32 iStopTimeInMs;    
            };
        
        enum TDTMFSendingMode
            {
            EModeNotDefined = 0,
            EModeManual     = 1,
            EModeAutomatic  = 2
            };

        /**
         * The FourCC code for DTMF PayloadFormatWrite
         */ 
        TFourCC iFourCC;
        
        /**
         * Pointer to sink buffer that holds encoded DTMF / RTP payload
         * Own.
         */        
        CMMFDataBuffer* iSinkBuffer;
                
        /**
         * Pointer to formatter class for encoding of RTP payload
         * Own.
         */        
        CDTMFPayloadEncoder* iPayloadEncoder;
        
        /**
         * Timer used for event updates and incremental DTMF string send.
         * Own.
         */
        CDtmfHighResTimer* iSendTimer;

        /**
         * RTP header used for sending RTP packet
         */        
        TRtpSendHeader iRtpSendHeader;

        /**
         * Contains current duration of tone
         */        
        TUint16 iToneDuration;
        
        /**
         * Payload format used for encoding
         */        
        TDTMFPayloadFormat iPayloadFormat;
        
        /**
         * Send buffer for timed DTMF sending
         */
        RArray<TSendBufferElement> iSendBuffer;
        
        /**
         * Indicates is DTMF generated inbandly or outbandly
         */        
        TInt iGenerationMode;
        
        /**
         * Pointer to asynchronous event handler
         * Not own.
         */        
        MAsyncEventHandler* iEventHandler;
        
        /**
         * Boolean indicating is NotifyStopInDTMFString() request in force
         */        
        TBool iNotifyStopReq;
        
        /**
         * Contains valid DTMF digits
         */        
        RArray<TChar> iValidDigits;

        /**
         * Parameters relating to RTP data encoding
         */        
        TMccCodecInfo iCInfo;
        
        /**
         * Used to keep track on how many final packets is
         * sent with current tone
         */        
        TInt iFinalPacketCtr;
        
        /**
         * Sending mode
         */        
        TDTMFSendingMode iSendingMode;
        
        /**
         * Pointer to state machine for DTMF encoding and sending
         * Own.
         */        
        CDtmfEncStateMachine* iStateMachine;
        
        /**
         * Pointer to data sink capable to receive RTP data.
         * Not own.
         */        
        MMccRtpDataSink* iRtpDataSink;
        
        /**
         * Key for Rtp media clock
         */        
        TUint32 iKey;
        
        /**
         * Pointer to RTP media clock used to synchronize
         * DTMF sending with audio
         * Not own.
         */        
        CMccRtpMediaClock* iRtpMediaClock;
        
        /**
         * Sampling rate used.
         */
        TUint iSampleRate;
        
        /**
         * Nanokernel tick period in milliseconds.
         */
        TInt iTickPeriod;
        
    private: // Friend classes
        
        #ifdef TEST_EUNIT
            friend class UT_CDTMFPayloadFormatWrite;
        #endif
    };

#endif      // CDTMFPAYLOADFORMATWRITE_H
            
// End of File
