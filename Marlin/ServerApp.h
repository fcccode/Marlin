/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: ServerApp.h
//
// Marlin Server: Internet server/client
// 
// Copyright (c) 2015-2018 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#pragma once
#include "HTTPServerIIS.h"
#include "IISSiteConfig.h"
#include "ThreadPool.h"
#include "HTTPLoglevel.h"
#include "Analysis.h"
#include "ErrorReport.h"
#include <set>

// To prevent bug report from the Windows 8.1 SDK
#pragma warning (disable:4091)
#include <httpserv.h>
#pragma warning (error:4091)

using IISModules = std::set<CString>;

class ServerApp
{
public:
  ServerApp(IHttpServer* p_iis,CString p_appName,CString p_webroot);
  virtual ~ServerApp();

  // Starting and stopping the server
  virtual void InitInstance();
  virtual void ExitInstance();
  virtual bool LoadSite(IISSiteConfig& p_config);
  virtual ErrorReport* GetErrorReport();

  // The performance counter
  virtual void StartCounter();
  virtual void StopCounter();

  // Setting the logging level
  virtual void SetLogLevel(int p_logLevel);

  // Start our sites from the IIS configuration
  virtual void LoadSites(IHttpApplication* p_app,CString p_physicalPath);

  // Server app was correctly started by MarlinIISModule
  virtual bool CorrectlyStarted();

  // GETTERS
  // Never virtual. Derived classes should use these!!
  HTTPServerIIS* GetHTTPServer()  { return m_httpServer;  };
  ThreadPool*    GetThreadPool()  { return m_threadPool;  };
  LogAnalysis*   GetLogfile()     { return m_logfile;     };
  int            GetLogLevel()    { return m_logLevel;    };
  IISSiteConfig* GetSiteConfig(int ind);

protected:
  // Start the logging file for this application
  void  StartLogging();

  // Read the site's configuration from the IIS internal structures
  bool  ReadSite    (IAppHostElementCollection* p_sites,CString p_site,int p_num,IISSiteConfig& p_config);
  bool  ReadBinding (IAppHostElementCollection* p_bindings,int p_item,IISBinding& p_binding);
  void  ReadModules (CComBSTR& p_configPath);
  void  ReadHandlers(CComBSTR& p_configPath,IISSiteConfig& p_config);

  // General way to read a property
  CString GetProperty(IAppHostElement* p_elem,CString p_property);

  // DATA
  CString        m_applicationName;             // Name of our application / IIS Site
  CString        m_webroot;                     // WebRoot of our application
  IISModules     m_modules;                     // Global IIS modules for this application
  IISSiteConfigs m_sites;                       // Configures sites, modules and handlers
  IHttpServer*   m_iis          { nullptr };    // Main ISS application
  HTTPServerIIS* m_httpServer   { nullptr };    // Our Marlin HTTPServer for IIS
  ThreadPool*    m_threadPool   { nullptr };    // Pointer to our own ThreadPool
  LogAnalysis*   m_logfile      { nullptr };    // Logfile object
  ErrorReport*   m_errorReport  { nullptr };    // Error reporting object
  bool           m_ownReport    { false   };    // Owning the error report
  int            m_logLevel     { HLL_NOLOG };  // Logging level of server and logfile
};

// Factory for your application to create a new class derived from the ServerApp
// Implement your own server app factory or use this default one
class ServerAppFactory
{
public:
  ServerAppFactory();

  virtual ServerApp* CreateServerApp(IHttpServer* p_iis,CString p_appName,CString p_webroot);
};

extern ServerAppFactory* appFactory;

