/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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





// INCLUDE FILES
#include "plugin_stub.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPluginStub::CPluginStub
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPluginStub::CPluginStub()
    {
    }

// -----------------------------------------------------------------------------
// CPluginStub::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPluginStub::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CPluginStub::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPluginStub* CPluginStub::NewL()
    {
    CPluginStub* self = new( ELeave ) CPluginStub;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CPluginStub::~CPluginStub
// 
// -----------------------------------------------------------------------------
//
CPluginStub::~CPluginStub()
    {
    }

TUint CPluginStub::NewSessionL(MNSPSessionObserver& /*aNatSessionObserver*/, 
		TUint32 /*aIapId*/, const TDesC8& /*aDomain*/, TUint /*aProtocol*/ )
	{
	return 1;
	}

void CPluginStub::CloseSessionL( TUint /*aSessionId*/ )
	{
	
	}

TNatReturnStatus CPluginStub::CreateOfferL( TUint /*aSessionId*/, CSdpDocument*& /*aOffer*/ )
	{
	return 1;
	}

TNatReturnStatus CPluginStub::ResolveL( TUint /*aSessionId*/, CSdpDocument*& /*aOffer*/, CSdpDocument*& /*aAnswer*/ )
	{
	return 1;
	}


TNatReturnStatus CPluginStub::DecodeAnswerL( TUint /*aSessionId*/, CSdpDocument*& /*aAnswer*/ )
	{
	return 1;
	}

void CPluginStub::UpdateL( TUint /*aSessionId*/, CSdpDocument*& /*aOffer*/ )
	{
	
	}
	
TNatReturnStatus CPluginStub::RestartL( TUint /*aSessionId*/ )
	{
	return 1;
	}





TInt CPluginStub::SetSessionParam( TUint /*aSessionId*/, 
		CNSPPlugin::TNSPSessionParamKey /*aParamKey*/, 
		TUint /*aParamValue*/ )
	{
	return KErrNone;
	}

TInt CPluginStub::GetSessionParam( TUint /*aSessionId*/, 
		CNSPPlugin::TNSPSessionParamKey /*aParamKey*/ )
	{
	return KErrNone;
	}




// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
