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



#include "fcplugininfo.h"

// -----------------------------------------------------------------------------
// CFCPlugInInfo::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFCPlugInInfo* CFCPlugInInfo::NewL( const TUid& aUid, const TDesC8& aName )
	{
	CFCPlugInInfo* self = new (ELeave) CFCPlugInInfo( aUid );
	CleanupStack::PushL(self);
	self->ConstructL( aName );
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CFCPlugInInfo::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CFCPlugInInfo::ConstructL( const TDesC8& aName )
	{
	iName = aName.AllocL();
	}
	
// -----------------------------------------------------------------------------
// CFCPlugInInfo::~CFCPlugInInfo
// destructor.
// -----------------------------------------------------------------------------
//
CFCPlugInInfo::~CFCPlugInInfo()
	{
	delete iName;
	}

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
CFCPlugInInfo::CFCPlugInInfo( const TUid& aUid ):
	iUid( aUid )
    {
    }

// ----------------------------------------------------------------------------
// CFCPlugInInfo::Name
// ----------------------------------------------------------------------------
//
const TDesC8& CFCPlugInInfo::Name() const
    {
    return *iName;
    }
    
// ----------------------------------------------------------------------------
// CFCPlugInInfo::Uid
// ----------------------------------------------------------------------------
//
const TUid& CFCPlugInInfo::Uid() const
    {
    return iUid;
    }

//  End of File
