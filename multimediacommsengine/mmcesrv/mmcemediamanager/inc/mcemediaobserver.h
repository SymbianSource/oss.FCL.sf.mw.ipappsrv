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




#ifndef MMCEMEDIAOBSERVER_H
#define MMCEMEDIAOBSERVER_H

#include <e32std.h>  

class CMceComSession;
class TMceMccComEvent;

/**
 * @lib 
 */
class MMceMediaSessionObserver
    {
    public://new

        /**
        * Callback function to indicate that media has been updated
        * based on UpdateL request
        * @param aSession, media session
        */
        virtual void Updated( CMceComSession& aSession ) = 0;

        /**
        * Callback function to indicate that network resources have 
        * been reserved based on ReserveL request
        * @param aSession, media session
        */
        virtual void Reserved( CMceComSession& aSession ) = 0;
        
        /**
        * Callback function to indicate that event has been received
        * @param aEvent, received event
        */
        virtual void EventReceived( TMceMccComEvent& aEvent ) = 0;

        /**
        * Callback function to indicate that media session was stopped
        * @param aSession, media session
        */
        virtual void SessionClosed( CMceComSession& aSession ) = 0;

        /**
        * Callback function to indicate about new media session due fork
        * @param aSession, new session, onwership is been transfered
        */
        virtual void SessionCreatedL ( CMceComSession* aSession ) = 0;
        
        /**
        * Callback function to indicate about error
        * @param aEvent, received event
        */
        virtual void MediaError ( TMceMccComEvent& aEvent ) = 0;
        
        
        
    
    };

class MMceMediaGeneralObserver
    {
    public:
    
        /**
        * Callback function to indicate about error
        * @since Series 60 3.0
        * @param aError, error occured   
        * @return void
        */
        virtual void MediaError ( TInt aError ) = 0;
    };
    
#endif 
