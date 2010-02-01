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

#include "CTcCTRLCodec.h"
#include "TTcIdentifier.h"
#include "CTcNameValue.h"
#include "CTcParameterList.h"
#include "CTcArray.h"
#include "CTcStructure.h"
#include "TesterConstants.h"

_LIT8( KTcOctetStrindByteLen, "byteLen" );
_LIT8( KTcOctetStringData,	  "data" );


EXPORT_C CTcCTRLCodec::CTcCTRLCodec( TDes8& aRequest, TDes8& aResponse )
	: iRequest( aRequest ), iResponse( aResponse )
	{
	}

EXPORT_C CTcCTRLCodec::~CTcCTRLCodec()
	{
	}

EXPORT_C TBool CTcCTRLCodec::IsComplete() const
	{
	// Try to find the CTRL header
	TInt headerPos = iRequest.Find( KTcCTRLVersion );
	if( headerPos == KErrNotFound )
		{
		return EFalse;
		}

	// Delete any garbage before the header
	// (have yet to see any, but it's still good to be on the safe side..)
	if( headerPos > 0 )
		{
		iRequest.Delete( 0, headerPos );
		}

	// The request is considered complete if the end delimiter is found.
	if( iRequest.Locate( KTcPduEnd ) == KErrNotFound )
		{
		return EFalse;
		}
	return ETrue;
	}

EXPORT_C CTcCTRLCodec::TType CTcCTRLCodec::RequestType() const
	{
	// Make sure we have a complete request in hand
	if( !IsComplete() )
		{
		return EUnknown;
		}

	// Get request identifier
	TTcIdentifier id;
	TRAPD( err, GetIdentifierL( id ) );
	// Categorize request as unknown if there are parsing errors
	if( err )
		{
		return EUnknown;
		}

	// Determine request type
	if( id.iFunction == KTcResetPdu )
		{
		return EResetRequest;
		}
	else if( id.iFunction == KTcStartPdu )
		{
		return EStartRequest;
		}
	else if( id.iFunction == KTcEndPdu )
		{
		return EEndRequest;
		}
	return ERequest;
	}

EXPORT_C void CTcCTRLCodec::GetIdentifierL( TTcIdentifier& aIdentifier ) const
	{
	// Find the first field delimiter after the pdu-id
	TInt identifierPos = FindL( KTcReqPdu );
	identifierPos += FindL( KTcField, identifierPos );
	identifierPos++;	// skip the delimiter

	// Find the next field delimiter and a possible scope delimiter
	TInt nextFieldPos = FindL( KTcField, identifierPos );
	TInt nextScopePos = Find( KTcScope, identifierPos );
	// If the scope delimiter was not found in the identifier field, ignore it
	if( nextScopePos > nextFieldPos )
		{
		nextScopePos = KErrNotFound;
		}

	// Do we have just a function name or also a class/object name?
	if( nextScopePos == KErrNotFound )
		{
		aIdentifier.iFunction.Set( iRequest.Mid( identifierPos, nextFieldPos ) );
		}
	else
		{
		// Yes, it was "container.function" notation
		aIdentifier.iContainer.Set( iRequest.Mid( identifierPos,
												  nextScopePos ) );
		nextScopePos++;	// skip the delimiter
		aIdentifier.iFunction.Set( iRequest.Mid( identifierPos + nextScopePos,
												 nextFieldPos - nextScopePos ) );
		}
	}

