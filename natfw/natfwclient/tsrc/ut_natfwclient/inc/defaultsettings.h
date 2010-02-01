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




#ifndef __DEFAULTSETTINGS_H__
#define __DEFAULTSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CDefaultSettings : public CSettings
{
public:
    IMPORT_C static CDefaultSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    IMPORT_C static CDefaultSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

// from CSettings
    IMPORT_C void CreateSettingsL();
    
    virtual ~CDefaultSettings( );

private:
    void ConstructL( const TDesC8& aDomain );
    CDefaultSettings( TUint aIapId );
           
private: //DATA
    TUint iIapId;
    HBufC8* iDomain;
};

#endif //__NATFWDEFAULTSETTINGS_H__