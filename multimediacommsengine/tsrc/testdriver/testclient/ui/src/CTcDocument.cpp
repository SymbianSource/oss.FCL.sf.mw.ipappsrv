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
* Description:  Implementation.
*
*/

#include "CTcAppUi.h"
#include "CTcDocument.h"

CTcDocument* CTcDocument::NewL( CEikApplication& aApp )
	{
	CTcDocument* self = new( ELeave ) CTcDocument( aApp );

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
	}

CTcDocument::CTcDocument( CEikApplication& aApp )
	: CEikDocument( aApp )
	{
	}

CTcDocument::~CTcDocument()
	{
	}

void CTcDocument::ConstructL()
	{
	}

CEikAppUi* CTcDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new( ELeave ) CTcAppUi;
	}

