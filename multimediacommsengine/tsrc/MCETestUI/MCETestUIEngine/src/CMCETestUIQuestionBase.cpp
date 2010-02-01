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



#include "CMCETestUIQuestionBase.h"


// -----------------------------------------------------------------------------
// CMCETestUIQuestionBase::CMCETestUIQuestionBase
// -----------------------------------------------------------------------------
//
CMCETestUIQuestionBase::CMCETestUIQuestionBase()
	{
	}

// -----------------------------------------------------------------------------
// CMCETestUIQuestionBase::~CMCETestUIQuestionBase
// -----------------------------------------------------------------------------
//
EXPORT_C CMCETestUIQuestionBase::~CMCETestUIQuestionBase()
	{
	delete iCaption;
	}

// -----------------------------------------------------------------------------
// CMCETestUIQuestionBase::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMCETestUIQuestionBase::Caption() const
    {
    if ( iCaption )
        {
        return *iCaption;
        }
    return KNullDesC;
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIQuestionBase::SetCaptionL
// -----------------------------------------------------------------------------
//
void CMCETestUIQuestionBase::SetCaptionL( const TDesC& aCaption )
    {
    delete iCaption;
    iCaption = NULL;
    iCaption = aCaption.AllocL();
    }


// End of File
