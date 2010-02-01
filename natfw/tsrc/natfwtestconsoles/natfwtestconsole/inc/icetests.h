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




#ifndef _ICETESTS_H_
#define _ICETESTS_H_

// INCLUDES
#include <e32base.h>
#include <e32cons.h>

#include "iceteststubsobserver.h"

// FORWARD DECLARATIONS
class CNATFWConnectivityApi;
class CICETestStubs;
class CNATFWCandidate;
class CNATFWCandidatePair;


// CLASS
class CICETests : public CBase, public MICETestStubsObserver
    {
public:
    static CICETests* NewL(
        CConsoleBase& aConsole,      
        CNATFWConnectivityApi& aNat,
        CICETestStubs& aIFStub,
        const TDesC8& aDomain,
        TUint aIapId,
        TUint32 aDestAddress,
        TUint aDestPort,
        TUint aProtocol  );
    
    virtual ~CICETests();

// From MTestConsoleStubsObserver
    void LocalCandidateFound( CNATFWCandidate& aLocalCandidate );

// From MICETestStubsObserver
    void NewCandidatePairFound( 
        CNATFWCandidatePair* aCandidatePair );
        
    void DoRunIceTestsL();

private:

    void TestFetchCandidatesL();

    void TestSetOperationModeL();
    
    //void TestSetIdentificationL();
    
    //void TestPerformConnectivityChecksL();
    
    void PrintContents( CNATFWCandidate* aCandidate );
    
private:

    CICETests(   CConsoleBase& aConsole,
                    CNATFWConnectivityApi& aNat,
                    CICETestStubs& aIFStub,
                    TUint aIapId,
                    TUint32 aDestAddress,
                    TUint aDestPort,
                    TUint aProtocol );

    void ConstructL( const TDesC8& aDomain );

    void PrintTestResult( TInt aError );
    
private:

    CConsoleBase& iConsole;
    CNATFWConnectivityApi& iNat;
    CICETestStubs& iIfStub;
    TUint iTestIapId;
    HBufC8* iTestDomain;
    TUint32 iDestAddress;
    RPointerArray<CNATFWCandidate> iCandidateArray;
    CNATFWCandidatePair* iCandidatePair;
    TUint iDestPort;
    TUint iProtocol;
    };
    
#endif //_ICETESTS_H_

// End of File
