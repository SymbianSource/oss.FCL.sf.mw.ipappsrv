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
* Description:   
*
*/




#ifndef CMCESTATESERVERINITIALIZING_H
#define CMCESTATESERVERINITIALIZING_H

#include "mcestate.h"


/**
*  Represents MCE server state ServerInitializing
*/
class CMceStateServerInitializing :
	public CMceState,
	public MMCEStateTransitionHandler
	{
	
public: // Constructors and destructor

	/**
	* Consructor
	*/
	CMceStateServerInitializing();

	/**
	* Destructor
	*/
	~CMceStateServerInitializing();

public: // from CMceState

    TBool DoAcceptL( TMceStateTransitionEvent& aEvent );

    inline MMCEStateTransitionHandler& TransitionHandler() 
        {
        return *this;
        }

public: // from MMCEStateTransitionHandler

    void EntryL( TMceStateTransitionEvent& aEvent );

    void ExitL( TMceStateTransitionEvent& aEvent );

private: // new functions

	void ContinueHandlingIncomingSessionL(
		TMceStateTransitionEvent& aEvent ) const;
	};

#endif // CMCESTATESERVERINITIALIZING_H

// End of File
