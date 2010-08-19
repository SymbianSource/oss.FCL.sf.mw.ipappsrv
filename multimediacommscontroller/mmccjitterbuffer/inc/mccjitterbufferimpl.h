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
* Description:    Jitter buffer for Audio frames.
*  Interface   : 
*
*/




#ifndef MCCJITTERBUFFERIMPL_H
#define MCCJITTERBUFFERIMPL_H

//  INCLUDES
#include <e32base.h>
#include "mccinternalcodecs.h"
#include "mmccevents.h"

// FORWARD DECLARATIONS
class CMccCnGenerator;
class CMMFDevSound;
class TMccJitterBufferEventData;
class MAsyncEventHandler;
class MDataSink;
class MJitterBufferObserver;

// CLASS DECLARATION

/**
*  Jitter buffer element including packet received.
*
*  @lib MMccJitterBuffer.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( TJitterBufferElement )
	{
public: // Constructor
    inline TJitterBufferElement() : iDataFrame( NULL ), 
        iSequenceNumber( -1 ), iStamp( -1 )
        {   };
        
public: // Data members
    // Data
    CMMFDataBuffer* iDataFrame;
    
    // Data packet number
    TInt64 iSequenceNumber;

	TInt64 iStamp;
	
    /**
	* Compares two elements based on their sequence number
	* @since Series 60 3.0
	* @param aElem1 First element
	* @param aElem2 Second element
	* @return TInt
	*/
	static TInt CompareSeqNum( const TJitterBufferElement& aElem1, 
	        const TJitterBufferElement& aElem2 );
	        
    /**
	* Compares two elements based on their stamps
	* @since Series 60 3.0
	* @param aElem1 First element
	* @param aElem2 Second element
	* @return TInt
	*/
	static TInt CompareStamp( const TJitterBufferElement& aElem1, 
	        const TJitterBufferElement& aElem2 );
	};

/**
*  Jitter buffer.
*
*  @lib MMccJitterBuffer.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CMccJitterBufferImpl ) : public CBase
    {
public: // Enumerations

    enum TMccJitterBufferImplState
        {
        EUnknown,
        EIdle,
        EPlaying,
        EBuffering,
        EDetermining
        };
        
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    */
    static CMccJitterBufferImpl* NewL( MJitterBufferObserver* aObserver = NULL );

    /**
    * Destructor.
    */
    virtual ~CMccJitterBufferImpl();

public: // Methods

	/**
    * Setup jitterbuffer
    * @since Series 60 3.0
    * @param parameters
    * @return void
    */    
    void SetupL( TInt aPlayoutThreshold, 
                 const TMccCodecInfo& aCInfo, 
                 CMMFDevSound& aDevSound,
                 MAsyncEventHandler* aEventHandler,
                 TUint32 aEndpointId );

    /**
    * Reset buffer 
    * @since Series 60 3.0
    * @param aPlayTone ETrue, if tone is to played
    * @param aNotifyUser ETrue, if state change should be notified
    * @return void
    */
	void ResetBuffer( TBool aPlayTone = ETrue, TBool aNotifyUser = ETrue );

    /**
    * Add DataFrame to buffer
    * @since Series 60 3.0
    * @param aBuffer Buffer to read data from
    * @return void
    */
	void AddDataFrameL( CMMFBuffer* aDataBuffer );
	
    /**
    * Get DataFrame from buffer 
    * @since Series 60 3.0
    * @param aBuffer Buffer to write data to
    * @return void
    */	
	void GetDataFrameL( CMMFBuffer* aBuffer );
	
    /**
    * Get Current Codec 
    * @since Series 60 3.0
    * @param None
    * @return TFourCC
    */
	TFourCC CurrentCodec() const;
	
    /**
    * Delay Up 
    * @since Series 60 3.0
    * @param None
    * @return void
    */
	void DelayUpL();
	
    /**
    * Decreases jitter buffer delay
    * @since Series 60 3.0
    * @param None
    * @return void
    */
	void DelayDownL();
	
    /**
    * ?Description 
    * @since Series 60 3.0
    * @param None
    * @return TTimeIntervalMicroSeconds32
    */
	TTimeIntervalMicroSeconds32 CalculateDelay() const;

    /**
    * Get the length of jitter buffer, in terms of number of packets.
    * @since Series 60 3.0
    * @param none
    * @return buffer lenght.
    */
    TInt BufferLength() const;
    
    /**
    * Fills the given event with statistics of the jitterbuffer
    * @since Series 60 3.0
    * @param aEvent Event into which set the statistics
    * @return void
    */
    void GenerateStatistics( TMccJitterBufferEventData& aEvent ) const;

