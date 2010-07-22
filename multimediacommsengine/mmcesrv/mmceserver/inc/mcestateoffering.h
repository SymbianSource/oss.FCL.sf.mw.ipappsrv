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




#ifndef CMCESTATEOFFERING_H
#define CMCESTATEOFFERING_H

#include "mcestate.h"
#include "mcesrv.h"

/**
*  Represents MCE server state Offering
*  State transition is performed beacuse of following
*  trigger events:
*    - EMceResponse
*    - EMceProvisionalResponse
*    - EMceRedirectionResponse
*    - EMceErrorResponse
*    - EMceAnswerDecoded
*    - EMceAnswerToUpdateDecoded
*    - EMceItcCancel
*
*/
class CMceStateOffering : public CMceState,
                          public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateOffering ();

	/**
	* Destructor.
	*/
	~CMceStateOffering();

public: // from CMceState

	/**
	* Checks if state accepts event
	* @param aEvent atransition event
	*/
    TBool DoAcceptL( TMceStateTransitionEvent& aEvent );
    
	/**
	* Reruns transition handler
	* @return transition handler
	*/
    inline MMCEStateTransitionHandler& TransitionHandler() 
        {
        return *this;
        }

public: // from MMCEStateTransitionHandler

	/**
	* Hanldes state transition message and executes
	* necessary actions
	* @param aEvent atransition event
	*/
    void EntryL( TMceStateTransitionEvent& aEvent );

	/**
	* Resolves state to which current state exit
	* @param aEvent atransition event
	*/
    void ExitL( TMceStateTransitionEvent& aEvent );


private://handler routines
	
    /**
    * Handles entry of EMceUpdate
    * @param aEvent transition event
    */
    void EntryUpdateL( TMceStateTransitionEvent& aEvent );

    /**
    * Handles entry of EMceMediaUpdated
    * @param aEvent transition event
    */
    void EntryMediaUpdatedL( TMceStateTransitionEvent& aEvent );

    /**
	* Handles entry of EMceProvisionalResponse
	* @param aEvent transition event
	*/
    void EntryProvisionalResponseL( TMceStateTransitionEvent& aEvent );

    /**
	* Handles entry of EMceRedirectionResponse
	* @param aEvent transition event
	*/
    void EntryRedirectionResponseL( TMceStateTransitionEvent& aEvent );
    
    /*
    * handle exit 3xx redirection response
    */
    void ExitRedirectionEventL( TMceStateTransitionEvent& aEvent );
    
	/**
	* Handles entry of EMceResponse
	* @param aEvent transition event
	*/
    void EntryResponseL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceErrorResponse
	* @param aEvent transition event
	*/
    void EntryErrorResponseL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceItcCancel
	* @param aEvent transition event
	*/
    void EntryCancelL( TMceStateTransitionEvent& aEvent );

	/**
	* Handles entry of EMceAnswerToUpdateDecoded
	* @param aEvent transition event
	*/
    void EntryAnswerToUpdateDecodedL( TMceStateTransitionEvent& aEvent );
    
	    
    /**
	* Handles entry of EMceBye
	* @param aEvent transition event
	*/
    void EntryByeL( TMceStateTransitionEvent& aEvent );
      
	/**
	* Handles exit of EMceResponse, EMceErrorResponse, EMceProvisionalResponse
	* @param aEvent transition event
	*/
    void ExitSIPEventL( TMceStateTransitionEvent& aEvent );
    
    void EntryAnswerToOfferDecodedL( TMceStateTransitionEvent& aEvent );
	
private:
	TInt iLastResponse;
	TBool iReadyToSendACK;

    MCESRV_UT_DEFINITIONS;
	};

#endif

// End of File
