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




#ifndef STUNUDPSETTINGS_H__
#define STUNUDPSETTINGS_H__

#include <e32base.h>

#include "generalsettings.h" 


class CStunUdpSettings : public CGeneralSettings
{
public:
    static CStunUdpSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CStunUdpSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    ~CStunUdpSettings( );
    
public: // from CSettings
    void CreateSettingsL();

private:
    void ConstructL( const TDesC8& aDomain );
    CStunUdpSettings( TUint aIapId );
           
private: //DATA
    TUint32 iDomainKey;
};

#endif //STUNUDPSETTINGS_H__