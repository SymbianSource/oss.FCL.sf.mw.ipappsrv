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




#ifndef MCEFCACTIONSET_H
#define MCEFCACTIONSET_H

#include <e32base.h>
#include "mcesrv.h"
#include "mceclientserver.h"


//  FORWARD DECLARATION
class CMceSipSession;
class CMceMsgBase;
class CSdpDocument;

// CLASS DECLARATION


/**
*  MCE SIP session.
*
*  @lib 
*  @since
*/
class TMceFCActionSet 
	{
	
public: // Constructors and destructor

	/**
	* Two-phased constructor.
	* @param aSession, reference to session.
	*/
	TMceFCActionSet( CMceSipSession& aSession );

public://Floor control related

    /**
    * Update floor control related SDP lines in SDP offer.
    * @param aOffer the SDP offer
    */
	void UpdateFCOfferL( CSdpDocument& aOffer );
	
	/**
    * Update floor control related SDP lines in SDP answer.
    * @param aOffer the SDP answer
    */
	void UpdateFCAnswerL( CSdpDocument& aOffer );
	
	/**
	* Store floor control related data from SDP document
	* @param aOffer the SDP document
	*/
	void PeekFCDocumentL( CSdpDocument& aOffer );
	
	/**
	* Release floor control.
	*/
	void ReleaseFC();
    
    /**
    * Start floor control.
    */
    void StartFCL();
    
    /**
    * Initialize floor control for incoming session.
    */
	void InitializeFCL();
	
	/**
	* Initialize floor control for outgoing session.
	* @param aIds the ITC ids
	* @param aType floor control type
	*/
	void InitializeFCL( TMceIds& aIds, const TDesC8& aType );
	
	/**
	* Update floor control port.
	* @param aIds the ITC ids
	* @param aType floor control type
	*/
	void UpdateFCL( TMceIds& aIds, const TDesC8& aType );
    
    /**
    * Get floor control related SDP media lines.
    * @return aMediaLines the medialines
    */
    HBufC8* FCGetMediaLinesL();

    /**
    * Set floor control related SDP media lines.
    * @param aMediaLines the medialines
    */
	void FCSetMediaLinesL( CMceMsgBase& aMediaLines );
	
	/**
	* Send floor control message.
	* @param aMessage the message
	*/
    void SendFCMsgL( const TDesC8& aMessage );
    
    /**
    * Find out whether the floor control data has changed such that
    * new offer needs to be sent.
    * @return ETrue if new offer needs to be sent
    */
    TBool NeedToSendOffer();
    
private: // Utility 

    TUint RTCPPort();
    
private://data

    static void ArrayCleanup(TAny* aArray);

    
    CMceSipSession& iSession;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS

	};

#endif

// End of File
