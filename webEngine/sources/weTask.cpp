/*
    webEngine is the HTML processing library
    Copyright (C) 2009 Andrew Abramov aabramov@ptsecurity.ru

    This file is part of webEngine

    webEngine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    webEngine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with webEngine.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <webEngine.h>

#include <weHelper.h>
#include <weTask.h>
#include <weiTransport.h>
#include <weiInventory.h>
#include <weiAudit.h>
#include <weiVulner.h>
#include <weScan.h>
#include <weiStorage.h>
#include <weDispatch.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/functional/hash.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <weConstants.h>

using namespace boost;

namespace webEngine {

    namespace{
static std::string task_params_str("!!params!!");
    }

#define SERIALIZE_TASK_PARAMS(arch) \
    arch & BOOST_SERIALIZATION_NVP(total_reqs) & BOOST_SERIALIZATION_NVP(profile_id);

#ifndef __DOXYGEN__
static const we_option empty_option("_empty_");
#endif //__DOXYGEN__

typedef struct _sync_req_data {
    boost::shared_ptr<boost::mutex> req_mutex;
    boost::shared_ptr<boost::condition_variable> req_event;
    boost::shared_ptr<i_response> response;
} sync_req_data;

void task_sync_request(boost::shared_ptr<i_response> resp, void* context)
{
    LOG4CXX_DEBUG(iLogger::GetLogger(), "task_sync_request - process response");
    sync_req_data *req_data = (sync_req_data*)context;

    boost::lock_guard<boost::mutex> lock(*(req_data->req_mutex.get()));
    req_data->response = resp;
    req_data->req_event->notify_all();
}

void task_processor(task* tsk)
{
    we_option opt;
    size_t i;
    int iData;

    tsk->processThread = true;
	LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessor started for task " << std::hex << (size_t)tsk);

    bool holded = false;
    while (tsk->IsReady() && !holded) {
        tsk->WaitForData();
        tsk->calc_status();

        holded = (tsk->tsk_status == WI_TSK_STOPPED || tsk->tsk_status == WI_TSK_SUSPENDED || tsk->tsk_status == WI_TSK_FINISHED || tsk->tsk_status == WI_TSK_PAUSED);

        opt = tsk->Option(weoParallelReq);
        SAFE_GET_OPTION_VAL(opt, iData, 1);
        tsk->taskQueueSize = iData;
        LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessor max requests: " << tsk->taskQueueSize <<
                      " in queue: " << tsk->taskQueue.size() << " waiting: " << tsk->taskList.size() << " done: " << tsk->total_done);

        // send request if slots available
        {
            boost::unique_lock<boost::mutex> lock(tsk->tsk_mutex);

            while (tsk->taskQueueSize > tsk->taskQueue.size() && tsk->tsk_status == WI_TSK_RUNNING && !tsk->taskList.empty()) {
                i_request_ptr curr_url = tsk->taskList[0];
                i_response_ptr resp;

                // search for transport or recreate request to all transports
                if (curr_url->RequestUrl().is_valid()) {
                    // search for transport
                    for(i = 0; i < tsk->transports.size(); i++) {
                        if (tsk->transports[i]->is_own_protocol(curr_url->RequestUrl().protocol)) {
                            resp = tsk->transports[i]->request(curr_url.get());
                            resp->ID(curr_url->ID());
                            resp->processor = curr_url->processor;
                            resp->context = curr_url->context;
                            tsk->taskQueue.push_back(resp);
                            LOG4CXX_DEBUG(iLogger::GetLogger(), "WeTaskProcessor: send request to " << curr_url->RequestUrl().tostring());
                            resp.reset();
                            break;
                        }
                    }
                    if ( i == tsk->transports.size()) {
                        LOG4CXX_WARN(iLogger::GetLogger(), "WeTaskProcessor: can't send request to " << curr_url->RequestUrl().tostring() <<
                                     " no transports accept this request!");
                        // need to send the "abort request" message to caller
                        resp = curr_url->abort_request();
                        resp->ID(curr_url->ID());
                        resp->processor = curr_url->processor;
                        resp->context = curr_url->context;
                        tsk->taskQueue.push_back(resp);
                    }
                } else {
                    // try to send request though appropriate transports
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "WeTaskProcessor: send request to " << curr_url->RequestUrl().tostring());
                    for(i = 0; i < tsk->transports.size(); i++) {
                        resp = tsk->transports[i]->request(curr_url.get());
                        resp->ID(curr_url->ID());
                        resp->processor = curr_url->processor;
                        resp->context = curr_url->context;
                        tsk->taskQueue.push_back(resp);
                    }
                }

                //string u_req = curr_url->RequestUrl().tostring();
                tsk->taskList.erase(tsk->taskList.begin());
                //curr_url.reset(); - automatically desctroy on list.erase
            }//while
        }

        // if any requests pending process transport operations
        if ( !holded && tsk->taskQueue.size() > 0) {
            LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessor: transport->process_requests()");
            for(i = 0; i < tsk->transports.size(); i++) {
                tsk->transports[i]->process_requests();
            }
            // just switch context to wake up other tasks
            boost::this_thread::sleep(boost::posix_time::millisec(100));
            for(response_list::iterator rIt = tsk->taskQueue.begin(); rIt != tsk->taskQueue.end();) {
                if ((*rIt)->Processed()) {
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "WeTaskProcessor: received! Queue: " << tsk->taskQueue.size() << " done: " << tsk->total_done);
                    tsk->total_done++;
                    if ((*rIt)->processor) {
                        // send to owner
                        LOG4CXX_DEBUG(iLogger::GetLogger(), "WeTaskProcessor: send response to owner");
                        (*rIt)->processor((*rIt), (*rIt)->context);
                    } else /*if (tsk->tsk_status == WI_TSK_RUNNING)*/ { // process response only if task is running
                        // process response by task queue
                        // 1. Parse data
                        i_document_ptr parsed_doc;

                        for (size_t i = 0; i < tsk->parsers.size(); i++) {
                            LOG4CXX_DEBUG(iLogger::GetLogger(), "WeTaskProcessor: parse response by " << tsk->parsers[i]->get_description());
                            parsed_doc = tsk->parsers[i]->parse(*rIt);
                            if (parsed_doc) {
                                // parsed!
                                LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessor: parsed!");
                                break;
                            }
                        }
                        // 2. Create ScanData
                        scan_data_ptr scd = tsk->get_scan_data((*rIt)->BaseUrl().tostring());
                        scd->response = *rIt;
                        if (scd->data_id == "") {
                            scd->data_id = tsk->kernel->storage()->generate_id(weObjTypeScan);
                        }
                        if (! (scd->parsed_data)) {
                            scd->parsed_data = parsed_doc;
                        }
                        // 3. Push to processing
                        tsk->set_scan_data((*rIt)->BaseUrl().tostring(), scd);
                    }
                    //(*rIt).reset();
                    rIt = tsk->taskQueue.erase(rIt);
                } else {
                    rIt++;
                }
            }
            LOG4CXX_DEBUG(iLogger::GetLogger(), "WeTaskProcessor: queue: " << tsk->taskQueue.size() << " done: " << tsk->total_done);
        }
    };
    tsk->processThread = false;
	LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessor finished for task " << std::hex << (size_t)tsk);
}

