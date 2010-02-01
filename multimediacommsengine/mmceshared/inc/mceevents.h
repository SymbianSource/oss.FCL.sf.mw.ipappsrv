/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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






#ifndef MCEEVENTS_H
#define MCEEVENTS_H

#include <sipdialog.h>
#include "mceclientserver.h"


class CMceComEndpoint;
class CMceComMediaSink;
class CMceComMediaSource;
class CMceComRtpSink;
class CMceComRtpSource;
class CMceComSpeakerSink;
class CMceComDisplaySink;
class CMceComMicSource;
class CMceComCameraSource;
class CMceComFileSource;
class CMceComFileSink;
class CMceComCodec;
class CMceComMediaStream;

class CMceSipSession;
class CMceMsgBase;

const TInt KMceEventConsumed    = 0;
const TInt KMceEventNotOwner    = -1;
const TInt KMceEventNotConsumed = -2;
const TInt KMceEventUpdate      = -3;

const TUint KMceEventCodeNotAssigned = KMceNotAssigned;

enum TMceMediaEventCode
    {
    EMceMediaSessionStopped = 100,
    EMceMediaError,
    EMceMediaReserved,
    EMceMediaUpdated,
    EMceMediaEventReceived
    };
    
enum TMceSipEventCode 
    {
    EMceRegister = 200,
    EMceInvite,
    EMceCancel,
    EMceAck,
    EMceBye,
    EMcePrack,
    EMceUpdate,
    EMceSubscribe,
    EMceRefer,
    EMceMessage,
    EMceInfo,
    EMceResponse,
    EMceProvisionalResponse,
    EMceRedirectionResponse,
    EMceErrorResponse,
    EMceStandAloneRequest,
    EMceStandAloneResponse,
    EMceSessionExpired,
    EMceSessionRefresh,
    EMceSessionPending,
    EMceNoPrackReceived,
    EMceReliableFailed,
    EMceRequest,
	EMceServerInitialized
    };
    
enum TMceEventSource
    {
    EMceClient,
    EMceSip,
    EMceMedia
    };


/**
 * event class 
 *
 * @lib 
 */
class TMceEvent
    {
public:
        
    /**
     * Contructor
     */
    TMceEvent();

    /**
     * Contructor
     * @param aIds ITC ids
     * @param aMessage message
     * @param aContent content
     */
    TMceEvent( TMceIds& aIds, CBase* aMessage, HBufC8* aContent );

    /**
     * returns id
     * @return id
     */
    TMceIds& Id();

    /**
     * returns action
     * @return action
     */
    TUint Action();
        
    /**
     * returns action data
     * @return action data
     */
    TUint32 ActionData();
        
    /**
     * returns message type
     * @return message type
     */
    TUint MessageType();
        
    /**
     * returns message
     * @return message
     */
    CBase* Message();

    /**
     * returns content
     * @return content
     */
    HBufC8* Content();
        
private:
            
    //ids            
    TMceIds* iIds;  
    //message
    CBase* iMessage;
    //content
    HBufC8* iContent;
    
    };


class MMceEventHandler;
    
/**
 * com event class 
 *
 * @lib 
 */
class TMceComEvent : public TMceEvent
    {
public:
        
    /**
     * Contructor
     * @param aIds ITC ids
     * @param aMessage message
     * @param aAction action
     * @param aHandler event handler
     */
    TMceComEvent( TMceIds& aIds, CBase* aMessage, 
                  TMceItcFunctions aAction, MMceEventHandler& aHandler );

    /**
     * returns action
     * @return action
     */
    TUint Action();
    
    /**
     * returns event handler
     * @return event handler
     */
    MMceEventHandler& Handler();
        
        
protected:

    //action            
    TMceItcFunctions iAction;
    //event handler
    MMceEventHandler& iHandler;
    
    };


/**
 * mcc event class 
 *
 * @lib 
 */
class TMceMccComEvent
    {
    
        
public:
        
        
    /**
     * Contructor
     * @param aEvent event type
     * @param aLinkId Link ID
     * @param aStreamId Stream ID
     * @param aEndpointId Sink or source ID
     */
    inline TMceMccComEvent( TUint32 aEvent,
                             TUint32 aLinkId,
                             TUint32 aStreamId,
                             TUint32 aEndpointId = 0 )
                : iEvent( aEvent ),
                  iLinkId( aLinkId ),
                  iStreamId( aStreamId ),
                  iEndpointId( aEndpointId ),
                  iError( KErrNone ),
                  iEventSubType( KMceNotAssigned ),
                  iCodec( NULL ),
                  iSource( NULL ),
                  iSink( NULL ),
                  iStream( NULL ),
                  iItcEvent( EMceItcNotAssigned ),
                  iItcData( KMceNotAssigned ),
                  iItcDataDesC( NULL )
                    {
                    }

    /**
     * Contructor
     * @param aLinkId Link ID
     * @param aStreamId Stream ID
     * @param aEndpointId Sink or source ID
     */
    inline TMceMccComEvent(  TUint32 aLinkId,
                             TUint32 aStreamId,
                             TUint32 aEndpointId = 0 )
                : iEvent( KMceNotAssigned ),
                  iLinkId( aLinkId ),
                  iStreamId( aStreamId ),
                  iEndpointId( aEndpointId ),
                  iError( KErrNone ),
                  iEventSubType( KMceNotAssigned ),
                  iCodec( NULL ),
                  iSource( NULL ),
                  iSink( NULL ),
                  iStream( NULL ),
                  iItcEvent( EMceItcNotAssigned ),
                  iItcData( KMceNotAssigned ),
                  iItcDataDesC( NULL )
                    {
                    }

    /**
     * Contructor
     * @param aEvent event type
     */
    inline TMceMccComEvent( TUint32 aEvent )
                : iEvent( aEvent ),
                  iLinkId( KMceNotAssigned ),
                  iStreamId( KMceNotAssigned ),
                  iEndpointId( KMceNotAssigned ),
                  iError( KErrNone ),
                  iEventSubType( KMceNotAssigned ),
                  iCodec( NULL ),
                  iSource( NULL ),
                  iSink( NULL ),
                  iStream( NULL ),
                  iItcEvent( EMceItcNotAssigned ),
                  iItcData( KMceNotAssigned ),
                  iItcDataDesC( NULL )
                    {
                    }
                  
        
public://mcc/mce data


    TUint32 iEvent;
    TUint32 iLinkId;
    TUint32 iStreamId;
    TUint32 iEndpointId;
    TInt iError;
    TUint32 iEventSubType;
    CMceComCodec* iCodec;
    CMceComEndpoint* iSource;
    CMceComEndpoint* iSink;
    
    inline void SetDataDesC( const TDesC8& aData )
        {
        iItcDataDesC = &aData;
        }
        
public://mce/itc data
    
    CMceComMediaStream* iStream;
    TMceItcCallBack iItcEvent;
    TUint32 iItcData;
    const TDesC8* iItcDataDesC;
         
    };
	
	
	
