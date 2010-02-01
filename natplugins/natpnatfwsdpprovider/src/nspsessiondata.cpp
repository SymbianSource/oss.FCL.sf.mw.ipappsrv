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
* Description:  Session Sdp related storage, class implementation.
*
*/

#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "natfwcredentials.h"
#include "nspsessiondata.h"
#include "nspmediastream.h"
#include "nspmediastreamcomponent.h"
#include "nspcontentparser.h"
#include "nspcontrollerif.h"
#include "nspdefs.h"

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPSessionData::CNSPSessionData
// ---------------------------------------------------------------------------
//
CNSPSessionData::CNSPSessionData()
    {
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPSessionData::ConstructL()
    {
    iCredentials = CNATFWCredentials::NewL();
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::NewL
// ---------------------------------------------------------------------------
//
CNSPSessionData* CNSPSessionData::NewL()
    {
    CNSPSessionData* self = CNSPSessionData::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::NewLC
// ---------------------------------------------------------------------------
//
CNSPSessionData* CNSPSessionData::NewLC()
    {
    CNSPSessionData* self = new ( ELeave ) CNSPSessionData();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::~CNSPSessionData
// ---------------------------------------------------------------------------
//
CNSPSessionData::~CNSPSessionData()
    {
    delete iOffer;
    delete iAnswer;
    delete iCredentials;
    delete iUpdatedSdp;
    iRemoteCandidates.ResetAndDestroy();
    iRemoteCandidates.Close();
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SetOffer
// ---------------------------------------------------------------------------
//
void CNSPSessionData::SetOffer( CSdpDocument* aOffer )
    {
    if ( iOffer != aOffer )
        {
        delete iOffer;
        iOffer = aOffer;
        }
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::Offer
// ---------------------------------------------------------------------------
//
CSdpDocument& CNSPSessionData::Offer()
    {
    return (*iOffer);
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::ReleaseOffer
// ---------------------------------------------------------------------------
//
CSdpDocument* CNSPSessionData::ReleaseOffer()
    {
    CSdpDocument* offer = iOffer;
    iOffer = NULL;
    return offer;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SetAnswer
// ---------------------------------------------------------------------------
//
void CNSPSessionData::SetAnswer( CSdpDocument* aAnswer )
    {
    if ( iAnswer != aAnswer )
        {
        delete iAnswer;
        iAnswer = aAnswer;
        }
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::Answer
// ---------------------------------------------------------------------------
//
CSdpDocument& CNSPSessionData::Answer()
    {
    return (*iAnswer);
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::ReleaseAnswer
// ---------------------------------------------------------------------------
//
CSdpDocument* CNSPSessionData::ReleaseAnswer()
    {
    CSdpDocument* answer = iAnswer;
    iAnswer = NULL;
    return answer;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SetRole
// ---------------------------------------------------------------------------
//
void CNSPSessionData::SetRole( TNATFWIceRole aRole )
    {
    iRole = aRole;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::OperationMode
// ---------------------------------------------------------------------------
//
TNATFWIceRole CNSPSessionData::Role() const
    {
    return iRole;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SetUseIce
// ---------------------------------------------------------------------------
//
void CNSPSessionData::SetUseIce( TBool aUseIce )
    {
    iUseIce = aUseIce;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::UseIce
// ---------------------------------------------------------------------------
//
TBool CNSPSessionData::UseIce() const
    {
    return iUseIce;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SetSubState
// ---------------------------------------------------------------------------
//
void CNSPSessionData::SetSubState( CNSPSessionData::TSubState aSubState )
    {
    switch ( aSubState )
        {
        case CNSPSessionData::ESubMismatch:
            {
            iSubState = ( iSubState == CNSPSessionData::ESubRemoteLite ) ?
                        ESubAll :
                        aSubState;
            break;
            }
        case CNSPSessionData::ESubRemoteLite:
            {
            iSubState = ( iSubState == CNSPSessionData::ESubMismatch ) ?
                        ESubAll :
                        aSubState;
            break;
            }
        default:
            {
            iSubState = aSubState;
            break;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SubState
// ---------------------------------------------------------------------------
//
CNSPSessionData::TSubState CNSPSessionData::SubState() const
    {
    return iSubState;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::RemoteCandidates
// ---------------------------------------------------------------------------
//
RPointerArray<CNATFWCandidate>& CNSPSessionData::RemoteCandidates()
    {
    return iRemoteCandidates;
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::Credentials
// ---------------------------------------------------------------------------
//
CNATFWCredentials& CNSPSessionData::Credentials()
    {
    return (*iCredentials);
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::UpdatedSdp
// ---------------------------------------------------------------------------
//
CSdpDocument& CNSPSessionData::UpdatedSdp()
    {
    return (*iUpdatedSdp);
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::SetUpdatedSdp
// ---------------------------------------------------------------------------
//
void CNSPSessionData::SetUpdatedSdp( CSdpDocument* aDocument )
    {
    if ( iUpdatedSdp != aDocument )
        {
        delete iUpdatedSdp;
        iUpdatedSdp = aDocument;
        }
    }


// ---------------------------------------------------------------------------
// CNSPSessionData::ReleaseUpdatedSdp
// ---------------------------------------------------------------------------
//
CSdpDocument* CNSPSessionData::ReleaseUpdatedSdp()
	{
    CSdpDocument* update = iUpdatedSdp;
    iUpdatedSdp = NULL;
    return update;
	}


// end of file
