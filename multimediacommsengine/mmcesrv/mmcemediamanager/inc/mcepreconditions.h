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




#ifndef MCEPRECONDITIONS_H
#define MCEPRECONDITIONS_H

#include <e32std.h>
#include "mcemediadefs.h"
#include "mcesrv.h"
#include "mcesip.h"

class CMceComMediaStream;
class CSdpMediaField;
class CSdpAttributeField;
class RStringF;
class RStringPool;
class TStringTable;

/**
 * 
 *
 * @lib 
 */
 
class TMcePreconditions
    {


public: // 

    enum TState
        {
        ECreated,
        ENegotiating,
        EReserved
        };
        
    enum TPreconditionType
        {
        EQosPreconds = 1,
        ESecPreconds
        };    
        
public: // Constructors & Destructor


    /**
     * C++ default constructor.
     */
    TMcePreconditions( CMceComMediaStream& aStream );
    
    virtual ~TMcePreconditions();
    void ConstructL();
    
    
public: // API

    virtual void EncodeL( CSdpMediaField& aMedia ) = 0;
    virtual TMceSipWarningCode DecodeL( CSdpMediaField& aMedia ) = 0;
    virtual void Reserved() = 0;
    virtual TBool ReservationNeeded() const = 0;
    virtual TBool RemoteReservationNeeded() const = 0;
    virtual TMcePreconditions* CloneL( CMceComMediaStream& aStream ) = 0;

    TBool IsMet() const;
    
    TPreconditionType Type();
    

protected: //methods

    virtual void DoConstructL() = 0;
    void ConstrucL( const TMcePreconditions& aPreconditions );
    
protected: //data

    CMceComMediaStream& iStream;
    TState iState;
    RStringPool iStringPool;
    const TStringTable* iStringTable;
    TPreconditionType iType;


protected: //data

    RStringF iKAttributeCurr;
    RStringF iKAttributeDes;
    RStringF iKAttributeConf;
    RStringF iKAttributeNone;
    RStringF iKAttributeSend;
    RStringF iKAttributeRecv;
    RStringF iKAttributeSendRecv;
    RStringF iKAttributeRemote;
    RStringF iKAttributeLocal;
    RStringF iKAttributeMandatory;
    RStringF iKAttributeOptional;
    RStringF iKAttributeFailure;
    RStringF iKAttributeUnknown;
    RStringF iKAttributeEnd2End;

private:
	friend class TMcePreconditionsFactory;    
        
    };
    
    
/**
 * 
 *
 * @lib 
 */
class TMceSegmentedPreconditions : public TMcePreconditions
    {


public: // Constructors & Destructor


    /**
     * C++ default constructor.
     */
    TMceSegmentedPreconditions( CMceComMediaStream& aStream );
                                
    
public: // from TMcePreconditions

    void EncodeL( CSdpMediaField& aMedia );
    TMceSipWarningCode DecodeL( CSdpMediaField& aMedia );
    void Reserved();
    TBool ReservationNeeded() const;
    TBool RemoteReservationNeeded() const;
    TMcePreconditions* CloneL( CMceComMediaStream& aStream );

private: //from TMcePreconditions

    void DoConstructL();

private: //encode

    void EncodeCurrentStatusL( CSdpMediaField& aMedia,
                               RStringF aAccessNetwork ) const;
    void EncodeDesiredStatusL( CSdpMediaField& aMedia,
                               RStringF aAccessNetwork ) const;
    void EncodeConfirmationL( CSdpMediaField& aMedia );

    HBufC8* CurrentStausTextLC( const TDesC8& aAccessNetwork, 
                                const TDesC8& aMediaDirection ) const;
    HBufC8* DesiredStausTextLC( const TDesC8& aStrength, 
                                const TDesC8& aAccessNetwork, 
                                const TDesC8& aMediaDirection ) const;
                                
private: //decode
    
    TUint DecodeCurrentStatusL( CSdpAttributeField& aAttribute );
    TUint DecodeDesiredStatusL( CSdpAttributeField& aAttribute );
    TUint DecodeConfStatusL( CSdpAttributeField& aAttribute );
    TInt ParseCurrentStatus( const TDesC8& aLine,
                             TPtrC8& aAccessNetwork,
                             TPtrC8& aMediaDirection );
    TInt ParseDesiredStatus( const TDesC8& aLine,
                             TPtrC8& aStrength,
                             TPtrC8& aAccessNetwork,
                             TPtrC8& aMediaDirection );
    
    RStringF DecodeMediaDirectionL( const TDesC8& aMediaDirection );
    RStringF DecodeStrengthL( const TDesC8& aStrength );

private:
    
    void SetState();
    void SetRemoteStatus( RStringF aDirection );
    RStringF DesiredDirection( RStringF aAccessNetwork );
    void SetDesiredStatus( RStringF aAccessNetwork, 
                           RStringF aDirection, 
                           RStringF sStrength );
    void ConstrucL( const TMceSegmentedPreconditions& aPreconditions );

    
private: //data

        
    RStringF iCurrentLocalStatus;
    RStringF iCurrentRemoteStatus;
    RStringF iDesiredLocalSend;
    RStringF iDesiredLocalRecv;
    RStringF iDesiredRemoteSend;
    RStringF iDesiredRemoteRecv;
    RStringF iConfirmation;

    
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS
    
    
    };