/**
 * event handler interface
 *
 * @lib 
 */
class MMceEventHandler
    {
public:

    /**
     * Handles event for sink
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComMediaSink& aTarget, TMceComEvent& aEvent ) = 0;

    /**
     * Handles event for source
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComMediaSource& aTarget, TMceComEvent& aEvent ) = 0;

    /**
     * Handles event for rtp sink
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComRtpSink& aTarget, TMceComEvent& aEvent ) = 0;

    /**
     * Handles event for rtp source
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComRtpSource& aTarget, TMceComEvent& aEvent ) = 0;
    
    /**
     * Handles event for speaker sink
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComSpeakerSink& aTarget, TMceComEvent& aEvent ) = 0;

    /**
     * Handles event for display sink
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComDisplaySink& aTarget, TMceComEvent& aEvent ) = 0;
    
    /**
     * Handles event for mic source
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComMicSource& aTarget, TMceComEvent& aEvent ) = 0;

    /**
     * Handles event for camera source
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComCameraSource& aTarget, TMceComEvent& aEvent ) = 0;
    
    /**
     * Handles event for file source
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComFileSource& aTarget, TMceComEvent& aEvent ) = 0;

    /**
     * Handles event for file sink
     * @param aTarget target
     * @param aEvent event
     */
    virtual TInt HandleL( CMceComFileSink& aTarget, TMceComEvent& aEvent ) = 0;
    
    };
    
    
    
/**
 * empty event handler
 *
 * @lib 
 */
class TMceComEmptyEventHandler : public  MMceEventHandler
    {
    
public:
    
    inline TInt HandleL( CMceComMediaSink& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComMediaSource& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComRtpSink& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComRtpSource& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComSpeakerSink& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComDisplaySink& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComMicSource& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComCameraSource& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComFileSource& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    inline TInt HandleL( CMceComFileSink& /*aTarget*/, TMceComEvent& /*aEvent*/ ) 
        {
        User::Leave( KErrNotSupported );
        return KErrNotSupported;
        }
    };
    


/**
 * transition event
 *
 * @lib 
 */
class TMceStateTransitionEvent 
    {
public:


    TMceStateTransitionEvent( CMceSipSession& aSession, TMceSipEventCode aCode );
    
    TMceStateTransitionEvent( CMceSipSession& aSession, TMceSipEventCode aCode, 
    						   CSIPDialog& aDialog );

    TMceStateTransitionEvent( CMceSipSession& aSession, TMceSipEventCode aCode, 
                              TInt aStatus );
    
    TMceStateTransitionEvent( CMceSipSession& aSession, TMceItcFunctions aCode, 
                              TMceIds& aIds, CMceMsgBase& aClientMessage );

    TMceStateTransitionEvent( CMceSipSession& aSession, TMceItcFunctions aCode, 
                              TMceIds& aIds );

    //updated & reserved & session closed
    TMceStateTransitionEvent( CMceSipSession& aSession, TMceMediaEventCode aCode );
    
    //media error
    TMceStateTransitionEvent( CMceSipSession& aSession,
                              TMceMccComEvent& aEvent, 
                              TMceMediaEventCode aCode );
    
    //empty event
    TMceStateTransitionEvent();                          

    CMceSipSession& Session();
    TMceEventSource EventSource();
    TUint& Code();
    
    TMceIds& ParamIDs();
    CMceMsgBase& ParamClientMessage();
    TMceMccComEvent* ParamMccEvent();
    TInt& ParamStatus();
    
    // Ownership of Dialog is not Transfered
    CSIPDialog* Dialog();
    
    void SetParamIDs( TMceIds& aIds );
    void SetParamClientMessage( CMceMsgBase* aMsg );
    
private:

    CMceSipSession* iSession;
    TUint iCode;
    TMceEventSource iEventSource;
    TMceIds* iIds;
    CMceMsgBase* iClientMessage;
    TMceMccComEvent* iMccEvent;
    TInt iStatus;
    CSIPDialog* iDialog;
    };
    

#endif // MCECLIENTSERVER_H

// End of File
