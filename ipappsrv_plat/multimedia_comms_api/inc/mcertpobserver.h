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




#ifndef MMCERTPOBSERVER_H
#define MMCERTPOBSERVER_H

// INCLUDES
#include <e32std.h>  

// FORWARD DECLARATIONS
class CMceMediaStream;
class CMceRtpSource;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  events from CMceRtpSource.
*
*  This observer is set using CMceManager::SetRtpObserver function.
*
*  @lib mceclient.lib
*/
class MMceRtpObserver
	{
	public: 
        /**
        * Callback function to indicate that an RTCP sender report
		* has been received.
        * @param aSession the session that has changed
        * @param aStream the stream that has changed
        */
        virtual void SRReceived(
				    CMceSession& aSession,
					CMceMediaStream& aStream) = 0;

        /**
        * Callback function to indicate that an RTCP receiver report
		* has been received.
        * @param aSession the session that has changed
        * @param aStream the stream that has changed
        */
        virtual void RRReceived(
				    CMceSession& aSession,
					CMceMediaStream& aStream) = 0;
		
        /**
        * Callback function to indicate inactivity timer timeout
		* has occurred. Timer is disabled after the timeout.
        * @param aStream the stream that has changed
        * @param aSource, the source that the timeout occurred.
        */
        virtual void InactivityTimeout(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource) = 0;       
		
		/**
        * Callback function to indicate that spesified CMceRtpSource has
        * received RTP from a new synchronization source (SSRC).
        * @param aStream the stream that new SSRC was added to
        * @param aSource the source that new SSRC was added to
        * @param aSsrc added SSRC identifier
        */			
		virtual void SsrcAdded(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource,
					TUint aSsrc ) = 0;       
	    
	    /**
        * Callback function to indicate that spesified CMceRtpSource has
        * received RTP BYE message from a spesified synchronization 
        * source (SSRC).
        * @param aStream the stream that SSRC was removed from
        * @param aSource the source that SSRC was removed from
        * @param aSsrc removed SSRC identifier
        */	
		virtual void SsrcRemoved(
					CMceMediaStream& aStream,
					CMceRtpSource& aSource,
					TUint aSsrc ) = 0;       
	};

#endif 