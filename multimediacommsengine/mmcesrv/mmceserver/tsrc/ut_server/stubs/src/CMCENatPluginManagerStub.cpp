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
* Description:    
*
*/



#include <e32debug.h>
#include "../inc/mcenatpluginmanager.h"
#include "nspplugin.h"


// ================ MEMBER FUNCTIONS =======================



// ---------------------------------------------------------
// CMceNatPluginManager::CMceNatPluginManager
// C++ default constructor can NOT contain any code, that
// might leave.
// Status : Draft
// ---------------------------------------------------------
CMceNatPluginManager::CMceNatPluginManager()
    {
    }

// ---------------------------------------------------------
// CMceNatPluginManager::ConstructL
// Symbian 2nd phase constructor can leave.
// Status : Draft
// ---------------------------------------------------------
void CMceNatPluginManager::ConstructL()
    {  
    iReservStatus = CNSPPlugin::ENSPResourcesNotReserved; //Resource Not Reserved: Initialied
    }


// ---------------------------------------------------------
// CMceNatPluginManager::NewL
// Static constructor.
// Status : Draft
// ---------------------------------------------------------
CMceNatPluginManager* CMceNatPluginManager::NewL()
    {
    CMceNatPluginManager* self = new ( ELeave ) CMceNatPluginManager;
    return self;
    }

// ---------------------------------------------------------
// CMceNatPluginManager::~CMceNatPluginManager
// Destructor
// Status : Draft
// ---------------------------------------------------------    
CMceNatPluginManager::~CMceNatPluginManager()
    {
    // NOP
    }   

// -----------------------------------------------------------------------------
// CMceNatPluginManager::GetPluginL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::GetPluginL()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// CMceNatPluginManager::NewSessionL
// -----------------------------------------------------------------------------
//
TInt CMceNatPluginManager::NewSessionL( MNSPSessionObserver& aNatSessionObserver,
                                        TUint32 aIapId,
                                        const TDesC8& aDomain )
    {
    User::LeaveIfNull( &aNatSessionObserver );
    User::LeaveIfError( 0 != aIapId ? KErrNone : KErrArgument );
    User::LeaveIfError( aDomain.Length() ? KErrNone : KErrArgument );
    
    return 3;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::CloseSessionL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::CloseSessionL( TUint aSessionId )
    {
    User::LeaveIfError( 0 != aSessionId ? KErrNone : KErrArgument );
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::CreateOfferL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::CreateOfferL( TUint aSessionId, 
                                                     CSdpDocument*& aOffer )
    {
    User::LeaveIfError( 0 != aSessionId ? KErrNone : KErrArgument );
    User::LeaveIfNull( aOffer );
    return iReturnStatus;
    }


// -----------------------------------------------------------------------------
// CMceNatPluginManager::ResolveL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::ResolveL( TUint aSessionId,
                                                 CSdpDocument*& aOffer,
                                                 CSdpDocument*& aAnswer )
    {
    User::LeaveIfError( 0 != aSessionId ? KErrNone : KErrArgument );
    User::LeaveIfNull( aOffer );
    User::LeaveIfNull( aAnswer );
    return iReturnStatus;
    }


// -----------------------------------------------------------------------------
// CMceNatPluginManager::DecodeAnswerL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::DecodeAnswerL( TUint aSessionId,
                                                      CSdpDocument*& aAnswer )
    {
    User::LeaveIfError( 0 != aSessionId ? KErrNone : KErrArgument );
    User::LeaveIfNull( aAnswer );
    return iReturnStatus;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::UpdateL
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::UpdateL( TUint aSessionId, CSdpDocument*& aOffer )
    {
    User::LeaveIfError( 0 != aSessionId ? KErrNone : KErrArgument );
    User::LeaveIfNull( aOffer );
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::RestartL
// -----------------------------------------------------------------------------
//
TNatReturnStatus CMceNatPluginManager::RestartL( TUint aSessionId )
    {
    User::LeaveIfError( 0 != aSessionId ? KErrNone : KErrArgument );
    return iReturnStatus;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::NatEnabledL
// -----------------------------------------------------------------------------
//
TBool CMceNatPluginManager::NatEnabledL()
    {
    return iEnabled;
    }

// -----------------------------------------------------------------------------
// CMceNatPluginManager::SetSessionParam
// -----------------------------------------------------------------------------
//
TInt CMceNatPluginManager::SetSessionParam( TUint /*aSessionId*/,
        CNSPPlugin::TNSPSessionParamKey /*aParamKey*/, TUint /*aParamValue*/ )
	{
	return KErrNone;
   	}

// -----------------------------------------------------------------------------
// CMceNatPluginManager::GetSessionParam 
// -----------------------------------------------------------------------------
//

TInt CMceNatPluginManager::GetSessionParam( TUint /*aSessionId*/, 
                      		CNSPPlugin::TNSPSessionParamKey aParamKey )
                      
	{
	
	__ASSERT_ALWAYS( aParamKey == CNSPPlugin::ENSPResourseReservationStatusKey, User::Leave( KErrNotFound ) );
		
	return iReservStatus;
	}

// -----------------------------------------------------------------------------
// CMceNatPluginManager::WriteSdpToLog
// -----------------------------------------------------------------------------
//
void CMceNatPluginManager::WriteSdpToLog( 
    const TDesC& /*aMsg*/, CSdpDocument* /*aSdp*/ )
    {
    }

// end of file
