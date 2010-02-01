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

#ifndef NSPTESTHELPER_H
#define NSPTESTHELPER_H

#include <e32base.h>
#include <in_sock.h>
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/EUnitMacros.h>
#include <digia/eunit/EUnitDecorators.h>
#include "natfwcandidate.h"
#include "natfwconnectivityapidefs.h"
#include "nsptls.h"

#define NSP_DEL( aPtr ) delete aPtr; aPtr = NULL
#define NSP_STOPINTERNALTIMERLOOP( aUsing ) \
	if ( aUsing )\
		{\
		CActiveScheduler::Stop();\
		aUsing = EFalse;\
		}

#define NSP_TEST_METHOD( aDesc, aClass, aMethod, aTestType, aSetupFunc, aRunFunc, aTeardownFunc ) \
	EUNIT_TEST( aDesc, aClass, aMethod, aTestType, aSetupFunc, aRunFunc, aTeardownFunc )\
	EUNIT_ALLOC_TEST( aDesc, aClass, aMethod, aTestType, aSetupFunc, aRunFunc, aTeardownFunc )

#define NSP_TEST_USECASE( aDesc, aClass, aMethod, aTestType, aSetupFunc, aRunFunc, aTeardownFunc ) \
	EUNIT_TEST( aDesc, aClass, aMethod, aTestType, aSetupFunc, aRunFunc, aTeardownFunc )

#define NSP_ASSERT( aStatement ) EUNIT_ASSERT( aStatement )
#define NSP_ASSERT_PANIC( aStatement ) EUNIT_ASSERT_PANIC( aStatement )
#define NSP_ASSERT_LEAVE( aStatement ) \
	if ( iStorage->iOOMService )\
		{\
		TRAPD( err, aStatement );\
		__ASSERT_ALWAYS( KErrNoMemory != err, User::Leave( KErrNoMemory ) );\
		NSP_ASSERT( KErrNone != err );\
		}\
	else\
		{\
		EUNIT_ASSERT_LEAVE( aStatement );\
		}
#define NSP_ASSERT_NO_LEAVE( aStatement ) \
	if ( iStorage->iOOMService )\
		{\
		TRAPD( err, aStatement );\
		__ASSERT_ALWAYS( KErrNoMemory != err, User::Leave( KErrNoMemory ) );\
		NSP_ASSERT( KErrNone == err );\
		}\
	else\
		{\
		EUNIT_ASSERT_NO_LEAVE( aStatement );\
		}


#define EVENT( aStreamId, aEvent, aError, aDataPtr ) \
	iController->EventOccured( iSessionId, aStreamId, aEvent, aError, aDataPtr )

#define NEWCAND( aStreamId, aCandPtr ) \
    iController->EventOccured( iSessionId, aStreamId,\
            MNATFWConnectivityObserver::ELocalCandidateFound,\
            KErrNone, reinterpret_cast<TAny*>(aCandPtr) );\
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) ); \
    CleanupStack::Pop();//aCandPtr    
#define NEWCANDPAIR( aStreamId, aCandPairPtr ) \
    iController->EventOccured( iSessionId, aStreamId,\
            MNATFWConnectivityObserver::ECandidatePairFound,\
            KErrNone, reinterpret_cast<TAny*>(aCandPairPtr) );\
    __ASSERT_ALWAYS( KErrNoMemory != iLastError, User::Leave( KErrNoMemory ) ); \
    CleanupStack::Pop();//aCandPairPtr    

#define NSP_ASSERT_STATE( aSession, aState ) \
	NSP_ASSERT( aSession->iStateMachine->iStateArray[aState] == aSession->iStateMachine->iCurrentState )
#define NSP_SET_STATE( aSession, aNewState ) \
	aSession->iStateMachine->iCurrentState = aSession->iStateMachine->iStateArray[aNewState]

_LIT8( KTestDomain, "testdomain.com" );
_LIT8( KEmpty, "" );
_LIT8( KNokiaStun, "nokia.stun" );
_LIT8( KNokiaTurn, "nokia.turn" );
_LIT8( KNokiaIce, "nokia.ice" );
_LIT8( KNokiaAll, "nokia.ice,nokia.turn,nokia.stun" );
_LIT8( KNonNokia, "symbian.ice,symbian.turn,symbian.stun" );

const TUint32 KIapId = 2;
const TUint KProtocolUdp = 2;
const TUint KMediaTos = 184;
const TUint KPriorityMultiplier = 2050;

class CRepository;
class CNATFWCandidate;
class CNATFWCandidatePair;
class CNSPMediaStreamComponent;

/**
 * Static class that implements static methods for
 * initializing repository & different objects.
 * 
 */
class NSPTestHelper
    {
public: // Constructors and destructors

    NSPTestHelper();
    ~NSPTestHelper();

public: // New methods

    // CRepository
    static void ClearCRepositoryL( CRepository& aRep );
    static void CreateDomainKeyL( CRepository& aRep, const TDesC8& aDomain );
    static void SetCenrepTDesC8toAProtocolL( CRepository& aRep,
            const TUint32& aKey, const TDesC8& aValue );
    
    // Candidates
    static const TDesC8& GetAddr(
        CNATFWCandidate::TCandidateType aType );
    static CNATFWCandidate* CreateCandL( const TDesC8& aFoundation,
            TUint aComponentId, TUint aProtocol, TUint aPriority,
            CNATFWCandidate::TCandidateType aType, 
            TUint aSessionId, TUint aStreamId,
            TBool aSelected );
    static CNATFWCandidate* CreateCandL(
            const CNSPMediaStreamComponent& aStreamComp,
            const TDesC8& aFoundation, TUint aProtocol,
            CNATFWCandidate::TCandidateType aType, 
            TUint aSessionId, TBool aSelected );
    static CNATFWCandidatePair* CreateCandPairL(
            const CNSPMediaStreamComponent& aLocalStreamComp,
            const TDesC8& aLocalFoundation, TUint aLocalProtocol,
            CNATFWCandidate::TCandidateType aLocalType, TUint aLocalSessionId,
            TBool aLocalSelected,
            const CNSPMediaStreamComponent& aRemoteStreamComp,
            const TDesC8& aRemoteFoundation, TUint aRemoteProtocol,
            CNATFWCandidate::TCandidateType aRemoteType, 
            TUint aRemoteSessionId,
            TBool aRemoteSelected );

    static CNATFWCandidate* CreateCandLC(
            const CNSPMediaStreamComponent& aStreamComp,
            const TDesC8& aFoundation, TUint aProtocol,
            CNATFWCandidate::TCandidateType aType, 
            TUint aSessionId, TBool aSelected );

    static CNATFWCandidatePair* CreateCandPairLC(
            const CNSPMediaStreamComponent& aLocalStreamComp,
            const TDesC8& aLocalFoundation, TUint aLocalProtocol,
            CNATFWCandidate::TCandidateType aLocalType, TUint aLocalSessionId,
            TBool aLocalSelected,
            const CNSPMediaStreamComponent& aRemoteStreamComp,
            const TDesC8& aRemoteFoundation, TUint aRemoteProtocol,
            CNATFWCandidate::TCandidateType aRemoteType, 
            TUint aRemoteSessionId,
            TBool aRemoteSelected );
    
    };

#endif // NSPTESTHELPER_H

// End of file
