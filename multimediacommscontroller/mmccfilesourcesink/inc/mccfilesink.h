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



#ifndef MCCFILESINK_H
#define MCCFILESINK_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>
#include <e32std.h>
#include <3gplibrary/mp4lib.h>
#include <CamC3GPDataSink.h>
#include <CCMRMediaSink.h>
#include "mccdatasink.h"
#include "mmccevents.h"
#include "mccinternalcodecs.h"


/**
* Class to contain filesink user information.
*/   
 class TMccFileSinkUser
    {
    public:
    
        TMccFileSinkUser( MAsyncEventHandler* aEventHandler ) : 
            iEventHandler( aEventHandler ),
            iCodecInfo()
            {
            }
            
    public:
    
        MAsyncEventHandler* iEventHandler;
        TMccCodecInfo iCodecInfo;
    };
    
/**
* Class to handle file writing.
*/
class CMccFileSink: public CMccDataSink, public M3GPDataSinkObserver
    {

    public:
       
	    enum TFileSinkState 
	        {
	        EConstructed = 0,
	        EReady,           
	        EPaused,  
	        EStopped, 
	        ERecording 
	        };  
	        
        /**
        * Two-phased constructor.
        */
        static MDataSink* NewSinkL( TUid aImplementationUid, 
                                        const TDesC8& aInitData );
                                        
		void ConstructSinkL ( const TDesC8& aInitData );
		
		CMccFileSink();
		
		virtual ~CMccFileSink();
	
    public: // New

        virtual void SetCurrentUser( MAsyncEventHandler* aEventHandler );
		virtual void SetFileNameL( const TFileName aFileName );
		virtual void RecordTimeAvailableL( TTimeIntervalMicroSeconds& aTime );

    public: // From MDataSource

        TFourCC SinkDataTypeCode( TMediaId /*aMediaId*/ );

        TInt SetSinkDataTypeCode( TFourCC aCodec, 
                                    TMediaId aMedia );

        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        TBool CanCreateSinkBuffer();
        
        CMMFBuffer* CreateSinkBufferL( TMediaId, 
                                         TBool& aReference );
        
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );
        
        void SinkThreadLogoff();

        void SinkPrimeL();
        
        void SinkPlayL();
        
        void SinkPauseL();
        
        void SinkStopL();

        void EmptyBufferL( CMMFBuffer* aBuffer,
                          MDataSource* aProvider,
                          TMediaId aMediaId );
                          
        void BufferFilledL( CMMFBuffer* aBuffer );
                          
	public:	// from M3GPDataSinkObserver

		void MfcoDiskFullL();

		void MfcoSizeLimitReachedL();

    public: // New methods
    
        virtual void SetVideoCodecL( const TMccCodecInfo& aVideoCodec );

        virtual void SetAudioCodecL( const TMccCodecInfo& aAudioCodec );
        
    private:
    
		void SendStreamEventToClient( TMccEventType aEventType, 
		                              TInt aError = KErrNone,
		                              TBool aToAllClients = EFalse );
		                              
		TTimeIntervalMicroSeconds TimeToPlayL( 
		    TTimeIntervalMicroSeconds aCurrentTimestamp );
		
		void CalculateAverageTimestampDifferenceL( 
		    const TTimeIntervalMicroSeconds& aCurrentTimestamp );
		
        void ResetTimers();
        
        void SetPausedDuration( TTime aT1, TTime aT2 );
        
        void AutomaticPauseL();
        
        TInt FinalizeSendEvent( MAsyncEventHandler* aEventHandler, TMccEvent& aEvent );
        
        HBufC8* GetCodecTypeStringLC( const TMccCodecInfo& aCodecInfo );
        
        /**
        * Resolve buffer type of passed buffer based on datatype value and
        * contents of data in some cases.
        * @param aBuffer
        * @param aDataType
        * @return buffer type
        */
        CCMRMediaBuffer::TBufferType ResolveBufferType( CMMFDataBuffer& aBuffer,
                                                        TFourCC aDataType );
 
        /**
        * Checks whether buffer should be written to file.
        * @param aBuffer
        * @param aBufferType
        * @return ETrue if writing to file is allowed 
        */
        TBool CheckWritingPermission( CMMFDataBuffer& aBuffer, 
                                      const CCMRMediaBuffer::TBufferType& aBufferType );
        
        TBool SetStateL( TFileSinkState aState );
        
        void DoSinkPrimeL( TBool aSendEvent = ETrue );
        
        void DoSinkPlayL( TBool aSendEvent = ETrue );
        
        void DoSinkPauseL( TBool aSendEvent = ETrue );
        
        void DoSinkStopL( TBool aSendEvent = ETrue );
        
        void DoCodecChangeL();
        
        void AddUserL( MAsyncEventHandler* aEventHandler );
        
        TMccFileSinkUser& ActiveUserL();
        
        TBool IsActiveUser( MAsyncEventHandler* aUser );
        
        /**
        * Check if active user has changed and update sink accordingly.
        * @param aMediaId
        * @param aDataProvider
        * @return data type of current data provider
        */
        TFourCC UpdateActiveUserL( TMediaId aMediaId, MDataSource& aDataProvider );
        
        void SetActiveUserIndex( TInt aIndex );
        
    private:   
		
	    TFileSinkState iCurrentState;
		CCamC3GPDataSink* iFileComposer;
    	TInt iMaxFileSize;
    	
		TFileName iFileName; 
		TFourCC iAudioFourCC;
		
	    MAsyncEventHandler* iAsyncEventHandler;
	
	private:
	   	TTime iT1;
	   	TTime iT2;
	   	TTimeIntervalMicroSeconds iPausedDuration;
	   	
	   	RArray<TInt64> iTimestamps;
	   	TTimeIntervalMicroSeconds iPreviousTimestamp;
	   	TInt64 iAddToTimestamp;
	   	
	   	TBool iSizeLimitReached;
	   	TBool iNotifySizeLimitReached;
	   	
	   	TMccCodecInfo iVideoCodec;
	   	
	   	RPointerArray<TMccFileSinkUser> iUsers;
	   	
	   	TInt iActiveUserIndex;
	   	
	   	TBool iDecSpecInfoProvided;
	   	TBool iKeyFrameProvided;
	   	
	   	TInt64 iCurrAvgTimestampDifference;

	#ifdef EUNIT_TEST
        friend class UT_CMccFileSink;
    #endif	
        };


#endif 


// End of file

