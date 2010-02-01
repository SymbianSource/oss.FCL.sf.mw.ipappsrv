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




#ifndef __UT_CMCEEVENTSUBSCRIBETERMINATINGSTATE_H__
#define __UT_CMCEEVENTSUBSCRIBETERMINATINGSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventSubscribeTerminatingState;
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
class UT_CMceEventSubscribeTerminatingState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventSubscribeTerminatingState* NewL();
        static UT_CMceEventSubscribeTerminatingState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventSubscribeTerminatingState();

    private:    // Constructors and destructors

        UT_CMceEventSubscribeTerminatingState();
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
        
         void UT_CMceEventSubscribeTerminatingState_HandleLL();
        
         void UT_CMceEventSubscribeTerminatingState_HandleReceiveRequestLL();
           
         void UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedLL();
         
         void UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL2L();
         
         void UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL3L();
         
         void UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL4L();
         
         void UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL5L();

         void UT_CMceEventSubscribeTerminatingState_HandleResponseReceivedL6L();
         
    private:    // Data

        CMceEventSubscribeTerminatingState* iState;		
    	CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		TUint iEventState;
        
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;
		
    };

#endif      //  __UT_CMCEEVENTSUBSCRIBETERMINATINGSTATE_H__

// End of file
