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

#ifndef __TTCSIPCOMMANDBASE_H__
#define __TTCSIPCOMMANDBASE_H__

// INCLUDES
#include "TTcCommandBase.h"

#include <sipheaderbase.h>
#include <_sipcodecdefs.h>

// FORWARD DECLARATIONS
class CSIPAddress;
class CSIPClientTransaction;
class CSIPContactHeader;
class CSIPDialog;
class CSIPDialogAssocBase;
class CSIPExpiresHeader;
class CSIPFromHeader;
class CSIPHeaderBase;
class CSIPInviteDialogAssoc;
class CSIPManagedProfile;
class CSIPMessageElements;
class CSIPProfile;
class CSIPRefresh;
class CSIPRegistrationBinding;
class CSIPRouteHeader;
class CSIPServerTransaction;
class CSIPSubscribeDialogAssoc;
class CSIPToHeader;
class CSIPURI;
class CTcSIPConnectionContainer;
class CTcSIPProfileContainer;
class CTcSIPContext;
class CSIPNotifyDialogAssoc;
class CSIPReferDialogAssoc;
class CSIPSubscriptionStateHeader;
class CSIPEventHeader;
class CSIPReferToHeader;
class CSIPContentTypeHeader;
class CUri8;


// CLASS DEFINITION
/**
 * TTcSIPCommandBase implements the base class for all SIP commands.
 * It provides common SIP related functionality for
 * accessing request and creating responses.
 */
