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




#include "mcesipconnection.h"
#include "sipprofile.h"

// -----------------------------------------------------------------------------
// CMceSipConnection::NewL
// -----------------------------------------------------------------------------
//
CMceSipConnection* CMceSipConnection::NewL( CSIPProfile& aProfile )
	{
	
    CMceSipConnection* self = new( ELeave )CMceSipConnection( aProfile );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}

// -----------------------------------------------------------------------------
// CMceSipConnection::CMceSipConnection
// -----------------------------------------------------------------------------
//
CMceSipConnection::CMceSipConnection ( CSIPProfile& aProfile )
    : iProfile( aProfile )
	{


	}

// -----------------------------------------------------------------------------
// CMceSipConnection::ConstructL
// -----------------------------------------------------------------------------
//
void CMceSipConnection::ConstructL ()
	{
	}

// -----------------------------------------------------------------------------
// CMceSipConnection::CMceSipConnection
// -----------------------------------------------------------------------------
//
CMceSipConnection::~CMceSipConnection()
	{
	}

// -----------------------------------------------------------------------------
// CMceSipEvent::CurrentState
// -----------------------------------------------------------------------------
//
CSIPProfile& CMceSipConnection::Profile() const
    {
    return iProfile;
    }

