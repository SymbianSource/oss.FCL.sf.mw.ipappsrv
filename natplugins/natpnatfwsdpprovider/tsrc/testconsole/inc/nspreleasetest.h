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
* Description:  NAT FW SDP Provider release tests implementation.
*
*/

#ifndef NSPRELEASETEST_H
#define NSPRELEASETEST_H

#include "nsptest.h"

class CNSPPlugin;
class CRepository;
class CNSPTestConsoleSession;
class CNSPTestAsyncService;

/**
 * Release test implementation.
 */
class CNSPReleaseTest : public CNSPTest
	{
private: // Internal classes
	
	/**
	 * Release test for CNSPPlugin API method NewSession
	 */
	class CNSPTestNewSession : public CNSPTest
		{
		public: // Methods
			static CNSPTestNewSession* NewLC( CNSPPlugin& aApi, CRepository& aRep );
			virtual ~CNSPTestNewSession();
			virtual void SetupL();
			virtual void Teardown();
			virtual void ExecuteL( CConsoleBase& aConsole );
			virtual void Cancel();
		
		protected: // Methods
			CNSPTestNewSession( CNSPPlugin& aApi, CRepository& aRep );
			void ConstructL();
		
		protected: // Data
			TInt iTimerStatus;
			CNSPPlugin& iApi;
			CRepository& iRep;
			CNSPTestConsoleSession* iSession; // own
			CNSPTestAsyncService* iAsyncService; // own
		};
	
	/**
	 * Release test for CNSPPlugin API method CreateOffer
	 */
	class CNSPTestCreateOffer : public CNSPTestNewSession
		{
		public: // Methods
			static CNSPTestCreateOffer* NewLC( CNSPPlugin& aApi, CRepository& aRep );
			virtual ~CNSPTestCreateOffer();
			virtual void SetupL();
			virtual void Teardown();
			virtual void ExecuteL( CConsoleBase& aConsole );
		
		protected: // Methods
			CNSPTestCreateOffer( CNSPPlugin& aApi, CRepository& aRep );
			void ConstructL();
		};
	
	/**
	 * Release test for CNSPPlugin API method DecodeAnswer
	 */
	class CNSPTestDecodeAnswer : public CNSPTestCreateOffer
		{
		public: // Methods
			static CNSPTestDecodeAnswer* NewLC( CNSPPlugin& aApi, CRepository& aRep );
			virtual ~CNSPTestDecodeAnswer();
			virtual void SetupL();
			virtual void Teardown();
			virtual void ExecuteL( CConsoleBase& aConsole );
		
		protected: // Methods
			CNSPTestDecodeAnswer( CNSPPlugin& aApi, CRepository& aRep );
			void ConstructL();
		};
	
	/**
	 * Release test for CNSPPlugin API method Resolve
	 */
	class CNSPTestResolve : public CNSPTestNewSession
		{
		public: // Methods
			static CNSPTestResolve* NewLC( CNSPPlugin& aApi, CRepository& aRep );
			virtual ~CNSPTestResolve();
			virtual void SetupL();
			virtual void Teardown();
			virtual void ExecuteL( CConsoleBase& aConsole );
		
		protected: // Methods
			CNSPTestResolve( CNSPPlugin& aApi, CRepository& aRep );
			void ConstructL();
		};
	
	/**
	 * Release test for CNSPPlugin API method Update
	 */
	class CNSPTestUpdate : public CNSPTestNewSession
		{
		public: // Methods
			static CNSPTestUpdate* NewLC( CNSPPlugin& aApi, CRepository& aRep );
			virtual ~CNSPTestUpdate();
			virtual void SetupL();
			virtual void Teardown();
			virtual void ExecuteL( CConsoleBase& aConsole );
		
		protected: // Methods
			CNSPTestUpdate( CNSPPlugin& aApi, CRepository& aRep );
			void ConstructL();
		};
	
	
public: // Methods
	static CNSPReleaseTest* NewL( CNSPPlugin& aApi, CRepository& aRep );
	static CNSPReleaseTest* NewLC( CNSPPlugin& aApi, CRepository& aRep );
	virtual ~CNSPReleaseTest();
	virtual void SetupL();
	virtual void Teardown();
	virtual void ExecuteL( CConsoleBase& aConsole );
	virtual void Cancel();
	virtual void AcceptL( MNSPTestVisitor& aVisitor );

private: // Methods
	CNSPReleaseTest();
	void ConstructL( CNSPPlugin& aApi, CRepository& aRep );

private: // Data
	RPointerArray<CNSPTest> iTestArray;	
	};

#endif // NSPRELEASETEST_H
