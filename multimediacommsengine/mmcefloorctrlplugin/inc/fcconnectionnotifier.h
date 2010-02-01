/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    fcconnectionnotifier.h
*
*/




#ifndef __MFCCONNECTIONNOTIFIER_H__
#define __MFCCONNECTIONNOTIFIER_H__

// INCLUDES
#include <e32std.h>

/**
 *  an interface for observing connection 
 *
 *  MFCConnectionNotifier defines an interface for observing connection 
 *  for RSocketServer. 	
 *  @lib FCTBCP.lib
 *  @since S60 v3.0
 */
class MFCConnectionNotifier
	{
	protected: // Constructors and destructors
		/**
        * Virtual destructor. Prohibit deletion through this interface.       
        */
        virtual ~MFCConnectionNotifier() {};

	public: // Abstract methods
		/**
		* Called by a CFCConnectionContainer 
		* @param aError KErrNone if connection succesfully, otherwise
		*				 one of the system-wide error codes.
		*/
		virtual void ConnectionStarted( TInt aError ) = 0;
	};

#endif // __MFCCONNECTIONNOTIFIER_H__
