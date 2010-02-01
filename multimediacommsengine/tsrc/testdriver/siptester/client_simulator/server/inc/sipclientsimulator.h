/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  See below
*
*/

#ifndef CSIPCLIENTSIMULATOR_H
#define CSIPCLIENTSIMULATOR_H

//  INCLUDES
#include <sipconnectionobserver.h>
#include <sipobserver.h>
#include <e32base.h>

//  FORWARD DECLARATIONS
class CSIP;

//  CLASS DEFINITION
/**
* CSipClientSimulator creates connection to SIP stcak by creating CSIP and
* is able to receive SIP requests and send responses to them.
*/
class CSipClientSimulator : 
    public CBase,
    public MSIPObserver,
    public MSIPConnectionObserver
    {
    public:    // Constructors and destructor

        /**
        * Constructor.
        * @param aClientUid the client UID used when connecting to SIP stack
        * @return An initialized instance of this class.
        */
        static CSipClientSimulator* NewL( const TUid& aClientUid );

        /**
        * Destructor.
        */
        ~CSipClientSimulator();

    public: // From MSIPObserver

        void IncomingRequest( TUint32 aIapId,
                              CSIPServerTransaction* aTransaction );
                              
        void TimedOut( CSIPServerTransaction& aSIPServerTransaction );

    public: // From MSIPConnectionObserver

        void IncomingRequest( CSIPServerTransaction* aTransaction );
        
        void IncomingRequest( CSIPServerTransaction* aTransaction,
                              CSIPDialog& aDialog );

        void IncomingResponse( CSIPClientTransaction& aTransaction );
        
        void IncomingResponse( CSIPClientTransaction& aTransaction,
                               CSIPDialogAssocBase& aDialogAssoc );
        
        void IncomingResponse( CSIPClientTransaction& aTransaction,
                               CSIPInviteDialogAssoc* aDialogAssoc );
        
        void IncomingResponse( CSIPClientTransaction& aTransaction,
                               CSIPRegistrationBinding& aRegistration );

        void ErrorOccured( TInt aError, 
                           CSIPTransactionBase& aTransaction );
        
        void ErrorOccured( TInt aError, 
                           CSIPClientTransaction& aTransaction,
                           CSIPRegistrationBinding& aRegistration );
        
        void ErrorOccured( TInt aError,    
                           CSIPTransactionBase& aTransaction,
                           CSIPDialogAssocBase& aDialogAssoc );
        
        void ErrorOccured( TInt aError, 
                           CSIPRefresh& aSIPRefresh );
        
        void ErrorOccured( TInt aError, 
                           CSIPRegistrationBinding& aRegistration );
        
        void ErrorOccured( TInt aError, 
                           CSIPDialogAssocBase& aDialogAssoc );

        void InviteCompleted( CSIPClientTransaction& aTransaction );

        void InviteCanceled( CSIPServerTransaction& aTransaction );
            
        void ConnectionStateChanged( CSIPConnection::TState aState );

    private: // Constuctors

        CSipClientSimulator();
        void ConstructL( const TUid& aClientUid );

    private: // New methods

        void SendResponseL( 
            TUint32 aIapId, 
            CSIPServerTransaction& aTransaction );

    private: // data

        CSIP* iSip;
        CSIPConnection* iSipConnection;
    };

#endif // CSIPCLIENTSIMULATOR_H
