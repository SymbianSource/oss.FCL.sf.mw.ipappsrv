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




#ifndef MCC_STUBS_H
#define MCC_STUBS_H

#include "mmccinterfacedef.h"

class CMccInterfaceStub : public CBase, public MMccCtrlObserver
    {
    public:
        static CMccInterfaceStub* NewL();
        virtual ~CMccInterfaceStub();
    
    public:
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
                           
        void UnknownMediaReceived( TUint32 aSessionId,
                                       TUint32 aLinkId, 
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId,
                                       TUint8 aPayloadType );                          

        void WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
            TUint32 aStreamId, TMccEventType aEventType );
        
        void WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
            TUint32 aStreamId, TUint32 aSinkSourceId, TMccEventType aEventType );
        
        void WaitForEvent( TUint32 aSessionId, TUint32 aLinkId,
            TUint32 aStreamId, TMccEventType aEventType, TMccDtmfEventType aDtmfEventType );
        
    private:
        
        CMccInterfaceStub();
        
        TBool EventsAreEqual( const TMccEvent& aEvent1, 
            const TMccEvent& aEvent2 ) const;
        
        void ClearEvent( TMccEvent& aEvent );
        
        void HandleEventReceival( const TMccEvent& aEvent );
        
    private:
        
        TMccEvent iEventToWaitFor;
    
    };
    
#endif // MCC_STUBS_H