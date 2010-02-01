/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include <e32base.h>
#include "nsptestconsolestatemach.h"
#include "nsptestconsolecenrep.h"
#include "nsptestmanager.h"
#include "nsptest.h"
#include "nspresultcollector.h"

#define STATE_ID_L( statearray, state ) \
        User::LeaveIfError( statearray.Find( state ) )

_LIT( KTxtOngoing, "..\n" );
_LIT( KTxtSettings, "Settings." );
_LIT( KTxtReleaseTests, "Release tests." );
_LIT( KTxtIceTests, "ICE tests." );
_LIT( KTxtResults, "Results." );
_LIT( KPrev, "[Previous menu]" );
_LIT( KTxtRun, "Run." );
_LIT( KTxtCancel, "[Cancel]" );
_LIT( KTxtPressAnyKey, "[Press any key]" );
_LIT( KDomain, "Domain." );
_LIT( KIap, "IAP." );
_LIT( KAprotocol, "APROTOCOL." );
_LIT( KStunSrvAddr, "Stun srv address." );
_LIT( KStunSrvPort, "Stun srv port." );

/**************************************************/
/***** MAIN MENU **********************************/
_LIT( KTxtMainInstructions, "Main menu:\n" );

void TNPSTestConsoleStateMain::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtMainInstructions(), aEvent.Status() );
	
	aEvent.Actions().DisplaySelection( 1, KTxtSettings() );
	aEvent.Actions().DisplaySelection( 2, KTxtReleaseTests() );
	aEvent.Actions().DisplaySelection( 3, KTxtIceTests() );
	
	aEvent.Actions().DisplayPrompt( ETrue );
	aEvent.Actions().DoRead();
	}

void TNPSTestConsoleStateMain::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		case 49: // 1
		case 92:
			{
			aEvent.NextState() = KStateSettings;
			break;
			}
		
		case 50: // 2
		case 97:
			{
			aEvent.NextState() = KStateRelease;
			break;
			}
		
		case 51: // 3
		case 100:
			{
			aEvent.NextState() = KStateIce;
			break;
			}
		
		default:
			aEvent.Status() = KErrArgument;
		}
	}

/**************************************************/
/***** SETTINGS ***********************************/
_LIT( KTxtSettingsInstructions, "Settings:\n" );

void TNPSTestConsoleStateSettings::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtSettingsInstructions(), aEvent.Status() );
	
	TBuf<255> buffer;
	TCenrepReader reader( aEvent.Actions().Cenrep() );
	reader.ReadDomainL( buffer );
	aEvent.Actions().DisplaySelection( 1, KDomain(), buffer );
	
	TUint32 iap;
	reader.ReadIapL( iap );
	aEvent.Actions().DisplaySelection( 2, KIap(), iap );
	
	aEvent.Actions().DisplaySelection( 3, KPrev() );
	aEvent.Actions().DisplayPrompt( ETrue );
	aEvent.Actions().DoRead();
	}

void TNPSTestConsoleStateSettings::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		case 49: // 1
		case 92:
			{
			aEvent.Actions().DisplayOne();
			aEvent.Actions().DisplayPrompt();
			
			TBuf<255> buffer;
			TCenrepReader reader( aEvent.Actions().Cenrep() );
			reader.ReadDomainL( buffer );
			
			if ( !buffer.Length() )
				{
				aEvent.Actions().GetStringFromConsoleL( buffer );
				TCenrepWriter writer( reader.Cenrep() );
				writer.WriteDomainL( buffer );
				}
			else
				{
				aEvent.NextState() = KStateDomainSettings;
				}
			
			break;
			}
		
		case 50: // 2
		case 97:
			{
			aEvent.Actions().DisplayTwo();
			aEvent.Actions().DisplayPrompt();
			
			TUint32 iap( 0 );
			TCenrepReader reader( aEvent.Actions().Cenrep() );
			reader.ReadIapL( iap );
			aEvent.Actions().GetTUint32FromConsoleL( iap );
			TCenrepWriter writer( reader.Cenrep() );
			writer.WriteIapL( iap );
			break;
			}
		
		case 51: // 3
		case 100:
			{
			aEvent.NextState() = KStateMain;
			break;
			}
		
		default:
			aEvent.Status() = KErrArgument;
		}
	}

/**************************************************/
/***** DOMAIN SETTINGS ****************************/
_LIT( KTxtDomainSettingsInstructions, "Domain specific Settings:\n" );

