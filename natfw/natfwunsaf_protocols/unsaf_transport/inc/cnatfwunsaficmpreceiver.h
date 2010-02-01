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




#ifndef C_CICMPRECEIVER_H
#define C_CICMPRECEIVER_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MIcmpErrorObserver;
class RSocketServ;
class CIcmpV4Receiver;
class CIcmpV6Receiver;

// CLASS DECLARATION
class CIcmpReceiver : public CBase
    {
public: // Constructors and destructor

    IMPORT_C static CIcmpReceiver* NewL( MIcmpErrorObserver& aObserver,
                                         RSocketServ& aServer );
    ~CIcmpReceiver();

private: // Constructors, for internal use

    CIcmpReceiver();

    void ConstructL( MIcmpErrorObserver& aObserver, RSocketServ& aServer );

private: // Data

    //Owned
    CIcmpV4Receiver* iIcmpV4Receiver;
    //Owned
    CIcmpV6Receiver* iIcmpV6Receiver;
    };

#endif // end of C_CICMPRECEIVER_H


