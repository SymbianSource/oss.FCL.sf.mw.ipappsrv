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




#ifndef __ADDITIONALSTUNTCPSETTINGS_H__
#define __ADDITIONALSTUNTCPSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CAdditionalStunTcpSettings : public CSettings
{
public:
    static CAdditionalStunTcpSettings* NewL( const TDesC8& aDomain );
    static CAdditionalStunTcpSettings* NewLC( const TDesC8& aDomain );

    ~CAdditionalStunTcpSettings( );
    
protected: // from CSettings
    void CreateSettingsL();

private:
    void ConstructL( const TDesC8& aDomain );
    CAdditionalStunTcpSettings();
           
private: //DATA
    TUint32 iDomainKey;
    HBufC8 *iDomain;
};

#endif //__ADDITIONALSTUNTCPSETTINGS_H__