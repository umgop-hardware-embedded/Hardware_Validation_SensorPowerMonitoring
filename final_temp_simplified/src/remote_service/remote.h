#ifndef REMOTE_H_
#define REMOTE_H_

#include <zephyr/types.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#define BT_UUID_REMOTE_SERV_VAL \
    BT_UUID_128_ENCODE(0xe9ea0001, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

#define BT_UUID_REMOTE_TEMP_CHRC_VAL \
    BT_UUID_128_ENCODE(0xe9ea0004, 0xe19b, 0x482d, 0x9293, 0xc7907585fc48)

#define BT_UUID_REMOTE_SERVICE    BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)
#define BT_UUID_REMOTE_TEMP_CHRC  BT_UUID_DECLARE_128(BT_UUID_REMOTE_TEMP_CHRC_VAL)

struct bt_remote_service_cb {
    void (*data_received)(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
};


int      bluetooth_init(struct bt_conn_cb *bt_cb, struct bt_remote_service_cb *remote_cb);
uint32_t call_read_temp(void);
int      send_temp_notification(struct bt_conn *conn, int32_t temp_raw);

#endif 
