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

#ifndef __CTCSIPCONTEXT_H__
#define __CTCSIPCONTEXT_H__

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include "CTcContextBase.h"
#include "CTcIAPManager.h"
#include <sipobserver.h>
#include <sipprofileregistryobserver.h>
#include "TTcProfileReceived.h"
#include "sipclientsimulatorcli.h"

// FORWARD DECLARATIONS
class CSIP;
class CTcSIPConnectionContainer;
class CTcSIPProfileContainer;
class CTcSIPHttpDigestContainer;
class CTcSIPClientDiscoveryContainer;
class CTcFreezer;
class CTcFileHandler;
class CTcSIPIMSAuthenticator;
class CTcSIPIMSAuthenticationParams;

// CLASS DEFINITION
/**
 * CTcSIPContext implements the test context (MTcTestContext) for SIP testing.
 */
class CTcSIPContext
	: public CTcContextBase,
	  public MSIPObserver
	{
	public:	// Constructors and destructor

		/**
		 * Static constructor. Leaves pointer to cleanup stack.
		 *
		 * @param aCodec Reference to CTRL codec object,
		 *				 initialized to a valid request.
		 * @param aIAPId IAPId as selected by user from TC settings.
         * @param aIsMandatory  used for determining if the SIP stack 
         *        is automatically started. If default value (ETrue) is used, 
         *        then SIP stack is started automatically.
		 * @return An initialized instance of this class.
		 */
		static CTcSIPContext* NewLC( CTcCTRLCodec& aCodec,
									 TInt aIAPId,
                                     TBool aIsMandatory );

		/// Destructor
		~CTcSIPContext();

	private:

		/**
		 * Constructor.
		 *
		 * @param aCodec Reference to CTRL codec object,
		 *				 initialized to a valid request.
		 */
		CTcSIPContext( CTcCTRLCodec& aCodec );

		/// Default constructor. Not implemented.
		CTcSIPContext();

		/**
		 * 2nd phase constructor
		 *
		 * @param aIAPId IAPId as selected by user from TC settings.
         * @param aIsMandatory  used for determining if the SIP stack 
         *        is automatically started. If default value (ETrue) is used, 
         *        then SIP stack is started automatically.
		 */
		void ConstructL( TInt aIAPId, TBool aIsMandatory  );

	protected: // From CTcContextBase

		TTcCommandBase* CreateCommandL( TTcIdentifier& aId,
										MTcTestContext& aContext );

	public: // From MSIPObserver

		void IncomingRequest( TUint32 aIapId,
							  CSIPServerTransaction* aTransaction );
		void TimedOut( CSIPServerTransaction& aSIPServerTransaction );
    
	public: // New methods

		/// @return Reference to the default SIP connection.
		inline CTcSIPConnectionContainer& Connection()
				{ return *iSipConnection; }

        /// @return Reference to the default SIP profile.
        inline CTcSIPProfileContainer& Profile()
                { return *iSipProfile; }
                
        /// @return Reference to the FileHandler
        inline CTcFileHandler& FileHandler()
                { return *iFileHandler; }     

		/// @return Reference to the SIP stack.
		inline CSIP& SIP()
				{ return *iSip; }

		/// @return Reference to the Internet Access Point name array
		inline const MDesC8Array& IAPNames() const
				{ return *iIapManager; }

		/**
		 * Lookup the CommDb IAP Id matching the provided IAP name.
		 *
		 * @param aName IAP name
		 * @return IAP id
		 */
		inline TUint32 IAPIdL( const TDesC8& aName ) const
				{ return iIapManager->IdForNameL( aName ); };

		/**
		 * Lookup the IAP Name matching the provided CommDb IAP Id.
		 *
		 * @param aIapId IAP id
		 * @return IAP name reference
		 */
		inline const TDesC8& IAPNameL( TInt aIapId ) const
				{ return iIapManager->NameForIdL( aIapId ); };

		/**
		 * Get reference to GetMandatoryValue
		 * This is used only if SIP stack is created manually
		 * @return boolean value indicating the 
         *         SIP stack state (ETrue==created)
		 */
		TBool GetMandatoryValue( void ) const;

		/**
		 * Create SIP stack
         * Create CSIP, CSIPConnection, CTcSIPConnectionContainer
         * and CTcSIPProfileContainer
		 * This is used only if SIP stack is created manually
		 */
        void CreateSipStack( void );
		CTcSIPConnectionContainer* CreateSipStack( const TDesC8& aName ); //16.9.

        /**
         * Gives reference to the default SIP Http Digest container or 
         * leaves with KErrNotFound if container doesn't exist
         *
         */
        CTcSIPHttpDigestContainer& DigestContainerL();
        
        /**
		 * Remove HttpDigest container 
		 *
		 */
		void RemoveDigestContainer();
		
		/**
		* Creates SIP ClientDiscovery container
		* leaves with KErrAlreadyExists if container exists already
		*/
		void CreateClientDiscoveryL( TUid aUid );
		
		/**
		* Removes SIP ClientDiscovery container
		*/
		void RemoveClientDiscovery();
		
		 /**
         * Gives reference to the SIP ClientDiscovery container or 
         * leaves with KErrNotFound if container doesn't exist
         *
         */
		CTcSIPClientDiscoveryContainer& ClientDiscoveryL();
		
		/**
		 * Freeze Tester thread asynchronously
		 *
		 */
		void IssueFreezeL( TInt aFreezeTimeSeconds );
		
		/**
		 * Freeze tester timer callback
		 *
		 */
		static TInt Freeze( TAny* aAny );
		
		
		/**
		 * Authenticate
		 */
		CTcSIPIMSAuthenticationParams& AuthenticateL( const TDesC8& aNonce );
		
		/**
		 * Map a CSIP error code to a verbose description
		 *
		 * @param aError One of CSIP::TError codes
		 * @return Error message for the code
		 */
		const TDesC8& SIPErrorToText( TInt aError );

        RSipClientSimulator& SipClientSimulator();
        
        CRepository& Repository();

	private: // data

        /// Boolean value indicating the SIP stack creation. Owned.
        TBool iIsMandatory;

		/// Internet Access Point name/id list. Owned.
		CTcIAPManager* iIapManager;
    
        /// IAP id. Owned.
        TInt iIAPId;

		/// SIP stack. Owned.
		CSIP* iSip;

		/// Default SIP connection. Owned.
		CTcSIPConnectionContainer* iSipConnection;

		/// Default SIP profile. Owned.
		CTcSIPProfileContainer* iSipProfile;
		
		/// Defaul SIP Http Digest. Owned.
		CTcSIPHttpDigestContainer* iSipHttpDigest;
		
		/// Defaul SIP ClientDiscover. Owned.
		CTcSIPClientDiscoveryContainer* iSipClientDiscovery;
		
		/// For Tester thread freezing
		CDeltaTimer* iFreezer;
		
		TInt iFreezeTimeSeconds;
		
		TDeltaTimerEntry iTimerEntry;
		
		/// For file creation and deletion. Owned.
		CTcFileHandler* iFileHandler;
		
		/// For IMS authentication. Owned.
		CTcSIPIMSAuthenticator* iIMSAuthenticator;
		
		RSipClientSimulator iSipClientSimulator;
		CRepository* iCenRep;
	};

#endif // __CTCSIPCONTEXT_H__
