/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: SiteHandlerWebDAV.h
//
// Marlin Server: Internet server/client
// 
// Copyright (c) 2017 ir. W.E. Huisman
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
#include "SiteHandler.h"

class SiteHandlerWebDAV : public SiteHandler
{
protected:
  // Handlers: Override and return 'true' if handling is ready
  virtual bool  PreHandle(HTTPMessage* p_message) override;
  virtual bool     Handle(HTTPMessage* p_message) override;
  virtual void PostHandle(HTTPMessage* p_message) override;

  // WebDAV method handlers
  virtual void WebDavMove  (HTTPMessage* p_message);
  virtual void WebDavCopy  (HTTPMessage* p_message);
  virtual void WebDavPFind (HTTPMessage* p_message);
  virtual void WebDavPPatch(HTTPMessage* p_message);
  virtual void WebDavMkCol (HTTPMessage* p_message);
  virtual void WebDavLock  (HTTPMessage* p_message);
  virtual void WebDavUnlock(HTTPMessage* p_message);
  virtual void WebDavSearch(HTTPMessage* p_message);
};
