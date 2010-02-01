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




#include "mcedisplaysink.h"
#include "mcecomdisplaysink.h"
#include "mcesession.h"
#include "mce.h"
#include "mceitcsender.h"
#include "mcemediastream.h"
#include "mcemanager.h"
#include "mceavsink.h"
#include "mceclilogs.h"

#include <w32std.h>


#define _FLAT_DATA static_cast<CMceComDisplaySink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink* CMceDisplaySink::NewL( CMceManager& aManager )
    {
    CMceDisplaySink* self = CMceDisplaySink::NewLC( aManager );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink* CMceDisplaySink::NewLC( CMceManager& aManager )
    {
    CMceDisplaySink* self = new (ELeave) CMceDisplaySink();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink::~CMceDisplaySink()
    {
    if ( iEndpointProxy )
        {
        CMceAvSink* endpointProxy = static_cast<CMceAvSink*>( iEndpointProxy );
        endpointProxy->RemoveProxyClient( *this );
        if ( endpointProxy->ReferenceCount() == 0 )
            {
            delete endpointProxy;
            }
        iEndpointProxy = NULL;
        }
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::EnableL()
    {
    MCECLI_DEBUG("CMceDisplaySink::EnableL, Entry");
    
    CMceMediaSink::DoEnableL();

    MCECLI_DEBUG("CMceDisplaySink::EnableL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::DisableL()
    {
    MCECLI_DEBUG("CMceDisplaySink::DisableL, Entry");
    
    CMceMediaSink::DoDisableL();

    MCECLI_DEBUG("CMceDisplaySink::DisableL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceDisplaySink::DisplaysAvailable() const
    {
    return FLAT_DATA( iDisplaysAvailable );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetDisplayIndexL( TInt aDisplayIndex )
    {
    MCECLI_DEBUG("CMceDisplaySink::SetDisplayIndexL, Entry");
    MCECLI_DEBUG_DVALUE("index", aDisplayIndex );
    
    __ASSERT_ALWAYS( aDisplayIndex >= 0 &&
                     aDisplayIndex < this->DisplaysAvailable(),
                     User::Leave( KErrArgument ) );
                     
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iDisplayIndex ) = SetITCValueL( EMceItcSetDisplayIndex,
                                                   aDisplayIndex );
        }
    else
        {
    	FLAT_DATA( iDisplayIndex ) = aDisplayIndex;
        MCECLI_DEBUG("CMceDisplaySink::SetZoomFactorL, done locally");
        }      
    MCECLI_DEBUG("CMceDisplaySink::SetDisplayIndexL, Exit");
    }   
	    
	    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMceDisplaySink::SetDisplay( RWindow& aWindow, CWindowGc& aGc )
    {
    iWindow = &aWindow;
    iGc = &aGc;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetDisplayRectL( const TRect& aRect )
    {
    MCECLI_DEBUG("CMceDisplaySink::SetDisplayRectL, Entry");
    MCECLI_DEBUG_DVALUES("top left X", aRect.iTl.iX, "top left Y", aRect.iTl.iY );
    MCECLI_DEBUG_DVALUES("bottom right X", aRect.iBr.iX, "bottom right Y", aRect.iBr.iY );
    
    FLAT_DATA( iDisplayRectTlX ) = aRect.iTl.iX;
	FLAT_DATA( iDisplayRectTlY ) = aRect.iTl.iY;
	FLAT_DATA( iDisplayRectBrX ) = aRect.iBr.iX;
	FLAT_DATA( iDisplayRectBrY ) = aRect.iBr.iY;
    	
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        CMceSession* session = iStream->Session();
        
    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
    	ids.iSinkID  = Id();
    	
    	TMceItcArgTRect display( aRect );
    	
        session->ITCSender().WriteL( ids, EMceItcSetDisplayRect, display );    
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceDisplaySink::SetDisplayRectL, done locally");
        }      
        
    MCECLI_DEBUG("CMceDisplaySink::SetDisplayRectL, Exit");
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const TRect CMceDisplaySink::DisplayRectL()
    {
    MCECLI_DEBUG("CMceDisplaySink::DisplayRectL, Entry");
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {        
        CMceSession* session = iStream->Session();

        TRect displayRectangle;
        
        // Receive displayRect
        TPckgBuf<TRect> displayRect( displayRectangle );
        
        TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID  = iStream->Id();
    	ids.iSinkID = Id();
    	
        session->ITCSender().ReadL( ids, EMceItcDisplayRect, displayRect );
        
        displayRectangle = displayRect();
        
        FLAT_DATA( iDisplayRectTlX ) = (TUint)(displayRectangle.iTl.iX);
        FLAT_DATA( iDisplayRectTlY ) = (TUint)(displayRectangle.iTl.iY);
        FLAT_DATA( iDisplayRectBrX ) = (TUint)(displayRectangle.iBr.iX);
        FLAT_DATA( iDisplayRectBrY ) = (TUint)(displayRectangle.iBr.iY);
        }
    else
        {
        //NOP
        MCECLI_DEBUG("CMceDisplaySink::DisplayRectL, done locally");
        }      
        
    MCECLI_DEBUG("CMceDisplaySink::DisplayRectL, Exit");
    
    return TRect( FLAT_DATA( iDisplayRectTlX ),
                  FLAT_DATA( iDisplayRectTlY ),
                  FLAT_DATA( iDisplayRectBrX ),
                  FLAT_DATA( iDisplayRectBrY ) );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetPriorityL( TUint aPriority )
    {
    MCECLI_DEBUG("CMceDisplaySink::SetPriorityL, Entry");
    MCECLI_DEBUG_DVALUE("priority", aPriority );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        FLAT_DATA( iPriority ) = SetITCValueL( EMceItcSetDisplayPriority,
                                               aPriority );
        }
    else
        {
    	FLAT_DATA( iPriority ) = aPriority;
        MCECLI_DEBUG("CMceDisplaySink::SetPriorityL, done locally");
        }

    MCECLI_DEBUG("CMceDisplaySink::SetPriorityL, Exit");
        
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMceDisplaySink::PriorityL()
    {
    MCECLI_DEBUG("CMceDisplaySink::PriorityL, Entry");
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        TInt value = GetITCValueL( EMceItcDisplayPriority );
        __ASSERT_ALWAYS( value >= 0, User::Leave( KErrNotFound ) );
        FLAT_DATA( iPriority ) = value;
        }
    else
        {
        MCECLI_DEBUG("CMceDisplaySink::PriorityL, done locally");
        }
        
    MCECLI_DEBUG("CMceDisplaySink::PriorityL, Exit");
    return FLAT_DATA( iPriority ); 
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceDisplaySink::SetRotationL( TRotation aRotation )
    {
    MCECLI_DEBUG("CMceDisplaySink::SetRotationL, Entry");
    MCECLI_DEBUG_DVALUE("rotation", aRotation );
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        TInt value = SetITCValueL( EMceItcSetDisplayRotation,
                                      static_cast<TInt>( aRotation ) );
        FLAT_DATA( iRotation ) = static_cast<TRotation>( value );
        }
    else
        {
        FLAT_DATA( iRotation ) = aRotation;
        MCECLI_DEBUG("CMceDisplaySink::SetRotationL, done locally");
        }

    MCECLI_DEBUG("CMceDisplaySink::SetRotationL, Exit");   
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CMceDisplaySink::TRotation CMceDisplaySink::RotationL()
    {
    MCECLI_DEBUG("CMceDisplaySink::RotationL, Entry");
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
        TInt value = GetITCValueL( EMceItcDisplayRotation );
        __ASSERT_ALWAYS( value >= 0, User::Leave( KErrNotFound ) );
        FLAT_DATA( iRotation ) = static_cast<TRotation>( value );
        }
    else
        {
        MCECLI_DEBUG("CMceDisplaySink::RotationL, done locally");
        }
        
    MCECLI_DEBUG("CMceDisplaySink::RotationL, Exit");
    return FLAT_DATA( iRotation ); 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceDisplaySink::InitializeL( CMceManager* aManager )
    {
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    
    if ( !FLAT_DATA( iInitialInfoRetrieved ) )
        { 
        MCECLI_DEBUG("CMceDisplaySink::InitializeL, querying available displays");
        TMceIds ids;
        ids.iAppUID = aManager->AppUid().iUid;
        TMceItcArgTInt displays;
        
    	_FLAT_DATA->SenderL( 
            aManager->ServerSession() ).ReadL( ids, EMceItcDisplaysAvailable, displays );
    	
        FLAT_DATA( iDisplaysAvailable ) = displays();
        
        FLAT_DATA( iInitialInfoRetrieved ) = ETrue;
        
        }
    }

// -----------------------------------------------------------------------------
// CMceDisplaySink::UsesEndpointProxy()
// -----------------------------------------------------------------------------
//
TBool CMceDisplaySink::UsesEndpointProxy() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CMceDisplaySink::UsesEndpointProxy()
// -----------------------------------------------------------------------------
//
MMceEndPointProxy* CMceDisplaySink::CreateEndpointProxyL() const
    {
    return CMceAvSink::NewL();
    }

    
    
// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceDisplaySink* CMceDisplaySink::NewL()
    {
    CMceDisplaySink* self = CMceDisplaySink::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceDisplaySink* CMceDisplaySink::NewLC()
    {
    CMceDisplaySink* self = new (ELeave) CMceDisplaySink();
    CleanupStack::PushL( self );
    self->ConstructL( NULL );
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMceDisplaySink::GetITCValueL( TUint8 aITCFunction )
    {
    CMceSession* session = NULL;
    
    if ( iStream )
        {
        session = iStream->Session();
        }
    
    if ( !session )
        {
        User::Leave( KErrNotReady );
        }
    
    TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID  = iStream->Id();
	ids.iSinkID = Id();
	
    TMceItcArgTInt value;

	session->ITCSender().ReadL(
    	ids, static_cast<TMceItcFunctions>( aITCFunction ), value );
    
    return value();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMceDisplaySink::SetITCValueL( TUint8 aITCFunction, 
                                    TInt aValue )
    {
    CMceSession* session = NULL;
    
    if ( iStream )
        {
        session = iStream->Session();
        }
    
    if ( !session )
        {
        User::Leave( KErrNotReady );
        }

	TMceIds ids;
	session->PrepareForITC( ids );
	ids.iMediaID   = iStream->Id();
	ids.iSinkID  = Id();
    TMceItcArgTInt value( aValue );
	
    session->ITCSender().WriteL( 
        ids, static_cast<TMceItcFunctions>( aITCFunction ), value );
    
    return value();
    
    }
        

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMceDisplaySink::CMceDisplaySink()
    :CMceMediaSink(),
     iWindow( NULL ),
     iGc( NULL )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMceDisplaySink::ConstructL( CMceManager* aManager )
    {
    CMceComDisplaySink* sink = CMceComDisplaySink::NewLC();
    CMceMediaSink::ConstructL( sink );
    CleanupStack::Pop( sink );
    
    if ( aManager )
        {        
        InitializeL( aManager );
        }
    }





