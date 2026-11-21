#include <Preferences.h>

#define StorageKey  "3DPSL"

Preferences prefs;

void StorageInit(void) {
  prefs.begin(StorageKey, true);  // open the preferences in read-only mode
  if(!prefs.isKey("WiFi/SSID")) {
    Serial.println("WARN: Configuration not found. First boot?");
  }
  prefs.end();
}

void StorageClearAll(void) {
  prefs.begin(StorageKey, false); // open the preferences in read/write mode
  prefs.clear();
  prefs.end();
}

bool StorageDoesKeyExist(const char *key) {
  prefs.begin(StorageKey, true);  // open the preferences in read-only mode
  bool rv = prefs.isKey(key);
  prefs.end();

  return rv;
}

bool StorageDoesKeyExist(String key) {
  return StorageDoesKeyExist(key.c_str());
}

void StorageDeleteKey(const char *key) {
  prefs.begin(StorageKey, false);  // open the preferences in read/write mode
  prefs.remove(key);
  prefs.end();
}

void StorageDeleteKey(String key) {
  StorageDeleteKey(key.c_str());
}

void StoragePutString(const char *key, String value) {
  prefs.begin(StorageKey, false);  // open the preferences in read/write mode
  prefs.putString(key, value);
  prefs.end();
}

void StoragePutString(String key) {
  StoragePutString(key.c_str());
}

String StorageGetString(const char *key) {
  prefs.begin(StorageKey, true);  // open the preferences in read-only mode
  String rv = prefs.getString(key);
  prefs.end();

  return rv;
}

String StorageGetString(String key) {
  return StorageGetString(key.c_str());
}

void StoragePutUInt8(const char *key, uint8_t value) {
  prefs.begin(StorageKey, false);  // open the preferences in read/write mode
  prefs.putUChar(key, value);
  prefs.end();
}

void StoragePutUInt8(String key, uint8_t value) {
  StoragePutUInt8(key.c_str(), value);
}

uint8_t StorageGetUInt8(const char *key) {
  prefs.begin(StorageKey, true);  // open the preferences in read-only mode
  uint8_t rv = prefs.getUChar(key);
  prefs.end();

  return rv;
}

uint8_t StorageGetUInt8(String key) {
  return StorageGetUInt8(key.c_str());
}
