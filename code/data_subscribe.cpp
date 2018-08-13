#include "paho_client.h"

// g++ data_subscribe.cpp -lpaho-mqttpp3 -o subs

int mqttSubscribe(){
  mqtt::connect_options connOpts;
  connOpts.set_keep_alive_interval(20);
  connOpts.set_clean_session(true);

  mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

  callback cb(client, connOpts);
  client.set_callback(cb);

  try {
    cout << "Connecting to the MQTT server..." << flush;
    client.connect(connOpts, nullptr, cb);
  }
  catch (const mqtt::exception&) {
    cerr << "\nERROR: Unable to connect to MQTT server: '" << SERVER_ADDRESS << "'" << endl;
    return 1;
  }

  while (tolower(cin.get()) != 'q'){}

  try {
    cout << "\nDisconnecting from the MQTT server..." << flush;
    client.disconnect()->wait();
    cout << "OK" << endl;
  }
  catch (const mqtt::exception& exc) {
    cerr << exc.what() << endl;
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]){
  return mqttSubscribe();
}
