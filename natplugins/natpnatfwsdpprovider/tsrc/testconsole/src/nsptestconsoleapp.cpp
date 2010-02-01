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

#include <centralrepository.h>
#include <unsafprotocolscrkeys.h>
#include <e32std.h>
#include <e32base.h>
#include <e32debug.h>
#include <e32cons.h>
#include <badesca.h>
#include <sdpdocument.h>
#include "nspplugin.h"
#include "nsptestconsoleapp.h"
#include "nsptestconsolestatemach.h"
#include "nsptestmanager.h"
#include "nsptest.h"

_LIT( KMainTitle, " NSP Console " );
_LIT( KLineBreak, "\n" );
_LIT( KPrompt, "\\>" );
_LIT( KOne, "1" );
_LIT( KTwo, "2" );
_LIT( KThree, "3" );
_LIT( KFour, "4" );

const TInt KPluginUid = { 0x102832EF };

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::NewLC
// -----------------------------------------------------------------------------
//
CNSPTestConsoleApp* CNSPTestConsoleApp::NewLC()
    {
    CNSPTestConsoleApp* self = new ( ELeave ) CNSPTestConsoleApp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::CNSPTestConsoleApp
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleApp::CNSPTestConsoleApp()
    : CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::ConstructL
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::ConstructL()
    {
    iPlugin = CNSPPlugin::NewL( TUid::Uid( KPluginUid ) );
    iRepository = CRepository::NewL( KCRUidUNSAFProtocols );
    iTestManager = CNSPTestManager::NewL( *iPlugin, *iRepository );
    iStateMachine = CNSPTestConsoleStateMachine::NewL();
	iConsole = Console::NewL( KMainTitle,
			TSize( KConsFullScreen, KConsFullScreen ) );
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::CNSPTestConsoleApp
// -----------------------------------------------------------------------------
// 
CNSPTestConsoleApp::~CNSPTestConsoleApp()
    {
    Cancel();
    delete iConsole;
    delete iStateMachine;
    delete iTestManager;
    delete iRepository;
    delete iPlugin;
    REComSession::FinalClose();
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::CNSPTestConsoleApp
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::StartTesting()
    {
	TNSPStateEvent event( *this, EKeyNull, KErrNone );
	iStateMachine->Start( event );
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::RunL
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::RunL()
    {
    TKeyCode keycode = iConsole->KeyCode();
    
    switch ( keycode )
    	{
    	case EKeyEscape:
    		{
    		iTestManager->CancelAll();
    		CActiveScheduler::Stop();
    		break;
    		}
    	
    	default:
    		{
    		TNSPStateEvent event( *this, keycode, KErrNone );
    		iStateMachine->ProcessL( event );
    		}
    	}
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::RunError
// -----------------------------------------------------------------------------
// 
TInt CNSPTestConsoleApp::RunError( TInt aError )
	{
	TNSPStateEvent event( *this, EKeyNull, aError );
	TRAPD( err, iStateMachine->ProcessL( event ) );
	return err;
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DoCancel
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DoCancel()
    {
	iConsole->ReadCancel();
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayConsoleMenu
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayMenu( const TDesC& aText, TInt aError )
	{
	iConsole->ClearScreen();
	iConsole->Printf( KLineBreak() );
	iConsole->Write( aText );
	iConsole->Printf( KLineBreak() );
	
	if ( KErrNone != aError )
		{
		iConsole->Printf( _L("[error:%d]\n\n"), aError );
		}
	else
		{
		iConsole->Printf( _L("[ESC to abort]\n\n") );
		}
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplaySelection
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplaySelection(
		TInt aPrefix, const TDesC& aText, TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( _L("%d."), aPrefix );
	iConsole->Write( aText );
	iConsole->Printf( KLineBreak() );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplaySelection
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplaySelection(
		TInt aPrefix, const TDesC& aText, const TDesC& aSelect, TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( _L("%d."), aPrefix );
	iConsole->Write( aText );
	iConsole->Printf( _L("[%S]\n"), &aSelect );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplaySelection
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplaySelection(
		TInt aPrefix, const TDesC& aText, TUint32 aSelect, TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( _L("%d."), aPrefix );
	iConsole->Write( aText );
	iConsole->Printf( _L("[%u]\n"), aSelect );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplaySelection
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplaySelection(
		TInt aPrefix, const TDesC& aText, TInt aSelect, TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( _L("%d."), aPrefix );
	iConsole->Write( aText );
	iConsole->Printf( _L("[%d]\n"), aSelect );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayPrompt
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayPrompt( TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( KPrompt() );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayPrompt
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayPrompt( const TDesC& aText, TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( KPrompt() );
	iConsole->Write( aText );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayOne
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayOne( TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( KOne() );
	iConsole->Printf( KLineBreak() );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayTwo
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayTwo( TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( KTwo() );
	iConsole->Printf( KLineBreak() );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayThree
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayThree( TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( KThree() );
	iConsole->Printf( KLineBreak() );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DisplayFour
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DisplayFour( TBool aAddPreLinebreak )
	{
	if ( aAddPreLinebreak )
		{
		iConsole->Printf( KLineBreak() );
		}
	
	iConsole->Printf( KFour() );
	iConsole->Printf( KLineBreak() );
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::DoRead
// -----------------------------------------------------------------------------
// 
void CNSPTestConsoleApp::DoRead()
    {
    iConsole->Read( iStatus );
    
    if ( !IsActive() )
    	{
    	SetActive();
    	}
    }

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::GetStringFromConsole
// -----------------------------------------------------------------------------
// 
TKeyCode CNSPTestConsoleApp::GetStringFromConsoleL( TDes& aBuffer )
	{
    TKeyCode input = EKeyNull;
    const TInt startPos = iConsole->WhereX();
	iConsole->Write( aBuffer );
	
    do  {
    	input = iConsole->Getch();
    	
    	if ( ( EKeyBackspace == input || EKeyDelete == input ) &&
    		 startPos < iConsole->WhereX() ) // Backspace & Delete
            {
            iConsole->SetPos( iConsole->WhereX() - 1 );
            iConsole->ClearToEndOfLine();
            
            aBuffer.SetLength(  0 < aBuffer.Length() ?
            					aBuffer.Length() - 1 :
            					aBuffer.Length() );
            }
    	else{
    		TChar chr( input );
    		
    		if ( chr.IsPrint() )
    			{
    			aBuffer.Append( chr );
    			iConsole->Printf( _L("%c"), input );
    			}
    		}
    	}
    while ( EKeyEnter != input && EKeyEscape != input );
    
    return input;
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::GetTUint32FromConsole
// -----------------------------------------------------------------------------
// 
TKeyCode CNSPTestConsoleApp::GetTUint32FromConsoleL( TUint32& aTUint32 )
	{
	TBuf<255> buffer;
	buffer.AppendNum( aTUint32, EDecimal );
	TKeyCode input = GetStringFromConsoleL( buffer );
	TLex lex( buffer );
	User::LeaveIfError( lex.Val( aTUint32, EDecimal ) );
	return input;
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::Cenrep
// -----------------------------------------------------------------------------
// 
CRepository& CNSPTestConsoleApp::Cenrep()
	{
	return *iRepository;
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::Manager
// -----------------------------------------------------------------------------
// 
CNSPTestManager& CNSPTestConsoleApp::Manager()
	{
	return *iTestManager;
	}

// -----------------------------------------------------------------------------
// CNSPTestConsoleApp::Console
// -----------------------------------------------------------------------------
// 
CConsoleBase& CNSPTestConsoleApp::Console()
	{
	return *iConsole;
	}
