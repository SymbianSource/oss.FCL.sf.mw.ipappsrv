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
// Purpose     : Working as a stub for NAT Connectivity FW tests
// Parameters  : 
// Return Value: 
//*******************************************************************************
#include <e32base.h>
#include <e32debug.h>
#include <mmccinterface.h>
#include <mmccevents.h>

#include "mediateststubs.h"
#include "natfwcandidate.h"
#include "testconsolestubsobserver.h"


// ---------------------------------------------------------------------------
// CMediaTestStubs::CMediaTestStubs
// ---------------------------------------------------------------------------
//
CMediaTestStubs* CMediaTestStubs::NewL( CConsoleBase& aConsole )
    {
    CMediaTestStubs* self = new (ELeave) CMediaTestStubs( aConsole );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CMediaTestStubs::CMediaTestStubs
// ---------------------------------------------------------------------------
//    
CMediaTestStubs::CMediaTestStubs( CConsoleBase& aConsole ):
    CNATConnFWTestConsoleStubs( aConsole )
    {
    }
    
    
// ---------------------------------------------------------------------------
// CMediaTestStubs::~CMediaTestStubs
// ---------------------------------------------------------------------------
//   
CMediaTestStubs::~CMediaTestStubs()
    {
    }

// ---------------------------------------------------------------------------
// CMediaTestStubs::SetObserver
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::SetObserver( MTestConsoleStubsObserver* aObserver )
    {
    iObserver = aObserver;
    }

void CMediaTestStubs::MccEventReceived( const TMccEvent& aEvent )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccEventReceived Event:%d"),
        aEvent.iEventType );
        
    TMccNetSettingsPackage package;
    package.Copy( aEvent.iEventData );
    TBuf<50> localAddr;
    TBuf<50> localPublicAddr;
    
    iNetsettings = package();

    if ( KMccLinkCreated == aEvent.iEventType )
        {
        CActiveScheduler::Stop();
        iActive = EFalse;
        }
    }
        
void CMediaTestStubs::MccMediaStarted( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaStarted") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    
    CActiveScheduler::Stop();
    iActive = EFalse;  
    }
        
void CMediaTestStubs::MccMediaStopped( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaStopped") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamStopped, KErrNone, KNullDesC8 );
    */
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::MccMediaPaused( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaPaused") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamPaused, KErrNone, KNullDesC8 );
    */
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::MccMediaResumed( TUint32 aSessionId,
                                       TUint32 aLinkId,
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaResumed") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamResumed, KErrNone, KNullDesC8 );
    */
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::MccMediaPrepared( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaPrepared") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamPrepared, KErrNone, KNullDesC8 );
    */    
    CActiveScheduler::Stop();
    iActive = EFalse;
    }

// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::MccMediaInactive( TUint32 aSessionId,
                                        TUint32 aLinkId,
                                        TUint32 aStreamId,
                                        TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaInactive" ) );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryRtp, KMccInactivityEvent, KErrNone, KNullDesC8 );
    */
    }
        
        
// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::MccMediaActive( TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccMediaActive") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryRtp, KMccActivityEvent, KErrNone, KNullDesC8 );
    */
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::MccCtrlError( TInt aError,
                                      TUint32 aSessionId,
                                      TUint32 aLinkId,
                                      TUint32 aStreamId,
                                      TUint32 aSinkSourceId )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::MccCtrlError, ERR: %d"), aError );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, LINKID: %u, STREAMID: %u, SINKSOURCEID: %u"),
         aSessionId, aLinkId, aStreamId, aSinkSourceId );
    /*
    TMccEvent event = TMccEvent( aSessionId, aLinkId, aStreamId, aSinkSourceId,
        KMccEventCategoryStream, KMccStreamError, aError, KNullDesC8 );
    */
    }


// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//
void CMediaTestStubs::UnknownMediaReceived( TUint32 aSessionId,
                                              TUint32 /*aLinkId*/,  
                                              TUint32 aStreamId,
                                              TUint32 /*aSinkSourceId*/,
                                              TUint8 aPayloadType )
    {
    RDebug::Print( _L("TEST PRINT: CMediaTestStubs::UnknownMediaReceived") );
    RDebug::Print( _L("TEST PRINT: SESSID: %u, STREAMID: %u, PT: %u"),
        aSessionId, aStreamId, aPayloadType );
    } 
 
 
// ---------------------------------------------------------------------------
// CMuxTestConsoleStubs::
// 
// ---------------------------------------------------------------------------
//   
TMccNetSettings& CMediaTestStubs::NetSettings()
    {
    return iNetsettings;
    }      
