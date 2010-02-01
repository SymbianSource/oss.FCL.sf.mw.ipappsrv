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



#ifndef MCCSCREEN_H
#define MCCSCREEN_H

#include <w32std.h>

class CCamera;
class MCameraDirectViewFinder;

/**
* Screen for viewfinder, may use bitmap viewfinder or direct viewfinder
* depending on supported camera vf modes.
* 
*/
class CMccScreen : public CBase
    {

    public:
        
        enum TMccScreenType
            {
            EBitmapScreen,
            EDirectScreen
            };
    
    public:

        static CMccScreen* NewL( CCamera* aCamera,
                                 TPoint aPos, 
                                 TSize aArea, 
                                 TInt aIndex, 
                                 TInt aWindowOrdinalPosition,
                                 TInt aWindowOrdinalPriority );

        ~CMccScreen();
        
        
        
    public:

        virtual void StartL();
        
        virtual void Stop();
        
        virtual void Draw( CFbsBitmap& aFrame );
        
        virtual TInt SetCamera( CCamera* aCamera );
        
    protected:
        
        void StopCamera();
        
        TBool VfStartPossible();
        
        void ConstructL();

        CMccScreen( TMccScreenType aScreenType,
                    CCamera* aCamera,
                    TPoint aPos, 
                    TSize aArea, 
                    TInt aIndex, 
                    TInt aWindowOrdinalPosition,
                    TInt aWindowOrdinalPriority );

    protected:
        
        CCamera* iCamera;    
        TInt iDeviceIndex;
        TPoint iPosition;
        TSize iArea;
        TInt iWindowOrdinalPosition;
        TInt iWindowOrdinalPriority; 
        
        RWsSession           iRwSession;
        RWindowGroup*        iRwGroup;
        RWindow*             iRw;
        CWsScreenDevice*     iDev;
        
        CWindowGc* iGraphicsContext;
        
        TMccScreenType iScreenType;
        
        
#ifdef TEST_EUNIT
    friend class UT_CMccCameraHandler;
    friend class UT_CMccScreen;
#endif
    
};

/**
* Bitamp viewfinder screen. Uses DSA to draw vf bitmaps.
* 
*/
class CMccScreenBitmap : public CMccScreen, 
                         public MDirectScreenAccess
	{

	public:

		static CMccScreenBitmap* NewL( CCamera* aCamera,
                                       TPoint aPos, 
                                       TSize aArea, 
                                       TInt aIndex, 
                                       TInt aWindowOrdinalPosition,
                                       TInt aWindowOrdinalPriority );

		~CMccScreenBitmap();
	    
	public: // From CMccScreen

		virtual void StartL();
		
		virtual void Stop();
		
		virtual void Draw( CFbsBitmap& aFrame );
		
	private:
	    
	    void ConstructL();

	    CMccScreenBitmap( CCamera* aCamera,
                    TPoint aPos, 
                    TSize aArea, 
                    TInt aIndex, 
                    TInt aWindowOrdinalPosition,
                    TInt aWindowOrdinalPriority );
		
		void Restart( RDirectScreenAccess::TTerminationReasons aReason );
		
		void AbortNow( RDirectScreenAccess::TTerminationReasons aReason );
		
		void StartCameraL();
		
		void StartDsaL();
		
		TPoint UpdateViewFinderArea( TSize aViewFinderImageSize );

        void DoDraw( CFbsBitmap& aFrame );
        
        void DoScreenDeviceUpdate();
        
        void AttachFrameL( CFbsBitmap* aFrame );
                
        void DetachFrame();
        
        void StoreFrameDataL( CFbsBitmap& aFrame );
         
        CFbsBitmap* RestoreFrameLC();
		
		void DrawBlackBorders();
        
	private:

		CDirectScreenAccess* iDirectScreenAccess;
		CFbsBitGc* iGc;
		RRegion* iRegion;
		CFbsScreenDevice* iFbsDev;
		
		TRect iViewFinderImageRect;	
		
		TBool iFirstImageDrawn;
		
		CFbsBitmap* iAttachedFrame;
		
		TBool iAborted;
		
	    TSize iPausedFrameSize;
	    TDisplayMode iPausedFrameDisplayMode;
	    HBufC8* iPausedFrameData;
	    
	    TBool iStopped;
		
#ifdef TEST_EUNIT
    friend class UT_CMccCameraHandler;
    friend class UT_CMccScreen;
#endif
    	
	};

/**
* Direct viewfinder screen.
* 
*/
class CMccScreenDirect : public CMccScreen
    {

    public:

        static CMccScreenDirect* NewL( CCamera* aCamera,
                                       TPoint aPos, 
                                       TSize aArea, 
                                       TInt aIndex, 
                                       TInt aWindowOrdinalPosition,
                                       TInt aWindowOrdinalPriority );

        ~CMccScreenDirect();
        
        
        
    public: // From CMccScreen

        virtual void StartL();
        
        virtual void Stop();
        
        virtual TInt SetCamera( CCamera* aCamera );
        
    private:
        
        void ConstructL();

        CMccScreenDirect( 
                    CCamera* aCamera,
                    TPoint aPos, 
                    TSize aArea, 
                    TInt aIndex, 
                    TInt aWindowOrdinalPosition,
                    TInt aWindowOrdinalPriority );
    
        void StartDirectViewFinderL();
        
        TInt HandleDirectViewFinder();
        
    private:
        
        MCameraDirectViewFinder* iDirectViewFinder;

#ifdef TEST_EUNIT
    friend class UT_CMccCameraHandler;
    friend class UT_CMccScreen;
#endif
    
    };

#endif
