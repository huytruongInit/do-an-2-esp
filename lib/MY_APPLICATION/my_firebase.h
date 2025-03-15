#ifndef MY_FIREBASE
#define MY_FIREBASE

#include <secret.h>

// Thư viện kết nối firebase
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h"  //Provide the RTDB payload printing info and other helper functions.

FirebaseData fbdo;  // Define Firebase Data object
FirebaseAuth auth;
FirebaseConfig config;

void firebase_init(void) {
    config.api_key = API_KEY; /* Assign the api key (required) */
    config.database_url = DATABASE_URL; /* Assign the RTDB URL (required) */
}

#endif