bool clickSet(){
  mode = !mode;
  sendHere('m',(String)mode);
  return mode;
}

bool longStartSet(){
  start = !start;
  sendHere('s',(String)start);
  return start;
}
