#include "mqtt/async_client.h"

//const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string SERVER_ADDRESS("tcp://r-tree.nezads.com:1883");
const std::string CLIENT_ID("console_client");
const std::string TOPIC("#");

using namespace std;
using namespace std::chrono;

const int QOS = 1;
const int N_RETRY_ATTEMPTS = 5;
const auto PERIOD = seconds(5);
const int MAX_BUFFERED_MSGS = 120;
const string PERSIST_DIR { "data-persist" };

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
    if(msg->get_topic().compare("web/insert") == 0){
      // llamar a insertar nodo
      // publicar resultado en cpp/insert
    } else if(msg->get_topic().compare("web/knn") == 0){
      // llamar knn
      // publicar resultado en cpp/knn
    } else if(msg->get_topic().compare("web/search") == 0){
      // llamar search
      // publicar resultado en cpp/search
    }
  }

  void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
  callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
        : nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};
