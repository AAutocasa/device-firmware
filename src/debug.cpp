#include <Arduino.h>
#include <Ticker.h>

#include "debug.h"

Ticker debugTicker;

void printDebugInfo() {
  Serial.print("[debug] [printDebugInfo] SDK version: ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("[debug] [printDebugInfo] Flash Chip real size: ");
  Serial.println(ESP.getFlashChipRealSize());
  auto heap = ESP.getFreeHeap();
  Serial.print("[debug] [printDebugInfo] Free heap: ");
  Serial.println(heap);
}

void setupDebug(float tickRate) {
  debugTicker.attach(tickRate, printDebugInfo);
}