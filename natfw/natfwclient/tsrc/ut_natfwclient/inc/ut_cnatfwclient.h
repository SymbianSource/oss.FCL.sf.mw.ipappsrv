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




#ifndef __UT_CNATFWClient_H__
#define __UT_CNATFWClient_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mnatfwconnectivityobserver.h"
#include "natfwconnectivityapidefs.h"

//  FORWARD DECLARATIONS
class CNATFWClient;
class CSettings;

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
NONSHARABLE_CLASS( UT_CNATFWClient )
     : public CEUnitTestSuiteClass,
       public MNATFWConnectivityObserver
    {
    public: // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNATFWClient* NewL();
        static UT_CNATFWClient* NewLC();
        /**
         * Destructor
         */
        ~UT_CNATFWClient();

    private: // Constructors and destructors

        UT_CNATFWClient();
        void ConstructL();

    public: // From MNATFWConnectivityObserver
        
        void EventOccured( TUint aSessionId, TUint aStreamId,
            TNATFWConnectivityEvent aEvent, TInt aError, TAny* aEventData );


    private: // New methods

         void SetupL();
        
         void SetupSessionL();

         void SetupStreamL();

         void Teardown();
        
         void UT_CNATFWClient_NewLL();

         
         void UT_CNATFWClient_NewLCL();
        
         
         void UT_CNATFWClient_CreateSessionLL();
         
        
         void UT_CNATFWClient_CreateStreamLL();

         
         void UT_CNATFWClient_CreateWrapperLL();

         
         void UT_CNATFWClient_FetchCandidateLL();

        
         void UT_CNATFWClient_FetchCandidatesLL();
        

         void UT_CNATFWClient_SetReceivingStateLL();

         
         void UT_CNATFWClient_SetReceivingStateL_ActiveL();
         
         
         void UT_CNATFWClient_SetReceivingStateL_PassiveL();
        
         
         void UT_CNATFWClient_SetSendingStateLL();
         

         void UT_CNATFWClient_SetSendingStateL_ActiveL();
         
         
         void UT_CNATFWClient_SetSendingStateL_PassiveL();
         
         
         void UT_CNATFWClient_SetOperationModeLL();
        

         void UT_CNATFWClient_SetCredentialsLL();


         void UT_CNATFWClient_PerformConnectivityChecksLL();
        

         void UT_CNATFWClient_UpdateIceProcessingLL();
         
         
         void UT_CNATFWClient_UpdateIceProcessingL2L();
         

         void UT_CNATFWClient_FindStreamByIdL();
         
         
    private: // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CNATFWClient* iClient;
        CSettings* iSettings;
        TUint iSessionId;
        TUint iStreamId;
        TBool iIsRunning;
        TNATFWConnectivityEvent iLastNotifyEvent;
        TInt iLastNotifyError;
    };

#endif //  __UT_CNATFWClient_H__

// End of file
