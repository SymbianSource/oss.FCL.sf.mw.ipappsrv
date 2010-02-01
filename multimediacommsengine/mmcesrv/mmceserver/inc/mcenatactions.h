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



#ifndef CMCENATACTIONS_H
#define CMCENATACTIONS_H

#include <e32base.h>
#include "mcedefs.h"

class TMceNatStateTransitionEvent;
class CMceComSession;
class CSdpDocument;

const TInt KMceNatNonFatalErrorLimit = -11000;

typedef TUint TMceNatOfferedSdp;
const TMceNatOfferedSdp KMceNatOfferSdp = 0;
const TMceNatOfferedSdp KMceNatPrevOfferSdp = 1;

/**
*  MCE NAT SIP actions given to a single state.
*
*  @lib 
*  @since
*/
class MMceNatActions
	{
public: // New functions
    
	/**
	* Creates offer.
	*/
    virtual TInt CreateOfferL() = 0;
    
	/**
	* Creates answer, and afterwards decodes offer.
	*/
    virtual TInt ResolveL() = 0;
    
	/**
	* Decodes answer.
	*/
    virtual TInt DecodeAnswerL() = 0;
    
	/**
	* Updates MT/(MO) INVITE to NAT plugin, takes sdp doc from Offer()
	*/
    virtual void UpdateL() = 0;
    
    /**
    * Any existing mcc streams have probably wrong transport addres,
    * this method is used to update correct address to mcc streams.
    */
    virtual void UpdateRemoteAddressL() = 0;
    
    /**
    * Sets flag state which indicates which Sdp document is valid for sending
    * out to NET. Usually NOT loopback/local address Sdp's !!
    */
    virtual void SetOffered( TMceNatOfferedSdp aOffered ) = 0;
    
    /**
    * Replaces current SdpSession pointer with the current Offer.
    */
    virtual void SdpCleanup( CSdpDocument* aPrev, CSdpDocument* aRep ) = 0;
    
    /**
    * Called by state machine after NAT processing is completed.
    */
    virtual void Continue() = 0;
    
	/**
	* State of the state machine is changed via this method.
	*/
    virtual void StateChangedL( TMceNatStateTransitionEvent& aEvent,
                                TUint aNewState ) = 0;

	/**
	* Set Need to receive forces MCE to send provisional response reliably.
	* used if resource reservation is needed.
	*/
	                                
	virtual void SetNeedtoReceive() = 0; 
	
	/**
	* Get the Resource Reservation Status, returns the resource reservation status
	*/
	virtual TInt GetResourceReservationStatus() = 0; 
	
	
	virtual TMceReturnStatus GetReservationStatus() = 0;
	
	virtual void SetReservationStatus(TMceReturnStatus aStatus) = 0;

	/**
	* Continue processing NAT updated event, that was delayed because NAT plugin
	* had not yet been initialized.
	*/
	virtual void ResumeNatUpdated() = 0;
	};

#endif // CMCENATACTIONS_H

// End of File
