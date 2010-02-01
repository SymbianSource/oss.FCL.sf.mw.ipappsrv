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



#ifndef __CMCETESTUIQUESTIONSINGLESELECTION_H__
#define __CMCETESTUIQUESTIONSINGLESELECTION_H__

// INCLUDES
#include "CMCETestUIQuestionBase.h"

// FORWARD DECLARATIONS

// CLASS DEFINITION
/**
 * CMCETestUIQuestionSingleSelection is question that has a caption
 * (question or prompt) and several choices.
 */
class CMCETestUIQuestionSingleSelection: public CMCETestUIQuestionBase
	{
	public:	// destructor

		/// Virtual destructor
		IMPORT_C virtual ~CMCETestUIQuestionSingleSelection();

	public:	// Constructors
	
       /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIQuestionSingleSelection* NewLC();
	
	private: // Private consructors
	
        /**
        * C++ default constructor.
        */
		CMCETestUIQuestionSingleSelection();

       	/**
        * Second-phase constructor
        */
        void ConstructL();

	public: // New methods, imported

        /* 
        * Gets available choices
        *
        * @return Reference to array of choices
        */
        IMPORT_C MDesCArray& Choices() const;    

	public: // New methods
        
        /*
        * Appends a set of choices with a choice given as parameter
        */
        void AddChoiceL( const TDesC& aChoice );

	protected:	// Data

		/// Owned.
		CDesCArray* iChoices;
	
    };

#endif // __CMCETESTUIQUESTIONSINGLESELECTION_H__
