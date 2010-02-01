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



#ifndef __CSessionHelper_H__
#define __CSessionHelper_H__

// INCLUDES
#include <e32std.h>

class CMceCodec;
class CMceMediaStream;
class CMceManager;

// CLASS DEFINITION
/**
 * Class providing common functionality for CreateSession and UpdateSession
 * commands
 */
class CSessionHelper
	{
	public:	// Constructors and destructor

		/**
		 * Constructor.
		 */
		CSessionHelper( ) {};

	public: 
		enum priorities 
		{
			uplink,
			downlink,
			dtmf	
		};
		/**
		Sets given predefined priorities to a given codec
		@param aCodec Codec to be modified
		@param aDownlink Direction of the stream the codec belongs to
		*/
		void SetCodecPrioritiesL(CMceCodec& aCodec, priorities aPri);
		
		/**
		Sets given priorities and preferences to a given codec
		@param aCodec Codec to be modified
		@param aPri Priority
		@param aPref Preference
		*/
		void SetCodecPriPrefL(CMceCodec& aCodec, TUint aPri, TUint aPref);
		
		/**
		Sets RTP keepalive values for a given codec
		@param
		*/
		void SetKeepaliveL(CMceCodec& aCodec, TInt aKeepaliveTimeSec);
		
		/**
		 * Sets RTP InactivityTimer for given media stream
		 */
		void SetRtpInactivityTimerL(CMceMediaStream& aStream, TInt aInactivityTimer);
		
		/**
		Creates filesource from the given file and sets it to stream.
		@param aManager
		@param aStream
		@param aFilename
		*/
		void SetMediaFileSourceL(CMceManager& aManager, CMceMediaStream& aStream, TPtrC8 aFilename );
		
		/**
		Creates filesink from the given file and sets it to instream.
		@param aStream
		@param aFilename
		*/
		void SetMediaFileSinkL(CMceMediaStream& aStream, TPtrC8 aFilename );
	};

#endif // __CSessionHelper_H__
