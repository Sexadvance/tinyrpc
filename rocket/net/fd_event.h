#ifndef ROCKET_NET_FDEVENT_H
#define ROCKET_NET_FDEVENT_H

#include <functional>
#include <sys/epoll.h>



namespace rocket
{
    
class FdEvent
{
public:
    enum TriggerEvent
    {
        IN_EVENT = EPOLLIN,
        OUT_EVENT = EPOLLOUT,
    };

    FdEvent(int fd);

    FdEvent();

    ~FdEvent();

    std::function<void()> handler(TriggerEvent event);

    void listen(TriggerEvent event,std::function<void()> callback);

    int getFd ()const 
    {
        return m_fd;
    }

    epoll_event getEpollEvent()
    {
        return m_listen_events;
    }

protected:
    int m_fd{-1};
    std::function<void()> m_read_callback;
    std::function<void()> m_write_callback;

    epoll_event m_listen_events;

};



}

#endif