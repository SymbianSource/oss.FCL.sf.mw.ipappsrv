/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/


#ifndef MSRPPAYLOADFORMATWRITE_H
#define MSRPPAYLOADFORMATWRITE_H

//  INCLUDES
#include <e32base.h>
#include <mmfformat.h>
#include <mmfdatabuffer.h>
#include <mmfutilities.h>
#include "formatstatemachine.h"
#include "mccinternalcodecs.h"
#include "sendstatemachine.h"
#include "mccmsrpsink.h"


// RFC 4103 specifies that time stamp offset maxvalue
const TUint16 KHighestTimeStampOffset = 0x3FFF; // 16383
 
// 0x200 is selected because when using redundancy we must 
// be able to send 3x payload + header. This is the treshold 
// when RTP packet size grows too much
const TInt KMaxRTTChunkSize = 0x200; // 512

// When using level 2 redundancy, Redundancy Header size.
const TInt KRedHeaderSize = 0x48; // 32 + 32 + 8 = 72; 

// Maximum datasize
const TInt KDataSize = 0x600;// KMaxRTTChunkSize * 3;

// Default sampling Rate for RTP Clock
const TInt KDefaultSampleRate = 1000;


// FORWARD DECLARATIONS
class MMccRtpDataSink;
class CMccRtpMediaClock;
class TCharElement;


class TStringElement
 {
 	
	public :
	 TStringElement();
	 
	public :
	 TBuf16<512> iData;
	 TInt64 iTimestamp; 
 
 };
    
