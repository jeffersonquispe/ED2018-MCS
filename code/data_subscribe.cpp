#include "paho_client.h"

// g++ data_subscribe.cpp -lpaho-mqttpp3 -o subs

int main(int argc, char* argv[]){
  return mqttSubscribe();
}
