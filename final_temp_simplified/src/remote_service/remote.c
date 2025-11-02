#include "remote.h"
#include <zephyr/kernel.h>
#include <zephyr/types.h>

#define LOG_MODULE_NAME remote
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

static K_SEM_DEFINE(bt_init_ok, 1, 1);
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME)-1)

static struct bt_remote_service_cb remote_callbacks;
struct TEMP_STRUCT {
    char TASKS_START;       // 0x000
    char buff0[0x3];
    char TASKS_STOP;        // 0x004
    char buff1[0xfc];
    char EVENTS_DATARDY;    // 0x100
    char buff2[0x407];
    uint32_t TEMP;           // 0x508
};
static struct TEMP_STRUCT* TEMP_PTR = 0x4000c000;
static uint32_t die_temp;

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)
};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERV_VAL),
};


BT_GATT_SERVICE_DEFINE(remote_srv,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_TEMP_CHRC,
        BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_NONE,
        NULL, NULL, NULL),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

void on_sent(struct bt_conn *conn, void *user_data)
{
    ARG_UNUSED(user_data);
    LOG_INF("Notification sent on connection %p", (void *)conn);
}

void bt_ready(int err)
{
    if (err) {
        LOG_ERR("bt_enable returned %d", err);
    }
}

int send_temp_notification(struct bt_conn *conn, int32_t temp_raw)
{
    const struct bt_gatt_attr *attr = &remote_srv.attrs[2]; 
    struct bt_gatt_notify_params params = {
        .attr = attr,
        .data = &temp_raw,
        .len  = sizeof(temp_raw),
        .func = on_sent,
    };

    return bt_gatt_notify_cb(conn, &params);
}



void read_temp(void) {
    char data_flag = 0;

    // Start temperature measurement.
    //TEMP_PTR->TASKS_START = 1;
    *(char*)0x4000c000 = 1;

    // Wait until we get a temperature value back.
    printk("Waiting for temperature response..\n\r");

    //k_sleep(K_MSEC(1));
    // while (*(volatile char*)(0x4000c100) == 0) {
    //     k_msleep(1);  
    // }

    data_flag = *(char*)(0x4000c100);
    while (data_flag == 0) {
        data_flag = *(char*)(0x4000c100);
    } // TEMP_PTR->EVENTS_DATARDY == 0) {} 
    //printk("Got temperature!\n\r");

    // Get the temperature value.
    //die_temp = TEMP_PTR->TEMP;
    die_temp = *(uint32_t*)(0x4000c508);

    // Reset the Data Ready flag.
    //TEMP_PTR->EVENTS_DATARDY = 0;
    *(char*)(0x4000c100) = 0;

    // Stop the temperature measurement.
    *(char*)(0x4000c004) = 1;
}





uint32_t call_read_temp(void) {
    read_temp();
    return die_temp;
}


int bluetooth_init(struct bt_conn_cb *bt_cb, struct bt_remote_service_cb *remote_cb)
{
    int err;
    LOG_INF("Initializing Bluetooth");
    
    if (bt_cb == NULL || remote_cb == NULL) {
        return -NRFX_ERROR_NULL;
    }

    bt_conn_cb_register(bt_cb);
    //remote_callbacks.notif_changed = remote_cb->notif_changed;
    remote_callbacks.data_received = remote_cb->data_received;
    err = bt_enable(bt_ready);

    if(err) {
        LOG_ERR("bt_enable returned %d", err);
        return err;
    }


    k_sem_take(&bt_init_ok, K_FOREVER);

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Couldn't start advertising (err = %d)", err);
        return err;
    }

    return err;
}
