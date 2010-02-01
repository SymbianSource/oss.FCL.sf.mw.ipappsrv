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




#ifndef M_MICMPERROROBSERVER_H
#define M_MICMPERROROBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class TInetAddr;

// CLASS DECLARATION
class MIcmpErrorObserver
    {
public:

    /**
     * An ICMP occurred using the specified remote address.
     *
     * @param aAddress Remote address on which the error occurred
     */
    virtual void IcmpError( const TInetAddr& aAddress ) = 0;
    };

#endif // end of M_MICMPERROROBSERVER_H


