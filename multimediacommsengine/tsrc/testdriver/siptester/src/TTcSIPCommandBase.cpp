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

#include <sipaddress.h>
#include <sipcontenttypeheader.h>
#include <sipcseqheader.h>
#include <sipdialog.h>
#include <sipdialogassocbase.h>
#include <sipcontactheader.h>
#include <sipexpiresheader.h>
#include <sipfromheader.h>
#include <sipinvitedialogassoc.h>
#include <sipmanagedprofile.h>
#include <sipmanagedprofileregistry.h>
#include <sipmessageelements.h>
#include <siprefresh.h>
#include <siprouteheader.h>
#include <sipsubscribedialogassoc.h>
#include <siptoheader.h>
#include <sipprofile.h>
#include <sipheaderlookup.h>
#include <sipnotifydialogassoc.h>
#include <sipreferdialogassoc.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <_sipcodecdefs.h>
#include <sipsubscriptionstateheader.h>
#include <sipeventheader.h>
#include <siprefertoheader.h>
#include <uri8.h>

#include "CleanupResetAndDestroy.h"
#include "CTcSIPContext.h"
#include "SIPConstants.h"
#include "SipStrConsts.h"
#include "TcLog.h"
#include "TTcSIPCommandBase.h"

#include "CTcSIPProfileContainer.h"

TTcSIPCommandBase::TTcSIPCommandBase( MTcTestContext& aContext )
	: TTcCommandBase( static_cast< CTcContextBase& >( aContext ) ),
	  iContext( static_cast< CTcSIPContext& >( aContext ) ),
	  iPushed( 0 )
	{
	}

//
// - Header extraction functions ----------------------------------------------
//

CSIPToHeader* TTcSIPCommandBase::ExtractToHeaderLC(	TBool aIsMandatory )
	{
	return static_cast< CSIPToHeader* >(
				ExtractHeaderLC( KHeaderTo, aIsMandatory ) );
	}

CSIPContactHeader* TTcSIPCommandBase::ExtractContactHeaderLC(
														TBool aIsMandatory )
	{
	CSIPContactHeader* contact =
		static_cast< CSIPContactHeader* >(
							ExtractHeaderLC( KHeaderContact, aIsMandatory ) );
	return contact;
	}

CSIPRouteHeader* TTcSIPCommandBase::ExtractRouteHeaderLC( TBool aIsMandatory )
	{
	return static_cast< CSIPRouteHeader* >(
				ExtractHeaderLC( KHeaderRoute, aIsMandatory ) );				
	}

CSIPFromHeader* TTcSIPCommandBase::ExtractFromHeaderLC( TBool aIsMandatory )
	{
	CSIPFromHeader* from =
		static_cast< CSIPFromHeader* >(
							ExtractHeaderLC( KHeaderFrom, aIsMandatory ) );
	return from;
	}

CSIPExpiresHeader* TTcSIPCommandBase::ExtractExpiresHeaderLC( TBool aIsMandatory )
	{
	return static_cast< CSIPExpiresHeader* >(
				ExtractHeaderLC( KHeaderExpires, aIsMandatory ) );
	}

CSIPReferToHeader* TTcSIPCommandBase::ExtractReferToHeaderLC( TBool aIsMandatory )
	{
	return static_cast< CSIPReferToHeader* >(
				ExtractHeaderLC( KHeaderReferTo, aIsMandatory ) );
	}


CSIPSubscriptionStateHeader* TTcSIPCommandBase::ExtractSubStateHeaderLC( TBool aIsMandatory )
	{
	return static_cast< CSIPSubscriptionStateHeader* >(
				ExtractHeaderLC( KHeaderSubState, aIsMandatory ) );
	}

CSIPEventHeader* TTcSIPCommandBase::ExtractEventHeaderLC( TBool aIsMandatory )
	{
	return static_cast< CSIPEventHeader* >(
				ExtractHeaderLC( KHeaderEvent, aIsMandatory ) );
	}
	
CSIPContentTypeHeader* TTcSIPCommandBase::ExtractContentTypeHeaderLC( TBool aIsMandatory )
	{
	return static_cast< CSIPContentTypeHeader* >(
				ExtractHeaderLC( KHeaderContentType, aIsMandatory ) );
	}


