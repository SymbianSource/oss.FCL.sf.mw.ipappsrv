/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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

// INCLUDE FILES

#include "CMCETestUIStreamViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineSession.h"
#include "MMCETestUIEngineCmdProvider.h"
#include "CMCETestUIEngineSource.h"
#include "CMCETestUIEngineSink.h"
#include "CMCETestUIEngineCodec.h"
#include <MCEAudioCodec.h>
#include <MCEAudioStream.h>
#include "CMCETestUIEngineVideoSource.h"
#include "CMCETestUIEngineVideoSink.h"
#include "CMCETestUIEngineVideoCodec.h"
#include <MCEAudioCodec.h>
#include <MCEAudioStream.h>
#include <MCEMediaSource.h>
#include <MCEMicSource.h>
#include <MCEMediaSink.h>
#include <MCERtpSink.h>
#include <MCERtpSource.h>
#include <MCESpeakerSink.h>
#include <MCEDisplaySink.h>
#include <MCECameraSource.h>
#include <MCEFileSource.h>
#include <MCEfileSink.h>

#include "mcetestuiengineconstants.h"
#include "cmcetestuiengineaudiostream.h"
#include "cmcetestuienginevideostream.h"

// CONSTANTS


// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIStreamViewModel* CMCETestUIStreamViewModel::NewL(
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineAudioStream& aStream )
    {
	CMCETestUIStreamViewModel* self = 
	    new(ELeave) CMCETestUIStreamViewModel( aEngine, aStream );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::CMCETestUISessionViewModel
// -----------------------------------------------------------------------------
//
CMCETestUIStreamViewModel::CMCETestUIStreamViewModel(
        CMCETestUIEngine& aEngine ,
        CMCETestUIEngineAudioStream& aStream )
    :iEngine( aEngine ),
     iStream( aStream),
     audioSourceSuppressed( EFalse ),
     audioSinksSuppressed( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewModel::ConstructL()
    {
    iArray = new (ELeave) CDesCArrayFlat( 1 );

    PopulateSourcesL();
    PopulateSinksL();
    PopulateCodecsL();
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::~CMCETestUIStreamViewModel()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIStreamViewModel::~CMCETestUIStreamViewModel()
    {
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::MdcaCount() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIStreamViewModel::MdcaCount() const
    {
    return iArray->MdcaCount();
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::MdcaPoint() const
// 
// -----------------------------------------------------------------------------
//
TPtrC CMCETestUIStreamViewModel::MdcaPoint( TInt aIndex ) const
    {
    return iArray->MdcaPoint( aIndex );
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::IsAudioSourceSelected() const
// 
// -----------------------------------------------------------------------------
//
TBool CMCETestUIStreamViewModel::IsAudioSourceSelected( TInt aIndex ) const
    {
    if ( aIndex == 0 && !audioSourceSuppressed )
        {
        return ETrue;
        }
    
    return EFalse;    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIMainViewModel::SelectedAudioSinkIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIStreamViewModel::SelectedAudioSinkIndex( TInt aIndex ) const
    {

    if ( audioSourceSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0 )
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the source

    const RPointerArray<CMCETestUIEngineSink>& sinks = 
                iStream.SinksL();

    if ( audioSinksSuppressed )
        {
        aIndex += sinks.Count();
        }

    if ( aIndex < sinks.Count() )
        {
        return aIndex;
        }
    
    return KErrNotFound;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::SelectedAudioCodecIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIStreamViewModel::SelectedAudioCodecIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineSink>& sinks = 
                iStream.SinksL();

    const RPointerArray<CMCETestUIEngineCodec>& codecs = 
                iStream.CodecsL();               
                
    
    // Next does not work correctly
                    
    if ( audioSourceSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the audio source
    
    if ( audioSinksSuppressed )
        {
        aIndex += sinks.Count();
        }

    if ( aIndex < sinks.Count() )
        {
        return KErrNotFound;
        }
    
    aIndex -= sinks.Count(); // Selected is not a audio sink
    
    if ( aIndex < codecs.Count()  )
        {
        return aIndex;
        }
        
    return KErrNotFound;   // default      
    }
    


// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::CmdProvider()
// 
// -----------------------------------------------------------------------------
//
MMCETestUIEngineCmdProvider& CMCETestUIStreamViewModel::CmdProvider( 
        TInt aIndex )
    {
    const RPointerArray<CMCETestUIEngineSource>& sources = 
                iStream.SourcesL();
    
    const RPointerArray<CMCETestUIEngineSink>& sinks =
                iStream.SinksL();
                
    const RPointerArray<CMCETestUIEngineCodec>& codecs =
                iStream.CodecsL();                        
    
    // Next does not work correctly
                    
    if ( audioSourceSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return *(sources[aIndex]);
        }

    aIndex -= 1; // Selected is not the source
    
    if ( audioSinksSuppressed )
        {
        aIndex += sinks.Count();
        }

    if ( aIndex < sinks.Count() )
        {
        return *(sinks[aIndex]);
        }
    
    aIndex -= sinks.Count(); // Selected is not a sink
    if ( aIndex < codecs.Count() )
        {
        return *(codecs[aIndex]);
        }
    
    return iEngine;   // default         
    }


// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::PopulateStreamsL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
void CMCETestUIStreamViewModel::PopulateSourcesL()
    {
    const RPointerArray<CMCETestUIEngineSource>& sources = 
            iStream.SourcesL();

    for ( TInt i = 0; i < sources.Count(); ++i )
        {
                              
        TInt itemLength = KSource().Length() + 
                          KLeftParenthesis().Length() +
                      //    sources[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() +
                          sources[i]->State().Length() +
                          KTab().Length() * 3;
                          
        if(sources[i]->Type() == KMceRTPSource ) 
        	{
        	itemLength += KLabelRTPSource().Length();
        	}
         
        
        else if(sources[i]->Type() == KMceMicSource )         
        	{
        	itemLength += KLabelMicSource().Length();
        	}
        	
        else if(sources[i]->Type() == KMceFileSource )         
        	{
        	itemLength += KLabelFileSource().Length();
        	}
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KSource );
        itemPtr.Append( KLeftParenthesis() );
        if(sources[i]->Type() == KMceRTPSource )     
        	{
        	itemPtr.Append(KLabelRTPSource);	
        	}
         
        else if(sources[i]->Type() == KMceMicSource )         
        	{
        	itemPtr.Append(KLabelMicSource);	
        	}
        else if(sources[i]->Type() == KMceFileSource )         
        	{
        	itemPtr.Append(KLabelFileSource);	
        	}
        
       // itemPtr.Append( sources[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( sources[i]->State() );
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::PopulateSinksL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
    
void CMCETestUIStreamViewModel::PopulateSinksL()
    {
    const RPointerArray<CMCETestUIEngineSink>& sinks = 
            iStream.SinksL();

    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
                              
        TInt itemLength = KSink().Length() + 
                          KLeftParenthesis().Length() +
                     //     sinks[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() +
                          sinks[i]->State().Length() +
                          KTab().Length() * 3;
                          
        if(sinks[i]->Type() == KMceRTPSink ) 
        	{
        	itemLength += KLabelRTPSink().Length();	
        	}
         
        else  if(sinks[i]->Type() == KMceSpeakerSink)         
          	{
         	itemLength += KLabelSpeakerSink().Length();
         	}
        
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KSink );
        itemPtr.Append( KLeftParenthesis() );
        
        if(sinks[i]->Type() == KMceRTPSink ) 
        	{
        	itemPtr.Append(KLabelRTPSink);
        	}
        else if(sinks[i]->Type() == KMceSpeakerSink ) 
        	{
        	itemPtr.Append(KLabelSpeakerSink);
        	}
         
    //    itemPtr.Append( sinks[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( sinks[i]->State() );
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::PopulateStreamsL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
    
void CMCETestUIStreamViewModel::PopulateCodecsL()
    {
    const RPointerArray<CMCETestUIEngineCodec>& codecs = 
            iStream.CodecsL();

    for ( TInt i = 0; i < codecs.Count(); ++i )
        {
        TBuf8<KMceMaxSdpNameLength> name = codecs[i]->Codec().SdpName();
                              
        TInt itemLength = KCodec().Length() + 
                          KLeftParenthesis().Length() +
                   	      name.Length()+
                          KRightParenthesis().Length() +
                          codecs[i]->State().Length() +
                          KTab().Length() * 3;
      
        TBuf16<KMceMaxSdpNameLength>  name16;
        name16.Copy(name);
        
                      
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KCodec );
        itemPtr.Append( KLeftParenthesis() );
        itemPtr.Append( name16 ) ;
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( codecs[i]->State() );
        itemPtr.Append( KTab );
        iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIVideoStreamViewModel* CMCETestUIVideoStreamViewModel::NewL(
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineVideoStream& aStream )
    {
	CMCETestUIVideoStreamViewModel* self = 
	    new(ELeave) CMCETestUIVideoStreamViewModel( aEngine, aStream );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::CMCETestUIVideoStreamViewModel
// -----------------------------------------------------------------------------
//
CMCETestUIVideoStreamViewModel::CMCETestUIVideoStreamViewModel(
        CMCETestUIEngine& aEngine ,
        CMCETestUIEngineVideoStream& aStream )
    :iEngine( aEngine ),
     iStream( aStream),
     videoSourceSuppressed( EFalse ),
     videoSinksSuppressed( EFalse )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewModel::ConstructL()
    {
    iArray = new (ELeave) CDesCArrayFlat( 1 );

    PopulateVideoSourcesL();
    PopulateVideoSinksL();
    PopulateVideoCodecsL();
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::~CMCETestUIVideoStreamViewModel()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIVideoStreamViewModel::~CMCETestUIVideoStreamViewModel()
    {
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::MdcaCount() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIVideoStreamViewModel::MdcaCount() const
    {
    return iArray->MdcaCount();
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::MdcaPoint() const
// 
// -----------------------------------------------------------------------------
//
TPtrC CMCETestUIVideoStreamViewModel::MdcaPoint( TInt aIndex ) const
    {
    return iArray->MdcaPoint( aIndex );
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::IsVideoSourceSelected() const
// 
// -----------------------------------------------------------------------------
//
TBool CMCETestUIVideoStreamViewModel::IsVideoSourceSelected( TInt aIndex ) const
    {
    if ( aIndex == 0 && !videoSourceSuppressed )
        {
        return ETrue;
        }
    
    return EFalse;    
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::SelectedVideoSinkIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIVideoStreamViewModel::SelectedVideoSinkIndex( TInt aIndex ) const
    {

    if ( videoSourceSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0 )
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the source

    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = 
                iStream.SinksL();

    if ( videoSinksSuppressed )
        {
        aIndex += sinks.Count();
        }

    if ( aIndex < sinks.Count() )
        {
        return aIndex;
        }
    
    return KErrNotFound;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::SelectedVideoCodecIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIVideoStreamViewModel::SelectedVideoCodecIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = 
                iStream.SinksL();

    const RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
                iStream.CodecsL();               
                
    
    // Next does not work correctly
                    
    if ( videoSourceSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return KErrNotFound;
        }

    aIndex -= 1; // Selected is not the audio source
    
    if ( videoSinksSuppressed )
        {
        aIndex += sinks.Count();
        }

    if ( aIndex < sinks.Count() )
        {
        return KErrNotFound;
        }
    
    aIndex -= sinks.Count(); // Selected is not a audio sink
    
    if ( aIndex < codecs.Count()  )
        {
        return aIndex;
        }
        
    return KErrNotFound;   // default      
    }
    


// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::CmdProvider()
// 
// -----------------------------------------------------------------------------
//
MMCETestUIEngineCmdProvider& CMCETestUIVideoStreamViewModel::CmdProvider( 
        TInt aIndex )
    {
    const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
                iStream.SourcesL();
    
    const RPointerArray<CMCETestUIEngineVideoSink>& sinks =
                iStream.SinksL();
                
    const RPointerArray<CMCETestUIEngineVideoCodec>& codecs =
                iStream.CodecsL();                        
    
    // Next does not work correctly
                    
    if ( videoSourceSuppressed )
        {
        ++aIndex;
        }

    if ( aIndex == 0)
        {
        return *(sources[aIndex]);
        }

    aIndex -= 1; // Selected is not the source
    
    if ( videoSinksSuppressed )
        {
        aIndex += sinks.Count();
        }

    if ( aIndex < sinks.Count() )
        {
        return *(sinks[aIndex]);
        }
    
    aIndex -= sinks.Count(); // Selected is not a sink
    if ( aIndex < codecs.Count() )
        {
        return *(codecs[aIndex]);
        }
    
    return iEngine;   // default         
    }

// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::PopulateVideoStreamsL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewModel::PopulateVideoSourcesL()
    {
    const RPointerArray<CMCETestUIEngineVideoSource>& sources = 
            iStream.SourcesL();

    for ( TInt i = 0; i < sources.Count(); ++i )
        {
                              
        TInt itemLength = KSource().Length() + 
                          KLeftParenthesis().Length() +
                      //    sources[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() +
                          sources[i]->State().Length() +
                          KTab().Length() * 3;
                          
        if(sources[i]->Type() == KMceRTPSource ) 
        	{
        	itemLength += KLabelRTPSource().Length();
        	}
         
        
        else if(sources[i]->Type() == KMceCameraSource )         
        	{
        	itemLength += KLabelCamSource().Length();
        	}
        	
        else if(sources[i]->Type() == KMceFileSource )         
        	{
        	itemLength += KLabelFileSource().Length();
        	}
        
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KSource );
        itemPtr.Append( KLeftParenthesis() );
        if(sources[i]->Type() == KMceRTPSource )     
        	{
        	itemPtr.Append(KLabelRTPSource);	
        	}
         
        else if(sources[i]->Type() == KMceCameraSource )         
        	{
        	itemPtr.Append(KLabelCamSource);	
        	}
         
        else if(sources[i]->Type() == KMceFileSource )         
        	{
        	itemPtr.Append(KLabelFileSource);	
        	}
       // itemPtr.Append( sources[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( sources[i]->State() );
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIStreamViewModel::PopulateVideoSinksL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewModel::PopulateVideoSinksL()
    {
    const RPointerArray<CMCETestUIEngineVideoSink>& sinks = 
            iStream.SinksL();

    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
                              
        TInt itemLength = KSink().Length() + 
                          KLeftParenthesis().Length() +
                     //     sinks[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() +
                          sinks[i]->State().Length() +
                          KTab().Length() * 3;
                          
        if(sinks[i]->Type() == KMceRTPSink ) 
        	{
        	itemLength += KLabelRTPSink().Length();	
        	}
         
        else  if(sinks[i]->Type() == KMceDisplaySink)         
          	{
         	itemLength += KLabelDisplaySink().Length();
         	}
        else  if(sinks[i]->Type() == KMceFileSink)         
          	{
         	itemLength += KLabelFileSink().Length();
         	}
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KSink );
        itemPtr.Append( KLeftParenthesis() );
        
        if(sinks[i]->Type() == KMceRTPSink ) 
        	{
        	itemPtr.Append(KLabelRTPSink);
        	}
        else if(sinks[i]->Type() == KMceDisplaySink ) 
        	{
        	itemPtr.Append(KLabelDisplaySink);
        	}
        else if(sinks[i]->Type() == KMceFileSink ) 
        	{
        	itemPtr.Append(KLabelFileSink);
        	}
    //    itemPtr.Append( sinks[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( sinks[i]->State() );
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIVideoStreamViewModel::PopulateVideoStreamsL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
void CMCETestUIVideoStreamViewModel::PopulateVideoCodecsL()
    {
    const RPointerArray<CMCETestUIEngineVideoCodec>& codecs = 
            iStream.CodecsL();

    for ( TInt i = 0; i < codecs.Count(); ++i )
        {
        TBuf8<KMceMaxSdpNameLength> name = codecs[i]->Codec().SdpName();
                              
        TInt itemLength = KCodec().Length() + 
                          KLeftParenthesis().Length() +
                   	      name.Length()+
                          KRightParenthesis().Length() +
                          codecs[i]->State().Length() +
                          KTab().Length() * 3;
      
        TBuf16<KMceMaxSdpNameLength>  name16;
        name16.Copy(name);
        
                      
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KCodec );
        itemPtr.Append( KLeftParenthesis() );
        itemPtr.Append( name16 ) ;
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( codecs[i]->State() );
        itemPtr.Append( KTab );
        iArray->AppendL( *item );
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }

// End of File  
