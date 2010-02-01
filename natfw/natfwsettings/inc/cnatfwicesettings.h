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
* Description:    Encapsulates ICE related software settings.
*
*/




#ifndef C_CNATFWICESETTINGS_H
#define C_CNATFWICESETTINGS_H


#include <e32base.h>
#include <mnatfwicesettings.h>
#include "tnatfwicepreferences.h"

class CNATFWCenRepHandler;


/**
 *  Class instance encapsulates ICE related software settings.
 *
 *  @lib natfwnatsettings.dll
 *  @since S60 v3.2
 */
class CNATFWIceSettings : public CBase, public MNATFWIceSettings
    {

public:

    /**
     * Two-phased constructor.
     */
    static CNATFWIceSettings* NewL();
    
    /**
     * Two-phased constructor.
     */
    static CNATFWIceSettings* NewLC();
    
    /**
     * Destructor.
     */
    virtual ~CNATFWIceSettings();

    /**
     * Sets preference values.
     *
     * @since   S60 v3.2
     * @param   aPreferences    Preferences
     */
    void SetPreferences( const TNATFWIcePreferences& aPreferences );
    
    /**
     * Sets preference value for a specified setting.
     *
     * @since   S60 v3.2
     * @param   aSetting    Setting for which to set value
     * @param   aValue      Preference value
     * @return  KErrNotSupported if setting is not known, KErrNone otherwise
     */
    TInt SetPrefValue( TPrefSetting aSetting, TInt aValue );
    
    /**
     * Returns preference values.
     *
     * @since   S60 v3.2
     * @return  Preferences
     */
    const TNATFWIcePreferences& Preferences() const;
    
    /**
     * Sets utility plugins.
     * 
     * @since   S60 v3.2
     * @param   aUtilityPlugins Utility plugins
     */
    void SetUtilityPluginsL( const CDesC8Array& aUtilityPlugins );
    
    /**
     * Reads settings values from Central Repository
     * 
     * @since   S60 v3.2
     * @param   aDomainKey  Key for the domain specific settings
     * @param   aRepHandler Central Repository handler 
     */
    void ReadSettingsL( TUint32 aDomainKey,
        const CNATFWCenRepHandler& aRepHandler );
    
// from base class MNATFWIceSettings

    /**
     * From MNATFWIceSettings.
     * Returns preference value for a specifed setting.
     *
     * @since   S60 v3.2
     * @param   aSetting    Setting for which to return preference value
     * @param   aValue      Preference value if query was valid
     * @return  KErrNotSupported if setting is not known, KErrNone otherwise
     */
    TInt GetPrefValue( TPrefSetting aSetting, TInt& aValue ) const;
    
    /**
     * From MNATFWIceSettings.
     * Returns array of identifiers of utility plugins available for ICE.
     * 
     * @since   S60 v3.2
     * @return  Utilities available for ICE
     */
    const CDesC8Array& NatUtilityPlugins() const;
    
private:

    CNATFWIceSettings();

    void ConstructL();

private: // data

    /**
     * ICE preferences for candidate prioritization.
     */
    TNATFWIcePreferences iPreferences;

    /**
     * NAT utility plugins for ICE.
     * Own.
     */ 
    CDesC8Array* iUtilityPlugins;
    };

#endif // C_CNATFWICESETTINGS_H
