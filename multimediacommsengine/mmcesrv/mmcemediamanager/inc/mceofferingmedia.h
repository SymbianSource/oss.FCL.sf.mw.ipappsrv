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





#ifndef TMCEOFFERINGMEDIA_H
#define TMCEOFFERINGMEDIA_H

#include "mcemediastate.h"
#include "mcesdpsession.h"

class CMCCCodecInformationFactory;

const TBool KFromSubstate = ETrue;

/**
*  Represents MM offering state 
*
*/
class TMceOfferingMedia : public TMceMediaState
    {

public: // substates

    /**
    *  Represents MM offering sub state where media is ready for offer
    *
    */
    class TMceMediaReady : public TMceMediaState
        {
        public:
        
            static void SetAsCurrentStateL( 
                    CMceComSession& aSession );
                    
        
            inline TMceMMState Id() 
                {
                return KMceOfferingMedia;
                }
            
            void Callback();

            CSdpDocument* EncodeL();
            
        protected:            
        
            TMceMediaReady( CMceComSession& aSession );
                
        };

    /**
    *  Represents MM offering sub state where media update is ready for offer
    *
    */
    class TMceMediaUpdateReady : public TMceMediaReady
        {
        public:
        
            static void SetAsCurrentStateL( CMceComSession& aSession );
        
            CSdpDocument* EncodeL();
            
            TBool NeedToNegotiate( CSdpDocument* aSdpDocument = NULL );
        protected:            
        
            TMceMediaUpdateReady( CMceComSession& aSession );
        
        };


    /**
    *  Represents MM offering sub state where media waiting answer
    *
    */
    class TMceWaitingAnswer : public TMceMediaState
        {
        public:
        
            static void SetAsCurrentStateL( CMceComSession& aSession );
            
            inline TMceMMState Id() 
                {
                return KMceOfferingMedia;
                }
        
            TMceMediaState* CloneL( CMceComSession& aSession ) const;
    
            TMceSipWarningCode DecodeL( CSdpDocument& aSdpDocument );
            
            TBool NeedToNegotiate( CSdpDocument* aSdpDocument = NULL );
        protected:            
        
            TMceWaitingAnswer( CMceComSession& aSession );
            
        };

    
public: // Constructors


    static void SetAsCurrentStateL( CMceComSession& aSession, TBool aFromSubstate = EFalse );


public: // from TMceMediaState

    inline TMceMMState Id() 
        {
        return KMceOfferingMedia;
        }


public: // from TMceMediaState

    void UpdateL();

protected:

    void SetAsCurrentStateL( TMceOfferingMedia::TMceWaitingAnswer& aSubState );

    /**
    * Consructor
    */
    TMceOfferingMedia( CMceComSession& aSession );

    
    };

#endif//TMCEOFFERINGMEDIA_H

// End of File
