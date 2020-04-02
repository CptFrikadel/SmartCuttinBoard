import nl.tue.id.oocsi.*;

// ******************************************************
// This example requires a running OOCSI server!
//
// How to do that? Check: Examples > Tools > LocalServer
//
// More information how to run an OOCSI server
// can be found here: https://iddi.github.io/oocsi/)
// ******************************************************

OOCSI oocsi;

int wght = 300;

void setup() {
  
  //control panel for what to send, features can be added by replacing ?? with function
  
  size(600, 200);
  background(120);
  textSize(20);
  fill (0);
  text("garlic", 10, 30); 

  fill(200, 0, 0);
  rect(100, 0, 100, 200);
  fill (0);
  text("zuchini", 110, 30); 

  fill(0, 200, 0);
  rect(200, 0, 100, 200);
    fill (0);
  text("weigh", 210, 30);

  fill(0, 0, 200);
  rect(300, 0, 100, 200);
    fill (0);
  text("??", 310, 30);

  fill(200);
  rect(400, 0, 100, 200);
    fill (0);
  text("??", 410, 30);

  fill(150, 30, 200);
  rect(500, 0, 100, 200);
    fill (0);
  text("??", 510, 30);

  oocsi = new OOCSI(this, "group3Emulator", "oocsi.id.tue.nl");
}

void draw() {

  // first few functions, can be added by adding else if statements
  
  if (mouseX > 100) {
    oocsi.channel("cuttingSpeedChannel").data("frequency", 100).send();
  } else if (mouseX > 200 && mouseX < 300) {
    oocsi.channel("cuttingSpeedChannel").data("frequency", 500).send();
  } else if (mouseX > 300 && mouseX < 400) {
    for (int i = 0; i < wght; i = i+1) {
      oocsi.channel("boardWeightChannel").data("weight", i).send();
      delay (100);
    }
    oocsi.channel("boardWeightChannel").data("weight", wght).send();
  }
  println (mouseX);
}
