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
* Description:    
*
*/




#include "mccsession.h"

#include "natfwconnectivityapi.h"
#include <mmcccodecinformation.h>
#include <mmccinterface.h>
#include <e32debug.h>
#include "natfwtestconsolelogs.h"
#include "mcclink.h"
#include "mccmediastream.h"

// ---------------------------------------------------------------------------
// CMccSession::ConstructL
// ---------------------------------------------------------------------------
// 
void CMccSession::ConstructL()
    {
    __NATFWTESTCONSOLE( "CMccSession::ConstructL" )

    iMccInterface = CMccInterface::NewL( *this );
    
    // Create Mcc session
    User::LeaveIfError( iMccInterface->CreateSession( iMccSessionId ) );
    }

// ---------------------------------------------------------------------------
// CMccSession::NewL
// ---------------------------------------------------------------------------
//     
CMccSession* CMccSession::NewL( TUint aIapId )
    {
    CMccSession* self = new( ELeave ) CMccSession( aIapId );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CMccSession::CMccSession
// ---------------------------------------------------------------------------
// 
CMccSession::CMccSession( TUint aIapId ) :
    iIapId( aIapId )
    {
    }

// ---------------------------------------------------------------------------
// CMccSession::~CMccSession
// ---------------------------------------------------------------------------
// 
CMccSession::~CMccSession()
    {
    iMccStreams.ResetAndDestroy();
    iMccLinks.ResetAndDestroy();
    
    TInt error = iMccInterface->CloseSession( iMccSessionId );
    if ( error )
        {
        __NATFWTESTCONSOLE_INT1(
            "CCMccSession::~CMccSession Session closing error ", error )
        }
    delete iMccInterface;
    }

// ---------------------------------------------------------------------------
// CMccSession::CreateLinkL
// ---------------------------------------------------------------------------
//
void CMccSession::CreateLinkL( TInt32& aLinkId, TInt aLinkType,
    TInetAddr& aDestAddr, TRequestStatus& aStatus )
    {
    iObserverStatus = &aStatus;
    
    CMccLink* link = CMccLink::NewL( aLinkType, iIapId, this );
    CleanupStack::PushL( link );
    iMccLinks.AppendL( link );
    
    link->CreateLinkL( aLinkId, aDestAddr, aStatus );
    CleanupStack::Pop( link );
    }

// ---------------------------------------------------------------------------
// CMccSession::CreateLinkL
// ---------------------------------------------------------------------------
//
void CMccSession::CreateLinkL( TInt32& aLinkId, TInt aLinkType,
    TInetAddr& aDestAddr, TUint aRtcpPort, TRequestStatus& aStatus )
    {
    iObserverStatus = &aStatus;
    
    CMccLink* link = CMccLink::NewL( aLinkType, iIapId, this );
    CleanupStack::PushL( link );
    iMccLinks.AppendL( link );
    
    link->CreateLinkL( aLinkId, aDestAddr, aRtcpPort, aStatus );
    CleanupStack::Pop( link );
    } 
 
// ---------------------------------------------------------------------------
// CMccSession::CreateMediaStreamL
// ---------------------------------------------------------------------------
//
void CMccSession::CreateMediaStreamL( TInt32& aStreamId, TInt32 aLinkId,
    TStreamDirection aStreamDirection, TRequestStatus& aStatus )
    {
    iObserverStatus = &aStatus;
    
    CMccMediaStream* stream = CMccMediaStream::NewL( LinkByIdL( aLinkId ),
        this, aStreamDirection );
    CleanupStack::PushL( stream );
    
    iMccStreams.AppendL( stream );
    
    stream->CreateStreamL( aStreamId, aStatus );
    CleanupStack::Pop( stream );
    }

// ---------------------------------------------------------------------------
// CMccSession::LinkByIdL
// ---------------------------------------------------------------------------
// 
CMccLink* CMccSession::LinkByIdL( TInt32 aLinkId )
    {
    TInt count = iMccLinks.Count();
    
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( aLinkId == iMccLinks[i]->LinkId() )
            {
            return iMccLinks[i];
            }
        }
    User::Leave( KErrNotFound );
    return NULL;
    }

