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




#ifndef CNATFWUNSAFSERVERRESOLVER_H
#define CNATFWUNSAFSERVERRESOLVER_H

//  INCLUDES
#include <e32base.h>
#include <es_sock.h>
#include <in_sock.h>
#include "natfwinternaldefs.h"


// FORWARD DECLARATIONS
class MNATFWUNSAFServerResolverObserver;
class CNATFWUNSAFHostResolver;
class CNATFWUNSAFServerQuery;

// CLASS DECLARATION
/**
* A class for resolving server IP addresses and ports
* using SRV and A/AAAA queries.
*
*  @lib natfwunsafprotocols.lib
*/
class CNATFWUNSAFServerResolver : public CBase
    {
    public:// Constructors and destructor

        /**
        * Creates a new CNATFWUNSAFServerResolver instance
        * @param aSocketServer a connected socket server session
        * @param aConnection an opened and active connection
        * @param aObserver a callback for receiving notifications about
        *        completed resolving requests.
        * @param aFailIfNoSRVRecordsFound If ETrue, resolving fails if no SRV
        *         records were found.
        * @returns a new instance of CNATFWUNSAFServerResolver
        */
        IMPORT_C static CNATFWUNSAFServerResolver* NewL(
                    RSocketServ& aSocketServer,
                    RConnection& aConnection,
                    MNATFWUNSAFServerResolverObserver& aObserver,
                    TBool aFailIfNoSRVRecordsFound);

        /**
        * Creates a new CNATFWUNSAFServerResolver instance
        * and pushes it to CleanupStack
        * @param aSocketServer a connected socket server session
        * @param aConnection an opened and active connection
        * @param aObserver a callback for receiving notifications about
        *        completed resolving requests.
        * @param aFailIfNoSRVRecordsFound If ETrue, resolving fails if no SRV
        *         records were found.
        * @returns a new instance of CNATFWUNSAFServerResolver
        */
        IMPORT_C static CNATFWUNSAFServerResolver* NewLC(
                    RSocketServ& aSocketServer,
                    RConnection& aConnection,
                    MNATFWUNSAFServerResolverObserver& aObserver,
                    TBool aFailIfNoSRVRecordsFound);

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNATFWUNSAFServerResolver();

    public:

        /**
        * Initiates asynchronous resolving of a FQDN to 0..n IP addresses.
        * First performs a SRV query and then resolves the final results
        * using A/AAAA queries. Only one resolving request can be outstanding
        * at a time.
        * @param aHostName a host name to be resolved.
        * @param aServiceName service to be resolved. For example "stun".
        * @param aProtocol protocol to be resolved. For example "tcp".
        * @param aDefaulPort if SRV query returns no results,
        *        the default port to be used if no SRV records found.
        * @param aResult a client owned array
        *        that will be filled on succesful completion.
        */
        IMPORT_C void ResolveL(const TDesC8& aHostName,
                               const TDesC8& aServiceName,
                               const TDesC8& aProtocol,
                               TUint aDefaultPort,
                               RArray<TInetAddr>& aResult);

        /**
        * Cancels a pending resolving request.
        */
        IMPORT_C void CancelResolving();

    private:// Constructors and destructor

        CNATFWUNSAFServerResolver(
            MNATFWUNSAFServerResolverObserver& aObserver,
            TBool aFailIfNoSRVRecordsFound );

        void ConstructL( RSocketServ& aServer,
                         RConnection& aConnection );

        void ChooseQueryL( const TDesC8& aTarget,
                           const TDesC8& aServiceName,
                           const TDesC8& aProtocol,
                           TUint aDefaultPort,
                           RArray<TInetAddr>& aResult );

        void SetA_AAAAQueryL( CNATFWUNSAFServerQuery* aQuery );

        void SetSRVQueryL( CNATFWUNSAFServerQuery* aQuery );

        CNATFWUNSAFServerQuery* CreateQueryLC( const TDesC8& aTarget,
                                          const TDesC8& aServiceName,
                                          const TDesC8& aProtocol,
                                          TUint aDefaultPort,
                                          RArray<TInetAddr>& aResult );

    private://data
        MNATFWUNSAFServerResolverObserver& iObserver;
        CNATFWUNSAFHostResolver* iResolver;

        // If ETrue, resolving fails if no SRV records were found.
        TBool iFailIfNoSRVRecordsFound;

    private: // For testing purposes

        UNIT_TEST( UT_CNATFWUNSAFServerResolver )

    };

#endif // CNATFWUNSAFSERVERRESOLVER_H


