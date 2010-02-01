/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    The class for retrieving SIP profiles from permanent storage.
*
*/




#ifndef CSIPPROFILEREGISTRYBASE_H
#define CSIPPROFILEREGISTRYBASE_H

//  INCLUDES

#include <e32base.h>
#include <badesca.h>

#include "sipconcreteprofileobserver.h"

// FORWARD DECLARATIONS

class CSIPProfile;
class CSIPConcreteProfile;
class TSIPProfileTypeInfo;
class CSIPProfileCleanupItem;
class CSIPProfileItem;
class CSIPProfileITCReceiver;
class CSIPProfileITC;
class CSIPProfilePlugins;
class MSIPProfileRegistryObserver;
class RSIPProfile;
class CSIPConcreteProfileHolder;

// CLASS DECLARATION

/**
* The class functions for retrieving SIP profiles from permanent storage.
* It enables retrieval of single or more profiles from permanent
* storage located in profile server.
*  
* The class enables also the query of supported profile types.
*
* This class is an abstract class and cannot be instantiated.
*
* @lib sipprofilecli.lib
*/

class CSIPProfileRegistryBase: public CBase, public MSIPConcreteProfileObserver
	{

    public:  // Constructors and destructor

		IMPORT_C ~CSIPProfileRegistryBase();

	public: //new functions
		/**
		* Gets all SIP profiles
		* @pre aProfiles.Count() == 0
		* @param aProfiles a reference to a client-owned array, which
		*        on return is filled with all existing SIP profiles
		* @leave KErrNoMemory if out of memory
		*/
		IMPORT_C void ProfilesL(RPointerArray<CSIPProfile>& aProfiles);

		/**
		* Gets all SIP profiles of particular type
		* The owneship of profiles are transferred.
		* @pre aProfiles.Count() == 0
		* @param aType a SIP profile type that is used for a query
		* @param aProfiles a reference to a client-owned array, which
		*        on return is filled with profiles of the given type.
		* @leave KErrNoMemory if out of memory
		*/		
		IMPORT_C void ProfilesL(
					 const TSIPProfileTypeInfo& aType,
					 RPointerArray<CSIPProfile>& aProfiles);

		/**
		* Gets SIP profile for a particular SIP AOR
		* The owneship of profiles are transferred.
		* @pre aProfiles.Count() == 0
		* @param aAOR a SIP AOR e.g. user@domain.com
		* @param aProfiles a reference to a client-owned array, which
		*        on return is filled with profiles of the given type.
		* @leave KErrNoMemory if out memory
		*/
		IMPORT_C void ProfilesL(
			         const TDesC8& aAOR,
					 RPointerArray<CSIPProfile>& aProfiles);

		/**
		* Gets SIP profile identified by the Id
		* The owneship of profile is transferred.
		* @param aId a SIP profile Id
		* @return SIP profile 
		* @leave KErrNotFound if profile is not found
		*/
		IMPORT_C CSIPProfile* ProfileL(TUint32 aId);

		/**
		* Gets the default profile
		* @return default profile; the owneship is transfered
		* @leave KErrNotFound if profile not found
		* @leave KErrNoMemory if out of memory
		*/
		IMPORT_C CSIPProfile* DefaultProfileL();

		/**
		* Gets supported profile types
		* The owneship of returned objects are transferred.
		* @pre aSupportedProfileTypes.Count() == 0
		* @param aSupportedProfileTypes a reference to  client-owned array,
		*        which on return is filled with supported profile types.
		*	     Any existing data in the array is destroyed.
		* @leave KErrNoMemory if out of memory
		*/
		IMPORT_C void SupportedProfileTypesL(
					RArray<TSIPProfileTypeInfo>& aSupportedProfileTypes);

		/**
		* Gets the last occurred error during registration
		* @param aProfile 
		* @return last occurred registration error, KErrNone if no error
		*/
		IMPORT_C TInt LastRegistrationError(const CSIPProfile& aProfile) const;

	public:

		/**
		* Enable profile
		* @param aProfile to be enabled
		*/
		void EnableProfileL(CSIPProfile& aProfile);
		
		/**
		* Gets count of clients that have a profile enabled
		* @param aProfile profile to be checked
		* @return number of clients that have the profile enabled
		*/
		TInt UsageL(const CSIPProfile& aProfile) const;

		/**
		* Disable profile
		* @param aProfile to be disabled
		*/
		void DisableProfileL(CSIPProfile& aProfile);

		/**
		* Add profile
		* @param aProfile to be added
		*/
		void AddProfileL(CSIPProfile& aProfile);

		/**
		* Update profile
		* @param aProfile to be updated
		*/
		void UpdateProfileL(CSIPProfile& aProfile);

		/**
		* Remove profile
		* @pre UsageL(aProfile) == 0
		* @param aProfile to be removed
		*/
		void RemoveProfileL(CSIPProfile& aProfile);

		/**
		* Informs destruction of local profile object instance
		* @param aProfile instance of deleted profile
		*/
		void ProfileDeleted(CSIPProfile& aProfile);

		/**
		* Checks whether profile is default or not
		* @param aProfile checked profile
		* @return ETrue if is default, otherwise EFalse
		*/
		TBool IsDefaultProfileL(const CSIPProfile& aProfile);

		/**
		* Updates SIP profile value for negotiated security mechanism; leaves on failure
		* @param aProfile a SIP profile
		*/
		void NegotiatedSecurityMechanismL(CSIPConcreteProfile& aProfile);

		/**
		* Updates SIP profile value for registered AORs; leaves on failure
		* @param aProfile a SIP profile
		*/
		void RegisteredAORsL(CSIPConcreteProfile& aProfile);

	    /**
        * Gets new instance of either type CSIPProfile or CSIPManagedProfile
		* @return new instance
        */		
		virtual CSIPProfile* NewInstanceL() = 0;

		/**
		* Roll back in case of failure
		* @param aItem object to be cleaned
		*/
		static void CrashRevert (TAny* aItem);

		/**
		* Roll back in case of failure
		* @param aItem object to be cleaned
		*/
		static void LocalCrashRevert (TAny* aItem);

    public: // From MSIPConcreteProfileObserver

		/**
		* SIP profile registration status changed event
		* @param aProfileId id of profile which registration 
		*        status has been changed.
		* @param aStatus new status of the profile
		* @param aStatusId id related to registration
		*/
		void RegistrationStatusChangedL(TUint32 aProfileId, 
									   TInt aStatus,
									   TUint32 aStatusId);

		/** 
		* SIP profile creation event.
		* @param aProfileId id of new SIP Profile
		*/
		void AddedL(TUint32 aProfileId);

		/**
		* SIP profile destruction event.
		* @param aProfileId id of profile which was destroyed
		*/	
		void RemovedL(TUint32 aProfileId);

		/** 
		* SIP profile information update event
		* @param aProfileId id of updated SIP Profile
		* @param aSize size of updated SIP Profile
		*/
		void UpdatedL(TUint32 aProfileId, TUint aSize);

		/**
		* An asynchronous error has occurred related to SIP profile
		* @param aProfileId the id of failed profile 
		* @param aStatus
		* @param aError a error code
		*/
		void ErrorOccurredL(TUint32 aProfileId,
							TInt aStatus,
							TInt aError);

	protected: 
											
		/**
		* Constructor
		* @param aObserver observer to send events to 
		*/
		CSIPProfileRegistryBase(MSIPProfileRegistryObserver& aObserver); 

		/**
		* Symbian 2nd phase base class constructor
		*/
		void BaseConstructL();

		/**
		* Store profile into client cache
		* @param aItem a cleanup item for roll back in case of failure
		*/
		void StoreProfileL(CSIPProfileCleanupItem* aItem);

		/**
		* Store array of profiles into client cache
		* @param aProfiles a array containing the profiles
		* @param aRetProfiles returned array containing profiles
		*/
		void StoreArrayL(
			RPointerArray<CSIPConcreteProfile>& aProfiles,
			RPointerArray<CSIPProfile>& aRetProfiles);

	    /**
        * Gets profile cache item
		* @param aProfileId a profile id to be found
		* @return local cache item of the profile
        */		
		CSIPProfileItem* ProfileItemL(TUint aProfileId);

	    /**
        * Gets profile cache item index
		* @param aProfileId a profile id to be found
		* @return local cache item index of the profile
        */		
		TInt ProfileIndex (TUint aProfileId);

	    /**
        * Gets profile cache item by index
		* @param aIndex a index of the profile to be found
		* @return local cache item of the profile
        */		
		CSIPProfileItem* ProfileItemByIndex(TUint aIndex);

		/**
		* Gets SIP profile identified by the Id; leaves on failure
		* The owneship of profile is transferred.
		* @param aId a SIP profile Id
		* @param aSize of profile if known
		* @return SIP profile or 0 pointer if there's no such SIP profile
		*/
		CSIPProfile* SIPProfileL(TUint32 aId, TInt aSize = 0);

	protected:

		RSIPProfile* iSipClient;
		CSIPProfileITCReceiver* iITCReceiver;
		CSIPProfileITC* iITC;
		MSIPProfileRegistryObserver& iObserver;
		//RPointerArray<CSIPProfileItem> iProfiles;
		RPointerArray<CSIPProfile> iProfiles;
		CSIPProfilePlugins* iProfilePlugins;
		CSIPProfileItem* iFindEntry;
		
		
		
	private:
		
		void ProfilesCommonL(RPointerArray<CSIPProfile>& aProfiles, 
								CSIPConcreteProfileHolder* aHolder);

	};

#endif