//
//  Client.h
//  EECS 281 Project 2
//
//  Created by John West on 3/1/15.
//  Copyright (c) 2015 John West. All rights reserved.
//

#ifndef EECS_281_Project_2_Client_h
#define EECS_281_Project_2_Client_h

class Client {
private:
	int client_id;
	int bought;
	int sold;
	int net_value;

public:
	Client();
	Client(int c_id);

	int get_client_id();

	void add_bought(int num);
	void add_sold(int num);
	void add_net_value(int price);
	int get_bought() { return bought; }
	int get_sold() { return sold; }
	int get_net_value() { return net_value; }
};

Client::Client()
: client_id(0), bought(0), sold(0), net_value(0) {}

Client::Client(int c_id)
: client_id(c_id), bought(0), sold(0), net_value(0) {}

int Client::get_client_id() {

	return client_id;
}

void Client::add_bought(int num) {
	bought += num;
	std::cerr << "Client: " << client_id << ", bought: " << bought << std::endl;
	return;
}

void Client::add_sold(int num) {
	sold += num;
	std::cerr << "Client: " << client_id << ", sold: " << sold << std::endl;
	return;
}

void Client::add_net_value(int price) {
	net_value += price;
	std::cerr << "Client: " << client_id << ", net_value: " << net_value << std::endl;
	return;
}

#endif