void TNPSTestConsoleStateDomainSettings::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtDomainSettingsInstructions(), aEvent.Status() );
	
	TBuf<255> buffer;
	TCenrepReader reader( aEvent.Actions().Cenrep() );
	buffer.Zero();
	reader.ReadDomainL( buffer );
	aEvent.Actions().DisplaySelection( 1, KDomain(), buffer );
	
	buffer.Zero();
	reader.ReadAProtocolL( buffer );
	aEvent.Actions().DisplaySelection( 2, KAprotocol(), buffer );
	
	buffer.Zero();
	reader.ReadStunSrvAddrL( buffer );
	aEvent.Actions().DisplaySelection( 3, KStunSrvAddr(), buffer );
	
	TUint port;
	reader.ReadStunSrvPortL( port );
	aEvent.Actions().DisplaySelection( 4, KStunSrvPort(), (TUint32) port );
	
	aEvent.Actions().DisplaySelection( 5, KPrev() );
	aEvent.Actions().DisplayPrompt( ETrue );
	aEvent.Actions().DoRead();
	}

void TNPSTestConsoleStateDomainSettings::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		case 49: // 1
		case 92:
			{
			aEvent.Actions().DisplayOne();
			aEvent.Actions().DisplayPrompt();
			
			TBuf<255> buffer;
			TCenrepReader reader( aEvent.Actions().Cenrep() );
			reader.ReadDomainL( buffer );
			aEvent.Actions().GetStringFromConsoleL( buffer );
			TCenrepWriter writer( reader.Cenrep() );
			writer.WriteDomainL( buffer );
			break;
			}
		
		case 50: // 2
		case 97:
			{
			aEvent.Actions().DisplayTwo();
			aEvent.Actions().DisplayPrompt();
			
			TBuf<255> buffer;
			TCenrepReader reader( aEvent.Actions().Cenrep() );
			reader.ReadAProtocolL( buffer );
			aEvent.Actions().GetStringFromConsoleL( buffer );
			TCenrepWriter writer( reader.Cenrep() );
			writer.WriteAProtocolL( buffer );
			break;
			}
		
		case 51: // 3
		case 100:
			{
			aEvent.Actions().DisplayThree();
			aEvent.Actions().DisplayPrompt();
			
			TBuf<255> buffer;
			TCenrepReader reader( aEvent.Actions().Cenrep() );
			reader.ReadStunSrvAddrL( buffer );
			aEvent.Actions().GetStringFromConsoleL( buffer );
			TCenrepWriter writer( reader.Cenrep() );
			writer.WriteStunSrvAddrL( buffer );
			break;
			}
		
		case 52: // 4
			{
			aEvent.Actions().DisplayFour();
			aEvent.Actions().DisplayPrompt();
			
			TUint port;
			TCenrepReader reader( aEvent.Actions().Cenrep() );
			reader.ReadStunSrvPortL( port );
			
			TUint32 integer( port );
			aEvent.Actions().GetTUint32FromConsoleL( integer );
			TCenrepWriter writer( reader.Cenrep() );
			writer.WriteStunSrvPortL( (TUint) integer );
			break;
			}
		
		case 53: // 5
			{
			aEvent.NextState() = KStateSettings;
			break;
			}
		
		default:
			aEvent.Status() = KErrArgument;
		}
	}

/**************************************************/
/***** RELEASE ************************************/
_LIT( KTxtReleaseInstructions, "Release test:\n" );

void TNPSTestConsoleStateRelease::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtReleaseInstructions(), aEvent.Status() );
	
	aEvent.Actions().DisplaySelection( 1, KTxtRun() );
	aEvent.Actions().DisplaySelection( 2, KTxtResults() );
	aEvent.Actions().DisplaySelection( 3, KPrev() );
	
	aEvent.Actions().DisplayPrompt( ETrue );
	aEvent.Actions().DoRead();
	}

void TNPSTestConsoleStateRelease::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		case 49: // 1
		case 92:
			{
			aEvent.Actions().DisplayOne();
			aEvent.Actions().Manager().RelTestL();
			aEvent.NextState() = KStateRunning;
			break;
			}
		
		case 50: // 2
		case 97:
			{
			aEvent.NextState() = KStateResults;
			break;
			}
		
		case 51: // 3
		case 100:
			{
			aEvent.NextState() = KStateMain;
			break;
			}
		
		default:
			aEvent.Status() = KErrArgument;
		}
	}

/**************************************************/
/***** RUNNING ************************************/
_LIT( KTxtRunningInstructions, "Running:\n" );

void TNPSTestConsoleStateRunning::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtRunningInstructions(), aEvent.Status() );
	
	aEvent.Actions().DoRead();
	aEvent.Actions().Manager().StartAllL( aEvent.Actions().Console() );
	aEvent.Actions().DisplayPrompt( KTxtPressAnyKey(), ETrue );
	}

