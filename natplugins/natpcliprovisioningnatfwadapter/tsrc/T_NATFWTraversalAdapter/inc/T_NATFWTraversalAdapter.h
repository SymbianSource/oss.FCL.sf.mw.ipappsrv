/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __T_NATFWTRAVERSALADAPTER_H__
#define __T_NATFWTRAVERSALADAPTER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CWPProvisioningFile;
class CWPEngine;
class CNATFWIAPSpecificItem;
class CNATFWDomainSpecificItem;
class CWPStunServerItem;
class CWPTurnServerItem;

#include <e32def.h>
#include    <e32base.h>
#include    <f32file.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( T_NATFWTraversalAdapter )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_NATFWTraversalAdapter* NewL();
        static T_NATFWTraversalAdapter* NewLC();
        /**
         * Destructor
         */
        ~T_NATFWTraversalAdapter();

    private:    // Constructors and destructors

        T_NATFWTraversalAdapter();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

         void SetupL();
         
         void SaveDocumentL( const TDesC16& aFilename );
        
         void Teardown();
        
         void StoreL();
         
         void NATFWDomainSpecificItem_GetDomain(  );
         
         void NATFWDomainSpecificItem_GetSTUNAddr(  );
         
         void NATFWDomainSpecificItem_GetSTUNPort(  );
         
         void NATFWDomainSpecificItem_GetNATRefreshTCP(  );
         
         void NATFWDomainSpecificItem_GetNATRefreshUDP(  );
         
         void NATFWDomainSpecificItem_GetEnableCRLFRefresh(  );
         
         void NATFWIAPSpecificItem_GetIntervalUDP( );
         
         void NATFWIAPSpecificItem_GetIntervalTCP();
         
         void NATFWIAPSpecificItem_GetSTUNRetransmitTimer();

        void NATFWDomainSpecificItem_GetSTUNUsername();
        
        void NATFWDomainSpecificItem_GetSTUNPassword();

        void NATFWDomainSpecificItem_StunSrvAddrL();

        void NATFWDomainSpecificItem_StunSrvPort();
        
        void NATFWDomainSpecificItem_StunSrvUsernameL();
        
        void NATFWDomainSpecificItem_StunSrvPasswordL();

        void NATFWDomainSpecificItem_DisableStunSharedSecret();

        void NATFWDomainSpecificItem_StartPortRange();

        void NATFWDomainSpecificItem_EndPortRange();

        void NATFWDomainSpecificItem_UsedNatProtocol();

        void NATFWDomainSpecificItem_NatUtilities();

        void NATFWDomainSpecificItem_HostPref();

        void NATFWDomainSpecificItem_ServerRefPref();

        void NATFWDomainSpecificItem_RelayPref();

        void NATFWDomainSpecificItem_PeerRefPref();

        void NATFWDomainSpecificItem_IPv4Pref();

        void NATFWDomainSpecificItem_IPv6Pref();

        void NATFWDomainSpecificItem_VpnPref();

        void NATFWDomainSpecificItem_UdpPref();

        void NATFWDomainSpecificItem_TcpPref();

        void NATFWDomainSpecificItem_TcpActivePref();

        void NATFWDomainSpecificItem_TcpPassivePref();

        void NATFWDomainSpecificItem_TcpSimultPref();

        void NATFWDomainSpecificItem_TurnSrvAddrL();
        
        void NATFWDomainSpecificItem_TurnSrvPort();
        
        void NATFWDomainSpecificItem_TurnUsernameL();
        
        void NATFWDomainSpecificItem_TurnPasswordL();

        void StoreAgainL();

        void CheckUpdatedSettingsL();

        void CheckFlagsL( TInt aWbxmlNumber );
        
        void GetDomainSettingsL( TUint32 aDomainKeyId, 
            CNATFWDomainSpecificItem& aItem );
         
        void GetIAPSettingsL( TUint32 aIAPKey, CNATFWIAPSpecificItem& aItem );



    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

        HBufC8* iProvisioningDoc;

        TPtr8 iProvData;

        RFs iFs;

        RFile iFile;

        CNATFWDomainSpecificItem* iNATFWDomainSpecificItem;

        CNATFWIAPSpecificItem* iNATFWIAPSpecificItem;
    };

#endif      //  __T_NATFWTRAVERSALADAPTER_H__

// End of file
