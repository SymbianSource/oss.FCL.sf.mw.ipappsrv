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
#include <mmccinterface.h>
#include <mmccevents.h>

#include "multiplexerudptests.h"
#include "muxtestconsolestubs.h"
#include "mmccnetworksettings.h"
#include "ncmconnectionmultiplexerlogs.h"

_LIT16( KPanicArgument, "invalid argument" ); 
        
CMuxTestConsoleStubs* CMuxTestConsoleStubs::NewL( )
    {
    CMuxTestConsoleStubs* self = new (ELeave) CMuxTestConsoleStubs();
    return self;
    }
    

CMuxTestConsoleStubs::CMuxTestConsoleStubs()
    {

    }
    
CMuxTestConsoleStubs::~CMuxTestConsoleStubs()
    {
    }



// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccEventReceived( const TMccEvent& aEvent )
    {
    TMccNetSettingsPackage package;
    package.Copy( aEvent.iEventData );
    TBuf<50> localAddr;
    TBuf<50> localPublicAddr;
    
    iNetsettings = package();
    
    RDebug::Print( _L("CMuxTestConsoleStubs::MccEventReceived") );
    
    if ( KMccLinkCreated == aEvent.iEventType )
        {
        CActiveScheduler::Stop();
        }



    }
        
void CMuxTestConsoleStubs::MccMediaStarted( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaStarted") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
     CActiveScheduler::Stop();   
    }
        
