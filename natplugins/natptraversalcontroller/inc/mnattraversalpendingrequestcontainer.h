/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MNATTRAVERSALPENDINGREQUESTCONTAINER_H
#define MNATTRAVERSALPENDINGREQUESTCONTAINER_H

// INCLUDES
#include <e32std.h>


class MNATTraversalPendingRequestContainer
    {
    public: // New functions

        virtual void RequestCompleted( TUint32 aRequestId ) = 0;
    };

#endif // MNATTRAVERSALPENDINGREQUESTCONTAINER_H

// End of File
