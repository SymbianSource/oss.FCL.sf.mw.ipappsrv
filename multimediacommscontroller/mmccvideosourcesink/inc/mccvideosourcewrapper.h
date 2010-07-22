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



//  INCLUDES
#ifndef MCCVIDEOSOURCEWRAPPER_H
#define MCCVIDEOSOURCEWRAPPER_H

#include <e32base.h>
#include <f32file.h>
#include <e32std.h>
#include <mmf/server/mmfdatasource.h>
#include <mmf/server/mmfdatasink.h>
#include <3gplibrary/mp4lib.h>
#include <CCMRMediaSink.h>
#include <CCMRMediaRecorder.h>
#include <CCMRMediaRecorderObserver.h>
#include <mmf/devvideo/devvideobase.h>

#include "mmcccodecinformation.h"

class TMccCodecInfo;

const TInt KIPULRandomAccessPeriod = 2; //Time in seconds between intra frames

// CLASS DECLARATION

/**
*  
*/
class CMccVideoSourceWrapper: public CBase, public MCMRMediaSink, public MCMRMediaRecorderObserver
    {

    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        */
        static CMccVideoSourceWrapper* NewL(
            MCMRMediaSink& aObserver, 
            MCMRMediaRecorderObserver& aObserver2 );
                                        		
		virtual ~CMccVideoSourceWrapper();

    public: // new
            
        void OpenL( MDataSource* aAudioSource,
                    TInt aCameraHandle,
                    const TDesC8& aVideoMimeType = KNullDesC8,
                    TFourCC aAudioType = KFourCCNULL,
                    TUid aEncoderUid = KNullUid,
                    TBool aForceEncapsulationType = EFalse );

        void PlayL();
        
        void ResumeL();
        
        void PauseL();
        
        void StopL();

	    void SetVideoBitrateL( TUint aVideoBitRate );
	    
	    void SetVideoFrameRateL( TReal32 aFramesPerSecond );

        TUint GetVideoBitrateL();
        
        CCMRMediaRecorder::TRecorderState State();
        
        void SetVideoRateControlOptionsL( const TRateControlOptions& aOptions );

        void GetVideoRateControlOptionsL( TRateControlOptions& aOptions );
        
    public: // from MCMRMediaSink

        /**
        * Write a buffer to sink
        * @since 2.1
        * @param CCMRMediaBuffer* aBuffer   The buffer to write
        * @return TInt                      Error code
        */
        virtual void WriteBufferL(CCMRMediaBuffer* aBuffer);

        /**
        * Inform video frame size to sink
        * @since 2.1
        * @param TSize aSize    Video frame size
        * @return TInt          Error code
        */
        virtual TInt SetVideoFrameSize(TSize aSize);

        /**
        * Inform average video bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Average video bitrate
        * @return TInt          Error code
        */
        virtual TInt SetAverageVideoBitRate(TInt aBitRate);

        /**
        * Inform maximum video bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Maximum expected video bitrate
        * @return TInt          Error code
        */
        virtual TInt SetMaxVideoBitRate(TInt aBitRate);

        /**
        * Inform average audio bitrate to sink
        * @since 2.1
        * @param TInt aBitRate  Average audio bitrate
        * @return TInt          Error code
        */
        virtual TInt SetAverageAudioBitRate(TInt aBitRate);

    public: // from MCMRMediaRecorderObserver

        /**
        * Informs the user that PrepareL() has been completed with or
        * without errors. If aError != KErrNone, media recorder remains in 
        * open state
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroPrepareComplete(TInt aError);    

        /**
        * Informs the user about a state change. 
        * Useful to find out when a new prepare is needed after 
        * a setting was changed
        * @since 2.1
        * @param aState     New state (CCMRMediaRecorder - TRecorderState)
        * @param aError     Error code
        * @return void
        */
        virtual void MmroStateChange(TInt aState, TInt aError);

        /**
        * Informs the user about a temporary runtime error. 
        * Recording can't continue right now but it may
        * be possible to continue later (e.g. audio HW was 
        * taken by a higher priority application)
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroTemporaryError(TInt aError);    

        /**
        * Informs the user about a fatal runtime error (e.g. OOM). 
        * MediaRecorder must be deleted and recreated.
        * @since 2.1
        * @param aError     Error code
        * @return void
        */
        virtual void MmroFatalError(TInt aError);

    private:

		void ConstructL();

		CMccVideoSourceWrapper(
            MCMRMediaSink& aObserver, 
            MCMRMediaRecorderObserver& aObserver2 );

    private:
    
		CCMRMediaRecorder* iMediaRecorder;
        MCMRMediaSink& iObserver;
        MCMRMediaRecorderObserver& iObserver2;
        
        TBool iFatalErrorOccured;
        		
	private:
		#ifdef TEST_EUNIT
			friend class UT_CMccVideoSourceImpl;	
   		#endif
	};


#endif 


// End of file
