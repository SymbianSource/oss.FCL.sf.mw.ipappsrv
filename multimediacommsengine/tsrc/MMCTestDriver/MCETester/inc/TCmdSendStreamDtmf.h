/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TCmdSendStreamDtmf_H__
#define __TCmdSendStreamDtmf_H__

// INCLUDES
#include "TTcMceCommandBase.h"
#include "CDelayedProcess.h"

class CMceAudioStream;

// CLASS DEFINITION
/**
 * Command class responsible for "SendStreamDtmf" functionality.
 */
class TCmdSendStreamDtmf
	: public TTcMceCommandBase
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext MCE Test case context
		 */
		TCmdSendStreamDtmf( MTcTestContext& aContext )
					: TTcMceCommandBase( aContext ),iaudioStream(NULL),iDtmfTimer(NULL) {};
		

	public: // From TTcMceCommandBase

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
		
	private:

		CMceAudioStream* iaudioStream;
		CDelayedProcess* iDtmfTimer;
	};

#endif // __TCmdSendStreamDtmf_H__