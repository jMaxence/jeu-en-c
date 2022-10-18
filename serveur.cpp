#include "serveur.h"
#include "main.h"

MQTTClient * MQTTClient::instance = nullptr;

void messageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	std::cout << "Mensaje recibido del corredor sobre el tema " << message->topic << " : " << (char*)message->payload << std::endl; //Message recu à se sujet 

	extern int board[8][8];
	extern int mutare; //muter
	extern int numarPiesaMutataMultiJugador; //numération piéce mulitjoueur 

	//Variable globale representant déplacement des piéces
	int antiguoX = (char)((char*)message->payload)[0] - '0';
	int antiguoY = (char)((char*)message->payload)[1] - '0';
	int x = (char)((char*)message->payload)[2] - '0';
	int y = (char)((char*)message->payload)[3] - '0';

	board[y][x] = numarPiesaMutataMultiJugador == 0 ? board[antiguoY][antiguoX] : numarPiesaMutataMultiJugador;
	if (numarPiesaMutataMultiJugador == 0)
		board[antiguoY][antiguoX] = 0;

	if (mutare == 1)
		mutare = 0;
	else
		mutare = 1;

	numarPiesaMutataMultiJugador = 0;
}

MQTTClient::MQTTClient(const char * serverIP, int serverPort)
{
	mosquitto_lib_init(); //Doit être appelé avant tout autres fonctions mosquitto.

	client = mosquitto_new(NULL, true, NULL);

	int prueba = mosquitto_connect(client, serverIP, serverPort, 10);// Connexion au broker
	mosquitto_message_callback_set(client, messageCallback);// Définissez le callback de connexion. Ceci est appelé lorsque le broker envoie un message en réponse à une connexion.

	if (prueba == MOSQ_ERR_SUCCESS)
	{
		std::cout << "Estas conectado";
	}
	else
	{
		std::cout << "Usted no se ha identificado";
	}


	//mosquitto_lib_cleanup();

}

MQTTClient * MQTTClient::getInstance()
{
	if (instance == nullptr)
	{
		instance = new MQTTClient("192.168.64.55", 1883);
	}

	return instance;
}

void MQTTClient::loopOnce()
{
	// La boucle réseau principale pour le client.
	int timeout = 1; int max_packets = 10;
	mosquitto_loop(client, timeout, max_packets);
}

void MQTTClient::sendMessage(std::string topic, std::string message)
{
	int mid; //Publiez un message 
	mosquitto_publish(client, &mid, topic.c_str(), message.length() + 1, message.c_str(), 0, false);
}

void MQTTClient::subscribe(std::string topic)
{
	int mid;
	mosquitto_subscribe(client, &mid, topic.c_str(), 0);
}