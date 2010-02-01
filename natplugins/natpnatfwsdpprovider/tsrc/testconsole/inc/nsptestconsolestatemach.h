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

#ifndef NSPTESTCONSOLESTATEMACH_H
#define NSPTESTCONSOLESTATEMACH_H

#include <e32keys.h>
#include "nsptestconsoleactions.h"

typedef TInt TNSPTestConsoleStateIndex;
const TNSPTestConsoleStateIndex KStateMain = 0;
const TNSPTestConsoleStateIndex KStateSettings = 1;
const TNSPTestConsoleStateIndex KStateDomainSettings = 2;
const TNSPTestConsoleStateIndex KStateRelease = 3;
const TNSPTestConsoleStateIndex KStateRunning = 4;
const TNSPTestConsoleStateIndex KStateResults = 5;
const TNSPTestConsoleStateIndex KStateIce = 6;

class TNSPStateEvent
	{
public: // New methods
	
	inline TNSPStateEvent(
				MNSPTestConsoleActions& aActions,
				TKeyCode aKeyCode,
				TInt aStatus = KErrNone,
				TNSPTestConsoleStateIndex aNextState = KStateMain )
		: iActions( aActions ),
		  iKeyCode( aKeyCode ),
		  iStatus( aStatus ),
		  iNextState( aNextState )
		{
		}
	
	inline MNSPTestConsoleActions& Actions()
		{
		return iActions;
		}
	
	inline TKeyCode KeyCode()
		{
		return iKeyCode;
		}
	
	inline TInt& Status()
		{
		return iStatus;
		}
	
	inline TNSPTestConsoleStateIndex& NextState()
		{
		return iNextState;
		}
	
private: // data
	MNSPTestConsoleActions& iActions;
	TKeyCode iKeyCode;
	TInt iStatus;
	TNSPTestConsoleStateIndex iNextState;
	};

class TNSPTestConsoleState
	{
public:
	virtual void EntryL( TNSPStateEvent& aEvent ) = 0;
	virtual void ExitL( TNSPStateEvent& aEvent ) = 0;
	};

/**
 * Main menu.
 */
class TNPSTestConsoleStateMain : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};

/**
 * Settings menu.
 */
class TNPSTestConsoleStateSettings : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};

/**
 * Domain settings menu.
 */
class TNPSTestConsoleStateDomainSettings : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};

/**
 * Release menu.
 */
class TNPSTestConsoleStateRelease : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};

/**
 * Run menu.
 */
class TNPSTestConsoleStateRunning : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};

/**
 * Results menu.
 */
class TNPSTestConsoleStateResults : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};

/**
 * Results menu.
 */
class TNPSTestConsoleStateIce : public TNSPTestConsoleState
	{
	void EntryL( TNSPStateEvent& aEvent );
	void ExitL( TNSPStateEvent& aEvent );
	};


/**
 * State machine.
 */
class CNSPTestConsoleStateMachine : public CBase
	{
public:
	static CNSPTestConsoleStateMachine* NewL();
	~CNSPTestConsoleStateMachine();
	
	TInt Start( TNSPStateEvent& aEvent );
	void ProcessL( TNSPStateEvent& aEvent );

private:
	CNSPTestConsoleStateMachine();
	void ConstructL();

private: // data
	TNSPTestConsoleState* iCurrentState; // not own
	RPointerArray<TNSPTestConsoleState> iStateArray; // own
	};

#endif // NSPTESTCONSOLESTATEMACH_H
