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



#ifndef __SIP_INTERNAL_STATES_H__
#define __SIP_INTERNAL_STATES_H__

// INCLUDES
#include <e32base.h>
#include "_sipcodecdefs.h"

// FORWARD DECLARATIONS
class RSIP;
class CSIPTransactionBase;
class CSIPDialog;
class TSIPIds;

// CLASS DECLARATION
/**
 * @brief Class for getting transaction and dialog internal to the SIP stack
 *        This class is meant for testing purposes
 */
class CSIPInternalStates : public CBase
	{
public:

	/** SIP transaction state */
	enum TState
		{
		/** Trying state */
		ETransactionTrying=1,
		/** Calling state */
		ETransactionCalling=2,
		/** Proceeding state */
		ETransactionProceeding=3,
		/** Completed state */
		ETransactionCompleted=4,
		/** Confirmed state */
		ETransactionConfirmed=5,
		/** Terminated state */
		ETransactionTerminated=6,
		/** Establishing state */
		EDialogEstablishing=7,
		/** Early state */
		EDialogEarly=8,
		/** Confirmed state */
		EDialogConfirmed=9,
		/** Terminated state */
		EDialogTerminated=10
		};

public: // Constructors and destructor
	/**
	 * Constructs an object
	 * @returns new instance, the ownership is transferred to the caller
	 */
	IMPORT_C static CSIPInternalStates* NewL ();

	/**
	 * Constructs an object and adds the pointer to the cleanup stack
     * @returns new instance, the ownership is transferred to the caller
	 */
	IMPORT_C static CSIPInternalStates* NewLC ();

	/**
	 * Destructor
	 */
	IMPORT_C ~CSIPInternalStates ();

public: // New functions
    /**
     * Gets the state of a transaction
     *
     * @param aTransaction a transaction
     * @param aState on return contains the state of transaction if found
     *
	 * @returns KErrNone if a transaction was found. 
     *          Otherwise a system wide error code.
	 */
    IMPORT_C TInt GetTransactionState (const CSIPTransactionBase& aTransaction,
                                       TState& aState);

    /**
     * Gets the state of a dialog
     *
     * @param aDialog a dialog
     * @param aState on return contains the state of dialog if found
     *
	 * @returns KErrNone if a dialog was found. 
     *          Otherwise a system wide error code.
	 */
    IMPORT_C TInt GetDialogState (const CSIPDialog& aDialog,
                                  TState& aState);

    /**
     * Gets the current count of compartments used by SigComp.
     *
	 * @returns the compartment count or a system wide error code
	 */
    IMPORT_C TInt SigCompCompartmentCount ();

public: // New functions, for internal use
	static TInt
    	GetState (RSIP& aSip, TSIPIds& aIds, TState& aState, TInt aFunction);

private: // Constructors
    CSIPInternalStates ();
    void ConstructL ();

private: // Data
    RSIP* iSip;

	// Unit test class declared as friend
	UNIT_TEST(CSIPInternalStatesTest)
	};

#endif // end of __SIP_INTERNAL_STATES_H__

// End of File
