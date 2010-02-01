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




#include <sipprofile.h>
#include "CMCETestUIEngineProfile.h"

// Applicaple commands
#include "TMCETestUIEngineCmdBase.h"
#include "TMCETestUIEngineCmdRegister.h"
#include "TMCETestUIEngineCmdUnregister.h"
#include "TMCETestUIEngineCmdCreateSession.h"
#include "TMCETestUIEngineCmdCreatePoCSession.h"
#include "TMCETestUIEngineCmdCreateVoIPSession.h"
#include "TMCETestUIEngineCmdCreate2WayVideoSession.h"
#include "TMCETestUIEngineCmdCreateEvent.h"
#include "TMCETestUIEngineCmdCreateMceSettings.h"
#include "TMCETestUIEngineCmdCreateMceValueSettings.h"
#include "TMCETestUIEngineCmdDisplayProfileUser.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfile* CMCETestUIEngineProfile::NewL( 
        CSIPProfile* aProfile, CMCETestUIEngine& aEngine ) 
    {

    CMCETestUIEngineProfile* self = 
            new (ELeave) CMCETestUIEngineProfile( aProfile, aEngine );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;  
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::CMCETestUIEngineProfile
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfile::CMCETestUIEngineProfile( CSIPProfile* aProfile, 
                                                  CMCETestUIEngine& aEngine )
    :iProfile( aProfile ),
    iEngine( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::ConstructL
// -----------------------------------------------------------------------------
//
void CMCETestUIEngineProfile::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::~CMCETestUIEngineProfile
// -----------------------------------------------------------------------------
//
CMCETestUIEngineProfile::~CMCETestUIEngineProfile()
    {
    iCommands.ResetAndDestroy();
    delete iProfile;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::GetCommands
// -----------------------------------------------------------------------------
//
const RPointerArray<TMCETestUIEngineCmdBase>& 
            CMCETestUIEngineProfile::GetCommandsL()
    {
    iCommands.ResetAndDestroy();
    
    if ( !IsRegistered() )
        {    
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdRegister( 
                                iEngine, *this ) );
                                        }
    else
        {
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreateSession( 
                                iEngine, *this ) );
        
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreatePoCSession( 
                                iEngine, *this ) );
                                
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreateVoIPSession( 
                                iEngine, *this ) );
        
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreate2WayVideoSession( 
                                iEngine, *this ) );
                                
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreateEvent( 
                                iEngine, *this ) );
        iCommands.Append( 
                    new (ELeave) TMCETestUIEngineCmdCreateRefer( 
                    			iEngine, *this) );
                                        
   /*     iCommands.Append( new (ELeave) TMCETestUIEngineCmdUnregister( 
                                iEngine, *this ) );
        
    */    iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreateMceSettings( 
                                iEngine, *this ) );
                                
        iCommands.Append( new (ELeave) TMCETestUIEngineCmdCreateMceValueSettings( 
                                iEngine, *this ) );
                                
        }
    iCommands.Append( new (ELeave) TMCETestUIEngineCmdDisplayProfileUser(iEngine, *this) );
    
    return iCommands;
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::Profile
// -----------------------------------------------------------------------------
//
CSIPProfile& CMCETestUIEngineProfile::Profile()
    {
    return *iProfile;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::IsDefault
// -----------------------------------------------------------------------------
//            
EXPORT_C TBool CMCETestUIEngineProfile::IsDefault() const
     {
     TBool isDefault = EFalse;
     iProfile->GetParameter( KSIPDefaultProfile, isDefault );
     return isDefault;
     }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::IsRegistered
// -----------------------------------------------------------------------------
//            
EXPORT_C TBool CMCETestUIEngineProfile::IsRegistered() const
     {
     TBool profileRegistered = EFalse;
     iProfile->GetParameter( KSIPProfileRegistered, profileRegistered );
     return profileRegistered;
     }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::ProfileId
// -----------------------------------------------------------------------------
//            
EXPORT_C TInt CMCETestUIEngineProfile::ProfileId() const
     {
     TUint32 profileId = 0;
     iProfile->GetParameter( KSIPProfileId, profileId );
     return profileId;
     }

// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::ProviderName
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMCETestUIEngineProfile::ProviderName() const
     {
     const TDesC8* providerName = NULL;    
     iProfile->GetParameter( KSIPProviderName, providerName );
     return *providerName;
     }
// -----------------------------------------------------------------------------
// CMCETestUIEngineProfile::Aor
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CMCETestUIEngineProfile::Aor() const
     {
     const TDesC8* aor = NULL;    
     iProfile->GetParameter( KSIPUserAor, aor );
     return *aor;
     }