CSIPMessageElements* TTcSIPCommandBase::ExtractHeadersAndContentLC()
	{
	// Construct message elements container
	CSIPMessageElements* elements = CSIPMessageElements::NewLC();

	ExtractHeadersAndContentL( *elements );

	return elements;
	}

void TTcSIPCommandBase::ExtractHeadersAndContentL( CSIPMessageElements& aElements )
	{
	// Extract Content + Content-Type to elements.
	ExtractContentL( aElements );

	CTcArray* headers = FindArrayL( KResponseHeaders, EFalse );
	if( !headers )
		{
		return;
		}

	// Extract any headers still left in the request into userHeaders
	RPointerArray< CSIPHeaderBase > userHeaders;
	ExtractUserHeadersL( *headers, userHeaders );

	// Give userHeaders to elements
	CleanupResetAndDestroyPushL( userHeaders );
	aElements.SetUserHeadersL( userHeaders );
	CleanupStack::Pop();	// userHeaders
	}

//
// - Parameter extraction functions -------------------------------------------
//

CSIPRefresh* TTcSIPCommandBase::ExtractRefreshLC()
	{
	TPtrC8 value = ExtractTextL( KParamRefresh, EFalse );
	if( value.CompareF( KTcTrue ) == 0 )
		{
		CSIPRefresh* refresh = CSIPRefresh::NewLC();
		iPushed++;
		return refresh;
		}

	return NULL;
	}

CUri8* TTcSIPCommandBase::ExtractRemoteURILC( TBool aIsMandatory )
	{
	TPtrC8 value = ExtractTextL( KParamRemoteURI, aIsMandatory );
	if( value != KNullDesC8 )
		{
    	TUriParser8 parser;
    	User::LeaveIfError(parser.Parse(value));
    	CUri8* uri8 = CUri8::NewLC(parser);   
		iPushed++;
		return uri8;
		}

	return NULL;
	}

CSIPRouteHeader* TTcSIPCommandBase::ExtractProxyLC()
	{
    CSIPRouteHeader* proxy = NULL;
    
   	TPtrC8 value = ExtractTextL( KParamProxy, EFalse );
	if( value != KNullDesC8 )
		{
		RPointerArray<CSIPRouteHeader> proxies = CSIPRouteHeader::DecodeL( value );
		
		// First of the proxy parameter addresses will be used altought there
		// shouldn't be more		
		if ( proxies.Count() > 0 )
		    {
			proxy = proxies[ 0 ];
		    // Used proxy is not deleted over here
		    proxies.Remove( 0 );
		    // Others can be deleted
		    proxies.ResetAndDestroy();
		    
		    CleanupStack::PushL( proxy );
		    iPushed++;	    
		    }	
		}
	return proxy;
	}

CTcSIPConnectionContainer& TTcSIPCommandBase::SelectConnectionL()
	{
	TPtrC8 value = ExtractIdL( KConnectionId, EFalse );
	if( value != KNullDesC8 )
		{
		CTcSIPConnectionContainer* conn
							= reinterpret_cast< CTcSIPConnectionContainer* >(
								iContext.Registry().ObjectPtrL( value ) );
		return *conn;
		}
	else
		{
		return iContext.Connection();
		}
	}
CTcSIPProfileContainer& TTcSIPCommandBase::SelectProfileL()
	{
	TPtrC8 value = ExtractIdL( KRegistryId, EFalse );
	if( value != KNullDesC8 )
		{
		CTcSIPProfileContainer* conn
							= reinterpret_cast< CTcSIPProfileContainer* >(
								iContext.Registry().ObjectPtrL( value ) );
		return *conn;
		}
	else
		{
		return iContext.Profile();
		}
	}
