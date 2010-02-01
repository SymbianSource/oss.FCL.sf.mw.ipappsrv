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



#ifndef CNATFWUNSAFQUERYSRV_H
#define CNATFWUNSAFQUERYSRV_H

//INCLUDES
#include <dns_qry.h>
#include <e32std.h>
#include "cnatfwunsafquerybase.h"
#include "natfwinternaldefs.h"

// CLASS DEFINITION
/**
 * CNATFWUNSAFQuerySrv implements a
 */
NONSHARABLE_CLASS( CNATFWUNSAFQuerySrv ): public CNATFWUNSAFQueryBase
    {
    public:// Constructors and destructor

        static CNATFWUNSAFQuerySrv* NewL( const TDesC8& aTarget );

        ~CNATFWUNSAFQuerySrv();

    public:

        static HBufC8* SetTargetProtocolL( const TDesC8& aTarget,
                                           const TDesC8& aProtocol,
                                           const TDesC8& aServiceName );
        const TDesC8& QueryBuf();

        TDes8& QueryResultBuf();

        TDnsRespSRV QueryResult();

        void AddL( const TDnsRespSRV& aSrv );

        static TInt Compare( const TDnsRespSRV& aFirst,
                             const TDnsRespSRV& aSecond );

        TDnsRespSRV& SRVRecordL( TInt aIndex );

        RArray<TDnsRespSRV>& RecordArray();

        void SetTargetL( const TDesC8& aTarget );

        void Query( MNATFWUNSAFHostResolver& aResolver );

        void HandleQueryResultL( MNATFWUNSAFHostResolver& aResolver );

        const TDesC8& ResultTargetL();

        TUint ResultPortL();

        void RemoveElementL( TInt aIndex );

        TUint ArrayCountL();

    private:// Constructors and destructor

        void ConstructL( const TDesC8& aTarget );

        CNATFWUNSAFQuerySrv();

    private:// Data

        HBufC8* iTarget;

        TDnsRespSRVBuf iResult;

        TDnsQueryBuf iQueryBuf;

        RArray<TDnsRespSRV> iRecordArray;

    private: // For testing purposes

        UNIT_TEST(UT_CNATFWUNSAFQuerySrv)
        UNIT_TEST(UT_CNATFWUNSAFQuerySrvStub)
    };
#endif // end of CNATFWUNSAFQUERYSRV_H


