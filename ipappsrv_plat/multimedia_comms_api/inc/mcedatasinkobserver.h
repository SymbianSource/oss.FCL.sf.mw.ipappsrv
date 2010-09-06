/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MMCEDATASINKOBSERVER_H
#define MMCEDATASINKOBSERVER_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CMceMediaStream;
class CMceMediaSink;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  data from CMceDataSink.
*
*  This observer is set using CMceManager::SetDataSinkObserver function.
*
*  @lib mceclient.lib
*/
class MMceDataSinkObserver
	{
	public: 

	    /**
	    * Data received fro stream.
	    * @param aStream, the stream that uses the sink.
	    * @param aSink, the sink that received data.
	    * @param aData, received data.
	    */
	    virtual void DataReceived (
	                    CMceMediaStream& aStream,
	                    CMceMediaSink& aSink,
	                    const TDesC8& aData ) = 0;

	};

#endif
