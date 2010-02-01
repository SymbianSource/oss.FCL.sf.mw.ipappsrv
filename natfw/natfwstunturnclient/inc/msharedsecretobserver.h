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




#ifndef M_SHAREDSECRETOBSERVER_H
#define M_SHAREDSECRETOBSERVER_H

// INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  Defines the callback functions from CSTUNSharedSecret to the object using
 *  it. 
 */
class MSharedSecretObserver
    {
public:

    /**
      * Destructor
     */
    virtual ~MSharedSecretObserver() {}

    /**
     * A shared secret has been successfully obtained from a STUN server.
     */
    virtual void SharedSecretObtainedL() = 0;

    /**
     * Failure experienced while trying to get a shared secret.
     *
     * @param aError Reason for the failure.
     */
    virtual void SharedSecretErrorL( TInt aError ) = 0;
    };

#endif // M_SHAREDSECRETOBSERVER_H
