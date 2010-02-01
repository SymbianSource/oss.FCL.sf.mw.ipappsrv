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



#ifndef __TCmdGetStreamInfo_H__
#define __TCmdGetStreamInfo_H__

// INCLUDES
#include "TTcMceCommandBase.h"

class CMceAudioStream;
class CMceVideoStream;

// CLASS DEFINITION
/**
 * Command class responsible for GetStreamInfo functionality.
 */
class TCmdGetStreamInfo
	: public TTcMceCommandBase
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext MCE Test case context
		 */
		TCmdGetStreamInfo( MTcTestContext& aContext )
					: TTcMceCommandBase( aContext ) {};

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

	private: // Helper methods
 
 		void AddSourceToResponseL( const TDesC8& aParamNameForSourceId,
 								   const TDesC8& aParamNameForSource,
								   CMceMediaStream& aStream );

		void AddSinksToResponseL( const TDesC8& aParamNameForSinkIds,
		                         const TDesC8& aParamNameForSinks, 
								 CMceMediaStream& aStream );
								 
		void AddAudioCodecsToResponseL( const TDesC8& aName,
								  CMceAudioStream& aStream );
								  
	  	void AddVideoCodecsToResponseL( const TDesC8& aName,
								  CMceVideoStream& aStream );
	  	
	  	void GetAudioStreamInfo( CMceAudioStream* aAudioStream );
	  	
	  	void GetVideoStreamInfo( CMceVideoStream* aVideoStream );
	
	};

#endif // __TCmdGetStreamInfo_H__
