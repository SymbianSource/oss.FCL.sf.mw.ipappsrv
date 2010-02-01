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




#ifndef __UT_CMCEEVENTREFERMOESTABLISHINGSTATE_H__
#define __UT_CMCEEVENTREFERMOESTABLISHINGSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"


//  FORWARD DECLARATIONS
class CMceEventReferMoEstablishingState;
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
class UT_CMceEventReferMoEstablishingState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventReferMoEstablishingState* NewL();
        static UT_CMceEventReferMoEstablishingState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventReferMoEstablishingState();

    private:    // Constructors and destructors

        UT_CMceEventReferMoEstablishingState();
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
        
         void UT_CMceEventReferMoEstablishingState_HandleLL();
        
         void UT_CMceEventReferMoEstablishingState_HandleReceiveRequestLL();
           
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedLL();
         
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL2L();
         
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL3L();
         
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL4L();
         
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL5L();
         
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL6L();
         
         void UT_CMceEventReferMoEstablishingState_HandleResponseReceivedL7L();


    private:    // Data

        CMceEventReferMoEstablishingState* iState;		
    	CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		TUint iEventState;
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;
		
    };

#endif      //  __UT_CMCEEVENTREFERMOESTABLISHINGSTATE_H__

// End of file
