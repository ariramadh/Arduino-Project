void sendHere(char command, String value){  
  String sendIt = '/' + (String)command + (String)value + '#';
  Serial.println(sendIt);
}
