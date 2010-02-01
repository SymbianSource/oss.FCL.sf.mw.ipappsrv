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





#include "mcetestuiactivitymanager.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMCETestUIActivityManager* CMCETestUIActivityManager::NewL( TInt aTimeout )

    {
    CMCETestUIActivityManager* self = 
                                 new (ELeave) CMCETestUIActivityManager( aTimeout );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMCETestUIActivityManager::~CMCETestUIActivityManager()
    {
    Cancel();
    iTimer.Close();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMCETestUIActivityManager::CMCETestUIActivityManager( TInt aTimeout )
                : CActive( CActive::EPriorityHigh ),
                  iTimeout( aTimeout )

    {
    CActiveScheduler::Add(this);
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMCETestUIActivityManager::ConstructL()
    {
    iTimer.CreateLocal();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMCETestUIActivityManager::SetTimeout( TInt aTimeout )
    {
    iTimeout = aTimeout;
    Reset();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMCETestUIActivityManager::Start()
    {
    if (!IsActive())
        {
        iTimer.Inactivity( iStatus, iTimeout );
        SetActive();
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMCETestUIActivityManager::Reset()
    {
    Cancel();
    Start();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMCETestUIActivityManager::DoCancel()
    {
    iTimer.Cancel();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMCETestUIActivityManager::RunL()
    {
    
    if ( iStatus == KErrNone )
        {
        
        TInt inactivity = User::InactivityTime().Int();

        if ( inactivity >= iTimeout )
            {
            User::ResetInactivityTime();
            Reset();  
            }
        else
            {
            iTimer.Inactivity( iStatus, iTimeout );
            }
               
        if ( !IsActive() )
            {
            SetActive();
            }

        }   

    }


// end of file
