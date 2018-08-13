#include "paho_client.h"

// g++ data_publish.cpp -lpaho-mqttpp3

int main(int argc, char* argv[]) {
  string payload;

  auto tm = steady_clock::now();
  do{
  this_thread::sleep_until(tm);
  srand((unsigned)time(0));
  vector<data_node> data_tree;
  
  data_tree.push_back({{(rand()%800)+1.0,(rand()%800)+1.0,(rand()%600)+1.0,(rand()%600)+1.0}, false, 1, "R0"});
  data_tree.push_back({{(rand()%800)+1.0,(rand()%800)+1.0,(rand()%600)+1.0,(rand()%600)+1.0}, false, 2, "R1"});
  data_tree.push_back({{(rand()%800)+1.0,(rand()%800)+1.0,(rand()%600)+1.0,(rand()%600)+1.0}, false, 2, "R2"});
  
  payload = convertRegionsToJSON(data_tree);
  tm += PERIOD;
  
  ObjectRTree a = convertJSONtoObject("{\"order\":1, \"min\":[6, 3], \"max\":[5, 9]}");
  
  cout << a.order << " " << a.rect.min[0] << " " << a.rect.min[1] << " " << a.rect.max[0] << " " << a.rect.max[1] << endl;
  
  }while( mqttPublish("web/rand", payload) == 0);
}
