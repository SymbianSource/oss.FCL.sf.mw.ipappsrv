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
* Description:  State machine actions set.
*
*/

#ifndef NSPACTIONSET_H
#define NSPACTIONSET_H

#include <e32base.h>
#include "nsppluginreturndef.h"
#include "mnatfwconnectivityobserver.h"
#include "nspsession.h"

class CSdpDocument;

/**
 *  State machine action set.
 *
 *  Class implements actions that are executed in certain states.
 *  Each action is atomic.
 *
 *  @lib natfwsdpprovider.dll
 *  @since S60 3.2
 */
class TNSPActionSet
    {
public: // Constructors and destructor

    /**
     * Default C++ constructor.
     *
     * @since       S60 3.2
     * @param       aSession            Session object reference.
     */
    TNSPActionSet( CNSPSession& aSession );


public: // New functions

    /**
     * Method is used to create NATFW streams according to the sdp document.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     * @return		Returns 'KNatAsync' if streams were created,
     * 				otherwise 'KNatReady'.
     */
    TNatReturnStatus CreateStreamsL( CSdpDocument& aDoc );
    
    /**
     * Method is used to remove NATFW streams according to the sdp document.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     * @return		Returns 'KNatAsync' if streams were removed,
     * 				otherwise 'KNatReady'.
     */
    TNatReturnStatus RemoveStreamsL( CSdpDocument& aDoc );
    
    /**
     * Method is used to control NATFW streams.
     * 
     * @since		S60 3.2
     * @param		aStreamId			Stream identifier.
     * @param		aEvent				Stream event.
     * @param		aError				Error code for specific stream, and event.
     * @param		aData				Event specific data.
     * @return		Returns 'KNatReady' if media is ready, otherwise 'KNatAsync' or 
     * 				error code is returned.
     */
    TNatReturnStatus ControlMediaL( TUint aStreamId,
    		MNATFWConnectivityObserver::TNATFWConnectivityEvent aEvent,
    		TInt aError, TAny* aData );
    
    /**
     * Method is used to modify Sdp document which is sent to the session peer.
     * This method also adds NAT protocol specific attributes to Sdp document.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     * @param       aStoreSdp           Flag indicating whether to clone given Sdp.
     */
    void FirstRoundL( CSdpDocument& aDoc, TBool aStoreSdp = ETrue );
    
    /**
     * Method is used to modify Sdp document which is sent to the session peer.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     */
    void LatterRoundL( CSdpDocument& aDoc );

    /**
     * Method is used to modify Sdp document which is given to the local host.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     */
    void LocalRoundL( CSdpDocument& aDoc );
    
    /**
     * Method is used to check the need of update, and to generate new offer,
     * if updated O/A exchange is needed.
     *
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference, local media desc.
     */
    void UpdateRoundL( CSdpDocument& aDoc );
    
    /**
     * Method is used to retrieve ice - credentials from the given Sdp document.
     * Return status informs whether credentials have changed.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     * @return      KNatAsync if credentials changed in some level.
     *              KNatReady otherwise.
     */
    TNatReturnStatus GetCredentialsL( CSdpDocument& aDoc );
    
    /**
     * Method is used to retrieve ice - candidates from the given Sdp document.
     * Return status informs whether candidates have changed.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     * @return      KNatAsync if candidates changed in some level.
     *              KNatReady otherwise.
     */
    TNatReturnStatus GetCandidatesL( CSdpDocument& aDoc );
    
    /**
     * This method is used to check session modifiers that affect on ICE 
     * role, and ICE processing in general
     * 
     * @since       S60 3.2
     * @param       aDoc                Session Sdp document object.
     * @param       aRole               Role of the document.
     * @return      Nat status, if asynchronous processing needed,
     *              KNatAsync is returned. Otherwise KNatReady.
     */
    TNatReturnStatus GetSessionModifiersL( CSdpDocument& aDoc,
            CNSPSession::TSdpRole aRole = CNSPSession::EUndefined );
    
    /**
     * Method is used to retrieve transport addresses from m/c -lines,
     * from the given Sdp document. Return status informs if any changes were made.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     * @return      KNatAsync if modifiers changed in some level.
     *              KNatReady otherwise.
     */
    TNatReturnStatus GetSessionConnInfoL( CSdpDocument& aDoc );
    
    /**
     * Method is used to remove Ice candidates & remove Ice strings.
     * 
     * @since       S60 3.2
     */
    void RestartWithoutIce();
    
    /**
     * Method is used to load the correct plugin in NATFW.
     * 
     * @since       S60 3.2
     */
    void LoadPluginL();
    
    /**
     * Method is used to activate sending/receiving in external NATFW's socket.
     * 
     * @since       S60 3.2
     * @param       aDoc                Sdp document reference.
     */
    void ActivateL( CSdpDocument& aDoc );
    
    /**
     * Method is used to deactivate sending/receiving in external NATFW's socket.
     * 
     * @since       S60 3.2
     */
    void DeActivateL();
    
    /**
     * Method is used to initiate local candidate search in NATFW.
     * 
     * @since       S60 3.2
     */
    void FetchL();
    
    /**
     * Method is used to set ICE role to NATFW.
     * 
     * @since       S60 3.2
     */
    void SetRoleL();
    
    /**
     * Method is used to set ICE Inbound credentials to NATFW.
     * 
     * @since       S60 3.2
     */
    void SetInboundCredentialsL();
    
    /**
     * Method is used to set ICE Outbound credentials to NATFW.
     * 
     * @since       S60 3.2
     */
    void SetOutboundCredentialsL();
    
    /**
     * Method is used to initiate connectivity checks in NATFW.
     * 
     * @since       S60 3.2
     */
    void ConnChecksL();
    
    /**
     * Method is used to initiate remote candidate update to NATFW.
     * 
     * @since       S60 3.2
     */
    void UpdateL();
    
private: // data
    
    /**
     * Session instance
     */
    CNSPSession& iSession;
    
    };

#endif // NSPACTIONSET_H

// end of file
