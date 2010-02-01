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
* Description:  See class definition below.
*
*/

#ifndef __CTCCTRLCODEC_H__
#define __CTCCTRLCODEC_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CTcArray;
class CTcParameterList;
class CTcStructure;
class CTcNameValue;
class TTcIdentifier;

// CLASS DEFINITION
/**
 * CTcCTRLCodec implements a codec for the request/response/error
 * PDUs of the IUTCP control protocol. It is able to both parse existing
 * requests and construct new responses.
 */
class CTcCTRLCodec
	: public CBase
	{
	public: // Enumerations

		/// Request types
		enum TType
			{
			/// Unknown request
			EUnknown,
			/// Normal function invokation request
			ERequest,
			/// Reset request
			EResetRequest,
			/// Start Test Case request
			EStartRequest,
			/// End Test Case request
			EEndRequest
			};

	public:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aRequest External request PDU buffer
		 * @param aResponse External response PDU buffer
		 */
		IMPORT_C CTcCTRLCodec( TDes8& aRequest, TDes8& aResponse );

		/// Destructor
		IMPORT_C ~CTcCTRLCodec();

	private: // Constructors and destructor

		/// Default constructor. Not implemented.
		CTcCTRLCodec();

	public: // New methods

		/// @return ETrue if the request buffer contains a complete request.
		IMPORT_C TBool IsComplete() const;

		/// @return Request type
		IMPORT_C CTcCTRLCodec::TType RequestType() const;

		/**
		 * Extract the request identifier to the supplied object
		 *
		 * @param aIdentifier Reference to an identifier object that gets
		 *					  filled in.
		 */
		IMPORT_C void GetIdentifierL( TTcIdentifier& aIdentifier ) const;


		/**
		 * Extract the list of parameters to the supplied object. Note that
		 * CTcParameterList::Reset() is called before adding any parameters.
		 *
		 * @param aParameters Reference to a list container that gets filled in.
		 */
		IMPORT_C void GetParametersL( CTcParameterList& aParameters ) const;

		/**
		 * Constructs a complete response PDU.
		 *
		 * @param aIdentifier Class name from the reset request.
		 *					  Can also be KNullDesC8
		 * @param aReturnValue Initialized return value for the response
		 * @param aParameters Initialized parameter list for the response
		 */
		IMPORT_C void ConstructResponse( const TTcIdentifier& aIdentifier,
										 const CTcNameValue& aReturnValue,
										 const CTcParameterList& aParameters );

		/**
		 * Constructs a complete ok response PDU.
		 */
		IMPORT_C void ConstructOkResponse();

		/**
		 * Constructs a complete error response PDU.
		 *
		 * @param aMessage Error message text.
		 */
		IMPORT_C void ConstructErrorResponse( const TDesC8& aMessage );

	private: // New methods

		/**
		 * Append an identifier ("container.function;") to the response
		 *
		 * @param aIdentifier An initialized identifier
		 */
		void AddIdentifier( const TTcIdentifier& aIdentifier );

		/**
		 * Append a name-value pair ("TInt:10") to the response
		 *
		 * @param aPair An initialized name-value pair
		 */
		void AddNameValue( const CTcNameValue& aPair );

		/**
		 * Append a complex parameter list to the response.
		 * The list can contain arrays, structures and type-value pairs.
		 *
		 * @param aParameters An initialized parameter list container object
		 */
		void AddParameterList( const CTcParameterList& aParameters );

		/**
		 * Append a value array to the response
		 *
		 * @param aReturnValue An initialized array container object
		 */
		void AddArray( const CTcArray& aArray );

		/**
		 * Append a structure containing type-value pairs to the response
		 *
		 * @param aStruct An initialized structure container object
		 */
		void AddStructure( const CTcStructure& aStruct );

		/**
		 * Start constructing a new PDU to the response buffer. Clears
		 * the buffer and appends a PDU header.
		 *
		 * @param aPdu Pdu type header to be used.
		 */
		void StartPdu( const TDesC8& aPdu );

		/**
		 * Finish constructing a response PDU, adds the PDu end-mark "$".
		 */
		void FinishPdu();

		/**
		 * Apend a delimited string field to the response PDU
		 *
		 * @param aField Field contents
		 * @param aDelimiter Delimiter character
		 */
		void AddDelimited( const TDesC8& aField, TUint8 aDelimiter );

		/**
		 * Search for a character within the request PDU.
		 *
		 * @param aCharacter Character to be searched for.
		 * @param aStartPos Position within the request buffer from where to
		 *					start searching.
		 * @return Position of aCharacter, relational to aStartPos
		 * @exceptions Leaves with KTcErrRequestCorrupt if aDelimiter
		 *				was not found
		 */
		TInt FindL( TUint8 aCharacter, TInt aStartPos = 0 ) const;

		/**
		 * Search for a string within the request PDU.
		 *
		 * @param aString string to be searched for.
		 * @param aStartPos Position within the request buffer from where to
		 *					start searching.
		 * @return Position of aString, relational to aStartPos
		 * @exceptions Leaves with KTcErrRequestCorrupt if aDelimiter
		 *			   was not found
		 */
		TInt FindL( const TDesC8& aString, TInt aStartPos = 0 ) const;

		/**
		 * Search for a character within the request PDU.
		 *
		 * @param aCharacter Character to be searched for.
		 * @param aStartPos Position within the request buffer from where to
		 *					start searching.
		 * @return Position of aCharacter, relational to aStartPos
		 *		   or KErrNotFound(-1) if not found.
		 */
		TInt Find( TUint8 aCharacter, TInt aStartPos = 0 ) const;

		/**
		 * Search for a string within the request PDU.
		 *
		 * @param aString string to be searched for.
		 * @param aStartPos Position within the request buffer from where to
		 *					start searching.
		 * @return Position of aString, relational to aStartPos
		 *		   or KErrNotFound(-1) if not found.
		 */
		TInt Find( const TDesC8& aString, TInt aStartPos = 0 ) const;

		/**
		 * Construct a pointer descriptor to the request buffer.
		 * Starting at position aPos and ending at the position of
		 * aDelimiter or aSecondaryDelimiter, whichever is encountered first.
		 *
		 * @param aPos Start position. Points to next character
		 *			   after the used delimiter when this function returns.
		 * @param aDelimiter Primary delimiter character
		 * @param aSecondaryDelimiter Secondary delimiter character
		 * @return Pointer descriptor to extracted string
		 */
		TPtrC8 Extract( TInt& aPos, TUint8 aDelimiter,
						TUint8 aSecondaryDelimiter = 0 ) const;

	private: // Data

		/// Request buffer. Not owned.
		TDes8& iRequest;

		/// Response buffer. Not owned.
		TDes8& iResponse;

	};

#endif // __CTCCTRLCODEC_H__
