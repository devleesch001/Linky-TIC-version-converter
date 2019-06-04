//
// Created by dev on 08/04/19.
//


#include "header.h"

extern string dbconnect;
//#include "Config.h"

using namespace std;

void loadConfig(Config& config) {
    ifstream fin("config.ini"); //flux d'entrée sur le fichier config.ini
    string line;
    while (getline(fin, line)) { //boucle pour obtenir la ligne, avec le flux dans la varible line  
        istringstream sin(line.substr(line.find("=") + 1)); // permet de recupere la chaine de caractere apres le caractere = dans le chier config.ini
        if (line.find("user") != -1)
            sin >> config.db.user;          // transfer la variable recupere precedement dans la structure config.db
        else if (line.find("password") != -1)
            sin >> config.db.password;
        else if (line.find("dbname") != -1)
            sin >> config.db.dbname;
        else if (line.find("table") != -1)
            sin >> config.db.table;
        else if (line.find("adresseIP") != -1)
            sin >> config.db.adresseIP;
        else if (line.find("port") != -1)
            sin >> config.db.port;
    }
}

void buildConfig() {
    std::ofstream outfile ("config.ini");           // crée un fichier config.ini
    outfile << "### ConfigDB ###" << std::endl      // cree la config text par default 
            << "user = utilisateur" << std::endl
            << "password = toor" << std::endl
            << "dbname = nom_de_la_db" << std::endl
            << "table = nom_de_la_table" << std::endl
            << "adresseIP = addrese_IP_du_serveur " << std::endl
            << "port = 80" << std::endl;
    outfile.close();
}

void initConfig(Config& config){ 
    fstream fileStream;             //
    fileStream.open("config.ini");  // charge le fichier config.ini
    if (fileStream.fail()) {        // si le fichier n'est pas charger
        buildConfig();              // alors on appelle la fonction buildConfig
    }
    loadConfig(config);             // appale la fonction loadconfig
}

void reloadConfig(){
    removeConfig();     //appelle les fonctions
    buildConfig();      // reconstuit le fichier 
//    configure();
}

void removeConfig(){
    remove("config.ini"); //suprime le fichier
}

void displayConfig(){
    ifstream fichier("config.ini", ios::in);  // on ouvre en lecture
    if(fichier)  // si l'ouverture a fonctionné
    {
        string ligne;  // déclaration d'une chaîne qui contiendra la ligne lue
        while(getline(fichier, ligne)){  // on met dans "contenu" la ligne
            cout << ligne << endl;  // on affiche la ligne
        }
        fichier.close();
    }
    else
        //cerr << "Fichier Config inexistant ou inacesible (--config-re) pour régénérer la configuration !" << endl;
        cerr << "Config file doesn't exist or is not accessible... (--config-re) to reload configuration !" << endl;

//    initConfig(config);
//    cout << config.user << '\n';
//    cout << config.password << '\n';
//    cout << config.dbname << '\n';
//    cout << config.adresseIP << '\n';
//    cout << config.port << '\n';
}

void configure(Config& config){     // construit  la chaine en fonction des variable 

    ostringstream oss;
    oss << "user="  << config.db.user
    << " password=" << config.db.password
    << " dbname="   << config.db.dbname
    << " hostaddr=" << config.db.adresseIP
    << " port="     << config.db.port;

     dbconnect = oss.str();
}