void task_data_process(task* tsk)
{
    scan_data_ptr sc_data;

	LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessData started for task " << std::hex << ((size_t)tsk));
    tsk->add_thread();
    while (tsk->sc_process.size() > 0) {
        {
            boost::unique_lock<boost::mutex> lock(tsk->scandata_mutex);
            sc_data = tsk->sc_process[0];
            tsk->sc_process.erase(tsk->sc_process.begin());
        }
        if (sc_data) {
            /*if (tsk->tsk_status == WI_TSK_RUN)*/ {
                LOG4CXX_TRACE(iLogger::GetLogger(), "task_data_process: response for " << sc_data->object_url << " id = " << sc_data->data_id );
                // send to all inventories
                for (size_t i = 0; i < tsk->inventories.size(); i++) {
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "task_data_process: send response to " << tsk->inventories[i]->get_description());
                    tsk->inventories[i]->process(tsk, sc_data);
                }
                // send to all auditors
                for (size_t i = 0; i < tsk->auditors.size(); i++) {
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "task_data_process: send response to " << tsk->auditors[i]->get_description());
                    tsk->auditors[i]->process(tsk, sc_data);
                }
                // send to all vulner detectors
                for (size_t i = 0; i < tsk->vulners.size(); i++) {
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "task_data_process: send response to " << tsk->vulners[i]->get_description());
                    tsk->vulners[i]->process(tsk, sc_data);
                }
            }
            /// @todo !!! REMOVE THIS!!! It's only debug! need to implement clever clean-up scheme
            tsk->free_scan_data(sc_data);
        } // if sc_data valid
    } // while sc_process.size > 0
    tsk->ping_tm = btm::second_clock::local_time();
    tsk->remove_thread();
    tsk->dataThread = false;
	LOG4CXX_TRACE(iLogger::GetLogger(), "WeTaskProcessData finished for task " << std::hex << ((size_t)tsk));
}

task::task(engine_dispatcher *krnl /*= NULL*/):need_lock(true)
{
    // set the default options
    kernel = krnl;
    processThread = false;
    dataThread = false;

    taskList.clear();
    taskQueue.clear();
    thread_count = 0;
    total_reqs = 0;
    total_done = 0;
    profile_id = "0";
    tsk_status = WI_TSK_INIT;
    tsk_completion = 0;

    parsers.clear();
    transports.clear();
    inventories.clear();
    auditors.clear();
    vulners.clear();

    LOG4CXX_TRACE(iLogger::GetLogger(), "task created");
}

task::task( task& cpy ):need_lock(true)
{
    kernel = cpy.kernel;
    total_reqs = cpy.total_reqs;
    total_done = cpy.total_done;
    profile_id = cpy.profile_id;
    tsk_status = cpy.tsk_status;
    tsk_completion = cpy.tsk_completion;

    parsers.assign(cpy.parsers.begin(), cpy.parsers.end());
    transports.assign(cpy.transports.begin(), cpy.transports.end());
    inventories.assign(cpy.inventories.begin(), cpy.inventories.end());
    auditors.assign(cpy.auditors.begin(), cpy.auditors.end());
    vulners.assign(cpy.vulners.begin(), cpy.vulners.end());

    LOG4CXX_TRACE(iLogger::GetLogger(), "task assigned");
}

task::~task()
{
    if (IsReady()) {
        Stop();
    }
    size_t i;
    for (i = 0; i < parsers.size(); i++) {
        parsers[i]->release();
    }
    for (i = 0; i < transports.size(); i++) {
        transports[i]->release();
    }
    for (i = 0; i < inventories.size(); i++) {
        inventories[i]->release();
    }
    for (i = 0; i < auditors.size(); i++) {
        auditors[i]->release();
    }
    for (i = 0; i < vulners.size(); i++) {
        vulners[i]->release();
    }
}

