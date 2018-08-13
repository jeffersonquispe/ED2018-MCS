#include "paho_client.h"

// g++ data_publish.cpp -lpaho-mqttpp3

int main(int argc, char* argv[]) {
  string payload;
  auto tm = steady_clock::now();
  do{
  this_thread::sleep_until(tm);
  srand((unsigned)time(0));
  payload = "{\"data\":[{\"nivel\":1, \"tag\":\"R0\", \"max\":["+to_string((rand()%800)+1)+","+to_string((rand()%600)+1)+"], \"min\":["+to_string((rand()%800)+1)+","+to_string((rand()%600)+1)+"]}, {\"nivel\":2, \"tag\":\"R1\", \"max\":["+to_string((rand()%800)+1)+","+to_string((rand()%600)+1)+"], \"min\":["+to_string((rand()%800)+1)+","+to_string((rand()%600)+1)+"]}, {\"nivel\":2, \"tag\":\"R2\", \"max\":["+to_string((rand()%800)+1)+","+to_string((rand()%600)+1)+"], \"min\":["+to_string((rand()%800)+1)+","+to_string((rand()%600)+1)+"]} ]}";
  tm += PERIOD;
  }while( mqttPublish("data/rand", payload) == 0);
}
