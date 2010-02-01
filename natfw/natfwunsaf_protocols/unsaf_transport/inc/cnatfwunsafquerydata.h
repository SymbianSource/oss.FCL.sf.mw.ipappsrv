/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CNATFWUNSAFQUERYDATA_H
#define CNATFWUNSAFQUERYDATA_H

//INCLUDES
#include <dns_qry.h>
#include "cnatfwunsafqueryconditionbase.h"
#include "cnatfwunsafquerybase.h"
#include "natfwinternaldefs.h"

// FORWARD DECLARATIONS
class MNATFWUNSAFServerResolverObserver;
class MNATFWUNSAFHostResolver;
class CNATFWUNSAFQueryA_AAAA;

// CLASS DEFINITION
/**
 * CNATFWUNSAFQueryData implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFQueryData ): public CBase
    {
    public:// Constructors and destructor

        static CNATFWUNSAFQueryData* NewL(
            MNATFWUNSAFServerResolverObserver* aObserver,
            TUint aPort,
            const TDesC8& aProtocol,
            const TDesC8& aOriginalTarget,
            const TDesC8& aServiceName,
            MNATFWUNSAFHostResolver& aResolver,
            RArray<TInetAddr>& aResult );

        ~CNATFWUNSAFQueryData();

    public:

        RArray<TInetAddr>& ResultArray();

        MNATFWUNSAFServerResolverObserver* Observer();

        TUint Port() const;

        TUint DefaultPort() const;

        const TDesC8& Protocol() const;

        const TDesC8& Target() const;

        const TDesC8& OriginalTarget() const;

        const TDesC8& ServiceName() const;

        MNATFWUNSAFHostResolver& Resolver();

        void SetPort( TUint aPort );

        void SetDefaultPort( TUint aPort );

        void SetProtocolL( const TDesC8& aProtocol );

        void SetTargetL( const TDesC8& aTarget );

        void SetOriginalTargetL( const TDesC8& aOriginalTarget );

        TBool CreateResultL( CNATFWUNSAFQueryA_AAAA& aQuery );

    private:// Constructors and destructor

        void ConstructL( MNATFWUNSAFServerResolverObserver* aObserver,
                         TUint aPort,
                         const TDesC8& aProtocol,
                         const TDesC8& aOriginalTarget,
                         const TDesC8& aServiceName );

        CNATFWUNSAFQueryData( MNATFWUNSAFHostResolver& aResolver,
                         RArray<TInetAddr>& aResult  );

        TBool AddResultToArrayL();

    private:// Data

        HBufC8* iTarget;

        HBufC8* iOriginalTarget;

        //Not owned
        CNATFWUNSAFQueryA_AAAA* iQuery;

        TUint iPort;
        TUint iDefaultPort;
        HBufC8* iProtocol;
        HBufC8* iServiceName;

        //Not owned
        MNATFWUNSAFServerResolverObserver* iObserver;

        MNATFWUNSAFHostResolver& iResolver;
        RArray<TInetAddr>& iResultArray;
    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFQueryData)
        UNIT_TEST(UT_CNATFWUNSAFA_AAAAOrigDomain)
    };
#endif // end of CNATFWUNSAFQUERYDATA_H


