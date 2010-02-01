/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef ICECANDIDATEHANDLERDEFS_H
#define ICECANDIDATEHANDLERDEFS_H

#include <e32base.h>
#include <in_sock.h>

class CNATFWPluginApi;

_LIT8( KNatPluginIdNokiaStun, "nokia.stun" );
_LIT8( KNatPluginIdNokiaTurn, "nokia.turn" );
_LIT8( KNatPluginIdNokiaIce, "nokia.ice" );
_LIT8( KNatPluginIdNokiaHost, "nokia.host" );

_LIT8( KNatPluginTypeStun, "stun" );
_LIT8( KNatPluginTypeTurn, "turn" );
_LIT8( KNatPluginTypeIce, "ice" );
_LIT8( KNatPluginTypeHost, "host" );

const TInt KErrRoleConflict = 487;

template <class T>
class CleanupResetAndDestroy
    {

public:
    inline static void PushL( T& aRef );

private:
    static void ResetAndDestroy( TAny *aPtr );
    };

template <class T>
inline void CleanupResetAndDestroyPushL( T& aRef );

template <class T>
inline void CleanupResetAndDestroy<T>::PushL( T& aRef )
    { CleanupStack::PushL( TCleanupItem( &ResetAndDestroy, &aRef ) ); }

template <class T>
void CleanupResetAndDestroy<T>::ResetAndDestroy( TAny *aPtr )
    { 
    if ( aPtr )
        {
        static_cast<T*>( aPtr )->ResetAndDestroy();
        }
    }

template <class T>
inline void CleanupResetAndDestroyPushL( T& aRef )
    { CleanupResetAndDestroy<T>::PushL( aRef ); }

class TCounter
    {
public:
    
    TCounter( TUint aStreamId )
        :
        iStreamId( aStreamId ),
        iCount( 0 ) { }
    
    TCounter( TUint aStreamId, TInt aCount )
        :
        iStreamId( aStreamId ),
        iCount( aCount ) { }
    
    TUint StreamId() const { return iStreamId; }
    TUint Count() const { return iCount; }
    
    TBool operator==( TInt aCount ) const
        { return ( iCount == aCount ); }
    
    TBool operator!=( TInt aCount ) const
        { return ( iCount != aCount ); }
    
    TCounter operator++( TInt )
        { TCounter temp = *this; ++iCount; return temp; }
    
    TCounter& operator++()
        { ++iCount; return *this; }
            
    TCounter operator--( TInt )
        { TCounter temp = *this; --iCount; return temp; }
    
    TCounter& operator--()
        { --iCount; return *this; }
    
    static TBool MatchStreamId( 
        const TCounter& aCnt1,
        const TCounter& aCnt2 )
        {
        return ( aCnt1.StreamId() == aCnt2.StreamId() );
        }
    
private:
    
    TCounter();
    TUint iStreamId;
    TInt iCount;
    };

class TIceUtils
    {
public:
    static TBool MatchAddresses( 
            const TInetAddr& aAddr1, const TInetAddr& aAddr2 )
        {
        // CmpAddr does not interpret IPv4 and IPv4-mapped/compatible addresses
        // as same even if they represent same address. Thus extra testing is
        // needed to handle that case.
        TBool isMatch = 
            ( aAddr1.CmpAddr( aAddr2 ) || ( aAddr1.Address() == aAddr2.Address() 
                && aAddr1.Port() == aAddr2.Port() ) );
        return isMatch;
        }
    };

    
#endif // ICECANDIDATEHANDLERDEFS_H
