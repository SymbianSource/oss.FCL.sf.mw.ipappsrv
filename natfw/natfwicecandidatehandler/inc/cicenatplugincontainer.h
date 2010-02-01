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




#ifndef C_CICENATPLUGINCONTAINER_H
#define C_CICENATPLUGINCONTAINER_H

#include <e32base.h>
#include "mnatfwpluginobserver.h"
#include "natfwpluginapi.h"
#include "micenatplugineventobs.h"
#include "natfwinternaldefs.h"    //UNIT_TEST

class MNcmConnectionMultiplexer;
class CIceSessionData;
class CNATFWPluginApi;
class TIceNatPluginContainerIter;

/**
 *  Container for NAT-protocol plugins. Configures itself; resolves available
 *  plugins based on NAT-settings. Makes possible for different clients to 
 *  request different operations on contained plugins.
 *  
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceNatPluginContainer
        : public CBase, public MNATFWPluginObserver
    {

    UNIT_TEST( UT_CIceNatPluginContainer )
    friend class TIceNatPluginContainerIter;

public:

    /**
     * Two-phased constructor.
     * @param aSessionData      Interface for querying session data
     * @param aMultiplexer      The multiplexer interface
     */
    static CIceNatPluginContainer* NewL(
        const CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer );

    /**
     * Two-phased constructor.
     * @param aSessionData      Interface for querying session data
     * @param aMultiplexer      The multiplexer interface
     */
    static CIceNatPluginContainer* NewLC(
        const CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer );

    /**
     * Destructor.
     */
    virtual ~CIceNatPluginContainer();

    /**
     * Connects contained plugins to server.
     *  
     * @since   S60 v3.2
     * @param   aSocketServ         The handle to socket server session
     * @param   aConnectionName     The unique name of the RConnection
     */
    void ConnectPluginsL( const RSocketServ& aSocketServ,
        const TName& aConnectionName );

    /**
     * Returns number of contained plugins.
     *  
     * @since   S60 v3.2
     * @return  Number of plugins
     */
    TInt Count() const;
    
    /**
     * Registers given observer for specified event.
     *
     * @since   S60 v3.2
     * @param   aObserver       The observer to be registered
     * @param   aEvent          The event for which to register
     * @leave   KErrNoMemory
     */
    void RegObserverForEventL( const MIceNatPluginEventObs& aObserver,
        MIceNatPluginEventObs::TNatPluginEvent aEvent );

    /**
     * Unregisters given observer for specified event.
     *
     * @since   S60 v3.2
     * @param   aObserver       The observer to be unregistered
     * @param   aEvent          The event for which to unregister
     */
    void UnregObserverForEvent( const MIceNatPluginEventObs& aObserver,
        MIceNatPluginEventObs::TNatPluginEvent aEvent );
    
    /**
     * Unregisters given observer for all events.
     *
     * @since   S60 v3.2
     * @param   aObserver       The observer to be unregistered
     */
    void UnregObserver( const MIceNatPluginEventObs& aObserver );
    
    /**
     * Releases plugins.
     *
     * @since   S60 3.2
     * @param   aRetainRelay    Whether relay should be kept up
     */
    void ReleasePlugins( TBool aRetainRelay );
    
    /**
     * Gets plugin by identifier (e.g. vendor.stun).
     *
     * @since   S60 3.2
     * @param   aIdentifier     The identifier for the plugin to search
     * @return  NAT plugin
     */
    CNATFWPluginApi* PluginByIdentifier( const TDesC8& aIdentifier );
    
    /**
     * Gets plugin by type (stun, turn, ice, host).
     *
     * @since   S60 3.2
     * @param   aType           The type for the plugin to search
     * @return  NAT plugin
     */
    CNATFWPluginApi* PluginByType( const TDesC8& aType );

// from base class MNATFWPluginObserver

    void Error( const CNATFWPluginApi& aPlugin, 
        TUint aStreamId, TInt aErrorCode );
    
    void Notify( const CNATFWPluginApi& aPlugin,
        TUint aStreamId, TNATFWPluginEvent aEvent, TInt aErrCode );
    
    void NewCandidatePairFound( const CNATFWPluginApi& aPlugin,
        CNATFWCandidatePair* aCandidatePair );
    
    void NewLocalCandidateFound( const CNATFWPluginApi& aPlugin,
        CNATFWCandidate* aLocalCandidate );

private:

    CIceNatPluginContainer( 
        const CIceSessionData& aSessionData,
        MNcmConnectionMultiplexer& aMultiplexer );

    void ConstructL();
    
    void ResolvePluginsL();

    TInt GetConvertedEvent( TNATFWPluginEvent aEvent, 
        MIceNatPluginEventObs::TNatPluginEvent& aConvertedEvent ) const;

    TPtrC8 ParsePluginType( const TDesC8& aIdentifier ) const;
    
private: // data

    /**
     * Initialization parameters for the utility plugins.
     */
    CNATFWPluginApi::TNATFWPluginInitParams iInitParams;
    
    /**
     * Observers indexed by the NAT-plugin events.
     * Own.
     */
    RPointerArray< RPointerArray<MIceNatPluginEventObs> > iObserversForEvent;
    
    /**
     * NAT protocol plugins.
     * Own.
     */
    RPointerArray<CNATFWPluginApi> iPlugins;
    };

#endif // C_CICENATPLUGINCONTAINER_H
