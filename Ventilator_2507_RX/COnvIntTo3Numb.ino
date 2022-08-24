String convInt (int i) {
  String result;
  if (i >= 10 ) {
    result = String(0) + i;
  }
  if (i <= 9) {
    result = String(0) + String(0) + i;
  }
  if (i >= 100 ) {
    result = String(0) +  i;
  }  
//    if (i >= 1000 ) {
//    result = i;
//  }  
  return result;
}
