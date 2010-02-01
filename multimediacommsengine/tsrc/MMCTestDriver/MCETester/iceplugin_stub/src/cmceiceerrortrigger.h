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
* Description:    Generates error callbacks to client
*
*/



#ifndef CMCEICEERRORTRIGGER_H
#define CMCEICEERRORTRIGGER_H

// INCLUDES
#include "cmceicepropertywatcher.h"
#include <e32base.h>


// Forwared Declaration
class MNSPSessionObserver;

/**
 *  Generates error callbacks to the client, when the specified property is
 *  changed.
 *
 *  @lib N/A
 *  @since S60 3.2
 */
class CMceIceErrorTrigger : public CMceIcePropertyWatcher
    {
	public: // Constructors and destructor

	    /**
	     * A two-phase constructor.
	     * @param aCategory UID that identifies the property category
	     * @param aKey Property sub-key, identifying the specific property
	     */
	    static CMceIceErrorTrigger* NewL( TUid aCategory, TUint aKey );

	    /**
	     * Destructor.
	     */
	    ~CMceIceErrorTrigger();

	private: // Constructors

	    CMceIceErrorTrigger( TUid aCategory, TUint aKey );

	    void ConstructL();

	private: // From CMceIcePropertyWatcher

		void ParsePropertyL();

	public: // New functions

		/**
	     * Sets the callback interface.
	     * @param aObserver Callback where the errors will be passed
	     * @param aSessionId Session id
	     */
		void SetObserver( MNSPSessionObserver& aObserver, TInt aSessionId );

	private: // New functions

		TInt ParseErrorCode() const;

	private: // Data

		// Callback interface, not owned.
		MNSPSessionObserver* iObserver;
		TInt iSessionId;

	    // For unit testing
		friend class UT_CMceIceDirector;
 	};

#endif // CMCEICEERRORTRIGGER_H

// end of file
