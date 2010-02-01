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




#ifndef __UT_CMCEEVENTREFERMTESTABLISHINGSTATE_H__
#define __UT_CMCEEVENTREFERMTESTABLISHINGSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventReferMtEstablishingState;
class CMceComEvent;
class CMceSipEvent;
class CSIPMessageElements;
class CMceSipConnection;
class CSIPProfile;


//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceEventReferMtEstablishingState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventReferMtEstablishingState* NewL();
        static UT_CMceEventReferMtEstablishingState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventReferMtEstablishingState();

    private:    // Constructors and destructors

        UT_CMceEventReferMtEstablishingState();
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
        
         void SetupFullL();
         
         void SetupEmptyL();
         
         void Teardown();
        
         void UT_CMceEventReferMtEstablishingState_HandleLL();
         
         void UT_CMceEventReferMtEstablishingState_HandleL2L();
         
         void UT_CMceEventReferMtEstablishingState_HandleL3L();
         
         void UT_CMceEventReferMtEstablishingState_HandleL4L();
         
         void UT_CMceEventReferMtEstablishingState_HandleL5L();
         
         void UT_CMceEventReferMtEstablishingState_HandleL6L();
        
         void UT_CMceEventReferMtEstablishingState_HandleReceiveRequestLL();
           
         void UT_CMceEventReferMtEstablishingState_HandleResponseReceivedLL();
         
         void UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL2L();
         
         void UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL3L();
         
         void UT_CMceEventReferMtEstablishingState_HandleResponseReceivedL4L();
         
         
    private:    // Data

        CMceEventReferMtEstablishingState* iState;		
    	CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		CSIPMessageElements* iMsgElem; // not owned
        TUint iEventState;
        
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;
		
    };

#endif      //  __UT_CMCEEVENTREFERMTESTABLISHINGSTATE_H__

// End of file
