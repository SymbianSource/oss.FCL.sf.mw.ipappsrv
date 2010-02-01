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
* Description:    Controller
*
*/




// INCLUDE FILES
#include "mccaudiorouter.h"
#include "mmccinterfacedef.h"
#include "mccsubcontrollerlogs.h"
#include <AudioOutput.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccAudioRouter::NewL
// -----------------------------------------------------------------------------
// 
CMccAudioRouter* CMccAudioRouter::NewL( CMMFDevSound& aDevSound )
    {
    CMccAudioRouter* self = new ( ELeave ) CMccAudioRouter;
    CleanupStack::PushL( self );
    self->ConstructL( aDevSound );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccAudioRouter::~CMccAudioRouter
// -----------------------------------------------------------------------------
// 
CMccAudioRouter::~CMccAudioRouter()
    {
    delete iAudioRouter;
    }

// -----------------------------------------------------------------------------
// CMccAudioRouter::SetAudioRouteL
// -----------------------------------------------------------------------------
//    
void CMccAudioRouter::SetAudioRouteL( 
    TUint32 aRoutingDestination )
    {
    __SUBCONTROLLER_INT1( "CMccAudioRouter::SetAudioRouteL, route to:", 
                          aRoutingDestination );
                          
    __ASSERT_ALWAYS( iRoutingSupported, User::Leave( KErrNotSupported ) );
                     
    // Mcc audio route constants imitate enumerations defined in audiooutput.h
    CAudioOutput::TAudioOutputPreference output = CAudioOutput::ENoPreference;
    switch ( aRoutingDestination )
        {
        case KMccAudioToPrivate:
            {
            output = CAudioOutput::EPrivate;
            break;
            }
        case KMccAudioToPublic:
            {
            output = CAudioOutput::EPublic;
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            break;
            }
        }
    
    iAudioRouter->SetAudioOutputL( output );
        
    __SUBCONTROLLER( "CMccAudioRouter::SetAudioRouteL, exit" );
    }
    
// -----------------------------------------------------------------------------
// CMccAudioRouter::GetAudioRouteL
// -----------------------------------------------------------------------------
//    
void CMccAudioRouter::GetAudioRouteL( 
    TUint32& aRoutingDestination )
    {
    __SUBCONTROLLER( "CMccAudioRouter::GetAudioRouteL");
    
    __ASSERT_ALWAYS( iRoutingSupported, User::Leave( KErrNotSupported ) );
                           
    // Mcc audio route constants imitate enumerations defined in audiooutput.h 
    aRoutingDestination = (TUint32)iAudioRouter->AudioOutput();
    
    __SUBCONTROLLER( "CMccAudioRouter::GetAudioRouteL, exit");
    }
                    
// -----------------------------------------------------------------------------
// CMccAudioRouter::ConstructL
// -----------------------------------------------------------------------------
//
void CMccAudioRouter::ConstructL( CMMFDevSound& aDevSound )
    {    
    TRAPD( err, iAudioRouter = CAudioOutput::NewL( aDevSound ) );
    iRoutingSupported = ( !err && iAudioRouter );
    }
    
// -----------------------------------------------------------------------------
// CMccAudioRouter::CMccAudioRouter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccAudioRouter::CMccAudioRouter()
    {
    }

