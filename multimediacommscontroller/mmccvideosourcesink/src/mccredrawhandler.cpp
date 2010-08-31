/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
#include <w32std.h>
#include "mccredrawhandler.h"
#include "mccvideosourcesinklogs.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRedrawHandler::NewL
// -----------------------------------------------------------------------------
//
CMccRedrawHandler* CMccRedrawHandler::NewL( 
    RWsSession& aWsSession, 
    RWindow& aWindow, 
    CWindowGc& aGc )
    {
    CMccRedrawHandler* self = 
        new ( ELeave ) CMccRedrawHandler( aWsSession, aWindow, aGc );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccRedrawHandler::~CMccRedrawHandler
// -----------------------------------------------------------------------------
//
CMccRedrawHandler::~CMccRedrawHandler()
    {
	Cancel();
    }

// -----------------------------------------------------------------------------
// CMccRedrawHandler::BlackDrawingL
// -----------------------------------------------------------------------------
//
void CMccRedrawHandler::BlackDrawingL( TBool aIsEnabled )
    {
    __V_SOURCESINK_CONTROLL_INT1( "CMccRedrawHandler::BlackDrawingL, enabled:", 
                                  aIsEnabled )
    
    iDrawBlack = aIsEnabled;
    
    if ( DoRedrawing() )
        {
        StartRedrawing();
        }
    else
        {
        Cancel();
        }
        
    __V_SOURCESINK_CONTROLL( "CMccRedrawHandler::BlackDrawingL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccRedrawHandler::RunL
// -----------------------------------------------------------------------------
//
void CMccRedrawHandler::RunL()
    {
    TInt status( iStatus.Int() );
    __V_SOURCESINK_CONTROLL_INT1( "CMccRedrawHandler::RunL, status:", status )
    
    if ( status == KErrNone )
        {
        TWsRedrawEvent event;
        iWsSession.GetRedraw( event );
        Draw( event.Rect() );
        
        StartRedrawing();
        }
        
    __V_SOURCESINK_CONTROLL( "CMccRedrawHandler::RunL, exit" )
    }
        
// -----------------------------------------------------------------------------
// CMccRedrawHandler::DoCancel
// -----------------------------------------------------------------------------
//
void CMccRedrawHandler::DoCancel()
    {
    iWsSession.RedrawReadyCancel();
    }
            
// -----------------------------------------------------------------------------
// CMccRedrawHandler::CMccRedrawHandler
// -----------------------------------------------------------------------------
//
CMccRedrawHandler::CMccRedrawHandler( 
    RWsSession& aWsSession, 
    RWindow& aWindow, 
    CWindowGc& aGc ) :
    CActive( CActive::EPriorityStandard ),
    iWsSession( aWsSession ),
    iWindow( aWindow ),
    iGc( aGc ),
    iDrawBlack( EFalse )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CMccRedrawHandler::ConstructL
// -----------------------------------------------------------------------------
//        
void CMccRedrawHandler::ConstructL()
    {
    __V_SOURCESINK_CONTROLL( "CMccRedrawHandler::ConstructL, entry" )

    StartRedrawing();

    __V_SOURCESINK_CONTROLL( "CMccRedrawHandler::ConstructL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRedrawHandler::Draw
// -----------------------------------------------------------------------------
//
void CMccRedrawHandler::Draw( const TRect& aRect )
    {
    __V_SOURCESINK_CONTROLL_INT2( "CMccRedrawHandler::Draw rect x1:", 
                                  aRect.iTl.iX, " y1 ", aRect.iTl.iY ) 
    __V_SOURCESINK_CONTROLL_INT2( "CMccRedrawHandler::Draw rect x2:", 
                                  aRect.iBr.iX, " y2 ", aRect.iBr.iY ) 
        
    iGc.Activate( iWindow ); 
    iWindow.Invalidate( aRect );
    iWindow.BeginRedraw( aRect );

    if ( iDrawBlack )
        {
        iGc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        iGc.SetBrushColor( KRgbBlack );
        iGc.Clear( aRect );
        iDrawBlack = EFalse;
        }
    else if ( iWindow.DisplayMode() >= EColor16MA )
        { 
        // Need to set alpha mode, otherwise video cannot be seen
    	iGc.SetDrawMode( CGraphicsContext::EDrawModeWriteAlpha );
    	iGc.SetBrushColor( TRgb( 0, 0 ) );
    	iGc.Clear( aRect );
        }
	else
	    {
	    // NOP
	    }
	    
    iWindow.EndRedraw();
    iGc.Deactivate();
    iWsSession.Flush();
    
    __V_SOURCESINK_CONTROLL( "CMccRedrawHandler::Draw, exit" )
    }

// -----------------------------------------------------------------------------
// CMccRedrawHandler::DoRedrawing
// -----------------------------------------------------------------------------
//
TBool CMccRedrawHandler::DoRedrawing() const
    {
    return ( iDrawBlack || iWindow.DisplayMode() >= EColor16MA );
    }

// -----------------------------------------------------------------------------
// CMccRedrawHandler::StartRedrawing
// -----------------------------------------------------------------------------
//
void CMccRedrawHandler::StartRedrawing()
    {
    if ( DoRedrawing() && !IsActive() )
        {
        __V_SOURCESINK_CONTROLL( "CMccRedrawHandler::StartRedrawing, activate" )
        iWsSession.RedrawReady( &iStatus );
        SetActive();
        }
    }
    
// End of file

