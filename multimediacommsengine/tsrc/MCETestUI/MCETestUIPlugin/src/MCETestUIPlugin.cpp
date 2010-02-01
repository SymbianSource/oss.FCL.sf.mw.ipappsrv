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
* Description:    Implementation of MCETestUI client resolver for incoming SIP messages
*
*/




// INCLUDE FILES
#include "MCETestUIPlugin.h"
#include "EComImplementationProxy.h"
#include <badesca.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <stringpool.h>
// CONSTANTS
const TUid KMyApplicationUid = { 0x10281F9C };
_LIT(KCapabiliesFileName, "C:\\MCETestUICaps.xml");

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIPlugin* CMCETestUIPlugin::NewL()
    {
    CMCETestUIPlugin* self = new( ELeave ) CMCETestUIPlugin;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }                      

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::CMCETestUIPlugin
// -----------------------------------------------------------------------------
//
CMCETestUIPlugin::CMCETestUIPlugin() 
    {                      
    }

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIPlugin::ConstructL()
    {
	RImplInfoPtrArray infoArray;
	REComSession::ListImplementationsL( KSIPResolvedClientIFUid, infoArray );
	CleanupStack::PushL( TCleanupItem( ResetAndDestroy, &infoArray ) );

	TBool found = EFalse;
	for (TInt i=0; !found && i<infoArray.Count(); i++)
		{
		CImplementationInformation* info = infoArray[ i ];
		if ( info->ImplementationUid() == KMyApplicationUid )
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
		
	iCapabilitiesFromFile = KNullDesC8().AllocL();
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIPlugin::~CMCETestUIPlugin
// -----------------------------------------------------------------------------
//
CMCETestUIPlugin::~CMCETestUIPlugin()
    {
    delete iCapabilitiesFromFile;
    }

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::ChannelL
// -----------------------------------------------------------------------------
//
TUid CMCETestUIPlugin::ChannelL( 
	RStringF aMethod,
	const TDesC8& /*aRequestUri*/,
	const RPointerArray<CSIPHeaderBase>& /*aHeaders*/,
	const TDesC8& /*aContent*/,
	const CSIPContentTypeHeader* /*aContentType*/)
    {
    RDebug::Print(_L("New TestUI with KUidFCTestUI channel ID"));
	const TUid KUidFCTestUI = { 0x10281F9B };
	const TUid KUIOptionsUid = { 0x10123F9E };
	if ( aMethod == SIPStrings::StringF( SipStrConsts::EOptions ) ) 
		{
		return KUIOptionsUid;
		}
	else
		{
		return KUidFCTestUI;	
		}
    }

// -----------------------------------------------------------------------------
// ChannelL::ConnectL
// -----------------------------------------------------------------------------
//
void CMCETestUIPlugin::ConnectL( TUid /*aUid*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::Capabilities
// -----------------------------------------------------------------------------
//
const TDesC8& CMCETestUIPlugin::Capabilities()
	{
	TRAP_IGNORE(RefreshCapabilitiesL())
	return *iCapabilitiesFromFile;
	}

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::RefreshCapabilitiesL
// -----------------------------------------------------------------------------
//
void CMCETestUIPlugin::RefreshCapabilitiesL()
    {    
   	RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );

	RFile capabilitiesFile;
	TInt err = capabilitiesFile.Open( fs, KCapabiliesFileName, EFileRead );
	User::LeaveIfError( err );
	
	CleanupClosePushL( capabilitiesFile );

	TInt size( 0 );
	User::LeaveIfError( capabilitiesFile.Size( size ) );    
	HBufC8* tmp = HBufC8::NewLC( size );
	TPtr8 ptr( tmp->Des() );
	User::LeaveIfError( capabilitiesFile.Read( ptr ) );
	CleanupStack::Pop( tmp );
	delete iCapabilitiesFromFile;
	iCapabilitiesFromFile = tmp;   
	    
	CleanupStack::PopAndDestroy(); // capabilitiesFile
	CleanupStack::PopAndDestroy(); // fs
    }

// -----------------------------------------------------------------------------
// CMCETestUIPlugin::ResetAndDestroy
// -----------------------------------------------------------------------------
//
void CMCETestUIPlugin::ResetAndDestroy( TAny* anArray )
	{
    RImplInfoPtrArray* array = reinterpret_cast<RImplInfoPtrArray*>( anArray );
    if (array)
        {
        array->ResetAndDestroy();
        }
	}

//  End of File