// CLASS DECLARATION
/**
*  Packetizes RTP payload containing Msrp data.
*
*  @lib MccMsrpPlFormat.dll

*/
class CMsrpPayloadFormatWrite : public CPayloadFormatWrite, 
							MPayloadFormatWrite, MSendStateObserver
    {
    public:  // Constructors and destructor
    
   
        static CMsrpPayloadFormatWrite* NewL( MDataSink* aSink );

        /**
        * Destructor.
        */
        virtual ~CMsrpPayloadFormatWrite();

    public: // New functions

        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        *
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams,
                                              CMccRtpMediaClock& aClock );
    
    public: // From MPayloadFormatWrite
    
        /**
        * Empty the source buffer by formatting the Msrp frames into payload.
        * Source buffer is given in "iBuffer".
        * Called by the state machine.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void EmptySourceBufferL();

        /**
        * Handle the event that source buffer has been emptied.
        * Source buffer is given in "iBuffer".
        * Called by the state machine.
      	*/
        void SourceBufferEmptiedL();
    
    public: // From CMMFFormatEncode

        /**
        * Funtion to return time interval for the complete frame
        *
        * @param aMediaType KUidMediaTypeText
        *
        * @return           time interval in micro seconds
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;
                    
                    
        /**   *** NOT SUPPORTED ***
        * Function to return the clip duration
        *
        * @param aMediaType KUidMediaTypeText
        *
        * @return           clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;

        /**
        * Create a sink buffer for the given media
        *
        * @param aMediaId   contains the media type KUidMediaTypeText
        * @param aReference value of False is returned to caller if sink 
        *                   buffer is created
        * @return           Pointer to sink buffer created
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, 
                                       TBool& aReference );

        /**
        * Return the sink four CC code for the given media
        *
        * @param aMediaId    contains the media type KUidMediaTypeText
        * @return            FourCC code for the mediaId
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
        * Set the sink data type to the given four CC code for the given media
        *
        * @param aSourceFourCC    fourCC code
        * @param aMediaId         contains the media type KUidMediaTypeText
        * @return                 error code KErrNotSupported if invalid media 
        *                         ID, else return KErrNone
        */
        TInt SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                  TMediaId aMediaId );

        /**
        * Log in to the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log on procedures.
        *
        * @param aEventHandler    address of event handler
        *
        * @return                 error code returned by source clip
        */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * Log out of the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log off procedures.
        *
        * @param
        *
        * @return
        */
        void SinkThreadLogoff();

        /**
        * Empty the given source buffer.
        *
        * @param aBuffer    data buffer containing Msrp data / redundancy data
        * @param aSupplier  data source pointer
        * @param aMediaId   contains the media type KUidMediaTypeText
        * @return
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, 
                           MDataSource* aSupplier, 
                           TMediaId aMediaId );
        
        /**
        * Called after the data buffer is written. Update the number of bytes 
        * written and the current write position for the next write operation. 
        *
        * @param aBuffer       data buffer emptied
        *
        * @return
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );

    public: // From MDataSink
    
        /**
        * From MDataSink Primes the source.
        * @since 
        * @param
        * @return
        */
        void SinkPrimeL();

        /**
        * From MDataSink Plays the source.
        * @since 
        * @param
        * @return
        */
        void SinkPlayL();

        /**
        * From MDataSink Pauses the source.
        * @since 
        * @param
        * @return
        */
        void SinkPauseL();

        /**
        * From MDataSink Stops the source.
        * @since 
        * @param
        * @return
        */
        void SinkStopL();
        
        
       //FROM NONE, THIS IS A TEST METHOD
       //TInt TimerExpired();
       
       TInt SendRedundantDataL();
       
       TInt FormUserDataL();
        
        //FROM MSendStateObserver
       void TimerExpiredL();
       
       void ForwardDataL();
       
       void GotoIdleL();
       
       TInt GenerateRedPacket(); 
       
    
    private: // New functions

        /**
        * Prepare packet header and deliver the packet to the datasink.
        * @since Series 60 3.0
        * @param aPayload Payload to deliver to the datasink.
        * @return
        */        
        void DeliverPacketL( CMMFDataBuffer& aPayload );
        
        void AssembleRedPacket();
        

        
        /**
        * Create a sink buffer of the given size.
        *
        * @param   aSize     size of sink buffer to create
        *
        * @return  Pointer to sink buffer created
        */
        CMMFDataBuffer* CreateSinkBufferOfSizeL( TUint aSize );
        
        /**
         * Update payload format with format spesific parameters.
         * Leaves with KErrArgument if parameters are invalid.
         *
         * @since    Series 60 3.0 
         * @param    aConfigParams       Configuration parameters
         * @return   void
         */
        void UpdateConfigurationL( const TDesC8& aConfigParams );

    private:
    
        /**
        * C++ default constructor.
        */
        CMsrpPayloadFormatWrite ();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aConf    Decoder Configuration params
        */
        void ConstructL( MDataSink* aSource );
        
        void Write24( TUint8* const aPointer, TUint32 aValue );
        
        void Write32( TUint8 *const aPointer, TUint32 aValue );
        
        TUint16 SetTimeOffset( TInt64& aPrim, TInt64& aSecond );
        
        void CancelUlRequest();
        
    private: // Data

        // FourCC
        TFourCC iFourCC;
        
        // Format encoding state machine
        CFormatEncodeStateMachine* iStateMachine;
        
        // Pointer to source data buffer, that is passed from DataPath
        CMMFDataBuffer* iSourceBuffer;

        // Sink buffer to hold Msrp/RTP payload
        CMMFDataBuffer* iSinkBuffer;
        
        // Current RTP timestamp 
        TUint32 iCurrentTimestamp;

        // Flag to indicate whether to set MARK field to be 1 (the first 
        // packet in a session should have MARK field set. Also first packet 
        // after and idle period should have MARK field seet to 1 )
        TBool iFirstPacketFinished;
                
        // Data sink capable to receive RTP data
        MMccRtpDataSink* iRtpDataSink; 
        
        // Data sink capable to receive MSRP data
        CMccMsrpSink* iMsrpDataSink; 
        
        // Parameters relating to RTP data encoding
        TMccCodecInfo iCodecInfo;
        
        // Rtp Sink, this is default ETrue
        TBool iIsRtpSink;
        
        // Msrp Sink
        TBool iIsMsrpSink;
        
        // Key for Rtp media clock 
        TUint32 iKey;
        
        /**
         * Rtp media clock instance
         * Not own.
         */
        CMccRtpMediaClock* iRtpMediaClock;
        
        
        //Redundancy Data
        CArrayFixFlat< TStringElement >* iRedData;
        
        CArrayFixFlat< TStringElement >* iUserBuffer;
        
        //Data that is modified so that we can create 
        //msrppacket
        HBufC8* iBuf;
        
        // pointer to the beginning of the iBuf
        TUint8* iData;
        
        //Timer that is used to send data in buffer periods
         CSendStateMachine* iTimer;
        
        //Flag indicating that we are in idle mode
        TBool iIdleMode;
        
  
    private: 
        
        //#ifdef TEST_EUNIT
            // For EUnit test cases
        friend class UT_CMsrpPayloadFormatWrite;
        //#endif
        
    };

#endif //MsrpPAYLOADFORMATWRITE_H

// End of File
