#include "myqueue.h"

CMyMessagQueue::CMyMessagQueue()
{
    m_msg = new std::queue< std::pair< int32_t, std::shared_ptr<std::string> > >;
    m_mutex = new std::mutex;
}
CMyMessagQueue::~CMyMessagQueue()
{
    delete m_msg;
    delete m_mutex;
}

void CMyMessagQueue::onPush(int32_t code_in, std::shared_ptr<std::string> msg_in)
{
    std::lock_guard<std::mutex> guard(*m_mutex);
    m_msg->push({ code_in,msg_in });
}
bool CMyMessagQueue::onEmpty()
{
    return m_msg->empty();
}
std::shared_ptr<std::string> CMyMessagQueue::onPop(int32_t& code_out)
{
    if (m_msg->empty())
        return NULL;

    std::lock_guard<std::mutex> guard(*m_mutex);
    code_out = m_msg->front().first;
    std::shared_ptr<std::string> re = m_msg->front().second;
    m_msg->pop();
    return re;
}
