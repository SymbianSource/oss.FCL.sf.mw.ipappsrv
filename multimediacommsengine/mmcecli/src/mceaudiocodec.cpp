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




#include "mceaudiocodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"
#include "mcecomaudiocodec.h"
#include "mceclientserver.h"
#include "mcefactory.h"
#include "mceevents.h"


#define _FLAT_DATA static_cast<CMceComAudioCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceAudioCodec::InitializeL
// -----------------------------------------------------------------------------
//
void CMceAudioCodec::InitializeL( CMceMediaStream& aParent )
    {
    CMceCodec::InitializeL( aParent );
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::~CMceAudioCodec
// -----------------------------------------------------------------------------
//
CMceAudioCodec::~CMceAudioCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::SamplingFreq
// -----------------------------------------------------------------------------
//
TUint CMceAudioCodec::SamplingFreq() const
    {
    return FLAT_DATA( iSamplingFreq );
    
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::VAD
// -----------------------------------------------------------------------------
//
TBool CMceAudioCodec::VAD() const
    {
    return FLAT_DATA( iEnableVAD );
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::PTime
// -----------------------------------------------------------------------------
//
TUint CMceAudioCodec::PTime() const        
    {
    return FLAT_DATA( iPTime );
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::MaxPTime
// -----------------------------------------------------------------------------
//
TUint CMceAudioCodec::MaxPTime() const        
    {
    return FLAT_DATA( iMaxPTime );
    }
            
// -----------------------------------------------------------------------------
// CMceAudioCodec::CMceAudioCodec
// -----------------------------------------------------------------------------
//
CMceAudioCodec::CMceAudioCodec() :
    CMceCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceAudioCodec::ConstructL( CMceComAudioCodec* aFlatData )
    {
    CMceCodec::ConstructL( aFlatData );
    }
            

// -----------------------------------------------------------------------------
// CMceAudioCodec::Factory
// -----------------------------------------------------------------------------
//
TMceAudioCodecFactory CMceAudioCodec::Factory()
    {
    return TMceAudioCodecFactory();
    }

// -----------------------------------------------------------------------------
// CMceAudioCodec::ComparePreferences
// -----------------------------------------------------------------------------
//  
TInt CMceAudioCodec::ComparePreferences( const CMceAudioCodec& aIndex1, 
                                         const CMceAudioCodec& aIndex2 )
    {
    if ( aIndex1.Preference() > aIndex2.Preference() )
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

