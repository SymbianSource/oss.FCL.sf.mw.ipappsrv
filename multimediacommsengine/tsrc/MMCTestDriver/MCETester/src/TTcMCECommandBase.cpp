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


#include "CTcMCEContext.h"
#include "MCEConstants.h"
#include "TTcMceCommandBase.h"
#include "CTcMCEProfileContainer.h"
#include <MCESession.h>
#include <MCEEvent.h>
#include <MCERefer.h>
#include <MceMediaStream.h>


TTcMceCommandBase::TTcMceCommandBase( MTcTestContext& aContext )
	: TTcCommandBase( static_cast< CTcContextBase& >( aContext ) ),
	  iContext( static_cast< CTcMCEContext& >( aContext ) ),
	  iPushed( 0 )
	{
	}

//
// - Parameter extraction functions -------------------------------------------
//

TInt TTcMceCommandBase::ExtractProfileIdL()
    {
    TPtrC8 profileIdStr = ExtractIdL( KParamProfileId );
	TInt profileId( 0 );
	TLex8 lexer( profileIdStr );
	User::LeaveIfError( lexer.Val( profileId ) );
	return profileId;
    }

CSIPProfile& TTcMceCommandBase::GetProfileL( CTcMCEProfileContainer& aProfileContainer )
	{
	// Get profile from the registry
    return aProfileContainer.GetProfileByIdL( ExtractProfileIdL() );
	}
	
CTcMCEProfileContainer& TTcMceCommandBase::SelectProfileL()
	{
	TPtrC8 value = ExtractIdL( KRegistryId, EFalse );
	if( value != KNullDesC8 )
		{
		CTcMCEProfileContainer* conn
							= reinterpret_cast< CTcMCEProfileContainer* >(
								iContext.Registry().ObjectPtrL( value ) );
		return *conn;
		}
	else
		{
		return iContext.Profile();
		}
	}
	
CDesC8Array* TTcMceCommandBase::ExtractArrayL( const TDesC8& aName,
											   TBool aIsMandatory ) 
	{
	CTcArray* tcArray = FindArrayL(aName, aIsMandatory);
	if ( !tcArray )
		{
		return NULL;
		}
		
	CDesC8Array* array = new(ELeave)CDesC8ArraySeg(1);
	CleanupStack::PushL( array );
	
	const MDesC8Array& aItems = tcArray->AsMDesCArray(); 
	TInt count = aItems.MdcaCount();

	for( TInt i = 0; i < count; i++ )
		{
		array->AppendL( aItems.MdcaPoint( i ) );
		}
		
	CleanupStack::Pop( array );	
	return array;			
	}
	
CDesC8Array* TTcMceCommandBase::ExtractHeadersL( TBool aIsMandatory ) 
	{
	CDesC8Array* array = ExtractArrayL(KParamSipHeaders, aIsMandatory );
	if ( array )
		{
		if ( array->Count() > 0 )
			{
			return array;
			}
		}
	delete array;
	return NULL;
	}

HBufC8* TTcMceCommandBase::HBufCParameterL( TPtrC8 ptr )
	{
	HBufC8* buf = NULL;
	// MCE API should accept null parameters, but as long as it does not,
	// zero-length descriptors are used instead. When problem is corrected,
	// remove comments.
	//if (ptr != KNullDesC8 )
	//	{
		buf = ptr.AllocL();
	//	}	
	return buf;
	}
	
//
// - Response creation functions ----------------------------------------------
//

