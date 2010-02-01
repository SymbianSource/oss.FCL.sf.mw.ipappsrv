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
* Description:    Implementation.
*
*/




//INCLUDES

#include "CMCETestUIQuestionDataQuery.h"

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIQuestionDataQuery* CMCETestUIQuestionDataQuery::NewLC()
    {

    CMCETestUIQuestionDataQuery* self = 
            new (ELeave) CMCETestUIQuestionDataQuery();
    CleanupStack::PushL( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::CMCETestUIQuestionDataQuery
// -----------------------------------------------------------------------------
//
CMCETestUIQuestionDataQuery::CMCETestUIQuestionDataQuery() 
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::~CMCETestUIQuestionDataQuery
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIQuestionDataQuery::~CMCETestUIQuestionDataQuery()
	{
	delete iAnswer8;
	delete iAnswer;
	delete iDefaultValue;
	}

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::SetAnswerL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMCETestUIQuestionDataQuery::SetAnswerL( const TDesC16& aAnswer )
    {
    delete iAnswer8;
    delete iAnswer;
    iAnswer = aAnswer.AllocL();
    iAnswer8 = HBufC8::NewL( iAnswer->Length() );
    TPtr8 ptrAnswer8 = iAnswer8->Des();
    ptrAnswer8.Copy( *iAnswer );  
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::Answer
// -----------------------------------------------------------------------------
//
const TDesC16& CMCETestUIQuestionDataQuery::Answer() const
    {
    if ( iAnswer )
        {
        return *iAnswer;
        }
    return KNullDesC16;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::Answer8
// -----------------------------------------------------------------------------
//
const TDesC8& CMCETestUIQuestionDataQuery::Answer8() const
    {
    if ( iAnswer8 )
        {
        return *iAnswer8;
        }
    return KNullDesC8;    
    }   

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::DefaultValue
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMCETestUIQuestionDataQuery::DefaultValue() const
    {
    if ( iDefaultValue )
        {    
        return *iDefaultValue;    
        }
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::SetDefaultValueL
// -----------------------------------------------------------------------------
//
void CMCETestUIQuestionDataQuery::SetDefaultValueL( 
            const TDesC16& aDefaultValue )
    {
    delete iDefaultValue;
    iDefaultValue = aDefaultValue.AllocL(); 
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionDataQuery::SetDefaultValueL
// -----------------------------------------------------------------------------
//
void CMCETestUIQuestionDataQuery::SetDefaultValueL( const TDesC8& aDefaultValue )
    {
    delete iDefaultValue;
    HBufC* tmp = HBufC::NewL( aDefaultValue.Length() );
    TPtr ptr = tmp->Des(); 
    ptr.Copy( aDefaultValue );
    iDefaultValue = tmp; 
    }



// End of File
