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




#ifndef __UT_CMCEEVENTREFERTERMINATINGSTATE_H__
#define __UT_CMCEEVENTREFERTERMINATINGSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventReferTerminatingState;
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
class UT_CMceEventReferTerminatingState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventReferTerminatingState* NewL();
        static UT_CMceEventReferTerminatingState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventReferTerminatingState();

    private:    // Constructors and destructors

        UT_CMceEventReferTerminatingState();
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
        
         void UT_CMceEventReferTerminatingState_HandleLL();
        
         void UT_CMceEventReferTerminatingState_HandleReceiveRequestLL();
           
         void UT_CMceEventReferTerminatingState_HandleResponseReceivedLL();
         
         void UT_CMceEventReferTerminatingState_HandleResponseReceivedL2L();
         
         void UT_CMceEventReferTerminatingState_HandleResponseReceivedL3L();
         
         void UT_CMceEventReferTerminatingState_HandleResponseReceivedL4L();
         
         void UT_CMceEventReferTerminatingState_HandleResponseReceivedL5L();
         
         void UT_CMceEventReferTerminatingState_HandleResponseReceivedL6L();


    private:    // Data

        CMceEventReferTerminatingState* iState;		
    	CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		TUint iEventState;
        CSIPMessageElements* iMsgElem; // not owned
        
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;

    };

#endif      //  __UT_CMCEEVENTREFERTERMINATINGSTATE_H__

// End of file
