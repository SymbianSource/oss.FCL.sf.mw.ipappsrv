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
* Description:    Executes client specified callbacks in timely manner.
*
*/




#ifndef C_ICECALLBACKEXECUTER_H
#define C_ICECALLBACKEXECUTER_H

#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST

/**
 *  Encapsulates callback function.
 *
 *  @lib icecandidatehandler.lib
 *  @since S60 v3.2
 */
class TIceCallBack
    {

public:
    
    TIceCallBack( void ( *aFunction )( CBase& aObjectRef, TInt aData ),
        CBase& aObjectRef, TInt aData, 
        const TTimeIntervalMicroSeconds32& aDelay )
        :
        iFunction( aFunction ),
        iObjectRef( aObjectRef ),
        iData( aData ),
        iDelay( aDelay ) { };
    
    /**
     * Calls encapsulated callback function with client specified parameters.
     *
     * @since   S60 v3.2
     */
    void CallBack() const
        { 
        if ( iFunction )
            {
            (*iFunction)( iObjectRef, iData );
            }
        }
    
    /**
     * Returns a wait time after which callback should be called.
     *
     * @since   S60 v3.2
     */
    TTimeIntervalMicroSeconds32 Delay() const { return iDelay; }
    
private:
    
    TIceCallBack();
    
public:
    
    /**
     * A pointer to the callback function.
     */
    void ( *iFunction )( CBase& aObjectRef, TInt aData );
    
    /**
     * A pointer that is passed to the callback function when
     * the function is called.
     */
    CBase& iObjectRef;
    
    /**
     * A data that is passed to the callback function when
     * the function is called.
     */
    TInt iData;
    
    /**
     * A delay after which callback is issued.
     */    
    TTimeIntervalMicroSeconds32 iDelay;
    };


/**
 *  Executes client specified callbacks in timely manner.
 *
 *  @lib icecandidatehandler.lib
 *  @since S60 v3.2
 */
class CIceCallBackExecuter : public CTimer
    {
    
    UNIT_TEST( UT_CIceCallBackExecuter )
    
public:

    /**
     * Two-phased constructor.
     * @param aPriority         Active object priority
     */
    static CIceCallBackExecuter* NewL( TInt aPriority );
    
    /**
     * Two-phased constructor.
     * @param aPriority         Active object priority
     */
    static CIceCallBackExecuter* NewLC( TInt aPriority );
    
    /**
     * Destructor.
     */
    virtual ~CIceCallBackExecuter();

    /**
     * Adds specified callback to the queue. Starts executing callbacks
     * if queue was empty.
     *
     * @since   S60 v3.2
     * @param   aCallBack       The callback to add to the queue
     */
    void AddCallBackL( const TIceCallBack& aCallBack );

protected:
    
    void RunL();
    
    void DoCancel();
    
private:

    CIceCallBackExecuter( TInt aPriority );
    
    void ConstructL();

private: // data

    /**
     * Callback queue.
     */
    RArray<TIceCallBack> iCallBacks;
    };

#endif // C_ICECALLBACKEXECUTER_H