class TTcSIPCommandBase
	: public TTcCommandBase
	{
	protected:	// Constructors and destructor

		/**
		 * Constructor.
		 *
		 * @param aContext SIP test context
		 */
		TTcSIPCommandBase( MTcTestContext& aContext );

	protected: // New methods

		/**
		 * Finds and extracts a SIP To-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found.
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPToHeader* ExtractToHeaderLC( TBool aIsMandatory = ETrue );

		/**
		 * Finds and extracts a SIP Contact-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPContactHeader* ExtractContactHeaderLC(
												TBool aIsMandatory = ETrue );

		/**
		 * Finds and extracts a SIP Route-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPRouteHeader* ExtractRouteHeaderLC( TBool aIsMandatory = ETrue );

		/**
		 * Finds and extracts a SIP From-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPFromHeader* ExtractFromHeaderLC( TBool aIsMandatory = ETrue );

		/**
		 * Finds and extracts a SIP Expires-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPExpiresHeader* ExtractExpiresHeaderLC(
												TBool aIsMandatory = ETrue );

		/**
		 * Finds and extracts a SIP ReferTo-header parameter from 
		 * the CTRL request. 
		 * Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPReferToHeader* ExtractReferToHeaderLC( 
												TBool aIsMandatory = ETrue );
		
		/**
		 * Finds and extracts a SIP Subscriptionstate-header parameter from 
		 * the CTRL request. 
		 * Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPSubscriptionStateHeader* ExtractSubStateHeaderLC(
												TBool aIsMandatory = ETrue );

		/**
		 * Finds and extracts a SIP Event-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPEventHeader* ExtractEventHeaderLC( TBool aIsMandatory = ETrue );

        
        /**
		 * Finds and extracts a SIP ContentType-header parameter from the CTRL
		 * request. Leaves pointer to cleanup stack if header is found
		 *
		 * @param aIsMandatory ETrue if the Header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if
		 *			   a required Header cannot be found.
		 */
		CSIPContentTypeHeader* ExtractContentTypeHeaderLC( TBool aIsMandatory = ETrue );
		
		/**
		 * Extracts any and all headers that are still left in the CTRL
		 * request (ExtractXXXHeaderLC() functions have removed found
		 * headers). Creates a SIP message elements object to contain
		 * the headers (it's created even if there are not headers).
		 *
		 * Uses ExtractHeadersAndContentL() with the created elements
		 * container.
		 *
		 * @return SIP message elements object.
		 */
		CSIPMessageElements* ExtractHeadersAndContentLC();

		/**
		 * Extracts any and all headers that are still left in the CTRL
		 * request (ExtractXXXHeaderLC() functions have removed found
		 * headers) to an existing message elements container.
		 *
		 * Uses also ExtractContentL() to get Content + Content-Type from
		 * CTRL request.
		 *
		 * @param aElements Container to be populated.
		 */
		void ExtractHeadersAndContentL( CSIPMessageElements& aElements );

		/**
		 * Search for a "Refresh" parameter from the CTRL request.
		 * Create a SIP refresh object if the parameter is found.
		 * Leaves pointer to cleanup stack if object is created.
		 *
		 * @return SIP refresh object or NULL if parameter cannot be found.
		 */
		CSIPRefresh* ExtractRefreshLC();

		/**
		 * Search for a "RemoteURI" parameter from the CTRL request.
		 * Create a SIP URI object if the parameter is found.
		 * Leaves pointer to cleanup stack if object is created.
		 *
		 * @return SIP URI object or NULL if parameter cannot be found.
		 * or leave if aIsMandatory is true and parameter cannot be found
		 */
		CUri8* ExtractRemoteURILC( TBool aIsMandatory = EFalse );

        /**
		 * Search for a "Proxy" parameter from the CTRL request.
		 * Create a SIP RouteHeader object if the parameter is found.
		 * Leaves pointer to cleanup stack if object is created.
		 *
		 * @return SIP RouteHeader object or NULL if parameter cannot be found.
		 */
		CSIPRouteHeader* ExtractProxyLC();
		
		/**
		 * Search for a aParamName parameter from the CTRL request.
		 * Extract the value of a found parameter to a heap descriptor
		 * and return it.
		 *
		 * @param aParamName Parameter name to be searched for
		 * @return Created heap descriptor or NULL
		 */
		HBufC8* ExtractHBufLC( const TDesC8& aParamName );

        /**
         * Extracts profileId and returns it as a integer
         *
         * @return profileId
         */
        TInt ExtractProfileIdL();
        
        /**
         * Extracts Uid
         *
         * @return uid
         */
        TUid ExtractUidL( const TDesC8& aName, TBool aIsMandatory = ETrue  );
        
		/**
		 * Returns the default connection, or a user defined connection
		 * if a ConnectionId is present in the CTLR request.
		 *
		 * @return Reference to a connection container.
		 */
		CTcSIPConnectionContainer& SelectConnectionL();

		/**
		 * Returns the default profile, or a user defined connection
		 * if a RegistryId is present in the CTLR request.
		 *
		 * @return Reference to a profile container.
		 */
		CTcSIPProfileContainer& SelectProfileL();

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
		 * Adds a ProfileId item to the CTRL response. Ownership is
		 * is transferred.
		 *
		 * @param aProfile Profile Id of this object is looked added
		 *				   to the response. aProfile is registered
		 *				   if it's not already in the registry.
		 */
		void AddProfileIdResponseL( CSIPProfile* aProfile );
		
		/**
		 * Adds a ProfileId item to the CTRL response. Ownership is
		 * is NOT transferred.
		 *
		 * @param aProfile Profile Id of this object is looked added
		 *				   to the response. aProfile is registered
		 *				   if it's not already in the registry.
		 */
		void AddProfileIdResponseL( CSIPProfile& aProfile );

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
		 * Searches for a DialogId field from the CTRL request. Fetches the
		 * associated SIP object from the test context's object registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPDialog* GetDialogL( TBool aIsMandatory = ETrue );

		/**
		 * Searches for a InviteDialogId or SubscribeDialogId field from
		 * the CTRL request. Fetches the associated SIP object from
		 * the test context's object registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPDialogAssocBase* GetAnyDialogAssocL( TBool aIsMandatory = ETrue );

		/**
		 * Searches for a InviteDialogId field from the CTRL request. Fetches
		 * the associated SIP object from the test context's object registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPInviteDialogAssoc* GetInviteDialogAssocL(
												TBool aIsMandatory = ETrue );

		/**
		 * Searches for a SubscribeDialogId field from the CTRL request.
		 * Fetches the associated SIP object from the test context's object
		 * registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPSubscribeDialogAssoc* GetSubscribeDialogAssocL(
												TBool aIsMandatory = ETrue );

		/**
		 * Searches for a CSIPNotifyDialogAssoc field from the CTRL request.
		 * Fetches the associated SIP object from the test context's object
		 * registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPNotifyDialogAssoc* GetNotifyDialogAssocL(
												TBool aIsMandatory = ETrue );

		/**
		 * Searches for a CSIPReferDialogAssoc field from the CTRL request.
		 * Fetches the associated SIP object from the test context's object
		 * registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPReferDialogAssoc* GetReferDialogAssocL(
												TBool aIsMandatory = ETrue );

		/**
		 * Searches for a TransactionId field from the CTRL request. Fetches
		 * the associated SIP object from the test context's object registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPClientTransaction* GetClientTransactionL(
												TBool aIsMandatory = ETrue );

		/**
		 * Searches for a ServerTransactionId field from the CTRL request.
		 * Fetches the associated SIP object from the test context's object
		 * registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPServerTransaction* GetServerTransactionL(
												TBool aIsMandatory = ETrue );

		/**
		 * Searches for a RegistrationId field from the CTRL request. Fetches
		 * the associated SIP object from the test context's object registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPRegistrationBinding* GetRegistrationL( TBool aIsMandatory = ETrue );

		/**
		 * Searches for a RefreshId field from the CTRL request. Fetches
		 * the associated SIP object from the test context's object registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return SIP object, or NULL if aIsMandatory = EFalse
		 *		   and object id was not found.
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPRefresh* GetRefreshL( TBool aIsMandatory = ETrue );

		/**
		 * Searches for a ProfileId field from the CTRL request. Fetches
		 * the associated ManagerProfile object from the test context's managed
		 * profile registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return reference to Profile object or leaves if profile for wanted
		 *         id is not found
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPManagedProfile& GetManagedProfileL( CTcSIPProfileContainer& aProfileContainer );

		/**
		 * Searches for a ProfileId field from the CTRL request. Fetches
		 * the associated Profile object from the test context's managed
		 * profile registry.
		 *
		 * @return reference to Profile object or leaves if profile for wanted
		 *         id is not found
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPProfile& GetProfileL( CTcSIPProfileContainer& aProfileContainer );
		
		/**
		 * Searches for a ProfileId field from the CTRL request. Fetches
		 * the associated Profile object from the test context's managed
		 * profile registry.
		 *
		 * @param aIsMandatory ETrue if the object is required.
		 * @return pointer to Profile object or NULL
		 * @exceptions Leaves with KTcErrMandatoryIdNotFound if
		 *			   a required object or object id cannot be found.
		 */
		CSIPProfile* GetProfileL( CTcSIPProfileContainer& aProfileContainer, TBool aIsMandatory );

		/**
		 * Returns a SIP profile if a ProfileId is present in the CTLR request.
		 *
		 * @return pointer to Profile object or NULL, 
		 *         the ownership is not transferred.
		 */
		CSIPProfile* GetProfileL();


	private: // New methods

		/**
		 * Search for a "Content" parameter from the CTRL request.
		 * Create a SIP content object if the parameter is found and
		 * pass it to the provided CSIPMessageElements object.
		 *
		 * @param aElements Reference to an initialized SIP message elements
		 *					container. Found content is placed here.
		 */
		void ExtractContentL( CSIPMessageElements& aElements );

		/**
		 * Extracts the named SIP header from the CTRL request parameters.
		 *
		 * @param aHeaderName SIP header name (e.g. "To")
		 * @param aIsMandatory ETrue if the header is required.
		 * @return SIP header object, or NULL if aIsMandatory = EFalse
		 *		   and header was not found.
		 * @exceptions Leaves with KTcErrMandatoryHeaderNotFound if a required
		 *			   header cannot be found.
		 */
		CSIPHeaderBase* ExtractHeaderLC( const TDesC8& aHeaderName,
										 TBool aIsMandatory );

		/**
		 * Extracts all user/extension headers from the CTRL request.
		 * Used by ExtractUserHeadersLC() to do the work.
		 *
		 * @param aHeaders Reference to a CTRL request header array
		 * @param aHeaderArray Reference to a SIP header array
		 */
		void ExtractUserHeadersL(
								CTcArray& aHeaders,
								RPointerArray< CSIPHeaderBase >& aHeaderArray );

        
		/**
		 * Extract the Name field from a SIP header
		 *
		 * @param aHeader Complete SIP header (e.g. "To: <sip:jesse@dot.net>"
		 * @return Pointer descriptor to Name field (e.g. "To")
		 */
		TPtrC8 NameFromHeader( const TDesC8& aHeader ) const;

		/**
		 * Extract the Value field from a SIP header
		 *
		 * @param aHeader Complete SIP header (e.g. "To: <sip:jesse@dot.net>"
		 * @return Pointer descriptor to Value field (e.g. "<sip:jesse@dot.net>")
		 */
		TPtrC8 ValueFromHeader( const TDesC8& aHeader ) const;

		/**
		 * Check if the specified SIP header is allowed to be passed through
		 * the SIP API.
		 *
		 * @param aName Header full name (e.g. "From")
		 * @return ETrue if header is allowed, EFalse if it is not allowed
		 *		   (i.e. it is private to the SIP stack)
		 */
		TBool IsHeaderAllowed( RStringF aName ) ;

	protected:	// Data

		/// Reference to the SIP test context. Not owned.
		CTcSIPContext& iContext;

		/// CleanupStack push count
		TInt iPushed;

	};

#endif // __TTCSIPCOMMANDBASE_H__
