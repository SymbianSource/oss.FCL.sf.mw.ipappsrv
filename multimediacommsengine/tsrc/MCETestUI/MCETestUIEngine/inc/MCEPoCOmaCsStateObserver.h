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
* Description:    Handles CS call status changes
*
*/





#ifndef MCEPOCOMACSSTATEOBSERVER_H
#define MCEPOCOMACSSTATEOBSERVER_H

// first include paltform includes, like e32base.h then api etc <> files then 
// always include this file as the first omapoc related include file
//#include "PoCOmaCommonDefs.h"

// INCLUDES

// CONSTANTS

/**
* CS call states
* ref. TSACurrentCall from SACLS.H 
* ref. EPSCurrentCall from PSVariables.H 
*
*/
    // ! keep enum start value way under 100
    enum TPoCOmaCsState
        {
        EPoCOmaCallUninitialized,
        EPoCOmaCallStateNone,
        EPoCOmaCallStateVoice,
        EPoCOmaCallStateFax,
        EPoCOmaCallStateData,
        EPoCOmaCallStateAlerting,
	    EPoCOmaCallStateRinging,
	    EPoCOmaCallStateAlternating,
	    EPoCOmaCallStateDialling,
	    EPoCOmaCallStateAnswering,
	    EPoCOmaCallStateDisconnecting,
	    EPoCOmaCallStateConnected,
	    EPoCOmaCallStateHold
	    };

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Mixin class for having CS call status notifications. 
*/
class MMcePoCOmaCSStateObserver
    {
    public:
        /**
        * Called when CS Call status has changed.
        * @param aStatus Status of CS call.
        */
        virtual void CsStatusChangedL( TPoCOmaCsState aStatus ) = 0;
    };

#endif // POCOMACSSTATEOBSERVER_H

// End of File
