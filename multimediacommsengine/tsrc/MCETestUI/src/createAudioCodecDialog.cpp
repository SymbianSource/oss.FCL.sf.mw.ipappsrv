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
#include "createAudioCodecDialog.h"
#include "CMCETestUIEngineAudioStream.h"
#include "CMCETestUIEngineCodec.h"
#include <MCEAudioCodec.h>
#include <MCEAMRCodec.h>
#include <MCEAudioStream.h>
#include "mcetestuiengineconstants.h"
#include <AknPopupFieldText.h> 

// CONSTANTS
const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateAudioCodecDialog::CCreateAudioCodecDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateAudioCodecDialog::CCreateAudioCodecDialog(
						 CMCETestUIEngineAudioStream& aStream, TInt aIndex ) :
    iStream(aStream),iCodecIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateAudioCodecDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateAudioCodecDialog::PreLayoutDynInitL()
    {
    const RPointerArray<CMCETestUIEngineCodec>& codecs = 
            iStream.CodecsL();
            
    CMceAudioCodec& audioCodec = static_cast<CMceAudioCodec&>(codecs[iCodecIndex]->Codec());
    TBool aEnableVAD = audioCodec.VAD();
   	CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EAudioCodecEnableVAD));
    if(aEnableVAD)
    	{
    	popupFieldText->SetCurrentValueIndex(1);
    	}
    else
    	{
    	popupFieldText->SetCurrentValueIndex(0);
    	}
    	
     // Set defaultBuf packet size
    CEikNumberEditor* bitrate = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecDialogBitRate ) );
    bitrate->SetNumber(codecs[iCodecIndex]->Codec().Bitrate());   
    
    CEikNumberEditor* allowedBitrate = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecDialogAllowedBitRate ) );
    allowedBitrate->SetNumber( codecs[iCodecIndex]->Codec().AllowedBitrates() );   
    
    CEikNumberEditor* ptime = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecDialogPTime ) );
    ptime->SetNumber( codecs[iCodecIndex]->Codec().PTime() );   
    
    CEikNumberEditor* maxptime = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecDialogMaxPTime ) );
    maxptime->SetNumber( codecs[iCodecIndex]->Codec().MaxPTime() );   
    
    CEikNumberEditor* codecmode = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecDialogCodecMode ) );
    codecmode->SetNumber( codecs[iCodecIndex]->Codec().CodecMode() );   
    
    CEikNumberEditor* ptype = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecDialogPayLoadType ) );
    ptype->SetNumber( codecs[iCodecIndex]->Codec().PayloadType() );  
    
    
    CEikNumberEditor* keepalivetimer = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecKeepalivetimer ) );
    keepalivetimer->SetNumber( 0 );  
    
    CEikNumberEditor* keepalivePt = static_cast<CEikNumberEditor*>(
                                  Control( EAudioCodecKeepalivePt ) );
    keepalivePt->SetNumber( 0 );  
     
    
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreateAudioCodecDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateAudioCodecDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineCodec>& codecs = 
            iStream.CodecsL();
            
    CMceAudioCodec& audioCodec = static_cast<CMceAudioCodec&>(codecs[iCodecIndex]->Codec());
    
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EAudioCodecEnableVAD));
    if(popupFieldText->CurrentValueIndex())
    	{
    	audioCodec.EnableVAD(ETrue);
    	}
    else
        {
        audioCodec.EnableVAD(EFalse);	
        }
    
    CEikNumberEditor* bitrate = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecDialogBitRate ) );
    audioCodec.SetBitrate( bitrate->Number() );  
    
     CEikNumberEditor* ptime = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecDialogPTime ) );
    audioCodec.SetPTime( ptime->Number());
    
    CEikNumberEditor* maxptime = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecDialogMaxPTime ) );
    audioCodec.SetMaxPTime( maxptime->Number());
    
    CEikNumberEditor* ptype = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecDialogPayLoadType ) );
    audioCodec.SetPayloadType( ptype->Number()) ;
    
    
    CEikNumberEditor* codecmode = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecDialogCodecMode ) );
    audioCodec.SetCodecMode( codecmode->Number());
    
    
    CEikNumberEditor* keepalive = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecKeepalivetimer ) );
    
    if( keepalive->Number() != 0 )
        {
        audioCodec.SetKeepAliveTimerL( keepalive->Number());
        }

    CEikNumberEditor* keepalivePt = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecKeepalivePt ) );
    
    if( keepalivePt->Number() != 0 )
        {
        audioCodec.SetKeepAlivePayloadTypeL( keepalivePt->Number());
        }
       
    TBuf8<KMceMaxSdpNameLength> sdpname = audioCodec.SdpName();
    
    if( sdpname == KMceCodecAMR || sdpname == KSDPNameiLBC  )
    	{
    	CMceAmrCodec& amrCodec  = ( CMceAmrCodec& ) audioCodec;
    	 CEikNumberEditor* allowedBitrate = static_cast<CEikNumberEditor*>(
                              Control( EAudioCodecDialogAllowedBitRate ) );
    	amrCodec.SetAllowedBitrates( allowedBitrate->Number());
    	}
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreatePoCSessionDialog::~CCreatePoCSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateAudioCodecDialog::~CCreateAudioCodecDialog()
    {
    return;
    } 

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CAddCodecDialog::CAddCodecDialog( )
// .
// ----------------------------------------------------------------------------
//
CAddCodecDialog::CAddCodecDialog(
						  TInt& aCodecIndex ) :
    iCodecIndex( aCodecIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CAddCodecDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CAddCodecDialog::PreLayoutDynInitL()
    {
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EMceAddCodec));   
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CAddCodecDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//
TBool CAddCodecDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    CAknPopupFieldText* popupFieldText = 
        static_cast <CAknPopupFieldText*> (Control(EMceAddCodec));
   
    iCodecIndex = popupFieldText->CurrentValueIndex();
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CAddCodecDialog::~CAddCodecDialog()
// .
// ----------------------------------------------------------------------------
//
CAddCodecDialog::~CAddCodecDialog()
    {
    return;
    } 

// End of File