//
// - Response creation functions ----------------------------------------------
//
void TTcSIPCommandBase::AddIdResponseL( const TDesC8& aName,
										const CBase* aObject )
	{
	// Register object and fetch its name
	iContext.Registry().AddObjectL(	const_cast< CBase* >( aObject ) );
	TPtrC8 idName = iContext.Registry().ObjectNameL( aObject );

	CTcStructure& tcIDs = FindCreateTestClientIdsL();
	CTcNameValue* idNameValue = tcIDs.ItemL( aName );
	idNameValue->SetValueL( idName );
	}
	
void TTcSIPCommandBase::AddIdResponseL( const TDesC8& aName,
										const CBase& aObject )
	{
	// Register object and fetch its name
	iContext.Registry().AddObjectL(	aObject );
	TPtrC8 idName = iContext.Registry().ObjectNameL( &aObject );

	CTcStructure& tcIDs = FindCreateTestClientIdsL();
	CTcNameValue* idNameValue = tcIDs.ItemL( aName );
	idNameValue->SetValueL( idName );
	}

void TTcSIPCommandBase::AddProfileIdResponseL( CSIPProfile* aProfile )
	{
	// Register object
	iContext.Registry().AddObjectL(	aProfile );
	TBuf8< KTcMaxIntConversion > idName;
	
	TUint32 id( 0 );
	User::LeaveIfError( aProfile->GetParameter( KSIPProfileId, id ) );
	idName.AppendNum( (TInt)id );	

	CTcStructure& tcIDs = FindCreateTestClientIdsL();
	CTcNameValue* profileId =
					const_cast< CTcNameValue* >( tcIDs.ItemL( KProfileId ) );
	profileId->SetValueL( idName );
	}
	
void TTcSIPCommandBase::AddProfileIdResponseL( CSIPProfile& aProfile )
	{
	// Register object
	iContext.Registry().AddObjectL(	aProfile );
	TBuf8< KTcMaxIntConversion > idName;
	
	TUint32 id( 0 );
	User::LeaveIfError( aProfile.GetParameter( KSIPProfileId, id ) );
	idName.AppendNum( (TInt)id );	

	CTcStructure& tcIDs = FindCreateTestClientIdsL();
	CTcNameValue* profileId =
					const_cast< CTcNameValue* >( tcIDs.ItemL( KProfileId ) );
	profileId->SetValueL( idName );
	}

CTcStructure& TTcSIPCommandBase::FindCreateTestClientIdsL()
	{
	// Find the TestClientIds structure, it may be in several places
	// (there might already be a ReceivedMsg item in the return list)
	// It may also be missing altogether, if this is the first time
	// AddIdResponseL() is called.
	TInt pos = iContext.ReturnList().FindParameter( KResponseTestClientIds,
													MTcTyped::EStructure );
	if( pos == KErrNotFound )
		{
		// Figure out the correct position for the structure: just
		// after the ReceivedMsg if it exists.
		pos = iContext.ReturnList().FindParameter( KResponseHeaders,
												   MTcTyped::EArray );
		if( pos == KErrNotFound )
			{
			pos = 0;	// no headers, insert at the beginning
			}
		else
			{
			pos++;	// insert after headers
			}

		// Add a structure to the response parameter list for TestClient Ids
		CTcStructure* tcIDs = CTcStructure::NewLC();
		tcIDs->SetName( KResponseTestClientIds );
		iContext.ReturnList().InsertParameterL( tcIDs, pos );
		CleanupStack::Pop(); // tcIDs

		// All IDs, in alphabetical order
		// (this has to be function-local in order to compile for THUMB)
		const TDesC8* const TIdNames[] = {
			&KConnectionId,
			&KDialogId,
			&KInviteDialogId,
            &KNotifyDialogId,
			&KProfileId,
            &KReferDialogId,
			&KRefreshId,
            &KRegistryId,
			&KRegistrationId,
			&KServerTransactionId,
			&KSubscribeDialogId,
			&KTransactionId
			};

		// Add an entry for ALL possible Ids. This is done to go around
		// a Telelogic Tau logging problem related to template mismatches
		TInt count = sizeof( TIdNames ) / sizeof( const TDesC8* );
		for( TInt i = 0; i < count; i++ )
			{
			CTcNameValue* nameValue = CTcNameValue::NewLC();
			nameValue->SetL( *TIdNames[ i ], 0 );
			tcIDs->AddItemL( nameValue );
			CleanupStack::Pop( nameValue );
			}
		}

	return iContext.ReturnList().AsStructure( pos );
	}
