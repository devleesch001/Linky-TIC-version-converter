//
// Created by dev on 08/04/19.
//

#include <fstream>
#include "header.h"
//#include "Config.h"

using namespace std;

void loadConfig(Config& config) {
    ifstream fin("config.ini");
    string line;
    while (getline(fin, line)) {
        istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("user") != -1)
            sin >> config.user;
        else if (line.find("password") != -1)
            sin >> config.password;
        else if (line.find("dbname") != -1)
            sin >> config.dbname;
        else if (line.find("adresseIP") != -1)
            sin >> config.adresseIP;
        else if (line.find("port") != -1)
            sin >> config.port;
    }
}

void buildConfig() {
    std::ofstream outfile ("config.ini");
    outfile << "### ConfigDB ###" << std::endl
            << "user = postgres" << std::endl
            << "password = toor" << std::endl
            << "dbname = alexis" << std::endl
            << "adresseIP = 127.0.0.1" << std::endl
            << "port = 8080" << std::endl;
    outfile.close();
}

void initConfig(Config& config){
    fstream fileStream;
    fileStream.open("config.ini");
    if (fileStream.fail()) {
        buildConfig();
        loadConfig(config);
    } else{
        loadConfig(config);
    }
}

void reloadConfig(){
    buildConfig();
}

void removeConfig(){
    remove("config.ini");
}

void displayConfig(Config& config){
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
        cerr << "Fichier Config  !" << endl;

//    initConfig(config);
//    cout << config.user << '\n';
//    cout << config.password << '\n';
//    cout << config.dbname << '\n';
//    cout << config.adresseIP << '\n';
//    cout << config.port << '\n';
}