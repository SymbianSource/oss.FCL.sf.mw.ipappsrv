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




#ifndef __ICESETTINGS2_H__
#define __ICESETTINGS2_H__

#include <e32base.h>

#include "generalsettings.h" 


class CICEUDPSettings : public CGeneralSettings
{
public:
    static CICEUDPSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CICEUDPSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    virtual ~CICEUDPSettings( );
    
public:
    
    void CreateSettingsL();

private:
    
    void ConstructL( const TDesC8& aDomain );
    CICEUDPSettings( TUint aIapId );
           
private: //DATA
    
    TUint32 iDomainKey;
};

#endif //__NATFWDEFAULTSETTINGS_H__