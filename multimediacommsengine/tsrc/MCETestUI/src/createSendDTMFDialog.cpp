/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




// INCLUDES
#include "createSendDTMFDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineSink.h"
#include <MCEAudioStream.h>
#include <MCERtpSink.h>
#include <MCEMediaSink.h>
#include <MCESpeakerSink.h>
// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CStartDTMFDialog::CCreateRtpSinkDialog( )
// .
// ----------------------------------------------------------------------------
//
CSendDTMFDialog::CSendDTMFDialog( TChar& aChar, TInt& aInterval ) :
    iChar(aChar), iInterval( aInterval )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CSendDTMFDialog::PreLayoutDynInitL()
    {
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CSendDTMFDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    TBuf<1> buffer;
    
	CEikEdwin* dtmf_char = static_cast<CEikEdwin*>(
                             Control( ESendDtmfTone ) );
	dtmf_char->GetText( buffer );
	
	TLex lexer( buffer );
	
	iChar = lexer.Get();
   	
   	CEikNumberEditor* interval = static_cast<CEikNumberEditor*>(
                                  Control( ESendDtmfTone_duration ) );
	iInterval = interval->Number();
	    
	           	  
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::~CStartDTMFDialog()
// .
// ----------------------------------------------------------------------------
//
CSendDTMFDialog::~CSendDTMFDialog()
    {
    return;
    } 

// End of File
