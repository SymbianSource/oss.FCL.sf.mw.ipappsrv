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




#ifndef MMCESTREAMOBSERVER_H
#define MMCESTREAMOBSERVER_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CMceMediaStream;
class CMceMediaSink;
class CMceMediaSource;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  media state events.
*
*  This observer is set using CMceManager::SetMediaObserver function.
*
*  @lib mceclient.lib
*/
class MMceStreamObserver
	{
	public: 

	    /**
	    * The state of the stream has changed.
	    * @param aStream, the stream that has changed.
	    */
	    virtual void StreamStateChanged( 
	                   CMceMediaStream& aStream) = 0;

	    /**
	    * The state of the sink has changed.
	    * @param aStream, the stream that uses the sink.
	    * @param aSink, the sink that has changed.
	    */
	    virtual void StreamStateChanged(
	                    CMceMediaStream& aStream,
	                    CMceMediaSink& aSink) = 0;

	    /**
	    * The state of the source has changed.
	    * @param aStream, the stream that uses the source.
	    * @param aSource, the source that has changed.
	    */
	    virtual void StreamStateChanged(
	                    CMceMediaStream& aStream,
	                    CMceMediaSource& aSource) = 0;

	};

#endif
