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




#include "SipResponseElements.h"
#include "SipMessageElements.h"
#include "sipstrings.h"
#include "sipstrconsts.h"
#include "_sipcodecdefs.h"


// -----------------------------------------------------------------------------
// CSIPResponseElements::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPResponseElements*
CSIPResponseElements::NewL (TUint aStatusCode, RStringF aReasonPhrase)
	{
    CSIPResponseElements* self = 
		CSIPResponseElements::NewLC(aStatusCode,aReasonPhrase);
    CleanupStack::Pop (self);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPResponseElements*
CSIPResponseElements::NewLC (TUint aStatusCode, RStringF aReasonPhrase)
	{
	CSIPResponseElements* self = new(ELeave)CSIPResponseElements;
    CleanupStack::PushL (self);
    self->ConstructL (aStatusCode,aReasonPhrase);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::CSIPResponseElements
// -----------------------------------------------------------------------------
//
CSIPResponseElements::CSIPResponseElements ()
	{
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::ConstructL
// -----------------------------------------------------------------------------
//
void CSIPResponseElements::ConstructL (TUint aStatusCode,
									   RStringF aReasonPhrase)
	{
    SetStatusCodeL (aStatusCode);
	iReasonPhrase = aReasonPhrase.Copy();
    iMessageElements = CSIPMessageElements::NewL();
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::~CSIPResponseElements
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPResponseElements::~CSIPResponseElements () 
	{
    delete iMessageElements;
    iReasonPhrase.Close();
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::SetStatusCodeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPResponseElements::SetStatusCodeL (TUint aStatusCode)
	{
	__ASSERT_ALWAYS (aStatusCode > 100 && aStatusCode < 700,
	                 User::Leave(KErrArgument));
	iStatusCode = aStatusCode;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::StatusCode
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSIPResponseElements::StatusCode() const
	{
	return iStatusCode;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::SetReasonPhraseL
// -----------------------------------------------------------------------------
//
EXPORT_C void 
CSIPResponseElements::SetReasonPhraseL (RStringF aReasonPhrase)
	{
	iReasonPhrase.Close();
	iReasonPhrase = aReasonPhrase.Copy();
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::ReasonPhrase
// -----------------------------------------------------------------------------
//
EXPORT_C RStringF CSIPResponseElements::ReasonPhrase () const
	{
	return iReasonPhrase;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::FromHeader
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPFromHeader* CSIPResponseElements::FromHeader () const
    {
    return iMessageElements->From();
    }

// -----------------------------------------------------------------------------
// CSIPResponseElements::ToHeader
// -----------------------------------------------------------------------------
//	
EXPORT_C const CSIPToHeader* CSIPResponseElements::ToHeader () const
    {
    return iMessageElements->To();
    }

// -----------------------------------------------------------------------------
// CSIPResponseElements::CSeqHeader
// -----------------------------------------------------------------------------
//		
EXPORT_C const CSIPCSeqHeader* CSIPResponseElements::CSeqHeader() const
    {
    return iMessageElements->CSeq();
    }

// -----------------------------------------------------------------------------
// CSIPResponseElements::MessageElements
// -----------------------------------------------------------------------------
//
EXPORT_C const CSIPMessageElements& 
CSIPResponseElements::MessageElements () const
    {
    return *iMessageElements;
    }

// -----------------------------------------------------------------------------
// CSIPResponseElements::MessageElements
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPMessageElements& CSIPResponseElements::MessageElements ()
    {
    return *iMessageElements;
    }

// -----------------------------------------------------------------------------
// CSIPResponseElements::InternalizeL
// -----------------------------------------------------------------------------
//
CSIPResponseElements* 
CSIPResponseElements::InternalizeL (RReadStream& aReadStream)
	{
	CSIPResponseElements* self =
		CSIPResponseElements::InternalizeLC(aReadStream);
	CleanupStack::Pop(self); // self
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::InternalizeLC
// -----------------------------------------------------------------------------
//
CSIPResponseElements* 
CSIPResponseElements::InternalizeLC (RReadStream& aReadStream)
	{
	CSIPResponseElements* self = new(ELeave)CSIPResponseElements;
    CleanupStack::PushL (self);
    self->DoInternalizeL(aReadStream);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::DoInternalizeL
// -----------------------------------------------------------------------------
//
void CSIPResponseElements::DoInternalizeL (RReadStream& aReadStream)
	{
	iStatusCode = aReadStream.ReadUint16L();
	TUint32 reasonPhraseLength = aReadStream.ReadUint32L();
	HBufC8* reasonPhraseBuf = HBufC8::NewLC(reasonPhraseLength);
	TPtr8 reasonPhrasePtr(reasonPhraseBuf->Des());
	aReadStream.ReadL(reasonPhrasePtr,reasonPhraseLength);
	iReasonPhrase = SIPStrings::Pool().OpenFStringL(reasonPhrasePtr);
	CleanupStack::PopAndDestroy(reasonPhraseBuf);
    iMessageElements = CSIPMessageElements::InternalizeL(aReadStream);
	}

// -----------------------------------------------------------------------------
// CSIPResponseElements::ExternalizeL
// -----------------------------------------------------------------------------
//
void CSIPResponseElements::ExternalizeL (RWriteStream& aWriteStream) const
	{
	aWriteStream.WriteUint16L(iStatusCode);
	TPtrC8 reasonPhrase(iReasonPhrase.DesC());	
	aWriteStream.WriteUint32L(reasonPhrase.Length());
	aWriteStream.WriteL(reasonPhrase);
    iMessageElements->ExternalizeL(aWriteStream);
	}
