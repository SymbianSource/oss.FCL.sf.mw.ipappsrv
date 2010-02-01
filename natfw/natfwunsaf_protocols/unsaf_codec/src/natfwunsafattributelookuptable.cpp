/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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




#include "natfwunsafattributelookuptable.h"

#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafresponseaddressattribute.h"
#include "natfwunsafchangedaddressattribute.h"
#include "natfwunsafchangerequestattribute.h"
#include "natfwunsafsourceaddressattribute.h"
#include "natfwunsafreflectedfromattribute.h"
#include "natfwunsafmessageintegrityattribute.h"
#include "natfwunsafusernameattribute.h"
#include "natfwunsafpasswordattribute.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafunknownattributesattribute.h"
#include "natfwunsafxoronlyattribute.h"
#include "natfwunsafserverattribute.h"
#include "natfwunsafxormappedaddressattribute.h"
#include "natfwunsaffingerprintattribute.h"
// TURN specific attributes
#include "natfwunsafalternateserverattribute.h"
#include "natfwunsafdestinationaddressattribute.h"
#include "natfwunsafremoteaddressattribute.h"
#include "natfwunsaflifetimeattribute.h"
#include "natfwunsafbandwidthattribute.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafmagiccookieattribute.h"
#include "natfwunsafnonceattribute.h"
#include "natfwunsafrealmattribute.h"
#include "natfwunsafrelayaddressattribute.h"
#include "natfwunsafrequestedportpropsattribute.h"
#include "natfwunsafrequestedtransportattribute.h"
#include "natfwunsafrequestedipattribute.h"
#include "natfwunsaftimervalattribute.h"
#include "natfwunsafconnectstatattribute.h"
// ICE specific attributes
#include "natfwunsafpriorityattribute.h"
#include "natfwunsafusecandidateattribute.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsaficecontrollingattribute.h"

// -----------------------------------------------------------------------------
// CAttributeLookupTable::NewL
// -----------------------------------------------------------------------------
//
CAttributeLookupTable* CAttributeLookupTable::NewL()
    {
    CAttributeLookupTable* self = new (ELeave) CAttributeLookupTable();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CAttributeLookupTable::CAttributeLookupTable
// -----------------------------------------------------------------------------
//
CAttributeLookupTable::CAttributeLookupTable()
    {
    }

// -----------------------------------------------------------------------------
// CAttributeLookupTable::ConstructL
// -----------------------------------------------------------------------------
//
void CAttributeLookupTable::ConstructL()
    {
    AddItemL(CNATFWUNSAFAttribute::EMappedAddress,
             CNATFWUNSAFMappedAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EResponseAddress,
             CNATFWUNSAFResponseAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EChangedAddress,
             CNATFWUNSAFChangedAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EChangeRequest,
             CNATFWUNSAFChangeRequestAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ESourceAddress,
             CNATFWUNSAFSourceAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EReflectedFrom,
             CNATFWUNSAFReflectedFromAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EMessageIntegrity,
             CNATFWUNSAFMessageIntegrityAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EUsername,
             CNATFWUNSAFUsernameAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EPassword,
             CNATFWUNSAFPasswordAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EErrorCode,
             CNATFWUNSAFErrorCodeAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EUnknownAttributes,
             CNATFWUNSAFUnknownAttributesAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EXorOnly,
             CNATFWUNSAFXorOnlyAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EServer,
             CNATFWUNSAFServerAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EXorMappedAddress,
             CNATFWUNSAFXorMappedAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EFingerprint,
             CNATFWUNSAFFingerprintAttribute::DecodeAttributeL);
    // TURN specific attributes
    AddItemL(CNATFWUNSAFAttribute::EAlternateServer,
             CNATFWUNSAFAlternateServerAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EDestinationAddress,
             CNATFWUNSAFDestinationAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ERemoteAddress,
             CNATFWUNSAFRemoteAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ELifetime,
             CNATFWUNSAFLifetimeAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EBandwidth,
             CNATFWUNSAFBandwidthAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EData,
             CNATFWUNSAFDataAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EMagicCookie,
             CNATFWUNSAFMagicCookieAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ENonce,
             CNATFWUNSAFNonceAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ERealm,
             CNATFWUNSAFRealmAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ERelayAddress,
             CNATFWUNSAFRelayAddressAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ERequestedPortProps,
             CNATFWUNSAFRequestedPortPropsAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ERequestedTransport,
             CNATFWUNSAFRequestedTransportAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ERequestedIp,
             CNATFWUNSAFRequestedIpAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::ETimerVal,
             CNATFWUNSAFTimerValAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EConnectStat,
             CNATFWUNSAFConnectStatAttribute::DecodeAttributeL);
    // ICE specific attributes
    AddItemL(CNATFWUNSAFAttribute::EPriority,
             CNATFWUNSAFPriorityAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EUseCandidate,
             CNATFWUNSAFUseCandidateAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EIceControlled,
             CNATFWUNSAFIceControlledAttribute::DecodeAttributeL);
    AddItemL(CNATFWUNSAFAttribute::EIceControlling,
             CNATFWUNSAFIceControllingAttribute::DecodeAttributeL);
    }

// -----------------------------------------------------------------------------
// CAttributeLookupTable::~CAttributeLookupTable
// -----------------------------------------------------------------------------
//
CAttributeLookupTable::~CAttributeLookupTable()
    {
    iTable.Close();
    }

// -----------------------------------------------------------------------------
// CAttributeLookupTable::Find
// -----------------------------------------------------------------------------
//
TDecodeFnPtr CAttributeLookupTable::Find(TUint16 aAttributeType)
    {
    for (TInt i = 0; i < iTable.Count(); i++)
        {
        if (iTable[i].AttributeType() == aAttributeType)
            {
            return iTable[i].DecodeFunction();
            }
        }

    return NULL;
    }

// -----------------------------------------------------------------------------
// CAttributeLookupTable::AddItemL
// -----------------------------------------------------------------------------
//
void CAttributeLookupTable::AddItemL(CNATFWUNSAFAttribute::TType aAttributeType,
                                        TDecodeFnPtr aDecodeFn)
    {
    TAttributeLookupTableItem newItem(aAttributeType, aDecodeFn);
    iTable.AppendL(newItem);
    }
