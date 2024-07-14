void logSetup (){
      Serial.begin(115200);
}

void log(String message) {
      Serial.println(message);
}