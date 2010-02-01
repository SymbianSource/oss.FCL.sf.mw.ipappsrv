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




#include "mcevideocodec.h"
#include "mcecomvideocodec.h"
#include "mcefactory.h"
#include "mceevents.h"


#define _FLAT_DATA static_cast<CMceComVideoCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceVideoCodec::~CMceVideoCodec
// -----------------------------------------------------------------------------
//
CMceVideoCodec::~CMceVideoCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceVideoCodec::AllowedFrameRates
// -----------------------------------------------------------------------------
//	    
EXPORT_C TUint CMceVideoCodec::AllowedFrameRates() const
    {
    return FLAT_DATA( iAllowedFrameRates );
    }
    
// -----------------------------------------------------------------------------
// CMceVideoCodec::FrameRate
// -----------------------------------------------------------------------------
//	    
EXPORT_C TReal CMceVideoCodec::FrameRate() const
    {
    return FLAT_DATA( iFrameRate );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	   
EXPORT_C TUint CMceVideoCodec::MaxBitRate() const
    {
    return FLAT_DATA( iMaxBitRate );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//		
EXPORT_C TUint CMceVideoCodec::AllowedResolutions() const
    {
    return FLAT_DATA( iAllowedResolutions );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	    
EXPORT_C TSize CMceVideoCodec::Resolution() const
    {
    return TSize( FLAT_DATA( iResolutionWidth ), 
                  FLAT_DATA( iResolutionHeight ) );
    }
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
EXPORT_C HBufC8* CMceVideoCodec::ConfigKeyL() const
    {
    HBufC8* configKey = NULL;
    if ( FLAT_DATA( iConfigKey ) )
        {
        configKey = FLAT_DATA( iConfigKey )->AllocL();
        }
    return configKey;
    
    } 
    
// -----------------------------------------------------------------------------
// CMceVideoCodec::Factory
// -----------------------------------------------------------------------------
//
TMceVideoCodecFactory CMceVideoCodec::Factory()
    {
    return TMceVideoCodecFactory();
    }
 
// -----------------------------------------------------------------------------
// CMceVideoCodec::ComparePreferences
// -----------------------------------------------------------------------------
//  
TInt CMceVideoCodec::ComparePreferences( const CMceVideoCodec& aIndex1, 
                                         const CMceVideoCodec& aIndex2 )
    {
    if ( aIndex1.Preference() >= aIndex2.Preference() )
        {
        return (1);
        }
    else if ( aIndex1.Preference() < aIndex2.Preference() )
        {
        return (-1);
        }
    else
        {
        return (0);
        }
    }
                                                 
// -----------------------------------------------------------------------------
// CMceVideoCodec::CMceVideoCodec
// -----------------------------------------------------------------------------
//
CMceVideoCodec::CMceVideoCodec()
    {
    iType = KMceVideoCodec;
    }

// -----------------------------------------------------------------------------
// CMceVideoCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceVideoCodec::ConstructL( CMceComVideoCodec* aFlatData )
    {
    CMceCodec::ConstructL( aFlatData );
    }
            
