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



#include "mcemessagesink.h"
#include "mcecommessagesink.h"
#include "mcesession.h"
#include "mceclilogs.h"
#include "mcemediastream.h"
#include "mcemanager.h"
#include "mceclilogs.h"
#include "mcedatareceiver.h"
#include "mcedatasinkobserver.h"

#define _FLAT_DATA static_cast<CMceComMessageSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMessageSink::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageSink* CMceMessageSink::NewL( CMceManager& aManager )
    {
    MCECLI_DEBUG("CMceMessageSink::NewL, Entry");
    CMceMessageSink* self = CMceMessageSink::NewLC( aManager );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceMessageSinke::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageSink* CMceMessageSink::NewLC( CMceManager& aManager )
    {
    MCECLI_DEBUG("CMceMessageSink::NewLC, Entry");
    CMceMessageSink* self = new (ELeave) CMceMessageSink();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager );
    return self;
    }

    
// -----------------------------------------------------------------------------
// CMceMessageSink::~CMceMessageSink
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageSink::~CMceMessageSink()
    {
    MCECLI_DEBUG("CMceMessageSink::~CMceMessageSink, Entry");
    delete iReceiver;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageSink::EnableL()
    {
    MCECLI_DEBUG("CMceMessageSink::EnableL, Entry");
    
    iReceiver = CMceDataReceiver::NewL( *this );    
    CMceMediaSink::DoEnableL();

    MCECLI_DEBUG("CMceMessageSink::EnableL, Exit");
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageSink::DisableL()
    {
    MCECLI_DEBUG("CMceMessageSink::DisableL, Entry");
    
    delete iReceiver;
    iReceiver = 0;
    CMceMediaSink::DoDisableL();

    MCECLI_DEBUG("CMceMessageSink::DisableL, Exit");
    
    }
	    
	    
        
// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceMessageSink* CMceMessageSink::NewL()
    {
    MCECLI_DEBUG("CMceMessageSink::NewL, Entry");
    CMceMessageSink* self = CMceMessageSink::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceMessageSink* CMceMessageSink::NewLC()
    {
    MCECLI_DEBUG("CMceMessageSink::NewLC, Entry");
    CMceMessageSink* self = new (ELeave) CMceMessageSink();
    CleanupStack::PushL( self );
    self->ConstructL( NULL );
    return self;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMceMessageSink::DataReceived( CMceMediaStream& /*aStream*/, CMceMediaSink& /*aSink*/,
    const TDesC8& aData )
    {
    MCECLI_DEBUG("CMceMessageSink::DataReceived, Entry");
    MMceDataSinkObserver* observer =
        iStream->Session()->Manager().DataSinkObserver();
    
    observer->DataReceived( *iStream, *this, aData );
    }

// -----------------------------------------------------------------------------
// CMceMessageSink::CMceMessageSink
// -----------------------------------------------------------------------------
//
CMceMessageSink::CMceMessageSink()
	: CMceMediaSink()
    { 
    }
	
	
// -----------------------------------------------------------------------------
// CMceMessageSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMessageSink::ConstructL( CMceManager* /*aManager*/ )
    {    
    MCECLI_DEBUG("CMceMessageSink::ConstructL, Entry");
	CMceComMessageSink *sink = CMceComMessageSink::NewLC();
	CMceMediaSink::ConstructL( sink );
	CleanupStack::Pop(sink);
    
	//iReceiver = CMceDataReceiver::NewL( *this, iIsexits );
    
    }

