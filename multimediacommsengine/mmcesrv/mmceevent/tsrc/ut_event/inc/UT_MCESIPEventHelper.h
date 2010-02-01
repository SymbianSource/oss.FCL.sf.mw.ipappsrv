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




#ifndef __UT_MCESIPEVENTHELPER_H__
#define __UT_MCESIPEVENTHELPER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include  "mmcetesteventobserver.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceComEvent;
class CMceSipEvent;
class CMceSipConnection;
class CSIPProfile;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_MCESIPEventHelper
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MCESIPEventHelper* NewL();
        static UT_MCESIPEventHelper* NewLC();
        /**
         * Destructor
         */
        ~UT_MCESIPEventHelper();

    private:    // Constructors and destructors

        UT_MCESIPEventHelper();
        void ConstructL();

	public: 	// From observer interface
		
		//From  test event observer
	    void IncomingSubscribeL( CDesC8Array* aHeaders,
                                 HBufC8* aContentType,
					             HBufC8* aBody );
		
        void IncomingReferL( HBufC8* aReferTo,
					         CDesC8Array* aHeaders,
                             HBufC8* aContentType,
					         HBufC8* aBody );
		
		void EventStateChangedL( TUint32 aState );
		
		void NotifyReceivedL( CDesC8Array* aHeaders,
                    		  HBufC8* aContentType,
							  HBufC8* aBody );
							


    private:    // New methods

         void SetupL();
         
         void SetupEmpty();
        
         void Teardown();
        
         void UT_MCESIPEventHelper_SetContentLCL();
         
         void UT_MCESIPEventHelper_CreateMessageElementsLCL();
         
         void UT_MCESIPEventHelper_CreateMessageElementsLC2L();
         
         void UT_MCESIPEventHelper_CreateMessageElementsLC3L();
         
         void UT_MCESIPEventHelper_CreateReferNotifyMessageElementsLCL();
        
         void UT_MCESIPEventHelper_HandleSubscriptionStateHeaderLL();
         
         void UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL2L();
         
         void UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL3L();
        
         void UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL4L();
         
         void UT_MCESIPEventHelper_HandleSubscriptionStateHeaderL5L();
         
         void UT_MCESIPEventHelper_FindHeaderL();
        
         void UT_MCESIPEventHelper_NotifyRequestReceivedLCL();
        
         void UT_MCESIPEventHelper_NotifyRequestReceivedLC2L();
        
         void UT_MCESIPEventHelper_NotifyRequestReceivedLC3L();
        
         void UT_MCESIPEventHelper_ReferNotifyRequestReceivedLCL();
        
         void UT_MCESIPEventHelper_ReferNotifyRequestReceivedLC2L();
        
         void UT_MCESIPEventHelper_ReferNotifyRequestReceivedLC3L();
        
         void UT_MCESIPEventHelper_SubscribeRequestReceivedLCL();
        
         void UT_MCESIPEventHelper_HandleExpireHeaderLL();
         
         void UT_MCESIPEventHelper_ResolveReferTypeLL();
         
         void UT_MCESIPEventHelper_ResolveReferTypeL2L();
         
         void UT_MCESIPEventHelper_ResolveReferTypeL3L();
         
         void UT_MCESIPEventHelper_ResolveReferTypeL4L();
         
         void UT_MCESIPEventHelper_MatchEventsL();
         
         void UT_MCESIPEventHelper_MatchEventsLL();
         
         void UT_MCESIPEventHelper_CreateResponseLL();
         
         void UT_MCESIPEventHelper_CreateResponseL2L();
         
    private:    // Data

        CMceComEvent* iEvent;
        CMceSipEvent* iSIPEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_MCESIPEVENTHELPER_H__

// End of file
