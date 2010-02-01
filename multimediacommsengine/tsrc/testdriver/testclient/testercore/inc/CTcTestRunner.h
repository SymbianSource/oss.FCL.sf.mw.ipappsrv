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
* Description:  See class definition below.
*
*/

#ifndef __CTCTESTRUNNER_H__
#define __CTCTESTRUNNER_H__

// INCLUDES
#include "CTcCTRLCodec.h"
#include "MTcBearerObserver.h"
#include "TesterConstants.h"

// FORWARD DECLARATIONS
class CTcBearer;
class CTcContextFactory;
class MTcTestContext;
class MTcTestObserver;



// CLASS DEFINITION
/**
 * CTcTestRunner is responsible for co-ordinating CTRL communication using
 * CTcBearer and test execution using test contexts (MTcTestContext).
 * It owns the request and response buffers used in communication.
 * Its task is also to load and manage an arbitrary number of test context
 * plugin DLLs.
 */
class CTcTestRunner
	: public CBase,
	  public MTcBearerObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor. Note that CTcTestRunner registers as
		 * the observer of aBearer, replacing any previous observer.
		 *
		 * @param aBearer Reference to a connected bearer object.
		 * @param aObserver Test status observer
		 * @param aIAPId Internet Access Point for the tester implementation(s).
		 */
		IMPORT_C static CTcTestRunner* NewL( CTcBearer& aBearer,
											 MTcTestObserver& aObserver,
											 TInt aIAPId );

		/// Destructor
		IMPORT_C ~CTcTestRunner();

	private: // Constructors and destructor

		/**
		 * Constructor
		 *
		 * @param aBearer Reference to a connected bearer object.
		 * @param aObserver Test status observer
		 * @param aIAPId Internet Access Point for the tester implementation(s).
		 */
		CTcTestRunner( CTcBearer& aBearer,
					   MTcTestObserver& aObserver,
					   TInt aIAPId );

		/// Default constructor. Not implemented
		CTcTestRunner();

		/// 2nd phase constructor
		void ConstructL();

	public: // From MTcBearerObserver

		void BearerCompletion( MTcBearerObserver::TOperation aOp,
							   TInt aStatus );

	public: // New methods

		/// Start receivig remote requests and executing them.
		IMPORT_C void Start();

		/// Stop all processing and cease receiving remote requests.
		/// Destroy the active test context (if any)
		IMPORT_C void Stop();

		/// @return Name of test case in progress or KNullDesC8
		IMPORT_C const TDesC8& CurrentTestCaseName() const;

		/// @return Name of CTRL request in execution or KNullDesC8
		IMPORT_C const TDesC8& CurrentRequestName() const;

	private: // New methods

		/// Execute a received request
		void ExecuteL();

		/**
		 * Create a new test context based on a received START request.
		 *
		 * @param aId Request identifier.
		 */
		void CreateContextL( const TTcIdentifier& aId );

		/// Send an OK response.
		void SendOkResponse();

		/**
		 * Send an ERROR response with the specified message
		 *
		 * @param aError Error code, mapped to an error message
		 */
		void SendErrorResponse( TInt aError );

		/// Load tester ECom plugins and populate the context factory array.
		void LoadPluginsL();

	private: // Data

		/// CTRL Codec. Owned
		CTcCTRLCodec iCodec;

		/// Reference to bearer. Not owned.
		CTcBearer& iBearer;

		/// CTRL request buffer. Owned.
		TBuf8< KTcRequestMaxLength > iRequest;

		/// CTRL response buffer. Owned.
		TBuf8< KTcResponseMaxLength > iResponse;

		/// Pointer descriptor to free portion of iRequest
		TPtr8 iFreeRequest;

		/// Plugin DLL handle array, for cleanup. Owned.
		CArrayFixFlat< RLibrary > iPlugins;

		/// Array of test context factories. Owned.
		CArrayPtrFlat< CTcContextFactory > iContextFactories;

		/// Pointer to active test context or NULL. Owned.
		MTcTestContext* iContext;

		/// Reference to test observer. Not owned.
		MTcTestObserver& iObserver;

		/// Name of test case in progress or KNullDesC8. Owned.
		TBuf8< KTcMaxInfoName > iCurrentTestCase;

		/// Name of CTRL request in execution or KNullDesC8. Owned.
		TBuf8< KTcMaxInfoName > iCurrentRequest;

		/// Internet Access Point for the tester implementation(s).
		TInt iIAPId;

	};

#endif // __CTCTESTRUNNER_H__
