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




#ifndef __UT_CMCEEVENTCONTEXT_H__
#define __UT_CMCEEVENTCONTEXT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <badesca.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventContext;
class CMceSipEvent;
class CMceComEvent;
class CSIPProfile;
class CMceSipConnection;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceEventContext
     : public CEUnitTestSuiteClass,
      public MMCETestEventObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventContext* NewL();
        static UT_CMceEventContext* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventContext();

    private:    // Constructors and destructors

        UT_CMceEventContext();
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
         
         void SetupEmptyL();
        
         void Teardown();
        
         void UT_CMceEventContext_NewLL();
         
         void UT_CMceEventContext_DestructorL();
        
         void UT_CMceEventContext_SetCurrentStateL();
        
         void UT_CMceEventContext_CurrentStateL();
        
         void UT_CMceEventContext_ProcessEventLL();
        
         void UT_CMceEventContext_ProcessRequestReceivedEventLL();
        
         void UT_CMceEventContext_ProcessIncomingResponseEventLL();
        
         void UT_CMceEventContext_IsStateTransitionAcceptedL();
         
         void UT_CMceEventContext_IsMoStateTransitionAcceptedL();
         
         void UT_CMceEventContext_IsMtStateTransitionAcceptedL();
         

    private:    // Data

        CMceEventContext* iContext;
        CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		
		EUNIT_DECLARE_TEST_TABLE; 
		
    };

#endif      //  __UT_CMCEEVENTCONTEXT_H__

// End of file
