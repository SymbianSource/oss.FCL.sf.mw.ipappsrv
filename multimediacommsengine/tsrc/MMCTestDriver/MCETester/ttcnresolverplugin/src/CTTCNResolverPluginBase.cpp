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
* Description:    Implementation.
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
	const TUid KMyImplementationUid = { MyUid() };

	RImplInfoPtrArray infoArray;
	REComSession::ListImplementationsL( KSIPResolvedClientIFUid, infoArray );
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, &infoArray ) );

	TBool found = EFalse;
	for (TInt i=0; !found && i<infoArray.Count(); i++)
		{
		CImplementationInformation* info = infoArray[ i ];
		if ( info->ImplementationUid() == KMyImplementationUid )
			{
			TLex8 lex( info->DataType() );
			TUint value( 0 );
			User::LeaveIfError( lex.Val( value, EHex ) );
		    iApplicationUID.iUid = value;
			found = ETrue;
			}
		}

	CleanupStack::PopAndDestroy( 1 ); // infoArray

	if ( !found )
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
    //return iApplicationUID;
    const TUid KServerUid3 = {0x10203355};
    return KServerUid3;
    
    }


// -----------------------------------------------------------------------------
// CTTCNResolverPluginBase::ConnectL
// -----------------------------------------------------------------------------
//
void CTTCNResolverPluginBase::ConnectL( TUid /*aUid*/ )
	{
		/*
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
	*/
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