EXPORT_C void CTcCTRLCodec::GetParametersL( CTcParameterList& aParameters ) const
	{
	// Delete any previous parameters from the array
	aParameters.Reset();

	// Skip three fields (version + pdu-id + identifier)
	TInt pos = FindL( KTcReqPdu );
	pos += FindL( KTcField, pos );
	pos++;	// skip the delimiter
	pos += FindL( KTcField, pos );
	pos++;	// skip the delimiter

	// loop until the end delimiter is encountered
	FOREVER
		{
		// Determine parameter type
		TUint8 c = iRequest[ pos ];

		// Is it an array?
		if( c == KTcArrayBegin )
			{
			// Construct a new Array
			CTcArray* array = CTcArray::NewLC();
			// skip array begin delimiter
			pos++;
			// Extract and set array name
			array->SetName( Extract( pos, KTcType ) );

			// Read all array items
			do
				{
				// Try to exact an array item value
				TPtrC8 token = Extract( pos, KTcArrayValue, KTcArrayEnd );
				// If the array is empty, Extract would return KNullDesC8
				if( token != KNullDesC8 )
					{
					array->AddItemL( token );
					}
				} while( iRequest[ pos ] != KTcField );

			pos++;	// skip the field delimiter after the array

			// Store the Array object in our parameter array
			aParameters.AddParameterL( array );
			CleanupStack::Pop( array );
			}
		// Or a structure?
		else if( c == KTcStructBegin )
			{
			// Construct a new Structure
			CTcStructure* structure = CTcStructure::NewLC();
			// skip structure begin delimiter
			pos++;
			// Extract and set structure name
			structure->SetName( Extract( pos, KTcType ) );

			// Read all structure items (Type-Value pairs)
			FOREVER
				{
				// End of structure?
				if( iRequest[ pos ] == KTcStructEnd )
					{
					pos += 2;	// skip also field delimiter after structure
					break;
					}

				// Construct a new Type-Value pair
				CTcNameValue* tv = CTcNameValue::NewLC();
				// Extract values from the request to it
				tv->SetNameL( Extract( pos, KTcType ) );
				tv->SetValueL( Extract( pos, KTcField ) );
				// ..and store it to the structure
				structure->AddItemL( tv );
				CleanupStack::Pop();	// tv
				}

			/// Store the Structure object in our parameter array
			aParameters.AddParameterL( structure );
			CleanupStack::Pop( structure );
			}
		// Did the request end?
		else if( c == KTcPduEnd )
			{
			// end of request reached, stop processing
			break;
			}
		// it's a type-value pair if nothing else matches
		else
			{
			// Construct a new Type-Value pair
			CTcNameValue* tv = CTcNameValue::NewLC();
			// Extract values from the request to it
			tv->SetNameL( Extract( pos, KTcType ) );
			tv->SetValueL( Extract( pos, KTcField ) );
			// ..and store it to our parameter array
			aParameters.AddParameterL( tv );
			CleanupStack::Pop();	// tv
			if ( (tv->Value()).CompareC( KTcOctetStrindByteLen ) )
				{
				// Construct a new Type-Value pair
				CTcNameValue* dataName = CTcNameValue::NewLC();				
				
				TInt start( 0 );
				TInt delim1Pos = Find( KTcPduEnd, start );

				TInt len=iRequest.Length();
				
				TPtrC8 data=iRequest.Right( len - delim1Pos-1 );

				// Extract values from the request to it
				dataName->SetNameL( KTcOctetStringData );
				dataName->SetValueL( data );
				
				aParameters.AddParameterL( dataName );

				CleanupStack::Pop();	// dataName
				}
			}
		}
	}

EXPORT_C void CTcCTRLCodec::ConstructResponse(
										const TTcIdentifier& aIdentifier,
										const CTcNameValue& aReturnValue,
										const CTcParameterList& aParameters )
	{
	StartPdu( KTcResPdu );
	AddIdentifier( aIdentifier );
	AddNameValue( aReturnValue );
	AddParameterList( aParameters );
	FinishPdu();
	}

EXPORT_C void CTcCTRLCodec::ConstructOkResponse()
	{
	StartPdu( KTcResPdu );
	AddIdentifier( TTcIdentifier( KTcOkPdu ) );
	FinishPdu();
	}

EXPORT_C void CTcCTRLCodec::ConstructErrorResponse( const TDesC8& aMessage )
	{
	StartPdu( KTcErrPdu );
	AddDelimited( aMessage, KTcField );
	FinishPdu();
	}

void CTcCTRLCodec::AddIdentifier( const TTcIdentifier& aIdentifier )
	{
	if( aIdentifier.iContainer != KNullDesC8 )
		{
		AddDelimited( aIdentifier.iContainer, KTcScope );
		}
	AddDelimited( aIdentifier.iFunction, KTcField );
	}

void CTcCTRLCodec::AddNameValue( const CTcNameValue& aPair )
	{
	AddDelimited( aPair.Name(), KTcType );
	AddDelimited( aPair.Value(), KTcField );
	}

