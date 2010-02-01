/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Class definition for the MCC Qos Event Observer
*
*/




#ifndef MCCQOSEVENTOBSERVER_H
#define MCCQOSEVENTOBSERVER_H


// CLASS DECLARATION
#include <e32base.h>

/**
 * Observer which receives notification of RSubConnection(QoS) events.
 *
 * @since Series 60 3.1
 */  
class MMccQosEventObserver
    {
public:

    /**
     * Observer for QoS param related event.
     *
     * @since Series 60 3.1
     * @param aError QoS Error
     * @param aEvent Event id
     * @return void
     */
    virtual void QoSParamEvent( TInt aError, TUint32 aEvent ) = 0;

    /**
     * Observer for QoS connection related event.
     *
     * @since Series 60 3.1
     * @param aError QoS Error
     * @param aEvent Event id
     * @return void
     */
    virtual void QoSConnEvent( TInt aError, TUint32 aEvent ) = 0;
    };

#endif // MCCQOSEVENTOBSERVER_H

// End of File
