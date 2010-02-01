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

#ifndef __TTCSIPRECEIVED_H__
#define __TTCSIPRECEIVED_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSIPClientTransaction;
class CSIPDialog;
class CSIPDialogAssocBase;
class CSIPInviteDialogAssoc;
class CSIPSubscribeDialogAssoc;
class CSIPNotifyDialogAssoc;
class CSIPReferDialogAssoc;
class CSIPRefresh;
class CSIPRegistrationBinding;
class CSIPServerTransaction;
class CSIPTransactionBase;

// CLASS DEFINITION
/**
 * TTcSIPReceived implements a container class for SIP notification messages.
 * Objects of this class are created and stored in CSIPConnection observer
 * functions.
 */
class TTcSIPReceived
	{
	public:	// Constructors and destructor
	
	    /// Possible SIP events
	    enum TTcSIPEvent
	        {
	        ENone,
	        EIncomingRequest,
	        EIncomingResponse,
	        EErrorOccured,
	        EInviteCompleted,
	        EInviteCanceled
	        };

		/// Default constructor
		TTcSIPReceived();

		/// Destructor
		~TTcSIPReceived();

	public: // New methods

		/**
		 * Set SIP client transaction object pointer.
		 *
		 * @param aTransaction SIP client transaction
		 */
		void Set( const CSIPClientTransaction* aTransaction );

		/**
		 * Set SIP dialog object pointer.
		 *
		 * @param aDialog SIP dialog
		 */
		void Set( const CSIPDialog* aDialog );

		/**
		 * Set SIP dialog association object pointer.
		 *
		 * @param aDialogAssoc SIP dialog association
		 */
		void Set( const CSIPDialogAssocBase* aDialogAssoc );

		/**
		 * Set SIP dialog association object pointer.
		 *
		 * @param aDialogAssoc SIP dialog association
		 */
		void Set( const CSIPInviteDialogAssoc* aDialogAssoc );

		/**
		 * Set SIP dialog association object pointer.
		 *
		 * @param aDialogAssoc SIP dialog association
		 */
		void Set( const CSIPNotifyDialogAssoc* aDialogAssoc );

		/**
		 * Set SIP dialog association object pointer.
		 *
		 * @param aDialogAssoc SIP dialog association
		 */
		void Set( const CSIPReferDialogAssoc* aDialogAssoc );

		/**
		 * Set SIP refresh object pointer.
		 *
		 * @param aRefresh SIP refresh
		 */
		void Set( const CSIPRefresh* aRefresh );

		/**
		 * Set SIP registration object pointer.
		 *
		 * @param aRegistration SIP registration
		 */
		void Set( const CSIPRegistrationBinding* aRegistration );

		/**
		 * Set SIP server transaction object pointer.
		 *
		 * @param aTransaction SIP server transaction
		 */
		void Set( const CSIPServerTransaction* aTransaction );

		/**
		 * Set SIP transaction object pointer.
		 *
		 * @param aTransactionBase SIP transaction
		 */
		void Set( const CSIPTransactionBase* aTransactionBase );

		/**
		 * Set Internet Access Point ID.
		 *
		 * @param aIapId Internet Access Point ID
		 */
		void Set( TInt aIapId );

		/**
		 * Set error code
		 *
		 * @param aError System-wide or SIP specific error code
		 */
		void SetError( TInt aError );
		
		/**
		 * Set received event
		 *
		 * @param aEvent
		 */
		void SetEvent( TTcSIPEvent aEvent );

	public: // Data

		/// SIP client transaction pointer. Not owned.
		const CSIPClientTransaction* iClientTransaction;

		/// SIP dialog pointer. Not owned.
		const CSIPDialog* iDialog;

		/// SIP invite dialog association pointer. Not owned.
		const CSIPInviteDialogAssoc* iInviteDialogAssoc;

		/// SIP notify dialog association pointer. Not owned.
        const CSIPNotifyDialogAssoc* iNotifyDialogAssoc;

        /// SIP subscribe dialog association pointer. Not owned.
		const CSIPSubscribeDialogAssoc* iSubscribeDialogAssoc;

		/// SIP refer dialog association pointer. Not owned.
        const CSIPReferDialogAssoc* iReferDialogAssoc;

		/// SIP refresh pointer. Not owned.
		const CSIPRefresh* iRefresh;

		/// SIP registration pointer. Not owned.
		const CSIPRegistrationBinding* iRegistration;

		/// SIP server transaction pointer. Not owned.
		const CSIPServerTransaction* iServerTransaction;

		/// SIP transaction pointer. Not owned.
		const CSIPTransactionBase* iTransactionBase;

		/// Internet Access Point ID
		TInt iIapId;

		/// System-wide or SIP specific error code
		TInt iError;
		
		/// SIP event
		TTcSIPEvent iEvent;

	};

#endif // __TTCSIPRECEIVED_H__
