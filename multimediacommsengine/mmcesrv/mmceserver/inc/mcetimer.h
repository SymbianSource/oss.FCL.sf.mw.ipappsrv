/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef __CMCETIMER_H
#define __CMCETIMER_H

// INCLUDES
#include <e32base.h>
#include "MTimeOutNotify.h"
#include "mcesrv.h"

// CLASS DECLARATION

/**
*  Timer class. 
*
*  @lib 
*/
class CMceTimer : public CTimer
    {
    public:
        // construct/destruct
        static CMceTimer* NewL( const TInt aPriority,
                                MTimeOutNotify& aNotify );
        ~CMceTimer();

    protected: // From CTimer
        virtual void RunL();

    private:
        // construct/destruct
        CMceTimer( const TInt aPriority, MTimeOutNotify& aNotify );
        void ConstructL();

    private:
        // pointers elsewhere
        MTimeOutNotify* iNotify;
        
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS
        
    };

#endif  // __CMCETIMER_H

// End of File