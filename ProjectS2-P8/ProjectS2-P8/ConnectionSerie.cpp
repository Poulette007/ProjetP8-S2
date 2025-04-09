#include "ConnectionSerie.h"
std::string ConnectionSerie::raw_msg;
json ConnectionSerie::json_data;
SerialPort* ConnectionSerie::SP;
ConnectionSerie::ConnectionSerie()
{
	SP = new SerialPort(com.c_str(), BAUD);
    if (!SP->isConnected()) {
        cerr << "Impossible de se connecter au port " << string(com) <<  endl;
    }
}

bool ConnectionSerie::Send(SerialPort* SP, json j_msg)
{
	// Return 0 if error
	string msg = j_msg.dump();
	bool ret = SP->writeSerialPort(msg.c_str(), msg.length());
	return ret;
}
bool ConnectionSerie::Envoie(json j_msg)
{
	if (SP->isConnected())
	{
		return Send(SP, j_msg);
	}
    return false;
}
bool ConnectionSerie::ReceiveRaw(SerialPort* SP, string& msg)
{

    // Message output in msg
    string str_buffer;
    char char_buffer[MSG_MAX_SIZE];
    int buffer_size;

    msg.clear(); // Effacer l'ancien message

    // Lire depuis le port série
    buffer_size = SP->readSerialPort(char_buffer, MSG_MAX_SIZE);
    if (buffer_size > 0)
    {
        str_buffer.assign(char_buffer, buffer_size);
        raw_msg.append(str_buffer);  // Ajoute au buffer global

        size_t trouve = raw_msg.find("\n");  // Cherche le délimiteur `\n`
		//std::cout << "Buffer actuel avant extraction : " << raw_msg << std::endl; //debug

		if (trouve != string::npos)  // Si le \n est trouve (npos = Not Found)
        {
			msg = raw_msg.substr(0, trouve);  // Extrait le message complet sans le \n
            raw_msg.erase(0, trouve + 1);  // Supprime le message 
            return true;
        }
    }

    return false;  // Aucun message complet n'a été reçu

}
int ConnectionSerie::getValue(string index)
{
    if (!json_data.is_object()) {  // Vérifie si JSON est un objet valide
        std::cerr << "Erreur: JSON recu n'est pas un objet valide : " << json_data.dump() << std::endl;
        return -1;
    }

    if (!json_data.contains(index)) {  // Vérifie si la clé existe
		//std::cerr << "Erreur: Cle '" << index << "' introuvable dans le JSON !" << std::endl;     //debug
        return -2;
    }

    if (json_data[index].is_null()) {  // Vérifie si la valeur est NULL
        std::cerr << "Erreur: Cle '" << index << "' contient une valeur null !" << std::endl;
        return -3;
    }

    try {
		return json_data[index].get<int>();  // Retourne la valeur en int
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "Erreur JSON: " << e.what() << std::endl;
        return -4;
    }
}
json ConnectionSerie::getJson()
{
    //json j_msg_rcv;

    //// Reception message Arduino
    //j_msg_rcv.clear(); // effacer le message precedent
	


    //if (raw_msg.empty()) 
    //{
    //    return json::object();  // Retourne un JSON vide si aucune donnée
    //}

    //try 
    //{
    //    std::cout << "Données brutes reçues : " << raw_msg << std::endl;
    //    j_msg_rcv = json::parse(raw_msg);  // Parsing sécurisé
    //    raw_msg.clear();  // Nettoie après un parsing réussi
    //    return j_msg_rcv;
    //}
    //catch (const nlohmann::json::parse_error& e)
    //{
    //    cerr << "Erreur de parsing JSON: " << e.what() << " | Données brutes: " << raw_msg << endl;
    //    raw_msg.clear();  // Vide `raw_msg` pour éviter de reparser un JSON corrompu
    //    return json::object();
    //} //retourne un json vide, faut pas faie NULL
    if (json_data.empty())
    {
		return json::object();  // Retourne un JSON vide si aucune donnée
    }
	return json_data;
    
}
bool ConnectionSerie::hasData()
{
    std::string tempBuffer;


    // Vérifie si on peut lire des données sans les consommer
    if (ReceiveRaw(SP, tempBuffer))
    {
		if (!tempBuffer.empty()) 
        {
            raw_msg.append(tempBuffer);
            try
            {
				json_data = json::parse(raw_msg); //met a jour le json publix
				raw_msg.clear();
                //cout << "hasData() appelé, retour " << endl;
				return true;
			}
            catch (const nlohmann::json::parse_error& e)
            {
             
                //cerr << "Erreur de parsing JSON: " << e.what() << " | Données brutes: " << raw_msg << endl;
                raw_msg.clear();  // Vide `raw_msg` pour éviter de reparser un JSON corrompu
                json_data.clear();
                return false;
            }
		
		}
	}

    return false;
  
}
bool ConnectionSerie::isConnected()
{
	return SP->isConnected();
}

void ConnectionSerie::closeSerial()
{
    CloseHandle(SP->handler);
}
