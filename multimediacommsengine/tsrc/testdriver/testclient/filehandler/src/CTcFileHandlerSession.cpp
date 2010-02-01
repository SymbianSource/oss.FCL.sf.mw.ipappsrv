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

#include "CTcFileHandlerSession.h"
#include "CTcFileHandlerServer.h"
#include "FileHandlerConstants.h"

#include <f32file.h>

CTcFileHandlerSession* CTcFileHandlerSession::NewL( 
    CTcFileHandlerServer* aServer )
	{
	CTcFileHandlerSession* self = new( ELeave ) CTcFileHandlerSession;

	CleanupStack::PushL( self );
	self->ConstructL( aServer );
	CleanupStack::Pop();

	return self;
	}

CTcFileHandlerSession::~CTcFileHandlerSession()
	{
	iFileHandlerServer->SessionRemoved();
	}

CTcFileHandlerSession::CTcFileHandlerSession()
	{
	}

void CTcFileHandlerSession::ConstructL( CTcFileHandlerServer* aServer )
	{
	iFileHandlerServer = aServer;
	iFileHandlerServer->SessionAdded();
	}

void CTcFileHandlerSession::ServiceL( const RMessage2& aMessage )
	{
	// Any leaves from this function will be catched by the Active Scheduler
	// and forwarded to CTcFileHandler::RunError().

	// Check for session related requests
	TInt status( KErrNone );
	switch( aMessage.Function() )
		{
		case ECloseSession:
			{
			// Session destruction is handled
			// by the client-server framework.
			break;
			}
		case EConfigAndStart:
			{
			// Nothing to do actually
			break;
			}
		case ECreateFile:
			{
			DoCreateFileL( aMessage );	
			break;
			}
		case ECopyFile:
			{
			DoCopyFileL( aMessage );	
			break;
			}
		case EDeleteFile:
			{
			DoDeleteFileL( aMessage );
			break;
			}
			
		default:
			{
			// Unknown request
			status = KErrUnknown;
			}
		}

	aMessage.Complete( status );
	}
	
void CTcFileHandlerSession::DoCreateFileL( const RMessage2& aMessage ) const
    {
    TFileName destinationPath;
    ReadFileNameL( 0, aMessage, destinationPath );
   	
   	HBufC8* data = ReadLC( 1, aMessage );
   	
   	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );
	
	// Make sure path exists, ignore errors since path might exist already
	fs.MkDirAll( destinationPath );	

	RFile out;
	User::LeaveIfError( out.Replace( fs, destinationPath, 
						EFileWrite ) );
	CleanupClosePushL( out );
	User::LeaveIfError( out.Write( *data ) );
	
	CleanupStack::PopAndDestroy( 3 );	// out, fs, data
    }
	
void CTcFileHandlerSession::DoCopyFileL( const RMessage2& aMessage ) const
    {
    TFileName sourcePath;
    ReadFileNameL( 0, aMessage, sourcePath );
    
    TFileName destinationPath;
    ReadFileNameL( 1, aMessage, destinationPath );
   	
    RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );
	
	CFileMan* fileMan = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMan); 
	
	// Make sure path exists, ignore errors since path might exist already
	fs.MkDirAll( destinationPath );	
	
	// Make the destination file writeable, ignore errors since most probably
	// file doesn't exist yet
    fileMan->Attribs( destinationPath, 0, KEntryAttReadOnly, TTime( 0 ), 0 );
    
	User::LeaveIfError( fileMan->Copy( sourcePath, destinationPath ) );
    
	CleanupStack::PopAndDestroy( 2 ); // fileMan, fs
    }
    
void CTcFileHandlerSession::DoDeleteFileL( const RMessage2& aMessage ) const
    {
    TFileName destinationPath;
    ReadFileNameL( 0, aMessage, destinationPath );
    
    // Connect to file server
	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	// Clear read-only and system flags in order to be able to delete the file
	// Ignore errors.
	fs.SetAtt( destinationPath, 0, KEntryAttReadOnly | KEntryAttSystem );

	// Try deleting the file, report errors (typically KErrNotFound)
	User::LeaveIfError( fs.Delete( destinationPath ) );

	// Close file server session
	CleanupStack::PopAndDestroy();	// fs
    }
    
void CTcFileHandlerSession::ReadFileNameL( 
    TInt aItcArgIndex,
    const RMessage2& aMessage,
    TFileName& aFileName ) const
    {
   	HBufC8* fileName = ReadLC( aItcArgIndex, aMessage );
   	
   	__ASSERT_ALWAYS( fileName->Des().Length() <= KMaxFileName, 
   	                 User::Leave( KErrArgument ) );
   	aFileName.Copy( *fileName );
   	
   	CleanupStack::PopAndDestroy(); // fileName
    }
    
HBufC8* CTcFileHandlerSession::ReadLC( 
    TInt aItcArgIndex,
    const RMessage2& aMessage ) const
    {
    TInt length = aMessage.GetDesLength( aItcArgIndex );
    if ( length < 0 )
        {
        User::Leave( KErrBadDescriptor );
        }
    HBufC8* buf = HBufC8::NewLC( length );
	if ( length > 0 )
		{
		TPtr8 bufPtr( buf->Des() );
		aMessage.ReadL( aItcArgIndex, bufPtr );
		}
    return buf;
    }
