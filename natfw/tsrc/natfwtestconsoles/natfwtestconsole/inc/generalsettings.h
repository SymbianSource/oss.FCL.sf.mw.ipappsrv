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




#ifndef __GENERALSETTINGS_H__
#define __GENERALSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CGeneralSettings : public CSettings
{
public:
    static CGeneralSettings* NewL( const TDesC8& aDomain, TUint aIapId );
    static CGeneralSettings* NewLC( const TDesC8& aDomain, TUint aIapId );

    virtual ~CGeneralSettings( );
    
public: // from CSettings
    
    virtual void CreateSettingsL();

public:

    TInt GetDomainKey( TUint32& aDomainKey );
    
protected:
    
    void ConstructL( const TDesC8& aDomain );
    CGeneralSettings( TUint aIapId );
           
private: //DATA
    
    TUint iIapId;
    HBufC8* iDomain;
};

#endif //__GENERALSETTINGS_H__