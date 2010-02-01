/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation.
*
*/



#include "TMCETestUIEngineCmdDeleteAudioCodec.h"
#include "CMceTestUIEngineVideoCodec.h"
#include "CMceTestUIEngineCodec.h"
#include "cmcetestuiengineaudiostream.h"
#include <mceaudiostream.h>
#include <mceaudiocodec.h>
#include <mceamrcodec.h>
#include <mceg711codec.h>
#include <mceg729codec.h>
#include <mceilbccodec.h>
#include <mcedtmfcodec.h>
#include <mceredcodec.h>
#include <mcecncodec.h>
#include <mcedefs.h>
#include <mcemanager.h>
#include <mcecodec.h>
#include <mcevideocodec.h>

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteAudioCodec::TMCETestUIEngineCmdDeleteAudioCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDeleteAudioCodec::TMCETestUIEngineCmdDeleteAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDeleteAudioCodec::ExecuteL()
	{
	iEngine.DeleteAudioCodec( iCodec );
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDeleteAudioCodec::Caption() const
	{
	return KCommandCaptionDeleteAudioCodec;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteVideoCodec::TMCETestUIEngineCmdDeleteVideoCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDeleteVideoCodec::TMCETestUIEngineCmdDeleteVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteVideoCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDeleteVideoCodec::ExecuteL()
	{
	iEngine.DeleteVideoCodec( iCodec );
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDeleteSession::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDeleteVideoCodec::Caption() const
	{
	return KCommandCaptionDeleteVideoCodec;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAddCodec::TMCETestUIEngineCmdAddCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdAddCodec::TMCETestUIEngineCmdAddCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineAudioStream& aAudioStream  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iAudioStream( aAudioStream )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAddCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdAddCodec::ExecuteL()
	{
	const RPointerArray<const CMceAudioCodec>& codecs =  iEngine.MCEManager().SupportedAudioCodecs();
	TInt codecIndex = 0;
	iEngine.GetCodecTypeL( codecIndex  );
	switch(codecIndex)
		{
			case 0:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for AMR codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNameAMR )  == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 1:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for PCMU codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNamePCMU ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 2:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for PCMU codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNamePCMA ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 3:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for G729 codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNameG729 ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 4:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for iLBC codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNameiLBC ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 5:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for iLBC codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNameDtmf ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 6:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for iLBC codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNameRed ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			case 7:
				{
				for( int i = 0; i < codecs.Count(); i++ )
			    {
			   			    
			    // Search for iLBC codec 
				    if( codecs[i]->SdpName().Find( KMceSDPNameCn ) == KErrNone)
				        {
				    	iAudioStream.AudioStream().AddCodecL(codecs[i]->CloneL());
				    	}
			    }
				break;
				}
			default:
			   break;
		}
	 iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdAddCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdAddCodec::Caption() const
	{
	return KCommandCaptionAddCodec;
	}

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableAudioCodec::TMCETestUIEngineCmdEnableAudioCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableAudioCodec::TMCETestUIEngineCmdEnableAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableAudioCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableAudioCodec::ExecuteL()
	{
	iCodec.Codec().SetStateL(CMceCodec::EEnabled);
	iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableAudioCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableAudioCodec::Caption() const
	{
	return KCommandCaptionEnableVideoCodec;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableAudioCodec::TMCETestUIEngineCmdDisableAudioCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableAudioCodec::TMCETestUIEngineCmdDisableAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableAudioCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableAudioCodec::ExecuteL()
	{
	iCodec.Codec().SetStateL(CMceCodec::EDisabled);
	iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableAudioCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableAudioCodec::Caption() const
	{
	return KCommandCaptionDisableVideoCodec;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStandByAudioCodec::TMCETestUIEngineCmdStandByAudioCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdStandByAudioCodec::TMCETestUIEngineCmdStandByAudioCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStandByAudioCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdStandByAudioCodec::ExecuteL()
	{
	iCodec.Codec().SetStateL(CMceCodec::EStandby);
	iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStandByAudioCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdStandByAudioCodec::Caption() const
	{
	return KCommandCaptionStandByVideoCodec;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoCodec::TMCETestUIEngineCmdEnableVideoCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdEnableVideoCodec::TMCETestUIEngineCmdEnableVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdEnableVideoCodec::ExecuteL()
	{
	iCodec.Codec().SetStateL(CMceCodec::EEnabled);
	iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdEnableVideoCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdEnableVideoCodec::Caption() const
	{
	return KCommandCaptionEnableVideoCodec;
	}


// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoCodec::TMCETestUIEngineCmdDisableVideoCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdDisableVideoCodec::TMCETestUIEngineCmdDisableVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdDisableVideoCodec::ExecuteL()
	{
	iCodec.Codec().SetStateL(CMceCodec::EDisabled);
	iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdDisableVideoCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdDisableVideoCodec::Caption() const
	{
	return KCommandCaptionDisableVideoCodec;
	}
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStandByVideoCodec::TMCETestUIEngineCmdStandByVideoCodec
// -----------------------------------------------------------------------------
//
TMCETestUIEngineCmdStandByVideoCodec::TMCETestUIEngineCmdStandByVideoCodec( 
		            CMCETestUIEngine& aEngine,
		            CMCETestUIEngineVideoCodec& aCodec  )
    : TMCETestUIEngineCmdBase( aEngine ),
      iCodec( aCodec )
    {
    }

// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStandByVideoCodec::ExecuteL
// -----------------------------------------------------------------------------
//
EXPORT_C void TMCETestUIEngineCmdStandByVideoCodec::ExecuteL()
	{
	iCodec.Codec().SetStateL(CMceCodec::EStandby);
	iEngine.EngineStateChangedL();
	}
	
// -----------------------------------------------------------------------------
// TMCETestUIEngineCmdStandByVideoCodec::Caption
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& TMCETestUIEngineCmdStandByVideoCodec::Caption() const
	{
	return KCommandCaptionStandByVideoCodec;
	}



// End of File
