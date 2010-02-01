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
#include "createVideoCodecDialog.h"
#include "CMCETestUIEngineVideoStream.h"
#include "CMCETestUIEngineVideoCodec.h"
#include "MCETestUIEngineConstants.h"
#include <MCEAudioCodec.h>
#include <MCEVideoCodec.h>
#include <MCEAMRCodec.h>
#include <MCEVideoStream.h>
// CONSTANTS
const TInt KBufLength = 100;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CCreateVideoCodecDialog::CCreateVideoCodecDialog( )
// .
// ----------------------------------------------------------------------------
//
CCreateVideoCodecDialog::CCreateVideoCodecDialog(
						 CMCETestUIEngineVideoStream& aStream, TInt aIndex ) :
    iStream(aStream),iCodecIndex( aIndex )
    {
    
    // No implementation required
    }

// ----------------------------------------------------------------------------
// CCreateVideoCodecDialog::PreLayoutDynInitL()
// .
// ----------------------------------------------------------------------------
//
void CCreateVideoCodecDialog::PreLayoutDynInitL()
    {
    const RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            iStream.CodecsL();
            
     // Set defaultBuf packet size
    CEikNumberEditor* bitrate = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogBitRate ) );
    bitrate->SetNumber(codecs[iCodecIndex]->Codec().Bitrate()/1000);   
    
    CEikNumberEditor* allowedBitrate = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogAllowedBitRate ) );
    allowedBitrate->SetNumber( codecs[iCodecIndex]->Codec().AllowedBitrates() );   
    
 /*   CEikNumberEditor* ptime = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogPTime ) );
    ptime->SetNumber( codecs[iCodecIndex]->Codec().PTime() );   
    
    CEikNumberEditor* maxptime = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogMaxPTime ) );
    maxptime->SetNumber( codecs[iCodecIndex]->Codec().MaxPTime() );   
 */   
    CEikNumberEditor* codecmode = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogCodecMode ) );
    codecmode->SetNumber( codecs[iCodecIndex]->Codec().CodecMode() );   
    
    CEikNumberEditor* ptype = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogPayLoadType ) );
    ptype->SetNumber( codecs[iCodecIndex]->Codec().PayloadType() ); 
    

    CEikNumberEditor* keepalivetimer = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecKeepalivetimer ) );
    keepalivetimer->SetNumber( 0 );  
    
    CEikNumberEditor* keepalivePt = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecKeepalivePt ) );
    keepalivePt->SetNumber( 0 );   
    
    CEikNumberEditor* encDecDevice = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogEncDecDevice ) );
    
    TUint32 defaultEncDecDeviceVal = 0;
    if ( codecs[iCodecIndex]->Codec().SdpName().Compare( KMceSDPNameH264 ) == 0 )
        {
        defaultEncDecDeviceVal = KMceTestUiAvcEncoderUid;
        }
    
    encDecDevice->SetNumber( defaultEncDecDeviceVal ); 
    
    CAknForm::PreLayoutDynInitL();
    }

// ----------------------------------------------------------------------------
// CCreateVideoCodecDialog::OkToExitL( TInt )
// .
// ----------------------------------------------------------------------------
//

TBool CCreateVideoCodecDialog::OkToExitL( TInt aKey )
    {
    if ( aKey == EEikCmdCanceled )
        {
        // Cancel pressed. Just exit.
        return ETrue;
        }
    const RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            iStream.CodecsL();
            
    CMceVideoCodec& videoCodec = static_cast<CMceVideoCodec&>(codecs[iCodecIndex]->Codec());
    
    CEikNumberEditor* bitrate = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecDialogBitRate ) );
    videoCodec.SetBitrate( bitrate->Number()*1000 );  
    
/*     CEikNumberEditor* ptime = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecDialogPTime ) );
    videoCodec.SetPTime( ptime->Number());
    
    CEikNumberEditor* maxptime = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecDialogMaxPTime ) );
    videoCodec.SetMaxPTime( maxptime->Number());
  */  
    CEikNumberEditor* ptype = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecDialogPayLoadType ) );
    videoCodec.SetPayloadType( ptype->Number()) ;
    
    
    CEikNumberEditor* codecmode = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecDialogCodecMode ) );
    videoCodec.SetCodecMode( codecmode->Number());
    
    
	CEikNumberEditor* allowedBitrate = static_cast<CEikNumberEditor*>(
	                      Control( EVideoCodecDialogAllowedBitRate ) );
	videoCodec.SetAllowedBitrates( allowedBitrate->Number());
	
	 CEikNumberEditor* keepalive = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecKeepalivetimer ) );
    
    if( keepalive->Number() != 0 )
        {
        videoCodec.SetKeepAliveTimerL( keepalive->Number());
        }

    CEikNumberEditor* keepalivePt = static_cast<CEikNumberEditor*>(
                              Control( EVideoCodecKeepalivePt ) );
    
    if( keepalivePt->Number() != 0 )
        {
        videoCodec.SetKeepAlivePayloadTypeL( keepalivePt->Number());
        }
        
        
    CEikNumberEditor* encDecDevice = static_cast<CEikNumberEditor*>(
                                  Control( EVideoCodecDialogEncDecDevice ) );   
    
    TUid encDecUid;
    encDecUid.iUid = encDecDevice->Number();                              
    TRAP_IGNORE( videoCodec.SetPreferredEncodingDecodingDeviceL( encDecUid ) )
    	
    return ETrue;
    }

// ----------------------------------------------------------------------------
// CCreatePoCSessionDialog::~CCreatePoCSessionDialog()
// .
// ----------------------------------------------------------------------------
//
CCreateVideoCodecDialog::~CCreateVideoCodecDialog()
    {
    return;
    } 

// End of File
