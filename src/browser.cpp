#include "pch.h"
#include "browser.h"

void Browser::OpenLink(const char *link)
{
    ShellExecute(nullptr, "open", link, nullptr, nullptr, SW_SHOWNORMAL);
}

cHandle Browser::AddDownloadRequest(const char* link)
{
    Request req {m_nReqCount++, link, eConnectionState::Queued};
    m_nRequests.push_back(std::move(req));

    return m_nRequests.back().m_nHandle;
}

Browser::eConnectionState Browser::GetDownloadRequestState(cHandle handle)
{
    for (auto e : m_nRequests)
    {
        if (e.m_nHandle == handle)
        {
            return e.m_eState;
        }
    }
    return Browser::eConnectionState::NoConnection;
}

void Browser::HandleDownloadRequests()
{
    for (auto e : m_nRequests)
    {
        if (e.m_eState != eConnectionState::Finished)
        {
            HRESULT res = URLDownloadToFile(NULL, e.m_URL.c_str(), "./TESTS/", 0, NULL);

            if (res == E_OUTOFMEMORY || res == INET_E_DOWNLOAD_FAILURE)
            {
                e.m_eState = eConnectionState::Error;
                return;
            }
        }
    }
}

