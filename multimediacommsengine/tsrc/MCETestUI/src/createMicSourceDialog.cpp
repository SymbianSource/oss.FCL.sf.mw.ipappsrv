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
#include "createMicSourceDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineSource.h"
#include <MCEAudioStream.h>
#include <MCEMicSource.h>
#include <MCEMediaSource.h>

// CONSTANTS

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateMicSourceDialog::CCreateMicSourceDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateMicSourceDialog::CCreateMicSourceDialog(
						 CMCETestUIEngineAudioStream& aStream, TInt aIndex ) :
    iStream(aStream),iSourceIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateMicSourceDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateMicSourceDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();
   
   	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    CMceMicSource& micSource = static_cast<CMceMicSource&>(mediaSource);
    	
    CEikNumberEditor* gain = static_cast<CEikNumberEditor*>(
                                  Control( ERtpSourceGainSetting ) );
    gain->SetNumber(micSource.GainL()); 
      
    }

// ----------------------------------------------------------------------------
// CCreateMicSourceDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateMicSourceDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();
            
    if(sources[iSourceIndex]->Type() == KMceMicSource)
    	{
    	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    	CMceMicSource& micSource = static_cast<CMceMicSource&>(mediaSource);
    	
    	CEikNumberEditor* gain = static_cast<CEikNumberEditor*>(Control( ERtpSourceGainSetting ) );
    	micSource.SetGainL( gain->Number() );  
        }
    
        
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateMicSourceDialog::~CCreateMicSourceDialog)
// .
// ----------------------------------------------------------------------------
//
CCreateMicSourceDialog::~CCreateMicSourceDialog()
    {
    return;
    } 

// End of File
