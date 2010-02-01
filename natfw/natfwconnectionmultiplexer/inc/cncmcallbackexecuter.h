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
* Description:    Executes client specified callbacks
*
*/




#ifndef C_CNCMCALLBACKEXECUTER_H
#define C_CNCMCALLBACKEXECUTER_H

#include <e32base.h>

#include "ncmconnectionmultiplexer.hrh"

/**
 *  Encapsulates callback function.
 *
 *  @lib icecandidatehandler.lib
 *  @since S60 v3.2
 */
class TNcmCallBack
    {

public:
    
    TNcmCallBack( void ( *aFunction )( CBase& aObjectRef, TUint aStreamId,
        TUint aConnectionId, TMultiplexerConnectionNotifyType aType,
        TInt aErrorCode ),
        CBase& aObjectRef, TUint aStreamId, TUint aConnectionId,
        TMultiplexerConnectionNotifyType aType, TInt aErrorCode )
        :
        iFunction( aFunction ),
        iObjectRef( aObjectRef ),
        iStreamId( aStreamId ),
        iConnectionId( aConnectionId ),
        iType( aType ),
        iErrorCode( aErrorCode )
        { };
    
    /**
     * Calls encapsulated callback function with client specified parameters.
     *
     * @since   S60 v3.2
     * return   void
     */
    void CallBack() const
        { 
        if ( iFunction )
            {
            (*iFunction)( iObjectRef, iStreamId, iConnectionId, iType,
                    iErrorCode );
            }
        }
    
    
private:
    
    TNcmCallBack();
    
public:
    
    /**
     * A pointer to the callback function.
     */
    void ( *iFunction )( CBase& aObjectRef, TUint aStreamId,
        TUint aConnectionId, TMultiplexerConnectionNotifyType aType,
        TInt aErrorCode );
    
    /**
     * A pointer that is passed to the callback function when
     * the function is called.
     */
    CBase& iObjectRef;
    
    /**
     * A stream ID that is passed to the callback function when
     * the function is called.
     */
    TUint iStreamId;
    
    /**
     * A connection ID that is passed to the callback function when
     * the function is called.
     */
    TUint iConnectionId;
    
    /**
     * A connection notify type that is passed to the callback function when
     * the function is called.
     */    
    TMultiplexerConnectionNotifyType iType;
    
    /**
     * A error code that is passed to the callback function when
     * the function is called.     
     */    
    TInt iErrorCode;        
    };


/**
 *  Executes client specified callbacks.
 *
 *  @lib connectionmux.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CNcmCallBackExecuter ) : public CActive
    {

public:

    /**
     * Two-phased constructor.
     */
    static CNcmCallBackExecuter* NewL();
    
    /**
     * Two-phased constructor.
     */
    static CNcmCallBackExecuter* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNcmCallBackExecuter();

    /**
     * Adds specified callback to the queue. Starts executing callbacks
     * if queue was empty.
     *
     * @since   S60 v3.2
     * @param   aCallBack       The callback to add to the queue
     * return   void
     */
    void AddCallBackL( const TNcmCallBack& aCallBack );

protected:
    
    void RunL();
    
    void DoCancel();
    
private:

    CNcmCallBackExecuter();
    
    void ConstructL();
    
    void ExecuteFirstCallback();

    void WaitForRequestsL();

    void Wakeup();

private: // data

    /**
     * Callback queue.
     */
    RArray<TNcmCallBack> iCallBacks;
    };

#endif // C_CNCMCALLBACKEXECUTER_H
