#include "paho_client.h"

// g++ data_publish.cpp -lpaho-mqttpp3

int mqttClient(string TOPIC) {
	string address = SERVER_ADDRESS;

  // mqtt::async_client cli(address, "", MAX_BUFFERED_MSGS, PERSIST_DIR);
	mqtt::async_client cli(address, "");

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);

	mqtt::topic top(cli, TOPIC, QOS, true);

	try {
		cout << "Connecting to server '" << address << "'..." << flush;
		cli.connect(connOpts)->wait();
		cout << "OK\n" << endl;

		char tmbuf[32];
		unsigned nsample = 0;

		auto tm = steady_clock::now();

		while (true) {
			this_thread::sleep_until(tm);

			time_t t = system_clock::to_time_t(system_clock::now());
			strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));

			string payload = "{\"data\":[{\"id\":\"R0\", \"points\":[2,3]}, {\"id\":\"R1\", \"points\":[9,3]}, {\"id\":\"R2\", \"points\":[8,4]} ]}";
			char* cstr = new char [payload.size() + 1];
			std::strcpy(cstr, payload.c_str());
			cout << payload << endl;

			top.publish(std::move(cstr));

			tm += PERIOD;
		}

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

int main(int argc, char* argv[]) {
  return mqttClient("data/rand");
}

