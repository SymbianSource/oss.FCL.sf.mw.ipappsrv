/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef TMceSessionUtility_H
#define TMceSessionUtility_H

//  INCLUDES
#include <e32base.h>
#include <mcedefs.h>
#include <mcemediastream.h>
#include <e32des8.h>
#include <in_sock.h>
#include <sdpdocument.h>


// CLASS DECLARATION

/**
* Base class for MCE sessions, inherited by both CMceInSession and CMceOutSession.
*
* CMceSession represents peer-to-peer connection made with remote terminal.
* This connection is also known as dialog and it is set up using SIP INVITE
* method.
*
* The characteristics of the session are defined by user of this class by attaching
* media streams into the session.
*
* After the session has been succesfylly established, it can be later updated and
* must be finally terminated.
*
* If for any reason instance of this class is deleted while in established state,
* session is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class TMceSessionUtility : public CBase
    {   
    public: // Session states

        enum TSDPLineName
            {
            EVersion, // v=
            //EOrigin, // o=
            ESession, // s=
            EInfo,    // i=
            EUri,     // u=
            //EEmail,   // e=
            //EPhone,   // p=
            //EConnection, // c=
            //EBandwidth, // b=
            EZone      // z=
            //EKey,       // k=
            //EAttribute, // a=
            //ETime,      //t=
            //ERepeat,    //r=
            //EMedia,     //m=    
            };
        
    public:  // Constructors and destructor
        
        
        //static TMceSessionUtility(CMceSession* aSession);          

        /**
        * Destructor.
        */
        //virtual ~TMceSessionUtility();

    public: // Functions

        /**
        * Returns value of remote session SDP line.
        * @param aSess MCE session object
        * @param TSDPLineName  SDP session level SDP line name
        * @return The valid pointer of HBufC8 if the value exists otherwise returns NULL
        *  The ownership of HBufC8 is transferred to the clietns.
        */
        
        static HBufC8* getSessionSdPLineL(CMceSession* aSess,
                TSDPLineName aLSdpLineName);
        
        /**
         * Returns the value of remote media sdp attribute value.
         * @param aStream . The stream object for which the remote attribute is required 
         * @param aAttrName  attribte name like "accept-types", "accept-wrapped-types" etc
         * @return The valid pointer of HBufC8 if the value for the attribute exists otherwise returns NULL
         *  The ownership of HBufC8 is transferred to the clietns.
         *  @Pre session->State() < CMceSession::ECancelling) || 
         *       session->State() > CMceSession::EIdle
         */
        
        IMPORT_C static HBufC8* getRemoteMediaSDPAttrL(CMceMediaStream* aStream,
                const TDesC8& aAttrName);
        
        /**
         * Returns the local conneciton address for the passed session
         * @param CMceSession MCE session object for which the conneciton address is requested
         * @param TInetAddr The conneciton address of the session will be filled in this parameter
         *  
         */
        
        IMPORT_C static void GetLocalConnectionAddrL(CMceSession* aSess, TInetAddr& aAddr); 
        
        
        /**
         * Returns the remote conneciton address for the passed stream
         * @param aStream The stream object for which the remote connection addr is requested
         * @return The HBufC8 pointer either holds the FQDN or the IP address of the remote end
         *  The Ownership is transferred to the clients
         * @Pre session->State() < CMceSession::ECancelling) || 
         *      session->State() > CMceSession::EIdle  
         */
        
        IMPORT_C static HBufC8* GetRemoteConnectionAddrL(CMceMediaStream* aStream);
        
        
        /**
         * Get local media port of the stream
         * @aSession MCE session
         * @aMediaPort On return filled with local media port 
         * @Pre session->State() < CMceSession::ECancelling) || 
         *      session->State() > CMceSession::EIdle
         */
        
        IMPORT_C static void GetLocalMediaPortL(CMceMediaStream* aStream, TUint& aMediaPort );
         
         
         /**
          * Get remote media port of the session
          * @aSession MCE session
          * @aMediaPort On return filled with local media port 
          * @Pre session->State() < CMceSession::ECancelling) || 
          *      session->State() > CMceSession::EIdle
          */
        
        IMPORT_C static void GetRemoteMediaPortL(CMceMediaStream* aStream, TUint& aMediaPort );
        
    private:

        static void ConstructSdpDocumentL(CDesC8Array* asdpLines, CSdpDocument*& asdpDoc);
        
        static HBufC8* FindAttributeL(CDesC8Array* asdpLines , const TDesC8& aAttrName);
        
    private:
        MCE_UNIT_TEST_DEFS
    };

#endif // TMceSessionUtility_H
