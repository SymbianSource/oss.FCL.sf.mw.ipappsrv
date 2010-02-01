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


//*******************************************************************************
// Method      : 
// Purpose     : Working as a stub for CMccInterface instantation tests
// Parameters  : 
// Return Value: 
//*******************************************************************************
#include <e32base.h>
#include <e32debug.h>
#include "mccteststubs.h"
#include "mmccnetworksettings.h"

CMccInterfaceStub* CMccInterfaceStub::NewL()
    {
    CMccInterfaceStub* self = new (ELeave) CMccInterfaceStub();
    return self;
    }
    

CMccInterfaceStub::CMccInterfaceStub()
    {
    
    }
    
CMccInterfaceStub::~CMccInterfaceStub()
    {
    }

// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccEventReceived( const TMccEvent& aEvent )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccEventReceived") );
    RDebug::Print( _L("SESSID: %d, LINKID: %d, STREAMID: %d, ENDPOINTID: %d"),
        aEvent.iSessionId, aEvent.iLinkId, aEvent.iStreamId, aEvent.iEndpointId );
    RDebug::Print( _L("CATEGORY: %d, EVENTTYPE: %d, ERRCODE: %d"),
        aEvent.iEventCategory, aEvent.iEventType, aEvent.iErrorCode );
    
    HandleEventReceival( aEvent );
    
    if ( KMccEventCategoryDtmf == aEvent.iEventCategory 
            && KMccDtmfControl == aEvent.iEventType )
        {
        const TMccDtmfEventData& dtmfEvent = 
            (*reinterpret_cast<const TMccDtmfEventDataPackage*>( 
                &aEvent.iEventData ))();
        
        switch ( dtmfEvent.iDtmfEventType )
            {
            case KMccDtmfManualStart:
                RDebug::Print( _L("EMccDtmfManualStart") );
                break;
            case KMccDtmfManualStop:
                RDebug::Print( _L("EMccDtmfManualStop") );
                break;
            case KMccDtmfManualAbort:
                RDebug::Print( _L("EMccDtmfManualAbort") );
                break;
            case KMccDtmfSequenceStart:
                RDebug::Print( _L("EMccDtmfSequenceStart") );
                break;
            case KMccDtmfSequenceStop:
                RDebug::Print( _L("EMccDtmfSequenceStop") );
                break;
            case KMccDtmfSequenceAbort:
                RDebug::Print( _L("EMccDtmfSequenceAbort") );
                break;
            case KMccDtmfStopInDtmfString:
                RDebug::Print( _L("EMccDtmfStopInDtmfString") );
                break;
            case KMccDtmfSendingComplete:
                RDebug::Print( _L("KMccDtmfSendingComplete") );
                break;
            default:
                break;
            }
        }
    
    if ( KMccEventCategoryRtp == aEvent.iEventCategory 
            && KMccMediaQualityStatus == aEvent.iEventType )
        {
        RDebug::Print( _L("CMccInterfaceStub::MccEventReceived JITTER STATUS RECV") );
        
        const TMccRtpEventData& rtpEvent = 
        (*reinterpret_cast<const TMccRtpEventDataPackage*>( 
            &aEvent.iEventData ))();
        
        RDebug::Print( _L("jitter estimate: %u, packets received: %u, prev trans time: %u, triggered jitter level: %u"), 
            rtpEvent.iJitterEstimate, rtpEvent.iPacketsReceived, rtpEvent.iPrevTransTime, rtpEvent.iTriggeredJitterLevel );
        }
        
    if ( KMccLinkCreated == aEvent.iEventType )
        {
        RDebug::Print( _L("KMccLinkCreated Event arrived succesfully!") );

        TMccNetSettingsPackage package;
        package.Copy( aEvent.iEventData );
        TBuf<50> localAddr;
        TBuf<50> localPublicAddr;
        TMccNetSettings netSettings = package();
        netSettings.iLocalAddress.Output( localAddr );
        netSettings.iLocalPublicAddress.Output( localPublicAddr );
        
        RDebug::Print( _L("Local ADDR: %S, PORT: %d, Public ADDR: %S, PORT: %d,"),
            &localAddr, netSettings.iLocalAddress.Port(), 
            &localPublicAddr, netSettings.iLocalPublicAddress.Port() );
        RDebug::Print( _L("Local RTCP PORT: %d, Public RTCP PORT: %d"), 
            netSettings.iLocalRtcpPort, netSettings.iLocalPublicRtcpPort );
        }
    }
        
void CMccInterfaceStub::MccMediaStarted( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaStarted") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamStarted, KErrNone, KNullDesC8 );
    
    HandleEventReceival( event );
    }
        
void CMccInterfaceStub::MccMediaStopped( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaStopped") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamStopped, KErrNone, KNullDesC8 );

    HandleEventReceival( event );
    }

// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccMediaPaused( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaPaused") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamPaused, KErrNone, KNullDesC8 );

    HandleEventReceival( event );
    }

// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccMediaResumed( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaResumed") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamResumed, KErrNone, KNullDesC8 );

    HandleEventReceival( event );
    }

// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccMediaPrepared( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaPrepared") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamPrepared, KErrNone, KNullDesC8 );

    HandleEventReceival( event );
    }

// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccMediaInactive( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaInactive" ) );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryRtp, KMccInactivityEvent, KErrNone, KNullDesC8 );

    HandleEventReceival( event );
    }
        
// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccMediaActive( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccMediaActive") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryRtp, KMccActivityEvent, KErrNone, KNullDesC8 );

    HandleEventReceival( event );
    }


// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::MccCtrlError( TInt aError,
                                      TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMccInterfaceStub::MccCtrlError, ERR: %d"), aError );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamError, aError, KNullDesC8 );

    HandleEventReceival( event );
    }

// ---------------------------------------------------------------------------
// CMccInterfaceStub::
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::UnknownMediaReceived( TUint32 aSessionId,
                                              TUint32 /*aLinkId*/,  
                                              TUint32 aStreamId,
                                              TUint32 /*aSinkSourceId*/,
                                              TUint8 aPayloadType )
    {
    RDebug::Print( _L("CMccInterfaceStub::UnknownMediaReceived") );
    RDebug::Print( _L("SESSID: %u, STREAMID: %u, PT: %u"),
     aSessionId, aStreamId, aPayloadType );
    } 

// ---------------------------------------------------------------------------
// CMccInterfaceStub::WaitForEvent
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TMccEventType aEventType )
    {
    ClearEvent( iEventToWaitFor );
    iEventToWaitFor.iSessionId = aSessionId;
    iEventToWaitFor.iLinkId = aLinkId;
    iEventToWaitFor.iStreamId = aStreamId;
    iEventToWaitFor.iEventType = aEventType;
    
    CActiveScheduler::Start();
    }
    

// ---------------------------------------------------------------------------
// CMccInterfaceStub::WaitForEvent
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId, TMccEventType aEventType )
    {
    ClearEvent( iEventToWaitFor );
    iEventToWaitFor.iSessionId = aSessionId;
    iEventToWaitFor.iLinkId = aLinkId;
    iEventToWaitFor.iStreamId = aStreamId;
    iEventToWaitFor.iEndpointId = aSinkSourceId;
    iEventToWaitFor.iEventType = aEventType;
    
    CActiveScheduler::Start();
    }
            
// ---------------------------------------------------------------------------
// CMccInterfaceStub::WaitForEvent
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TMccEventType aEventType, TMccDtmfEventType aDtmfEventType )
    {
    ClearEvent( iEventToWaitFor );
    iEventToWaitFor.iSessionId = aSessionId;
    iEventToWaitFor.iLinkId = aLinkId;
    iEventToWaitFor.iStreamId = aStreamId;
    iEventToWaitFor.iEventCategory = KMccEventCategoryDtmf;
    iEventToWaitFor.iEventType = aEventType;
    
    TMccDtmfEventData dtmfEvent;
    dtmfEvent.iDtmfEventType = aDtmfEventType;
    iEventToWaitFor.iEventData = TMccDtmfEventDataPackage( dtmfEvent );
    
    CActiveScheduler::Start();
    }
    

// ---------------------------------------------------------------------------
// CMccInterfaceStub::EventsAreEqual
// 
// ---------------------------------------------------------------------------
//
TBool CMccInterfaceStub::EventsAreEqual( 
        const TMccEvent& aEvent1, const TMccEvent& aEvent2 ) const
    {
    TBool match( EFalse );
    match = ( aEvent1.iSessionId == aEvent2.iSessionId );
    
    if ( match && aEvent1.iLinkId != 0 )
        {
        match = ( aEvent1.iLinkId == aEvent2.iLinkId );
        }
    
    if ( match && aEvent1.iStreamId != 0 )
        {
        match = ( aEvent1.iStreamId == aEvent2.iStreamId );
        }
        
    if ( match && aEvent1.iEndpointId != 0 )
        {
        match = ( aEvent1.iEndpointId == aEvent2.iEndpointId );
        }

    if ( match && aEvent1.iEventCategory != 0 )
        {
        match = ( aEvent1.iEventCategory == aEvent2.iEventCategory );
        }

    if ( match && aEvent1.iEventType != KMccEventNone )
        {
        match = ( aEvent1.iEventType == aEvent2.iEventType );
        }

    if ( match && aEvent1.iEventData != KNullDesC8 )
        {
        if ( KMccEventCategoryDtmf == aEvent1.iEventCategory )
            {
            const TMccDtmfEventData& dtmfEvent1 = 
            (*reinterpret_cast<const TMccDtmfEventDataPackage*>
            ( &aEvent1.iEventData ) )();
            
            const TMccDtmfEventData& dtmfEvent2 = 
            (*reinterpret_cast<const TMccDtmfEventDataPackage*>
            ( &aEvent1.iEventData ) )();
            
            match = ( dtmfEvent1.iDtmfEventType == dtmfEvent2.iDtmfEventType );
            }
        }
    
    return match;
    }
    
// ---------------------------------------------------------------------------
// CMccInterfaceStub::ClearEvent
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::ClearEvent( TMccEvent& aEvent )
    {
    aEvent = TMccEvent();
    }


// ---------------------------------------------------------------------------
// CMccInterfaceStub::HandleEventReceival
// 
// ---------------------------------------------------------------------------
//
void CMccInterfaceStub::HandleEventReceival( const TMccEvent& aEvent )
    {
    if ( EventsAreEqual( iEventToWaitFor, aEvent ) )
        {
        RDebug::Print( _L("CMccInterfaceStub::HandleEventReceival, STOP WAIT LOOP") );
        CActiveScheduler::Stop();
        }
    else
        {
        // Eat event and continue to listen a new one
        RDebug::Print( _L("CMccInterfaceStub::HandleEventReceival, UNEXPECTED EVENT") );
        }
    }
