// main.c — BLE temp peripheral (no UART/RTT/logging)

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>

#include "remote.h"

static struct bt_conn *current_conn = NULL;
static uint32_t sample_count = 0;

static void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (!err) {
        current_conn = bt_conn_ref(conn);
        const struct bt_le_conn_param param = {
            .interval_min = 80,   
            .interval_max = 400, 
            .latency      = 4,
            .timeout      = 600 
        };
        bt_conn_le_param_update(conn, &param);
    }
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    ARG_UNUSED(reason);
    if (current_conn) {
        bt_conn_unref(current_conn);
        current_conn = NULL;
    }
}


static void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
    ARG_UNUSED(conn);
    ARG_UNUSED(data);
    ARG_UNUSED(len);
}


static struct bt_conn_cb bluetooth_callbacks = {
    .connected    = on_connected,
    .disconnected = on_disconnected,
};

static struct bt_remote_service_cb remote_callbacks = {
    .data_received = on_data_received,
};

void main(void)
{
    int err = bluetooth_init(&bluetooth_callbacks, &remote_callbacks);
    ARG_UNUSED(err);

    while (1) {
        k_sleep(K_SECONDS(1));
        sample_count++;
        while (!current_conn) {
            k_sleep(K_SECONDS(1));
        }

        if (current_conn) {
            uint32_t temp_val = call_read_temp();   
            if ((sample_count % 1) == 0) {
                send_temp_notification(current_conn, temp_val);
            }
        }
    }
}
