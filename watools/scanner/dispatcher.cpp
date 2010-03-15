#include <signal.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#ifdef WIN32
#include <errno.h>
#include <winsock2.h>
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif //WIN32

#include <log4cxx/logger.h>

#include "../common/redisclient.h"

#include "version.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

namespace po = boost::program_options;
namespace bfs = boost::filesystem;
namespace btm = boost::posix_time;

// global variables
bool inLoop = true;
enum _keep_alive_fileds {
    ip_addr = 0,
    instance,
    scanner_name,
    timeout,
    status,

    last_keep_alive_field
} keep_alive_fileds;
string keep_alive_packet[last_keep_alive_field];

extern LoggerPtr scan_logger;
extern redis::client* db1_client;
extern boost::mutex db1_lock;
extern string scaner_uuid;
extern string db1_instance_name;

void signal_halt(int sig)
{
    inLoop = false;
    //! todo: stop all tasks
    if (sig == 0) {
        LOG4CXX_INFO(scan_logger, "Received software request to exit");
    }
    else {
        LOG4CXX_INFO(scan_logger, "Signal received - exit scanner");
    }
} 

void send_keepalive(int timeout) {
    LOG4CXX_DEBUG(scan_logger, "Send keep-alive signal");
    try {
        if (db1_client->exists(db1_instance_name)) {
            db1_client->del(db1_instance_name);
        }
        for (int i = 0; i < last_keep_alive_field; i++) {
            db1_client->rpush(db1_instance_name, keep_alive_packet[i]);
        }
        db1_client->expire(db1_instance_name, timeout);
    } catch(redis::redis_error& re) {
        LOG4CXX_FATAL(scan_logger, "Can't save keep-alive information. Redis error: " << (string)re);
    }
}

void send_sysinfo(int timeout) {
    LOG4CXX_DEBUG(scan_logger, "Send system information");
}

void dispatcher_routine(int inst, po::variables_map& vm)
{
    btm::ptime  keep_alive;
    btm::ptime  sys_info;
    btm::ptime  curr_time;
    int         keep_alive_timeout;
    int         sys_info_timeout;
    char        ac[80] = {0};
    string      queue_key;

    if (db1_client == NULL) {
        LOG4CXX_FATAL(scan_logger, "Redis client seems to be uninitialized! Exiting...");
        return;
    }
    // set signal processor
    signal(SIGINT, signal_halt);

    // init values 
    queue_key = "ScanModule:Queue:" + scaner_uuid + ":" + boost::lexical_cast<string>(inst);
    keep_alive_timeout = vm["keepalive_timeout"].as<int>();
    sys_info_timeout = vm["sysinfo_timeout"].as<int>();

    keep_alive_packet[ip_addr] = "";
    // find all ip's of machine
    if (gethostname(ac, sizeof(ac)) != SOCKET_ERROR) 
    {
        struct hostent *phe = gethostbyname(ac);
        if (phe != 0) 
        {
            for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
            {
                struct in_addr addr;
                string saddr;
                memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
                if ((addr.s_addr & 0xFF0000) != 0x7F0000) {
                    saddr  = boost::lexical_cast<string>((int)addr.s_net) + ".";
                    saddr += boost::lexical_cast<string>((int)addr.s_host) + ".";
                    saddr += boost::lexical_cast<string>((int)addr.s_lh) + ".";
                    saddr += boost::lexical_cast<string>((int)addr.s_impno);
                    if (keep_alive_packet[ip_addr].length() != 0 ) {
                        keep_alive_packet[ip_addr] += "; ";
                    }
                    LOG4CXX_TRACE(scan_logger, "Found ip addr: " << saddr);
                    keep_alive_packet[ip_addr] += saddr;
                    break;
                }
            }
        }
        else {
            LOG4CXX_ERROR(scan_logger, "gethostbyname failed");
            keep_alive_packet[ip_addr] = "<unknown>";
        }
    }
    else {
        LOG4CXX_ERROR(scan_logger, "gethostname failed");
        keep_alive_packet[ip_addr] = "<unknown>";
    }

    keep_alive_packet[instance] =  boost::lexical_cast<string>(inst);
    keep_alive_packet[scanner_name] = vm["scanner_name"].as<string>();
    keep_alive_packet[timeout] = boost::lexical_cast<string>(vm["keepalive_timeout"].as<int>());
    keep_alive_packet[status] = "READY";
    send_keepalive(keep_alive_timeout);

    send_sysinfo(sys_info_timeout);

    inLoop = true;
    keep_alive = btm::second_clock::local_time();
    sys_info = btm::second_clock::local_time();
    // run endless loop
    while(inLoop) {
        vector<string>  out;
        int             reply;

        boost::this_thread::sleep(boost::posix_time::milliseconds(500)); // :seconds(1));
        curr_time = btm::second_clock::local_time();
        if ( (keep_alive + btm::seconds(keep_alive_timeout)) <= curr_time) {
            keep_alive = curr_time;
            send_keepalive(keep_alive_timeout);
        }
        if ( (sys_info + btm::seconds(sys_info_timeout)) <= curr_time) {
            sys_info = curr_time;
            send_sysinfo(sys_info_timeout);
        }
        // check commands
        try {
            string cmd;
            cmd = db1_client->rpop(queue_key);
            while (cmd != redis::client::missing_value) {
                LOG4CXX_DEBUG(scan_logger, "Process command: " << cmd);
                if (cmd == "EXIT") {
                    signal_halt(0);
                    // need to exit and save other commands in the queue
                    break;
                }
                cmd = db1_client->rpop(queue_key);
            }
        } catch(redis::redis_error& re) {
            LOG4CXX_FATAL(scan_logger, "Can't get commands queue " << (string)re);
            signal_halt(0); 
        }
    }

}
