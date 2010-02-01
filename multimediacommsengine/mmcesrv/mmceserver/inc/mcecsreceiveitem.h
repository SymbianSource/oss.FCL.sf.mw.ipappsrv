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




#ifndef TMCECSRECEIVEITEM_H
#define TMCECSRECEIVEITEM_H

//  INCLUDES
#include <e32base.h>
#include "mceclientserver.h"

/**
*  MCE CS server receive item for asyncronous events.
*
*  @lib 
*  @since
*/
class TMceCSReceiveItem
    {
	public: // Constructors and destructor
    
		/**
		* Constructor.
		* @param aIds, message ids.
		* @param aContext, message context.
		* @param aContent, message content.
		* @param aError, error code.
		*/         
		TMceCSReceiveItem (const TMceIds& aIds,
						   HBufC8* aContext,
						   HBufC8* aContent,
						   TInt aError=KErrNone);

		/**
		* Constructor.
		* @param aIds, message ids.
		* @param aError, error code.
		*/         
		TMceCSReceiveItem (const TMceIds& aIds, TInt aError);

	public: // New

		/**
		* Gets context of the message.
		* @return content.
		*/         
		HBufC8* Context ();

		/**
		* Gets content of the message.
		* @return content.
		*/         
		HBufC8* Content ();

		/**
		* Gets ids of the message.
		* @return ids.
		*/         
		const TMceIds& Ids () const;

		/**
		* Gets error code of the message.
		* @return error code.
		*/         
		TInt Error() const;
		
		/**
		* Gets the sizes of buffers.
		* @return sizes of buffers.
		*/         
		TMceMessageBufSizes BufSizes () const;

	private:

		TMceCSReceiveItem ();

		// data
		TMceIds iIds;
		HBufC8* iContext;
		HBufC8* iContent;
		TInt iError;
    };

#endif

// End of File
