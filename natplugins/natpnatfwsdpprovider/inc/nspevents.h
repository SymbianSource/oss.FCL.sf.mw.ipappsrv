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
* Description:  Events for NAT FW SDP Providers state machines
*
*/

#ifndef NSPEVENTS_H
#define NSPEVENTS_H

#include "mnatfwconnectivityobserver.h"

class CSdpDocument;
class CNSPSession;

/**
 * Class encapsulates callback information for session observer.
 * Execution of observer callbacks must be done by Controller - object
 * due to possible synchronous closing, and the following stack unwind.
 * 
 * @lib natfwsdpprovider.dll
 * @since S60 3.2
 */
class TEventReturnStatus
	{
public: // Enumerations & data

	enum TType /* Types of callbacks */
		{
		ENone,
		EInitialized,
		EOfferReady,
		EAnswerReady,
		EUpdateSdp,
		EError
		} iType;
	
	TInt iStatus;
	CSdpDocument* iOffer;
	CSdpDocument* iAnswer;


public: // New functions

	inline TEventReturnStatus(
			TType aType = ENone,
			TInt aStatus = KErrNone,
			CSdpDocument* aOffer = NULL,
			CSdpDocument* aAnswer = NULL )
		: iType( aType ),
		  iStatus( aStatus ),
		  iOffer( aOffer ),
		  iAnswer( aAnswer )
		{
		}
	
	};


/**
 *  State machine event.
 *
 *  Allows incoming event specific data to stored in session level, and handled
 *  in state machine level.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class TNSPStateMachineEvent : public TEventReturnStatus
    {
public: // Enumerations
    
    enum TRequest /* Types of external events */
        {
        ENat,
        ECreateOffer,
        EResolve,
        EDecodeAnswer,
        EUpdate,
        EReservationStatus,
        ECloseSession
        };


public: // New functions

    inline TNSPStateMachineEvent( TInt aStatus,
    		CSdpDocument* aOffer,
            CSdpDocument* aAnswer,
    		TRequest aRequest,
            CNSPSession& aSession )
        : TEventReturnStatus( TEventReturnStatus::ENone, aStatus, aOffer, aAnswer ),
          iRequest( aRequest ),
          iSession( aSession ),
          iNextState( KErrNone )
        {
        }
    
    inline TEventReturnStatus::TType& CallbackType()
    	{
    	return iType;
    	}
    
    inline TInt& Status()
    	{
    	return iStatus;
    	}
    
    inline CSdpDocument*& Offer()
    	{
    	return iOffer;
    	}
    
    inline CSdpDocument*& Answer()
    	{
    	return iAnswer;
    	}
    
    inline TRequest& Request()
        {
        return iRequest;
        }
    
    inline CNSPSession& Session()
        {
        return iSession;
        }
    
    inline TInt& NextState()
        {
        return iNextState;
        }


protected: // data

    TRequest iRequest;
    CNSPSession& iSession;
    TInt iNextState;
    
    };



/**
 *  State machine event, for media level specific events.
 *
 *  Allows incoming event specific data to stored in session level & media level,
 *  and handled in state machine level.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */

class TNSPStateMachineMediaEvent : public TNSPStateMachineEvent
	{
public: // New functions

	inline TNSPStateMachineMediaEvent( TUint aStreamId,
			MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
			TAny* aData,
			TInt aStatus,
            CSdpDocument* aOffer,
            CSdpDocument* aAnswer,
			TRequest aRequest,
            CNSPSession& aSession )
		: TNSPStateMachineEvent( aStatus, aOffer, aAnswer, aRequest, aSession ),
		  iStreamId( aStreamId ),
		  iEvent( aEvent ),
		  iData( aData )
		{
		}
	
	inline TUint& StreamId()
		{
		return iStreamId;
		}
	
	inline MNATFWConnectivityObserver::TNATFWConnectivityEvent& Event()
		{
		return iEvent;
		}

	inline TAny*& Data()
		{
		return iData;
		}


protected: // data

	TUint iStreamId;
	MNATFWConnectivityObserver::TNATFWConnectivityEvent iEvent;
	TAny* iData;
	
	};


#define MEDIAEVENT( aEventRef ) \
	static_cast<TNSPStateMachineMediaEvent&> ( aEventRef )


#endif // NSPEVENTS_H

