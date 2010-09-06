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



#include "mcemessagesource.h"
#include "mcecommessagesource.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceitcsender.h"
#include "mceclilogs.h"
#include "mcemanager.h"
#include "mce.h"
#include "mceserial.h"
#include "mcedatasender.h"


#define _FLAT_DATA static_cast<CMceComMessageSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CmceMessagesource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageSource* CMceMessageSource::NewL( CMceManager& aManager )
    {  
    MCECLI_DEBUG("CMceMessageSource::NewL(aManager), Entry");
    CMceMessageSource* self = CMceMessageSource::NewLC(aManager);
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceMessageSource::NewL(aManager), Exit");
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceMessageSource::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageSource* CMceMessageSource::NewLC( CMceManager& aManager )
    {    
    MCECLI_DEBUG("CMceMessageSource::NewLC(aManager), Entry");
    CMceMessageSource* self = new (ELeave) CMceMessageSource();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager);
    MCECLI_DEBUG("CMceMessageSource::NewLC(aManager), Exit");
    return self;
    }
	
// -----------------------------------------------------------------------------
// CMceMessageSource::~CMceMessageSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMessageSource::~CMceMessageSource()
    {
    MCECLI_DEBUG("CMceMessageSource::~CMceMessageSource");
    delete iDataSender;
    }


// -----------------------------------------------------------------------------
// CMceMicSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageSource::EnableL()
    {
    MCECLI_DEBUG("CMceMessageSource::EnableL, Entry");
    
    iDataSender = CMceDataSender::NewL();
    CMceMediaSource::DoEnableL(); // Not required for Signalling purpose    

    MCECLI_DEBUG("CMceMessageSource::EnableL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceMessageSource::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMessageSource::DisableL()
    {
    MCECLI_DEBUG("CMceMessageSource::DisableL, Entry");
    
    delete iDataSender;
    iDataSender = 0;
    CMceMediaSource::DoDisableL();  // Not required for Signalling purpose
    
    MCECLI_DEBUG("CMceMessageSource::DisableL, Exit");
    }


// -----------------------------------------------------------------------------
// CMceMessageSource::SendDataL
// -----------------------------------------------------------------------------
//      
EXPORT_C void CMceMessageSource::SendDataL( const TDesC8& aData )
    {
    MCECLI_DEBUG("CMceMessageSource::SendDataL, Entry");
    __ASSERT_ALWAYS( iDataSender, User::Leave( KErrNotReady ) );
    
    iDataSender->Send( aData);
    MCECLI_DEBUG("CMceMessageSource::SendDataL, Exit");
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceMessageSource::InitializeL( CMceManager* aManager )
    {
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    
    }
// -----------------------------------------------------------------------------
// Factory method to inner usage
// -----------------------------------------------------------------------------
//
CMceMessageSource* CMceMessageSource::NewL()
    {
    MCECLI_DEBUG("CMceMessageSource::NewL, Entry");
    CMceMessageSource* self = CMceMessageSource::NewLC();
    CleanupStack::Pop( self );
    MCECLI_DEBUG("CMceMessageSource::NewL, Exit");
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method to inner usage
// -----------------------------------------------------------------------------
//
CMceMessageSource* CMceMessageSource::NewLC()
    {
    MCECLI_DEBUG("CMceMessageSource::NewLC, Entry");
    CMceMessageSource* self = new (ELeave) CMceMessageSource();
    CleanupStack::PushL( self );
    self->ConstructL();
    MCECLI_DEBUG("CMceMessageSource::NewLC, Entry");
    return self;
    }
	
	
// -----------------------------------------------------------------------------
// CMceMessageSource::~CMceMessageSource
// -----------------------------------------------------------------------------
//
CMceMessageSource::CMceMessageSource()
	:CMceMediaSource()
    {
    }


// -----------------------------------------------------------------------------
// CMceMessageSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMessageSource::ConstructL( CMceManager* /*aManager*/ )
    {    
    MCECLI_DEBUG("CMceMessageSource::ConstructL, Entry");
	CMceComMessageSource *source = CMceComMessageSource::NewLC();
	CMceMediaSource::ConstructL( source );
	CleanupStack::Pop(source);
	
	//iDataSender = CMceDataSender::NewL();
	MCECLI_DEBUG("CMceMessageSource::ConstructL, Exit");
    }

// -----------------------------------------------------------------------------
// CMceMessageSink::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMessageSource::ConstructL()
    {    
    MCECLI_DEBUG("CMceMessageSource::ConstructL, Entry");
    CMceComMessageSource *source = CMceComMessageSource::NewLC();
    CMceMediaSource::ConstructL( source );
    CleanupStack::Pop(source);
    
     //iDataSender = CMceDataSender::NewL();
     MCECLI_DEBUG("CMceMessageSource::ConstructL, Exit");
    }

