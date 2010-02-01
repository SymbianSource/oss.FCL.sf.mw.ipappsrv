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




#include "natfwunsafmessagelookuptable.h"

#include "natfwunsafbindingrequest.h"
#include "natfwunsafbindingindication.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsafbindingerrorresponse.h"
#include "natfwunsafsharedsecretrequest.h"
#include "natfwunsafsharedsecretresponse.h"
#include "natfwunsafsharedsecreterrorresponse.h"
#include "natfwunsafallocaterequest.h"
#include "natfwunsafallocateresponse.h"
#include "natfwunsafallocateerrorresponse.h"
#include "natfwunsafdataindication.h"
#include "natfwunsafsetactivedestinationrequest.h"
#include "natfwunsafsetactivedestinationresponse.h"
#include "natfwunsafsetactivedestinationerrorresponse.h"
#include "natfwunsafsendindication.h"
#include "natfwunsafconnectrequest.h"
#include "natfwunsafconnectresponse.h"
#include "natfwunsafconnecterrorresponse.h"
#include "natfwunsafconnectstatusindication.h"

// -----------------------------------------------------------------------------
// CMessageLookupTable::NewL
// -----------------------------------------------------------------------------
//
CMessageLookupTable* CMessageLookupTable::NewL()
    {
    CMessageLookupTable* self = new (ELeave) CMessageLookupTable();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CMessageLookupTable::CMessageLookupTable
// -----------------------------------------------------------------------------
//
CMessageLookupTable::CMessageLookupTable()
    {
    }

// -----------------------------------------------------------------------------
// CMessageLookupTable::ConstructL
// -----------------------------------------------------------------------------
//
void CMessageLookupTable::ConstructL()
    {
    AddItemL(CNATFWUNSAFMessage::EBindingRequest,
             CNATFWUNSAFBindingRequest::CreateL);
    AddItemL(CNATFWUNSAFMessage::EBindingIndication,
             CNATFWUNSAFBindingIndication::CreateL);
    AddItemL(CNATFWUNSAFMessage::EBindingResponse,
             CNATFWUNSAFBindingResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::EBindingErrorResponse,
             CNATFWUNSAFBindingErrorResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESharedSecretRequest,
             CNATFWUNSAFSharedSecretRequest::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESharedSecretResponse,
             CNATFWUNSAFSharedSecretResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESharedSecretErrorResponse,
             CNATFWUNSAFSharedSecretErrorResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::EAllocateRequest,
             CNATFWUNSAFAllocateRequest::CreateL);
    AddItemL(CNATFWUNSAFMessage::EAllocateResponse,
             CNATFWUNSAFAllocateResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::EAllocateErrorResponse,
             CNATFWUNSAFAllocateErrorResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESendIndication,
             CNATFWUNSAFSendIndication::CreateL);
    AddItemL(CNATFWUNSAFMessage::EDataIndication,
             CNATFWUNSAFDataIndication::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESetActiveDestinationRequest,
             CNATFWUNSAFSetActiveDestinationRequest::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESetActiveDestinationResponse,
             CNATFWUNSAFSetActiveDestinationResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::ESetActiveDestinationErrorResponse,
             CNATFWUNSAFSetActiveDestinationErrorResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::EConnectRequest,
             CNATFWUNSAFConnectRequest::CreateL);
    AddItemL(CNATFWUNSAFMessage::EConnectResponse,
             CNATFWUNSAFConnectResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::EConnectErrorResponse,
             CNATFWUNSAFConnectErrorResponse::CreateL);
    AddItemL(CNATFWUNSAFMessage::EConnectStatusIndication,
             CNATFWUNSAFConnectStatusIndication::CreateL);
    }

// -----------------------------------------------------------------------------
// CMessageLookupTable::~CMessageLookupTable
// -----------------------------------------------------------------------------
//
CMessageLookupTable::~CMessageLookupTable()
    {
    iTable.Close();
    }

// -----------------------------------------------------------------------------
// CMessageLookupTable::Find
// -----------------------------------------------------------------------------
//
TFactoryFnPtr CMessageLookupTable::Find(TUint16 aMessageType)
    {
    for (TInt i = 0; i < iTable.Count(); i++)
        {
        if (iTable[i].MessageType() == aMessageType)
            {
            return iTable[i].FactoryFunction();
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CMessageLookupTable::AddItemL
// -----------------------------------------------------------------------------
//
void CMessageLookupTable::AddItemL(CNATFWUNSAFMessage::TType aMessageType,
                                      TFactoryFnPtr aFactoryFn)
    {
    TMessageLookupTableItem newItem(aMessageType, aFactoryFn);
    iTable.AppendL(newItem);
    }