CTcStructure& TTcMceCommandBase::FindCreateTestClientIdsL()
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
		CleanupStack::Pop( tcIDs);

		// All IDs, in no spesific order (in alphabetical order in SIPTester)
        // (this has to be function-local in order to compile for THUMB)
		// Confirm that you don't have comma after last Id!
		const TDesC8* const TIdNames[] = {
			&KProfileObj, // Profile's name in TesterCores registry
			&KSessionId,
			&KEventId,
			&KReferId,
			&KStreamId,
			&KCodecId,
			&KSourceId,
			&KSinkId
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

void TTcMceCommandBase::AddIdResponseL( const TDesC8& aName,
										const CBase* aObject )
	{
	// Register object and fetch its name
	iContext.Registry().AddObjectL(	const_cast< CBase* >( aObject ) );
	TPtrC8 idName = iContext.Registry().ObjectNameL( aObject );	
	CTcStructure& tcIDs = FindCreateTestClientIdsL();
	CTcNameValue* idNameValue = tcIDs.ItemL( aName );
	idNameValue->SetValueL( idName );
	}
	
void TTcMceCommandBase::AddIdResponseL( const TDesC8& aName,
										const CBase& aObject )
	{
	// Register object and fetch its name
	iContext.Registry().AddObjectL(	aObject );
	TPtrC8 idName = iContext.Registry().ObjectNameL( &aObject );

	CTcStructure& tcIDs = FindCreateTestClientIdsL();
	CTcNameValue* idNameValue = tcIDs.ItemL( aName );
	idNameValue->SetValueL( idName );
	}

void TTcMceCommandBase::AddHeadersL( const CDesC8Array* aHeaders ) 
	{
	if ( aHeaders ) 
		{
		if ( aHeaders->Count() > 0 )
			{
			AddArrayResponseL( KResponseHeaders, *aHeaders );
			}
		}
	}

void TTcMceCommandBase::AddTextualSessionStateL( TInt aState) 
	{
	switch( aState ) 
		{
		case CMceSession::EIdle: AddTextResponseL(KResponseStateText, KIdle); break;
		case CMceSession::EOffering: AddTextResponseL(KResponseStateText, KOffering); break;
		case CMceSession::EIncoming: AddTextResponseL(KResponseStateText, KIncoming); break;
		case CMceSession::EAnswering: AddTextResponseL(KResponseStateText, KAnswering); break;
		case CMceSession::EProceeding: AddTextResponseL(KResponseStateText, KProceeding); break;
		case CMceSession::EEstablished: AddTextResponseL(KResponseStateText, KEstablished); break;
		case CMceSession::EReserving: AddTextResponseL(KResponseStateText, KUpdating); break;
		case CMceSession::ECancelling: AddTextResponseL(KResponseStateText, KCancelling); break;
		case CMceSession::ETerminating: AddTextResponseL(KResponseStateText, KTerminating); break;
		case CMceSession::ETerminated: AddTextResponseL(KResponseStateText, KTerminated); break;
		//case CMceSession::EOnHold: AddTextResponseL(KResponseStateText, KOnHold); break;
		//case CMceSession::EReferred: AddTextResponseL(KResponseStateText, KReferred); break;
		default: AddTextResponseL(KResponseStateText, KUnknownState); break;
		}
	}


void TTcMceCommandBase::AddTextualMediaStreamStateL( TInt aState) 
	{
	switch( aState ) 
		{
		case CMceMediaStream::EUninitialized: AddTextResponseL(KResponseMediaStreamStateText, KUninitialized); break;
		case CMceMediaStream::EInitialized: AddTextResponseL(KResponseMediaStreamStateText, KInitialized); break;
		case CMceMediaStream::EBuffering: AddTextResponseL(KResponseMediaStreamStateText, KBuffering); break;
		//case CMceMediaStream::EIdle: AddTextResponseL(KResponseMediaStreamStateText, KIdle); break;
		case CMceMediaStream::EIdle: AddTextResponseL(KResponseMediaStreamStateText, KmsIdle); break;
		case CMceMediaStream::EStreaming: AddTextResponseL(KResponseMediaStreamStateText, KStreaming); break;
		case CMceMediaStream::EDisabled: AddTextResponseL(KResponseMediaStreamStateText, KDisabled); break;
		case CMceMediaStream::ENoResources: AddTextResponseL(KResponseMediaStreamStateText, KNoResources); break;
		case CMceMediaStream::ETranscodingRequired: AddTextResponseL(KResponseMediaStreamStateText, KTranscodingRequired); break;
		case CMceMediaStream::ETranscoding: AddTextResponseL(KResponseMediaStreamStateText, KTranscoding); break;
		 
		default: AddTextResponseL(KResponseMediaStreamStateText, KUnknownState); break;
		}
	}	
	
// MediaStream state is removed. There are only 2 state enable and disable
void TTcMceCommandBase::AddTextualStreamStateL( TBool aState, TBool aBound ) 
	{
	TPtrC8 addedText;
	
	if ( aBound )
	    {
	    addedText.Set(KResponseBoundStreamStateText);
	    }
	else
	    {
	    addedText.Set(KResponseStreamStateText);
	    }
	
	if ( aState)
		{
		AddTextResponseL( addedText, KStreamEnable);	
		}
	else
		{
		AddTextResponseL( addedText, KStreamDisable);		
		}
	}

void TTcMceCommandBase::AddTextualEventStateL( TInt aState ) 
	{
	
	switch( aState ) 
		{
		case CMceEvent::EIdle: 
			{
			AddTextResponseL(KResponseEventStateText, KIdle); 
			break;
			}
		case CMceEvent::EPending: 
			{
			AddTextResponseL(KResponseEventStateText, KPending); 
			break;
			}
		case CMceEvent::EActive: 
			{	
			AddTextResponseL(KResponseEventStateText, KActive); 
			break;
			}
		case CMceEvent::ETerminated: 
			{
			AddTextResponseL(KResponseEventStateText, KTerminated); 
			break;
			}
		default: 
			{
			AddTextResponseL(KResponseEventStateText, KUnknownState); 
			break;
			}
		}
	
	}

void TTcMceCommandBase::AddTextualReferStateL( TInt aState ) 
	{
	
	switch( aState ) 
		{
		case CMceRefer::EIdle: 
			{
			AddTextResponseL(KResponseReferStateText, KIdle); 
			break;
			}
		case CMceRefer::EAccepted: 
			{	
			AddTextResponseL(KResponseReferStateText, KAccepted); 
			break;
			}
		case CMceRefer::ETerminated: 
			{
			AddTextResponseL(KResponseReferStateText, KTerminated); 
			break;
			}
		default: 
			{
			AddTextResponseL(KResponseReferStateText, KUnknownState); 
			break;
			}
		}

	}
