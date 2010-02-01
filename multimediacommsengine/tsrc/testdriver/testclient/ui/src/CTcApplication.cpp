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

#include "CTcApplication.h"
#include "CTcDocument.h"
#include "tcuid.h"


#ifdef EKA2
#include <eikstart.h>
#endif

EXPORT_C CApaApplication* NewApplication()
	{
	return new CTcApplication;
	}	

#ifdef EKA2
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }
#endif

CTcApplication::CTcApplication()
	{
	}

CTcApplication::~CTcApplication()
	{
	}

TUid CTcApplication::AppDllUid() const
	{
	return TUid::Uid( TCUID );
	}

CApaDocument* CTcApplication::CreateDocumentL()
	{
	// Create a new document, and return a pointer to it
	return CTcDocument::NewL( *this );
	}

