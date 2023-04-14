#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__


#include <string>
#include <queue>
#include <mutex>


class FRAMEWORK_TOOL_API CMyMessagQueue
{
public:
    CMyMessagQueue();
    ~CMyMessagQueue();

    void onPush(int32_t code_in, std::shared_ptr<std::string> msg_in);
    bool onEmpty();
    std::shared_ptr<std::string> onPop(int32_t& code_out);

private:
    std::queue< std::pair< int32_t, std::shared_ptr<std::string> > >* m_msg;
    std::mutex* m_mutex;
};




#endif //__MYQUEUE_H__
