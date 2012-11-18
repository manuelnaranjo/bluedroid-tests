/*
 * Copyright (C) 2012 Naranjo Manuel Francisco <naranjo.manuel@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "bluedroid-test"
#include "btif_util.h"
#include "hardware/bluetooth.h"
#include "hardware/bt_sock.h"
#include "utils/Log.h"
#include "utils/misc.h"
#include "cutils/properties.h"
#include "android_runtime/AndroidRuntime.h"

#include <string.h>
#include <pthread.h>

#include <sys/stat.h>
#include <fcntl.h>

static const bt_interface_t *sBtInterface = NULL;
static const btsock_interface_t *sBtSocketInterface = NULL;

#define DEBUG printf

#define FALSE 0
#define TRUE 1

static void adapter_state_change_callback(bt_state_t status) {
    DEBUG("%s: Status is %d\n", __FUNCTION__, status);
}

const char * debug_bt_property_t(bt_property_t prop) {
    bt_property_type_t ptype = prop.type;
    DEBUG("%s: \n", dump_property_type(ptype));
}


#define DEBUG_STATUS_PROP()                                             \
    DEBUG("%s: Status is: %d, Properties: %d\n", __FUNCTION__, status, num_properties)

#define DEBUG_STATUS()                          \
    DEBUG("%s: Status is: %d\n", __FUNCTION__)

static void adapter_properties_callback(bt_status_t status, int num_properties,
                                            bt_property_t *properties) {
    DEBUG_STATUS_PROP();

    if (status != BT_STATUS_SUCCESS) {
        return;
    }

    for (int i = 0; i < num_properties; i++) {
        debug_bt_property_t(properties[i]);
    }

}

static void remote_device_properties_callback(bt_status_t status, bt_bdaddr_t *bd_addr,
                                              int num_properties, bt_property_t *properties) {
    DEBUG_STATUS_PROP();
    if (status != BT_STATUS_SUCCESS) {
        return;
    }

    for (int i = 0; i < num_properties; i++) {
        debug_bt_property_t(properties[i]);
    }

    return;
}


static void device_found_callback(int num_properties, bt_property_t *properties) {
    int addr_index = -1;
    DEBUG_STATUS_PROP();

    for (int i = 0; i < num_properties; i++) {
        if (properties[i].type == BT_PROPERTY_BDADDR) {
            addr_index = i;
        }
        debug_bt_property_t(properties[i]);
    }

    if ( addr_index == -1 ){
        DEBUG("no bluetooth address provided\n");
        return;
    }

    remote_device_properties_callback(BT_STATUS_SUCCESS,
                                      (bt_bdaddr_t *)properties[addr_index].val,
                                      num_properties, properties);

}

static void bond_state_changed_callback(bt_status_t status, bt_bdaddr_t *bd_addr,
                                        bt_bond_state_t state) {
    DEBUG_STATUS();
    if (!bd_addr){
        DEBUG("Address is null in %s", __FUNCTION__);
        return;
    }
}

static void acl_state_changed_callback(bt_status_t status, bt_bdaddr_t *bd_addr,
                                       bt_acl_state_t state)
{
    DEBUG_STATUS();
    if (!bd_addr) {
        DEBUG("Address is null in %s", __FUNCTION__);
        return;
    }
}

static void discovery_state_changed_callback(bt_discovery_state_t state) {
    DEBUG("%s: DiscoveryState:%d ", __FUNCTION__, state);
}

static void pin_request_callback(bt_bdaddr_t *bd_addr, bt_bdname_t *bdname, uint32_t cod) {
    DEBUG("%s\n", __FUNCTION__);
    if (!bd_addr) {
        DEBUG("Address is null in %s", __FUNCTION__);
        return;
    }
    return;
}

static void ssp_request_callback(bt_bdaddr_t *bd_addr, bt_bdname_t *bdname, uint32_t cod,
                                 bt_ssp_variant_t pairing_variant, uint32_t pass_key) {
    DEBUG("%s\n", __FUNCTION__);
    return;
}

static void callback_thread_event(bt_cb_thread_evt event) {
    if (event  == ASSOCIATE_JVM) {
        DEBUG("%s: ASSOCIATE\n", __FUNCTION__);
    } else if (event == DISASSOCIATE_JVM) {
        DEBUG("%s: DISASSOCIATE\n", __FUNCTION__);
    }
}


bt_callbacks_t sBtCallbacks = {
    sizeof(sBtCallbacks),
    adapter_state_change_callback,
    adapter_properties_callback,
    remote_device_properties_callback,
    device_found_callback,
    discovery_state_changed_callback,
    pin_request_callback,
    ssp_request_callback,
    bond_state_changed_callback,
    acl_state_changed_callback,
    callback_thread_event,
};

static bool init() {
    if (sBtInterface != NULL) {
        DEBUG("double init call\n");
        return FALSE;
    }
    DEBUG("init interface\n");

    int err;
    hw_module_t *module;

    err = hw_get_module( BT_STACK_MODULE_ID, (hw_module_t const**)&module);

    if (err != 0) {
        DEBUG("hw_get_module failed with %i", err);
        return FALSE;
    }

    hw_device_t* abstraction;
    err = module->methods->open(module, BT_STACK_MODULE_ID, &abstraction);

    if (err != 0) {
        DEBUG("hw_module_t->open failed with %i", err);
        return FALSE;
    }

    bluetooth_module_t* btStack = (bluetooth_module_t *)abstraction;
    sBtInterface = btStack->get_bluetooth_interface();
    DEBUG("library opened");

    //int ret =
}


void main() {
    init();
}
