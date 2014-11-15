#include "msg_cb_mgr.h"

#include <maya/MMessage.h>
#include <maya/MSceneMessage.h>

#include <common/log.h>

///------------------------------------------------------------------
/// Callback Functions
///------------------------------------------------------------------
static void MSceneMessage_AfterOpen(void* clientData)
{
    LOG("test");
    //printf();
    fflush(stdout);
}
///------------------------------------------------------------------
/// MessageCallbackMgr
///------------------------------------------------------------------
MessageCallbackMgr::MessageCallbackMgr()
{

}
//
MessageCallbackMgr::~MessageCallbackMgr()
{

}
//
MStatus MessageCallbackMgr::regist()
{
    MStatus status;
    MCallbackId msgid;

    // kAfterOpen
    msgid = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, MSceneMessage_AfterOpen,this, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(m_callback_ids.append(msgid));

	return MS::kSuccess;
}
//
MStatus MessageCallbackMgr::deregist()
{
    MMessage::removeCallbacks(m_callback_ids);

    return MS::kSuccess;
}
