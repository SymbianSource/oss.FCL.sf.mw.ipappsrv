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




#ifndef MCEFACTORY_H
#define MCEFACTORY_H

#include <e32base.h> 
#include <s32strm.h>

#include "mcemediastream.h"
#include "mcemediasink.h"
#include "mcemediasource.h"
#include "mceaudiocodec.h"
#include "mcevideocodec.h"
#include "mcecomsession.h"
#include "mcedefs.h"
#include "mceclientserver.h"
#include "mcecomserializationcontext.h"

class CMceMediaSource;
class CMceMediaSink;
class CMceSession;
class CMceMsgBase;
class CMceStreamBundle;
class MMceComSerializable;


class TMceMediaStreamFactory
	{

public:
    
    CMceMediaStream* CreateLC( TMceMediaType aType );	

    CMceMediaStream* CreateLC( MMceComSerializationContext& aSerCtx );
    

	};

class TMceSourceFactory
	{

public:
    
    CMceMediaSource* CreateLC( TMceSourceType aType );	

    CMceMediaSource* CreateLC( MMceComSerializationContext& aSerCtx );	

	};

class TMceSinkFactory
	{

public:
    
    CMceMediaSink* CreateLC( TMceSinkType aType );	

    CMceMediaSink* CreateLC( MMceComSerializationContext& aSerCtx );
    
    void CreateLC( MMceComSerializable*& aObject, MMceComSerializationContext& aSerCtx );

	};

class TMceAudioCodecFactory
	{

public:
    
    CMceAudioCodec* CreateL( TBuf8<KMceMaxSdpNameLength> aSdpName );	
    
    CMceAudioCodec* CreateLC( TBuf8<KMceMaxSdpNameLength> aSdpName );

    CMceAudioCodec* CreateLC( MMceComSerializationContext& aSerCtx );	

	};

class TMceVideoCodecFactory
	{

public:
    
    CMceVideoCodec* CreateL( TBuf8<KMceMaxSdpNameLength> aSdpName );
    
    CMceVideoCodec* CreateLC( TBuf8<KMceMaxSdpNameLength> aSdpName );	

    CMceVideoCodec* CreateLC( MMceComSerializationContext& aSerCtx );	

	};

class TMceFactory
    {
    
public:
    

    CMceSession* CreateLC( CMceComSession::TType aType );

    CMceSession* CreateLC( MMceComSerializationContext& aSerCtx );

    CMceMsgBase* CreateL( TMceItcDataType aType,
                          RReadStream& aReadStream );

    CMceMsgBase* CreateL( TMceItcDataType aType,
                          MMceComSerializationContext& aSerCtx );
    
    CMceStreamBundle* CreateBundleLC( CMceSession& aSession, 
                                      MMceComSerializationContext& aSerCtx );

    inline static TMceMediaStreamFactory MediaStreamFactory()
        {
        return TMceMediaStreamFactory();
        }
    
    inline TMceSourceFactory SourceFactory()
        {
        return TMceSourceFactory();
        }

    inline TMceSinkFactory SinkFactory()
        {
        return TMceSinkFactory();
        }
        
    inline TMceAudioCodecFactory AudioCodecFactory()
        {
        return TMceAudioCodecFactory();
        }
        
    inline TMceVideoCodecFactory VideoCodecFactory()
        {
        return TMceVideoCodecFactory();
        }   
   	TInt iDummy; 
    };


#endif 