void CTcCTRLCodec::AddParameterList( const CTcParameterList& aParameters )
	{
	// Add parameter list elements to the request one by one
	TInt paramCount = aParameters.Count();
	for( TInt i = 0; i < paramCount; i++ )
		{
		switch( aParameters.ParameterType( i ) )
			{
			case MTcTyped::EArray:
				{
				AddArray( aParameters.AsArray( i ) );
				break;
				}
			case MTcTyped::EStructure:
				{
				AddStructure( aParameters.AsStructure( i ) );
				break;
				}
			case MTcTyped::ENameValue:
			default:
				{
				AddNameValue( aParameters.AsNameValue( i ) );
				break;
				}
			}
		}
	}

void CTcCTRLCodec::AddArray( const CTcArray& aArray )
	{
	// Add array header
	iResponse.Append( KTcArrayBegin );
	AddDelimited( aArray.Name(), KTcType );

	// Add array items one by one
	TInt count = aArray.Count();
	for( TInt i = 0; i < count; i++ )
		{
		AddDelimited( aArray.Item( i ), KTcArrayValue );
		}
	// Remove the last value delimiter, it is not allowed..
	iResponse.Delete( iResponse.Length() - 1, 1 );

	// Add array trailer
	iResponse.Append( KTcArrayEnd );
	iResponse.Append( KTcField );
	}

void CTcCTRLCodec::AddStructure( const CTcStructure& aStruct )
	{
	// Add structure header
	iResponse.Append( KTcStructBegin );
	AddDelimited( aStruct.Name(), KTcType );

	// Add structure items one by one
	TInt count = aStruct.Count();
	for( TInt i = 0; i < count; i++ )
		{
		AddNameValue( aStruct.Item( i ) );
		}

	// Add structure trailer
	iResponse.Append( KTcStructEnd );
	iResponse.Append( KTcField );
	}

void CTcCTRLCodec::StartPdu( const TDesC8& aPdu )
	{
	iResponse.Zero();
	AddDelimited( KTcCTRLVersion, KTcField );
	AddDelimited( aPdu, KTcField );
	}

void CTcCTRLCodec::FinishPdu()
	{
	iResponse.Append( KTcPduEnd );
	}

void CTcCTRLCodec::AddDelimited( const TDesC8& aField,
								 TUint8 aDelimiter )

	{
	iResponse.Append( aField );
	iResponse.Append( aDelimiter );
	}

TInt CTcCTRLCodec::FindL( TUint8 aCharacter, TInt aStartPos ) const

	{
	// Try finding the specified character, leave with KTcErrRequestCorrupt if it
	// cannot be found
	TInt pos = Find( aCharacter, aStartPos );
	if( pos == KErrNotFound )
		{
		User::Leave( KTcErrRequestCorrupt );
		}
	return pos;
	}

TInt CTcCTRLCodec::FindL( const TDesC8& aString, TInt aStartPos ) const

	{
	// Try finding the specified string, leave with KTcErrRequestCorrupt if it
	// cannot be found
	TInt pos = Find( aString, aStartPos );
	if( pos == KErrNotFound )
		{
		User::Leave( KTcErrRequestCorrupt );
		}
	return pos;
	}

TInt CTcCTRLCodec::Find( TUint8 aCharacter, TInt aStartPos ) const

	{
	return iRequest.Mid( aStartPos ).Locate( aCharacter );
	}

TInt CTcCTRLCodec::Find( const TDesC8& aString, TInt aStartPos ) const

	{
	return iRequest.Mid( aStartPos ).Find( aString );
	}

TPtrC8 CTcCTRLCodec::Extract( TInt& aPos, TUint8 aDelimiter,
						      TUint8 aSecondaryDelimiter ) const
	{
	// Search for the delimiters, determine which one comes first
	TInt delim1Pos = Find( aDelimiter, aPos );
	TInt delim2Pos = Find( aSecondaryDelimiter, aPos );
	if( ( ( delim1Pos > delim2Pos ) || ( delim1Pos == KErrNotFound ) )
		&& ( delim2Pos != KErrNotFound ) )
		{
		delim1Pos = delim2Pos;
		}

	TInt startPos = aPos;
	aPos += delim1Pos + 1;
	return iRequest.Mid( startPos, delim1Pos );
	}
