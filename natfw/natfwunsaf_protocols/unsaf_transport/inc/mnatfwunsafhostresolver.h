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




#ifndef MNATFWUNSAFHOSTRESOLVER_H
#define MNATFWUNSAFHOSTRESOLVER_H

//INCLUDES
#include <e32std.h>

class MNATFWUNSAFHostResolver
    {
    public:

        /**
         * Destructor.
         */
        virtual ~MNATFWUNSAFHostResolver () {}

        /**
         * Gets the reference of RHostResolver.
         *
         * @returns the RHostResolver
         */
        virtual RHostResolver& Resolver() = 0;

        virtual TRequestStatus& RequestStatus() = 0;

        virtual void SetIPListFailed( TBool aFailed ) = 0;
    };

#endif // MNATFWUNSAFHOSTRESOLVER_H
