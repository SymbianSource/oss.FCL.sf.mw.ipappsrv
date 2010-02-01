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




#ifndef __UT_CMCECSSUBSESSION_H__
#define __UT_CMCECSSUBSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include "mceClientServer.h"
#include "mceServerCore.h"
//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMceCsSubSession;
class CMceServerCore;
class CMceCsSession;
class CMceCsSubSession;
class CMCETls;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceCsSubSession
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceCsSubSession* NewL();
        static UT_CMceCsSubSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceCsSubSession();

    private:    // Constructors and destructors

        UT_CMceCsSubSession();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();
        void Teardown();

        void UT_CMceCsSubSession_GetFunctionsL();

        void UT_CMceCsSubSession_ServiceLL();

        void UT_CMceCsSubSession_SetDialogL();
        void UT_CMceCsSubSession_SetPendingTransactionLL();
        void UT_CMceCsSubSession_CanceledL();
        void UT_CMceCsSubSession_ClientReadyToReceiveL();
        void UT_CMceCsSubSession_CancelClientReceiveL();
        void UT_CMceCsSubSession_InviteCompletedL();
        
        void UT_CMceCsSubSession_PopRequestL();
        void UT_CMceCsSubSession_PopResponseL();
        void UT_CMceCsSubSession_ClientExistsL();
        
        void UT_CMceCsSubSession_DecodeLL();
        
        void UT_CMceCsSubSession_ProfileL();
        
        void UT_CMceCsSubSession_HasInitialInviteTransactionBeenCompletedL();
        void UT_CMceCsSubSession_GetRequestL();
        void UT_CMceCsSubSession_DialogRequestReceivedL();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		TMceIds iIds;
	    CMceServerCore* iServerCore;
	    CMceCsSession* iSession;
	    CMceCsSubSession* iSubSession;
	    
    private: //not Owned
    	    
	    CMCETls* iStorage;
		
    };

#endif      //  __UT_CMCECSSUBSESSION_H__

// End of file
