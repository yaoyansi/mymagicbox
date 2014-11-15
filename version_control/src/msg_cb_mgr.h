#pragma once

#include <maya/MCallbackIdArray.h>

class MessageCallbackMgr
{
public:
    MessageCallbackMgr();
    ~MessageCallbackMgr();

    MStatus regist();
    MStatus deregist();

private:
    MCallbackIdArray m_callback_ids;

protected:
    MessageCallbackMgr(const MessageCallbackMgr& o);
    MessageCallbackMgr& operator=(const MessageCallbackMgr& o);
};
