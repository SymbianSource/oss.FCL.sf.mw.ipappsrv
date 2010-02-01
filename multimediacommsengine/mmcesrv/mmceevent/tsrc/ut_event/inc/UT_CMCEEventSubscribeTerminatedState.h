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




#ifndef __UT_CMCEEVENTSUBSCRIBETERMINATEDSTATE_H__
#define __UT_CMCEEVENTSUBSCRIBETERMINATEDSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventSubscribeTerminatedState;
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
class UT_CMceEventSubscribeTerminatedState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventSubscribeTerminatedState* NewL();
        static UT_CMceEventSubscribeTerminatedState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventSubscribeTerminatedState();

    private:    // Constructors and destructors

        UT_CMceEventSubscribeTerminatedState();
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
        
         void UT_CMceEventSubscribeTerminatedState_HandleLL();
        
         void UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestLL();
         
         void UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestL2L();
         
         void UT_CMceEventSubscribeTerminatedState_HandleReceiveRequestL3L();
           
         void UT_CMceEventSubscribeTerminatedState_HandleResponseReceivedLL();


    private:    // Data

        CMceEventSubscribeTerminatedState* iState;		
        CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		TUint iReceived;
        TUint iEventState;
        CSIPMessageElements* iMsgElem; // not owned
        
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;

    };

#endif      //  __UT_CMCEEVENTSUBSCRIBETERMINATEDSTATE_H__

// End of file
