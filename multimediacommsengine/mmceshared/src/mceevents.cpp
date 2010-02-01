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




#include "mceevents.h"
#include "mcecommediastream.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TMceEvent::TMceEvent
// -----------------------------------------------------------------------------
//
TMceEvent::TMceEvent()
    : iIds( NULL ), 
      iMessage( NULL ),
      iContent( NULL ) 
    {
    }

// -----------------------------------------------------------------------------
// TMceEvent::TMceEvent
// -----------------------------------------------------------------------------
//
TMceEvent::TMceEvent( TMceIds& aIds, CBase* aMessage, HBufC8* aContent )
    : iIds( &aIds ), 
      iMessage( aMessage ),
      iContent( aContent )
    {
    }

// -----------------------------------------------------------------------------
// TMceEvent::Id
// -----------------------------------------------------------------------------
//
TMceIds& TMceEvent::Id()
    {
    return *iIds;
    }

// -----------------------------------------------------------------------------
// TMceEvent::Action
// -----------------------------------------------------------------------------
//
TUint TMceEvent::Action()
    {
    return iIds->iCallbackID;
    }
    
// -----------------------------------------------------------------------------
// TMceEvent::ActionData
// -----------------------------------------------------------------------------
//
TUint32 TMceEvent::ActionData()
    {
    return iIds->iState;
    }
    
// -----------------------------------------------------------------------------
// TMceEvent::MessageType
// -----------------------------------------------------------------------------
//
TUint TMceEvent::MessageType()
    {
    return iIds->iMsgType;
    }
    
// -----------------------------------------------------------------------------
// TMceEvent::Message
// -----------------------------------------------------------------------------
//
CBase* TMceEvent::Message()
    {
    return iMessage;
    }

// -----------------------------------------------------------------------------
// TMceEvent::Content
// -----------------------------------------------------------------------------
//
HBufC8* TMceEvent::Content()
    {
    HBufC8* ret = iContent;
    iContent = NULL;
    return ret;
    }


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// TMceComEvent::TMceComEvent
// -----------------------------------------------------------------------------
//
TMceComEvent::TMceComEvent( TMceIds& aIds, CBase* aMessage, 
                            TMceItcFunctions aAction, 
                            MMceEventHandler& aHandler )
    : TMceEvent( aIds, aMessage, NULL ),
      iAction( aAction ),
      iHandler( aHandler ) 
    {
    }


// -----------------------------------------------------------------------------
// TMceComEvent::Action
// -----------------------------------------------------------------------------
//
TUint TMceComEvent::Action()
    {
    return iAction;
    }

// -----------------------------------------------------------------------------
// TMceComEvent::Handler
// -----------------------------------------------------------------------------
//
MMceEventHandler& TMceComEvent::Handler()
    {
    return iHandler;
    }


// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession, 
                                                    TMceSipEventCode aCode )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceSip ),
    iIds( NULL ),
    iClientMessage( NULL ),
    iMccEvent( NULL ),
    iStatus( KErrNone ),
    iDialog ( NULL )
    {
    }




// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession, 
                                                    TMceSipEventCode aCode,
                                                    CSIPDialog& aDialog )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceSip ),
    iIds( NULL ),
    iClientMessage( NULL ),
    iMccEvent( NULL ),
    iStatus( KErrNone ),
    iDialog ( &aDialog )
    {
    }


// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession, 
                                                    TMceSipEventCode aCode,
                                                    TInt aStaus )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceSip ),
    iIds( NULL ),
    iClientMessage( NULL ),
    iMccEvent( NULL ),
    iStatus( aStaus ),
    iDialog ( NULL )
    {
    }



    
// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession, 
                                                    TMceItcFunctions aCode,
                                                    TMceIds& aIds,
                                                    CMceMsgBase& aClientMessage )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceClient ),
    iIds( &aIds ),
    iClientMessage( &aClientMessage ),
    iMccEvent( NULL ),
    iStatus( KErrNone ),
    iDialog ( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession, 
                                                    TMceItcFunctions aCode,
                                                    TMceIds& aIds )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceClient ),
    iIds( &aIds ),
    iClientMessage( NULL ),
    iMccEvent( NULL ),
    iStatus( KErrNone ),
    iDialog ( NULL )
    {
    }


// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession, 
                                                    TMceMediaEventCode aCode )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceMedia ),
    iIds( NULL ),
    iClientMessage( NULL ),
    iMccEvent( NULL ),
    iStatus( KErrNone ),
    iDialog ( NULL )
    {
    }

    
// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent( CMceSipSession& aSession,
                                                    TMceMccComEvent& aEvent, 
                                                    TMceMediaEventCode aCode )
  : iSession( &aSession ),
    iCode( (TUint)aCode ),
    iEventSource( EMceMedia ),
    iIds( NULL ),
    iClientMessage( NULL ),
    iMccEvent( &aEvent ),
    iStatus( aEvent.iError ),
    iDialog ( NULL )
    {
    }
    
// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::TMceStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceStateTransitionEvent::TMceStateTransitionEvent()
  : iSession( NULL ),
    iCode( 0 ),
    iEventSource( EMceMedia ),
    iIds( NULL ),
    iClientMessage( NULL ),
    iMccEvent( NULL ),
    iStatus( KErrNone ),
    iDialog ( NULL )
    {
    }

// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::Session
// -----------------------------------------------------------------------------
//
CMceSipSession& TMceStateTransitionEvent::Session()
    {
    return *iSession;
    }
    
// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::EventSource
// -----------------------------------------------------------------------------
//
TMceEventSource TMceStateTransitionEvent::EventSource()
    {
    return iEventSource;
    }

// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::Code
// -----------------------------------------------------------------------------
//
TUint& TMceStateTransitionEvent::Code()
    {
    return iCode;
    }

// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::ParamIDs
// -----------------------------------------------------------------------------
//
TMceIds& TMceStateTransitionEvent::ParamIDs()
    {
    return *iIds;
    }


// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::ParamClientMessage
// -----------------------------------------------------------------------------
//
CMceMsgBase& TMceStateTransitionEvent::ParamClientMessage()
    {
    return *iClientMessage;
    }
    
    
// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::ParamMccEvent
// -----------------------------------------------------------------------------
//
TMceMccComEvent* TMceStateTransitionEvent::ParamMccEvent()
    {
    return iMccEvent;
    }

    
// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::Status
// -----------------------------------------------------------------------------
//
TInt& TMceStateTransitionEvent::ParamStatus()
    {
    return iStatus;
    }

// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::Status
// -----------------------------------------------------------------------------
//    
void TMceStateTransitionEvent::SetParamIDs( TMceIds& aIds )
    {
    if ( iIds )
        {
        iIds->Copy( aIds );
        }
    }

// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::SetParamClientMessage
// -----------------------------------------------------------------------------
//
void TMceStateTransitionEvent::SetParamClientMessage( CMceMsgBase* aMsg )
    {
    iClientMessage = aMsg;
    }



// -----------------------------------------------------------------------------
// TMceStateTransitionEvent::Dialog
// -----------------------------------------------------------------------------
//
CSIPDialog* TMceStateTransitionEvent::Dialog( )
    {
    return iDialog;
    }


// End of File

