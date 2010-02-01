/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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




#include "mcetransactiondatacontainer.h"
#include "mceclilogs.h"
     
// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetStatusCode
// -----------------------------------------------------------------------------
//
EXPORT_C TMceTransactionDataContainer::TMceTransactionDataContainer() :
	iStatusCode(0),
	iReasonPhrase(0),
	iHeaders(0),
	iContentType(0),
	iContentHeaders(0),
	iContent(0),
	iReserved(0)
	{		
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetStatusCode
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMceTransactionDataContainer::GetStatusCode()
	{
	TInt tmpStatusCode = iStatusCode;
	iStatusCode = 0;
	return tmpStatusCode;
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetReasonPhrase
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TMceTransactionDataContainer::GetReasonPhrase()
	{
	HBufC8* tmpReasonPhrase = iReasonPhrase;
	iReasonPhrase = 0;
	return tmpReasonPhrase;		
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetHeaders
// -----------------------------------------------------------------------------
//
EXPORT_C CDesC8Array* TMceTransactionDataContainer::GetHeaders()
	{
	CDesC8Array* tmpHeaders = iHeaders;
	iHeaders = 0;
	return tmpHeaders;		
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetContentType
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TMceTransactionDataContainer::GetContentType()
	{
	HBufC8* tmpContentType = iContentType;
	iContentType = 0;
	return tmpContentType;		
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetHeaders
// -----------------------------------------------------------------------------
//
EXPORT_C CDesC8Array* TMceTransactionDataContainer::GetContentHeaders()
	{
	CDesC8Array* tmpHeaders = iContentHeaders;
	iContentHeaders = 0;
	return tmpHeaders;		
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::GetContent
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* TMceTransactionDataContainer::GetContent()
	{
	HBufC8* tmpContent = iContent;
	iContent = 0;
	return tmpContent;		
	}
     
// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::SetStatusCode
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::SetStatusCode( TInt aStatusCode)
	{
	iStatusCode = aStatusCode;		
	}
     	
// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::SetReasonPhrase
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::SetReasonPhrase( HBufC8* aReasonPhrase )
	{
	delete iReasonPhrase;
	iReasonPhrase = aReasonPhrase;
	}
     	
// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::SetHeaders
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::SetHeaders( CDesC8Array* aHeaders )
	{
	delete iHeaders;
	iHeaders = aHeaders;
	}
     	
// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::SetContentType
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::SetContentType( HBufC8* aContentType )
	{
	delete iContentType;
	iContentType = aContentType;
	}
     	
// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::SetContent
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::SetContent( HBufC8* aContent )
	{
	delete iContent;
	iContent = aContent;			
	}

// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::SetContentHeaders
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::SetContentHeaders( CDesC8Array* aHeaders )
	{
	delete iContentHeaders;
	iContentHeaders = aHeaders;
	}
     	
// -----------------------------------------------------------------------------
// TMceTransactionDataContainer::Clear
// -----------------------------------------------------------------------------
//
void TMceTransactionDataContainer::Clear()
	{	
	MCECLI_DEBUG( "TMceTransactionDataContainer::Clear, Entry" )
	
	iStatusCode = 0;
	delete iReasonPhrase;
	iReasonPhrase = 0;
	delete iHeaders;
	iHeaders = 0;
	delete iContentType;
	iContentType = 0;
	delete iContentHeaders;
	iContentHeaders = 0;
	delete iContent;
	iContent = 0;	

	MCECLI_DEBUG( "TMceTransactionDataContainer::Clear, Exit" )
	}
