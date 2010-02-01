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

#include "CMCETestUISessionViewModel.h"
#include "CMCETestUIEngine.h"
#include "CMCETestUIEngineSession.h"
#include "MMCETestUIEngineCmdProvider.h"
#include "CMceTestUIEngineAudioStream.h"
#include "CMceTestUIEngineVideoStream.h"
#include "mcetestuiengineconstants.h"

// CONSTANTS


// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::NewL
// -----------------------------------------------------------------------------
//
CMCETestUISessionViewModel* CMCETestUISessionViewModel::NewL(
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineSession& aSession )
    {
	CMCETestUISessionViewModel* self = 
	    new(ELeave) CMCETestUISessionViewModel( aEngine, aSession );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;    
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::CMCETestUISessionViewModel
// -----------------------------------------------------------------------------
//
CMCETestUISessionViewModel::CMCETestUISessionViewModel(
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineSession& aSession )
    :iEngine( aEngine ),
     iSession( aSession ),
     audioStreamsSuppressed(EFalse)
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewModel::ConstructL()
    {
    iArray = new (ELeave) CDesCArrayFlat( 1 );

    PopulateStreamsL();
    PopulateVideoStreamsL();
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::~CMCETestUISessionViewModel()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUISessionViewModel::~CMCETestUISessionViewModel()
    {
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::MdcaCount() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUISessionViewModel::MdcaCount() const
    {
    return iArray->MdcaCount();
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::MdcaPoint() const
// 
// -----------------------------------------------------------------------------
//
TPtrC CMCETestUISessionViewModel::MdcaPoint( TInt aIndex ) const
    {
    return iArray->MdcaPoint( aIndex );
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::CmdProvider()
// 
// -----------------------------------------------------------------------------
//
MMCETestUIEngineCmdProvider& CMCETestUISessionViewModel::CmdProvider( 
        TInt aIndex )
    {
    const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iSession.AudioStreamsL();
            
    const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iSession.VideoStreamsL();
    
    
    if ( audioStreamsSuppressed )
        {
        aIndex += audioStreams.Count();
        }

    if ( aIndex < audioStreams.Count() )
        {
        return *(audioStreams[aIndex]);
        }
    
    aIndex -= audioStreams.Count(); // Selected is not a audio stream
    
    if ( aIndex < videoStreams.Count() )
        {
        return *(videoStreams[aIndex]);
        }        
/*    if ( aIndex < audioStreams.Count() )
        {
        return *(audioStreams[aIndex]);
        }
  */      
    return iEngine;   // default         
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::SelectedAudioStreamIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUISessionViewModel::SelectedAudioStreamIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iSession.AudioStreamsL();
            
    if ( audioStreamsSuppressed )
        {
        aIndex += audioStreams.Count();
        }

    if ( aIndex < audioStreams.Count() )
        {
        return aIndex;
        }
    
    return KErrNotFound;    
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::SelectedVideoStreamIndex() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUISessionViewModel::SelectedVideoStreamIndex( TInt aIndex ) const
    {

    const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iSession.AudioStreamsL();

    const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iSession.VideoStreamsL();           
                
    
    // Next does not work correctly
                    
    
    if ( audioStreamsSuppressed )
        {
        aIndex += audioStreams.Count();
        }

    if ( aIndex < audioStreams.Count() )
        {
        return KErrNotFound;
        }
    
    aIndex -= audioStreams.Count(); // Selected is not a audio sink
    
    if ( aIndex < videoStreams.Count()  )
        {
        return aIndex;
        }
        
    return KErrNotFound;   // default      
    }
    

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::PopulateStreamsL
// Populate streams of a session
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewModel::PopulateStreamsL()
    {
    const RPointerArray<CMCETestUIEngineAudioStream>& audioStreams = 
            iSession.AudioStreamsL();

    for ( TInt i = 0; i < audioStreams.Count(); ++i )
        {
                              
        TInt itemLength = KAudioStream().Length() + 
                          KLeftParenthesis().Length() +
                          audioStreams[i]->TextualDirection().Length() +
                          KRightParenthesis().Length() +
                          audioStreams[i]->State().Length() +
                          KLeftParenthesis().Length() +
                          audioStreams[i]->StreamState().Length() +
                          KRightParenthesis().Length() +
                          KTab().Length() * 3;
                      
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KAudioStream );
        itemPtr.Append( KLeftParenthesis() );
        itemPtr.Append( audioStreams[i]->TextualDirection() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( audioStreams[i]->State() );
        itemPtr.Append( KLeftParenthesis() );
        itemPtr.Append( audioStreams[i]->StreamState() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }
// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::PopulateVideoStreamsL
// Populate videostreams of a session
// -----------------------------------------------------------------------------
//
void CMCETestUISessionViewModel::PopulateVideoStreamsL()
    {
    const RPointerArray<CMCETestUIEngineVideoStream>& videoStreams = 
            iSession.VideoStreamsL();

    for ( TInt i = 0; i < videoStreams.Count(); ++i )
        {
                              
        TInt itemLength = KVideoStream().Length() + 
                          KLeftParenthesis().Length() +
                          videoStreams[i]->TextualVideoDirection().Length() +
                          KRightParenthesis().Length() +
                          videoStreams[i]->State().Length() +
                          KLeftParenthesis().Length() +
                          videoStreams[i]->StreamState().Length() +
                          KRightParenthesis().Length() +
                          KTab().Length() * 3;
                      
        HBufC16* item = HBufC16::NewLC( itemLength );
        TPtr16 itemPtr = item->Des();
        itemPtr.Append( KTab );
        itemPtr.Append( KVideoStream );
        itemPtr.Append( KLeftParenthesis() );
        itemPtr.Append( videoStreams[i]->TextualVideoDirection() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
        itemPtr.Append( videoStreams[i]->State() );
        itemPtr.Append( KLeftParenthesis() );
        itemPtr.Append( videoStreams[i]->StreamState() );
        itemPtr.Append( KRightParenthesis() );
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }

// End of File  
