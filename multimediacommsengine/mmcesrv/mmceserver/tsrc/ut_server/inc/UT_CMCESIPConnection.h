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




#ifndef __UT_CMceSipCONNECTION_H__
#define __UT_CMceSipCONNECTION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipConnection;
class CMceServerCore;
class CMCETls;
class CMceCsSession;
class CMceSipSession;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceSipConnection
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

    /**
         * Two phase construction
         */
        static UT_CMceSipConnection* NewL();
        static UT_CMceSipConnection* NewLC();
    /**
         * Destructor
         */
        ~UT_CMceSipConnection();

    private:    // Constructors and destructors

        UT_CMceSipConnection();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();
         
        //void UT_CMceSipConnection_ProfileL();
        void UT_CMceSipConnection_StateL();
        void UT_CMceSipConnection_ConnectionL();
        void UT_CMceSipConnection_AttachLL();
        void UT_CMceSipConnection_DetachL();
        void UT_CMceSipConnection_LocalIPAddressLL();
        void UT_CMceSipConnection_IncomingRequest_1L();
        void UT_CMceSipConnection_IncomingRequest_2L();
        void UT_CMceSipConnection_IncomingResponse_1L();
        void UT_CMceSipConnection_IncomingResponse_2L();
        void UT_CMceSipConnection_IncomingResponse_3L();
        void UT_CMceSipConnection_IncomingResponse_4L();
        void UT_CMceSipConnection_ErrorOccured_1L();
        void UT_CMceSipConnection_ErrorOccured_2L();
        void UT_CMceSipConnection_ErrorOccured_3L();
        void UT_CMceSipConnection_ErrorOccured_4L();
        void UT_CMceSipConnection_ErrorOccured_5L();
        void UT_CMceSipConnection_ErrorOccured_6L();
        void UT_CMceSipConnection_InviteCompletedL();
        void UT_CMceSipConnection_InviteCanceledL();
        void UT_CMceSipConnection_ConnectionStateChangedL();
        void UT_CMceSipConnection_FillWithMatchingAddrFamilyL();
        //void UT_CMceSipConnection_IsProfileContactSecureLL();
        void UT_CMceSipConnection_ConnectionUpdateLL();
        void UT_CMceSipConnection_IsProfileUsedInSessionL();


    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
        
		TMceIds iIds;
	    CMceServerCore* iServerCore;
	    CMceCsSession* iSession;
	    CMceSipConnection* iConnection;
	    CMceSipSession* iSipSession;
	    
    private: //not Owned
    	    
        CMCETls* iStorage;
	    
		
    };

#endif      //  __UT_CMceSipCONNECTION_H__

// End of file