// ---------------------------------------------------------------------------
// CMccSession::StreamByIdL
// ---------------------------------------------------------------------------
//     
CMccMediaStream* CMccSession::StreamByIdL( TInt32 aStreamId )
    {
    TInt count = iMccStreams.Count();
    
    for ( TInt i( 0 ); i < count; ++i )
        {
        if ( aStreamId == iMccStreams[i]->StreamId() )
            {
            return iMccStreams[i];
            }
        }
    User::Leave( KErrNotFound );
    return NULL; 
    }
    
// ---------------------------------------------------------------------------
// CMccSession::CloseLinkL
// ---------------------------------------------------------------------------
// 
void CMccSession::CloseLinkL( TInt32 aLinkId )
    {
    
    CMccLink* link = LinkByIdL( aLinkId );
    
    User::LeaveIfError( link->Close() );
    
    TInt index = iMccLinks.FindL( link );
    
    iMccLinks.Remove( index );
    delete link;
    link = NULL;
    }

// ---------------------------------------------------------------------------
// CMccSession::CloseMediaStreamL
// ---------------------------------------------------------------------------
//       
void CMccSession::CloseMediaStreamL( TInt32 aStreamId )
    {
    CMccMediaStream* stream = StreamByIdL( aStreamId );
    
    User::LeaveIfError( stream->Delete() );
    
    TInt index = iMccStreams.FindL( stream );
    
    iMccStreams.Remove( index );
    delete stream;
    stream = NULL;
    }

// ---------------------------------------------------------------------------
// CMccSession::StartStreamL
// ---------------------------------------------------------------------------
//   
void CMccSession::StartStreamL( TInt32 aStreamId,
    TRequestStatus& aStatus )
    {
    if ( ERtcpStreamPrepared  == StreamByIdL( aStreamId )->State() )
        {
        iObserverStatus = &aStatus;
        StreamByIdL( aStreamId )->StartStreamL( aStatus );
        }
    else
        {
        User::Leave( KErrNotReady );
        }
    }

// ---------------------------------------------------------------------------
// CMccSession::SetRemoteAddressL
// ---------------------------------------------------------------------------
// 
void CMccSession::SetRemoteAddressL( TInt32 aLinkId, TInetAddr& aDestAddr )
    {
    __NATFWTESTCONSOLE( "CMccSession::SetRemoteAddressL" )
    
    LinkByIdL( aLinkId )->SetRemoteAddressL( aDestAddr );
    }

// ---------------------------------------------------------------------------
// CMccSession::SetRemoteAddressL
// ---------------------------------------------------------------------------
// 
void CMccSession::SetRemoteAddressL( TInt32 aLinkId, TInetAddr& aDestAddr,  
    TUint aRtcpPort )
    {
    __NATFWTESTCONSOLE( "CMccSession::SetRemoteAddressL + rtcp" )
    
    LinkByIdL( aLinkId )->SetRemoteAddressL( aDestAddr, aRtcpPort );
    }

// ---------------------------------------------------------------------------
// CMccSession::MccInterface
// ---------------------------------------------------------------------------
// 
CMccInterface* CMccSession::MccInterface()
    {
    return iMccInterface;
    }

// ---------------------------------------------------------------------------
// CMccSession::MccSessionId
// ---------------------------------------------------------------------------
// 
TUint32 CMccSession::MccSessionId()
    {
    return iMccSessionId;
    }

