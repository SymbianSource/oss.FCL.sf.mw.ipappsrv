/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    See class definition below.
*
*/



#ifndef __TTCMCECOMMANDBASE_H__
#define __TTCMCECOMMANDBASE_H__

// INCLUDES
#include "TTcCommandBase.h"
#include "CTcMCEContext.h"


// FORWARD DECLARATIONS



// CLASS DEFINITION
/**
 * TTcSIPCommandBase implements the base class for all SIP commands.
 * It provides common SIP related functionality for
 * accessing request and creating responses.
 */
class TTcMceCommandBase
	: public TTcCommandBase
	{
	protected:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP test context
		 */
		TTcMceCommandBase( MTcTestContext& aContext );

	protected: // New methods

		// Add here parameter extraction functions when needed
		
		/**
         * Extracts profileId and returns it as a integer
         *
         * @return profileId
         */
		TInt ExtractProfileIdL();
		
		/**
		 * Extracts array from CTRL request
		 *
		 * @param aName Name of the array to be extracted.
		 * @param aIsMandatory ETrue if the object is required.
		 * @return Pointer to array. Null if not found
		 *         and object is not required. Ownership is transferred.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if the 
		 *			   object is required but cannot be found.
		 */
		CDesC8Array* ExtractArrayL(const TDesC8& aName, 
								   TBool aIsMandatory = ETrue);
		
		/**
		 * Extracts SIP headers from CTRL request
		 *
		 * @param aIsMandatory ETrue if the headers are required.
		 * @return Pointer to array including headers. Null if not found
		 *         and headers are not required. Ownership is transferred.
		 * @exceptions Leaves with KTcErrMandatoryParameterNotFound if headers 
		 *			   are required but cannot be found.
		 */
		CDesC8Array* ExtractHeadersL(TBool aIsMandatory = ETrue);

		/**
		 * Converts any TPtrC8 to HBufC*
		 *
		 * @param ptr Descriptor to be converted
		 * @return Converted descriptor. Null if param is not a valid pointer.
		 *         Ownership is transferred.
		 */
		HBufC8* HBufCParameterL( TPtrC8 ptr );

		/**
		 * Returns the default profile, or a user defined profile
		 * if a RegistryId is present in the CTLR request.
		 *
		 * @return Reference to a profile container.
		 */
		CTcMCEProfileContainer& SelectProfileL();
		
		/**
		 * Searches for a ProfileId field from the CTRL request. Fetches
		 * the associated Profile object from the test context's
		 * profile registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return reference to Profile object or leaves if profile for wanted
		 *         id is not found
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPProfile& GetProfileL( CTcMCEProfileContainer& aProfileContainer );

	
		// Response creation functions

		/**
		 * Try to find an existing TestClientIds structure from the
		 * CTRL response. Create one if it cannot be found and add
		 * all possible Id items to it.
		 *
		 * @return Reference to the structure parameter containing
		 *		   pre-created TestClient Ids.
		 */
		CTcStructure& FindCreateTestClientIdsL();

		/**
		 * Adds a TestClientId item to the CTRL response. Ownership is
		 * is transferred.
		 *
		 * NOTE!! This function uses CleanupStack for safe array insertion.
		 * You must not call CleanupStack::PushL()/Pop() when calling this!!
		 *
		 * @param aName Id name (e.g. "RequestId")
		 * @param aObject Id of this object is looked up from the registry
		 *				  and added to the response. aObject is registered
		 *				  if it's not already in the registry.
		 */
		void AddIdResponseL( const TDesC8& aName, const CBase* aObject );

        /**
		 * Adds a TestClientId item to the CTRL response. Ownership is
		 * is NOT transferred.
		 *
		 * @param aName Id name (e.g. "RequestId")
		 * @param aObject Id of this object is looked up from the registry
		 *				  and added to the response. aObject is registered
		 *				  if it's not already in the registry.
		 */
        void AddIdResponseL( const TDesC8& aName, const CBase& aObject );
		 
		/**
		 * Adds headers to CTRL response. Ownership is NOT transferred.
		 * @param aHeaders Headers to be added to response.
		 */
		void AddHeadersL( const CDesC8Array* aHeaders );
		  
		/*
		 * Adds session state enumeration's textual correspondent to response
		 *
		 * @param aState State, which textual correspondent is added to response
		 */
		void AddTextualSessionStateL( TInt aState );

		/*
		 * Adds stream state enumeration's textual correspondent to response
		 *
		 * @param aState State, which textual correspondent is added to response
         * @param aBound If ETrue, param's name is "StreamStateText", else
         *               "BoundStreamStateText"
		 */
		void AddTextualStreamStateL( TBool aState, TBool aBound );


		/*
		 * Adds media stream state enumeration's textual correspondent to response
		 *
		 * @param aState State, which textual correspondent is added to response         
		 */
		 void AddTextualMediaStreamStateL( TInt aState );

			
				
		/*
		 * Adds event state enumeration's textual correspondent to response
		 *
		 * @param aState State, which textual correspondent is added to response
		 */
		void AddTextualEventStateL( TInt aState );

		/*
		 * Adds refer state enumeration's textual correspondent to response
		 *
		 * @param aState State, which textual correspondent is added to response
		 */
		void AddTextualReferStateL( TInt aState );


	protected:	// Data

		/// Reference to the MCE test context. Not owned.
		CTcMCEContext& iContext;

		/// CleanupStack push count
		TInt iPushed;

	};

#endif // __TTCMCECOMMANDBASE_H__
