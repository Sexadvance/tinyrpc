#ifndef ROCKET_NET_TCP_SERVER_H
#define ROCKET_NET_TCP_SERVER_H

#include <set>
#include "rocket/net/tcp/tcp_acceptor.h"
#include "rocket/net/tcp/tcp_connection.h"
#include "rocket/net/tcp/net_addr.h"
#include "rocket/net/eventloop.h"
#include "rocket/net/io_thread_group.h"

namespace rocket
{
class TcpServer
{
public:
    TcpServer(NetAddr::s_ptr local_addr);

    ~TcpServer();

    void start();

    void stop();
private:
    void init();

    //当有新客户端连接之后需要执行
    void onAccept();
    
private:
    TcpAcceptor::s_ptr m_acceptor;

    NetAddr::s_ptr m_local_addr;       //本地监听地址

    EventLoop* m_main_eventloop{NULL};    //mainReactor

    IOThreadGroup* m_io_thread_group{NULL};  //subReactor

    FdEvent* m_listen_fd_event;

    int m_client_counts{0};

    std::set<TcpConnection::s_ptr> m_client;
};

 


}


#endif