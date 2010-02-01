/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Factory class to create Codec information instances
*
*/





// INCLUDE FILES
#include "mmcccodecinformationfactory.h"
#include "mmcccodecinformation.h"
#include "mmcccodecamr.h"
#include "mmcccodecg711.h"
#include "mmcccodecg729.h"
#include "mmcccodecilbc.h"
#include "mmcccodech263.h"
#include "mmcccodecavc.h"
#include "mmcccodecred.h"
#include "mmcccodecdtmf.h"
#include "mmcccodeccn.h"
#include "mmcccodecamrwb.h"


// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccCodecInformationFactory::CMccCodecInformationFactory
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccCodecInformationFactory::CMccCodecInformationFactory()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecInformationFactory::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMccCodecInformationFactory* CMccCodecInformationFactory::NewL()
    {
    CMccCodecInformationFactory* self = new( ELeave ) CMccCodecInformationFactory;
    
    // Nothing else to do, so return
    return self;
    }

    
// Destructor
CMccCodecInformationFactory::~CMccCodecInformationFactory()
    {
    }

// -----------------------------------------------------------------------------
// CMccCodecInformationFactory::CreateCodecInformationL
// Factory method for codec information instance creation.
// -----------------------------------------------------------------------------
//
EXPORT_C CMccCodecInformation* CMccCodecInformationFactory::CreateCodecInformationL(
    const TDesC8& aSdpName )
    {
    CMccCodecInformation* codec = NULL;

    if ( !aSdpName.CompareF( KAMRSdpName ))
        {
        codec = CMccCodecAMR::NewL();
        CleanupStack::PushL( codec );
        }
    else if ( !aSdpName.CompareF( KAMRWbSdpName ))
        {
        codec = CMccCodecAmrWb::NewL();
        CleanupStack::PushL( codec );
        }
    else if ( !aSdpName.CompareF( KPCMUSdpName ) ||
              !aSdpName.CompareF( KPCMASdpName ))
        {
        codec = CMCCCodecG711::NewL();
        CleanupStack::PushL( codec );
        codec->SetSdpName( aSdpName );
        }
    else if ( !aSdpName.CompareF( KILBCSdpName ))
        {
        codec = CMCCCodecILBC::NewL();
        CleanupStack::PushL( codec );
        }
    else if ( !aSdpName.CompareF( KG729SdpName ))
        {
        codec = CMCCCodecG729::NewL();
        CleanupStack::PushL( codec );
        }
    else if ( !aSdpName.CompareF( KH263SdpName ) ||
              !aSdpName.CompareF( KH2632000SdpName ))
        {
        codec = CMccCodecH263::NewL();
        CleanupStack::PushL( codec );
        }
    else if ( !aSdpName.CompareF( KH2631998SdpName ))
        {
        codec = CMccCodecH263::NewL();
        CleanupStack::PushL( codec );
        codec->SetSdpName( KH2631998SdpName );
        }
    else if ( !aSdpName.CompareF( KAVCSdpName ))
        {
        codec = CMccCodecAVC::NewL();
        CleanupStack::PushL( codec );
        }
    else if ( !aSdpName.CompareF( KRedSdpName ))
        {
        codec = CMccCodecRed::NewL();
        CleanupStack::PushL( codec );
        }
    else if( !aSdpName.CompareF( KTelephoneEvent ) ) 
        {
        codec = CMccCodecDTMF::NewL();
        CleanupStack::PushL( codec );
        }
    else if( !aSdpName.CompareF( KCnSdpName ) ) 
        {
        codec = CMccCodecCn::NewL();
        CleanupStack::PushL( codec );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    CleanupStack::Pop( codec );
    return codec;
    }

// -----------------------------------------------------------------------------
// CMccCodecInformationFactory::CreateCodecInformationL
// Factory method for codec information instance creation.
// -----------------------------------------------------------------------------
//
EXPORT_C CMccCodecInformation* CMccCodecInformationFactory::CreateCodecInformationL(
    TFourCC aFourCC )
    {
    CMccCodecInformation* codec = NULL;

    if ( aFourCC == KMccFourCCIdAMRNB )
        {
        codec = CMccCodecAMR::NewL();
        }
    else if ( aFourCC == KMccFourCCIdAMRWB )
        {
        codec = CMccCodecAmrWb::NewL();
        }
    else if ( aFourCC == KMccFourCCIdH263 )
        {
        codec = CMccCodecH263::NewL();
        }
    else if( aFourCC == KMccFourCCIdDTMF ) 
        {
        codec = CMccCodecDTMF::NewL();
        }
    else if ( aFourCC == KMccFourCCIdAVC )
        {
        codec = CMccCodecAVC::NewL();
        }        
    else if ( aFourCC == KMccFourCCIdG711 )
        {
        codec = CMCCCodecG711::NewL();
        }        
    else if ( aFourCC == KMccFourCCIdILBC )
        {
        codec = CMCCCodecILBC::NewL();
        }              
    else if ( aFourCC == KMccFourCCIdG729 )
        {
        codec = CMCCCodecG729::NewL();
        }
    else if ( aFourCC == KMccFourCCIdRed )
        {
        codec = CMccCodecRed::NewL();
        }
    else if ( aFourCC == KMccFourCCIdCN )
        {
        codec = CMccCodecCn::NewL();
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
        
    return codec;
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================


//  End of File  
