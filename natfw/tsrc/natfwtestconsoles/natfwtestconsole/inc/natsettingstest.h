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
* Description:    
*
*/




// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CNATFWNatSettingsApi;
class CConsoleBase;

// CLASS
class CNATSettingsTest : public CBase
    {
public:

    static CNATSettingsTest* NewL( const TDesC8& aDomain, TUint aIapId );
    
    ~CNATSettingsTest();
    
    void PrintSettingsL( CConsoleBase& aConsole );

private:
    
    void ConstructL( const TDesC8& aDomain );
    
    CNATSettingsTest( TUint aIapId );
    
    void PrintStunServerSettings( CConsoleBase& aConsole );
    
    void PrintTurnServerSettings( CConsoleBase& aConsole );
    
private: // Data
    
    TUint iIapId;
    HBufC8* iDomain;
    CNATFWNatSettingsApi* iSettings;
    };
