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
#include "sipconnection.h"
#include "sipprofileregistry.h"
#include "sipprofileregistryobserver.h"
#include "sipprofile.h"
//#include "sipconcreteprofile.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfileRegistry* CSIPProfileRegistry::NewL(
	CSIP& aSip,
	MSIPProfileRegistryObserver& aObserver)
	{
    CSIPProfileRegistry* self = CSIPProfileRegistry::NewLC(aSip, aObserver);
    CleanupStack::Pop(self);
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfileRegistry* CSIPProfileRegistry::NewLC(
	CSIP& aSip,
	MSIPProfileRegistryObserver& aObserver)
	{
	CSIPProfileRegistry* self = new (ELeave) CSIPProfileRegistry(aSip, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::~CSIPProfileRegistry
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPProfileRegistry::~CSIPProfileRegistry()
	{
	
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CSIPProfileRegistry::EnableL(CSIPProfile& /*aSIPProfile*/,
						MSIPConnectionObserver& /*&aObserver*/)
	{
	//__ASSERT_ALWAYS (!aSIPProfile.IsEnabled(), User::Leave (KErrArgument));
	//aSIPProfile.ConcreteProfile().SetLastRegistrationError(KErrNone);
	//iConnectionObserver = &aObserver;

	//EnableProfileL(aSIPProfile);
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSIPProfileRegistry::Disable(CSIPProfile& /*aProfile*/)
	{
	TInt err = KErrNone;
	//aProfile.ConcreteProfile().SetLastRegistrationError(KErrNone);
	//TRAPD(err, DisableProfileL(aProfile));
	return err;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::SIP
// -----------------------------------------------------------------------------
//
EXPORT_C CSIP& CSIPProfileRegistry::SIP() const
	{
	return iSip;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::ConnectionL
// -----------------------------------------------------------------------------
//
EXPORT_C CSIPConnection* CSIPProfileRegistry::ConnectionL(
	CSIPProfile& aSIPProfile)
	{
	TUint32 apId( 0 );
	aSIPProfile.GetParameter(KSIPAccessPointId, apId);
	CSIPConnection* connection = CSIPConnection::NewL(iSip, apId, *iConnectionObserver);
	return connection;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::IsEnabled
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSIPProfileRegistry::IsEnabled(const CSIPProfile& /*aSIPProfile*/) const
	{
	//return aSIPProfile.IsEnabled();
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::CSIPProfileRegistry
// -----------------------------------------------------------------------------
//
CSIPProfileRegistry::CSIPProfileRegistry(
	CSIP& aSip,
	MSIPProfileRegistryObserver& aObserver):
	CSIPProfileRegistryBase(aObserver), iSip(aSip)
	{

	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::ConstructL
// -----------------------------------------------------------------------------
//
void CSIPProfileRegistry::ConstructL()
	{
	BaseConstructL();

	}

// -----------------------------------------------------------------------------
// CSIPProfileRegistry::NewInstanceL
// -----------------------------------------------------------------------------
//
CSIPProfile* CSIPProfileRegistry::NewInstanceL()
	{
    return CSIPProfile::NewL(this);
	}

