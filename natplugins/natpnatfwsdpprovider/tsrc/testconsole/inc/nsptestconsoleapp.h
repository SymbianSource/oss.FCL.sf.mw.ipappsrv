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

#ifndef NSPTESTCONSOLEAPP_H
#define NSPTESTCONSOLEAPP_H

#include "nsptestconsoleactions.h"

class CConsoleBase;
class CNSPPlugin;
class CNSPTestConsoleStateMachine;

class CNSPTestConsoleApp : public CActive, public MNSPTestConsoleActions
	{
public: // Methods

	static CNSPTestConsoleApp* NewL();
	static CNSPTestConsoleApp* NewLC();
	virtual ~CNSPTestConsoleApp();
	void StartTesting();
	
	// From CActive
	void RunL();
	TInt RunError( TInt aError );
	void DoCancel();
	
	// From MNSPTestConsoleActions
	void DisplayMenu( const TDesC& aMenu, TInt aError );
	void DisplaySelection( TInt aPrefix, const TDesC& aText, TBool aAddPreLinebreak );
	void DisplaySelection( TInt aPrefix, const TDesC& aText, const TDesC& aSelect, TBool aAddPreLinebreak );
	void DisplaySelection( TInt aPrefix, const TDesC& aText, TUint32 aSelect, TBool aAddPreLinebreak );
	void DisplaySelection( TInt aPrefix, const TDesC& aText, TInt aSelect, TBool aAddPreLinebreak );
	void DisplayPrompt( const TDesC& aText, TBool aAddPreLinebreak );
	void DisplayPrompt( TBool aAddPreLinebreak );
	void DisplayOne( TBool aAddPreLinebreak );
	void DisplayTwo( TBool aAddPreLinebreak );
	void DisplayThree( TBool aAddPreLinebreak );
	void DisplayFour( TBool aAddPreLinebreak );
	
	void DoRead();
	TKeyCode GetStringFromConsoleL( TDes& aBuffer );
	TKeyCode GetTUint32FromConsoleL( TUint32& aTUint32 );
	CRepository& Cenrep();
	CNSPTestManager& Manager();
	CConsoleBase& Console();

private: // Methods

	CNSPTestConsoleApp();
	void ConstructL();

private: // Data
	
	CRepository* iRepository; // own
	CNSPTestConsoleStateMachine* iStateMachine; // own
	CNSPPlugin* iPlugin; // own
	CConsoleBase* iConsole; // own
	CNSPTestManager* iTestManager; // own
	
	};

#endif // NSPTESTCONSOLEAPP_H
