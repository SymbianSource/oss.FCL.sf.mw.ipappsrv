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




#ifndef __UT_CMCESTATEACKNOWLEDGEMENTREQUIRED_H__
#define __UT_CMCESTATEACKNOWLEDGEMENTREQUIRED_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceCsSession;
class CMceServerCore;
class CMCETls;
class CMceStateAcknowledgementRequired;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceStateAcknowledgementRequired
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceStateAcknowledgementRequired* NewL();
        static UT_CMceStateAcknowledgementRequired* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceStateAcknowledgementRequired();

    private:    // Constructors and destructors

        UT_CMceStateAcknowledgementRequired();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

         void SetupL();
         void Teardown();        
        
         void UT_CMceStateAcknowledgementRequired_AcceptLL();
         void UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateFailsLL();
         void UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateOkLL();
         void UT_CMceStateAcknowledgementRequired_EntryPrack_UpdateAsyncLL();
         void UT_CMceStateAcknowledgementRequired_EntryPrack_NoSdpLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryPrack_NoMatchingPrackLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_NoReservationLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedNoNegotateLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservedAsyncLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_ReservationErrorLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryMediaUpdated_WrongTransactionLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryNoPrackRecievedLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryReliableFailedLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryItcRejectSessionLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryResponseLL();
    	 void UT_CMceStateAcknowledgementRequired_EntryItcAcceptSessionLL();
         void UT_CMceStateAcknowledgementRequired_ExitLL();
         void UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedLL();
         void UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL2L();
         void UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL3L();
         void UT_CMceStateAcknowledgementRequired_EntryMediaUpdatedL4L();

    private:    // Data

    	EUNIT_DECLARE_TEST_TABLE; 
    		
    	TMceIds iIds;
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceSipSession* iSipSession;
    
    private: //not Owned


        CMCETls* iStorage;
		CMceStateAcknowledgementRequired* iState;
		
    };

#endif      //  __UT_CMCESTATEACKNOWLEDGEMENTREQUIRED_H__

// End of file
