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




#ifndef __UT_CMCEEVENTREFERMOIDLESTATE_H__
#define __UT_CMCEEVENTREFERMOIDLESTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventReferMoIdleState;
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
class UT_CMceEventReferMoIdleState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventReferMoIdleState* NewL();
        static UT_CMceEventReferMoIdleState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventReferMoIdleState();

    private:    // Constructors and destructors

        UT_CMceEventReferMoIdleState();
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

         private:    // New methods

         void SetupL();
        
         void SetupEmptyL();
         
         void Teardown();
        
         void UT_CMceEventReferMoIdleState_HandleLL();
         
         void UT_CMceEventReferMoIdleState_HandleL2L();
        
         void UT_CMceEventReferMoIdleState_HandleReceiveRequestLL();
           
         void UT_CMceEventReferMoIdleState_HandleResponseReceivedLL();


    private:    // Data

        CMceEventReferMoIdleState* iState;		
    	CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;		
    };

#endif      //  __UT_CMCEEVENTREFERMOIDLESTATE_H__

// End of file
