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

#ifndef NSPTESTMANAGER_H
#define NSPTESTMANAGER_H

#include "nsptest.h"

class CRepository;
class CConsoleBase;
class CNSPPlugin;
//class MNSPTestVisitor;
class MNSPTest;
class CNSPTest;

class CNSPTestManager : public CBase
	{
public: // Enums

	enum TestType
		{
		ESingle,
		ERelease,
		EIce
		};

public: // Methods
	static CNSPTestManager* NewL( CNSPPlugin& aApi, CRepository& aRep );
	static CNSPTestManager* NewLC( CNSPPlugin& aApi, CRepository& aRep );
	~CNSPTestManager();
	
	MNSPTest& RelTestL();
	TInt Remove( MNSPTest& aTest );
	void StartL( MNSPTest& aTest, CConsoleBase& aConsole );
	void Cancel( MNSPTest& aTest );
	void StartAllL( CConsoleBase& aConsole );
	void CancelAll();
	void AcceptL( MNSPTestVisitor& aVisitor );

private: // Methods
	CNSPTestManager( CNSPPlugin& aApi, CRepository& aRep );
	void ConstructL();
	CNSPTest* Find( TestType aType );

private: // data
	CNSPPlugin& iApi;
	CRepository& iRep;
	RPointerArray<CNSPTest> iTestArray; // own
	};

class TFinder : public MNSPTestVisitor
	{
public:
	TFinder( CNSPTestManager::TestType aType );
	void VisitL( MNSPTest& aTest );
	void VisitL( CNSPTest& aTest );
	void VisitL( CNSPReleaseTest& aTest );
	CNSPTest* Ptr();

protected:
	CNSPTestManager::TestType iType;
	CNSPTest* iTest; // not own
	};

#endif // NSPTESTMANAGER_H
