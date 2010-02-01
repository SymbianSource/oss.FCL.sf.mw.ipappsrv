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
#include "createSpeakerSinkDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineSink.h"
#include <MCEAudioStream.h>
#include <MCESpeakerSink.h>
#include <MCEMediaSink.h>
// CONSTANTS

const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateSpeakerSinkDialog::CCreateSpeakerSinkDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateSpeakerSinkDialog::CCreateSpeakerSinkDialog(
						 CMCETestUIEngineAudioStream& aStream, TInt aIndex ) :
    iStream(aStream),iSinkIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateSpeakerSinkDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateSpeakerSinkDialog::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    
    const RPointerArray<CMCETestUIEngineSink>& sinks = 
            iStream.SinksL();
   	CMceMediaSink& mediaSink = static_cast<CMceMediaSink&>(sinks[iSinkIndex]->Sink());
    	CMceSpeakerSink& speakerSink = static_cast<CMceSpeakerSink&>(mediaSink);
    	
    CEikNumberEditor* volume = static_cast<CEikNumberEditor*>(
                                  Control( ESpeakerSinkVolumeSetting ) );
    volume->SetNumber(speakerSink.VolumeL());   
 //   volume->SetNumber(50);   
 
 	CEikNumberEditor* routing = static_cast<CEikNumberEditor*>(
                                  Control( ESpeakerSinkRoutingSetting ) );
    routing->SetNumber(speakerSink.RoutingL());   
    
    
    }

// ----------------------------------------------------------------------------
// CCreateSpeakerSinkDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateSpeakerSinkDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineSink>& sinks = 
            iStream.SinksL();
            
   	CMceMediaSink& mediaSink = static_cast<CMceMediaSink&>(sinks[iSinkIndex]->Sink());
    CMceSpeakerSink& speakerSink = static_cast<CMceSpeakerSink&>(mediaSink);
    	
    CEikNumberEditor* volume = static_cast<CEikNumberEditor*>(
                              Control( ESpeakerSinkVolumeSetting ) );
    speakerSink.SetVolumeL( volume->Number() );  
        
    CEikNumberEditor* routing = static_cast<CEikNumberEditor*>(
                              Control( ESpeakerSinkRoutingSetting ) );
    speakerSink.SetRoutingL( routing->Number() );  
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreateSpeakerSinkDialog::~CCreateRtpSinkDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateSpeakerSinkDialog::~CCreateSpeakerSinkDialog()
    {
    return;
    } 

// End of File
