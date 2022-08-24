void run_assist() {
  for (int i = 0; i <= 4; i++) { //Iterasi siklus motor
    limState = 0; //reset limit
    //    Serial.println(i);
    //    i = 0;

    perintah(duration[i], command[i]);  //Panggil fungsi durasi dan siklus
  }

  //==================Indikator==================
  for (int i = 0; i <= 3; i++) {
    Serial.print(indi[i]);
    if (indikator[i]) {
      Serial.print("OK!!!");
    } else {
      Serial.print("FAIL!!!");
    }
  }
  //  delay(5000);
  //=============================================
}
