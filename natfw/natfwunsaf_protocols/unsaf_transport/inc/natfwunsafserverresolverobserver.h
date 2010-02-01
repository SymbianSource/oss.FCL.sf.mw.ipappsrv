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




#ifndef MNATFWUNSAFSERVERRESOLVEROBSERVER_H
#define MNATFWUNSAFSERVERRESOLVEROBSERVER_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS


// CLASS DECLARATION
/**
* A callback for CNATFWUNSAFServerResolver
*
*  @lib N/A
*/
class MNATFWUNSAFServerResolverObserver
    {
    public:

        /**
        * Resolving was completed successfully. 1..n TInetAddr(s) returned.
        * If this function leaves,
        * ErrorOccured will be called with the leave code
        */
        virtual void CompletedL() = 0;

        /**
        * An error occured while resolving or no tuples found.
        *
        * @param aError the reason for the failure.
        */
        virtual void ErrorOccured(TInt aError ) = 0;
    };

#endif // MNATFWUNSAFSERVERRESOLVEROBSERVER_H


