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

#ifndef NSPRESULTCOLLECTOR_H
#define NSPRESULTCOLLECTOR_H

#include <e32std.h>
#include "nsptest.h"

class CConsoleBase;

/**
 * result collector implementation
 */
class CResultCollector : public CBase, public MNSPTestVisitor
	{
public: // Methods
	static CResultCollector* NewL( CConsoleBase& aConsole );
	static CResultCollector* NewLC( CConsoleBase& aConsole );
	virtual ~CResultCollector();
	
	// From MNSPTestVisitor
	void VisitL( MNSPTest& aTest );
	void VisitL( CNSPTest& aTest );
	void VisitL( CNSPReleaseTest& aTest );

private: // Methods
	CResultCollector( CConsoleBase& aConsole );
	void ConstructL();
	void PrintResult( TResult& aResult );
	
private: // Data
	CConsoleBase& iConsole;
	};

#endif // NSPRESULTCOLLECTOR_H
