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

#ifndef NSPTESTCONSOLEACTIONS_H
#define NSPTESTCONSOLEACTIONS_H

#include <e32base.h>

class CRepository;
class CNSPTestManager;
class MNSPTestVisitor;

class MNSPTestConsoleActions
	{
public:
	// Write to the console
	virtual void DisplayMenu( const TDesC& aMenu , TInt aError = KErrNone ) = 0;
	virtual void DisplaySelection( TInt aPrefix, const TDesC& aText, TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplaySelection( TInt aPrefix, const TDesC& aText,
		const TDesC& aSelect, TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplaySelection( TInt aPrefix, const TDesC& aText,
		TUint32 aSelect, TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplaySelection( TInt aPrefix, const TDesC& aText,
		TInt aSelect, TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplayPrompt( const TDesC& aText, TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplayPrompt( TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplayOne( TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplayTwo( TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplayThree( TBool aAddPreLinebreak = EFalse ) = 0;
	virtual void DisplayFour( TBool aAddPreLinebreak = EFalse ) = 0;
	
	// Read from the console
	virtual void DoRead() = 0; // ASYNC
	virtual TKeyCode GetStringFromConsoleL( TDes& aBuffer ) = 0; // SYNC
	virtual TKeyCode GetTUint32FromConsoleL( TUint32& aTUint32 ) = 0; // SYNC
	
	// Central repository, used to write NAT settings
	virtual CRepository& Cenrep() = 0;
	
	// Test manager reference, used to execute tests.
	virtual CNSPTestManager& Manager() = 0;
	
	// Test console where output is directed.
	virtual CConsoleBase& Console() = 0;
	
	};

#endif // NSPTESTCONSOLEACTIONS_H
