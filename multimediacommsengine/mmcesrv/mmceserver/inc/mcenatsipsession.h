/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CMCENATSIPSESSION_H
#define CMCENATSIPSESSION_H

#include "nspsessionobserver.h"
#include "mcenatactions.h"
#include "mcenatevents.h"
#include "mcesrv.h"
#include "mcesipsession.h"
#include <e32base.h>

// FORWARD DECLARATION
class CMceNatSipStateMachine;
class CMceComSession;
class CDesC8Array;
class TMceNatStateTransitionEvent;
class CSIPProfile;

/**
* This class represents NAT SIP session
* 
* Class provides methods to handle SIP session
* specifig ITC functions and to receive SIP requests and 
* responses.

*  @lib 
*  @since
*/
class CMceNatSipSession : public CMceSipSession,
                          public MNSPSessionObserver,
                          public MMceNatActions
	{
public: // Enumerations

    /* Nat session entry point */
    enum TMceNatEntryPoint
        {
        ENatEntryNone,
        ENatEntryUpdated,
        ENatEntryHandleSIPEvent
        };

public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @param aParams session parameters, ownership is not transferred
	* @return SIP session
	*/
	static CMceNatSipSession* NewL( CMceCsSession& aClientSession, 
	                                CMceSipConnection& aSIPConnection,
								    CMceSipStateMachine& aStateMachine,
								    CSIPProfile& aProfile,
								    const CDesC8Array* aParams = NULL );

	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @param aParams session parameters, ownership is not transferred
	* @return SIP session
	*/
	static CMceNatSipSession* NewLC( CMceCsSession& aClientSession, 
	                                 CMceSipConnection& aSIPConnection,
								     CMceSipStateMachine& aStateMachine,
								     CSIPProfile& aProfile,
								     const CDesC8Array* aParams = NULL );


	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @param aParams session parameters, ownership is not transferred
	* @return SIP session
	*/
	static CMceNatSipSession* NewL( CMceCsSession& aClientSession, 
	                                CMceSipConnection& aSIPConnection,
								    CMceSipStateMachine& aStateMachine,
								    const CDesC8Array* aParams = NULL );

	/**
	* Two-phased constructor.
	* @param aClientSession client
	* @param aSIPConnection SIP connection 
	* @param aStateMachine SIP session state machine
	* @param aParams session parameters, ownership is not transferred
	* @return SIP session
	*/
	static CMceNatSipSession* NewLC( CMceCsSession& aClientSession, 
	                                 CMceSipConnection& aSIPConnection,
								     CMceSipStateMachine& aStateMachine,
								     const CDesC8Array* aParams = NULL );
	
	/**
	* Destructor.
	*/
	~CMceNatSipSession();


public: // from CMceSipSession

    /**
    * Updates media
    * @return return status:
    *       -System error < 0: error situation
    *       -KMceReady: update was synchronous
    *       -KMceAsync: updating asynchronously
    */
    TMceReturnStatus UpdateMediaL();
    
    /**
    * Callback function to indicate MCC session is updated.
    *
    * @param        aSession            session body
    */
    void Updated( CMceComSession& aSession );

    CSdpDocument* Offer();

    TBool NatSession() const;

    TInt NatState() const;

	TBool IsNatDisabled() const;

	/**
    * Method used to check Resource Reservation Status
    *
    * @return       status TMceReturnStatus 
    *				-KMceAsync (Reservation in-progress) 
    *				-KMceReady (Resource Reserved)
    */
    TMceReturnStatus ReserveL();


public: // from MNSPSessionObserver

    /**
    * see base class
    */
    void Initialized( TUint aSessionId );
    
    /**
    * see base class
    */
    void OfferReady( TUint aSessionId, CSdpDocument* aOffer );
    
    /**
    * see base class
    */
    void AnswerReady( TUint aSessionId, CSdpDocument* aAnswer );
    
    /**
    * see base class
    */
    void UpdateSdp( TUint aSessionId, CSdpDocument* aOffer );
    
    /**
    * see base class
    */
    void ErrorOccurred( TUint aSessionId, TInt aError );

	
   /**
    * see base class
    */
    void IcmpErrorOccurred( TUint aSessionId, TInt aError );
    


public: // from MMceNatActions

    /**
    * see base class
    */
    TInt CreateOfferL();
    
    /**
    * see base class
    */
    TInt ResolveL();
    
    /**
    * see base class
    */
    TInt DecodeAnswerL();
    
    /**
    * see base class
    */
    void UpdateL();
    
    /**
    * see base class
    */
    void UpdateRemoteAddressL();
    
    /**
    * see base class
    */
    void SetOffered( TMceNatOfferedSdp aOffered );
    
    /**
    * see base class
    */
    void SdpCleanup( CSdpDocument* aPrev, CSdpDocument* aRep );
    
    /**
    * see base class
    */
    void Continue();
    
    /**
    * see base class
    */
    void StateChangedL( TMceNatStateTransitionEvent& aEvent,
                        TUint aNewState );

    void SetNeedtoReceive();

	/** 
	* Get Resource Reservation Status
	*/       
	TInt GetResourceReservationStatus();
		
	TMceReturnStatus GetReservationStatus();
	
	void SetReservationStatus(TMceReturnStatus aStatus);

	void ResumeNatUpdated();

private: // Constructors

	CMceNatSipSession( CMceCsSession& aClientSession,
	                   CMceSipConnection& aSIPConnection,
					   CMceSipStateMachine& aStateMachine,
					   CSIPProfile& aProfile );

	CMceNatSipSession( CMceCsSession& aClientSession,
	                   CMceSipConnection& aSIPConnection,
					   CMceSipStateMachine& aStateMachine );
    
	void ConstructL( CMceCsSession& aClientSession,
	                 CMceSipConnection& aSIPConnection,
	                 const CDesC8Array* aParams );
    
    void TryNatSessionL( CMceSipConnection& aSIPConnection,
                         const CDesC8Array* aParams );


private: // from CMceSipSession

    void HandleSIPEvent( TMceSipEventCode aEventCode, CSIPDialog& aDialog );


private: // New functions
    
    void SafeDelete( CSdpDocument* aDocument );
    
    void NatErrorOccurred( TInt aError );
    
    void PrepareNatSessionUpdate();
    
    void SetMediaQosL();

private: // Own Data
    
    TMceReturnStatus iReserveStatus;
    
    // Entry point from base class, stored for later use.
    TMceNatEntryPoint iEntryPoint;
    
    // Sip event code stored here for later use.
    TMceSipEventCode iSipEvent;
    
    // Session identifier from NAT FW
    TUint iSessionId;
    
    // Flag indicating which Sdp document is valid.
    TUint iOfferedSdp;
    
    // Flag indicating local failure in NAT FW
    TBool iNatDisabled;
    
    // NAT state machine, own.
    CMceNatSipStateMachine* iNatStateMachine;
    
	MCESRV_UT_DEFINITIONS
    
	};

#endif // CMCENATSIPSESSION_H

// End of File
