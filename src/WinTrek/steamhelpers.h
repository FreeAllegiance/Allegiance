#pragma once

// helper class for steam callbacks
template <typename ResponseType>
class SteamCallback {

    CCallResult<SteamCallback, ResponseType> m_response;
    bool m_bError;
    bool m_bDone;

    SteamAPICall_t m_handle;

protected:

    SteamCallback(SteamAPICall_t handle) :
        m_bError(false),
        m_bDone(false),
        m_handle(handle)
    {
        m_response.Set(m_handle, this, &SteamCallback::OnCallback);
    }

    virtual void OnSuccess(ResponseType* pResponse) {};
    virtual void OnError(ResponseType* pResponse) {};

public:
    bool IsSuccess() {
        return m_bDone == true && m_bError == false;
    }

    bool isDone() {
        return m_bDone;
    }

    void OnCallback(ResponseType* pResponse, bool bIOFailure) {
        m_bDone = true;
        m_bError = bIOFailure;

        if (m_bError == false) {
            OnSuccess(pResponse);
        }
        else {
            OnError(pResponse);
        }
    }

    bool Wait() {
        SteamAPI_RunCallbacks();
        for (int i = 0; i < 30 && m_bDone == false; i++)
        {
            Sleep(100);
            SteamAPI_RunCallbacks();
        }

        if (m_bDone == false) {
            debugf("Steam query took too long, cancelling");
            m_response.Cancel();
        }

        if (m_bError == true || m_bDone == false) {
            debugf("Steam query had an error");
            return false;
        }
        return true;
    }
};