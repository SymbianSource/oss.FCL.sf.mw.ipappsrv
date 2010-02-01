/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef CNATFWUNSAFQUERYCONDITIONBASE_H
#define CNATFWUNSAFQUERYCONDITIONBASE_H

//INCLUDES
#include <e32base.h>

// CONSTANTS
const TUint KSIPDefaultPort = 5060;

// FORWARD DECLARATIONS
class CNATFWUNSAFQueryBase;

// CLASS DEFINITION
/**
 * CNATFWUNSAFQueryConditionBase implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFQueryConditionBase ):
    public CBase
    {
    public:// Constructors and destructor

        virtual ~CNATFWUNSAFQueryConditionBase();

    public:

        virtual CNATFWUNSAFQueryBase& QueryBase() = 0;

        virtual CNATFWUNSAFQueryConditionBase* HandleQueryResultL(
            TInt aStatus ) = 0;

    protected:

        CNATFWUNSAFQueryConditionBase();

    };
#endif // end of CNATFWUNSAFQUERYCONDITIONBASE_H


