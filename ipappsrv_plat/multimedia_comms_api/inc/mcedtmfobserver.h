/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MMCEDTMFOBSERVER_H
#define MMCEDTMFOBSERVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMceSession;
class CMceAudioStream;
class CMceMediaSource;

// CLASS DECLARATION

/**
*  An interface to be implemented by users of MCE if they wish to receive
*  DTMF related notifications.
*
*  @lib mceclient.lib
*/
class MMceDtmfObserver
    {    
    public:
    
        enum TMceDtmfEvent
            {
            EDtmfSendStarted,
            EDtmfSendCompleted
            };
    
    public: // New functions

        /**
        * Incoming DTMF tone received.
        * @param aSession Session that tone was received.
        * @param aStream Stream that tone was received.
        * @param aTone Received tone.
        */
        virtual void DtmfToneReceived( CMceSession& aSession,
                                       CMceAudioStream& aStream,
                                       const TChar& aTone ) = 0;
        
        /**
        * DTMF event received.
        * @param aSession Session that event was received.
        * @param aStream Stream that event was received.
        * @param aSource Source that event was received.
        * @param aEvent Received event.
        */
        virtual void DtmfEventReceived( CMceSession& aSession,
                                        CMceAudioStream& aStream,
                                        CMceMediaSource& aSource,
                                        TMceDtmfEvent aEvent ) = 0;
                                      		
        /**
        * DTMF error occured. If error occurs, DTMFs becomes
        * unavailable for that source.
        * @param aSession Session that event was received.
        * @param aStream Stream that event was received.
        * @param aStream Stream that event was received.
        * @param aError Error code.
        */
        virtual void DtmfErrorOccured( CMceSession& aSession,
                                       CMceAudioStream& aStream,
                                       CMceMediaSource& aSource,
                                       TInt aError ) = 0;
    };

#endif
