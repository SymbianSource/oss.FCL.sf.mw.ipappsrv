/*
* ==============================================================================
*  Name        : exorts.inc
*  Part of     : TestClient
*  Description : TestClient exports definition file
*  Version     : $Version: 0 $, $Revision: 4 $
*
*  Copyright © 2004 Nokia Corporation.
*  This material, including documentation and any related
*  computer programs, is protected by copyright controlled by
*  Nokia Corporation. All rights are reserved. Copying,
*  including reproducing, storing, adapting or translating, any
*  or all of this material requires the prior written consent of
*  Nokia Corporation. This material also contains confidential
*  information which may not be disclosed to others without the
*  prior written consent of Nokia Corporation.
* ==============================================================================
*/

#include <bldvariant.hrh>

#if defined __S60_50__
#include <platform_paths.hrh>
#else 
#include <domain/osextensions/platform_paths.hrh>
#endif

tcuid.h \epoc32\include\testclient\tcuid.h

// TesterCore exports
..\TesterCore\Inc\CTcArray.h \epoc32\include\testclient\CTcArray.h
..\TesterCore\Inc\CTcContextBase.h \epoc32\include\testclient\CTcContextBase.h 
..\TesterCore\Inc\CTcCTRLCodec.h \epoc32\include\testclient\CTcCTRLCodec.h
..\TesterCore\Inc\CTcNameValue.h \epoc32\include\testclient\CTcNameValue.h 
..\TesterCore\Inc\CTcParameterList.h \epoc32\include\testclient\CTcParameterList.h
..\TesterCore\Inc\CTcRegistry.h \epoc32\include\testclient\CTcRegistry.h
..\TesterCore\Inc\CTcStructure.h \epoc32\include\testclient\CTcStructure.h
..\TesterCore\Inc\CTcTestRunner.h \epoc32\include\testclient\CTcTestRunner.h
..\TesterCore\Inc\CTcContextFactory.h \epoc32\include\testclient\CTcContextFactory.h 
..\TesterCore\Inc\MTcTestContext.h \epoc32\include\testclient\MTcTestContext.h
..\TesterCore\Inc\MTcTestObserver.h \epoc32\include\testclient\MTcTestObserver.h
..\TesterCore\Inc\MTcTyped.h \epoc32\include\testclient\MTcTyped.h 
..\TesterCore\Inc\TcLog.h \epoc32\include\testclient\TcLog.h
..\TesterCore\Inc\TesterConstants.h \epoc32\include\testclient\TesterConstants.h
..\TesterCore\Inc\TTcCommandBase.h \epoc32\include\testclient\TTcCommandBase.h 
..\TesterCore\Inc\TTcIdentifier.h \epoc32\include\testclient\TTcIdentifier.h

// Net exports
..\Net\Inc\CTcBearer.h \epoc32\include\testclient\CTcBearer.h
..\Net\Inc\CTcIAPManager.h \epoc32\include\testclient\CTcIAPManager.h
..\Net\Inc\MTcBearerFactory.h \epoc32\include\testclient\MTcBearerFactory.h 
..\Net\Inc\MTcBearerObserver.h \epoc32\include\testclient\MTcBearerObserver.h
..\Net\Inc\TTcBtFactory.h \epoc32\include\testclient\TTcBtFactory.h
..\Net\Inc\TTcTcpFactory.h \epoc32\include\testclient\TTcTcpFactory.h

// FileHandler exports
..\FileHandler\Inc\CTcFileHandler.h \epoc32\include\testclient\CTcFileHandler.h
..\FileHandler\Inc\RTcFileHandler.h \epoc32\include\testclient\RTcFileHandler.h

// Iby-file export
..\..\rom\siptestdriver.iby CORE_MW_LAYER_IBY_EXPORT_PATH(siptestdriver.iby)

