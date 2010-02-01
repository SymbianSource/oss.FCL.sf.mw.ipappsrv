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

#include <mmccctrlobserver.h>
#include <mmccnetworksettings.h>

#include "natfwconnectivityapidefs.h"
#include "mncmconnectionmultiplexerobserver.h"
#include "mncmincomingconnectionobserver.h"
#include "mncmoutgoingconnectionobserver.h"
#include "mncmconnectionobserver.h"
#include "mncmmessageobserver.h"

#include "mncmsenderobserver.h"

class CTestInterface;
class TNcmConnectionNotifyType;

class CMuxTestConsoleStubs : public CBase, public MNcmConnectionMultiplexerObserver,
    public MNcmIncomingConnectionObserver, public MNcmOutgoingConnectionObserver,
    public MNcmSenderObserver, public MMccCtrlObserver,
    public MNcmConnectionObserver, public MNcmMessageObserver
    {
    public:
        static CMuxTestConsoleStubs* NewL( );
        virtual ~CMuxTestConsoleStubs();
    
public:
    
    /**
     * Called by Connection Multiplexer when actions for stream completes
     *
     * @since S60 v3.2
     * @param aStreamId which is target of notify
     * @param aType of notify
     * @return void
     */
    void Notify(  TUint aSessionId, TUint aStreamId,
        TNotifyType aType, TInt aError );
        
    void IcmpError( TUint aSessionId, TUint aStreamId,
        const TInetAddr& aAddress );
     
    void WaitForEvent( TUint aStreamId, TUint aSubStreamId,
        MNcmConnectionObserver::TConnectionNotifyType aNotifyType );
        
     
    void IncomingMessageL(  TUint aStreamId, const TDesC8& aMessage,
        const TInetAddr& aLocalAddr, const TInetAddr& aFromAddr,
        const TInetAddr& aPeerRemoteAddress, TBool& aConsumed );
        
    void ConnectionNotify(  TUint aStream, TUint aConnectionId,
    MNcmConnectionObserver::TConnectionNotifyType aType, TInt aError );
        
    void OutgoingMessageL( TUint aStreamId, TUint aConnectionId,
        const TInetAddr& aDestinationAddress, const TDesC8& aMessage, TBool& aConsumed );


HBufC8* IncomingMessageNotify( TUint aStreamId,
        const TDesC8& aMessage, const TInetAddr& aLocalAddr,
        const TInetAddr& aFromAddr, TInetAddr& aPeerRemoteAddr );

HBufC8* OutgoingMessageNotify( TUint aStreamId,
        TUint aConnectionId, const TInetAddr& aDestinationAddress,
        const TDesC8& aMessage );

    
    // MNcmMultiplexerSenderObserver
    void MessageSent();
    
    void MessageSentFailure( TInt aError );
    
    
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
                           
        void UnknownMediaReceived( TUint32 aSessionId,
                                       TUint32 aLinkId, 
                                       TUint32 aStreamId,
                                       TUint32 aSinkSourceId,
                                       TUint8 aPayloadType );                          


        TMccNetSettings& NetSettings();
             
private:
        
    CMuxTestConsoleStubs();
    
    TBool EventsAreEqual( /*const TMccEvent& aEvent1, 
        const TMccEvent& aEvent2*/ ) const;
    
    void ClearEvent( /*TMccEvent& aEvent*/ );
    
    void HandleEventReceival( TUint aStreamId );
        

    
    private:
        
        TUint iWaitStreamId;
        TUint iWaitSubStreamId;
        MNcmConnectionObserver::TConnectionNotifyType iWaitNotifyType;  
        TMccNetSettings iNetsettings;
    };
    
#endif // MCC_STUBS_H