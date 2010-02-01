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




#ifndef MCCH263PAYLOADFORMATWRITE_H
#define MCCH263PAYLOADFORMATWRITE_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfformat.h>
#include <mmf/common/mmfcontrollerframework.h>
#include "rtpheader.h"
#include "mccdef.h"
#include "mccinternalcodecs.h"
#include "formatstatemachine.h"
#include "mcch263payloadencoder.h"
#include "mcch263newpayloadencoder.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MMccEventHandler;
    
// CLASS DECLARATION
/**
*  Handles DTMF encoding and passing to the RTP data sink.
*  Supported DTMF payload format is telephone-event payload.
*
*  @lib MMMccDtmfPlFormat.dll
*  @since Series 60 3.0
*/
class CMccH263PayloadFormatWrite : public CPayloadFormatWrite, public MPayloadFormatWrite
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CMccH263PayloadFormatWrite* NewL( MDataSink* aSink );
        
        /**
        * Destructor.
        */
        virtual ~CMccH263PayloadFormatWrite();

    public: // New functions  
    
    public: // Functions from MPayloadFormatWrite
        /**
        * Empty sourcebuffer state change
        * @since    Series 60 3.0
        */
        void EmptySourceBufferL();
        
        /**
        * Sourcebuffer emptied state change
        * @since    Series 60 3.0
        */
        void SourceBufferEmptiedL();

    public: // Functions from CMMFFormatEncode

        /**
        * From CMMFFormatEncode. Gets the time interval for one frame.
        *
        * @since    Series 60 3.0
        * @param    aMediaType      The media type ID
        * @return   The time interval for one frame, in microseconds.
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;
        
        /**
        * From CMMFFormatEncode. Gets the duration of the sink clip for the specified media ID.
        *
        * @since    Series 60 3.0
        * @param    aMediaType      The media type ID
        * @return   Duration of the sink clip
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;
        
        /**
        * From CMMFFormatEncode. Empty the given source buffer.
        *
        * @since    Series 60 3.0
        * @param    aBuffer    data buffer containing DTMF payload
        * @param    aSupplier  data source pointer
        * @param    aMediaId   contains the media type KUidMediaTypeAudio or 
        *                      KUidMediaTypeVideo
        * @return   None
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, 
                           MDataSource* aSupplier, 
                           TMediaId aMediaId );

        /**
        * From CMMFFormatEncode. Called by the clip to indicate data has been processed.
        *
        * @since    Series 60 3.0
        * @param    aBuffer    The emptied buffer.
        * @return   None
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
        * From CMMFFormatEncode. DTMF payload format write doesn't support
        * sink buffer creation.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   EFalse
        */        
        TBool CanCreateSinkBuffer();
        
        /**
        * From CMMFFormatEncode. DTMF payload format write doesn't support
        * sink buffer creation.
        *
        * @param aMediaId   contains the media type KUidMediaTypeAudio or 
        *                   KUidMediaTypeVideo 
        * @param aReference value of False is returned to caller if sink 
        *                   buffer is created
        * @return           NULL
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, 
                                       TBool &aReference );        
        
    public:     // Functions from MDataSink
        
        /**
        * From MDataSink. Passes prime transition to the RTP data sink.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   None
        */
        void SinkPrimeL();
        
        /**
        * From MDataSink. Passes play transition to the RTP data sink.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   None
        */
        void SinkPlayL();
        
        /**
        * From MDataSink. Passes pause transition to the RTP data sink.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   None
        */
        void SinkPauseL();
        
        /**
        * From MDataSink. Passes stop transition to the RTP data sink.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   None
        */
        void SinkStopL();
                                     
        /**
        * Log in to the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log on procedures.
        *
        * @since    Series 60 3.0
        * @param    aEventHandler    address of event handler
        * @return   error code returned by source clip
        */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * Log out of the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log off procedures.
        *
        * @since    Series 60 3.0        
        * @param    None
        * @return   None
        */
        void SinkThreadLogoff();
        
        /**
        * Return the sink four CC code for the given media
        *
        * @since    Series 60 3.0
        * @param aMediaId    contains the media type KUidMediaTypeAudio or
        *                    KUidMediaTypeVideo
        * @return            FourCC code for the mediaId
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
        * Set the sink data type to the given four CC code for the given media
        *
        * @since    Series 60 3.0
        * @param aSinkFourCC      fourCC code
        * @param aMediaId         contains the media type KUidMediaTypeAudio or
        *                         KUidMediaTypeVideo
        * @return                 error code KErrNotSupported if invalid media 
        *                         ID, else return KErrNone
        */
        TInt SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                  TMediaId aMediaId );
    public:    

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

        
    protected:  // New functions


    protected:  // Functions from base classes


    private:

        /**
        * C++ default constructor.
        */
        CMccH263PayloadFormatWrite();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( MDataSink* aSink );
       
    public:
    
        static void PayloadEncodeCleanup( TAny* aAny );
        
    protected:  // Data

    private:    // Data
            
        // The FourCC code for DTMF PayloadFormatWrite
        TFourCC iFourCC;
        
        // Format encoding state machine
        CFormatEncodeStateMachine* iStateMachine;
        
		RPointerArray<CBufferContainer> iBuffers;

		MDataSource* iDataSource;
		
        // Current Media id ( audio, video )
        TMediaId  iMediaId;
                
        CMMFDataBuffer* iSourceBuffer;
        
        // Formatter class for encoding of RTP payload
        CMccH263PayloadEncoder* iPayloadEncoder;
        
        // Parameters relating to RTP data encoding
        TMccCodecInfo iCInfo;
        
        TBool iIsRtpSink;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

		#ifdef EUNIT_TEST
			friend class UT_CMccH263PayloadFormatWrite;	
   		#endif
    };

#endif      // MCCH263PAYLOADFORMATWRITE_H
            
// End of File
