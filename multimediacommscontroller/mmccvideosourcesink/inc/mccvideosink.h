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





#ifndef MCCVIDEOSINK_H
#define MCCVIDEOSINK_H

//  INCLUDES


#include <e32std.h>
#include <in_sock.h>
#include <es_sock.h>
#include <mmf/common/mmfcontrollerframeworkbase.h>
#include <videoplayer.h>
#include <mmf/server/mmfbuffer.h>
#include "rtpdef.h"
#include "mccdatasink.h"



// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MDataSource;
class MVideoPlayerUtilityObserver;
class CMccVideoSinkImpl;
class CMMFDataBuffer;
class TMccCodecInfo;

// CLASS DECLARATION
    
/**
*  
*
*  @lib 
*  @since 
*/
class CMccVideoSink : public CMccDataSink
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
        virtual ~CMccVideoSink();
        


    public:    // New functions
    
        void EmptyVideoSinkBufferL( CMMFBuffer* aBuffer, 
	                       MDataSource* aSupplier,
	                       TMediaId aMediaId,
	                       TRtpRecvHeader& aHeaderInfo );
      
        virtual void SetVideoCodecL( const TMccCodecInfo& aVideoCodec );       
        
        virtual void SetAudioCodecL( const TMccCodecInfo& aAudioCodec );  
        
        virtual void SetCurrentUser( MAsyncEventHandler* aEventHandler );
       
    public :   // New functions From MDataSink
        
     
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
        * From MDataSink. 
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void SetSinkPrioritySettings( const TMMFPrioritySettings& aPrioritySettings );
        
        /**
        * From MDataSink. Used for custom command receiving.
        * @since    Series 60 3.0
        * @param    aMessage    The message specifying the custom command.
        * @return   void
        */
        void SinkCustomCommand( TMMFMessage& aMessage );
        
    public: // from MMccSourceSink 
    
        virtual void GetParameterL( TUint32 aParam, TDes8& aVal );
        
        virtual void SetParameterL( TUint32 aParam, const TDesC8& aVal );
        
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
        
    private:

        /**
        * C++ default constructor.
        */
        CMccVideoSink();
        
        
    private: 
        
        CMccVideoSinkImpl* iMediaInterface;
        
		TFourCC iVideoFourCC;
		TFourCC iAudioFourCC;		
				
		TUint8  iVideoPayloadType;
		TUint   iMaxVideoBitRate;	
        TUint   iVideoCodecMode;
        
        TUint8  iAudioPayloadType;	
        TUint   iAudioCodecMode;
        
        TBool   iAudioReady;
        TBool   iVideoReady;
	    	
	private:
    	#ifdef TEST_EUNIT
			friend class UT_CMccVideoSink;	
   		#endif
        
    };

#endif
