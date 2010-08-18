/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RTP Datasink
*
*/





#ifndef MCCRTPDATASINK_H
#define MCCRTPDATASINK_H

//  INCLUDES
#include "rtpapi.h"
#include "mccdatasink.h"
#include "mccrtpinterface.h"
#include "rtpdatasink.h"
#include "mccrtpsender.h"

#ifdef FTD_ENABLED
#include <e32msgqueue.h>
#include "Mccstreamstats.h"
#endif

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MDataSource;

// CLASS DECLARATION
    
/**
*  Mcc RTP DataSink send RTP packet using to RtpAPI
*
*  @lib MccRtpSourceSink.dll
*  @since Series 60 3.0
*/
class CMccRtpDataSink : public CMccDataSink,
                        public MMccRtpInterface, 
                        public MMccRtpDataSink,
                        public MMccRtpSenderObserver
    {
    public:    // Constructors and destructor

        /**
        * Two-phased constructor. Virtual function from MDataSink
        */
        static MDataSink* NewSinkL( TUid aImplementationUid, 
                                    const TDesC8& aInitData );
   
        /**
        * Destructor.
        */
        virtual ~CMccRtpDataSink();

    public:    // New functions
        
        /**
        * Returns Synchronization source.
        * @since Series 60 3.0
        * @param None
        * @return Synchronization source value
        */
        inline TRtpSSRC GetSSRC() const
       		{
        	return iSSRC;
        	}

        /**
         * Mutes/Unmutes RTP sink. When mute is enabled, only packets with
         * exception payload type are sent.
         *
         * @since   Series 60 3.2
         * @param   aMuteOn         When ETrue mute is enabled
         * @param   aExceptionPt    Payload to send regardless of mute state
         */
        virtual void Mute( TBool aMuteOn, TUint8 aExceptionPt );

    public: // From CMccDataSink
    
        virtual void SetCurrentUser( MAsyncEventHandler* aEventHandler );
    
    public: // From MDataSink

        /**
        * From MDataSink. Pure virtual function that must be implemented.
        *                  Out of usage. Leave KErrNotSupported.
        * @since Series 60 3.0
        * @param "aBuffer" Out of usage
        * @param "aSupplier" Out of usage
        * @param "aMediaId" Out of usage
        * @return void
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, 
                           MDataSource* aSupplier, 
                           TMediaId aMediaId );

        /**
        * From MDataSink Method to 'logon' the data source to 
        *                 the same thread that source will be supplying data in.
        * @since Series 60 3.0
        * @param aEventHandler for send event to datasource when buffer is embtied. 
        * @return KErrNone if successful, otherwise a system wide error code.
        */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * From MDataSink Method to 'logoff' the data source from 
        *                 the same thread that source supplies data in.
        * @since Series 60 3.0
        * @param None
        * @return void
        */
        void SinkThreadLogoff();

        /**
        * From MDataSink pure virtual function that must be implemented.
        *                 Returns the data type as a fourCC code of the data sink.
        * @since Series 60 3.0
        * @param "aMediaId" This identifies the type of media eg audio and the stream id.
        *                    This parameter is required in cases where the sink can accept data
        *                    of more than one media type.
        * @return the datatype code ( codec ).
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
        * From MDataSink pure virtual function that must be implemented.
        *                 Sets the data type as a fourCC code for the data sink.
        * @since Series 60 3.0
        * @param "aSinkFourCC" This specifies the data type as a fourCC code to set the sink to
        * @param "aMediaId" This identifies the type of media eg audio and the stream id.
        * @return KErrNone if successful.
        * @return KErrLocked if Sink thread locked. Codec must to be set before thread logon.
        * @return KErrNotSupported if the sink does not support media type.
        */
        TInt SetSinkDataTypeCode( TFourCC aCodec, TMediaId aMedia );

        /**
        * From MDataSink pure virtual function that must be implemented. 
        *                 Out of usage. Function leaves with KErrNotSupported.  
        * @since    Series 60 3.0
        * @param    "aCodec" Out of usage
        * @param    aMedia    Out of usage
        * @return    void
        */
        void BufferFilledL( CMMFBuffer* aBuffer );

        /**
        * From MDataSink pure virtual function that must be implemented.
        * Method to indicate whether the data sink can create a buffer.
        * @deprecated
        *
        * @since Series 60 3.0
        * @param "aBuffer" Out of usage
        * @return False
        */
        TBool CanCreateSinkBuffer();

        /**
        * From MDataSink pure virtual function that must be implemented.
        * Out of usage. Function leaves with KErrNotSupported.
        * @deprecated
        *
        * @since Series 60 3.0
        * @param "aMediaId" Out of usage.
        * @param "aReference" Out of useage.
        * @return 
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId /*aMediaId*/, 
                                       TBool& aReference );
                                  
        /**
        * From MDataSink. This is a virtual function that a derived data sink 
        *                 can implement if any data sink specific 'priming' is required
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void SinkPrimeL();

        /**
        * From MDataSink. This is a virtual function that a derived data sink can implement if
        *                 any data sink specific action is required prior to 'playing'ie the 
        *                 start of data transfer
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void SinkPlayL();

        /**
        * From MDataSink. This is a virtual function that a derived data sink can implement if 
        *                 any data sink specific action is required to 'pause'
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void SinkPauseL();
        
        /**
        * From MDataSink. This is a virtual function that a derived data sink can implement if 
        *                 any data sink specific action is required to 'stop'
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void SinkStopL();
        
        /**
        * From MDataSink. Used for custom command receiving.
        * @since    Series 60 3.0
        * @param    aMessage    The message specifying the custom command.
        * @return   void
        */
        void SinkCustomCommand( TMMFMessage& aMessage );
        

    
    public: // From MMccRtpInterface
    
            
        /**
        * From CRtpInterface Sends media signals (RTCP)
        * @since Series 60 3.0
        * @param aSignal Signal to be sent
        * @return void
        */
        virtual void SendMediaSignallingL( const TMccEvent& aEvent );
        
        /**
        * Sends Non-RTCP data
        * @param aData - [input] Data to send
        * @return One of the standard system-wide error codes.
        */
        virtual void SendRTCPDataL( const TDesC8& aData );
        
        virtual TBool HandleBySsrc( const TRtpSSRC& aSsrc );

        TRtpId RtpStreamId();
    
        virtual TBool IsSink() const;
        
        virtual TBool IsSending() const;
        
        void DoCreateSrtpStreamL();
              

    public: // From MMccRtpDataSink
    
        /**
        * From MMccRtpDataSink. Empties specified buffer.
        *
        * @since    Series 60 3.0
        * @param    aBuffer     Buffer containing the RTP packet
        * @param    aSupplier   Data source to notify when buffer is emptied
        * @param    aMediaId    Identifies the media type of a particular stream
        * @param    aHeaderInfo RTP packet header
        * @return   void
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, 
                           MDataSource* aSupplier,
                           TMediaId aMediaId,
                           TRtpSendHeader& aHeaderInfo );
                           
                           
    protected:  // Functions from base classes
        
        /**
        * Method to perform any sink construction dependant on the sink construction
        * initialisation data aInitData
        *
        * This is a pure virtual function that a derrived data sink must implement
        *
        * @param    "aInitData"
        *            Sink specific initialisation data required for sink construction
        */
        void ConstructSinkL( const TDesC8& aInitData );

    protected:  // From CMccRtpInterface
    
        /**
        * From CMccRtpInterface
        */
        void DoCreateStreamL();
        
    protected: // From MMccRtpSenderObserver
    
        void SendErrorOccured( TInt aError );
        
    private:

        /**
        * C++ default constructor.
        */
        CMccRtpDataSink();
        
        void SendStreamEventToClient( TMccEventType aEventType, 
                                      TInt aError = KErrNone );
                                                           
        TMccRtpUser* FindRtpUserEntryForCurrent( MAsyncEventHandler& aEventHandler );
        
        void RemoveCurrentRtpUser();
        
        static TBool RtpUserMatch( const TMccRtpUser& aUser1, 
                                   const TMccRtpUser& aUser2 );
                                   
        TBool SendingAllowed( const TRtpSendHeader& aHeaderInfo ) const;
                                      
     private: // From MSRTPReKeyingObserver
    
        /**
         * FROM SRTP API
         *
         * This function is called by SRTP when a master key is stale and
         * needs to be refreshed. 
         */
        void SRTPMasterKeyStaleEvent(const CSRTPStream& aStream);
        
        /**
         *  FROM SRTP API
         *
         * This function is called by SRTP Stream initiated with 
         * CSRTPSession  when a master key is stale and
         * needs to be refreshed.
         */ 
        void SRTPMasterKeyStaleEvent(const CSRTPSession& aSession );
            
    private:    // Data

        // Pointer to buffer, which datapath has requested to be emptied
        CMMFDataBuffer* iBufferToBeEmptied;     // Uses

        // Pointer to datasource for notify when buffer is emptied
        MDataSource* iDataSource;               // Uses
        
        // Synchronization source identifier.
        TRtpSSRC iSSRC;
        
        // Stream ID for media packets
        TRtpId iRtpStreamId;
               
        // Registered users for sink
        RArray<TMccRtpUser> iUsers;
        
        // flag for secure key expired
        TBool iSecureKeyExpired;

        // flag indicating is mute enabled
        TBool iMuteOn;
        
        // payload to sent regardless of mute state
        TUint8 iExceptionPt;

        CMccRtpSender* iSender;
        
        TMediaId iMedia;
        
#ifdef FTD_ENABLED
        
        // Message queue carrying stream statistics
        RMsgQueue<TMccStreamStats> iStreamStatsQueue;
        
        // Number of packets sent
        TUint32 iPacketsSent;
        
#endif

    #ifdef TEST_EUNIT
    public:
        friend class UT_CMccRtpDataSink;
    #endif
    };

#endif        // MCCRTPDATASINK_H

// End of File 