// ---------------------------------------------------------------------------
// CMccSession::NetSettings
// ---------------------------------------------------------------------------
// 
TMccNetSettings& CMccSession::NetSettings()
    {
    return iNetSettings;
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccEventReceived
// ---------------------------------------------------------------------------
//
void CMccSession::MccEventReceived( const TMccEvent& aEvent )
    {
    __NATFWTESTCONSOLE( "CMccSession::MccEventReceived" )
    RDebug::Print( _L("TEST PRINT: Event: %u, Category: %u, Error: %u"),
            aEvent.iEventType, aEvent.iEventCategory, aEvent.iErrorCode );
    if ( KMccLinkCreated == aEvent.iEventType )
        {
        TMccNetSettingsPackage package;
        package.Copy( aEvent.iEventData );
        TBuf<50> localAddr;
        TBuf<50> localPublicAddr;
    
        iNetSettings = package();

        User::RequestComplete( iObserverStatus, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaStarted
// ---------------------------------------------------------------------------
// 
void CMccSession::MccMediaStarted( TUint32 aSessionId, TUint32 aLinkId,
    TUint32 aStreamId, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMccSession::MccMediaStarted") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
        aSessionId, aLinkId, aStreamId, aSinkSourceId );
        
    TMccStreamState state = StreamByIdL( aStreamId )->State();    
    
    if ( ERtcpStreamPrepared == state )
        {
        StreamByIdL( aStreamId )->SetState( ERtpStreamStarted );
        }
    else if ( ERtpStreamStarted == state )
        {
        StreamByIdL( aStreamId )->SetState( ERtcpStreamStarted );
        User::RequestComplete( iObserverStatus, KErrNone );
        }
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaStopped
// ---------------------------------------------------------------------------
//                          
void CMccSession::MccMediaStopped( TUint32 aSessionId, TUint32 aLinkId,
    TUint32 aStreamId, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMccSession::MccMediaStopped") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
        aSessionId, aLinkId, aStreamId, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaPaused
// ---------------------------------------------------------------------------
//
void CMccSession::MccMediaPaused( TUint32 aSessionId, TUint32 aLinkId,
    TUint32 aStreamId, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMccSession::MccMediaPaused") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
        aSessionId, aLinkId, aStreamId, aSinkSourceId );
    }
    
// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaResumed
// ---------------------------------------------------------------------------
//
void CMccSession::MccMediaResumed( TUint32 aSessionId, TUint32 aLinkId,
    TUint32 aStreamId, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMccSession::MccMediaResumed") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
        aSessionId, aLinkId, aStreamId, aSinkSourceId );
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaPrepared
// ---------------------------------------------------------------------------
//
void CMccSession::MccMediaPrepared( TUint32 aSessionId, TUint32 aLinkId,
    TUint32 aStreamId, TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMccSession::MccMediaPrepared") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
        aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    TMccStreamState state = StreamByIdL( aStreamId )->State();
        
    if ( EStreamCreated == state )
        {
        StreamByIdL( aStreamId )->SetState( ERtpStreamPrepared );
        }
    else if ( ERtpStreamPrepared == state )
        {
        StreamByIdL( aStreamId )->SetState( ERtcpStreamPrepared );
        User::RequestComplete( iObserverStatus, KErrNone );
        }
    }
    

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaInactive
// ---------------------------------------------------------------------------
//
void CMccSession::MccMediaInactive( TUint32 /*aSessionId*/, TUint32 /*aLinkId*/,
    TUint32 /*aStreamId*/, TUint32 /*aSinkSourceId*/ )
    {
     __NATFWTESTCONSOLE( "CMccSession::MccMediaInactive" )
    }
  
// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccMediaActive
// ---------------------------------------------------------------------------
//   
void CMccSession::MccMediaActive( TUint32 /*aSessionId*/, TUint32 /*aLinkId*/,
    TUint32 /*aStreamId*/, TUint32 /*aSinkSourceId*/ )
    {
    __NATFWTESTCONSOLE( "CMccSession::MccMediaActive" )
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::MccCtrlError
// ---------------------------------------------------------------------------
//    
void CMccSession::MccCtrlError( TInt aError, TUint32 /*aSessionId*/,
    TUint32 /*aLinkId*/, TUint32 /*aStreamId*/, TUint32 /*aSinkSourceId*/ )
    {
     __NATFWTESTCONSOLE_INT1( "CMccSession::MccCtrlError ", aError )
    }

// ---------------------------------------------------------------------------
// From MMccCtrlObserver
// CMccSession::UnknownMediaReceived
// ---------------------------------------------------------------------------
//                       
void CMccSession::UnknownMediaReceived(  TUint32 /*aSessionId*/, TUint32 /*aLinkId*/, 
    TUint32 /*aStreamId*/, TUint32 /*aSinkSourceId*/, TUint8 /*aPayloadType*/ )
    {
     __NATFWTESTCONSOLE( "CMccSession::UnknownMediaReceived" )
    }
