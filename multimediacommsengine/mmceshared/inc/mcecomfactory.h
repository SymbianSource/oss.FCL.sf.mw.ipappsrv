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




#ifndef MCECOMFACTORY_H
#define MCECOMFACTORY_H

#include <e32base.h> 
#include <s32strm.h>
#include "mcedefs.h"
#include "mcemediastream.h"
#include "mcemediasource.h"
#include "mcemediasink.h"
#include "mceclientserver.h"
#include "mcecomserializationcontext.h"

class CMceComMediaStream;
class CMceComMediaSource;
class CMceComMediaSink;
class CMceComAudioCodec;
class CMceComVideoCodec;
class CMceComSession;
class CMceMsgBase;
class CMceComStreamBundle;
class MMceComSerializable;

class TMceComMediaStreamFactory
	{

public:
    
    CMceComMediaStream* CreateLC( TMceMediaType aType );	

    CMceComMediaStream* CreateLC( MMceComSerializationContext& aSerCtx );	

	};

class TMceComSourceFactory
	{

public:
    
    CMceComMediaSource* CreateLC( TMceSourceType aType );	

    CMceComMediaSource* CreateLC( MMceComSerializationContext& aSerCtx );	

	};

class TMceComSinkFactory
	{

public:
    
    CMceComMediaSink* CreateLC( TMceSinkType aType );	

    CMceComMediaSink* CreateLC( MMceComSerializationContext& aSerCtx );	

	};

class TMceComAudioCodecFactory
	{

public:
    
    //might leave KErrNotSupported
    CMceComAudioCodec* CreateLC( const TBuf8<KMceMaxSdpNameLength> aSdpName  );	

    CMceComAudioCodec* CreateLC( MMceComSerializationContext& aSerCtx );	
    
    //return NULL, if codec cannot been created
    CMceComAudioCodec* CreateCodecLC( const TBuf8<KMceMaxSdpNameLength> aSdpName  );	

    //return NULL, if codec cannot been created
    CMceComAudioCodec* CreateCodecLC( TUint aPayload );
    
    TInt iDummy;

	};

class TMceComVideoCodecFactory
	{

public:
    
    CMceComVideoCodec* CreateLC( const TBuf8<KMceMaxSdpNameLength> aSdpName  );	

    CMceComVideoCodec* CreateLC( MMceComSerializationContext& aSerCtx );	
    
    //return NULL, if codec cannot been created
    CMceComVideoCodec* CreateCodecLC( const TBuf8<KMceMaxSdpNameLength> aSdpName  );	

    //return NULL, if codec cannot been created
    CMceComVideoCodec* CreateCodecLC( TUint aPayload );
    
    TInt iDummy;

	};

class TMceComFactory
    {
    
public:

    CMceComSession* CreateLC( MMceComSerializationContext& aSerCtx );

    CMceMsgBase* CreateL( TMceItcDataType aType,
                          RReadStream& aReadStream );
    
    CMceMsgBase* CreateL( TMceItcDataType aType,
                          MMceComSerializationContext& aSerCtx );


    CMceComStreamBundle* CreateBundleLC( CMceComSession& aSession, 
                                         MMceComSerializationContext& aSerCtx );
                                         
    void CreateLC( MMceComSerializable*& aObject, MMceComSerializationContext& aSerCtx );
                                         

    inline TMceComMediaStreamFactory MediaStreamFactory()
        {
        return TMceComMediaStreamFactory();
        }

    inline TMceComSourceFactory SourceFactory()
        {
        return TMceComSourceFactory();
        }

    inline TMceComSinkFactory SinkFactory()
        {
        return TMceComSinkFactory();
        }

    inline TMceComAudioCodecFactory AudioCodecFactory()
        {
        return TMceComAudioCodecFactory();
        }
    
    inline TMceComVideoCodecFactory VideoCodecFactory()
        {
        return TMceComVideoCodecFactory();
        }
    
    TInt iDummy;
    
    };


#endif 