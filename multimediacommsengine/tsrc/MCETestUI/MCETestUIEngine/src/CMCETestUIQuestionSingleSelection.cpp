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
* Description:    Implementation.
*
*/



#include <badesca.h>
#include "CMCETestUIQuestionSingleSelection.h"

// -----------------------------------------------------------------------------
// CMCETestUIQuestionSingleSelection::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIQuestionSingleSelection* CMCETestUIQuestionSingleSelection::NewLC()
    {

    CMCETestUIQuestionSingleSelection* self = 
            new (ELeave) CMCETestUIQuestionSingleSelection();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionSingleSelection::CMCETestUIQuestionSingleSelection
// -----------------------------------------------------------------------------
//
CMCETestUIQuestionSingleSelection::CMCETestUIQuestionSingleSelection() 
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIQuestionSingleSelection::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIQuestionSingleSelection::ConstructL()
    {
    iChoices = new CDesCArrayFlat( 1 );
    }

// -----------------------------------------------------------------------------
// CMCETestUIQuestionSingleSelection::~CMCETestUIQuestionSingleSelection
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIQuestionSingleSelection::~CMCETestUIQuestionSingleSelection()
	{
	iChoices->Reset();
	delete iChoices;
	}

// -----------------------------------------------------------------------------
// CMCETestUIQuestionSingleSelection::Choices
// -----------------------------------------------------------------------------
//
EXPORT_C MDesCArray& CMCETestUIQuestionSingleSelection::Choices() const
    {
    return *iChoices;    
    }


// -----------------------------------------------------------------------------
// CMCETestUIQuestionSingleSelection::AddChoiceL
// -----------------------------------------------------------------------------
//
void CMCETestUIQuestionSingleSelection::AddChoiceL( const TDesC& aChoice )
    {
    iChoices->AppendL( aChoice );
    }




// End of File
