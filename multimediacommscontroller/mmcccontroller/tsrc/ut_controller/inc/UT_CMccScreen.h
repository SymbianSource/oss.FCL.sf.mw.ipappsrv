/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __UT_CMCCSCREEN_H__
#define __UT_CMCCSCREEN_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include <ecam.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMccScreen;
class CMccScreenBitmap;
class CMccScreenDirect;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMccScreen )
     : public CEUnitTestSuiteClass, public MCameraObserver2
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccScreen* NewL();
        static UT_CMccScreen* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccScreen();

    private:    // Constructors and destructors

        UT_CMccScreen();
        void ConstructL();

    public: // From observer interface

         // From MCameraObserver2
        virtual void HandleEvent(const TECAMEvent& aEvent);
        virtual void ViewFinderReady(MCameraBuffer& aCameraBuffer,TInt aError);
        virtual void ImageBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);
        virtual void VideoBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);

    private:    // New methods

         void SetupL();
         void Setup2L();
         void Setup3L();
         
         void Teardown();
        
         void UT_CMccScreen::UT_CMccScreen_CMccScreen_NewLL();

         void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_StartLL();
         void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_StopL();
         void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_DrawL();
         void UT_CMccScreen::UT_CMccScreen_CMccScreenBitmap_SetCameraL();

         void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_StartLL();
         void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_StopL();
         void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_DrawL();
         void UT_CMccScreen::UT_CMccScreen_CMccScreenDirect_SetCameraL();
         
    public:
        
        CMccScreenBitmap* BitmapScreen();
        CMccScreenDirect* DirectScreen();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        CMccScreen* iScreen;
        CCamera* iCamera;

    };

#endif      //  __UT_CMCCSCREEN_H__

// End of file
