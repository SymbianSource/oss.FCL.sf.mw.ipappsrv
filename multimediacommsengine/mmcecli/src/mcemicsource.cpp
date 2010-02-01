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




#include "mcemicsource.h"
#include "mcecommicsource.h"
#include "mcesession.h"
#include "mcemediastream.h"
#include "mceitcsender.h"
#include "mceclilogs.h"
#include "mcemanager.h"

#define _FLAT_DATA static_cast<CMceComMicSource*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMicSource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMicSource* CMceMicSource::NewL()
    {
    
    CMceMicSource* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    
    }

// -----------------------------------------------------------------------------
// CMceMicSource::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMicSource* CMceMicSource::NewLC()
    {
    
    CMceMicSource* self = new (ELeave) CMceMicSource();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    
    }
    
// -----------------------------------------------------------------------------
// CMceMicSource::~CMceMicSource
// -----------------------------------------------------------------------------
//
EXPORT_C CMceMicSource::~CMceMicSource()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMicSource::EnableL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMicSource::EnableL()
    {
    MCECLI_DEBUG("CMceMicSource::EnableL, Entry");
    
    CMceMediaSource::DoEnableL();
    
    MCECLI_DEBUG("CMceMicSource::EnableL, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceMicSource::Disable
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceMicSource::DisableL()
    {
    MCECLI_DEBUG("CMceMicSource::DisableL, Entry");
    
    CMceMediaSource::DoDisableL();

    MCECLI_DEBUG("CMceMicSource::DisableL, Exit");
    
    }
    
// -----------------------------------------------------------------------------
// CMceMicSource::SetGainL
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMceMicSource::SetGainL( TInt aGain )
	{
    MCECLI_DEBUG("CMceMicSource::SetGainL, Entry");
    MCECLI_DEBUG_DVALUE("gain", aGain);
    
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	CMceSession* session = iStream->Session();

    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID   = iStream->Id();
    	ids.iSourceID  = Id();
    	
    	TMceItcArgTInt gain( aGain );
        session->ITCSender().WriteL( ids, EMceItcSetGain, gain );
        }
        
    FLAT_DATA( iGain ) = aGain; 
    
    MCECLI_DEBUG("CMceMicSource::SetGainL, Exit");
    
	}
		
// -----------------------------------------------------------------------------
// CMceMicSource::GainL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMceMicSource::GainL() const
	{
	MCECLI_DEBUG("CMceMicSource::GainL, Entry");
	
    TInt gain( 0 );
	
    if ( MCE_ENDPOINT_ITC_ALLOWED( *this ) )
        {
    	CMceSession* session = iStream->Session();

    	TMceIds ids;
    	session->PrepareForITC( ids );
    	ids.iMediaID  = iStream->Id();
    	ids.iSourceID = Id();

    	TMceItcArgTInt gainArg;

        session->ITCSender().ReadL( ids, EMceItcGain, gainArg );
        
        gain = gainArg();
        }
    else
        {
        gain = FLAT_DATA( iGain );
        }

    MCECLI_DEBUG("CMceMicSource::GainL, Exit");
    
	return gain;
	}

// -----------------------------------------------------------------------------
// CMceMicSource::MaxGainL
// -----------------------------------------------------------------------------
//		  
EXPORT_C TInt CMceMicSource::MaxGainL() const
	{
	MCECLI_DEBUG("CMceMicSource::MaxGainL");
	
	TMceItcArgTInt maxGain;
    
    if ( iStream && iStream->Session() && &iStream->Session()->Manager() )
        {
        CMceManager& manager = iStream->Session()->Manager();
        TMceIds ids;
        ids.iAppUID = manager.AppUid().iUid;
        _FLAT_DATA->SenderL( 
            manager.ServerSession() ).ReadL( ids, EMceItcMaxGain, maxGain );
        }
    else
        {
        MCECLI_DEBUG("CMceMicSource::MaxGainL, not ready");
        User::Leave( KErrNotReady );
        }
    
    // If gain is not set, set it to half of the max gain
    if ( FLAT_DATA( iGain ) == KMceNotAssignedInt )
        {
        const TInt KMceGainDivider = 2;
        FLAT_DATA( iGain ) = ( maxGain() / KMceGainDivider );
        }
        
	return maxGain();
	}
    
// -----------------------------------------------------------------------------
// CMceMicSource::CMceMicSource
// -----------------------------------------------------------------------------
//
CMceMicSource::CMceMicSource()
 : CMceMediaSource()
    {
    }
    
// -----------------------------------------------------------------------------
// CMceMicSource::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMicSource::ConstructL()
    {
    CMceComMicSource* mic = CMceComMicSource::NewLC();
    CMceMediaSource::ConstructL( mic );
    CleanupStack::Pop( mic );
    }
    

