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




#ifndef __ICETCPSETTINGS_H__
#define __ICETCPSETTINGS_H__

#include <e32base.h>

#include "Generalsettings.h" 


class CICETCPSettings : public CGeneralSettings
{
public:
    static CICETCPSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CICETCPSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    ~CICETCPSettings( );
    
public: // from CSettings
    
    void CreateSettingsL();

private:
    
    void ConstructL( const TDesC8& aDomain );
    CICETCPSettings( TUint aIapId );
           
private: //DATA
    
    TUint32 iDomainKey;
};

#endif //__ICETCPSETTINGS_H__