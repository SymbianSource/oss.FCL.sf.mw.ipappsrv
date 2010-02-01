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
* Description:    See class definition below.
*
*/



#ifndef __CMCETESTUIQUESTIONBASE_H__
#define __CMCETESTUIQUESTIONBASE_H__

// INCLUDES
#include <e32base.h>
#include "MCETestUIEngineConstants.h"

// FORWARD DECLARATIONS

// CLASS DEFINITION
/**
 * CMCETestUIQuestionBase implements the base class for all 
 * MCETestUI questions meant to be asked from user. It provides common 
 * interface for resolving meaning of a question. 
 */
class CMCETestUIQuestionBase: public CBase
	{

	public:	// destructor

    	/**
    	* Virtual destructor.
    	*/
		IMPORT_C virtual ~CMCETestUIQuestionBase();

	protected:	// Constructors

        /**
        * C++ default constructor.
        */	
		CMCETestUIQuestionBase();

	public: // New methods, imported

        /**
        * @return Question or prompt to be shown to user
        *        to guide user in answering.
        */
	    IMPORT_C const TDesC& Caption() const;

	public:
	
        /**
        * @param aCaption question or prompt to be shown to user
        *        to guide user in answering.
        */		
		void SetCaptionL( const TDesC& aCaption );

	protected:	// Data

		/// Owned.
		HBufC* iCaption;

	};

#endif // __CMCETESTUIQUESTIONBASE_H__
