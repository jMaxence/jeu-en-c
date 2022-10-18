#pragma once
#include <iostream>
#include <mosquitto.h>


class MQTTClient
{
	//Attribut de la class
private:
	static MQTTClient * instance;
	MQTTClient(const char * serverIP, int serverPort);//Constructeur avec param�tre IP du serveur et port du serveur

	struct mosquitto * client;

public:
	static MQTTClient * getInstance();

	//M�thodes de la class 
	void loopOnce();
	void sendMessage(std::string topic, std::string message);
	void subscribe(std::string topic);
};