//
// -- SIP object / registry access functions ----------------------------------
//

CSIPDialog* TTcSIPCommandBase::GetDialogL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPDialog* >(
							GetObjectForIdL( KDialogId, aIsMandatory ) );
	}

CSIPDialogAssocBase* TTcSIPCommandBase::GetAnyDialogAssocL(
														TBool aIsMandatory )
	{
	CSIPDialogAssocBase* dialogAssoc = GetInviteDialogAssocL( EFalse );
	if( dialogAssoc )
		{
		return dialogAssoc;
		}

	dialogAssoc = GetSubscribeDialogAssocL( EFalse );
	if( dialogAssoc )
		{
		return dialogAssoc;
		}

    dialogAssoc = GetReferDialogAssocL( EFalse );
	if( dialogAssoc )
		{
		return dialogAssoc;
		}

    dialogAssoc = GetNotifyDialogAssocL( EFalse );
	if( dialogAssoc )
		{
		return dialogAssoc;
		}
	
	if( aIsMandatory )
		{
		User::Leave( KTcErrMandatoryIdNotFound );
		}
	return NULL;
	}

CSIPInviteDialogAssoc* TTcSIPCommandBase::GetInviteDialogAssocL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPInviteDialogAssoc* >(
							GetObjectForIdL( KInviteDialogId, aIsMandatory ) );
	}

CSIPSubscribeDialogAssoc* TTcSIPCommandBase::GetSubscribeDialogAssocL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPSubscribeDialogAssoc* >(
							GetObjectForIdL( KSubscribeDialogId, aIsMandatory ) );
	}

CSIPNotifyDialogAssoc* TTcSIPCommandBase::GetNotifyDialogAssocL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPNotifyDialogAssoc* >(
							GetObjectForIdL( KNotifyDialogId, aIsMandatory ) );
	}

CSIPReferDialogAssoc* TTcSIPCommandBase::GetReferDialogAssocL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPReferDialogAssoc* >(
							GetObjectForIdL( KReferDialogId, aIsMandatory ) );
	}

CSIPClientTransaction* TTcSIPCommandBase::GetClientTransactionL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPClientTransaction* >(
							GetObjectForIdL( KTransactionId, aIsMandatory ) );
	}

CSIPServerTransaction* TTcSIPCommandBase::GetServerTransactionL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPServerTransaction* >(
							GetObjectForIdL( KServerTransactionId, aIsMandatory ) );
	}

CSIPRegistrationBinding* TTcSIPCommandBase::GetRegistrationL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPRegistrationBinding* >(
							GetObjectForIdL( KRegistrationId, aIsMandatory ) );
	}

CSIPRefresh* TTcSIPCommandBase::GetRefreshL( TBool aIsMandatory )
	{
	return reinterpret_cast< CSIPRefresh* >(
							GetObjectForIdL( KRefreshId, aIsMandatory ) );
	}

CSIPManagedProfile& TTcSIPCommandBase::GetManagedProfileL( CTcSIPProfileContainer& aProfileContainer )
	{
	// Get profile from the registry
    return aProfileContainer.GetManagedProfileByIdL( ExtractProfileIdL() );
	}

CSIPProfile& TTcSIPCommandBase::GetProfileL( CTcSIPProfileContainer& aProfileContainer )
	{
	// Get profile from the registry
    return aProfileContainer.GetProfileByIdL( ExtractProfileIdL() );
	}

CSIPProfile* TTcSIPCommandBase::GetProfileL( CTcSIPProfileContainer& aProfileContainer, TBool aIsMandatory )
	{
	// Get profile from the registry
	TPtrC8 profileIdStr = ExtractIdL( KParamProfileId, aIsMandatory );
	
	CSIPProfile* profile = 0;
	
	if ( profileIdStr != KNullDesC8 )
	    {
	    TInt profileId( 0 );
	    TLex8 lexer( profileIdStr );
	    User::LeaveIfError( lexer.Val( profileId ) );
	    profile = &aProfileContainer.GetProfileByIdL( profileId );
	    }
	
    return profile;
	}
	
