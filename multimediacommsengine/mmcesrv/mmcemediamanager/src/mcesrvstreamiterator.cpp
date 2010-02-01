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




#include "mcesrvstreamiterator.h"
#include "mcecommediastream.h"
#include "mcecommediasource.h"
#include "mcecommediasink.h"
#include "mcesrvendpoint.h"
#include "mceevents.h"
#include "mcecomendpointproxy.h"
#include "mcecomcodec.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::TMceSrvStreamIterator
// -----------------------------------------------------------------------------
//
TMceSrvStreamIterator::TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams )
    : iStreams( aStreams ),
      iDirection( EZero ),
      iStream( NULL ),
      iEndpoint( NULL ),
      iCodec( NULL ),
      iCurrentIndex( 0 ),
      iUseProxyMatch( EFalse )
    {
    }
    

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::TMceSrvStreamIterator
// -----------------------------------------------------------------------------
//
TMceSrvStreamIterator::TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                                              TDirection aDirection )
    : iStreams( aStreams ),
      iDirection( aDirection ),
      iStream( NULL ),
      iEndpoint( NULL ),
      iCodec( NULL ),
      iCurrentIndex( 0 ),
      iUseProxyMatch( EFalse )
    {
    }
    

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::TMceSrvStreamIterator
// -----------------------------------------------------------------------------
//
TMceSrvStreamIterator::TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                                              const CMceComMediaStream& aStream )
    : iStreams( aStreams ),
      iDirection( EZero ),
      iStream( &aStream ),
      iEndpoint( NULL ),
      iCodec( NULL ),
      iCurrentIndex( 0 ),
      iUseProxyMatch( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::TMceSrvStreamIterator
// -----------------------------------------------------------------------------
//
TMceSrvStreamIterator::TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                                              const CMceComEndpoint& aEndpoint,
                                              TBool aUseProxyMatch,
                                              TDirection aDirection,
                                              const CMceComCodec* aCodec )
    : iStreams( aStreams ),
      iDirection( aDirection ),
      iStream( NULL ),
      iEndpoint( &aEndpoint ),
      iCodec( aCodec ),
      iCurrentIndex( 0 ),
      iUseProxyMatch( aUseProxyMatch )
    {
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::TMceSrvStreamIterator
// -----------------------------------------------------------------------------
//
TMceSrvStreamIterator::TMceSrvStreamIterator( const RPointerArray<CMceSrvStream>& aStreams,
                                              const CMceComCodec& aCodec )
    : iStreams( aStreams ),
      iDirection( EZero ),
      iStream( NULL ),
      iEndpoint( NULL ),
      iCodec( &aCodec ),
      iCurrentIndex( 0 ),
      iUseProxyMatch( EFalse )
    {
    }



// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Next
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::Next( CMceSrvStream*& aCandidate,
                                   TMceMccComEvent& aRecipient,
                                   TStreamStateMatchType aMatchType )

    {
    CMceSrvStream* next = NULL;
    TBool eof = EFalse;
    while ( !eof && Next( next, aMatchType ) )
        {
        eof = next->Consumes( aRecipient );
        next = eof ? next : NULL;
        }
        
    aCandidate = next;
    return aCandidate ? ETrue : EFalse;
    
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Next
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::Next( CMceSrvStream*& aCandidate,
                                   CMceSrvStream& aClone,
                                   TStreamStateMatchType aMatchType )
    {
    CMceSrvStream* next = NULL;
    TBool eof = EFalse;
    while ( !eof && Next( next, aMatchType ) )
        {
        eof = next->IsEqual( aClone );
        next = eof ? next : NULL;
        }
        
    aCandidate = next;
    return aCandidate ? ETrue : EFalse;
    
    }
    

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::NextIntersect
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::NextIntersect( CMceSrvStream*& aCandidate,
                                            TMceSrvStreamIterator& aSet, 
                                            CMceSrvStream*& aSetIntersect )
    {

    CMceSrvStream* intersect = NULL;
    aSetIntersect = NULL;
    
    while( !aSetIntersect && Next( intersect ) )
        {
        aSet.Reset();
        aSet.Next( aSetIntersect, *intersect );
        }

    intersect = aSetIntersect ? intersect : NULL;
    
    aCandidate = intersect;
    return aCandidate ? ETrue : EFalse;
    
    }
    
// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::NextComplement
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::NextComplement( CMceSrvStream*& aCandidate,
                                             TMceSrvStreamIterator& aSet )
    {
    TBool found = EFalse;
    
    CMceSrvStream* complement = NULL;
    
    while( !found && Next( complement ) )
        {
        aSet.Reset();
        CMceSrvStream* c = NULL;
        found = !aSet.Next( c, *complement );
        }
    
    aCandidate = complement;
    return aCandidate ? ETrue : EFalse;
    
    }
    

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Next
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::Next( 
                CMceSrvStream*& aCandidate,
                CMceSrvStream::TState aState, 
                TMceSrvStreamIterator::TStreamStateMatchType aMatchType )
    {    
    CMceSrvStream* next = NULL;
    TBool eof = EFalse;
    
    while ( !eof && Next( next ) )
        {
        TBool condition = ETrue;
        TBool negation = ETrue;
        switch( aMatchType )
            {
             case ExactMatch:
                {
                condition = next->State() == aState;
                negation = EFalse;
                break;
                }
            case ExactReverseMatch:
                {
                condition = next->State() == aState;
                negation = ETrue;
                break;
                }
            case GreaterOrEqualMatch:
                {
                condition = next->State() >= aState;
                negation = EFalse;
                break;
                }
            case GreaterOrEqualReverseMatch:
                {
                condition = next->State() >= aState;
                negation = ETrue;
                break;
                }
            }
        eof = ( ( negation && !condition ) ||
                ( !negation && condition ) );
        next = eof ? next : NULL;
        }

    aCandidate = next;
    return aCandidate ? ETrue : EFalse;
    
    }


// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Current
// -----------------------------------------------------------------------------
//
TInt TMceSrvStreamIterator::Current()
    {
    TInt current = KErrNotFound;
    
    if ( iStreams.Count() > 0 )
        {
        current = iCurrentIndex-1;
        current = current < 0 ? 0 : current;
        }
    return current;
    
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Remove
// -----------------------------------------------------------------------------
//
TInt TMceSrvStreamIterator::Remove( RPointerArray<CMceSrvStream>& aStreams )
    {
    TInt current = Current();
    
    if ( current != KErrNotFound )
        {
        aStreams.Remove( current );
        iCurrentIndex = current;
        }
    
    return current;
    
    }


// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::IsEof
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::IsEof()
    {
    return iStreams.Count() == 0 || iCurrentIndex >= iStreams.Count();
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Direction
// -----------------------------------------------------------------------------
//
TMceSrvStreamIterator::TDirection TMceSrvStreamIterator::Direction()
    {
    return iDirection;
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Next
// -----------------------------------------------------------------------------
//
TBool TMceSrvStreamIterator::Next( 
                    CMceSrvStream*& aCandidate,
                    TMceSrvStreamIterator::TStreamStateMatchType aMatchType )

    {
    CMceSrvStream* next = NULL;
    TBool negation = aMatchType == ExactMatch ? EFalse : ETrue;
    
    while( !next && !IsEof() )
        {
        CMceSrvStream* srvStream = iStreams[ iCurrentIndex ];
        TBool condition = EFalse;
        
        if ( iEndpoint )
            {
            condition = srvStream->EndpointMatch( *iEndpoint, iUseProxyMatch );
            if ( condition && iCodec )
                {
                condition = srvStream->Codec().Decodes( 
                                iCodec->iSdpName, iCodec->FmtpAttr() );
                }
                 
            if ( condition && ( iDirection != EZero || iCodec ) )
                {
                condition = DoDirectionMatch( *srvStream );
                }   
            }
        else if ( iDirection != EZero )
            {
            condition = DoDirectionMatch( *srvStream );
            }
        else if ( iStream )
            {
            condition = srvStream->Data().Id() == iStream->Id();
            }
        else if ( iCodec )
            {
            condition = &srvStream->Codec() == iCodec;
            }
        else
            {
            condition = ETrue;
            }
        next = ( ( negation && !condition ) ||
               ( !negation && condition ) ) ? srvStream : NULL;
               
        iCurrentIndex++;                            
        }
    
    aCandidate = next;
    return aCandidate ? ETrue : EFalse;
    
    }
    


// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::Reset
// -----------------------------------------------------------------------------
//
void TMceSrvStreamIterator::Reset()
    {
    iCurrentIndex = 0;
    }

// -----------------------------------------------------------------------------
// TMceSrvStreamIterator::DoDirectionMatch
// -----------------------------------------------------------------------------
//    
TBool TMceSrvStreamIterator::DoDirectionMatch( CMceSrvStream& aSrvStream )
    {
    TBool condition( ETrue );
    CMceComMediaStream::TStreamType type = aSrvStream.Data().iStreamType;
    switch( iDirection )
        {
        case ELocal:
            {
            condition = type == CMceComMediaStream::ELocalStream;
            break;
            }
        case ESendOrReceive:
            {
            condition = type != CMceComMediaStream::ELocalStream;
            break;
            }
        case ESend:
            {
            condition = ( type == CMceComMediaStream::ESendOnlyStream ||
                          type == CMceComMediaStream::ESendStream );
            break;
            }
        case EReceive:
            {
            condition = ( type == CMceComMediaStream::EReceiveOnlyStream ||
                          type == CMceComMediaStream::EReceiveStream );
            break;
            }
        case EReceiveOrLocal:
            {
            condition = ( type == CMceComMediaStream::EReceiveOnlyStream ||
                          type == CMceComMediaStream::EReceiveStream ||
                          type == CMceComMediaStream::ELocalStream );
            break;
            }
        case ESendOrLocal:
            {
            condition = ( type == CMceComMediaStream::ESendOnlyStream ||
                          type == CMceComMediaStream::ESendStream || 
                          type == CMceComMediaStream::ELocalStream );
            break;
            }
        default://EAny
            {
            condition = ETrue;
            break;
            }
        }
    return condition;
    }

