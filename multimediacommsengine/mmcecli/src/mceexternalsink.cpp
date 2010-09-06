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



#include "mceexternalsink.h"
#include "mcecomexternalsink.h"
#include "mcesession.h"
#include "mceclilogs.h"
#include "mcemanager.h"

#define _FLAT_DATA static_cast<CMceComExternalSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceExternalSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceExternalSink* CMceExternalSink::NewL()
    {
    
    CMceExternalSink* self = CMceExternalSink::NewLC();
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceExternalSinke::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceExternalSink* CMceExternalSink::NewLC()
    {
    
    CMceExternalSink* self = new (ELeave) CMceExternalSink();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    
    }
	

// -----------------------------------------------------------------------------
// CMceExternalSink::CMceExternalSink
// -----------------------------------------------------------------------------
//
CMceExternalSink::CMceExternalSink()
	: CMceMediaSink()
    {    
    
    }
	
	
// -----------------------------------------------------------------------------
// CMceExternalSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceExternalSink::ConstructL()
    {    
	CMceComExternalSink *sink = CMceComExternalSink::NewLC();
	CMceMediaSink::ConstructL( sink );
	CleanupStack::Pop(sink);
    }

    
// -----------------------------------------------------------------------------
// CMceExternalSink::~CMceExternalSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceExternalSink::~CMceExternalSink()
    {
    }

// -----------------------------------------------------------------------------
// CMceMicSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceExternalSink::EnableL()
    {
    MCECLI_DEBUG("CMceExternalSink::EnableL, Entry");
    
    //CMceMediaSource::DoEnableL(); // Not required for Signalling purpose    
    
    MCECLI_DEBUG("CMceExternalSink::EnableL, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceExternalSink::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceExternalSink::DisableL()
    {
    MCECLI_DEBUG("CMceExternalSink::DisableL, Entry");
    
    //CMceMediaSource::DoDisableL();  // Not required for Signalling purpose

    MCECLI_DEBUG("CMceExternalSink::DisableL, Exit");
    
    }
