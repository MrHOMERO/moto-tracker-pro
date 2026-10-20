#include <WiFi.h>
#include <esp_sleep.h>

// Configuración de red (Se conecta a la red móvil compartida o Wi-Fi del vehículo)
const char* ssid = "TU_SSID_RED";
const char* password = "TU_PASSWORD";

// Configuración de pines y variables de estado
const int BATTERY_PIN = 34; // Pin analógico para medir el voltaje de la batería LiPo
float batteryThreshold = 3.5; // Umbral de voltaje bajo

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  
  // Medir estado de batería al despertar
  checkBatteryLevel();

  // Configurar temporizador de Sueño Profundo (Deep Sleep) para ahorrar batería
  // Se despierta cada 30 minutos para hacer un "check-in" rápido de 5 segundos con el servidor
  esp_sleep_enable_timer_wakeup(30 * 60 * 1000000ULL); // 30 minutos en microsegundos
  
  // Si no hay comando de robo activo, vuelve a dormir automáticamente
  // (En modo pánico activado por la app, este sleep se deshabilita para transmisión continua)
  Serial.println("Entrando en modo de bajo consumo (Deep Sleep)...");
  // esp_deep_sleep_start(); // Descomentar al implementar el despliegue final
}

void loop() {
  // Bucle principal cuando está activo en modo de emergencia (Anti-robo)
  // Aquí se activa el GPS continuo y la captura de la cámara si se solicita.
}

void checkBatteryLevel() {
  int sensorValue = analogRead(BATTERY_PIN);
  float voltage = sensorValue * (3.3 / 4095.0) * 2; // Factor divisor de tensión
  
  Serial.print("Voltaje actual de batería: ");
  Serial.print(voltage);
  Serial.println("V");

  if (voltage < batteryThreshold) {
    Serial.println("¡ALERTA: Batería baja en el dispositivo portátil!");
    // Aquí se envía una notificación push de baja batería al servidor/App
  }
}
