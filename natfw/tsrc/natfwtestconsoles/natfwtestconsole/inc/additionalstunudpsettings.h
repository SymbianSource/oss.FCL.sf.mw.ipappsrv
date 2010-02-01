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




#ifndef __ADDITIONALSTUNUDPSETTINGS_H__
#define __ADDITIONALSTUNUDPSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CAdditionalStunUdpSettings : public CSettings
{
public:
    static CAdditionalStunUdpSettings* NewL( const TDesC8& aDomain );
    static CAdditionalStunUdpSettings* NewLC( const TDesC8& aDomain );

    ~CAdditionalStunUdpSettings( );
    
protected: // from CSettings
    void CreateSettingsL();

private:
    void ConstructL( const TDesC8& aDomain );
    CAdditionalStunUdpSettings();
           
private: //DATA
    HBufC8 *iDomain;
    TUint32 iDomainKey;

};

#endif //__ADDITIONALSTUNUDPSETTINGS_H__