void TNPSTestConsoleStateRunning::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		default:
			{
			aEvent.NextState() = KStateRelease;
			break;
			}
		}
	}

/**************************************************/
/***** RESULTS ************************************/
_LIT( KTxtResultsInstructions, "Results:\n" );

void TNPSTestConsoleStateResults::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtResultsInstructions(), aEvent.Status() );
	
	CResultCollector* results = CResultCollector::NewLC( aEvent.Actions().Console() );
	aEvent.Actions().Manager().AcceptL( *results );
	CleanupStack::PopAndDestroy( results );
	
	aEvent.Actions().DoRead();
	aEvent.Actions().DisplayPrompt( KTxtPressAnyKey(), ETrue );
	}

void TNPSTestConsoleStateResults::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		default:
			{
			aEvent.NextState() = KStateMain;
			break;
			}
		}
	}

/**************************************************/
/***** ICE ****************************************/
_LIT( KTxtIceInstructions, "Ice:\n" );

void TNPSTestConsoleStateIce::EntryL( TNSPStateEvent& aEvent )
	{
	aEvent.Actions().DisplayMenu( KTxtIceInstructions(), aEvent.Status() );
	
	// TBD:
	aEvent.Actions().DoRead();
	aEvent.Actions().DisplayPrompt( KTxtPressAnyKey(), ETrue );
	}

void TNPSTestConsoleStateIce::ExitL( TNSPStateEvent& aEvent )
	{
	switch ( aEvent.KeyCode() )
		{
		default:
			{
			aEvent.NextState() = KStateMain;
			break;
			}
		}
	}


// -----------------------------------------------------------------------------
// CNSPTestConsoleStateMachine::NewL
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleStateMachine* CNSPTestConsoleStateMachine::NewL()
    {
    CNSPTestConsoleStateMachine* self = new (ELeave) CNSPTestConsoleStateMachine;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleStateMachine::CNSPTestConsoleStateMachine
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleStateMachine::CNSPTestConsoleStateMachine()
    {
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleStateMachine::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleStateMachine::ConstructL()
    {
    iCurrentState = new (ELeave) TNPSTestConsoleStateMain;
    
    TNSPTestConsoleState* state = iCurrentState;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateMain ) );
    CleanupStack::Pop( state );
    
    state = new (ELeave) TNPSTestConsoleStateSettings;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateSettings ) );
    CleanupStack::Pop( state );

    state = new (ELeave) TNPSTestConsoleStateDomainSettings;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateDomainSettings ) );
    CleanupStack::Pop( state );
    
    state = new (ELeave) TNPSTestConsoleStateRelease;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateRelease ) );
    CleanupStack::Pop( state );
    
    state = new (ELeave) TNPSTestConsoleStateRunning;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateRunning ) );
    CleanupStack::Pop( state );
    
    state = new (ELeave) TNPSTestConsoleStateResults;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateResults ) );
    CleanupStack::Pop( state );
    
    state = new (ELeave) TNPSTestConsoleStateIce;
    CleanupStack::PushL( state );
    User::LeaveIfError( iStateArray.Insert( state, KStateIce ) );
    CleanupStack::Pop( state );
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleStateMachine::~CNSPTestConsoleStateMachine
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleStateMachine::~CNSPTestConsoleStateMachine()
    {	
    iStateArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleStateMachine::Start
// -----------------------------------------------------------------------------
// 
TInt CNSPTestConsoleStateMachine::Start( TNSPStateEvent& aEvent )
    {
    TRAPD( err, iCurrentState->EntryL( aEvent ) );
    return err;
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleStateMachine::ProcessL
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleStateMachine::ProcessL( TNSPStateEvent& aEvent )
    {
    __ASSERT_ALWAYS( &aEvent, User::Leave( KErrArgument ) );
    
    TNSPTestConsoleStateIndex index = STATE_ID_L( iStateArray, iCurrentState );
    aEvent.NextState() = index;
    
    iCurrentState->ExitL( aEvent );
    
    if ( index != aEvent.NextState() &&
    	 0 <= aEvent.NextState() &&
    	 iStateArray.Count() > aEvent.NextState() )
    	{
    	TNSPTestConsoleState* newstate = iStateArray[ aEvent.NextState() ];
    	TRAPD( err, newstate->EntryL( aEvent ) );
    	
    	if ( KErrNone == err )
    		{
    		iCurrentState = newstate;
    		}
    	else
    		{
    		aEvent.Status() = err;
    		TRAP_IGNORE( iCurrentState->EntryL( aEvent ) );
    		}
    	}
    else
    	{
    	TRAP_IGNORE( iCurrentState->EntryL( aEvent ) );
    	}
    }

