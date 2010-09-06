/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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


//  INCLUDES
#ifndef MCCMSRPSOURCE_H
#define MCCMSRPSOURCE_H

#include <e32std.h>
#include "mccdatasource.h"
#include "mmccevents.h"

#include "mccdatareceiverobserver.h"
#include "mccmsrpinterface.h"
#include "mccmsrpdataobserver.h"

// FORWARD DECLARATIONS

/**
*  
*/
class CMccMsrpSource: public CMccDataSource,  public TMccMsrpSessionParams, public MMsrpDataObserver, public MMccDataReceiverObserver
    {

    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        */
        static MDataSource* NewSourceL( TUid aImplementationUid, 
                                        const TDesC8& aInitData );
                                        
		void ConstructSourceL( const TDesC8& aInitData );
		
		~CMccMsrpSource();

    public: // From MDataSource

        /**
        * From MDataSource. Returns the data type as a fourCC code of the data source.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        * @return    The fourCC code identifying the source datatype for the specified aMediaId.
        */
        TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ );

        /**
        * From MDataSource. Sets the data type as a fourCC code for the data source
        *
        * @since    Series 60 3.0
        * @param    "aSourceFourCC"
        *            This specifies the data type as a fourCC code to set the source to.
        *
        * @param    "aMediaId"
        *            This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        *
        * @return    KErrNone if successful, KErrNotSupported if the source does not support having
        *            it's data type set, otherwise a system wide error code.
        */
        TInt SetSourceDataTypeCode( TFourCC aCodec, 
                                    TMediaId aMedia );

        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *            The buffer that has been emptied by a data sink and is now available for reuse
        * @return   None
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
        * From MDataSource. Method to indicate whether the data source can create a buffer.
        *
        * @since    Series 60 3.0
        * 
        * @return    EFalse
        */
        TBool CanCreateSourceBuffer();
        
        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        *
        * @param    "aReference"
        *           This must be written to by the method to indicate whether the created buffer is
        *           a 'reference' buffer.  A 'reference' buffer is a buffer that is owned by the source
        *           and should be used in preference to the sink buffer provided the sink buffer
        *           is also not a reference buffer.
        * .  
        * @return   The created buffer
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId, 
                                         TBool& aReference );
        
        /**
        * From MDataSource. Method to 'logon' the data source to the same thread that source will be supplying data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are created here.
        * 
        * @param    "aEventHandler"
        *           This is an MAsyncEventHandler to handle asynchronous events that occur during the
        *           transfer of multimedia data.  The event handler must be in the same thread as the data transfer
        *           thread - hence the reason it is passed in the SourceThreadLogon as opposed to say the constructor.
        *
        *
        * @return   KErrNone if successful, otherwise a system wide error code.
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
        
        /**
        * From MDataSource. Method to 'logoff' the data source from the same thread that source supplies data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are deleted. Datasource deletes the 
        * stream(s) and unregisters itself as an observer for the RTP session for incoming RTP and RTCP packets.
        *
        * @return None
        */
        void SourceThreadLogoff();

        /**
        * From MDataSource. Method to 'prime' the data source.
        *
        * Datasource is set to primed state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePrimeL();
        
        /**
        * From MDataSource. Method to 'play' the data source.
        *
        * Downlink timeout timer is started and datasource is set to playing state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePlayL();
        
        /**
        * From MDataSource. Method to 'pause' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to paused state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePauseL();
        
        /**
        * From MDataSource. Method to 'stop' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to stopped state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourceStopL();

        /**
        * From MDataSource. Method called by a MDataSink to request the data source to fill aBuffer with data.
        *
        * Datasource gets packet from jitterbuffer and forwards it to the payloadformatdecoder
        * for further processing. Datasource works synchronously; consumers BufferFilledL
        * is called when RTP packet is got succesfully from jitterbuffer.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *           The buffer that needs filling with data
        *
        * @param    "aConsumer" 
        *           The data sink that consumes the data. The data source needs this to make the BufferFilledL
        *           callback on aConsumer when the data source has completed filling the aBuffer.
        *
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        * @return   None
        */
        void FillBufferL( CMMFBuffer* aBuffer,
                          MDataSink* aConsumer,
                          TMediaId aMediaId );
                          
        /**
        * From MDataSource
        */
        void NegotiateSourceL( MDataSink& aDataSink );	
        
    public:
        
        /**
         * A MSRP message has been received from the network.
         * This function is called when the MSRP message was received from the network
         * the ownership is transferred
         * @param aMessage the received message
         * @param aStatus, KErrNone with complete message, KErrCancel if terminated
         */
        void IncomingMessage( HBufC8* aMessageContent, TInt aStatus );

        /**
         * A MSRP REPORT message has been received from the network
         * This function is called to indicate to the client that a recipient
         * wishes to indicate a range of bytes as received. Alternative the
         * recipient might use REPORT to indicate an error. In case of error the
         * MSRP stops sending the message. However, it is client's responsibility
         * how the handle the information
         * @param aStatus status code from the REPORT message
         * @param aRangeStart start of byte range received
         * @param aRangeEnd end of byte range received
         * @param aTotal Total number of bytes in the message
         */
        void IncomingReport( TInt aStatus, TInt aRangeStart,
            TInt aRangeEnd, TInt aTotal );

        /**
         * The SendMessage status of the message sending
         * The possible status code are defined by CMSRPConnection::TMSRPErrorCodes
         * or system wide error codes
         * @param aStatus status of the operation
         * @param aMessageId message identifier of the sent message
         */
        void SendResult( TInt aStatus, const TDesC8& aMessageid );

        /**
         * If listen port request fails this is called
         * @param aStatus error code of the operation
         */
        void ListenPortFailure( TInt aStatus );

        
        //Connection Status
        TInt iConnectStatus;

    public:
    			
		void DataReceivedL( const TDesC8& aData );
		
        void SetSessionParamsL( const TMccMsrpSessionParams& aParams );
        
        /**
         * @since   Series 60 3.0
         *
         * @param   aMessageContent     MSRP Payload
         * @return  void
         */
        void PlayoutMsrpPacketL( HBufC8* aMessageContent );
        
    private:
    	
		CMccMsrpSource();
		
		void SendStreamEventToClient( TMccEventType aEventType, 
		                              TInt aError = KErrNone );

    private:

        // The client who requested "FillBuffer"
        MDataSink* iFillBufferRequester;
    
        MAsyncEventHandler* iAsyncEventHandler;
	    TBool iPaused;
	    TFourCC iFourCC;


    	CMMFDataBuffer* iBufferToBeFilled;
        MDataSink* iConsumer;

	#ifdef EUNIT_TEST
        friend class UT_CMccMsrpSource;
    #endif	
	
	};


#endif 


// End of file