CSIPProfile* TTcSIPCommandBase::GetProfileL()	
    {
    CSIPProfile* profile = NULL;
    TPtrC8 profileIdStr = ExtractIdL( KParamProfileId, EFalse );
	if ( profileIdStr.Length() > 0 )
	    {
	    TInt profileId( 0 );
	    TLex8 lexer( profileIdStr );
	    User::LeaveIfError( lexer.Val( profileId ) );
        CTcSIPProfileContainer& conn = SelectProfileL();
        profile = &( conn.GetProfileByIdL( profileId ) );
        }
	return profile;
    }		
//
// - Internal header utility functions ----------------------------------------
//

void TTcSIPCommandBase::ExtractContentL( CSIPMessageElements& aElements )
	{
	// Extract content type, if any
	CSIPContentTypeHeader* contentType = static_cast< CSIPContentTypeHeader* >(
				ExtractHeaderLC( KHeaderContentType, EFalse ) );
	if( !contentType )
		{
		return;
		}

	// Extract content, if any
	HBufC8* content = ExtractHBufLC( KParamContent );
	if( !content )
		{
		// If content didn't exist use empty content
		content = KNullDesC8().AllocLC();
		iPushed++;
		}

	aElements.SetContentL( content, contentType );

	CleanupStack::Pop( 2 );	// content, contentType
	iPushed -= 2; 	// content, contentType
	}

CSIPHeaderBase* TTcSIPCommandBase::ExtractHeaderLC(	const TDesC8& aHeaderName,
													TBool aIsMandatory )
	{
	TBuf8< KTcMaxTypeName > objectName( aHeaderName );
	TBuf8< KTcOptionalLength > optional; if( aIsMandatory ) optional = KTcMandatory; else optional = KTcOptional;
	TcLog::WriteFormat( _L8("-- Extract %s header \"%s\""), optional.PtrZ(), objectName.PtrZ() );

	CTcArray* headers = FindArrayL( KResponseHeaders, aIsMandatory );
	if( !headers )
		{
		// no headers
		return NULL;
		}

	TInt count = headers->Count();
	for( TInt i = 0; i < count; i++ )
		{
		TPtrC8 header = headers->Item( i );
		if( NameFromHeader( header ).CompareF( aHeaderName ) == 0 )
			{
			// Get headername from stringpool 
			TPtrC8 headerPtr = NameFromHeader( header );
			RStringF headerStr = SIPStrings::Pool().OpenFStringL(headerPtr);
			CleanupClosePushL(headerStr);
			
			RPointerArray< CSIPHeaderBase > headerArray =
				SIPHeaderLookup::CreateHeaderL( headerStr,
												 ValueFromHeader( header ) );
			CSIPHeaderBase* headerObject = headerArray[ 0 ];
			headerArray.Close();
			CleanupStack::PopAndDestroy(); // headerStr
			CleanupStack::PushL( headerObject );
			// header was extracted, remove it from original array
			headers->RemoveItem( i );
			iPushed++;
			TcLog::Write( KLogOk );
			return headerObject;
			}
		}

	TcLog::Write( KLogNotFound );
	if( aIsMandatory )
		{
		User::Leave( KTcErrMandatoryHeaderNotFound );
		}
	return NULL;
	}

