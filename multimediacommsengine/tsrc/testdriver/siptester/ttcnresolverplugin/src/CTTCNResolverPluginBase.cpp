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

#include "CTTCNResolverPluginBase.h"
#include <apacmdln.h>


// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ConstructL
// -----------------------------------------------------------------------------
//
void CTTCNResolverPluginBase::ConstructL()
	{
	TBool found = EFalse;
	
	const TUid KMyImplementationUid = { MyUid() };
	
	TFileName uidFileName;
    ConstructUidFileNameL( _L("c:\\ttcnresolverpluginuid"), 
                           MyUid(), 
                           _L(".txt"),
                           uidFileName );
	
	HBufC8* uidText = ReadFileContentsL( uidFileName );
	if ( uidText )
	    {
	    found = ETrue;
	    }
    else
        {
    	RImplInfoPtrArray infoArray;
    	REComSession::ListImplementationsL( KSIPResolvedClientIFUid, 
    	                                    infoArray );
    	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, &infoArray ) );
    	
    	for (TInt i=0; !found && i<infoArray.Count(); i++)
    		{
    		CImplementationInformation* info = infoArray[ i ];
    		if ( info->ImplementationUid() == KMyImplementationUid )
    			{
    			uidText = info->DataType().AllocL();
    			found = ETrue;
    			}
    		}
    		
    	CleanupStack::PopAndDestroy(); // infoArray
        }

    if ( found )
		{
		CleanupStack::PushL( uidText );
		TLex8 lex( *uidText );
    	TUint value( 0 );
    	User::LeaveIfError( lex.Val( value, EHex ) );
        iApplicationUID.iUid = value;
        CleanupStack::PopAndDestroy( uidText );
		}
    else
        {
        User::Leave( KErrNotFound );
        }

	User::LeaveIfError( iApaSession.Connect() );
	}

// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::~CTTCNResolverPluginBase
// -----------------------------------------------------------------------------
//
CTTCNResolverPluginBase::~CTTCNResolverPluginBase()
	{
	iApaSession.Close();
	}
	
// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ChannelL
// -----------------------------------------------------------------------------
//	
TUid CTTCNResolverPluginBase::ChannelL( RStringF /*aMethod*/,
    const TDesC8& /*aRequestUri*/,
    const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
    const TDesC8& /*aContent*/,
    const CSIPContentTypeHeader* /*aContentType*/)
    {
    return iApplicationUID;
    }


// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ConnectL
// -----------------------------------------------------------------------------
//
void CTTCNResolverPluginBase::ConnectL( TUid aUid )
	{
	TApaAppInfo appInfo;
	User::LeaveIfError( iApaSession.GetAppInfo( appInfo, aUid ) );
	CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
	#ifdef EKA2
	cmdLine->SetExecutableNameL( appInfo.iFullName );
	#else
	cmdLine->SetLibraryNameL( appInfo.iFullName );
	#endif
	User::LeaveIfError( iApaSession.StartApp( *cmdLine ) );
	CleanupStack::PopAndDestroy( cmdLine ); 
	}

// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ResetAndDestroy
// -----------------------------------------------------------------------------
//
void CTTCNResolverPluginBase::ResetAndDestroy( TAny* anArray )
	{
    RImplInfoPtrArray* array = reinterpret_cast<RImplInfoPtrArray*>( anArray );
    if (array)
        {
        array->ResetAndDestroy();
        }
	}

// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ReadFileContentsL
// -----------------------------------------------------------------------------
//
HBufC8* CTTCNResolverPluginBase::ReadFileContentsL( 
    const TFileName& aFileName )
    {
    RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile file;
	TInt err = file.Open( fs, aFileName, EFileRead );
	
	HBufC8* fileContents = 0;
	
	if ( err == KErrNone )
	    {
	    CleanupClosePushL( file );

        TInt size( 0 );
        
        User::LeaveIfError( file.Size( size ) );

        fileContents = HBufC8::NewLC( size );
        
        TPtr8 ptr( fileContents->Des() );
	    User::LeaveIfError( file.Read( ptr ) );  
	     
	    CleanupStack::Pop( fileContents );
	    CleanupStack::PopAndDestroy(); // file
	    }
	
	CleanupStack::PopAndDestroy(); // fs
	
	return fileContents;
    }

// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ConstructUidFileNameL
// -----------------------------------------------------------------------------
//
void CTTCNResolverPluginBase::ConstructUidFileNameL( 
    const TDesC& aFilePath, 
    TInt aUid, 
    const TDesC& aExtension, 
    TFileName& aFileName )
    {
    __ASSERT_ALWAYS( aFilePath.Length() + aExtension.Length() + 14 <= KMaxFileName,
                     User::Leave( KErrOverflow ) );
                     
    aFileName.Copy( aFilePath );
    aFileName.Append( _L("_") );
	aFileName.AppendNum( aUid, EHex );
	aFileName.Append( aExtension );
    }

