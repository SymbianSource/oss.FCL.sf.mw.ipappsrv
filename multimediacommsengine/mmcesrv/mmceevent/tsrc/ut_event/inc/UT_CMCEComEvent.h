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




#ifndef __UT_CMCECOMEVENT_H__
#define __UT_CMCECOMEVENT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <sipexpiresheader.h>
#include <e32base.h>
#include <badesca.h>
  	    
//  INTERNAL INCLUDES
#include "mcesipevent.h"

#include  "sipsubscribedialogassoc.h"
#include  "sipreferdialogassoc.h"
#include  "sipdialog.h"
#include  "sipservertransaction.h"
#include  "sipconnection.h"
#include  "sipclienttransaction.h"
#include  "mmcetesteventobserver.h"

//  FORWARD DECLARATIONS
class CMceComEvent;
class CMceComEventSipMsgHelper;
class MSIPObserver;
class CMceSipEvent;
class CSIPProfileRegistry;
class CMceSipConnection;
class CSIPProfile;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceComEvent
     : public CEUnitTestSuiteClass,
     	public MMCETestEventObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceComEvent* NewL();
        static UT_CMceComEvent* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceComEvent();
        
    private:    // Constructors and destructors

        UT_CMceComEvent();
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
         
         void SetupReferL();
        
         void Teardown();
         
         void TeardownRefer();
        
         void UT_CMceComEvent_NewLL();
        
         void UT_CMceComEvent_DestructorL();
         
         void UT_CMceComEvent_PerformActionLL();
        
         void UT_CMceComEvent_ProceedL();
        
         void UT_CMceComEvent_ReceivedRequestL();
        
         void UT_CMceComEvent_EventContextL();
        
         void UT_CMceComEvent_PreviousActionL();
        
         void UT_CMceComEvent_SetPreviousActionL();
        
         void UT_CMceComEvent_RefreshIntervalL();
        
         void UT_CMceComEvent_SetRefreshIntervalL();
         
         void UT_CMceComEvent_SetReasonPhraseLL();
         
         void UT_CMceComEvent_ReasonPhraseL();
         
         void UT_CMceComEvent_StatusCodeL();
         
         void UT_CMceComEvent_SetMsgContentL();
        
         void UT_CMceComEvent_SIPEventL();
         
         void UT_CMceComEvent_SetMsgHeadersL();
         
         void UT_CMceComEvent_MsgHeadersL();
         
         void UT_CMceComEvent_SetReferTypeL();
         
         void UT_CMceComEvent_ReferTypeL();
         
         void UT_CMceComEvent_SetSilentSuppressionOnL();
         
         void UT_CMceComEvent_SilentSuppressionL();
         
         void UT_CMceComEvent_IdValueL();
         
         void UT_CMceComEvent_SetIdValueL();
         
		
    private:    // Data

		TUint32 iState;
		CMceComEvent* iEvent;
		CMceSipEvent* iSIPEvent;
		CSIPProfile* iSipProfile;
		CMceSipConnection* iConnection;
		
		EUNIT_DECLARE_TEST_TABLE; 
    };

#endif      //  __UT_CMCECOMEVENT_H__

// End of file