void TTcSIPCommandBase::ExtractUserHeadersL(
								CTcArray& aHeaders,
								RPointerArray< CSIPHeaderBase >& aHeaderArray )
	{
	TInt count = aHeaders.Count();
	for( TInt i = 0; i < count; i++ )
		{
		// Instantiate correct SIP header object by using the Header factory
		TPtrC8 header = aHeaders.Item( i );

		TBuf8< KTcMaxTypeName > headerName( NameFromHeader( header ) );
		TcLog::WriteFormat( _L8("-- Extract user header \"%s\""), headerName.PtrZ() );
	
		// Get headername from stringpool 
		RStringF headerNameStr = SIPStrings::Pool().OpenFStringL(headerName);
		CleanupClosePushL(headerNameStr);
		
		if( IsHeaderAllowed( headerNameStr ) )
			{
			RPointerArray< CSIPHeaderBase > newArray =
					SIPHeaderLookup::CreateHeaderL( headerNameStr,
													 ValueFromHeader( header ) );
			CleanupResetAndDestroyPushL( newArray );

			// The factory might have created a bunch of headers, move them to
			// our array.
			TInt newCount = newArray.Count();
			for( TInt j = 0; j < newCount; j++ )
				{
				User::LeaveIfError( aHeaderArray.Append( newArray[ 0 ] ) );
				newArray.Remove( 0 );
				}
			CleanupStack::PopAndDestroy();	// newArray

			TcLog::Write( KLogOk );
			}
		else
			{
			TcLog::Write( KLogNotAllowed );
			User::Leave( KTcErrHeaderNotAllowed );
			}
		CleanupStack::PopAndDestroy(); // headerNameStr
		}
	}

HBufC8* TTcSIPCommandBase::ExtractHBufLC( const TDesC8& aParamName )
	{
	TPtrC8 value = ExtractTextL( aParamName, EFalse );
	if( value != KNullDesC8 )
		{
		HBufC8* buf = value.AllocLC();
		iPushed++;
		return buf;
		}

	return NULL;
	}
	
TInt TTcSIPCommandBase::ExtractProfileIdL()
    {
    TPtrC8 profileIdStr = ExtractIdL( KParamProfileId );
	TInt profileId( 0 );
	TLex8 lexer( profileIdStr );
	User::LeaveIfError( lexer.Val( profileId ) );
	return profileId;
    }
    
TUid TTcSIPCommandBase::ExtractUidL( const TDesC8& aName,TBool aIsMandatory  )
    {
    const TInt KHexAsTextValueStartPos = 2;
      
    TPtrC8 uidStr = ExtractTextL( aName, aIsMandatory );
    
    if ( !aIsMandatory && uidStr.Length() == 0 )
        {
        return TUid::Uid( 0 );
        }
    
    __ASSERT_ALWAYS( uidStr.Length() > KHexAsTextValueStartPos, User::Leave( KErrArgument ) );
	TUint uidVal( 0 );
	TLex8 lexer( uidStr.Mid( KHexAsTextValueStartPos ) );
	User::LeaveIfError( lexer.Val( uidVal, EHex ) );
	return TUid::Uid( static_cast<TInt>( uidVal ) );
    }

TPtrC8 TTcSIPCommandBase::NameFromHeader( const TDesC8& aHeader ) const
	{
	TInt pos = aHeader.Locate( KHeaderSeparator );
	if( pos == KErrNotFound )
		{
		// return an empty pointer descriptor
		return TPtrC8();
		}
	return aHeader.Left( pos );
	}

TPtrC8 TTcSIPCommandBase::ValueFromHeader( const TDesC8& aHeader ) const
	{
	TInt pos = aHeader.Locate( KHeaderSeparator );
	if( pos == KErrNotFound )
		{
		// return an empty pointer descriptor
		return TPtrC8( 0 );
		}
	pos++;	// skip the separator
	return aHeader.Mid( pos );
	}

TBool TTcSIPCommandBase::IsHeaderAllowed( RStringF aName )
	{
	// The SIP headers can be divided to two sets:
	// Supported headers and extension headers.
	// The supported headers can be further divided into two subsets:
	// API headers and private headers. 
	// Private headers cannot be set by the application.
	if( SIPHeaderLookup::IsAPIHeader( aName ))
		{
        if( aName == SIPStrings::StringF( SipStrConsts::EFromHeader ) ||
            aName == SIPStrings::StringF( SipStrConsts::EToHeader ) ||
            aName == SIPStrings::StringF( SipStrConsts::ECSeqHeader ) ||
            aName == SIPStrings::StringF( SipStrConsts::EContentTypeHeader ) )
		  	{
			return EFalse;
			}
		}
	else
		{
		if( SIPHeaderLookup::IsSupported( aName ) ) // Private header
			{
			return EFalse;
			}
		}
	return ETrue;
	}
