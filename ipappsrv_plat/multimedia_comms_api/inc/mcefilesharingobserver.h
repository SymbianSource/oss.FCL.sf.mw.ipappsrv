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




#ifndef MMCEFILESHARINGOBSERVER_H
#define MMCEFILESHARINGOBSERVER_H

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
*  This observer is set using CMceManager::SetFileSharingObserver function.
*
*  @lib mceclient.lib
*/
class MMceFileSharingObserver
	{
	public: 

        /**
         * Will be invoked when sending file completes
         * 
         * @return KErrNone if successful otherwise any of the system wide error
         */
	    void virtual SendFileCompleted(TInt error) = 0;
	    
	    /**
	     * Will be called when the receiving of the file completes. 
	     * @return KErrNone if successful otherwise any of the system wide error
	     */
	    void virtual ReceiveFileCompleted(TInt error) = 0;
	    
	    /**
	     * Progress notificaiton on sending the file
	     * @return aBytesTransferred. holds the part of the file size in bytes which has been set successfully
	     * @return aTotalBytesOffile. Total size of the file being sent in bytes
	     */
	    void virtual SendFileProgressNotification(TUint32 aBytesTransferred, TUint32& aTotalBytesOffile) = 0;
	    
	    /**
	     * Progress notificaiton on receiving file
	     * @return aReceivedBytes. holds the part of the file size in bytes which has been received successfully
         * @return aTotalBytesOffile. Total size of the file being sent in bytes 
	     */
	    void virtual ReceiveFileProgressNotification(TUint32 aReceivedBytes, TUint32 aTotalBytesOfFile) = 0;
	    
	};

#endif
