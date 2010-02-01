/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


// INCLUDE FILES

#include "CMCETestUIMainViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineProfile.h"
#include "CMCETestUIEngineOutSession.h"
#include "CMceTestUIEngineOutEvent.h"
#include "CMceTestUIEngineInEvent.h"
#include "CMceTestUIEngineOutRefer.h"
#include "CMceTestUIEngineInRefer.h"
#include "CMceTestUIEngineInSession.h"
#include <mcesession.h>

// CONSTANTS

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIMainViewModel* CMCETestUIMainViewModel::NewL(
        CMCETestUIEngine& aEngine )
    {
	CMCETestUIMainViewModel* self = 
	    new(ELeave) CMCETestUIMainViewModel( aEngine );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::CMCETestUIMainViewModel
// -----------------------------------------------------------------------------
//
CMCETestUIMainViewModel::CMCETestUIMainViewModel( CMCETestUIEngine& aEngine )
    :iEngine( aEngine ),
    managerSuppressed( EFalse ),
    profilesSuppressed( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::ConstructL()
    {
    iArray = new (ELeave) CDesCArrayFlat( 1 );

    PopulateMainViewManagerL();
	PopulateMainViewProfilesL();
    PopulateMainViewOutSessionsL();
    PopulateMainViewInSessionsL();
    PopulateMainViewOutEventsL();
    PopulateMainViewInEventsL();
    PopulateMainViewOutRefersL();
    PopulateMainViewInRefersL();
    
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::~CMCETestUIMainViewModel()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIMainViewModel::~CMCETestUIMainViewModel()
    {
    delete iArray;
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::MdcaCount() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIMainViewModel::MdcaCount() const
    {
    return iArray->MdcaCount();
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::MdcaPoint() const
// 
// -----------------------------------------------------------------------------
//
TPtrC CMCETestUIMainViewModel::MdcaPoint( TInt aIndex ) const
    {
    return iArray->MdcaPoint( aIndex );
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::IsManagerSelected() const
// 
// -----------------------------------------------------------------------------
//
TBool CMCETestUIMainViewModel::IsManagerSelected( TInt aIndex ) const
    {
    if ( aIndex == 0 && !managerSuppressed )
        {
        return ETrue;
        }
    
    return EFalse;    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::SelectedProfileIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIMainViewModel::SelectedProfileIndex( TInt aIndex ) const
    {

    if ( managerSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0 )
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the manager

    const RPointerArray<CMCETestUIEngineProfile>& profiles = 
                iEngine.UsedProfiles();

    if ( profilesSuppressed )
        {
        aIndex += profiles.Count();
        }

    if ( aIndex < profiles.Count() )
        {
        return aIndex;
        }
    
    return KErrNotFound;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::SelectedSessionIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIMainViewModel::SelectedSessionIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineProfile>& profiles = 
                iEngine.UsedProfiles();

    const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
                iEngine.OutSessions();               
                
    const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
                iEngine.InSessions(); 


    // Next does not work correctly
                    
    if ( managerSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the manager
    
    if ( profilesSuppressed )
        {
        aIndex += profiles.Count();
        }

    if ( aIndex < profiles.Count() )
        {
        return KErrNotFound;
        }
    
    aIndex -= profiles.Count(); // Selected is not a profile
    
    if ( aIndex < outSessions.Count() + inSessions.Count() )
        {
        return aIndex;
        }
        
    return KErrNotFound;   // default      
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::SelectedEventIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIMainViewModel::SelectedEventIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineProfile>& profiles = 
                iEngine.UsedProfiles();

    const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
                iEngine.OutSessions();               
                
    const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
                iEngine.InSessions(); 
                
    const RPointerArray<CMCETestUIEngineOutEvent>& outEvents = 
                iEngine.OutEvents();               
                
    const RPointerArray<CMCETestUIEngineInEvent>& inEvents = 
                iEngine.InEvents(); 
            


    // Next does not work correctly
                    
    if ( managerSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the manager
    
    if ( profilesSuppressed )
        {
        aIndex += profiles.Count();
        }

    if ( aIndex < profiles.Count() )
        {
        return KErrNotFound;
        }
    
    aIndex -= profiles.Count(); // Selected is not a profile
    
    if ( aIndex < outSessions.Count() + inSessions.Count() )
        {
        return aIndex;
        }
        
    aIndex -= (outSessions.Count() + inSessions.Count() ); // Selected is not session
    
    if ( aIndex < outEvents.Count() + inEvents.Count() )
        {
        return aIndex;
        }    
        
    return KErrNotFound;   // default      
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::SelectedReferIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIMainViewModel::SelectedReferIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineProfile>& profiles = 
                iEngine.UsedProfiles();

    const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
                iEngine.OutSessions();               
                
    const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
                iEngine.InSessions(); 
                
    const RPointerArray<CMCETestUIEngineOutEvent>& outEvents = 
                iEngine.OutEvents();               
                
    const RPointerArray<CMCETestUIEngineInEvent>& inEvents = 
                iEngine.InEvents(); 
            
	const RPointerArray<CMCETestUIEngineOutRefer>& outRefers = 
                iEngine.OutRefers();               
                
    const RPointerArray<CMCETestUIEngineInRefer>& inRefers = 
                iEngine.InRefers();

    // Next does not work correctly
                    
    if ( managerSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the manager
    
    if ( profilesSuppressed )
        {
        aIndex += profiles.Count();
        }

    if ( aIndex < profiles.Count() )
        {
        return KErrNotFound;
        }
    
    aIndex -= profiles.Count(); // Selected is not a profile
    
    if ( aIndex < outSessions.Count() + inSessions.Count() )
        {
        return aIndex;
        }
        
    aIndex -= (outSessions.Count() + inSessions.Count() ); // Selected is not session
    
    if ( aIndex < outEvents.Count() + inEvents.Count() )
        {
        return aIndex;
        }    
   	
   	aIndex -= (outEvents.Count() + inEvents.Count() ); // Selected is not event
    
    if ( aIndex < outRefers.Count() + inRefers.Count() )
        {
        return aIndex;
        }    
    
    return KErrNotFound;   // default      
    }
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::CmdProvider()
// 
// -----------------------------------------------------------------------------
//
MMCETestUIEngineCmdProvider& CMCETestUIMainViewModel::CmdProvider( TInt aIndex )
    {
    
    const RPointerArray<CMCETestUIEngineProfile>& profiles = 
                iEngine.UsedProfiles();

    const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
                iEngine.OutSessions();               
                
    const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
                iEngine.InSessions(); 

	const RPointerArray<CMCETestUIEngineOutEvent>& outEvents = 
                iEngine.OutEvents();               
                
    const RPointerArray<CMCETestUIEngineInEvent>& inEvents = 
                iEngine.InEvents();

	const RPointerArray<CMCETestUIEngineOutRefer>& outRefers = 
                iEngine.OutRefers();               
                
    const RPointerArray<CMCETestUIEngineInRefer>& inRefers = 
                iEngine.InRefers();

    // Next does not work correctly
                    
    if ( managerSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return iEngine;
        }

    aIndex -= 1; // Selected is not the manager
    
    if ( profilesSuppressed )
        {
        aIndex += profiles.Count();
        }

    if ( aIndex < profiles.Count() )
        {
        return *(profiles[aIndex]);
        }
    
    aIndex -= profiles.Count(); // Selected is not a profile
    
    if ( aIndex < outSessions.Count() )
        {
        return *(outSessions[aIndex]);
        }
    
    aIndex -= outSessions.Count(); // Selected is not an outsession
    
    if ( aIndex < inSessions.Count() )
        {
        return *(inSessions[aIndex]);
        }
    
    aIndex -= inSessions.Count(); // Selected is not a session
    
    if ( aIndex < outEvents.Count() )
        {
        return *(outEvents[aIndex]);
        }
    
    aIndex -= outEvents.Count(); // Selected is not an outEvent
    
    if ( aIndex < inEvents.Count() )
        {
        return *(inEvents[aIndex]);
        }
    
   aIndex -= inEvents.Count(); // Selected is not an event
    
    if ( aIndex < outRefers.Count() )
        {
        return *(outRefers[aIndex]);
        }
    
    aIndex -= outRefers.Count(); // Selected is not an outrefer
    
    if ( aIndex < inRefers.Count() )
        {
        return *(inRefers[aIndex]);
        }
        
    return iEngine;   // default      
    
        
    }


// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewManagerL
// Populate main view manager
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewManagerL()
    {
    if ( !managerSuppressed )
        {
        TInt sessionCount = iEngine.OutSessions().Count() +
                            iEngine.InSessions().Count();
                            
        TInt eventCount =  iEngine.OutEvents().Count() +
                            iEngine.InEvents().Count(); 
                            
        TInt referCount =  iEngine.OutRefers().Count() +
                            iEngine.InRefers().Count();                    
                            
        TInt itemLength = KMCEManager().Length() + KSessionCount().Length() + 
        				  KEventCount().Length() + KReferCount().Length() + KTab().Length() * 4 + KSpace().Length() + 10;
                          
        HBufC* item =  HBufC::NewLC( itemLength );
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KMCEManager );
        itemPtr.Append( KTab );
        itemPtr.Append( KSessionCount );
        itemPtr.AppendNum( sessionCount );
     //   itemPtr.Append( KSpace );
        itemPtr.Append( KEventCount );
        itemPtr.AppendNum( eventCount );
     //   itemPtr.Append( KSpace );
        itemPtr.Append( KReferCount );
        itemPtr.AppendNum( referCount );
               
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewProfilesL
// Populate main view profiles
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewProfilesL()
    {   
    if ( !profilesSuppressed )
        {
        const RPointerArray<CMCETestUIEngineProfile>& profiles = 
            iEngine.UsedProfiles();
            
        for ( TInt i = 0; i < profiles.Count(); ++i )
            {
            const TDesC8& providerName = profiles[i]->ProviderName();
            HBufC* providerName16 = HBufC16::NewLC( providerName.Length() );
            TPtr providerNamePtr = providerName16->Des();
            providerNamePtr.Copy( providerName );
            
            const TDesC8& aor = profiles[i]->Aor();
            HBufC* aor16 = HBufC16::NewLC( aor.Length() );
            TPtr aorPtr = aor16->Des();
            aorPtr.Copy( aor );
            
            TPtrC registerStatus;
            if ( profiles[i]->IsRegistered() )
                {
                registerStatus.Set( KRegistered );
                }
            else
                {
                registerStatus.Set( KNotRegistered );
                }
            TInt itemLength = providerName.Length() + registerStatus.Length() + 
                              KProfile().Length() + aor.Length() +
                              KSpace().Length() +KTab().Length()*4;
                            
            HBufC* item =  HBufC::NewLC( itemLength );
            TPtr itemPtr = item->Des();
            itemPtr.Append( KTab );
            itemPtr.Append( KProfile );
            itemPtr.Append( *providerName16 );
            itemPtr.Append( KSpace );
            itemPtr.Append( registerStatus );
            itemPtr.Append( KTab );
            //itemPtr.Append(KSpace);
            itemPtr.Append( *aor16 );
            itemPtr.Append( KTab );
            iArray->AppendL( *item );

            CleanupStack::PopAndDestroy( item );
            item = NULL;
            CleanupStack::PopAndDestroy( aor16 );
            aor16 = NULL;
            CleanupStack::PopAndDestroy( providerName16 );
            providerName16 = NULL;
          
            
            }
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewOutSessionsL
// Populate main view outgoing sessions
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewOutSessionsL()
    {
    const RPointerArray<CMCETestUIEngineOutSession>& outSessions = 
            iEngine.OutSessions();

	if( outSessions.Count() == 0) return;
	
    for ( TInt i = 0; i < outSessions.Count(); ++i )
        {
        TInt itemLength = KOutSession().Length() + 
                          KLeftParenthesis().Length() + 
                          KProfileId().Length() + 2 +KSpace().Length()+
                          outSessions[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() +
                          outSessions[i]->State().Length() +
                          KTab().Length() * 4 + 5 ;  
        HBufC* item =  HBufC::NewLC( itemLength ); 
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );        
        itemPtr.Append( KOutSession );
        itemPtr.AppendNum( outSessions[i]->Id() );
        itemPtr.Append( KLeftParenthesis );
        itemPtr.Append( KProfileId);
        itemPtr.AppendNum( outSessions[i]->ProfileId());
        itemPtr.Append( KSpace);
        itemPtr.Append( outSessions[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis );
        itemPtr.Append( KTab );
        itemPtr.Append( outSessions[i]->State() );
    	iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;     
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewInSessionsL
// Populate main view incoming sessions
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewInSessionsL()
    {
    const RPointerArray<CMCETestUIEngineInSession>& inSessions = 
            iEngine.InSessions();
	if( inSessions.Count() == 0) return;

    for ( TInt i = 0; i < inSessions.Count(); ++i )
        {
        TInt itemLength = KInSession().Length() + 
                          KLeftParenthesis().Length() + 
                          KProfileId().Length() + 2 +KSpace().Length()+
                          inSessions[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() + 
                          inSessions[i]->State().Length() +
                          KTab().Length() * 4 + 5 ;  
        HBufC* item =  HBufC::NewLC( itemLength ); 
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );        
        itemPtr.Append( KInSession );
        itemPtr.AppendNum( inSessions[i]->Id() );
        itemPtr.Append( KLeftParenthesis );
        itemPtr.Append( KProfileId);
        itemPtr.AppendNum( inSessions[i]->ProfileId());
        itemPtr.Append( KSpace);
        itemPtr.Append( inSessions[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis );
        itemPtr.Append( KTab );
        itemPtr.Append( inSessions[i]->State() );
        iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;     
        }
    }
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewOutSessionsL
// Populate main view outgoing sessions
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewOutEventsL()
    {
    const RPointerArray<CMCETestUIEngineOutEvent>& outEvents = 
            iEngine.OutEvents();
    if( outEvents.Count() == 0) return;

    for ( TInt i = 0; i < outEvents.Count(); ++i )
        {
        TInt itemLength = KOutEvent().Length() + 
                          outEvents[i]->State().Length() +
                          KTab().Length() * 3 + 5 ;  
        HBufC* item =  HBufC::NewLC( itemLength ); 
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );        
        itemPtr.Append( KOutEvent);
        itemPtr.AppendNum( outEvents[i]->Id() );
        itemPtr.Append( KTab );
        itemPtr.Append( outEvents[i]->State() );
   	    iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;     
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewInEventsL
// Populate main view incoming sessions
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewInEventsL()
    {
    const RPointerArray<CMCETestUIEngineInEvent>& inEvents = 
            iEngine.InEvents();
	if( inEvents.Count() == 0) return;

    for ( TInt i = 0; i < inEvents.Count(); ++i )
        {
        TInt itemLength = KInEvent().Length() + 
                          inEvents[i]->State().Length() +
                          KTab().Length() * 3 + 5 ;  
        HBufC* item =  HBufC::NewLC( itemLength ); 
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );        
        itemPtr.Append( KInEvent );
        itemPtr.AppendNum( inEvents[i]->Id() );
        itemPtr.Append( KTab);
        itemPtr.Append( inEvents[i]->State() );
        iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;     
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewOutRefersL
// Populate main view outgoing sessions
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewOutRefersL()
    {
    const RPointerArray<CMCETestUIEngineOutRefer>& outRefers = 
            iEngine.OutRefers();
    if( outRefers.Count() == 0) return;

    for ( TInt i = 0; i < outRefers.Count(); ++i )
        {
        TInt itemLength = KOutRefer().Length() + 
                          outRefers[i]->State().Length() +
                          KTab().Length() * 3 + 5 ;  
        HBufC* item =  HBufC::NewLC( itemLength ); 
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );        
        itemPtr.Append( KOutRefer);
        itemPtr.AppendNum( outRefers[i]->Id() );
        itemPtr.Append( KTab );
        itemPtr.Append( outRefers[i]->State() );
   	    iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;     
        }
    }

// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::PopulateMainViewInRefersL
// Populate main view incoming sessions
// -----------------------------------------------------------------------------
//
void CMCETestUIMainViewModel::PopulateMainViewInRefersL()
    {
    const RPointerArray<CMCETestUIEngineInRefer>& inRefers = 
            iEngine.InRefers();
	if( inRefers.Count() == 0) return;

    for ( TInt i = 0; i < inRefers.Count(); ++i )
        {
        TInt itemLength = KInRefer().Length() + 
                          inRefers[i]->State().Length() +
                          KTab().Length() * 3 + 5 ;  
        HBufC* item =  HBufC::NewLC( itemLength ); 
        TPtr itemPtr = item->Des();
        itemPtr.Append( KTab );        
        itemPtr.Append( KInRefer );
        itemPtr.AppendNum( inRefers[i]->Id() );
        itemPtr.Append( KTab);
        itemPtr.Append( inRefers[i]->State() );
        iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;     
        }
    }

// End of File  
