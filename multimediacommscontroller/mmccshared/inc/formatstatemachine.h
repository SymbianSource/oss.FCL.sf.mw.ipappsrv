/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides statemachine definitions for MCC Payload formatters
*
*/




#ifndef FORMATSTATEMACHINE_H
#define FORMATSTATEMACHINE_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <mmf/server/mmfformat.h>

// DATA TYPES
// Payload format encoding states.
enum TFormatEncodeState
    {
    EEncodeIdle             = 0,
    EEmptySourceBuffer      = 1,
    ESourceBufferEmptied    = 2
    };

// Payload format decoding states
enum TFormatDecodeState
    {
    EDecodeIdle         = 0,
    ESourceDataReady    = 1,
    EEmptyDataToSink    = 2
    };

// CLASS DECLARATION

class TRtpRecvHeader;
class CMccRtpMediaClock;

/**
*  Format decode statemachine observer
*  
*  @lib     Mccamrplformat.dll
*  @since   Series 60 3.0
*/
class MPayloadFormatRead
    {
    public:
        
        /**
        * Fill sinkbuffer state change
        * @since    Series 60 3.0
        */
        virtual void FillSinkBufferL() = 0;
        
        /**
        * Send data to sink state change
        * @since    Series 60 3.0
        */
        virtual void SendDataToSinkL() = 0;

    };

/**
*  Format decode statemachine observer
*  
*  @lib     Mccamrplformat.dll
*  @since   Series 60 3.0
*/
class CPayloadFormatRead: public CMMFFormatDecode
    {
    public:
        
        /**
        * Called after the data buffer is filled. Update the number of bytes read
        * and the current read position for the next read operation. 
        *
        * @param    aBuffer         data buffer filled
        * @param    aHeaderInfo     RTP packet header information
        * @return None
        */
        virtual void DataBufferFilledL( CMMFBuffer* aBuffer, 
                                    const TRtpRecvHeader& aHeaderInfo ) = 0;
    
        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams ) = 0;
        
        /**
        * Sets maxpayloadsize
        * @param    aTimeOut      Timeout value to set.
        * @return	None
        */
		virtual void SetPayloadSize( TInt /*aMaxPayloadSize*/ )
		    {
		    }
    };

/**
*  Format encode statemachine observer
*  
*  @lib     Mccamrplformat.dll
*  @since   Series 60 3.0
*/
class MPayloadFormatWrite
    {
    public:
    
        /**
        * Empty sourcebuffer state change
        * @since    Series 60 3.0
        */
        virtual void EmptySourceBufferL() = 0;
        
        /**
        * Sourcebuffer emptied state change
        * @since    Series 60 3.0
        */
        virtual void SourceBufferEmptiedL() = 0;
    };

/**
*  Format encode statemachine observer
*  
*  @lib     Mccamrplformat.dll
*  @since   Series 60 3.0
*/
class CPayloadFormatWrite: public CMMFFormatEncode 
    {
    public:
    
        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams, 
                                              CMccRtpMediaClock& aClock ) = 0;
    };
    
/**
*  Format encoding state machine
*
*  @lib     Mccamrplformat.dll
*  @since   Series 60 3.0
*/
class CFormatEncodeStateMachine : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CFormatEncodeStateMachine* NewL( MPayloadFormatWrite* aClient );

        /**
        * Destructor.
        */
        virtual ~CFormatEncodeStateMachine() ;

    public: // New functions

        /**
        * Change state to the new state.
        * @since    Series 60 3.0
        * @param    aNewState State where to proceed to
        */
        void ChangeState( enum TFormatEncodeState aNewState );

        /**
        * Returns current state.
        * @since    Series 60 3.0
        * @return   TFormatEncodeState - Current encoding state
        */
        TFormatEncodeState CurrentState() const;

    private: // Functions from base classes
    
        /**
        * From CActive Handles an active object’s request completion event.
        * @since    Series 60 3.0
        */
        void RunL();

        /**
        * From CActive Implements cancellation of an outstanding request.
        * @since    Series 60 3.0
        */
        void DoCancel();
        
        /**
        * From CActive Implements RunL leave error handling
        * @since    Series 60 3.0
        * @param    aError The leave error code from RunL()
        * @return   TInt System wide error code
        */
        TInt RunError( TInt aError );

    private:
   
        /**
        * C++ default constructor.
        */
        CFormatEncodeStateMachine( MPayloadFormatWrite* aClient );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data
        
        // Client who uses this state machine
        MPayloadFormatWrite* iClient;
        
        // Current format encoding state
        TFormatEncodeState iState;
    };

/**
*  Format decoding state machine
*
*  @lib     Mccamrplformat.dll
*  @since   Series 60 3.0
*/
class CFormatDecodeStateMachine : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CFormatDecodeStateMachine* NewL( MPayloadFormatRead* aClient );

        /**
        * Destructor.
        */
        virtual ~CFormatDecodeStateMachine() ;

    public: // New functions
  
        /**
        * Change state to the new state.
        * @since    Series 60 3.0
        * @param    aNewState State where to proceed to
        */
        void ChangeState( enum TFormatDecodeState aNewState );

        /**
        * Returns current state.
        * @since    Series 60 3.0
        * @return   TFormatDecodeState - Current decoding state
        */
        TFormatDecodeState CurrentState() const;

    private: // Functions from base classes

        /**
        * From CActive Handles an active object’s request completion event.
        * @since    Series 60 3.0
        */
        void RunL();

        /**
        * From CActive Implements cancellation of an outstanding request.
        * @since    Series 60 3.0
        */
        void DoCancel();
        
        /**
        * From CActive Implements RunL leave error handling
        * @since    Series 60 3.0
        * @param    aError The leave error code from RunL()
        * @return   TInt System wide error code
        */
        TInt RunError( TInt aError );

    private:

        /**
        * C++ default constructor.
        */
        CFormatDecodeStateMachine( MPayloadFormatRead* aClient );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data
        
        // Client who uses this state machine
        MPayloadFormatRead* iClient;
        
        // Current format decoding state
        TFormatDecodeState iState;
    };

#endif /* FORMATSTATEMACHINE_H */


