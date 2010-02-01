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




#include "dtmfhighresolutiontimer.h"
#include "dtmfpayloadformatdefs.h"

// ---------------------------------------------------------------------------
// CDtmfHighResTimer::CDTMFPayloadDecoder
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CDtmfHighResTimer::CDtmfHighResTimer( TInt aPriority )
    :
    CTimer( aPriority )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CDtmfHighResTimer::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CDtmfHighResTimer::ConstructL()
    {
    CTimer::ConstructL();
    }


// ---------------------------------------------------------------------------
// CDtmfHighResTimer::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDtmfHighResTimer* CDtmfHighResTimer::NewL( TInt aPriority )
    {
    CDtmfHighResTimer* self = CDtmfHighResTimer::NewLC( aPriority );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CDtmfHighResTimer::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CDtmfHighResTimer* CDtmfHighResTimer::NewLC( TInt aPriority )
    {
    CDtmfHighResTimer* self = new( ELeave ) CDtmfHighResTimer( aPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CDtmfHighResTimer::~CDtmfHighResTimer
// Destructor.
// ---------------------------------------------------------------------------
//
CDtmfHighResTimer::~CDtmfHighResTimer()
    {
    
    }


// ---------------------------------------------------------------------------
// CDtmfHighResTimer::Start
// ---------------------------------------------------------------------------
//
void CDtmfHighResTimer::Start(
        TTimeIntervalMicroSeconds32 aDelay,
        TTimeIntervalMicroSeconds32 anInterval,
        TCallBack aCallBack )
    {
    DP_DTMF_WRITE3( _L("CDtmfHighResTimer::Start, DELAY: %d, INTERVAL: %d"),
        aDelay.Int(), anInterval.Int() );
    ASSERT( aDelay.Int() >= 0 );
    ASSERT( anInterval.Int() >= 0 );
    
	iInterval = anInterval.Int();
	iCallBack = aCallBack;
	CTimer::HighRes( aDelay );
    }


// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CDtmfHighResTimer::RunL()
    {
    CTimer::HighRes( iInterval );
	iCallBack.CallBack();
    }
