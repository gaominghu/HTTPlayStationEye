#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  camWidth    = 1080;
  camHeight     = 720;

  vector<ofVideoDevice> devices = vidGrabber.listDevices();

  for(int i = 0; i < devices.size(); i++){
    cout << devices[i].id << ": " << devices[i].deviceName;
    if(devices[i].bAvailable){
      cout << endl;
    }else{
     cout << " - unavailable " << endl;
    }
  }

  vidGrabber.setDeviceID(0);
  vidGrabber.setDesiredFrameRate(60);
  vidGrabber.initGrabber(camWidth, camHeight);

  // videoInverted = new unsigned char[camWidth * camHeight * 3];
  // videoTexture.allocate(camWidth, camHeight, GL_RGB);
  ofSetVerticalSync(true);

  colorImg.allocate(camWidth, camHeight);
  grayImage.allocate(camWidth, camHeight);
  grayBg.allocate(camWidth, camHeight);
  grayDiff.allocate(camWidth, camHeight);

  bLearnBakground = true;
  threshold = 30;

  ofx::HTTP::BasicIPVideoServerSettings settings;
  settings.setPort(1337);
  // The default maximum number of client connections is 5.
  // settings.setMaxClientConnections(10);

  server = ofx::HTTP::BasicIPVideoServer::makeShared(settings);
  server->start();

  #if !defined(TARGET_LINUX_ARM)
    ofLaunchBrowser(server->getURL());
  #endif
}

//--------------------------------------------------------------
void ofApp::update(){
  ofBackground(0, 0, 0);

  vidGrabber.update();

  if(vidGrabber.isFrameNew()){
    // int totalPixels = camWidth * camHeight * 3;
    // unsigned char * pixels = vidGrabber.getPixels();
    // for (int i = 0; i < totalPixels; i++){
    //   videoInverted[i] = 255 - pixels[i];
    // }
    // videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);

    colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
    grayImage = colorImg;

    if (bLearnBakground == true){
      grayBg = grayImage;
      bLearnBakground = false;
    }

    grayDiff.absDiff(grayBg, grayImage);
    grayDiff.threshold(threshold);

    server->send(grayDiff.getPixelsRef());
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetHexColor(0xffffff);
  // videoTexture.draw(0, 0, camWidth, camHeight);
  grayDiff.draw(0, 0, camWidth, camHeight);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  switch (key){
    case 'b':
      bLearnBakground = true;
      break;
    case 'p':
      threshold ++;
      if (threshold > 255) threshold = 255;
      break;
    case 'm':
      threshold --;
      if (threshold < 0) threshold = 0;
      break;
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
