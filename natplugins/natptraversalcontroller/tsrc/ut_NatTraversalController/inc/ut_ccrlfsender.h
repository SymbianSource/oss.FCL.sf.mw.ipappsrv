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

#ifndef CCRLFSENDERSTUB_H
#define CCRLFSENDERSTUB_H

//  INCLUDES
#include "CCRLFSender.h"

// FORWARD DECLARATIONS


// CLASS DECLARATION
class CCRLFSenderStub : public CCRLFSender
    {
    public:    // Constructors and destructor

        CCRLFSenderStub(
            MNATBindingRefresherObserver& aObserver );

        ~CCRLFSenderStub();

    public: // From CCRLFSender

        TBool HasSocket( const RSocket& aSocket ) const;

    protected: // From CActive

        void DoCancel();

    protected: // From CCRLFSender

        void Send( const TDesC8& aData );
    };

#endif // CCRLFSENDERSTUB_H

// End of File
