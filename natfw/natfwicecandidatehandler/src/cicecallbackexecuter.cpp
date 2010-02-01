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




#include "cicecallbackexecuter.h"
#include "icecandidatehandlerlogs.h"

// ======== MEMBER FUNCTIONS ========

CIceCallBackExecuter::CIceCallBackExecuter( TInt aPriority )
    :
    CTimer( aPriority )
    {
    CActiveScheduler::Add( this );
    }


void CIceCallBackExecuter::ConstructL()
    {
    CTimer::ConstructL();
    }


CIceCallBackExecuter* CIceCallBackExecuter::NewL( TInt aPriority )
    {
    CIceCallBackExecuter* self = CIceCallBackExecuter::NewLC( aPriority );
    CleanupStack::Pop( self );
    return self;
    }


CIceCallBackExecuter* CIceCallBackExecuter::NewLC( TInt aPriority )
    {
    CIceCallBackExecuter* self 
        = new( ELeave ) CIceCallBackExecuter( aPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CIceCallBackExecuter::~CIceCallBackExecuter()
    {
    Cancel();
    iCallBacks.Close();
    }


// ---------------------------------------------------------------------------
// CIceCallBackExecuter::AddCallBackL
// ---------------------------------------------------------------------------
//
void CIceCallBackExecuter::AddCallBackL( const TIceCallBack& aCallBack )
    {
    __ICEDP( "CIceCallBackExecuter::AddCallBackL" )
    
    iCallBacks.AppendL( aCallBack );
    
    if ( !IsActive() )
        {
        After( aCallBack.Delay() );
        }
    }


// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CIceCallBackExecuter::RunL()
    {
    __ICEDP( "CIceCallBackExecuter::RunL" )
    __ASSERT_ALWAYS( iCallBacks.Count(), User::Leave( KErrNotFound ) );
    
    iCallBacks[0].CallBack();
    iCallBacks.Remove( 0 );
    
    if ( iCallBacks.Count() )
        {
        After( iCallBacks[0].Delay() );
        }
    }


// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CIceCallBackExecuter::DoCancel()
    {
    __ICEDP( "CIceCallBackExecuter::DoCancel" )
    
    iCallBacks.Reset();
    CTimer::DoCancel();
    }
