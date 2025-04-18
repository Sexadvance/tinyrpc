#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rocket/common/msg_id_util.h"
#include "rocket/common/log.h"

namespace rocket
{

static int g_msg_id_length = 20;
static int g_random_fd = -1;


static thread_local std::string t_msg_id_num;
static thread_local std::string t_max_msg_id_num;

std::string MsgIDUtil::GenMsgID()
{
    if(t_msg_id_num.empty() || t_msg_id_num == t_max_msg_id_num)
    {
        if(g_random_fd == -1)
        {
            g_random_fd = open("/dev/urandom",O_RDONLY);
        }
        std::string res(g_msg_id_length,0);
        if( (read(g_random_fd,&res[0],g_msg_id_length)) != g_msg_id_length )
        {
            ERRORLOG("read from /dev/urandom error");
            return "";
        }

        for(int i = 0;i < g_msg_id_length; i++)
        {
            uint8_t x = (uint8_t)(res[i]) % 10;
            res[i] = x + '0';
            t_max_msg_id_num += "9";
        }
        t_msg_id_num = res;
    }
    else
    {
        int i = t_msg_id_num.length() - 1;
        while(t_msg_id_num[i] == '9' && i >=0)
        {
            i--;
        }
        if(i >= 0)
        {
            t_msg_id_num[i] +=1;
            for(size_t j = i + 1;j < t_msg_id_num.length(); j++)
            {
                t_msg_id_num[j] = '0';
            }
        }
    }

    return t_msg_id_num;
}

}