/**
 * 
 *
 * @lib 
 */
class TMceSecurePreconditions : public TMcePreconditions
    {


public: // Constructors & Destructor


    /**
     * C++ default constructor.
     */
    TMceSecurePreconditions( CMceComMediaStream& aStream );
                                
    
public: // from TMcePreconditions

    void EncodeL( CSdpMediaField& aMedia );
    TMceSipWarningCode DecodeL( CSdpMediaField& aMedia );
    void Reserved();
    TBool ReservationNeeded() const;
    TBool RemoteReservationNeeded() const;
    TMcePreconditions* CloneL( CMceComMediaStream& aStream );

private: //from TMcePreconditions

    void DoConstructL();

private: //encode

    void EncodeCurrentStatusL( CSdpMediaField& aMedia,
                               TMceNegotiationRole aRole );
    
    void EncodeDesiredStatusL( CSdpMediaField& aMedia );
    
    void EncodeConfirmationL( CSdpMediaField& aMedia );

    HBufC8* CurrentStatusTextLC( const TDesC8& aMediaDirection ) const;
    
    HBufC8* DesiredStatusTextLC( const TDesC8& aStrength, 
                                 const TDesC8& aMediaDirection ) const;
   
                                
protected: //decode
    
    TUint DecodeCurrentStatusL( CSdpAttributeField& aAttribute );
    
    TUint DecodeDesiredStatusL( CSdpAttributeField& aAttribute );
    
    TUint DecodeConfStatusL( CSdpAttributeField& aAttribute );
    
    TBool ParseCurrentStatus( const TDesC8& aLine,
                              TPtrC8& aMediaDirection );
                             
    TBool ParseDesiredStatus( const TDesC8& aLine,
                              TPtrC8& aStrength,
                              TPtrC8& aMediaDirection );
    
    RStringF DecodeMediaDirectionL( const TDesC8& aMediaDirection );
    
    RStringF DecodeStrengthL( const TDesC8& aStrength );

private:
    
    void SetState();
    
    void SetRemoteStatus( RStringF aDirection );
    
    void SetDesiredStatus( RStringF aEntity, RStringF sStrength );
                           
 	TBool FindSecPreconds( CSdpMediaField& aMedia );
 	
 	TBool StrengthDowngraded( RStringF aStrength );
    
    void ConstrucL( const TMceSecurePreconditions& aPreconditions );
    
private: //data

   	RStringF iCurrentLocalStatus;
    RStringF iCurrentRemoteStatus;
    RStringF iDesiredLocalSendRecv;
    RStringF iDesiredRemoteSendRecv;
    RStringF iConfirmation;
  
	//definitions for unit testing
	MCEMM_UT_DEFINITIONS 
    };


class TMcePreconditionsFactory
	{
	public:
	TMcePreconditions* CreateL( CMceComMediaStream& aMediaStream, CSdpMediaField& aMediaLine );
	TMcePreconditions* CreateL( CMceComMediaStream& aMediaStream );
	
	TInt iDummy;
	};

#endif //MCEPRECONDITIONS_H
