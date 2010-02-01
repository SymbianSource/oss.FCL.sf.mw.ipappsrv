/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Plugin interface implementation, class description.
*
*/



#ifndef CMCEICEDIRECTOR_H
#define CMCEICEDIRECTOR_H

// INCLUDES
#include "nspplugin.h"
#include "nspsessionobserver.h"
#include <e32base.h>
#include <e32svr.h>
#include <utf.h>
#include <sdpdocument.h>

// FORWARD DECLARATIONS
class CMceIcePropertyWatcher;
class CMceIceErrorTrigger;

/**
 *  NAT FW Sdp plugin interface implementation.
 *
 *  This class implements 'CNSPPlugin' ECOM interface.
 *
 *  @lib N/A
 *  @since S60 3.2
 */
class CMceIceDirector : public CNSPPlugin
    {
	public: // Constructors and destructor

	    /**
	     * A two-phase constructor.
	     */
	    static CMceIceDirector* NewL();
	    
	    /**
	     * Destructor.
	     */
	    virtual ~CMceIceDirector();

	public: // From CNSPPlugin

	    /**
	     * Creates session object, and returns a handle to that object.
	     * Client of the class uses services via this handle.
	     *
	     * @since       S60 3.2
	     * @param       aSessionObserver    Observer for created session, for
	     *									async callbacks.
	     * @param       aIapId              Internet access point identifier,
	     *									unique.
	     * @param       aProtocol           Used transport protocol, values are:
	     *                                  1 = TCP
	     *                                  2 = UDP
	     * @param       aQoS                Media quality of service value.
	     * @return      Session identifier, via this handle session is used.
	     */
	    TUint NewSessionL( MNSPSessionObserver& aSessionObserver,
	                       TUint32 aIapId,
	                       const TDesC8& aDomain,    
	                       TUint aProtocol );

	                       
	    /**
	     * With this call NAT FW session is terminated.
	     *
	     * @since       S60 3.2
	     * @param       aSessionId          Session unique identier.
	     */
	    void CloseSessionL( TUint aSessionId );
	    
	    /**
	     * Initial NAT FW specific SDP content added in here.
	     *
	     * @since       S60 3.2
	     * @param       aSessionId          Session unique identier.
	     * @param       aOffer              Sdp offer document, Object is cloned.
	     */
	    TNatReturnStatus CreateOfferL( TUint aSessionId, CSdpDocument*& aOffer );
	    
	   
	    /**
	     * NAT FW specific SDP content is added to both Offer and Answer.
	     *
	     * @since       S60 3.2
	     * @param       aSessionId      Session unique identier.
	     * @param       aOffer          Sdp offer document, Object is cloned.
	     * @param       aAnswer         Sdp answer document, Object is cloned.
	     */
	    TNatReturnStatus ResolveL( TUint aSessionId,
	                   CSdpDocument*& aOffer,
	                   CSdpDocument*& aAnswer );
	    
	    /**
	     * Answer to initial Answer interpred in here.
	     *
	     * @since       S60 3.2
	     * @param       aSessionId      Session unique identier.
	     * @param       aAnswer         Sdp answer document, Object is cloned.
	     */
	    TNatReturnStatus DecodeAnswerL( TUint aSessionId,
	    								CSdpDocument*& aAnswer );
	    
	    /**
	     * Offer can be updated while ResolveL is still ongoing, i.e.
	     * OfferReady callback is not yet received.
	     *
	     * @since       S60 3.2
	     * @param       aSessionId      Session unique identier.
	     * @param       aOffer          Sdp offer document, Object is cloned.
	     */
	    void UpdateL( TUint aSessionId, CSdpDocument*& aOffer );
	        
	    /**
	     * Empty INVITE, with certain NAT protocols, can be interpret as request
	     * to restart connection checks. This method must be called is such
	     * a message is received. If 'KNatAsync' return value is used,
	     * then async callbacks OfferReady and AnswerReady are expected.
	     *
	     * @since       S60 3.2
	     * @param       aSessionId      Session unique identier.
	     * @return      NAT status, if async signaling expected.
	     */
	     
	     TNatReturnStatus RestartL( TUint aSessionId );
	     
	    
	    /**
	     * Gets a value of a session parameter.
	     * @param aSessionId identifies the session
	     * @param aParamKey identifies the parameter
	     * @return value of the parameter or 
	     *         KErrNotFound if the value is not available or 
	     *         other system wide error code on failure.
	     */
	         
	     TInt GetSessionParam( TUint aSessionId, 
	                           TNSPSessionParamKey aParamKey );

	    /**
	     * Sets the value of a session parameter.
	     * 
	     * @param aSessionId The ID identifying session.
	     * @param aParamKey identifies the parameter.
	     * @param aParamValue value of the parameter.
	     * @return KErrNotFound if the key is not found or
	     *         other system wide error code on failure.
	     */
	     TInt SetSessionParam( TUint aSessionId,
	            TNSPSessionParamKey aParamKey, TUint aParamValue );

	private: // Constructors and destructor

	    CMceIceDirector();
	    
	    void ConstructL();


	private: // Enumerations
	    
	    /**
	     * Session services are used via methods listed below. Enumeration
	     * is used to store a current call, so that different processing
	     * can be done after state machine completes.
	     */
	    
	    // For Internal use only. 
	    
	    enum TRequestType
	        {
	        ENone,
	        ECreateOffer,
	        EDecodeAnswer,
	        EResolve,
	        EUpdate,
	        ERestart
	        };


	private:  // New private functions

		static TInt TimerExpire( TAny* aSelf );

		void StartTimer( TInt aDelay );

		TNatReturnStatus GetNatReturnStatus( TBool aStartTimer );

		void SetSecondCallbackTimer( CMceIceDirector& aIceDirector );

		void CloneSdpL( const CSdpDocument& aSource, CSdpDocument*& aDest );

		void UpdateOfferL( CSdpDocument& aOffer );
		
		void UpdateAnswerL( CSdpDocument& aAnswer );

		void UpdateAddressL( CSdpDocument& aSdpDocument,
							 const CMceIcePropertyWatcher& aProperty );

		void MakeOfferReadyCallback( CMceIceDirector& aIceDirector );

		void MakeAnswerReadyCallback( CMceIceDirector& aIceDirector );

	private: // Data

		// Observer, not owned.
		MNSPSessionObserver* iSessionOb;

		TRequestType iRequestType;

		// Has Initialized callback been done
		TBool iInitialized;
			
		// Delta Timer Active Object. Owned.
		CDeltaTimer* iDTimer;

	    TDeltaTimerEntry iDeltaEntry;
	        
	    TBool iOfferReady;
	    TBool iAnswerReady;
	        
	    // Owned Data
		TInt iSessionId; // Currently only 1 session is supported
	  	CSdpDocument* iOffer;
	    CSdpDocument* iAnswer;

		// Retrieves the local address property. Owned.
		CMceIcePropertyWatcher* iLocalAddressWatcher;
		// Retrieves the remote address property. Owned.
		CMceIcePropertyWatcher* iRemoteAddressWatcher;

		// Invokes the error callbacks. Owned.
		CMceIceErrorTrigger* iErrorTrigger;

	    // Definitions for unit testing
		friend class UT_CMceIceDirector;
 	};

#endif // CMCEICEDIRECTOR_H

// end of file
	