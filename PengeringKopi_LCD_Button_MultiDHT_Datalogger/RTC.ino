String waktu = "0";
String jmd = "0";

void time(){
  DateTime now = rtc.now();

  String tahun = (String)now.year();
  String bulan = (String)now.month();
  String hari = (String)now.day();
  String jam = (String)now.hour();
  String menit = (String)now.minute();
  String detik = (String)now.second();

  jmd = jam + menit + detik;
  waktu = tahun + bulan + hari + jam + menit + detik;
}
