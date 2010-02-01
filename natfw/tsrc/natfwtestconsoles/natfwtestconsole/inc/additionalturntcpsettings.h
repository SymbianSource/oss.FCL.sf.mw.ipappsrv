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




#ifndef __ADDITIONALTURNTCPSETTINGS_H__
#define __ADDITIONALTURNTCPSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CAdditionalTurnTcpSettings : public CSettings
{
public:
    static CAdditionalTurnTcpSettings* NewL( const TDesC8& aDomain );
    static CAdditionalTurnTcpSettings* NewLC( const TDesC8& aDomain );

    ~CAdditionalTurnTcpSettings( );
    
protected: // from CSettings
    void CreateSettingsL();

private:
    void ConstructL( const TDesC8& aDomain );
    CAdditionalTurnTcpSettings();
           
private: //DATA
    TUint32 iDomainKey;
    HBufC8 *iDomain;
};

#endif //__ADDITIONALTURNTCPSETTINGS_H__