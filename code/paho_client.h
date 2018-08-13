#include "mqtt/async_client.h"
#include "boost_ptree.h"

//const std::string SERVER_ADDRESS("tcp://localhost:1883");
const string SERVER_ADDRESS("tcp://r-tree.nezads.com:1883");
const string CLIENT_ID("console_client");
const string TOPIC("web/#");

const int QOS = 1;
const int N_RETRY_ATTEMPTS = 5;
const int MAX_BUFFERED_MSGS = 120;
const auto PERIOD = seconds(5);
const string PERSIST_DIR { "data-persist" };


int mqttPublish(string TOPIC, string payload) {
	string address = SERVER_ADDRESS;

  // mqtt::async_client cli(address, "", MAX_BUFFERED_MSGS, PERSIST_DIR);
	mqtt::async_client cli(address, "");

	mqtt::connect_options connOpts;
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);

	mqtt::topic top(cli, TOPIC, QOS, true);

	try {
		cout << "Connecting to server '" << address << "'..." << flush;
		cli.connect(connOpts)->wait();
		cout << "OK\n" << endl;
		
		char* cstr = new char [payload.size() + 1];
		std::strcpy(cstr, payload.c_str());
		cout << payload << endl;
		top.publish(std::move(cstr));

		cout << "\nDisconnecting..." << flush;
		cli.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

class action_listener : public virtual mqtt::iaction_listener{
  std::string name_;

  void on_failure(const mqtt::token& tok) override {
    std::cout << name_ << " failure";
    if (tok.get_message_id() != 0)
      std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
    std::cout << std::endl;
  }

  void on_success(const mqtt::token& tok) override {
    std::cout << name_ << " success";
    if (tok.get_message_id() != 0)
      std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
    auto top = tok.get_topics();
    if (top && !top->empty())
      std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
    std::cout << std::endl;
  }

public:
  action_listener(const std::string& name) : name_(name) {}
};

class callback : public virtual mqtt::callback, public virtual mqtt::iaction_listener {
  int nretry_;
  mqtt::async_client& cli_;
  mqtt::connect_options& connOpts_;
  action_listener subListener_;
  void reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    try {
      cli_.connect(connOpts_, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
      std::cerr << "Error: " << exc.what() << std::endl;
      exit(1);
    }
  }

  void on_failure(const mqtt::token& tok) override {
    std::cout << "Connection failed" << std::endl;
    if (++nretry_ > N_RETRY_ATTEMPTS)
      exit(1);
    reconnect();
  }

  void on_success(const mqtt::token& tok) override {
    std::cout << "\nConnection success" << std::endl;
    std::cout << "\nSubscribing to topic '" << TOPIC << "'\n"
      << "\tfor client " << CLIENT_ID
      << " using QoS" << QOS << "\n"
      << "\nPress Q<Enter> to quit\n" << std::endl;
    cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
  }

  void connection_lost(const std::string& cause) override {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
      std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    nretry_ = 0;
    reconnect();
  }

  void message_arrived(mqtt::const_message_ptr msg) override {
    std::cout << "Message arrived" << std::endl;
    std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
    std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
    string payload;
    vector<data_node> data_tree_regions;
    if(msg->get_topic().compare("web/insert") == 0){
      // llamar a insertar nodo
      // obj = convertJSONtoObject(msg->to_string());
      // data_tree_regions = insert();
      payload = convertRegionsToJSON(data_tree_regions);
      mqttPublish("cpp/insert", payload);
    } else if(msg->get_topic().compare("web/knn") == 0){
      // llamar knn y generar el payload
      mqttPublish("cpp/knn", payload);
    } else if(msg->get_topic().compare("web/search") == 0){
      // llamar search, generar el payload
      mqttPublish("cpp/search", payload);
    }
  }

  void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
  callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
        : nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};

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
