#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>

#include "remote.h"

static struct bt_conn *current_conn = NULL;
static bool notify_enabled = false;

static void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (!err) {
        current_conn = bt_conn_ref(conn);
        notify_enabled = true;
    }
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    if (current_conn) {
        bt_conn_unref(current_conn);
        current_conn = NULL;
    }
    notify_enabled = false;
    bt_le_adv_stop();
}

static void on_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    notify_enabled = (value == BT_GATT_CCC_NOTIFY);
}

static struct bt_conn_cb bt_callbacks = {
    .connected = on_connected,
    .disconnected = on_disconnected,
};

static struct bt_remote_service_cb remote_callbacks = {
    .data_received = NULL,
};

static void start_advertising(void)
{
    const struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
        BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1)
    };

    const struct bt_data sd[] = {
        BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_REMOTE_SERV_VAL),
    };

    bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
}

void main(void)
{
    bluetooth_init(&bt_callbacks, &remote_callbacks);
    start_advertising();

    while (1) {
        while (!current_conn || !notify_enabled) {
            k_sleep(K_MSEC(200));
        }

        k_sleep(K_SECONDS(1)); 

        uint32_t temp_val = call_read_temp();
        send_temp_notification(current_conn, temp_val);
        
        bt_conn_disconnect(current_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);

        k_sleep(K_SECONDS(10));

        start_advertising();
    }
}
