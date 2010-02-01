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
* Description:    Interface for querying STUN/TURN server settings
*                related software settings.
*
*/





#ifndef M_MNATFWSERVERSETTINGS_H
#define M_MNATFWSERVERSETTINGS_H


#include <e32base.h>


/**
 * Interface for querying STUN/TURN server related software settings.
 * 
 * Interface is not intended for derivation outside NATFW.
 *
 * @lib natfwnatsettings.lib
 * @since S60 v3.2
 */
class MNATFWServerSettings
    {
    
public:
    
    /**
     * Returns server address.
     *
     * @since S60 v3.2
     * @return Server address
     */
    virtual const TDesC8& Address() const = 0;
    
    /**
     * Returns server port number.
     *
     * @since S60 v3.2
     * @return Port
     */
    virtual TUint Port() const = 0;
    
    /**
     * Returns server username.
     * 
     * @since S60 v3.2
     * @return Username to the server
     */
    virtual const TDesC8& Username() const = 0;
    
    /**
     * Returns server password.
     *
     * @since S60 v3.2
     * @return Password to the server
     */
    virtual const TDesC8& Password() const = 0;
    
    /**
     * Returns is shared secred enabled.
     *
     * @since S60 v3.2
     * @return TRUE if shared secret is enabled
     */
    virtual TBool SharedSecretEnabled() const = 0;
    
    };


#endif // M_MNATFWSERVERSETTINGS_H
