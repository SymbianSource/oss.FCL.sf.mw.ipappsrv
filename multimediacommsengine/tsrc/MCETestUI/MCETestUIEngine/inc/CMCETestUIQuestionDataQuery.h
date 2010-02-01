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



#ifndef __CMCETESTUIQUESTIONDATAQUERY_H__
#define __CMCETESTUIQUESTIONDATAQUERY_H__

// INCLUDES
#include "CMCETestUIQuestionBase.h"

// FORWARD DECLARATIONS

// CLASS DEFINITION
/**
 * CMCETestUIQuestionDataQuery is question that has a caption
 * (question or prompt), single default value and ability to store answer. 
 * It expects one string as answer.
 */
class CMCETestUIQuestionDataQuery: public CMCETestUIQuestionBase
	{
	public:	// destructor

    	/**
    	* Virtual destructor.
    	*/
		IMPORT_C virtual ~CMCETestUIQuestionDataQuery();

	public:	// Constructors
	
       /**
        * Two-phased constructor. Not imported.
        */
        static CMCETestUIQuestionDataQuery* NewLC();
	
	private: // Private consructors
	
        /**
        * C++ default constructor.
        */	
        CMCETestUIQuestionDataQuery();

	public: // New methods, imported

        /**
        * @return Default answer to question
        */
        IMPORT_C const TDesC& DefaultValue() const;    

        /**
        * Sets the parameter as answer to question
        */
        IMPORT_C void SetAnswerL( const TDesC16& aAnswer );

	public: // New methods

        /**
        * @return Answer to question
        */
        const TDesC8& Answer8() const;   

        /**
        * @return Answer to question
        */        
        const TDesC16& Answer() const; 
        
        /**
        * Sets the parameter as default answer to question
        */
        void SetDefaultValueL( const TDesC16& aDefaultValue );
 
        /**
        * Sets the parameter as default answer to question
        */
        void SetDefaultValueL( const TDesC8& aDefaultValue );
        
	protected:	// Owned data

        HBufC8* iAnswer8;
		HBufC16* iAnswer;
		HBufC* iDefaultValue;
	    
    };

#endif // __CMCETESTUIQUESTIONDATAQUERY_H__
