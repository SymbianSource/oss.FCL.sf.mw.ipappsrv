/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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





#include "SipRequestElements.h"
#include "SipMessageElements.h"
#include "SipFromHeader.h"
#include "SipToHeader.h"
#include "uricontainer.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "_sipcodecdefs.h"


// -----------------------------------------------------------------------------
// CSIPRequestElements::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPRequestElements* CSIPRequestElements::NewL(CUri8* aRemoteUri)
	{
    CSIPRequestElements* self = CSIPRequestElements::NewLC(aRemoteUri);
    CleanupStack::Pop (self);
    return self;
	}	

// -----------------------------------------------------------------------------
// CSIPRequestElements::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPRequestElements* CSIPRequestElements::NewLC(CUri8* aRemoteUri)
	{
	CSIPRequestElements* self = new(ELeave)CSIPRequestElements;
    CleanupStack::PushL (self);
    self->ConstructL (aRemoteUri);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPRequestElements::CSIPRequestElements
// -----------------------------------------------------------------------------
//
CSIPRequestElements::CSIPRequestElements() 
	{
	}

// -----------------------------------------------------------------------------
// CSIPRequestElements::ConstructL
// -----------------------------------------------------------------------------
//
void CSIPRequestElements::ConstructL(CUri8* aRemoteUri)
	{
	__ASSERT_ALWAYS (aRemoteUri != 0, User::Leave(KErrArgument));

	iMessageElements = CSIPMessageElements::NewL();
    SetRemoteUriL(aRemoteUri);
	}

// -----------------------------------------------------------------------------
// CSIPRequestElements::~CSIPRequestElements
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPRequestElements::~CSIPRequestElements() 
	{
    delete iRemoteURI;
    iMethod.Close();
	delete iMessageElements;
	}

// -----------------------------------------------------------------------------
// CSIPRequestElements::SetToHeaderL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPRequestElements::SetToHeaderL(CSIPToHeader* aTo)
    {
    iMessageElements->SetToL(aTo);
    }
			
// -----------------------------------------------------------------------------
// CSIPRequestElements::ToHeader
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPToHeader* CSIPRequestElements::ToHeader() const  
    {
    return iMessageElements->To();
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::SetFromHeaderL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPRequestElements::SetFromHeaderL(CSIPFromHeader* aFrom)
    {
    iMessageElements->SetFromL(aFrom);
    }
			
// -----------------------------------------------------------------------------
// CSIPRequestElements::FromHeader
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPFromHeader* CSIPRequestElements::FromHeader() const
    {
    return iMessageElements->From();
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::SetRemoteURIL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPRequestElements::SetRemoteUriL(CUri8* aRemoteUri)
    {
	__ASSERT_ALWAYS (aRemoteUri != 0, User::Leave(KErrArgument));

    CURIContainer* tmp = CURIContainer::NewL(aRemoteUri);
    delete iRemoteURI;
    iRemoteURI = tmp;
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::RemoteURI
// -----------------------------------------------------------------------------
//
EXPORT_C const CUri8& CSIPRequestElements::RemoteUri() const
    {
    return *(iRemoteURI->Uri8());
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::SetMethodL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPRequestElements::SetMethodL(RStringF aMethod)
    {
	__ASSERT_ALWAYS (aMethod.DesC().Length() > 0, User::Leave(KErrArgument));

	iMethod.Close();
	iMethod = aMethod.Copy();
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::Method
// -----------------------------------------------------------------------------
//
EXPORT_C RStringF CSIPRequestElements::Method() const
    {
    return iMethod;
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::MessageElements
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPMessageElements& CSIPRequestElements::MessageElements() const
    {
    return *iMessageElements;
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::MessageElements
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPMessageElements& CSIPRequestElements::MessageElements()
    {
    return *iMessageElements;
    }

// -----------------------------------------------------------------------------
// CSIPRequestElements::InternalizeL
// -----------------------------------------------------------------------------
//
CSIPRequestElements* CSIPRequestElements::InternalizeL(RReadStream& aReadStream)
	{
	CSIPRequestElements* self = new(ELeave)CSIPRequestElements;
    CleanupStack::PushL (self);
    self->DoInternalizeL(aReadStream);
	CleanupStack::Pop(self); // self
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPRequestElements::DoInternalizeL
// -----------------------------------------------------------------------------
//
void CSIPRequestElements::DoInternalizeL(RReadStream& aReadStream)
	{
    // For incoming requests the method is always present
	TUint32 methodLength = aReadStream.ReadUint32L();
    if (methodLength > 0)
        {
	    HBufC8* methodBuf = HBufC8::NewLC(methodLength);
	    TPtr8 methodPtr(methodBuf->Des());
	    aReadStream.ReadL(methodPtr,methodLength);
	    iMethod = SIPStrings::Pool().OpenFStringL(methodPtr);
	    CleanupStack::PopAndDestroy(methodBuf);
        }
    aReadStream.ReadUint8L(); // remote-URI always present
    iRemoteURI = CURIContainer::InternalizeL(aReadStream);
    iMessageElements = CSIPMessageElements::InternalizeL(aReadStream);
	}

// -----------------------------------------------------------------------------
// CSIPRequestElements::ExternalizeL
// -----------------------------------------------------------------------------
//
void CSIPRequestElements::ExternalizeL(RWriteStream& aWriteStream) const
	{
	TPtrC8 method(iMethod.DesC());
    // For outgoing requests in some cases the method is filled in ServerCore
    if (method.Length() > 0)
        {
        aWriteStream.WriteUint32L(method.Length());
	    aWriteStream.WriteL(method);
        }
    aWriteStream.WriteUint8L(1); // remote-URI always present
    iRemoteURI->ExternalizeL(aWriteStream);
    iMessageElements->ExternalizeL(aWriteStream);
	}