void CMuxTestConsoleStubs::MccMediaStopped( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaStopped") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamStopped, KErrNone, KNullDesC8 );

   
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccMediaPaused( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaPaused") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamPaused, KErrNone, KNullDesC8 );

 
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccMediaResumed( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaResumed") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamResumed, KErrNone, KNullDesC8 );

   
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccMediaPrepared( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaPrepared") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamPrepared, KErrNone, KNullDesC8 );

    CActiveScheduler::Stop();
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccMediaInactive( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaInactive" ) );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryRtp, KMccInactivityEvent, KErrNone, KNullDesC8 );


    }
        
// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccMediaActive( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccMediaActive") );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryRtp, KMccActivityEvent, KErrNone, KNullDesC8 );


    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::MccCtrlError( TInt aError,
                                      TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MccCtrlError, ERR: %d"), aError );
    RDebug::Print( _L("SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamError, aError, KNullDesC8 );


    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::UnknownMediaReceived( TUint32 aSessionId,
                                              TUint32 /*aLinkId*/,  
                                              TUint32 aStreamId,
                                              TUint32 /*aSinkSourceId*/,
                                              TUint8 aPayloadType )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::UnknownMediaReceived") );
    RDebug::Print( _L("SESSID: %u, STREAMID: %u, PT: %u"),
     aSessionId, aStreamId, aPayloadType );
    } 








// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::StreamNotify
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::Notify( TUint aSessionId, TUint aStreamId,
    TNotifyType aType, TInt aError )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::Notify") );
    RDebug::Print( _L("aStreamId ID: %u, aStreamId ID: %u , ERROR: %d "), aSessionId, aStreamId, aError );
    
    // All test must notify with error code KErrNone
    ASSERT( KErrNone == aError || KErrDisconnected == aError );

    if ( MNcmConnectionMultiplexerObserver::ESessionCreated  == aType )
        {
        RDebug::Print( _L("Notify   -    ESessionCreated") );
        CActiveScheduler::Stop();

        }
        
    if ( MNcmConnectionMultiplexerObserver::EReceivingActivated  == aType )
        {
        RDebug::Print( _L("Notify   -    EReceivingActivated") );
        CActiveScheduler::Stop();  
        }
        
    if ( MNcmConnectionMultiplexerObserver::EReceivingDeactivated  == aType )
        {
        RDebug::Print( _L("Notify   -    EReceivingDeactivated") );
        CActiveScheduler::Stop();
        } 
        
    if ( MNcmConnectionMultiplexerObserver::ESendingActivated  == aType )
        {
        RDebug::Print( _L("Notify   -    ESendingActivated") );
        CActiveScheduler::Stop();
        }
        
    if ( MNcmConnectionMultiplexerObserver::ESendingDeactivated  == aType )
        {
        RDebug::Print( _L("Notify   -    ESendingDeActivated") );
        CActiveScheduler::Stop();
        }
        
    if ( MNcmConnectionMultiplexerObserver::EStreamError  == aType )
        {
        RDebug::Print( _L("Notify   -    EStreamError") );
        }            
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::IcmpError
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::IcmpError( TUint /*aSessionId*/, TUint /*aStreamId*/,
    const TInetAddr& aAddress )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::IcmpError") );
    
    TInetAddr fromAddr( aAddress );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("CMuxTestConsoleStubs::IcmpError - ADDRESS: %S "), &buffer3 );
    }
    
// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::ClearEvent
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::ClearEvent( /*TMccEvent& aEvent*/ )
    {
    //Event = TMccEvent();
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::HandleEventReceival
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::HandleEventReceival( TUint /*aStreamId*/ )
    {
    
    /*
    if ( EventsAreEqual( iEventToWaitFor, aEvent ) )
        {
        RDebug::Print( _L("CMuxTestConsoleStubs::HandleEventReceival, STOP WAIT LOOP") );
        CActiveScheduler::Stop();
        }
    else
        {
        // Eat event and continue to listen a new one
        RDebug::Print( _L("CMuxTestConsoleStubs::HandleEventReceival, UNEXPECTED EVENT") );
        }*/
        
/*
        iActive = EFalse;
        CActiveScheduler::Stop();
        */
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::IncomingMessageL
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::IncomingMessageL(  TUint /*aStreamId*/, const TDesC8& /*aMessage*/,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr, const TInetAddr& aPeerRemoteAddress,
        TBool& /*aConsumed*/ )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::IncomingMessageL") );
    
    TInetAddr fromAddr1( aLocalAddr );               
    TBuf<160> buffer1;
    fromAddr1.Output( buffer1 );
    RDebug::Print( _L("CMuxTestConsoleStubs::IncomingMessageL - LOCAL ADDRESS: %S "), &buffer1 );
    
    TInetAddr fromAddr( aPeerRemoteAddress );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("CMuxTestConsoleStubs::IncomingMessageL - PEER ADDRESS: %S "), &buffer3 );
    
    TInetAddr fromAddr2( aFromAddr );               
    TBuf<160> buffer4;
    fromAddr2.Output( buffer4 );
    RDebug::Print( _L("CMuxTestConsoleStubs::IncomingMessageL - FROM ADDRESS: %S "), &buffer4 );
    

    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::IncomingMessageNotify
// 
// ---------------------------------------------------------------------------
//
HBufC8* CMuxTestConsoleStubs::IncomingMessageNotify( TUint /*aStreamId*/,
        const TDesC8& /*aMessage*/, const TInetAddr& /*aLocalAddr*/,
        const TInetAddr& /*aFromAddr*/, TInetAddr& /*aPeerRemoteAddr*/ )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::IncomingMessageNotify") );
    return NULL;  
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::OutgoingMessageNotify
// 
// ---------------------------------------------------------------------------
//
HBufC8* CMuxTestConsoleStubs::OutgoingMessageNotify( TUint /*aStreamId*/,
        TUint /*aConnectionId*/, const TInetAddr& aDestinationAddress,
        const TDesC8& /*aMessage*/ )
    {
    
    RDebug::Print( _L("CMuxTestConsoleStubs::OutgoingMessageNotify") );
    
    TInetAddr fromAddr1( aDestinationAddress );               
    TBuf<160> buffer1;
    fromAddr1.Output( buffer1 );
    RDebug::Print( _L("CMuxTestConsoleStubs::OutgoingMessageNotify - aDestinationAddress: %S "), &buffer1 );
    
    return NULL;
    }
      
      
        
// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::ConnectionNotifyL
// 
// ---------------------------------------------------------------------------
//
 void CMuxTestConsoleStubs::ConnectionNotify(  TUint /*aStreamId*/, TUint aConnectionId,
    MNcmConnectionObserver::TConnectionNotifyType aType, TInt aError )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::ConnectionNotifyL") );
    RDebug::Print( _L("Connection ID: %u , ERROR: %d "), aConnectionId, aError );

    // All test must notify with error code KErrNone
    ASSERT( KErrNone == aError || KErrDisconnected == aError || KErrCancel == aError );
    
    if ( EConnectionError == aType && KErrDisconnected == aError )
        {
        CActiveScheduler::Stop(); 
        }
    else if ( MNcmConnectionObserver::EConnectionRemoved  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    EConnectionRemoved") );
        }
    
    else if ( MNcmConnectionObserver::EConnectionError  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    EConnectionError") );
        }  
    
    else if ( MNcmConnectionObserver::EReceivingActivated  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    EReceivingActivated") );
        CActiveScheduler::Stop();
        } 
        
    else if ( MNcmConnectionObserver::ESendingActivated  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    ESendingActivated") );
        CActiveScheduler::Stop();
        }
   
    else if ( MNcmConnectionObserver::EReceivingDeactivated  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    EReceivingDeactivated") );
        CActiveScheduler::Stop();
        } 
        
    else if ( MNcmConnectionObserver::ESendingDeactivated  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    ESendingDeActivated") );
        CActiveScheduler::Stop();
        }
        
    else if ( MNcmConnectionObserver::ETcpConnSetupCompleted  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    ETcpConnConnected") );
        CActiveScheduler::Stop();
        }
    else if ( MNcmConnectionObserver::EFirstMediaPacketSent  == aType )
        {
        RDebug::Print( _L("ConnectionNotifyL   -    EFirstMediaPacketSent") );
        CActiveScheduler::Stop();
        }
    else
        {
        RDebug::Print( _L("ConnectionNotifyL   -    default") );

        __ASSERT_ALWAYS( EFalse, User::Panic( KPanicArgument, KErrArgument ) );    
        }

    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::OutgoingMessage
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::OutgoingMessageL( TUint /*aStreamId*/, TUint /*aConnectionId*/,
        const TInetAddr& aDestinationAddress, const TDesC8& /*aMessage*/, TBool& /*aConsumed*/ )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::OutgoingMessage") );
    
    TInetAddr fromAddr( aDestinationAddress );               
    TBuf<160> buffer3;
    fromAddr.Output( buffer3 );
    RDebug::Print( _L("CMuxTestConsoleStubs::OutgoingMessage - ADDRESS: %S "), &buffer3 );

    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::WaitForEvent
// 
// ---------------------------------------------------------------------------
//
void CMuxTestConsoleStubs::WaitForEvent( TUint /*aStreamId*/, TUint /*aSubStreamId*/,
    MNcmConnectionObserver::TConnectionNotifyType /*aNotifyType*/ )
    {
    } 

void CMuxTestConsoleStubs::MessageSent()
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MessageSent") );
    CActiveScheduler::Stop();
    }
        
void CMuxTestConsoleStubs::MessageSentFailure( TInt aError )
    {
    RDebug::Print( _L("CMuxTestConsoleStubs::MessageSentFailure - error: %d"), aError );  
    }  
    
TMccNetSettings& CMuxTestConsoleStubs::NetSettings()
    {
    return iNetsettings;
    }      