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




#ifndef __UT_CMccVIDEOSINKIMPL_H__
#define __UT_CMccVIDEOSINKIMPL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <videoplayer.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMccVideoSinkImpl;
class CMccTestEventHandler;

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
NONSHARABLE_CLASS( UT_CMccVideoSinkImpl )
     : public CEUnitTestSuiteClass, 
       public MVideoLoadingObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMccVideoSinkImpl* NewL();
        static UT_CMccVideoSinkImpl* NewLC();
        /**
         * Destructor
         */
        ~UT_CMccVideoSinkImpl();

    private:    // Constructors and destructors

        UT_CMccVideoSinkImpl();
        void ConstructL();

    public:     // From observer MVideoLoadingObserver

        inline void MvloLoadingStarted()
	        {
	        	
	        }
	    inline void MvloLoadingComplete()
		    {
		    	
		    }

    private:    // New methods

         void SetupL();
         
         void Setup2L();
         
         void Setup3L();
        
         void Teardown();
        
         void UT_CMccVideoSinkImpl_NewLL();
        
         void UT_CMccVideoSinkImpl_SinkThreadLogoffL();
                 
         void UT_CMccVideoSinkImpl_OpenLL();
        
        
         void UT_CMccVideoSinkImpl_PrepareL();
        
        
         void UT_CMccVideoSinkImpl_RemoteSetupL();
        
        
         void UT_CMccVideoSinkImpl_StartL();
        
        
         void UT_CMccVideoSinkImpl_StopL();
        
        
         void UT_CMccVideoSinkImpl_PauseLL();
        
        
         void UT_CMccVideoSinkImpl_CloseL();
        
        
         void UT_CMccVideoSinkImpl_SetDisplayWindowLL();
        
        
         void UT_CMccVideoSinkImpl_PriorityLL();
        
        
         void UT_CMccVideoSinkImpl_SetPriorityLL();
        
        
         void UT_CMccVideoSinkImpl_VolumeL();
        
        
         void UT_CMccVideoSinkImpl_MaxVolumeL();
        
        
         void UT_CMccVideoSinkImpl_SetVolumeLL();
        
        
         void UT_CMccVideoSinkImpl_BalanceL();
        
        
         void UT_CMccVideoSinkImpl_SetBalanceLL();
        
        
         void UT_CMccVideoSinkImpl_RegisterForVideoLoadingNotificationL();
         
         void UT_CMccVideoSinkImpl_RegisterForVideoLoadingNotification2L();
        
         void UT_CMccVideoSinkImpl_VersionL();
         
         void UT_CMccVideoSinkImpl_FormatSDPInfoL();
         
         void UT_CMccVideoSinkImpl_EmptyBufferL();
         
         void UT_CMccVideoSinkImpl_UpdateSettingsL();
         
         void UT_CMccVideoSinkImpl_TimestampCorrectionTestL();
         
         void UT_CMccVideoSinkImpl_TimestampCorrectionTest2L();
         
         void UT_CMccVideoSinkImpl_TimestampCorrectionTest3L();
        
         void UT_CMccVideoSinkImpl_GetActualPrerollL();
        

    private:
    
         void DoSetupL( TInt aJitbufThreshold );
         
    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

 		CMccVideoSinkImpl* iSinkImpl;
 		CMccTestEventHandler* iAudioUser;
 		CMccTestEventHandler* iVideoUser;
 		
 		TTime iCurrentTime;
 		TInt64 iEmptyBufferInterval;
 		
 		TInt iJitterBufferThreshold;

    };

#endif      //  __UT_CMccVIDEOSINKIMPL_H__

// End of file
