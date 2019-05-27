//
// Created by dev on 08/04/19.
//


#include "header.h"

extern string dbconnect;
//#include "Config.h"

using namespace std;

void loadConfig(Config& config) {
    ifstream fin("config.ini");
    string line;
    while (getline(fin, line)) {
        istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("user") != -1)
            sin >> config.db.user;
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
    std::ofstream outfile ("config.ini");
    outfile << "### ConfigDB ###" << std::endl
            << "user = postgres" << std::endl
            << "password = toor" << std::endl
            << "dbname = alexis" << std::endl
            << "table = linky" << std::endl
            << "adresseIP = 127.0.0.1" << std::endl
            << "port = 8080" << std::endl;
    outfile.close();
}

void initConfig(Config& config){
    fstream fileStream;
    fileStream.open("config.ini");
    if (fileStream.fail()) {
        buildConfig();
    }
    loadConfig(config);
}

void reloadConfig(){
    removeConfig();
    buildConfig();
//    configure();
}

void removeConfig(){
    remove("config.ini");
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

void configure(Config& config){

    ostringstream oss;
    oss << "user="  << config.db.user
    << " password=" << config.db.password
    << " dbname="   << config.db.dbname
    << " hostaddr=" << config.db.adresseIP
    << " port="     << config.db.port;

     dbconnect = oss.str();
}