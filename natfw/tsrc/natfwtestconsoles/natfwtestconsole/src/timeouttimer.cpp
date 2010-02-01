/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDE FILES
#include "TimeOutTimer.h"

// ================= MEMBER FUNCTIONS =======================

// Constructor
CTimeOutTimer::CTimeOutTimer( const TInt aPriority,
                              MTimeoutNotifier& aNotify)
: CTimer( aPriority ), iNotify( aNotify )
    {
    
    }

// Destructor
CTimeOutTimer::~CTimeOutTimer()
    {
    }

// ----------------------------------------------------
// CTimeOutTimer::ConstructL()
// Second-phase constructor
// ----------------------------------------------------
//
void CTimeOutTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------
// CTimeOutTimer::NewL()
// Two-phased construction.
// ----------------------------------------------------
//
CTimeOutTimer* CTimeOutTimer::NewL( const TInt aPriority,
                                    MTimeoutNotifier& aNotify )
    {
    CTimeOutTimer* self = CTimeOutTimer::NewLC( aPriority, aNotify );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------
// CTimeOutTimer::NewLC()
// Two-phased construction.
// ----------------------------------------------------
//    
CTimeOutTimer* CTimeOutTimer::NewLC( const TInt aPriority,
                                     MTimeoutNotifier& aNotify )
    {
    CTimeOutTimer* self = new (ELeave) CTimeOutTimer( aPriority, aNotify );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ----------------------------------------------------
// CTimeOutTimer::RunL()
// Handles an active object’s request completion event.
// ----------------------------------------------------
//    
void CTimeOutTimer::RunL()
    {
    if( iStatus == KErrNone )
        {
        iNotify.TimerExpired();
        }
    else
        {
        User::Leave(iStatus.Int());
        }
    }

TInt CTimeOutTimer::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
    
// End of file
