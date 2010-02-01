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




#ifndef __ADDITIONALTURNUDPSETTINGS_H__
#define __ADDITIONALTURNUDPSETTINGS_H__

#include <e32base.h>

#include "settings.h" 


class CAdditionalTurnUdpSettings : public CSettings
{
public:
    static CAdditionalTurnUdpSettings* NewL( const TDesC8& aDomain );
    static CAdditionalTurnUdpSettings* NewLC( const TDesC8& aDomain );

    ~CAdditionalTurnUdpSettings( );
    
protected: // from CSettings
    void CreateSettingsL();

private:
    void ConstructL( const TDesC8& aDomain );
    CAdditionalTurnUdpSettings();
           
private: //DATA
    TUint32 iDomainKey;
    HBufC8 *iDomain;
};

#endif //__ADDITIONALTURNUDPSETTINGS_H__