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




#ifndef CMCECSSERVER_H
#define CMCECSSERVER_H

//  INCLUDES
#include <e32base.h>
#include "mceserver.pan"

//  FORWARD DECLARATION
class CMceServerCore;

// CLASS DECLARATION

/**
*  MCE server.
*
*  @lib 
*  @since
*/
class CMceCsServer : public CPolicyServer
	{
	public: // Constructors and destructor
    
		/**
		* Two-phased constructor.
		* @param aServerCore, reference to server core.
		*/
		static CMceCsServer* NewL (CMceServerCore& aServerCore);

		/**
		* Two-phased constructor.
		* @param aServerCore, reference to server core.
		*/
		static CMceCsServer* NewLC (CMceServerCore& aServerCore);

		/**
		* Destructor
		*/
		~CMceCsServer ();

	protected:
    
		TInt RunError (TInt aError);

	private:

		CMceCsServer (TInt aPriority, CMceServerCore& aServerCore);

		void ConstructL ();

		CSession2* NewSessionL (const TVersion &aVersion,
								   const RMessage2& aMessage) const;

	private:

		CMceServerCore& iServerCore;
	};

#endif

// End of File
