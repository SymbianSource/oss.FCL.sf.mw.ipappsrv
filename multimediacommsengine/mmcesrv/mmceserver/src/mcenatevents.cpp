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



#include <sdpdocument.h>
#include "mcenatevents.h"
#include "mcenatactions.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::TMceNatStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceNatStateTransitionEvent::TMceNatStateTransitionEvent(
                                                    CSdpDocument* aDoc,
                                                    MMceNatActions& aNatActions,
                                                    TMceNatEventCode aNatCode,
                                                    CMceSipSession& aSession, 
                                                    TMceSipEventCode aCode )
    : TMceStateTransitionEvent( aSession, aCode ),
      iDocument( aDoc ),
      iNatActions( aNatActions ),
      iNatCode( (TUint)aNatCode )
    {
    }

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::TMceNatStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceNatStateTransitionEvent::TMceNatStateTransitionEvent(
                                                    CSdpDocument* aDoc,
                                                    MMceNatActions& aNatActions,
                                                    TMceNatEventCode aNatCode,
                                                    CMceSipSession& aSession, 
                                                    TMceSipEventCode aCode, 
                                                    TInt aStatus )
    : TMceStateTransitionEvent( aSession, aCode, aStatus ),
      iDocument( aDoc ),
      iNatActions( aNatActions ),
      iNatCode( (TUint)aNatCode )      
    {    
    }
    
// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::TMceNatStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceNatStateTransitionEvent::TMceNatStateTransitionEvent(
                                                    CSdpDocument* aDoc,
                                                    MMceNatActions& aNatActions,
                                                    TMceNatEventCode aNatCode,
                                                    CMceSipSession& aSession, 
                                                    TMceItcFunctions aCode, 
                                                    TMceIds& aIds, 
                                                    CMceMsgBase& aClientMessage )
    : TMceStateTransitionEvent( aSession, aCode, aIds, aClientMessage ),
      iDocument( aDoc ),
      iNatActions( aNatActions ),
      iNatCode( (TUint)aNatCode )
    {    
    }

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::TMceNatStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceNatStateTransitionEvent::TMceNatStateTransitionEvent(
                                                    CSdpDocument* aDoc,
                                                    MMceNatActions& aNatActions,
                                                    TMceNatEventCode aNatCode,
                                                    CMceSipSession& aSession, 
                                                    TMceItcFunctions aCode, 
                                                    TMceIds& aIds )
    : TMceStateTransitionEvent( aSession, aCode, aIds ),
      iDocument( aDoc ),
      iNatActions( aNatActions ),
      iNatCode( (TUint)aNatCode )
    {    
    }

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::TMceNatStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceNatStateTransitionEvent::TMceNatStateTransitionEvent(
                                                    CSdpDocument* aDoc,
                                                    MMceNatActions& aNatActions,
                                                    TMceNatEventCode aNatCode,
                                                    CMceSipSession& aSession, 
                                                    TMceMediaEventCode aCode )
    : TMceStateTransitionEvent( aSession, aCode ),
      iDocument( aDoc ),
      iNatActions( aNatActions ),
      iNatCode( (TUint)aNatCode )
    {    
    }
    
// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::TMceNatStateTransitionEvent
// -----------------------------------------------------------------------------
//
TMceNatStateTransitionEvent::TMceNatStateTransitionEvent(
                                                    CSdpDocument* aDoc,
                                                    MMceNatActions& aNatActions,
                                                    TMceNatEventCode aNatCode,
                                                    CMceSipSession& aSession,
                                                    TMceMccComEvent& aEvent, 
                                                    TMceMediaEventCode aCode )
    : TMceStateTransitionEvent( aSession, aEvent, aCode ),
      iDocument( aDoc ),
      iNatActions( aNatActions ),
      iNatCode( (TUint)aNatCode )
    {    
    }

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::Document
// -----------------------------------------------------------------------------
//
CSdpDocument* TMceNatStateTransitionEvent::Document()
    {
    return iDocument;
    }

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::NatActions
// -----------------------------------------------------------------------------
//
MMceNatActions& TMceNatStateTransitionEvent::NatActions()
    {
    return iNatActions;
    }

// -----------------------------------------------------------------------------
// TMceNatStateTransitionEvent::NatCode
// -----------------------------------------------------------------------------
//
TUint& TMceNatStateTransitionEvent::NatCode()
    {
    return iNatCode;
    }

