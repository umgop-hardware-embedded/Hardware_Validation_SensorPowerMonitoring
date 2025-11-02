#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include "remote.h"

#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

static struct bt_conn *current_conn;

void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len);

struct bt_conn_cb bluetooth_callbacks = {
    .connected    = on_connected,
    .disconnected = on_disconnected,
};

struct bt_remote_service_cb remote_callbacks = {
    .data_received = on_data_received,
};

void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("connection err: %d", err);
    }
    LOG_INF("Connected.");
    current_conn = bt_conn_ref(conn);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected (reason: %d)", reason);
    if (current_conn) {
        bt_conn_unref(current_conn);
        current_conn = NULL;
    }
}

void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
    uint8_t temp_str[len + 1];
    memcpy(temp_str, data, len);
    temp_str[len] = 0x00;

    LOG_INF("Received data on conn %p. Len: %d", (void *)conn, len);
    LOG_INF("Data: %s", temp_str);
}


void disabled_uart(void) {
//  uint32_t err_code = app_uart_close();
//  APP_ERROR_CHECK(err_code);
  NRF_UARTE0->TASKS_STOPTX = 1;
  NRF_UARTE0->TASKS_STOPRX = 1;
  NRF_UARTE0->ENABLE = 0;
  /* Workaround by disabling the UART peripherals due to Nordic SDK15.0 issue */
  *(volatile uint32_t *)0x40002FFC = 0; /* Power down UARTE0 */
  *(volatile uint32_t *)0x40002FFC;     
  *(volatile uint32_t *)0x40002FFC = 1; /* Power on UARTE0 so it is ready for next time */
}


void main(void)
{
    int err;
    int sample_count = 0;

    err = bluetooth_init(&bluetooth_callbacks, &remote_callbacks);
    if (err) {
        LOG_ERR("bluetooth_init returned %d", err);
    }
   //disabled_uart(); // - somehow increases the power consump

    while (1) {
        k_sleep(K_MSEC(5000));  
        sample_count++;

        if (current_conn) {
            uint32_t temp_val = call_read_temp();
            send_temp_notification(current_conn, temp_val);
        }
    }
}
