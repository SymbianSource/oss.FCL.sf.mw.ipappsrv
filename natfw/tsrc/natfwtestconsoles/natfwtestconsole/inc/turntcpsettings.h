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




#ifndef TURNTCPSETTINGS_H__
#define TURNTCPSETTINGS_H__

#include <e32base.h>

#include "generalsettings.h" 


class CTurnTcpSettings : public CGeneralSettings
{
public:
    static CTurnTcpSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CTurnTcpSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    ~CTurnTcpSettings( );
    
public: // from CSettings
    
    void CreateSettingsL();

private:
    
    void ConstructL( const TDesC8& aDomain );
    CTurnTcpSettings( TUint aIapId );
           
private: //DATA
    
    TUint32 iDomainKey;
};

#endif //TURNTCPSETTINGS_H__