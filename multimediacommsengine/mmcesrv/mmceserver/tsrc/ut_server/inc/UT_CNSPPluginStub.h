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




#ifndef __UT_CNSPPluginStub_H__
#define __UT_CNSPPluginStub_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>
#include "nspsessionobserver.h"
#include "CNSPPluginStub.h"

//  FORWARD DECLARATIONS

class CNSPPluginStub;

#include <e32def.h>

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
 class UT_CNSPPluginStub:
   	   public CEUnitTestSuiteClass,
       public MNSPSessionObserver
     
    {
    
    
   	// =======================================================================================================================
	// Function from observer interface.
	// =======================================================================================================================
	//
    public: 

       
 	void Initialized( TUint aSessionId );
        
   	void OfferReady( TUint aSessionId, CSdpDocument* aOffer );
    
 	void AnswerReady( TUint aSessionId, CSdpDocument* aAnswer );
    
	void UpdateSdp( TUint aSessionId, CSdpDocument* aOffer );
    
  	void ErrorOccurred( TUint aSessionId, TInt aError );
  	
  	void IcmpErrorOccurred( TUint aSessionId, TInt aError );
  	
    
       
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CNSPPluginStub* NewL();
        static UT_CNSPPluginStub* NewLC();
        /**
         * Destructor
         */
        ~UT_CNSPPluginStub();

    private:    // Constructors and destructors

        UT_CNSPPluginStub();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
        
         void Teardown();
        
         void UT_CNSPPluginStub_NewLL();
        
        
         void UT_CNSPPluginStub_NewSessionLL();
        
        
         void UT_CNSPPluginStub_CloseSessionLL();
        
        
         void UT_CNSPPluginStub_CreateOfferLL();
        
        
         void UT_CNSPPluginStub_ResolveLL();
        
        
         void UT_CNSPPluginStub_DecodeAnswerLL();
        
        
         void UT_CNSPPluginStub_UpdateLL();
        
        
         void UT_CNSPPluginStub_RestartLL();
        
        
                 
         void UT_CNSPPluginStub_GetSessionParameterLL();
        

    private:    // Data

    EUNIT_DECLARE_TEST_TABLE; 
    
    CNSPPluginStub* iPlugin;    
    TUint iNatSessionId;	
  	TNatReturnStatus iRequestStatus;

	TUint iInitializedReceived;
	TUint iOfferReadyReceived;
	TUint iAnswerReadyReceived;
	TUint iUpdateSdpReceived;
	TUint iErrorOccurredReceived;
    };

#endif      //  __UT_CNSPPluginStub_H__

// End of file
