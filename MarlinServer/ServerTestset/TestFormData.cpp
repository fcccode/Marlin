/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestFormData.cpp
//
// Marlin Server: Internet server/client
// 
// Copyright (c) 2016 ir. W.E. Huisman
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
#include "stdafx.h"
#include "TestServer.h"
#include "SiteHandlerFormData.h"
#include "MultiPartBuffer.h"

static int totalChecks = 4;

//////////////////////////////////////////////////////////////////////////
//
// Form Data POST handler
//
//////////////////////////////////////////////////////////////////////////

class FormDataHandler : public SiteHandlerFormData
{
protected:
  int PreHandleBuffer(MultiPartBuffer* p_buffer);
  int HandleData(MultiPart* p_part);
  int HandleFile(MultiPart* p_part);
  int PostHandleBuffer(MultiPartBuffer* p_buffer);
private:
  int m_parts { 0 };
};

int 
FormDataHandler::PreHandleBuffer(MultiPartBuffer* p_buffer)
{
  // Resetting the m_parts counter to the number of parts received
  m_parts = (int) p_buffer->GetParts();
  --totalChecks;
  return 0;
}

int 
FormDataHandler::HandleData(MultiPart* p_part)
{
  SITE_DETAILLOGS("Handling form-data data-part: ",p_part->GetName());

  CString data = p_part->GetData();
  xprintf("MULTI-PART DATA = Name : %s\n",(LPCTSTR)p_part->GetName());
  xprintf("MULTI-PART Content-type: %s\n",(LPCTSTR)p_part->GetContentType());
  xprintf("MULTI-PART\n%s\n",             (LPCTSTR)p_part->GetData());

  // Remember the fact that we where called
  if(!data.IsEmpty())
  {
    --m_parts;
    --totalChecks;
  }

  // SUMMARY OF THE TEST
  // ---- "---------------------------------------------- - ------
  qprintf("Multi-part formdata - data part                : %s\n",data.IsEmpty() ? "ERROR" : "OK");
  return data.IsEmpty() ? 1 : 0;
}

int 
FormDataHandler::HandleFile(MultiPart* p_part)
{
  SITE_DETAILLOGV("Handling form-data file-part: [%s] %s",p_part->GetName(),p_part->GetFileName());

  xprintf("MULTI-PART FILE = Name : %s\n",(LPCTSTR)p_part->GetName());
  xprintf("MULTI-PART Content-type: %s\n",(LPCTSTR)p_part->GetContentType());
  xprintf("MULTI-PART Filename    : %s\n",(LPCTSTR)p_part->GetFileName());
  xprintf("File date creation     : %s\n",(LPCTSTR)p_part->GetDateCreation());
  xprintf("File date modification : %s\n",(LPCTSTR)p_part->GetDateModification());
  xprintf("File date last-read    : %s\n",(LPCTSTR)p_part->GetDateRead());
  xprintf("File indicated size    : %d\n",(int)    p_part->GetSize());

  // Keep debugging things together, by resetting the filename
  CString filename = WebConfig::GetExePath() + p_part->GetFileName();
  p_part->SetFileName(filename);
  // Re-write the file part buffer + optional file times.
  bool result = p_part->WriteFile();

  // Remember the fact that we where called
  m_parts -= result ? 1 : 0;

  // Check done
  if(result)
  {
    --totalChecks;
  }
  // SUMMARY OF THE TEST
  // ---- "---------------------------------------------- - ------
  qprintf("Multi-part formdata - file part                : %s\n",result ? "OK" : "ERROR");
  return result ? 0 : 1;
}

int 
FormDataHandler::PostHandleBuffer(MultiPartBuffer* p_buffer)
{
  // Essentially, test if all parts where received!
  // By checking the m_parts counter in the class
  bool result = m_parts == 0 && p_buffer->GetParts() == 2;

  // Check done
  if(result)
  {
    --totalChecks;
  }
  // SUMMARY OF THE TEST
  // --- "---------------------------------------------- - ------
  qprintf("Multi-part formdata - total test               : %s\n",result ? "OK" : "ERROR");
  return 0;  
}

//////////////////////////////////////////////////////////////////////////
//
// TESTING
//
//////////////////////////////////////////////////////////////////////////

int TestFormData(HTTPServer* p_server)
{
  int error = 0;

  // If errors, change detail level
  doDetails = false;

  CString url("/MarlinTest/FormData/");

  xprintf("TESTING FORM-DATA MULTI-PART-BUFFER FUNCTION OF THE HTTP SERVER\n");
  xprintf("===============================================================\n");

  // Create HTTP site to listen to "http://+:port/MarlinTest/FormData/"
  HTTPSite* site = p_server->CreateSite(PrefixType::URLPRE_Strong,false,TESTING_HTTP_PORT,url);
  if(site)
  {
    // SUMMARY OF THE TEST
    // --- "--------------------------- - ------\n"
    qprintf("HTTPSite multipart/form-data: OK : %s\n",site->GetPrefixURL().GetString());
  }
  else
  {
    ++error;
    xerror();
    qprintf("ERROR: Cannot make a HTTP site for: %s\n",(LPCTSTR)url);
    return error;
  }

  // Setting the POST handler for this site
  site->SetHandler(HTTPCommand::http_post,new FormDataHandler());

  // Modify the standard settings for this site
  site->AddContentType("","multipart/form-data;");

  // Start the site explicitly
  if(site->StartSite())
  {
    xprintf("Site started correctly: %s\n",url);
  }
  else
  {
    ++error;
    xerror();
    qprintf("ERROR STARTING SITE: %s\n",(LPCTSTR)url);
  }
  return error;
}

int
AfterTestFormData()
{
  // SUMMARY OF THE TEST
  // ---- "---------------------------------------------- - ------
  qprintf("Form-data multi-buffer test complete           : %s\n",totalChecks > 0 ? "ERROR" : "OK");
  return totalChecks > 0;
}