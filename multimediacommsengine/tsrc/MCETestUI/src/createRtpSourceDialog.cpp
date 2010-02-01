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
#include "createRtpSourceDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineSource.h"
#include <MCEAudioStream.h>
#include <MCERtpSource.h>
#include <MCEMediaSource.h>

// CONSTANTS

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateRtpSourceDialog::CCreateRtpSourceDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateRtpSourceDialog::CCreateRtpSourceDialog(
						 CMCETestUIEngineAudioStream& aStream, TInt aIndex ) :
    iStream(aStream),iSourceIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateRtpSourceDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateRtpSourceDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();
   
   	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    CMceRtpSource& micSource = static_cast<CMceRtpSource&>(mediaSource);
    	
   /* CEikNumberEditor* gain = static_cast<CEikNumberEditor*>(
                                  Control( ERtpSourceGainSetting ) );
    gain->SetNumber(micSource.GainL()); */
      
    }

// ----------------------------------------------------------------------------
// CCreateRtpSourceDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateRtpSourceDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();
            
    if(sources[iSourceIndex]->Type() == KMceRTPSource)
    	{
   /* 	CMceMediaSource& mediaSource = static_cast<CMceMediaSource&>(sources[iSourceIndex]->Source());
    	KMceRTPSource& rtpSource = static_cast<KMceRTPSource&>(mediaSource);
    	
    	CEikNumberEditor* gain = static_cast<CEikNumberEditor*>(Control( ERtpSourceGainSetting ) );
    	rtpSource.SetGainL( gain->Number() );  */
        }
    
        
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateRtpSourceDialog::~CCreateRtpSourceDialog)
// .
// ----------------------------------------------------------------------------
//
CCreateRtpSourceDialog::~CCreateRtpSourceDialog()
    {
    return;
    } 

// End of File
