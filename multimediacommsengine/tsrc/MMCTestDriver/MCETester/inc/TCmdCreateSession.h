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
* Description:    See class definition below.
*
*/



#ifndef __TCmdCreateSession_H__
#define __TCmdCreateSession_H__

// INCLUDES
#include "TTcMceCommandBase.h"
#include "CSessionHelper.h"

// FORWARD DECLARATIONS
class CMceOutSession;

// CLASS DEFINITION
/**
 * Command class responsible for "Establish" functionality.
 */
class TCmdCreateSession
	: public TTcMceCommandBase
	{
	public:
	
	enum TStreamType
		{
		EStreamAudio = 0,
		EStreamVideo
		};
	
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP Test case context
		 */
		TCmdCreateSession( MTcTestContext& aContext )
							  : TTcMceCommandBase( aContext ) {};

	public: 

		void ExecuteL();
		
	public: // New methods

		/**
		 * Static function for matching the name of this command to
		 * a function identifier.
		 *
		 * @param aId An initialized identifier containing a function name.
		 * @return ETrue if this command matches the specified name.
		 */
		static TBool Match( const TTcIdentifier& aId );

		/**
		 * Static fuction for creating an instance of this class
		 *
		 * @param aContext MCE Test case context
		 * @return An initialized instance of this class.
		 */
		static TTcCommandBase* CreateL( MTcTestContext& aContext );
		
	protected:
		
		void CreateOutSessionL();
											 
		virtual CMceSession* CreateSessionL( CMceManager& aManager,
                        					 CSIPProfile& aProfile,
                        					 const TDesC8& aRecipient,
											 HBufC8* aOriginator  );
											 
		virtual CMceSession* CreateSessionL( CMceEvent& aEvent );
		virtual CMceSession* CreateSessionL( CMceRefer& aRefer );
		
		/**
		 * Removes codecs in the given order. 
		 * Adds codecs priorities according to the given order in the list 
		 * @param aStreamOut
		 * @param aStreamIn
		 * @param aCodecsList
		 * @param aType type of the stream EStreamAudio || EStreamAudio
		 */
		TInt RemoveMediaCodecsL(CMceMediaStream* aStreamOut, CMceMediaStream* aStreamIn,
				CDesC8Array* aCodecsList, TStreamType aType);

	private:
		CSessionHelper iSessionHelper;

	};

#endif // __TCmdCreateSession_H__
