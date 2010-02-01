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
* Description:    
*
*/




#include "mcesrvendpoint.h"
#include "mcecommediastream.h"
#include "mcecomsession.h"
#include "mcecomendpointproxy.h"
#include "mcesrvstreamiterator.h"

#include "mcemediaobserver.h"
#include "mceevents.h"
#include "mcemmlogs.h"

// ============================ MEMBER FUNCTIONS ===============================


    
// -----------------------------------------------------------------------------
// CMceSrvEndpoint::CMceSrvEndpoint
// -----------------------------------------------------------------------------
//
CMceSrvEndpoint::CMceSrvEndpoint( CMceMediaManager& aManager,
                                  CMceComEndpoint& aData )
    : iID( KMceNotAssigned ),
      iState( CMceSrvStream::ECreated ),
      iManager( &aManager ),
      iData( &aData )
    {
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::Merge
// -----------------------------------------------------------------------------
//
void CMceSrvEndpoint::Merge( CMceSrvEndpoint& aMergeWith )
    {
    iID = aMergeWith.iID;
    iState = aMergeWith.iState;
    }
                   
    
// -----------------------------------------------------------------------------
// CMceSrvEndpoint::~CMceSrvEndpoint
// -----------------------------------------------------------------------------
//
CMceSrvEndpoint::~CMceSrvEndpoint()
    {
    //NOP
    }


// -----------------------------------------------------------------------------
// CMceSrvEndpoint::Id
// -----------------------------------------------------------------------------
//
TUint32& CMceSrvEndpoint::Id()
    {
    return iID;
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::SetState
// -----------------------------------------------------------------------------
//
void CMceSrvEndpoint::SetState( CMceSrvStream::TState aState )
    {
    iState = aState;
    }
    
// -----------------------------------------------------------------------------
// CMceSrvEndpoint::State
// -----------------------------------------------------------------------------
//
CMceSrvStream::TState CMceSrvEndpoint::State() const
    {
    return iState;
    }


// -----------------------------------------------------------------------------
// CMceSrvEndpoint::Data
// -----------------------------------------------------------------------------
//
CMceComEndpoint& CMceSrvEndpoint::Data() const
    {
    return *iData;
    }



// -----------------------------------------------------------------------------
// CMceSrvEndpoint::MccPrepareCalledL
// -----------------------------------------------------------------------------
//
void CMceSrvEndpoint::MccPrepareCalledL( CMceSrvStream& /*aStream*/ )
    {
    //NOP
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::EnableL
// -----------------------------------------------------------------------------
//

TBool CMceSrvEndpoint::EnableL( CMceSrvStream& aStream, TBool aSync )
    {
    if ( aStream.IsMccPrepared() && !Data().EndpointProxy() )
        {
        return KMceSrvStreamDefaultSequence;
        }

    MCEMM_DEBUG("CMceSrvEndpoint::EnableL(): alternative enable");
    
    TBool sequenceType = aStream.IsMccPrepared() ? 
            KMceSrvStreamDefaultSequence : 
            KMceSrvStreamAlternativeSequence;
    
    if ( State() >= CMceSrvStream::EStarted && 
         ( aSync || !Data().IsEnabled() ) )
        {
        MCEMM_DEBUG("CMceSrvEndpoint::EnableL(): enabling");
        
        if ( Data().EndpointProxy() )
            {
            sequenceType = 
                Data().EndpointProxy()->ClientEnableL( Data(), aStream.Codec() );
            }
            
        if ( !aStream.IsMccPrepared() )
            {     
            Data().EnableL( aStream.Codec() );
            sequenceType = KMceSrvStreamAlternativeSequence;
            }
        }
    
    if ( sequenceType )
        {
        Data().Enabled( ETrue );
        if ( State() >= CMceSrvStream::EStarted )
            {
            SetState( CMceSrvStream::EStarted ); 
            }
        
        MCEMM_DEBUG_STREAM( "CMceSrvEndpoint::EnableL(): stream of endpoint after enable", 
                            aStream );
        }
    
    return sequenceType;
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::DisableL
// -----------------------------------------------------------------------------
//

TBool CMceSrvEndpoint::DisableL( CMceSrvStream& aStream, TBool aSync, TBool aForce )
    {
    if ( aStream.IsMccPrepared() && !Data().EndpointProxy() )
        {
        return KMceSrvStreamDefaultSequence;
        }
        
    MCEMM_DEBUG("CMceSrvEndpoint::DisableL(): alternative disable");
        
    TBool sequenceType = aStream.IsMccPrepared() ? 
            KMceSrvStreamDefaultSequence : 
            KMceSrvStreamAlternativeSequence;
            
    if ( ( State() >= CMceSrvStream::EStarted || aForce ) && 
         ( aSync || Data().IsEnabled() ) )
        {
        MCEMM_DEBUG("CMceSrvEndpoint::DisableL(): disabling");
        
        if ( Data().EndpointProxy() )
            {
            sequenceType = Data().EndpointProxy()->ClientDisableL( Data() );
            }
       
        if ( !aStream.IsMccPrepared() )
            {  
            TRAPD( err, Data().DisableL() )
            if ( !aForce )
                {
                // When forcing disable, error is ignored
                User::LeaveIfError( err );
                }
                
            sequenceType = KMceSrvStreamAlternativeSequence;
            }
        }
    
    if ( sequenceType )
        {
        Data().Enabled( EFalse );
        if ( State() >= CMceSrvStream::EStarted )
            {
            SetState( CMceSrvStream::EPaused ); 
            }
        MCEMM_DEBUG_STREAM( "CMceSrvEndpoint::DisableL(): stream of endpoint after disable", 
                            aStream );
        } 
   
    return sequenceType;
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::Reusable
// -----------------------------------------------------------------------------
//
TBool CMceSrvEndpoint::Reusable( const CMceSrvStream& aStream ) const
    {
    return Data().Reusable( aStream.Data() );
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::EventReceived
// -----------------------------------------------------------------------------
//
void CMceSrvEndpoint::EventReceived( TMceMccComEvent& aEvent )
    {
    switch( aEvent.iEvent )
        {
        case KMccLinkCreated:
            {
            SetState( CMceSrvStream::ELinkCreated );
            break;
            }
        case KMccStreamPrepared:
            {
            SetState( CMceSrvStream::EPrepared ); 
            break;
            }
        case KMccStreamStarted:
            {
            SetState( CMceSrvStream::EStarted ); 
            break;
            }
        case KMccStreamStopped:
            {
            SetState( CMceSrvStream::EStopped ); 
            break;
            }
        case KMccStreamPaused:
            {
            SetState( CMceSrvStream::EPaused ); 
            break;
            }
        case KMccStreamResumed:
            {
            SetState( CMceSrvStream::EStarted ); 
            break;
            }
        case KMccMasterKeyStaled:
        	{
        	TRAPD ( err, HandleSecureEventReceivedL ());
        	//ignore err 
        	err = err != KErrNone ? KErrNone : err;
        	break;
        	}
        default:
            {
            //NOP
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::Type
// -----------------------------------------------------------------------------
//
const TUid CMceSrvEndpoint::Type() const
    {
    return iData->MccType();
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::StreamStateChangeEventReceived
// -----------------------------------------------------------------------------
//
TBool CMceSrvEndpoint::StreamStateChangeEventReceived( TMceMccComEvent& aEvent )
    {
    TBool allowEvent( ETrue );

    switch ( aEvent.iEvent )
        {
        case KMccStreamStarted:
        case KMccStreamResumed:
            {
            TMceSrvStreamIterator streams( Data().MediaStream()->Session()->MccStreams(), 
                                           Data() );
            CMceSrvStream* stream = NULL;
            while ( streams.Next( stream ) && allowEvent )
                {
                if ( stream->Codec().iIsEnabled &&
                     Endpoint( *stream ).State() > CMceSrvStream::EPrepared &&
                     Endpoint( *stream ).State() != CMceSrvStream::EStarted )
                    {
                    allowEvent = EFalse;
                    }
                }
            break;
            }
        case KMccStreamPaused:
            {
            TMceSrvStreamIterator streams( Data().MediaStream()->Session()->MccStreams(), 
                                           Data() );
            CMceSrvStream* stream = NULL;
            while ( streams.Next( stream ) && allowEvent )
                {
                if ( stream->Codec().iIsEnabled &&
                     Endpoint( *stream ).State() > CMceSrvStream::EPrepared &&
                     Endpoint( *stream ).State() != CMceSrvStream::EPaused )
                    {
                    allowEvent = EFalse;
                    }
                }
            break;
            }
        default:
            {
            break;
            }
        }
    return allowEvent;
    }

// -----------------------------------------------------------------------------
// CMceSrvEndpoint::EndpointMatch
// -----------------------------------------------------------------------------
//
TBool CMceSrvEndpoint::EndpointMatch( 
    const CMceComEndpoint& aEndpoint, 
    TBool aUseProxyMatch )
    {
    TBool match( EFalse );
    if ( !aUseProxyMatch )
        {
        match = aEndpoint == *iData;
        }
    else
        {
        match = aEndpoint == *iData ||
                ( aEndpoint.EndpointProxy() && 
                  aEndpoint.EndpointProxy()->ServesProxyClient( *iData ) );

        }
    return match;
    }
    
// End of file

