bool clickSet(){
//  Serial.print("==========MODE===========");
  mode = !mode;
  sendHere('m',(String)mode);
  return mode;
}

void longStartSet(){
//  Serial.print("=========START==========");
  start = !start;
  sendHere('s',(String)start);
}
