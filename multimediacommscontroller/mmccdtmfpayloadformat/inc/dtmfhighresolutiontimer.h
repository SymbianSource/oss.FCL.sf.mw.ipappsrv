/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides high resolution timer services.
*
*/




#ifndef C_CDTMFHIGHRESTIMER_H
#define C_CDTMFHIGHRESTIMER_H

#include <e32base.h>

/**
 *  Provides high resolution timer services.
 *
 *  @lib mmccdtmfplformat.dll
 *  @since S60 v3.2
 */
class CDtmfHighResTimer : public CTimer
    {

public:

    /**
     * Two-phased constructor.
     * @param aPriority     Active object priority
     */
    static CDtmfHighResTimer* NewL( TInt aPriority );

    /**
     * Two-phased constructor.
     * @param aPriority     Active object priority
     */
    static CDtmfHighResTimer* NewLC( TInt aPriority );

    /**
     * Destructor.
     */
    virtual ~CDtmfHighResTimer();

    /**
     * Starts generating periodic events.
     *
     * @since S60 v3.2
     * @param aDelay        The delay from the Start() function to the 
     *                      generation of the first event, in microseconds.
     * @param anInterval    The interval between events generated after the
     *                      initial delay, in microseconds. 
     * @param aCallBack     A callback specifying a function to be called when
     *                      the CPeriodic is scheduled after a timer event.
     */
    void Start( TTimeIntervalMicroSeconds32 aDelay,
        TTimeIntervalMicroSeconds32 anInterval, TCallBack aCallBack );

protected:

// from base class CActive
    void RunL();

private:

    CDtmfHighResTimer( TInt aPriority );

    void ConstructL();

private: // data

    /**
     * Time intarval for periodic events.
     */
	TTimeIntervalMicroSeconds32 iInterval;
	
    /**
     * Callback to call when timer event occurs.
     */
	TCallBack iCallBack;
    };

#endif // C_CDTMFHIGHRESTIMER_H