private:

    /**
    * C++ default constructor.
    */
    CMccJitterBufferImpl( MJitterBufferObserver* aObserver );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // New functions  

    /**
    * Is sequence number wrapped around TUint16 65535->0
    * @since Series 60 3.0
    * @param TInt aSeqNum1 is oldest packet sequence numper in buffer
	* @param TInt aSeqNum2 is new element sequence numper to buffer
    * @return detect the wrap-around condition.
    */   
    TBool IsSeqNumWrappedAround( TInt64 aSeqNum1, TInt64 aSeqNum2 ) const;
    
    /**
    * Find Largest SequenceNumber 
    * @since Series 60 3.0
    * @param None
    * @return TInt Largest sequence number
    */
	TInt FindLargestSeqNum() const;
	
	/**
    * Inserts a databuffer into the jitter buffer
    * @since Series 60 3.0
    * @param aBuffer Buffer containing the data
    * @param aLargestSeqNum Largest sequence number returned by 
    *                       FindLargestSeqNum()
    * @param aFrameNumber   Sequence number of frame to be inserted.
    * @return void
    */	
	void InsertBufferElement( const TDesC8& aBuffer, TInt64 aLargestSeqNum,
	    TInt64 aFrameNumber );
	
	/**
    * Is jitter buffer full?
    * @since Series 60 3.0
    * @param none
    * @return True if full.
    */
    TBool IsFull() const;

    /**
    * Is jitter buffer empty?
    * @since Series 60 3.0
    * @param none
    * @return True if empty.
    */
    TBool IsEmpty() const;
    
    void HandleNotifications( TBool aBuffering );
    
    void CheckThresholdBufferLength( TInt& aBufferLength, const TInt aTreshhold ) const;
    
    void SendStreamEventToClient( TMccEventType aEventType, 
                                  TBool aAllEndpoints = EFalse );
                                  
    void SetBufferState( TMccJitterBufferImplState aState );
    
    TMccJitterBufferImplState BufferState() const;
    
private: // Data
    
	// Playout thresholds for storing the original threshold and current
	// threshold
	TInt iCurrentPlayThreshold;
	TInt iOriginalPlayThreshold;
	
	// Number of packets in buffer
	TInt iPacketsInBuffer;
	
	// Jitterbuffer RArray.
	RArray<TJitterBufferElement> iBuffer;
	
	// instance of comfortnoise generator.
	CMccCnGenerator* iCnGenerator;

	// Sequence number of packet that is played out last time
	TInt64 iLastGetSeqNum;
    
    // Function object for jitterbuffer sorting
    TLinearOrder<TJitterBufferElement> iBufStampSorter;
    
    // Function object for jitterbuffer sorting
    TLinearOrder<TJitterBufferElement> iBufSequenceSorter;
    
    // Number of frames arrived late
    TUint iNumOfLateFrames;

    // Count of sequential late frames
    TUint iNumOfSequentialLateFrames;
    
    // Number of frames totally lost
    TUint iFramesLost;
  
    // Number of frames received
    TUint iFramesReceived;
    
    // Number of frames removed by adaptation or because of overflows
    TUint iFramesRemoved;
    
    // Number of frames played
    TUint iFramesPlayed;
    
    // Codec information
    TMccCodecInfo iCInfo;
    
    // Codec frame size
    TInt iFrameSize;
    
    // Codec frame time
    TInt iFrameTime;
    
    // Increment in frame sequence number
    TUint iSeqNumIncrement;
    
    TBool iPlay;
    
    TInt iInactivityTime;

    MJitterBufferObserver* iObserver;

    TUint iOverflowCounter;

	TInt iPlayToneInterval;
    TTime iTonePlayTime;
    
    MAsyncEventHandler* iEventHandler;
    
    // Endpoint identifier
    TUint32 iEndpointId;

    TMccEvent iMccEvent;
    
    TBool iNotifyUser;
    
    TMccJitterBufferImplState iBufferState;
    
    TMccEventType iLatestNotifiedEvent;
    
    TInt iSampleRate;
    
    /**
     * Flag to determine whether frame removal or frame drop should be done in
     * overflow situation.
     */
    TBool iDropNextFrame;
    
    // Saved bufLenMultiplier from SetupL()
    TInt iBufLenMultiplier;
    
    private:
        #ifdef TEST_EUNIT
            friend class UT_CMccJitterBufferImpl;
        #endif   
    };

#endif  //MCCJITTERBUFFERIMPL_H

// End of File

