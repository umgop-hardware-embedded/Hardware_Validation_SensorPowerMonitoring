#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>

#include "remote.h"

static void remote_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    ARG_UNUSED(value);
}

BT_GATT_SERVICE_DEFINE(remote_srv,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_REMOTE_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_REMOTE_TEMP_CHRC,
                           BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE,
                           NULL, NULL, NULL),
    BT_GATT_CCC(remote_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

#define TEMP_BASE        0x4000C000u
#define TEMP_TASKS_START (TEMP_BASE + 0x000u)
#define TEMP_TASKS_STOP  (TEMP_BASE + 0x004u)
#define TEMP_EVENTS_RDY  (TEMP_BASE + 0x100u)
#define TEMP_RESULT      (TEMP_BASE + 0x508u)

static uint32_t die_temp;

static void read_temp(void)
{
    *(volatile uint8_t *)TEMP_TASKS_START = 1u;

    while (*(volatile uint8_t *)TEMP_EVENTS_RDY == 0u) {
        k_usleep(100);
    }

    die_temp = *(volatile uint32_t *)TEMP_RESULT;

    *(volatile uint8_t *)TEMP_EVENTS_RDY = 0u;
    *(volatile uint8_t *)TEMP_TASKS_STOP = 1u;
}

uint32_t call_read_temp(void)
{
    read_temp();
    return die_temp;
}

int send_temp_notification(struct bt_conn *conn, int32_t temp_raw)
{
    const struct bt_gatt_attr *attr = &remote_srv.attrs[2];

    struct bt_gatt_notify_params params = {
        .attr = attr,
        .data = &temp_raw,
        .len  = sizeof(temp_raw),
        .func = NULL,
    };

    return bt_gatt_notify_cb(conn, &params);
}

int bluetooth_init(struct bt_conn_cb *bt_cb, struct bt_remote_service_cb *remote_cb)
{
    ARG_UNUSED(remote_cb);

    bt_conn_cb_register(bt_cb);

    int err = bt_enable(NULL);
    if (err)
        return err;

    return 0;
}
