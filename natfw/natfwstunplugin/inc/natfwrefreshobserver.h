/*
* Copyright (c) 2006 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Sends binding refresh request to Stun Connection Handler.
*
*/




#ifndef C_NATFWREFRESHOBSERVER_H
#define C_NATFWREFRESHOBSERVER_H

#include <e32base.h>


/**
 *  Sends binding refresh request to Stun Connection Handler.
 *
 *  @lib stunplugin.lib
 *  @since S60 v3.2
 */
class MNATFWRefreshObserver
    {
    
public:

    /**
     * Sends binding refresh request to Stun Connection Handler.
     *
     * @since S60 v3.2
     */
    virtual void BindingRefreshL() = 0;
    
    };

#endif // C_NATFWREFRESHOBSERVER_H