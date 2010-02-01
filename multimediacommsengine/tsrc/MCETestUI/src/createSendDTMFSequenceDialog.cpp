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
#include "createSendDTMFSequenceDialog.h"
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
CSendDTMFSequenceDialog::CSendDTMFSequenceDialog( TDes& aSequence ) :
    iSequence( aSequence )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CSendDTMFSequenceDialog::PreLayoutDynInitL()
    {
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CSendDTMFSequenceDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CEikEdwin* sourceFile = static_cast<CEikEdwin*>(
                                 Control( ESendDtmfToneSequence ) );
    
    sourceFile->GetText( iSequence );
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CStartDTMFDialog::~CStartDTMFDialog()
// .
// ----------------------------------------------------------------------------
//
CSendDTMFSequenceDialog::~CSendDTMFSequenceDialog()
    {
    return;
    } 

// End of File
