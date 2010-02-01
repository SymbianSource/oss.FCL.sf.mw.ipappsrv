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
#include "createStartDTMFDialog.h"
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
CStartDTMFDialog::CStartDTMFDialog( TChar& aChar ) :
    iChar(aChar)
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CStartDTMFDialog::PreLayoutDynInitL()
    {
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CStartDTMFDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    
    TBuf<1> buffer;
    
	CEikEdwin* dtmf_char = static_cast<CEikEdwin*>(
                             Control( EStartDtmfTone ) );
	dtmf_char->GetText( buffer );
	
	TLex lexer( buffer );
	
	iChar = lexer.Get();
   	       	  
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::~CStartDTMFDialog()
// .
// ----------------------------------------------------------------------------
//
CStartDTMFDialog::~CStartDTMFDialog()
    {
    return;
    } 

// End of File
