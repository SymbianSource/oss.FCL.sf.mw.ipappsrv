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




#ifndef STUNUDBMEDIATESTSTUBS_H
#define STUNUDBMEDIATESTSTUBS_H

#include "natfwconnectivityapidefs.h"
#include <mmccctrlobserver.h>
#include <mmccnetworksettings.h>

#include "natfwtestconsolestubs.h"

class MTestConsoleStubsObserver;
class CNATFWCandidate;
class CConsoleBase;

class CMediaTestStubs :
    public CNATConnFWTestConsoleStubs,
    public MMccCtrlObserver
    {
    
public:
    static CMediaTestStubs* NewL( CConsoleBase& aConsole );
    
    virtual ~CMediaTestStubs();
    
public:
    
    void SetObserver( MTestConsoleStubsObserver* aObserver );


    // MCC controller observer
    void MccEventReceived( const TMccEvent& aEvent );
        
        
    void MccMediaStarted( TUint32 aSessionId,
                          TUint32 aLinkId,     
                          TUint32 aStreamId,
                          TUint32 aSinkSourceId );
                          
    void MccMediaStopped( TUint32 aSessionId,
                          TUint32 aLinkId,
                          TUint32 aStreamId,
                          TUint32 aSinkSourceId );

    void MccMediaPaused( TUint32 aSessionId,
                         TUint32 aLinkId,
                         TUint32 aStreamId,
                         TUint32 aSinkSourceId );

    void MccMediaResumed( TUint32 aSessionId,
                          TUint32 aLinkId,
                          TUint32 aStreamId,
                          TUint32 aSinkSourceId );

    void MccMediaPrepared( TUint32 aSessionId,
                           TUint32 aLinkId,
                           TUint32 aStreamId,
                           TUint32 aSinkSourceId );

    void MccMediaInactive( TUint32 aSessionId,
                           TUint32 aLinkId,
                           TUint32 aStreamId,
                           TUint32 aSinkSourceId );
    
    void MccMediaActive( TUint32 aSessionId,
                         TUint32 aLinkId,
                         TUint32 aStreamId,
                         TUint32 aSinkSourceId );
    
    void MccCtrlError( TInt aError,
                       TUint32 aSessionId,
                       TUint32 aLinkId,
                       TUint32 aStreamId,
                       TUint32 aSinkSourceId );
                       
    void UnknownMediaReceived(  TUint32 aSessionId,
                                TUint32 aLinkId, 
                                TUint32 aStreamId,
                                TUint32 aSinkSourceId,
                                TUint8 aPayloadType );                          

    TMccNetSettings& NetSettings();
    
protected:
    
    CMediaTestStubs( CConsoleBase& aConsole );
    
private:

    MTestConsoleStubsObserver* iObserver;
    TMccNetSettings iNetsettings;
    };
    
#endif // STUNUDBMEDIATESTSTUBS_H