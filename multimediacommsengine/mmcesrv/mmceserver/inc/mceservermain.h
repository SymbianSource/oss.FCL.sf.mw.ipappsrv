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




#ifndef MCESERVERMAIN_H
#define MCESERVERMAIN_H

//  INCLUDES
#include <e32base.h>
#include "mceserver.pan"

/**
*  MCE CS server main class.
*
*  @lib 
*  @since
*/
class MceServerMain
	{
	public : // new
    
		/**
		* Starts thread for the MCE server.
		*/         
		static TInt ThreadFunction (TAny* aNone);

	private:

		MceServerMain ();

		static void PanicServer (TMceServerPanic aReason);

		static void ThreadFunctionL (RSemaphore& aSemaphore);
	};

#endif

// End of File
