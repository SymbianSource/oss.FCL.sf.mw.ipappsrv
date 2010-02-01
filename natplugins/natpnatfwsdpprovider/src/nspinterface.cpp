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
* Description:  Plugin interface implementation.
*
*/

#include "nspinterface.h"
#include "nspcontroller.h"
#include "nspsession.h"
#include "nspmediastreamcontainer.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPInterface::CNSPInterface
// ---------------------------------------------------------------------------
//
CNSPInterface::CNSPInterface()
    {
    }


// ---------------------------------------------------------------------------
// CNSPInterface::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPInterface::ConstructL()
    {
    iController = CNSPController::NewL();
    }


// ---------------------------------------------------------------------------
// CNSPInterface::NewL
// ---------------------------------------------------------------------------
//
CNSPInterface* CNSPInterface::NewL()
    {
    CNSPInterface* self = new( ELeave ) CNSPInterface;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPInterface::~CNSPInterface
// ---------------------------------------------------------------------------
//
CNSPInterface::~CNSPInterface()
    {
    delete iController;
    }


// ---------------------------------------------------------------------------
// CNSPInterface::NewSessionL
// ---------------------------------------------------------------------------
//
TUint CNSPInterface::NewSessionL( MNSPSessionObserver& aSessionObserver,
        TUint32 aIapId, const TDesC8& aDomain, TUint aProtocol )
    {
    return iController->NewSessionL( aSessionObserver, aIapId,
            aDomain, aProtocol );
    }


// ---------------------------------------------------------------------------
// CNSPInterface::CloseSessionL
// ---------------------------------------------------------------------------
//
void CNSPInterface::CloseSessionL( TUint aSessionId )
    {
    iController->ClosingSessionL( aSessionId );
    }


// ---------------------------------------------------------------------------
// CNSPInterface::SetSessionParam
// ---------------------------------------------------------------------------
//
TInt CNSPInterface::SetSessionParam( TUint aSessionId,
        TNSPSessionParamKey aParamKey, TUint aParamValue )
    {
    TInt status = KErrNone;
    CNSPSession* session = NULL;
    
    TRAPD( err, session = &iController->FindSessionObjectL( aSessionId ) );
    
    if ( KErrNone == err )
        {
        status = session->SetSessionParam( aParamKey, aParamValue );
        }
    else
        {
        status = err;
        }
    
    return status;
    }


// ---------------------------------------------------------------------------
// CNSPInterface::GetSessionParam
// ---------------------------------------------------------------------------
//
TInt CNSPInterface::GetSessionParam( TUint aSessionId,
        TNSPSessionParamKey aParamKey )
    {
    TInt value = KErrNone;
    CNSPSession* session = NULL;
    
    TRAPD( err, session = &iController->FindSessionObjectL( aSessionId ) );
    
    if ( KErrNone == err )
        {
        value = session->GetSessionParam( aParamKey );
        }
    else
        {
        value = err;
        }

    return value;
    }


// ---------------------------------------------------------------------------
// CNSPInterface::CreateOfferL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPInterface::CreateOfferL( TUint aSessionId,
        CSdpDocument*& aDocument )
    {
    CNSPSession& session = iController->FindSessionObjectL( aSessionId );
    return session.CreateOfferL( aDocument );
    }


// ---------------------------------------------------------------------------
// CNSPInterface::ResolveL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPInterface::ResolveL( TUint aSessionId,
        CSdpDocument*& aOffer, CSdpDocument*& aAnswer )
    {
    CNSPSession& session = iController->FindSessionObjectL( aSessionId );
    return session.ResolveL( aOffer, aAnswer );
    }


// ---------------------------------------------------------------------------
// CNSPInterface::DecodeAnswerL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPInterface::DecodeAnswerL( TUint aSessionId,
        CSdpDocument*& aAnswer )
    {
    CNSPSession& session = iController->FindSessionObjectL( aSessionId );
    return session.DecodeAnswerL( aAnswer );
    }


// ---------------------------------------------------------------------------
// CNSPInterface::UpdateL
// ---------------------------------------------------------------------------
//
void CNSPInterface::UpdateL( TUint aSessionId,
        CSdpDocument*& aOffer )
    {
    CNSPSession& session = iController->FindSessionObjectL( aSessionId );
    session.UpdateL( aOffer );
    }


// ---------------------------------------------------------------------------
// CNSPInterface::RestartL
// ---------------------------------------------------------------------------
//
TNatReturnStatus CNSPInterface::RestartL( TUint /*aSessionId*/ )
    {
    return KNatReady;
    }


// end of file
