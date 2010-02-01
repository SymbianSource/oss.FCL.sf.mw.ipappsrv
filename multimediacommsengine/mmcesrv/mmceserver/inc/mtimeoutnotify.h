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





#ifndef MCETIMEOUTNOTIFY_H
#define MCETIMEOUTNOTIFY_H

#include <e32std.h>

/**
*  Class for timeout callback.
*
*  @lib
*/
class MTimeOutNotify
    {
    public:
		/**
	    * Callback function to indicate about expiration of
	    * the timer
	    * @since Series 60 3.0
	    * @param aStatus
	    * @return void
	    */
        virtual void OnExpired( TInt aStatus ) = 0;
    };

#endif // end of MCETIMEOUTNOTIFY_H