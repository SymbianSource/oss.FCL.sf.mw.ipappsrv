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

#include "CMCETestUIEventViewModel.h"
#include "CMCETestUIEngine.h"
#include "MMCETestUIEngineCmdProvider.h"

// CONSTANTS

_LIT( KTab, "\t" );
_LIT( KLeftParenthesis, "( " );
_LIT( KRightParenthesis, " )" );

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CMCETestUIEventViewModel::NewL
// -----------------------------------------------------------------------------
//
CMCETestUIEventViewModel* CMCETestUIEventViewModel::NewL(
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineEvent& aEvent )
    {
	CMCETestUIEventViewModel* self = 
	    new(ELeave) CMCETestUIEventViewModel( aEngine, aEvent );
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;    
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewModel::CMCETestUIEventViewModel
// -----------------------------------------------------------------------------
//
CMCETestUIEventViewModel::CMCETestUIEventViewModel(
        CMCETestUIEngine& aEngine,
        CMCETestUIEngineEvent& aEvent )
    :iEngine( aEngine ),
     iEvent( aEvent )
    {
    }
    
// -----------------------------------------------------------------------------
// CMCETestUIEventViewModel::ConstructL()
// -----------------------------------------------------------------------------
//
void CMCETestUIEventViewModel::ConstructL()
    {
    iArray = new (ELeave) CDesCArrayFlat( 1 );

   // PopulateStreamsL();
    }

// -----------------------------------------------------------------------------
// CMCETestUISessionViewModel::~CMCETestUISessionViewModel()
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
//
CMCETestUIEventViewModel::~CMCETestUIEventViewModel()
    {
    delete iArray;
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewModel::MdcaCount() const
// 
// -----------------------------------------------------------------------------
//
TInt CMCETestUIEventViewModel::MdcaCount() const
    {
    return iArray->MdcaCount();
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewModel::MdcaPoint() const
// 
// -----------------------------------------------------------------------------
//
TPtrC CMCETestUIEventViewModel::MdcaPoint( TInt aIndex ) const
    {
    return iArray->MdcaPoint( aIndex );
    }

// -----------------------------------------------------------------------------
// CMCETestUIEventViewModel::CmdProvider()
// 
// -----------------------------------------------------------------------------
//
MMCETestUIEngineCmdProvider& CMCETestUIEventViewModel::CmdProvider( 
        TInt aIndex )
    {
    return iEngine;   // default         
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
        itemPtr.Append( KTab );
    
        iArray->AppendL( *item );
        
        CleanupStack::PopAndDestroy( item );
        item = NULL;
        }
    }

// End of File  
