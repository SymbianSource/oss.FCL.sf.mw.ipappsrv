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





#include "mcefilesink.h"
#include "mcecomfilesink.h"
#include "mceitcsender.h"
#include "mce.h"
#include "mcesession.h"
#include "mceserial.h"
#include "utf.h"
#include "mceclilogs.h"


#define _FLAT_DATA static_cast<CMceComFileSink*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSink* CMceFileSink::NewL(
            const TFileName& aFileName )
    {
    CMceFileSink* self = CMceFileSink::NewLC( aFileName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSink* CMceFileSink::NewLC(
            const TFileName& aFileName )
    {
    CMceFileSink* self = new (ELeave) CMceFileSink();
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSink::~CMceFileSink()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSink::EnableL()
    {
    MCECLI_DEBUG("CMceFileSink::EnableL, Entry");
    
    CMceMediaSink::DoEnableL();
    
    MCECLI_DEBUG("CMceFileSink::EnableL, Exit");
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSink::DisableL()
    { 
    MCECLI_DEBUG("CMceFileSink::DisableL, Entry");
    
    CMceMediaSink::DoDisableL();
    
    MCECLI_DEBUG("CMceFileSink::DisableL, Exit");
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
CMceFileSink::CMceFileSink()
    {    
    }
    
    
// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceFileSink* CMceFileSink::NewL()
    {
    CMceFileSink* self = CMceFileSink::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceFileSink* CMceFileSink::NewLC()
    {
    CMceFileSink* self = new (ELeave) CMceFileSink();
    CleanupStack::PushL( self );
    self->ConstructL( KNullDesC() );
    return self;
    }    
 
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMceFileSink::ConstructL( const TFileName& aFileName )
    { 
    
    CMceComFileSink* sink = CMceComFileSink::NewLC();
    CMceMediaSink::ConstructL( sink );
    CleanupStack::Pop( sink );
    
    FLAT_DATA( iFileInfo.iFileName ) = aFileName;
    FLAT_DATA( iFileInfo.iDirection ) = TMceFileInfo::EWrite;
      
    }

// -----------------------------------------------------------------------------
// CMceFileSink::Initialize
// -----------------------------------------------------------------------------
//
void CMceFileSink::InitializeL( CMceManager* aManager )
    {
    __ASSERT_ALWAYS( aManager, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( iStream, User::Leave( KErrArgument ) );
    
    if ( !FLAT_DATA( iInitialInfoRetrieved ) )
        {
        
        MCECLI_DEBUG("CMceFileSource::InitializeL, querying file info");
        GetFileInfoL();
        
        FLAT_DATA( iInitialInfoRetrieved ) = ETrue;

        }
        
    }


// -----------------------------------------------------------------------------
// CMceFileSink::GetFileInfoL
// -----------------------------------------------------------------------------
//
void CMceFileSink::GetFileInfoL()
    {
    
    TMceIds ids;
    TMceItcArgTFileInfo fileInfoBuf( FLAT_DATA( iFileInfo ) );

    iStream->Session()->PrepareForITC( ids );
    ids.iSessionID = KMceNotAssigned;
    iStream->Session()->ITCSender().ReadL( ids, EMceItcFileInfo, fileInfoBuf );
    
    FLAT_DATA( iFileInfo ) = fileInfoBuf();

    User::LeaveIfError( FLAT_DATA( iFileInfo.iAccessRights ) == TMceFileInfo::EZero ?
                        KErrAccessDenied : KErrNone );
    
    }
