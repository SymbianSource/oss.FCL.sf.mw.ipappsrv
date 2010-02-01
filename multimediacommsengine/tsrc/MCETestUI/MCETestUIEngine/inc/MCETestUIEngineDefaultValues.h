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
* Description:    Static functions to handle default values for 
*                MCETestUIEngine objects
*
*/





#ifndef __MCETESTUIENGINEDEFAULTVALUES_H__
#define __MCETESTUIENGINEDEFAULTVALUES_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* Class MCETestUIEngineDefaultValues is a static class that provides
* UI Engine with the default values for different purposes. Values 
* are retrieved through functions.
*/
class MCETestUIEngineDefaultValues
    {

    public:

        /**
        * @return ETrue if default profile is supposed to be taken to use
        *         automatically at startup        
        */		
        static TBool TakeDefaultProfileIntoUse();

        /**
        * @return ETrue if used profiles are supposed to be registered
        *         automatically at startup        
        */		
        static TBool RegisterUsedProfiles();

        /**
        * @return Address of session's default receiver        
        */		
        static const TDesC8& DefaultSessionReceiver();


    };


#endif // __MCETESTUIENGINEDEFAULTVALUES_H__
