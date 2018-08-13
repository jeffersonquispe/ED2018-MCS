#include <fstream>
#include <iostream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
namespace pt = boost::property_tree;

struct data_node{
  double limits[4];
  bool leaf;
  int nivel_data; // nivel
  string tag; // R1
};

data_node convertJSONtoObject(string input){
  stringstream fromJSON, toJSON;
  fromJSON << input;
  pt::ptree iroot, oroot;
  pt::read_json(fromJSON, iroot);

  data_node a;
  return a;
}

string convertRegionsToJSON(vector<data_node> data_tree){
  int size = data_tree.size();
  pt::ptree oroot, data, element[size];
  
  for(int i=0; i<size;i++){
    element[i].put<int>("nivel", data_tree[i].nivel_data);
    element[i].put("tag", data_tree[i].tag);
    element[i].put<bool>("leaf", data_tree[i].leaf);
    pt::ptree pointsMax, pointsMin;
    for(int j=0;j<2;j++){
      pt::ptree points;
      pointsMax.put<int>("", data_tree[i].limits[j]);
      pointsMin.put<int>("", data_tree[i].limits[j+2]);
      points.push_back(std::make_pair("", pointsMax));
      points.push_back(std::make_pair("", pointsMin));
      if(j==0){
        element[i].add_child("min",  points);
      } else if (j==1){
        element[i].add_child("max",  points);
      }
    }
    data.push_back(std::make_pair("", element[i]));
  }
  oroot.add_child("data", data);
  
  stringstream toJSON;
  pt::write_json(toJSON, oroot);
  string output = toJSON.str();
  return output;
}
