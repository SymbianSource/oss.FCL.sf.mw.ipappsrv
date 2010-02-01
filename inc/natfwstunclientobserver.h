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
* Description:    Provides STUN Client state information.
*
*/




#ifndef MSTUNCLIENTOBSERVER_H
#define MSTUNCLIENTOBSERVER_H

// INCLUDES
#include <e32std.h>

class CSTUNClient;
class CBinding;

/**
 * An observer that must be realized by the client in order to
 * receive notifications about asynchronous STUN events.
 * This class supports also STUN relay usage.
 * 
 * @lib  natfwstunclient.lib
 */
class MSTUNClientObserver
    {
public:
    
    /**  All STUN/STUN relay binding events */
    enum TSTUNBindingEvent
        {
        /** 
         * A public address was obtained for a CSTUNBinding 
         * or for a CSTUNRelayBinding.
         * The resolved address(es) can be queried from the binding.
         * Those addresses are server reflexive IP address and
         * STUN relay IP address.
         */
        EPublicAddressResolved,
        
        /** 
         * STUN server rejected the credentials provided that were set with
         * CSTUNClient::SetCredentialsL. Application should obtain valid
         * credentials and then use CSTUNClient::SetCredentialsL.
         */
        ECredentialsRejected,
        
        EActiveDestinationSet,
        
        ETCPConnectOk
        };

    /**
     * Indicates that asynchronous initialization of the related
     * CSTUNClient has been completed and it can be used
     * for creating CSTUNBinding(s) or CSTUNRelayBinding(s).
     *
     * @param   aClient the client for which the initialization 
     *          was completed.
     * @param   aCompletionCode value KErrNone indicates that 
     *          the client instance is ready for creating CSTUNBinding(s)
     *          and/or CSTUNRelayBinding(s).
     *          Any other completion code indicates that 
     *          the client instance cannot be used and should be deleted.
     * @return  void
     */
    virtual void STUNClientInitCompleted( 
        const CSTUNClient& aClient, TInt aCompletionCode ) = 0;   

    /**
     * A STUN/STUN Relay binding related event occurred. After this callback
     * the aBinding can be used again by calling its SendRequestL or 
     * AllocateRequestL method.
     *
     * @param   aEvent the STUN/STUN relay binding related event
     * @param   aBinding the STUN binding that is the target for the event.
     * @return  void
     */
    virtual void STUNBindingEventOccurredL( 
        TSTUNBindingEvent aEvent, const CBinding& aBinding ) = 0;

    /**
     * Error occurred during IP address resolution.
     * This callback will also be called if STUNBindingEventOccuredL leaves.
     *
     * @param   aBinding a binding for which the error occured
     * @param   aError error code.
     * @errors  401: Client MUST set long term credentials and try again.
     *              Realm can be read from existing binding through 
     *              RealmFromResponse().
     *          300: Client should use alternate server address,
     *              which can be fetched from binding through 
     *              AlternateServer() -method. After fetching server binding
     *              must be destroyed.
     *          434: Client should use realm with long term credentials.
     *              Realm can be read from existing binding through 
     *              RealmFromResponse().
     *          487: ICE specific error code - role conflict occured.
     * @return  void
     */
    virtual void STUNBindingErrorOccurred( const CBinding& aBinding,
        TInt aError ) = 0;
    };

#endif // MSTUNCLIENTOBSERVER_H

