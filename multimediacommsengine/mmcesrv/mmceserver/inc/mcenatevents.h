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



#ifndef MCENATEVENTS_H
#define MCENATEVENTS_H

#include "mceevents.h"
#include "mceclientserver.h"

class MMceNatActions;
class CSdpDocument;

enum TMceNatEventCode 
    {
    EMceNatInitialized,
    EMceNatOffer,
    EMceNatAnswer,
    EMceNatUpdateSdp,
    EMceNatError,
    EMceNatHandleSIP,
    EMceNatUpdated,
    EMceNatSync,
    EMceNatICMPError
    };

/**
 * Nat transition event
 *
 * @lib 
 */
class TMceNatStateTransitionEvent : public TMceStateTransitionEvent
    {
public:

    TMceNatStateTransitionEvent( CSdpDocument* aDoc,
                                 MMceNatActions& aNatActions,
                                 TMceNatEventCode aNatCode,
                                 CMceSipSession& aSession, 
                                 TMceSipEventCode aCode );

    TMceNatStateTransitionEvent( CSdpDocument* aDoc,
                                 MMceNatActions& aNatActions,
                                 TMceNatEventCode aNatCode,
                                 CMceSipSession& aSession, 
                                 TMceSipEventCode aCode, 
                                 TInt aStatus );
    
    TMceNatStateTransitionEvent( CSdpDocument* aDoc,
                                 MMceNatActions& aNatActions,
                                 TMceNatEventCode aNatCode,
                                 CMceSipSession& aSession, 
                                 TMceItcFunctions aCode, 
                                 TMceIds& aIds, 
                                 CMceMsgBase& aClientMessage );

    TMceNatStateTransitionEvent( CSdpDocument* aDoc,
                                 MMceNatActions& aNatActions,
                                 TMceNatEventCode aNatCode,
                                 CMceSipSession& aSession, 
                                 TMceItcFunctions aCode, 
                                 TMceIds& aIds );

    //updated & reserved & session closed
    TMceNatStateTransitionEvent( CSdpDocument* aDoc,
                                 MMceNatActions& aNatActions,
                                 TMceNatEventCode aNatCode,
                                 CMceSipSession& aSession, 
                                 TMceMediaEventCode aCode );
    
    //media error
    TMceNatStateTransitionEvent( CSdpDocument* aDoc,
                                 MMceNatActions& aNatActions,
                                 TMceNatEventCode aNatCode,
                                 CMceSipSession& aSession,
                                 TMceMccComEvent& aEvent, 
                                 TMceMediaEventCode aCode );
    
    CSdpDocument* Document();
    MMceNatActions& NatActions();
    TUint& NatCode();
    
private:

    CSdpDocument* iDocument; // not owned
    MMceNatActions& iNatActions;
    TUint iNatCode;
    
    };

#endif // MCENATEVENTS_H

// End of File
