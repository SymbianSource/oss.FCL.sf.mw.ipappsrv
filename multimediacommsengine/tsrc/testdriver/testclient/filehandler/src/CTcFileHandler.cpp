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
* Description:  Implementation
*
*/

#include "CTcFileHandler.h"
#include "FileHandlerConstants.h"

EXPORT_C CTcFileHandler* CTcFileHandler::NewL()
	{
	CTcFileHandler* self = new ( ELeave ) CTcFileHandler;

	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();

	return self;
	}

EXPORT_C CTcFileHandler::~CTcFileHandler()
	{
	iHandler.Close();
	}

EXPORT_C void CTcFileHandler::CreateFileL( 
    const TDesC8& aDestinationPath, 
    const TDesC8& aData )
    {
    User::LeaveIfError( iHandler.CreateFile( aDestinationPath, aData ) );
    }
    
EXPORT_C void CTcFileHandler::CopyFileL( 
    const TDesC8& aSourcePath, 
    const TDesC8& aDestinationPath )
    {
    User::LeaveIfError( iHandler.CopyFile( aSourcePath, aDestinationPath ) );
    }
    
EXPORT_C void CTcFileHandler::DeleteFileL( const TDesC8& aDestinationPath )
    {
    User::LeaveIfError( iHandler.DeleteFile( aDestinationPath ) );
    }


CTcFileHandler::CTcFileHandler()
	{
	}

void CTcFileHandler::ConstructL()
	{
	User::LeaveIfError( iHandler.Connect() );
	}

