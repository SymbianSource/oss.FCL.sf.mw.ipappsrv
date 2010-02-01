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




#ifndef __UT_CMCEEVENTSUBSCRIBEMOESTABLISHEDSTATE_H__
#define __UT_CMCEEVENTSUBSCRIBEMOESTABLISHEDSTATE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "MMCETestEventObserver.h"

//  FORWARD DECLARATIONS
class CMceEventSubscribeMoEstablishedState;
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
class UT_CMceEventSubscribeMoEstablishedState
     : public CEUnitTestSuiteClass,
       public MMCETestEventObserver

    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceEventSubscribeMoEstablishedState* NewL();
        static UT_CMceEventSubscribeMoEstablishedState* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceEventSubscribeMoEstablishedState();

    private:    // Constructors and destructors

        UT_CMceEventSubscribeMoEstablishedState();
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
        
         void UT_CMceEventSubscribeMoEstablishedState_HandleLL();
         
         void UT_CMceEventSubscribeMoEstablishedState_HandleL2L();
         
         void UT_CMceEventSubscribeMoEstablishedState_HandleL3L();
         
         void UT_CMceEventSubscribeMoEstablishedState_HandleL4L();
        
         void UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestLL();
         
         void UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL2L();
         
         void UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL3L();
         
         void UT_CMceEventSubscribeMoEstablishedState_HandleReceiveRequestL4L();
           
         void UT_CMceEventSubscribeMoEstablishedState_HandleResponseReceivedLL();

        

    private:    // Data

        CMceEventSubscribeMoEstablishedState* iState;
		CMceSipEvent* iSIPEvent;
        CMceComEvent* iEvent;
        CSIPMessageElements* iMsgElem; // not owned
        TUint iEventState;
        TUint iReceived;
        CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		
		EUNIT_DECLARE_TEST_TABLE; 
		friend class CMceSipEvent;

    };

#endif      //  __UT_CMCEEVENTSUBSCRIBEESTABLISHEDSTATE_H__

// End of file
