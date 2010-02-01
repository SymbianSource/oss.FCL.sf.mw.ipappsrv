/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Mcc EventHandler
*
*/




// INCLUDE FILES
#include "mmccevent.h"
#include "mmccinterface.h"
#include "mmccevents.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccEvent::CMccEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccEvent::CMccEvent( RMMFController& aController, MMccCtrlObserver& aCallback )
    : CActive( EPriorityStandard ),
      iController( aController ), 
      iEventCallBack( aCallback ),
      iMessageDest( KMccControllerUidInterface )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CMccEvent::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccEvent::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMccEvent::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccEvent* CMccEvent::NewL( RMMFController& aController, 
                            MMccCtrlObserver& aObserver )
    {
    CMccEvent* self = new (ELeave) CMccEvent( aController, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccEvent::~CMccEvent
// Destructor
// -----------------------------------------------------------------------------
//
CMccEvent::~CMccEvent()
    {
    if( IsActive() )
        {
        Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CMccEvent::RequestEventNotification
// Requests for event from Controller plugin and starts event listening loop.
// -----------------------------------------------------------------------------
//
void CMccEvent::RequestEventNotification()
    {
    if ( !IsActive() )
        {
        iController.CustomCommandAsync( iMessageDest,
                                        ERequestEventNotification,
                                        KNullDesC8,
                                        KNullDesC8,
                                        iPackage,
                                        iStatus);
        
        SetActive();
        }
    else
        {
        // Something is very wrong here
        }
    }

// -----------------------------------------------------------------------------
// CMccEvent::DoCancel
// From CActive Implements cancellation of an outstanding request.
// -----------------------------------------------------------------------------
//
void CMccEvent::DoCancel()
    {
    iController.CustomCommandSync( iMessageDest, EMccCancel, 
                                    KNullDesC8, KNullDesC8 );
    }
 
// -----------------------------------------------------------------------------
// CMccEvent::RunL
// From CActive Handles an active object’s request completion event.
// -----------------------------------------------------------------------------
//
void CMccEvent::RunL()
    {
    if ( KErrNone == iStatus.Int() )
        {
        TMccEvent event = iPackage();
        
        switch( event.iEventType )
            {
            //HandleMccMediaEvent
            case KMccRtcpReceived:
            case KMccDtmfReceived:
            case KMccDtmfControl:
            case KMccStreamPlaying:
            case KMccStreamBuffering:
            case KMccStreamIdle:
            case KMccLinkCreated:
            case KMccMediaQualityStatus:
            case KMccResourceNotAvailable:
            case KMccTranscodeCancelled:
            case KMccTranscodeInProgress:
            case KMccTranscodeCompleted:
            case KMccMasterKeyStaled:
                HandleMccMediaEvent( event );
                break;
            
            //HandleMccErrorEvent
            case KMccStreamError:
            case KMccStreamMMFEvent:
                HandleMccErrorEvent( event );
                break;
            
            //HandleMccStateChangeEvent
            case KMccStreamPrepared:
            case KMccStreamStarted:
            case KMccStreamPaused:
            case KMccStreamResumed:
            case KMccStreamStopped:
            case KMccStreamClosed:
                HandleMccStateChangeEvent( event );
                break;
            
            //HandleMccInactivityEvent
            case KMccInactivityEvent:
                HandleMccInactivityEvent( event );
                break;

            //HandleMccActivityEvent
            case KMccActivityEvent:
                HandleMccActivityEvent( event );
                break;
            
            case KMccUnknownMediaReceived:
                HandleMccUnknownMediaEvent( event );
                break;    

            default:
                break;
            }
            
        RequestEventNotification();
        }
    else
        {
        User::Leave( iStatus.Int() );
        }        
    }

// -----------------------------------------------------------------------------
// CMccEvent::RunError
// From CActive Handles 
// -----------------------------------------------------------------------------
//
TInt CMccEvent::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CMccEvent::HandleMccMediaEvent
// 
// -----------------------------------------------------------------------------
//
void CMccEvent::HandleMccMediaEvent( const TMccEvent& aEvent )
    {
    iEventCallBack.MccEventReceived( aEvent );
    }

// -----------------------------------------------------------------------------
// CMccEvent::HandleMccStateChangeEvent
// 
// -----------------------------------------------------------------------------
//
void CMccEvent::HandleMccStateChangeEvent( const TMccEvent& aEvent )
    {
    if ( aEvent.iErrorCode )
        {
        HandleMccErrorEvent( aEvent );
        return;
        }
    switch ( aEvent.iEventType )
        {
        case KMccStreamPrepared:
            iEventCallBack.MccMediaPrepared( aEvent.iSessionId, aEvent.iLinkId,
                                           aEvent.iStreamId, aEvent.iEndpointId );
            break;
        case KMccStreamStarted:
            iEventCallBack.MccMediaStarted( aEvent.iSessionId, aEvent.iLinkId,
                                          aEvent.iStreamId, aEvent.iEndpointId );
            break;        
        case KMccStreamPaused:
            iEventCallBack.MccMediaPaused( aEvent.iSessionId, aEvent.iLinkId,
                                         aEvent.iStreamId, aEvent.iEndpointId );
            break;        
        case KMccStreamResumed:
            iEventCallBack.MccMediaResumed( aEvent.iSessionId, aEvent.iLinkId,
                                          aEvent.iStreamId, aEvent.iEndpointId );
            break;        
        case KMccStreamStopped:
            iEventCallBack.MccMediaStopped( aEvent.iSessionId, aEvent.iLinkId,
                                          aEvent.iStreamId, aEvent.iEndpointId );
            break;        
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CMccEvent::HandleMccInactivityEvent
// 
// -----------------------------------------------------------------------------
//
void CMccEvent::HandleMccInactivityEvent( const TMccEvent& aEvent )
    {
    iEventCallBack.MccMediaInactive( aEvent.iSessionId,
                                   aEvent.iLinkId,
                                   aEvent.iStreamId,
                                   aEvent.iEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccEvent::HandleMccActivityEvent
// 
// -----------------------------------------------------------------------------
//
void CMccEvent::HandleMccActivityEvent( const TMccEvent& aEvent )
    {
    iEventCallBack.MccMediaActive( aEvent.iSessionId,
                                 aEvent.iLinkId,
                                 aEvent.iStreamId,
                                 aEvent.iEndpointId );
    }

// -----------------------------------------------------------------------------
// CMccEvent::HandleMccErrorEvent
// 
// -----------------------------------------------------------------------------
//
void CMccEvent::HandleMccErrorEvent( const TMccEvent& aEvent )
    {
    iEventCallBack.MccCtrlError( aEvent.iErrorCode, 
                                 aEvent.iSessionId, 
    	                         aEvent.iLinkId, 
    	                         aEvent.iStreamId, 
    	                         aEvent.iEndpointId );
    }
    
// -----------------------------------------------------------------------------
// CMccEvent::HandleMccUnknownMediaEvent
// 
// -----------------------------------------------------------------------------
//
void CMccEvent::HandleMccUnknownMediaEvent( const TMccEvent& aEvent )
    {
    iEventCallBack.UnknownMediaReceived( aEvent.iSessionId, aEvent.iLinkId, 
        aEvent.iStreamId, aEvent.iEndpointId, TUint8( aEvent.iErrorCode ) );
    }
