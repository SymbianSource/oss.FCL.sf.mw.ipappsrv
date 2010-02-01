/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef TMCEMEDIANEGOTIATED_H
#define TMCEMEDIANEGOTIATED_H

#include "mcemediastate.h"


/**
*  Represents MM negotiated state 
*
*/

class TMceMediaNegotiated : public TMceMediaState
    {


private: // substates


    /**
    *  Represents MM negotiated sub state idle
    *
    */
    class TMceEndState : public TMceMediaState
        {
        public:
        
            static void SetAsCurrentStateL( CMceComSession& aSession );
        
            void Callback();
        
            TBool NeedToNegotiate( CSdpDocument* aSdpDocument );
    
            CSdpDocument* EncodeL();

            void UpdateL();

            inline TMceMMState Id() 
                {
                return KMceMediaNegotiated;
                }

            TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
            
            TMceMediaState* CloneL( CMceComSession& aSession ) const;
            
                        
        protected:            
        
            TMceEndState( CMceComSession& aSession );
        
        };


    /**
    *  Represents negotiated sub state where answer is ready to be encoded
    *
    */
    class TMceAnswererWaitingAnswerState : public TMceEndState
        {
        public:
        
            static void SetAsCurrentStateL( CMceComSession& aSession );
            
            CSdpDocument* EncodeL();
            
            TBool NeedToNegotiate( CSdpDocument* aSdpDocument );

        protected:            
        
            TMceAnswererWaitingAnswerState( CMceComSession& aSession );
            
        };

    /**
    *  Represents negotiated sub state where answer is ready to be encoded
    *
    */
    class TMceAnswererEndState : public TMceEndState
        {
        public:
        
            static void SetAsCurrentStateL( CMceComSession& aSession );
            
            TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
            
            void UpdateL();
            
        protected:            
        
            TMceAnswererEndState( CMceComSession& aSession );
            
        };

    
public: // Constructors


    static void SetAsCurrentStateL( CMceComSession& aSession,
                                    TMceNegotiationRole aRole );

    /**
    * Consructor
    */
    TMceMediaNegotiated( CMceComSession& aSession );


public: // from TMceMediaState

    inline TMceMMState Id() 
        {
        return KMceMediaNegotiated;
        }

public: // from TMceMediaState

    TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );

    void UpdateL();
    
private: 

    void OfferL();

    void AnswerL();


private: 
    	MCEMM_UT_DEFINITIONS
    	friend class UT_CMceSdpSession;
    
    };

#endif//TMCEMEDIANEGOTIATED_H

// End of File
