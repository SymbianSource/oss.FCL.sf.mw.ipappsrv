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
* Description:  Stream component async state class description.
*
*/

#ifndef NSPNATFWASYNCSTATE_H
#define NSPNATFWASYNCSTATE_H

#include <e32base.h>

/**
 *  NAT FW stream state.
 *
 *  Class objects depict an async state of NAT FW stream, stream component
 *  stores/changes the state whenever it makes async call to NAT FW.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class CNSPNATFWAsyncState : public CBase
    {
public: // Enums

    /**
     * External events that can change the state of this object.
     */
    enum TNATFWEvent
        {
        ENATFWEventFetchCand,
        ENATFWEventPerformChecks,
        ENATFWEventRecvAct,
        ENATFWEventSendAct,
        ENATFWEventRecvDeAct,
        ENATFWEventSendDeAct
        };

private: // Enums

    /**
     * Internal state of this object.
     */
    enum TNATFWAsyncState
        {
        ENATFWIdle,
        ENATFWFetchCand,
        ENATFWCandChecks,
        ENATFWSend,
        ENATFWRecv,
        ENATFWSendRecv
        };    

public: // Constructors and destructor
    
    /**
     * A two-phase constructor.
     */
    static CNSPNATFWAsyncState* NewL();

    /**
     * A two-phase constructor.
     */
    static CNSPNATFWAsyncState* NewLC();

    /**
     * Copy constructor.
     */
    static CNSPNATFWAsyncState* NewL(
                const CNSPNATFWAsyncState& aState );
    
    /**
     * Copy constructor.
     */
    static CNSPNATFWAsyncState* NewLC(
                const CNSPNATFWAsyncState& aState );

    /**
     * Destructor.
     */
    virtual ~CNSPNATFWAsyncState();
    

private: // Constructors and destructor

    CNSPNATFWAsyncState();
    
    void Construct( const CNSPNATFWAsyncState& aState );


public: // New functions

    /**
     * This method is used to check if given event can be processed.
     *
     * @since       S60 3.2
     * @param       aEvent              Enumeration, external event.
     * @return      System wide error code.
     */
    TInt CheckState( TNATFWEvent aEvent ) const;
    
    /**
     * This method is used to check if given event can be processed.
     * Leave occurs if processing is not allowed.
     *
     * @since       S60 3.2
     * @param       aEvent              External event.
     */
    void CheckStateL( TNATFWEvent aEvent ) const;

    /**
     * This method is used to change to state. Use CheckState(L) 
     * method to first verify that event is acceptable. 
     *
     * @since       S60 3.2
     * @param       aEvent              External event.
     * @return      System wide error code.
     */
    TInt ChangeState( TNATFWEvent aEvent );

    /**
     * This method is used to change to state. Use CheckState(L) 
     * method to first verify that event is acceptable. 
     *
     * @since       S60 3.2
     * @param       aEvent              External event.
     */
    void ChangeStateL( TNATFWEvent aEvent );
    
    /**
     * This method is used to change to state, from async states
     * to idle, depending on current state.
     *
     * @since       S60 3.2
     * @return      ETrue if session state processing can continue.
     */
    TBool Continue();
    

private: // data

    /**
     * Current stream state is preserved.
     */
    TNATFWAsyncState iCurrentState;
    
    };

#endif // NSPNATFWASYNCSTATE_H


// end of file
