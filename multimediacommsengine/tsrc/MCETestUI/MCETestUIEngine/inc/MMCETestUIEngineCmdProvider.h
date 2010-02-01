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
* Description:    See below
*
*/




#ifndef __MMCETESTUIENGINECMDPROVIDER_H
#define __MMCETESTUIENGINECMDPROVIDER_H

#include <e32cmn.h>

class TMCETestUIEngineCmdBase;

/**
* Class MMCETestUIEngineCmdProvider is an interface.
* Implementing this interface enables controlled way
* to access implementing class. Implementing class can
* create commands that are suitable to be executed at
* certain time. These commands are returned through this
* interface.
*/
class MMCETestUIEngineCmdProvider
    {
    public:
    
        /**
        * Gets the commands applicaple to this object.
        *
        * @return reference to array of commands
        */
        virtual const RPointerArray<TMCETestUIEngineCmdBase>& 
                        GetCommandsL() = 0;
    };
    
#endif	// MMCETESTUIENGINECMDPROVIDER_H