i_response_ptr task::get_request( i_request_ptr req )
{
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::get_request (WeURL)");
    i_response_ptr retval;
    boost::scoped_ptr<sync_req_data> rdata(new sync_req_data);
    rdata->req_mutex.reset(new boost::mutex);
    rdata->req_event.reset(new boost::condition_variable);
    rdata->response.reset();

    bool init_state = false;
    if(tsk_status == WI_TSK_INIT) {
        init_state = true;
        tsk_status = WI_TSK_RUNNING;
    }
    req->processor = task_sync_request;
    req->context   = (void*)(rdata.get());
    get_request_async(req);
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::get_request - waiting for response");
    boost::unique_lock<boost::mutex> lock(*rdata->req_mutex);
    while (!rdata->response) {
        rdata->req_event->wait(lock);
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::get_request - data received");
    if(init_state) {
        Stop();
        tsk_status = WI_TSK_INIT;
    }
    retval = rdata->response;
    return retval;
}

void task::check_task_processor()
{
    if (!processThread) {
        processThread = true;
        LOG4CXX_DEBUG(iLogger::GetLogger(), "task::get_request_async: create WeTaskProcessor");
        boost::thread process(task_processor, this);
    }
}

void task::get_request_async( i_request_ptr req )
{
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::get_request_async");
    if (tsk_status < WI_TSK_PAUSING) {
        check_task_processor();
    }
    if(tsk_status < WI_TSK_FINISHED){
        boost::scoped_ptr<boost::unique_lock<boost::mutex> > lock;
        if(need_lock)
            lock.reset(new boost::unique_lock<boost::mutex>(tsk_mutex) );
        if (req->processor != NULL) {
            taskList.insert(taskList.begin(), req);
        } else {
            taskList.push_back(req);
        }
        total_reqs++;
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::get_request_async: new task size=" << taskList.size());
    } else {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::get_request_async: task isn't running (status="<< (int)tsk_status << L"), skip the request to " << req->BaseUrl().tostring());
        req->abort_request();
    }
    tsk_event.notify_all();
    return;
}

bool task::IsReady()
{
    bool result = !(tsk_status == WI_TSK_STOPPED || tsk_status == WI_TSK_FINISHED);
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::IsReady - " << result);
    return result;
}

void task::add_plg_parser(i_parser* plugin)
{
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_parser");
    for (size_t i = 0; i < parsers.size(); i++) {
        if (parsers[i]->get_id() == plugin->get_id()) {
            // transport already in list
            LOG4CXX_DEBUG(iLogger::GetLogger(), "task::add_plg_parser - parser already in list");
            return;
        }
    }
    parsers.push_back(plugin);
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_parser: added " << plugin->get_description());
}

void task::add_plg_transport( i_transport* plugin )
{
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_transport");
    for (size_t i = 0; i < transports.size(); i++) {
        if (transports[i]->get_id() == plugin->get_id()) {
            // transport already in list
            LOG4CXX_DEBUG(iLogger::GetLogger(), "task::add_plg_transport - transport already in list");
            return;
        }
    }
    transports.push_back(plugin);
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_transport: added " << plugin->get_description());
}

void task::add_plg_inventory( i_inventory* plugin )
{
    int plgPrio = plugin->get_priority();
    int inPlace = -1;

    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_inventory");
    for (size_t i = 0; i < inventories.size(); i++) {
        if (inventories[i]->get_id() == plugin->get_id()) {
            // plugin already in list
            LOG4CXX_DEBUG(iLogger::GetLogger(), "task::add_plg_inventory - inventory already in list");
            return;
        }
        if (inventories[i]->get_priority() > plgPrio) {
            inPlace = i;
        }
    }
    if (inPlace > -1) {
        inventories.insert(inventories.begin() + inPlace, plugin);
    } else {
        inventories.push_back(plugin);
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_inventory: added " << plugin->get_description());
}

void task::add_plg_auditor( i_audit* plugin )
{
    int plgPrio = plugin->get_priority();
    int inPlace = -1;

    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_auditor");
    for (size_t i = 0; i < auditors.size(); i++) {
        if (auditors[i]->get_id() == plugin->get_id()) {
            // plugin already in list
            LOG4CXX_DEBUG(iLogger::GetLogger(), "task::add_plg_auditor - auditor already in list");
            return;
        }
        if (auditors[i]->get_priority() > plgPrio) {
            inPlace = i;
        }
    }
    if (inPlace > -1) {
        auditors.insert(auditors.begin() + inPlace, plugin);
    } else {
        auditors.push_back(plugin);
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_auditor: added " << plugin->get_description());
}

void task::add_plg_vulner( i_vulner* plugin )
{
    int plgPrio = plugin->get_priority();
    int inPlace = -1;

    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_vulner");
    for (size_t i = 0; i < vulners.size(); i++) {
        if (vulners[i]->get_id() == plugin->get_id()) {
            // plugin already in list
            LOG4CXX_DEBUG(iLogger::GetLogger(), "task::add_plg_vulner - vulner already in list");
            return;
        }
        if (vulners[i]->get_priority() > plgPrio) {
            inPlace = i;
        }
    }
    if (inPlace > -1) {
        vulners.insert(vulners.begin() + inPlace, plugin);
    } else {
        vulners.push_back(plugin);
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_plg_vulner: added " << plugin->get_description());
}

void task::store_plugins(vector<i_plugin*>& plugins)
{
    string_list::iterator trsp;
    string_list ifaces;

	BOOST_FOREACH(i_plugin* plugin, plugins)
	{
            ifaces = plugin->interface_list();
            LOG4CXX_TRACE(iLogger::GetLogger(), "task::store_plugins - plugin: " << plugin->get_description() << " ifaces: " << ifaces.size());

            trsp = find(ifaces.begin(), ifaces.end(), "i_parser");
            if (trsp != ifaces.end()) {
			LOG4CXX_TRACE(iLogger::GetLogger(), "task::store_plugins - found parser: " << plugin->get_description());
			i_parser* plg = (i_parser*)plugin->get_interface("i_parser");
			if (plg)
				add_plg_parser(plg);
            }

        trsp = find(ifaces.begin(), ifaces.end(), "i_transport");
        if (trsp != ifaces.end()) {
			LOG4CXX_TRACE(iLogger::GetLogger(), "task::store_plugins - found transport: " << plugin->get_description());
			i_transport* plg = (i_transport*)plugin->get_interface("i_transport");
			if (plg)
				add_plg_transport(plg);
        }

        trsp = find(ifaces.begin(), ifaces.end(), "i_inventory");
        if (trsp != ifaces.end()) {
			LOG4CXX_TRACE(iLogger::GetLogger(), "task::store_plugins - found inventory: " << plugin->get_description());
			i_inventory* plg = (i_inventory*)plugin->get_interface("i_inventory");
			if (plg)
				add_plg_inventory(plg);
        }

        trsp = find(ifaces.begin(), ifaces.end(), "i_audit");
        if (trsp != ifaces.end()) {
			LOG4CXX_TRACE(iLogger::GetLogger(), "task::store_plugins - found auditor: " << plugin->get_description());
			i_audit* plg = (i_audit*)plugin->get_interface("i_audit");
			if (plg)
				add_plg_auditor(plg);
        }

        trsp = find(ifaces.begin(), ifaces.end(), "i_vulner");
        if (trsp != ifaces.end()) {
			LOG4CXX_TRACE(iLogger::GetLogger(), "task::store_plugins - found vulner: " << plugin->get_description());
			i_vulner* plg = (i_vulner*)plugin->get_interface("i_vulner");
			if (plg)
				add_plg_vulner(plg);
		}
        }

    }

void task::store_plugins(const string& plugins){
    vector<webEngine::i_plugin*> scan_plugins;
    if (!plugins.empty()) {
        vector<string> plgs;
        boost::split(plgs, plugins, boost::is_any_of(";"));
        for (size_t i = 0; i < plgs.size(); ++i) {
            if (plgs[i] != "") {
                webEngine::i_plugin* plg = kernel->load_plugin(plgs[i]);
                if (plg) {
                    scan_plugins.push_back(plg->get_interface("i_plugin"));
                } // if plugin loaded
            } // if name given
        } // foreach plugin
    } else {
        LOG4CXX_ERROR(iLogger::GetLogger(), "No plugins attached to profile " << scan_id << ". Nothing to do, exit.");
        return;
    }
    store_plugins(scan_plugins);
}

void task::Run(const string &task_id)
{
    LOG4CXX_DEBUG(iLogger::GetLogger(), "task::Run: create WeTaskProcessor");
    scan_id = task_id;
    if(task_id.empty()){
        if (kernel != NULL && kernel->storage() != NULL) {
            scan_id = kernel->storage()->generate_id(weObjTypeScanData);
        }
    }
    tsk_status = WI_TSK_RUNNING;
    boost::unique_lock<boost::mutex> lock(tsk_mutex);
    need_lock = false;
    start_tm = btm::second_clock::local_time();
    check_task_processor();

    load_task_params();

    total_reqs = 0;
    total_done = 0;
    //processThread = true;
    for (size_t i = 0; i < transports.size(); i++) {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Run: initialize " << transports[i]->get_description());
        transports[i]->init(this);
    }

    load_task_list();

    // init parsers first
    for (size_t i = 0; i < parsers.size(); i++) {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Run: initialize " << parsers[i]->get_description());
        parsers[i]->init(this);
    }

    // init vulner detectors
    for (size_t i = 0; i < vulners.size(); i++) {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Run: initialize " << vulners[i]->get_description());
        vulners[i]->init(this);
    }

    // init auditors
    for (size_t i = 0; i < auditors.size(); i++) {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Run: initialize " << auditors[i]->get_description());
        auditors[i]->init(this);
    }

    // last step: init inventories and run the process
    for (size_t i = 0; i < inventories.size(); i++) {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Run: start " << inventories[i]->get_description());
        inventories[i]->init(this);
    }
    need_lock = true;
}

TaskStatus task::Pause(bool wait)
{
    //    we_option opt;
    //    opt = Option(weoTaskStatus);
    //    SAFE_GET_OPTION_VAL(opt, idata, WI_TSK_RUN);
    tsk_status = WI_TSK_PAUSING;
    check_task_processor();
    {
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Pause notify all about TaskStatus change");
        boost::unique_lock<boost::mutex> lock(tsk_mutex);
        tsk_event.notify_all();
    }
    if(wait){
        while(tsk_status == WI_TSK_PAUSING){
            boost::this_thread::sleep(boost::posix_time::millisec(500));
        }
    }
    return tsk_status;
}

void task::Resume()
{
    if(tsk_status == WI_TSK_PAUSED || tsk_status == WI_TSK_SUSPENDED) {
        tsk_status = WI_TSK_RUNNING;
        check_task_processor();
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Resume notify all about TaskStatus change");
        boost::unique_lock<boost::mutex> lock(tsk_mutex);
        tsk_event.notify_all();
    }
}

bool task::Suspend()
{
    if(Pause(true) == WI_TSK_PAUSED){
        save_to_db();
        save_task_list_and_params();
        boost::unique_lock<boost::mutex> lock(tsk_mutex);
        tsk_status = WI_TSK_SUSPENDED;
        tsk_event.notify_all();
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Suspend notify all about TaskStatus change");
        return true;        
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::Suspend Error while pausing thread");
    return false;
}

bool task::Stop()
{
    if(Pause(true) == WI_TSK_PAUSED){
        boost::unique_lock<boost::mutex> lock(tsk_mutex);
        tsk_status = WI_TSK_STOPPED;
        tsk_event.notify_all();
        for(vector<i_request_ptr>::iterator it = taskList.begin(); it != taskList.end(); ++it) {
            (*it)->abort_request();
        }
        taskList.clear();
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::Stop notify all about TaskStatus change");
        return true;        
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::Stop Error while stopping thread");
    return false;
}

void task::calc_status()
{
    boost::unique_lock<boost::mutex> lock(tsk_mutex);

    const TaskStatus old = tsk_status;

    size_t count = taskList.size();
    int idata = 100;
    if (total_reqs != 0) {
        idata = (total_reqs - count) * 100 / total_reqs;
    }
    int active_threads = LockedGetValue(&thread_count);
    LOG4CXX_DEBUG(iLogger::GetLogger(), "task::calc_status: rest " << count << " queries  from " <<
                  total_reqs << " (" << (total_reqs - count) << ": " << idata << "%); waiting for: " << taskQueue.size() <<
                  " requests, done: " << total_done << "; "<< active_threads << " active threads");
    if ( (count + total_done + taskQueue.size()) < total_reqs) {
        LOG4CXX_WARN(iLogger::GetLogger(), "task::calc_status: counters mismatch! count + total_done + taskQueue.size() < total_reqs!");
    }
    if (idata == 100) {
        idata = 99;
    }
    tsk_completion = idata;

    switch(tsk_status){
        case WI_TSK_INIT:
            tsk_status = WI_TSK_RUNNING;
        case WI_TSK_RUNNING:
        if (taskList.size() == 0 && taskQueue.size() == 0 && active_threads == 0 && i_operation::get_instance_counter() == 0) {
            LOG4CXX_DEBUG(iLogger::GetLogger(), "task::calc_status: finish!");
                tsk_completion = 100;
                finish_tm = btm::second_clock::local_time();
                tsk_status = WI_TSK_FINISHED;
            }
#if 0 // search for missing requests
        else if(taskList.size() == 0 && taskQueue.size() == 0 && active_threads == 0) {
            transport_url url;
            for(size_t i = 0; i < i_operation::get_instance_counter(); ++i) {
                static_instance_counter<i_operation> *op = i_operation::debug_get_instance(i);
                HttpRequest* r = dynamic_cast<HttpRequest*>(op);
                if(r) {
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "task::calc_status: HttpRequest 0x" << (void*)r << " " << r->RequestUrl().tostring());
                }
                HttpResponse* s = dynamic_cast<HttpResponse*>(op);
                if(s) {
                    LOG4CXX_DEBUG(iLogger::GetLogger(), "task::calc_status: HttpResponse 0x" << (void*)s << " " << s->RealUrl().tostring());
                }
            }
        }
#endif
            break;
        case WI_TSK_PAUSING:
            if (taskQueue.size() == 0 && active_threads == 0) {
                LOG4CXX_DEBUG(iLogger::GetLogger(), "task::calc_status: finish!");
                tsk_status = WI_TSK_PAUSED;
            }
            break;
    }

    if(old != tsk_status) {
        tsk_event.notify_all();
        LOG4CXX_TRACE(iLogger::GetLogger(), "task::calc_status Status changed: " << (int)tsk_status);
    }
    // set task status
    save_to_db();
}

db_cursor task::get_scan()
{
    db_cursor retval;
    if (kernel != NULL && kernel->storage() != NULL) {
        db_filter query;
        db_condition icond;
        icond.field() = weObjTypeScanData "." "task_id";
        icond.operation() = db_condition::equal;
        icond.value() = scan_id;
        query.set(icond);
        retval = kernel->storage()->get(query, weObjTypeScanData);
    }
    return retval;
}

int task::get_scan_size()
{
    int retval = 0;
    if (kernel != NULL && kernel->storage() != NULL) {
        db_filter query;
        db_condition icond;
        icond.field() = weObjTypeScanData "." "task_id";
        icond.operation() = db_condition::equal;
        icond.value() = scan_id;
        query.set(icond);
        retval = kernel->storage()->count(query, weObjTypeScanData);
    }
    return retval;
}

boost::shared_ptr<ScanData> task::get_scan_data( const string& baseUrl )
{
    boost::unique_lock<boost::mutex> lock(scandata_mutex);
    boost::shared_ptr<ScanData> retval;
    ScanData* data;

    data = NULL;
    if (kernel != NULL && kernel->storage() != NULL) {
        db_filter flt;
        db_condition ucond;
        db_condition scond;

        ucond.field() = weObjTypeScanData "." "object_url";
        ucond.operation() = db_condition::equal;
        ucond.value() = baseUrl;
        scond.field() = weObjTypeScanData "." "task_id";
        scond.operation() = db_condition::equal;
        scond.value() = scan_id;
        flt.set(scond)._and_(ucond);
        db_cursor r = kernel->storage()->get(flt, weObjTypeScanData);
        if (r.is_not_end()) {
            data = new ScanData;
            if (! data->from_dataset(*r) ) {
                delete data;
                data = NULL;
            }
        }// if data found
    } // if storage present
    if (data == NULL) {
        // no data found
        data = new ScanData;
        data->object_url = baseUrl;
    } // if no data found
    if (data->scan_id == "") {
        data->scan_id = scan_id;
    }
    retval = boost::shared_ptr<ScanData>(data);
    return retval;
}

void task::set_scan_data( const string& baseUrl, boost::shared_ptr<ScanData> scData )
{
    {
        boost::unique_lock<boost::mutex> lock(scandata_mutex);
        if (kernel != NULL && kernel->storage() != NULL) {
            db_filter flt;
            db_condition ucond;
            db_condition scond;

            ucond.field() = weObjTypeScanData "." "object_url";
            ucond.operation() = db_condition::equal;
            ucond.value() = baseUrl;
            scond.field() = weObjTypeScanData "." "task_id";
            scond.operation() = db_condition::equal;
            scond.value() = scan_id;
            flt.set(scond)._and_(ucond);

            db_cursor r = kernel->storage()->set(flt, weObjTypeScanData);
            scData->to_dataset(*r);
            r.close();
        } // if storage present
        sc_process.push_back(scData);
    }
    if (!dataThread) {
        dataThread = true;
        boost::thread(task_data_process, this);
    }
}

void task::free_scan_data(boost::shared_ptr<ScanData> scData)
{
    boost::unique_lock<boost::mutex> lock(scandata_mutex);
	LOG4CXX_TRACE(iLogger::GetLogger(), "Free memory for parsed data (SC = " << scData.use_count() << "; PD = " << std::hex << (size_t)scData->parsed_data.get() << ")");
    scData->response.reset();
    scData->parsed_data.reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn db_recordset* task::save_to_db( )
///
/// @brief	Converts this object to an db_recordset. This function realizes alternate serialization
/// 		mechanism. It generates db_recordset with all necessary data. This
/// 		representation is used for internal data exchange (for example to store data through
/// 		iweStorage interface).
///
/// @retval	This object as a std::string.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool task::save_to_db( )
{
    bool res = false;

    if (kernel != NULL && kernel->storage() != NULL) {
        db_filter flt;
        db_condition icond;
        icond.field() = weObjTypeTask "." weoID;
        icond.operation() = db_condition::equal;
        icond.value() = scan_id;
        flt.set(icond);

        db_cursor rec = kernel->storage()->set(flt, weObjTypeTask);
        rec[weObjTypeTask "." weoID] = scan_id;
        rec[weObjTypeTask "." weoProfileID] = profile_id;
        rec[weObjTypeTask "." "name"] = task_name;
        rec[weObjTypeTask "." weoTaskCompletion] = tsk_completion;
        rec[weObjTypeTask "." weoTaskStatus] = tsk_status;
        rec[weObjTypeTask "." "start_time"] = boost::lexical_cast<string>(start_tm);
        rec[weObjTypeTask "." "finish_time"] = boost::lexical_cast<string>(finish_tm);
        rec[weObjTypeTask "." "ping_time"] = boost::lexical_cast<string>(ping_tm);
        rec[weObjTypeTask "." "requests"] = (int)total_reqs;
        string urls = "";
        BOOST_FOREACH( we_url_map::value_type i, processed_urls ) {
            urls += i.first;
            urls += "\t";
            urls += boost::lexical_cast<string>(i.second);
            urls += "\x0d";
        }
        rec[weObjTypeTask "." "processed_urls"] = urls;
        if (rec.close()) {
            // only one record inserted/updated
            res = true;
        }
    }
    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn void task::load_from_db( string& id )
///
/// @brief  Initializes this object from the given from db_recordset.
///
/// @param  id	 - task identifier in database.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool task::load_from_db( string& id  )
{
    bool res = false;

    if (kernel != NULL && kernel->storage() != NULL) {
        db_filter flt;
        db_condition icond;

        icond.field() = weObjTypeTask "." weoID;
        icond.operation() = db_condition::equal;
        icond.value() = scan_id;
        flt.set(icond);
        db_cursor rec = kernel->storage()->get(flt, weObjTypeTask);
        if(rec.is_not_end()) {
            res = true;
            string data;
            try {
                scan_id = rec[weObjTypeTask "." weoID].get<string>();
                profile_id = rec[weObjTypeTask "." weoProfileID].get<string>();
                task_name = rec[weObjTypeTask "." "name"].get<string>();
                tsk_completion = rec[weObjTypeTask "." weoTaskCompletion].get<int>();
                tsk_status = (TaskStatus)rec[weObjTypeTask "." weoTaskStatus].get<int>();
                data = rec[weObjTypeTask "." "start_time"].get<string>();
                start_tm = boost::lexical_cast<boost::posix_time::ptime>(data);
                data = rec[weObjTypeTask "." "finish_time"].get<string>();
                finish_tm = boost::lexical_cast<boost::posix_time::ptime>(data);
                data = rec[weObjTypeTask "." "ping_time"].get<string>();
                ping_tm = boost::lexical_cast<boost::posix_time::ptime>(data);
                total_reqs = rec[weObjTypeTask "." "requests"].get<int>();
                data = rec[weObjTypeTask "." "processed_urls"].get<string>();
                vector<string> urls;
                boost::split(urls, data, is_any_of("\x0d"));
                processed_urls.clear();
                for(size_t i = 0; i < urls.size(); i++) {
                    string u = urls[i];
                    string n = "0";
                    size_t pos = u.find('\t');
                    int nl = 0;
                    if(pos != string::npos) {
                        n = u.substr(pos+1);
                        u = u.substr(0, pos);
                        nl = boost::lexical_cast<int>(n);
                    }
                    processed_urls[u] = nl;
                } // foreach url
            } catch (std::exception &e) {
                LOG4CXX_ERROR(iLogger::GetLogger(), "task::load_from_db exception: " << std::string(e.what()));
                // may be out_of_range or bad_cast
                res = false;
            } // try/catch
        }
    } // if storage exist
    return res;
}

void task::WaitForData()
{
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::WaitForData synch object ready");
    boost::unique_lock<boost::mutex> lock(tsk_mutex);
    while( tsk_status == WI_TSK_INIT || tsk_status == WI_TSK_PAUSED) { // (taskList.size() == 0 && taskQueue.size() == 0) ||
        LOG4CXX_DEBUG(iLogger::GetLogger(), "task::WaitForData: go to sleep");
        tsk_event.wait(lock);
        LOG4CXX_DEBUG(iLogger::GetLogger(), "task::WaitForData: wake up");
    }
}

void task::add_vulner( const string& vId, const string& params, const string& parentId, int vLevel/* = -1*/ )
{
    LOG4CXX_INFO(iLogger::GetLogger(), "task::add_vulner add vulner ID=" << vId << "; ParentID=" << parentId);
    LOG4CXX_DEBUG(iLogger::GetLogger(), "task::add_vulner data: " << params);
    string plg_id, v_id, id;
    size_t pos;

    plg_id = "";
    pos = vId.find('/');
    if (pos != string::npos) {
        plg_id = vId.substr(0, pos);
        v_id = vId.substr(pos + 1);
    } else {
        v_id = vId;
    }
    if (kernel != NULL && kernel->storage() != NULL) {
        id = kernel->storage()->generate_id(weObjTypeVulner);

        db_filter flt;
        db_condition icond;
        db_condition tcond;

        icond.field() = weObjTypeVulner "." "id";
        icond.operation() = db_condition::equal;
        icond.value() = id;

        tcond.field() = weObjTypeVulner "." "task_id";
        tcond.operation() = db_condition::equal;
        tcond.value() = scan_id;

        flt.set(icond)._and_(tcond);

        db_cursor rec = kernel->storage()->set(flt, weObjTypeVulner);;

        rec[weObjTypeVulner "." "id"] = id;
        rec[weObjTypeVulner "." "task_id"] = scan_id;
        rec[weObjTypeVulner "." "object_id"] = parentId;
        rec[weObjTypeVulner "." "plugin_id"] = plg_id;
        rec[weObjTypeVulner "." "severity"] = vLevel;
        rec[weObjTypeVulner "." "vulner_id"] = v_id;
        rec[weObjTypeVulner "." "params"] = params;

        rec.close();
    }

}

int task::add_thread()
{
    int nt = LockedIncrement(&thread_count);
    tsk_completion = 99;
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::add_thread " << nt << " threads active");
    return nt;
}

int task::remove_thread()
{
    int nt = LockedDecrement(&thread_count);
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::remove_thread " << nt << " threads active");
    return nt;
}

we_option task::Option( const string& name )
{
    db_filter flt;
    we_option opt;
    char c;
    int i;
    bool b;
    double d;
    string s;

    LOG4CXX_TRACE(iLogger::GetLogger(), "task::Option(" << name << ")");
    opt = i_options_provider::empty_option;
    if (kernel != NULL && kernel->storage() != NULL) {
        vector<string> fields;
        fields.push_back(weObjTypeProfile "." weoTypeID);
        fields.push_back(weObjTypeProfile "." weoValue);

        db_condition p_cond;
        db_condition n_cond;

        p_cond.field() = weObjTypeProfile "." weoProfileID;
        p_cond.operation() = db_condition::equal;
        p_cond.value() = profile_id;

        n_cond.field() = weObjTypeProfile "." weoName;
        n_cond.operation() = db_condition::equal;
        n_cond.value() = name;

        flt.set(p_cond)._and_(n_cond);

        db_cursor rec = kernel->storage()->get(flt, fields);

        if (rec.is_not_end()) {
            try {
                if (!rec[1].empty()) {
                    int tp = boost::lexical_cast<int>(rec[0]);
                    opt.name(name);
                    switch(tp) {
                    case 0: // char
                        c = rec[weObjTypeProfile "." weoValue].get<char>();
                        opt.SetValue(c);
                        break;
                    case 1: // int
                        i = rec[weObjTypeProfile "." weoValue].get<int>();
                        opt.SetValue(i);
                        break;
                    case 2: // bool
                        b = rec[weObjTypeProfile "." weoValue].get<bool>();
                        opt.SetValue(b);
                        break;
                    case 3: // double
                        d = rec[weObjTypeProfile "." weoValue].get<double>();
                        opt.SetValue(d);
                        break;
                    case 4: // string
                        s = rec[weObjTypeProfile "." weoValue].get<string>();
                        opt.SetValue(s);
                        break;
                    default:
                        opt.SetValue(boost::blank());
                    }
                } // if result not <empty>
            } catch(std::exception &e) {
                opt = i_options_provider::empty_option;
                LOG4CXX_ERROR(iLogger::GetLogger(), "engine_dispatcher::Option(" << name << ") can't get option value: " << std::string(e.what()));
            }
        }
    }

    return opt;

}

void task::Option( const string& name, we_variant val )
{
    db_filter flt;

    if (kernel != NULL && kernel->storage() != NULL) {
        vector<string> fields;
        fields.push_back(weObjTypeProfile "." weoProfileID);
        fields.push_back(weObjTypeProfile "." weoName);
        fields.push_back(weObjTypeProfile "." weoTypeID);
        fields.push_back(weObjTypeProfile "." weoValue);

        db_condition p_cond;
        db_condition n_cond;

        p_cond.field() = weObjTypeProfile "." weoProfileID;
        p_cond.operation() = db_condition::equal;
        p_cond.value() = profile_id;

        n_cond.field() = weObjTypeProfile "." weoName;
        n_cond.operation() = db_condition::equal;
        n_cond.value() = name;

        flt.set(p_cond)._and_(n_cond);

        db_cursor rec = kernel->storage()->set(flt, fields);

        rec[0] = profile_id;
        rec[1] = name;
        rec[2] = val.which();
        rec[3] = val;

        rec.close();
    }
}

bool task::IsSet( const string& name )
{
    bool retval = false;
    db_filter flt;

    if (kernel != NULL && kernel->storage() != NULL) {
        vector<string> fields;
        fields.push_back(weObjTypeProfile "." weoTypeID);
        fields.push_back(weObjTypeProfile "." weoValue);

        db_condition p_cond;
        db_condition n_cond;

        p_cond.field() = weObjTypeProfile "." weoProfileID;
        p_cond.operation() = db_condition::equal;
        p_cond.value() = profile_id;

        n_cond.field() = weObjTypeProfile "." weoName;
        n_cond.operation() = db_condition::equal;
        n_cond.value() = name;

        flt.set(p_cond)._and_(n_cond);

        db_cursor rec = kernel->storage()->get(flt, fields);
        if (rec.is_not_end()) {
            try {
                if (!rec[1].empty()) {
                    int tp = boost::lexical_cast<int>(rec[0]);
                    if ( tp == 2) {
                        // we_variant::which(bool)
                        retval = boost::lexical_cast<bool>(rec[1]);
                    } else {
                        retval = true;
                    }
                } // if result not <empty>
            } catch(bad_cast &e) {
                retval = false;
                LOG4CXX_ERROR(iLogger::GetLogger(), "engine_dispatcher::IsSet(" << name << ") can't get option value: " << std::string(e.what()));
            }
        } // if result size > 0
    }
    LOG4CXX_TRACE(iLogger::GetLogger(), "task::IsSet(" << name << ") value=" << retval);
    return retval;
}

void task::Erase( const string& name )
{
    if (kernel != NULL && kernel->storage() != NULL) {
        db_filter filter;
        db_condition p_cond;
        db_condition n_cond;

        p_cond.field() = weObjTypeProfile "." weoProfileID;
        p_cond.operation() = db_condition::equal;
        p_cond.value() = profile_id;

        n_cond.field() = weObjTypeProfile "." weoName;
        n_cond.operation() = db_condition::equal;
        n_cond.value() = name;

        filter.set(p_cond)._and_(n_cond);
        LOG4CXX_TRACE(iLogger::GetLogger(), "engine_dispatcher::Erase - " << filter.tostring());
        kernel->storage()->del(filter);
    }
}

void task::Clear()
{
    if (kernel != NULL && kernel->storage() != NULL) {
        string_list opt_names;
        opt_names = OptionsList();
        for (size_t i = 0; i < opt_names.size(); i++) {
            Erase(opt_names[i]);
        }
    }
}

// void task::CopyOptions( i_options_provider* cpy )
// {
//     if (kernel != NULL) {
//         kernel->CopyOptions(cpy);
//     }
// }
//
string_list task::OptionsList()
{
    string_list retval;
    db_filter flt;
    string name;

    if (kernel != NULL && kernel->storage() != NULL) {
        vector<string> fields;
        fields.push_back(weObjTypeProfile "." weoName);
        db_condition p_cond;

        p_cond.field() = weObjTypeProfile "." weoProfileID;
        p_cond.operation() = db_condition::equal;
        p_cond.value() = profile_id;

        flt.set(p_cond);

        for(db_cursor rec = kernel->storage()->get(flt, fields); rec.is_not_end(); ++rec) {
            name = rec[0].get<string>();
            if (name != "") {
                retval.push_back(name);
            } // if name present
        } // foreach record
    }

    return retval;
}

size_t task::OptionSize()
{
    int retval = 0;
    db_filter flt;

    if (kernel != NULL && kernel->storage() != NULL) {
        vector<string> fields;
        fields.push_back(weObjTypeProfile "." weoName);
        db_condition p_cond;

        p_cond.field() = weObjTypeProfile "." weoProfileID;
        p_cond.operation() = db_condition::equal;
        p_cond.value() = profile_id;

        flt.set(p_cond);

        retval = kernel->storage()->count(flt, fields);;
    }

    return retval;
}

bool task::is_url_processed( string& url )
{
    we_url_map::iterator mit = processed_urls.find(url);
    return (mit != processed_urls.end());
}

void task::register_url( const string& url )
{
    we_url_map::iterator mit = processed_urls.find(url);

    if (mit == processed_urls.end()) {
        processed_urls[url] = 1;
    } else {
        processed_urls[url]++;
    }
}

i_plugin* task::get_active_plugin( const string& iface_name, bool autoload /*= false*/ )
{
    i_plugin* resp = NULL;

    BOOST_FOREACH(i_plugin * plugin, parsers)
    {
        resp = plugin->get_interface(iface_name);
		if (resp)
        return resp;
    }
    BOOST_FOREACH(i_plugin * plugin, transports)
    {
        resp = plugin->get_interface(iface_name);
		if (resp)
        return resp;
    }
    BOOST_FOREACH(i_plugin * plugin, inventories)
    {
        resp = plugin->get_interface(iface_name);
		if (resp)
        return resp;
    }
    BOOST_FOREACH(i_plugin * plugin, auditors)
    {
        resp = plugin->get_interface(iface_name);
		if (resp)
        return resp;
    }
    BOOST_FOREACH(i_plugin * plugin, vulners)
    {
        resp = plugin->get_interface(iface_name);
		if (resp)
        return resp;
    }
    if (autoload && kernel != NULL) {
        // try to load requested plugin
        resp = kernel->load_plugin(iface_name);
        if (resp != NULL) {
            vector<i_plugin*> plg_lst;
            plg_lst.push_back(resp);
            store_plugins(plg_lst);
        }
    }
    return resp;
}

void task::save_task_list_and_params()
{
    if(tsk_status != WI_TSK_PAUSED){
        LOG4CXX_ERROR(iLogger::GetLogger(), "task::save_task_list was called in not allowed state");
    }
    boost::scoped_ptr<boost::unique_lock<boost::mutex> > lock;
    if(need_lock)
        lock.reset(new boost::unique_lock<boost::mutex>(tsk_mutex) );
    vector<string> fields;
    fields.push_back("task_list.scan_id");
    fields.push_back("task_list.request_type");
    fields.push_back("task_list.request");
    
    kernel->storage()->del( db_condition().field("task_list.scan_id").operation(db_condition::equal).value(scan_id) );

    db_cursor cur = kernel->storage()->ins(fields);
    for(vector<i_request_ptr>::const_iterator it = taskList.begin(); it != taskList.end(); ++it, ++cur){
        cur[0] = scan_id;
        cur[1] = string(typeid(*(*it)).name());
        ostringstream str;
        we_oarchive arch(str);
        (*it)->to_archive(arch);
        cur[2] = str.str();
    }
    {
        cur[0] = scan_id;
        cur[1] = task_params_str;
        ostringstream str;
        we_oarchive arch(str);
        SERIALIZE_TASK_PARAMS(arch);
        cur[2] = str.str();
    }
    cur.close();
}

void task::load_task_list()
{
    boost::scoped_ptr<boost::unique_lock<boost::mutex> > lock;
    if(need_lock)
        lock.reset(new boost::unique_lock<boost::mutex>(tsk_mutex) );

    vector<string> fields;
    fields.push_back("task_list.request_type");
    fields.push_back("task_list.request");

    taskList.clear();
    i_request_ptr request;
    db_cursor cur = kernel->storage()->get(
        db_condition().field("task_list.scan_id").operation(db_condition::equal).value(scan_id)
        && db_condition().field("task_list.request_type").operation(db_condition::not_equal).value(task_params_str), 
        fields);
    for(; cur.is_not_end(); ++cur){
        istringstream str(cur[1].get<string>());
        we_iarchive arch(str);
        request = kernel->restore_request(cur[0].get<string>(), arch);
        if(request){
            taskList.push_back(request);
        } else {
            LOG4CXX_ERROR(iLogger::GetLogger(), "task::load_task_list Could not deserialize \"" << cur[0] << "\"");
        }
    }
}

void task::load_task_params()
{
    boost::scoped_ptr<boost::unique_lock<boost::mutex> > lock;
    if(need_lock)
        lock.reset(new boost::unique_lock<boost::mutex>(tsk_mutex) );

    vector<string> fields;
    fields.push_back("task_list.request_type");
    fields.push_back("task_list.request");

    db_cursor cur = kernel->storage()->get(
        db_condition().field("task_list.scan_id").operation(db_condition::equal).value(scan_id)
        && db_condition().field("task_list.request_type").operation(db_condition::equal).value(task_params_str), 
        fields);
    if(cur.is_not_end()) {
        istringstream str(cur[1].get<string>());
        we_iarchive arch(str);
        SERIALIZE_TASK_PARAMS(arch);
    }
}

#ifdef _DEBUG
namespace{
#define TO_STR(unused,data,elem) BOOST_PP_STRINGIZE(elem) ,
    const char * TaskStatus_strs[]={ BOOST_PP_SEQ_FOR_EACH(TO_STR,~,SEQ_TASK_STATUSES) };
#undef TO_STR
}

const char * task::get_status_name() const
{
    return TaskStatus_strs[tsk_status];
}

#endif

} // namespace webEngine
