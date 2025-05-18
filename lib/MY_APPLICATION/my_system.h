#ifndef MY_SYSTEM
#define MY_SYSTEM

#include <Arduino.h>

// ĐỊNH NGHĨA CÁC BIẾN THỜI GIAN NON-BLOCKING
#define ONE_SECOND 500     // 1000ms ~ 1s
long long last_time = 0;    // ...


// ĐỊNH NGHĨA CÁC TRẠNG THÁI CỦA HỆ THỐNG
typedef enum {
    IDLE    = 0,        // bình thường, luôn chờ đọc vân tay
    REGISTER  = 1,      // thêm vân tay (thêm người vào nhà)
    VERIFY    = 2,      // đang xử lý vân tay
    OPENED    = 3,      // dùng khi solenoid mở cửa thành công
} SystemState;
SystemState system_state = IDLE;

String system_name[] = {
    "NORMAL",
    "REGISTER",
    "VERIFY",
    "OPENED"
};

#endif