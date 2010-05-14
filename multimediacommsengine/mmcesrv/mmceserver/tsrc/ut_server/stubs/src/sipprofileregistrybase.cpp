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
* Description:    implementation
*
*/




//  INCLUDE FILES

#include "sip.h"
#include "sipprofileregistrybase.h"
#include "sipprofileregistryobserver.h"
#include "sipprofileregistry.h"
#include "sipprofileregistryobserver.h"
#include "sipmanagedprofile.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::~CSIPProfileRegistryBase
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfileRegistryBase::~CSIPProfileRegistryBase()
	{
	
		
	iProfiles.ResetAndDestroy();
	iProfiles.Close();
	
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfilesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPProfileRegistryBase::ProfilesL(
			 RPointerArray<CSIPProfile>& aProfiles)
	{
	// stubs
	CSIPProfileRegistry* reg = (CSIPProfileRegistry*) this;
	CSIPProfile* profile = CSIPProfile::NewLC(reg);
	CSIPProfile* profile2 = CSIPProfile::NewLC(reg);
	profile2->SetContextId(2);
	
	aProfiles.AppendL ( profile  );
	aProfiles.AppendL ( profile2  );
	
	CleanupStack::Pop( profile2 );
	CleanupStack::Pop( profile );

	
	}
	
// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfilesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPProfileRegistryBase::ProfilesL(
			 const TSIPProfileTypeInfo& /*aType*/,
			 RPointerArray<CSIPProfile>& aProfiles)
	{
		// stubs
	CSIPProfileRegistry* reg = (CSIPProfileRegistry*) this;
	
	CSIPProfile* profile = CSIPProfile::NewLC( reg );
	CSIPProfile* profile2 = CSIPProfile::NewLC(reg);
	profile2->SetContextId(2);
	aProfiles.AppendL ( profile  );
	aProfiles.AppendL ( profile2  );

	CleanupStack::Pop( profile2 );
	CleanupStack::Pop( profile );
	
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfilesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPProfileRegistryBase::ProfilesL(
			 const TDesC8& /*aAOR*/,
			 RPointerArray<CSIPProfile>& aProfiles)
	{
	// stubs
	ProfilesL( aProfiles );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfilesCommonL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::ProfilesCommonL(
			 				RPointerArray<CSIPProfile>& /*aProfiles*/,
			 				CSIPConcreteProfileHolder* /*aHolder */)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfileL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfile* CSIPProfileRegistryBase::ProfileL(TUint32 aId)
	{
	return SIPProfileL(aId);
	}

	
//
// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::DefaultProfileL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfile* CSIPProfileRegistryBase::DefaultProfileL()
	{
	User::Leave( KErrNotSupported );
	return NULL;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::SupportedProfileTypesL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPProfileRegistryBase::SupportedProfileTypesL(
					RArray<TSIPProfileTypeInfo>& /*aSupportedProfileTypes*/ )
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::LastRegistrationError
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSIPProfileRegistryBase::LastRegistrationError(
			 const CSIPProfile& /*aProfile*/) const
	{
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::CSIPProfileRegistryBase
// -----------------------------------------------------------------------------
//
CSIPProfileRegistryBase::CSIPProfileRegistryBase(
	MSIPProfileRegistryObserver& aObserver):
	iObserver(aObserver)
	{

	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfileDeleted
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::ProfileDeleted(CSIPProfile& /*aProfile*/)
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::EnableProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::EnableProfileL(
	CSIPProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::DisableProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::DisableProfileL(CSIPProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::UsageL
// -----------------------------------------------------------------------------
//
TInt CSIPProfileRegistryBase::UsageL(const CSIPProfile& /*aProfile*/) const
	{
	User::Leave( KErrNotSupported );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::AddProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::AddProfileL(CSIPProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::UpdateProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::UpdateProfileL(CSIPProfile& /*aProfile*/)
	{	
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::RemoveProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::RemoveProfileL(CSIPProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::IsDefaultProfileL
// -----------------------------------------------------------------------------
//
TBool CSIPProfileRegistryBase::IsDefaultProfileL(const CSIPProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::RegistrationStatusChangedL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::NegotiatedSecurityMechanismL(
	CSIPConcreteProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::RegisteredAORsL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::RegisteredAORsL(CSIPConcreteProfile& /*aProfile*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::SIPProfileL
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfileRegistryBase::SIPProfileL(TUint32 aId, TInt /*aSize*/)
	{
	__ASSERT_ALWAYS (aId > 1, User::Leave (KErrArgument));
	return NULL;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::StoreProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::StoreProfileL(CSIPProfileCleanupItem* /*aCleanupItem*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::StoreProfileL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::StoreArrayL(
	RPointerArray<CSIPConcreteProfile>& /*aProfiles*/,
	RPointerArray<CSIPProfile>& /*aRetProfiles*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfileItemL
// -----------------------------------------------------------------------------
//
CSIPProfileItem* CSIPProfileRegistryBase::ProfileItemL(TUint /*aProfileId*/ )
	{
	User::Leave( KErrNotSupported );
	return NULL;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfileIndex
// -----------------------------------------------------------------------------
//
TInt CSIPProfileRegistryBase::ProfileIndex (TUint /*profileId*/)
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ProfileItemByIndex
// -----------------------------------------------------------------------------
//
CSIPProfileItem* CSIPProfileRegistryBase::ProfileItemByIndex(TUint /*aIndex*/)
	{
	return NULL;
	}


// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::RegistrationStatusChangedL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::RegistrationStatusChangedL(TUint32 /*aProfileId*/,
							  TInt /*aStatus*/,
							  TUint32 /*aStatusId*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::UpdatedL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::UpdatedL(TUint32 /*aProfileId*/, TUint /*aSize*/)
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::AddedL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::AddedL(TUint32 /*aProfileId*/) 
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::RemovedL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::RemovedL(TUint32 /*aProfileId*/)
	{
	User::Leave( KErrNotSupported );
	}	

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::ErrorOccurredL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::ErrorOccurredL(TUint32 /*aProfileId*/,
										   TInt /*aStatus*/,
										   TInt /*aError*/ )
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::BaseConstructL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::BaseConstructL()
	{

	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::CrashRevert
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::CrashRevert (TAny* /*aItem*/)
	{
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistryBase::LocalCrashRevert
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistryBase::LocalCrashRevert (TAny* /*aItem*/)
	{
	}

