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
* Description:  
*
*/





#ifndef NATFWUNSAFTRANSPORTTESTUTILS_H
#define NATFWUNSAFTRANSPORTTESTUTILS_H

#include <e32base.h>

// FORWARD DECLARATIONS
class CNATFWUNSAFMessage;


// CLASS DECLARATION
class NATFWUNSAFTransportTestUtils
    {
public:

    static CNATFWUNSAFMessage* CreateUNSAFBindingRequestL();

    static CNATFWUNSAFMessage* CreateUNSAFBindingRequestLC();

    static CNATFWUNSAFMessage* CreateUNSAFBindingResponseLC();

    };

#endif // NATFWUNSAFTRANSPORTTESTUTILS_H
