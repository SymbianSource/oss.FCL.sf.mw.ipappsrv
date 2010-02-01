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




#ifndef CNATFWUNSAFQUERYBASE_H
#define CNATFWUNSAFQUERYBASE_H

//INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MNATFWUNSAFHostResolver;

NONSHARABLE_CLASS( CNATFWUNSAFQueryBase ): public CBase
    {
    public:

        virtual ~CNATFWUNSAFQueryBase();

    public:

        virtual void Query( MNATFWUNSAFHostResolver& aResolver ) = 0;

        virtual void HandleQueryResultL(
            MNATFWUNSAFHostResolver& aResolver ) = 0;

    public:

        virtual const TDesC8& ResultTargetL();

//        virtual const TDesC8& ResultProtocolL() const;

        virtual TUint ResultPortL();

        virtual void RemoveElementL( TInt aIndex );

        virtual TUint ArrayCountL();

        virtual void SetTargetL( const TDesC8& aTarget );

    protected:

        CNATFWUNSAFQueryBase();

    };
#endif // end of CNATFWUNSAFQUERYBASE_H


