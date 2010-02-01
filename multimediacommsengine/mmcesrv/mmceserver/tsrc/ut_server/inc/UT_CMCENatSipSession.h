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




#ifndef __UT_CMceNatSipSESSION_H__
#define __UT_CMceNatSipSESSION_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include "mceclientserver.h"

//  FORWARD DECLARATIONS
class CMceSipSession;
class CMceNatSipSession;
class CMceSipStateMachine;
class CSIPConnection;
class CSIP;
class CMceServerCore;
class CMceCsSession;
class CMceSipConnection;
class CMceSipManager;
class CSIPProfile;
class CMCETls;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMceNatSipSession
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMceNatSipSession* NewL();
        static UT_CMceNatSipSession* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceNatSipSession();

    private:    // Constructors and destructors

        UT_CMceNatSipSession();
        void ConstructL();


    private:    // New methods

        void SetupL();
        void Setup2L();
        void Teardown();

        // entry points
        void UT_CMceNatSipSession_UpdateMediaLL();
        void UT_CMceNatSipSession_UpdatedL();
        void UT_CMceNatSipSession_Updated2L();
        void UT_CMceNatSipSession_Updated3L();
        void UT_CMceNatSipSession_HandleSIPEvent();
        
        // SDP O/A & Utility
        void UT_CMceNatSipSession_StorePrevOffer();
        void UT_CMceNatSipSession_Offer();
        
        // callbacks, trigger events
        void UT_CMceNatSipSession_InitializedMO();
        void UT_CMceNatSipSession_InitializedMT();
        void UT_CMceNatSipSession_InitializedMT_PullMode();
        void UT_CMceNatSipSession_Initialized_Panic2();
        void UT_CMceNatSipSession_OfferReady();
        void UT_CMceNatSipSession_OfferReady_Panic2();
        void UT_CMceNatSipSession_AnswerReady();
        void UT_CMceNatSipSession_AnswerReady_Panic2();
        void UT_CMceNatSipSession_UpdateSdp();
        void UT_CMceNatSipSession_UpdateSdp_Panic2();
        void UT_CMceNatSipSession_UpdateSdp_Panic3();
        void UT_CMceNatSipSession_UpdateSdp_Panic4();
        void UT_CMceNatSipSession_UpdateSdp_Panic5();
        void UT_CMceNatSipSession_UpdateSdp_Panic6();
        void UT_CMceNatSipSession_UpdateSdp_Panic7();        
        void UT_CMceNatSipSession_ErrorOccurred();
        void UT_CMceNatSipSession_ErrorOccurred_Panic2();
        void UT_CMceNatSipSession_IcmpErrorOccurred();
        
        // actions
        void UT_CMceNatSipSession_CreateOfferL();
        void UT_CMceNatSipSession_ResolveL();
        void UT_CMceNatSipSession_DecodeAnswerL();
        void UT_CMceNatSipSession_UpdateL();
        void UT_CMceNatSipSession_UpdateRemoteAddressL();
        void UT_CMceNatSipSession_Continue();
        
        void UT_CMceNatSipSession_NatErrorOccurred();
        
        void UT_CMceNatSipSession_NatSessionWithoutProfile();
        
        void UT_CMceNatSipSession_GetResourceReservationStatusL();
		void UT_CMceNatSipSession_SetNeedToReceive();

		void UT_CMceNatSipSession_PluginInitAfterMedia();
		    
    private:    // Methods
    
        TInt& NatPluginManagerReturnStatusL();
        
    private:    // Data
        
        EUNIT_DECLARE_TEST_TABLE; 
        
        TUint iNatSessionId;    
	    TMceIds iIds;
	    
        CMceServerCore* iServerCore;
        CMceCsSession* iSession;
        CMceNatSipSession* iNatSipSession;
    
    private: //not Owned
    	    
        CMCETls* iStorage;
    };

#endif      //  __UT_CMceNatSipSESSION_H__

// End of file
