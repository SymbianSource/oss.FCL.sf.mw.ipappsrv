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




#ifndef __UT_CMCEEVENTSUBSCRIBEMTESTABLISHEDSTATE_H__
#define __UT_CMCEEVENTSUBSCRIBEMTESTABLISHEDSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventSubscribeMtEstablishedState;
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
class UT_CMceEventSubscribeMtEstablishedState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventSubscribeMtEstablishedState* NewL();
        static UT_CMceEventSubscribeMtEstablishedState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventSubscribeMtEstablishedState();

    private:    // Constructors and destructors

        UT_CMceEventSubscribeMtEstablishedState();
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
         
         void SetupFullL();
        
         void SetupEmptyL();
         
         void Teardown();
        
         void UT_CMceEventSubscribeMtEstablishedState_HandleLL();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleL2L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleL3L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleL4L();
        
         void UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestLL();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL2L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL3L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL4L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleReceiveRequestL5L();
           
         void UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedLL();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL2L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL3L();
         
         void UT_CMceEventSubscribeMtEstablishedState_HandleResponseReceivedL4L();

        

    private:    // Data

        CMceEventSubscribeMtEstablishedState* iState;
		CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		CSIPMessageElements* iMsgElem; // not owned
        TUint iEventState;
		
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;

    };

#endif      //  __UT_CMCEEVENTSUBSCRIBEMTESTABLISHEDSTATE_H__

// End of file
