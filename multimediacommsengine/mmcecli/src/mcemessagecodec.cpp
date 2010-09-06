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



#include "mcemessagecodec.h"
#include "mcesession.h"
#include "mcemanager.h"
#include "mcemediastream.h"
#include "mcecommessagecodec.h"
#include "mceclientserver.h"
#include "mcefactory.h"
#include "mceevents.h"


#define _FLAT_DATA static_cast<CMceComMessageCodec*>( iFlatData )
#define FLAT_DATA( data ) _FLAT_DATA->data

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceMessageCodec::InitializeL
// -----------------------------------------------------------------------------
//
void CMceMessageCodec::InitializeL( CMceMediaStream& aParent )
    {
    CMceCodec::InitializeL( aParent );
    }

// -----------------------------------------------------------------------------
// CMceMessageCodec::~CMceMessageCodec
// -----------------------------------------------------------------------------
//
CMceMessageCodec::~CMceMessageCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceMessageCodec::SamplingFreq
// -----------------------------------------------------------------------------
//
TUint CMceMessageCodec::SamplingFreq() const
    {
    return FLAT_DATA( iSamplingFreq );
    
    }

// -----------------------------------------------------------------------------
// CMceMessageCodec::VAD
// -----------------------------------------------------------------------------
//
TBool CMceMessageCodec::VAD() const
    {
    return FLAT_DATA( iEnableVAD );
    }

// -----------------------------------------------------------------------------
// CMceMessageCodec::PTime
// -----------------------------------------------------------------------------
//
TUint CMceMessageCodec::PTime() const        
    {
    return FLAT_DATA( iPTime );
    }

// -----------------------------------------------------------------------------
// CMceMessageCodec::MaxPTime
// -----------------------------------------------------------------------------
//
TUint CMceMessageCodec::MaxPTime() const        
    {
    return FLAT_DATA( iMaxPTime );
    }
            
// -----------------------------------------------------------------------------
// CMceMessageCodec::CMceMessageCodec
// -----------------------------------------------------------------------------
//
CMceMessageCodec::CMceMessageCodec() :
    CMceCodec()
    {
    }

// -----------------------------------------------------------------------------
// CMceMessageCodec::ConstructL
// -----------------------------------------------------------------------------
//
void CMceMessageCodec::ConstructL( CMceComMessageCodec* aFlatData )
    {
    CMceCodec::ConstructL( aFlatData );
    }
            

// -----------------------------------------------------------------------------
// CMceMessageCodec::Factory
// -----------------------------------------------------------------------------
//
/*TMceMessageCodecFactory CMceMessageCodec::Factory()
    {
    return TMceMessageCodecFactory();
    }*/
