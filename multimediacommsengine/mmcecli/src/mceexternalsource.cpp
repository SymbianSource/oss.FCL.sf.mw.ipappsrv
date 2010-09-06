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



#include "mceexternalsource.h"
#include "mcecomexternalsource.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceitcsender.h"
#include "mceclilogs.h"
#include "mcemanager.h"

#define _FLAT_DATA static_cast<CMceComExternalSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// Cmceexternalsource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceExternalSource* CMceExternalSource::NewL()
    {    
    CMceExternalSource* self = CMceExternalSource::NewLC();
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceExternalSource::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceExternalSource* CMceExternalSource::NewLC()
    {    
    CMceExternalSource* self = new (ELeave) CMceExternalSource();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    
    }
	
	
// -----------------------------------------------------------------------------
// CMceExternalSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceExternalSource::ConstructL()
    {    
	CMceComExternalSource *source = CMceComExternalSource::NewLC();
	CMceMediaSource::ConstructL( source );
	CleanupStack::Pop(source);
    }
    
// -----------------------------------------------------------------------------
// CMceExternalSource::~CMceExternalSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceExternalSource::~CMceExternalSource()
    {
    }
	
	
// -----------------------------------------------------------------------------
// CMceExternalSource::~CMceExternalSource
// -----------------------------------------------------------------------------
//
CMceExternalSource::CMceExternalSource()
	:CMceMediaSource()
    {
    }

// -----------------------------------------------------------------------------
// CMceMicSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceExternalSource::EnableL()
    {
    MCECLI_DEBUG("CMceExternalSource::EnableL, Entry");
    
    //CMceMediaSource::DoEnableL(); // Not required for Signalling purpose    
    
    MCECLI_DEBUG("CMceExternalSource::EnableL, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceExternalSource::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceExternalSource::DisableL()
    {
    MCECLI_DEBUG("CMceExternalSource::DisableL, Entry");
    
    //CMceMediaSource::DoDisableL();  // Not required for Signalling purpose

    MCECLI_DEBUG("CMceExternalSource::DisableL, Exit");
    
    }
