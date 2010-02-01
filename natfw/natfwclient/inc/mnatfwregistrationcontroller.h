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
* Description:    Event registration controller interface
*
*/




#ifndef M_NATFWREGISTRATIONCONTROLLER_H
#define M_NATFWREGISTRATIONCONTROLLER_H

#include "natfweventregistration.h"

/**
*  Defines an event registration interface of CNATFWClient
*
*  @lib natconfw.lib
*  @since S60 v3.2
*/
NONSHARABLE_CLASS( MNATFWRegistrationController )
    {

public: // methods

    virtual const RArray<TNATFWEventRegistration>& Registry() = 0;
    };

#endif // M_NATFWREGISTRATIONCONTROLLER_H