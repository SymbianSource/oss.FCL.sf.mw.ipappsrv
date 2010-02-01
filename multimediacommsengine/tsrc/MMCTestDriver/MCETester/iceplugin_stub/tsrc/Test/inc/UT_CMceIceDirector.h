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


#ifndef __UT_CMCEICEDIRECTOR_H__
#define __UT_CMCEICEDIRECTOR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <nspplugin.h>
#include <nspsessionobserver.h>

//  FORWARD DECLARATIONS

class CMceIceDirector;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
 class UT_CMceIceDirector:
   	   public CEUnitTestSuiteClass,
       public MNSPSessionObserver
    {
    
   	// From MNSPSessionObserver

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
        static UT_CMceIceDirector* NewL();
        static UT_CMceIceDirector* NewLC();
        /**
         * Destructor
         */
        ~UT_CMceIceDirector();

    private:    // Constructors and destructors

        UT_CMceIceDirector();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

		void SetupL();

		void Setup2L();

		void Teardown();

		void UT_CMceIceDirector_NewLL();

		void UT_CMceIceDirector_NewSessionLL();

		void UT_CMceIceDirector_CloseSessionLL();

		void UT_CMceIceDirector_CreateOfferLL();

		void UT_CMceIceDirector_CreateOfferLAsyncL();

		void UT_CMceIceDirector_ResolveLL();
		
		void UT_CMceIceDirector_ResolveL_noSDP_L();

		void UT_CMceIceDirector_DecodeAnswerLL();

		void UT_CMceIceDirector_RestartLL();
		                    
		void UT_CMceIceDirector_GetSessionParameterLL();
		
		void UT_CMceIceDirector_GetSessionParameterLReservedL();

		void UT_CMceIceDirector_GetDefaultValuesLL();

		void UT_CMceIceDirector_CloneSdpLL();

		void UT_CMceIceDirector_UpdateAddressLL();

		void UT_CMceIceDirector_IcmpErrorL();

		void UT_CMceIceDirector_ParseErrorCodeL();

		void SetSyncMode();
		
		void SetAsyncMode();

		void SetResourceReservationStatus();
		
		void ResetResourceReservationStatus();
		
		void SetPublicAddresses();

		void TriggerIcmpErrorCallback();

		void ClearErrorCallbackProperty();

		void SetIntProperty( TUint aKey, TInt aValue );

		void SetTextProperty( TUint aKey, const TDesC8 &aValue );

		void DefineProperty( TUint aKey, TInt aAttr );

    private:    // Data

	    EUNIT_DECLARE_TEST_TABLE; 
	    
	    CMceIceDirector* iIceDirector;    
	    TUint iNatSessionId;	
	  	TNatReturnStatus iRequestStatus;

		TUint iInitializedReceived;
		TUint iOfferReadyReceived;
		TUint iAnswerReadyReceived;
		TUint iUpdateSdpReceived;
		TUint iErrorOccurredReceived;

		// How many times the IcmpErrorOccurred callback has been called
		TInt iIcmpErrorOccurredReceived;
		
		// Offer given by plugin when it has updated it asyncronously. Owned.
		CSdpDocument* iReceivedAsyncOffer;
    };

#endif      //  __UT_CMCEICEDIRECTOR_H__

// End of file
