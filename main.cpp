#include "header.h"

// constante pour boost
namespace { 
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
}


bool stop;
string trame, ADSC, EAST, IRMS1, URMS1, PRM, DATE;
string dbconnect;
int fd, fl;


int main(int argc, char **argv) { //argc et **argv sont les variables arguments 
    Config config;          // Fait appelle a la structure declare dans le header.h
    initConfig(config);     // Fait appelle a la Fonction initConfig et envoye la structur de variable config
    configure(config);      // 
    try {
        /** Define and parse the program options */
        namespace po = boost::program_options;
        po::options_description desc("Options");
        desc.add_options()
                ("help,h", "Print help messages")
                ("config,c", "show config")
                ("config-re,r", "reset config to default")
                ("database,d", "show and test db-configuration")
                ("database-build,b", "create table in database")
                ("version,v", "version");

        po::variables_map vm;
        try {
            po::store(po::parse_command_line(argc, argv, desc),
                      vm); // can throw

            /** --help option */
            if (vm.count("help")) {
                std::cout << "Basic Command Line Parameter App" << std::endl
                          << desc << std::endl;
                return SUCCESS;
            }
            if (vm.count("config")) {
                displayConfig();
                return SUCCESS;
            }
            if (vm.count("config-re")) {
                reloadConfig();
                return SUCCESS;
            }
            if (vm.count("database")) {
                cout << "initC" << endl;
                initConfig(config);

                cout << "conf" << endl;
                configure(config);

                cout << "testCon" << endl;
                dbtestConn();
                return SUCCESS;
            }
            if (vm.count("database-build")) {
                initConfig(config);
                configure(config);
                databaseBuild(config);
                return SUCCESS;
            }
            if (vm.count("version")) {
                std::cout << "version 0.1a" << std::endl;
                return SUCCESS;
            }

            po::notify(vm); // throws on error, so do after help in case
            // there are any problems
        }
        catch (po::error &e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << desc << std::endl;
            return ERROR_IN_COMMAND_LINE;
        }
    }
    catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: "
                  << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;


    }

    openport();
    thread serialread(readport);
    serialread.detach();

    while (1) {

        postgres(config, ADSC, PRM, EAST, IRMS1, URMS1, DATE);
        sleep(1);
        if (stop == true) {
            return 0;
        }
    }
}

struct termios tty;

void openport(void) {
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror(MODEMDEVICE);
    }

    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &tty); /* save current serial port settings */
    bzero(&tty, sizeof(tty));

//    tty.c_cflag = BAUDRATE | CRTSCTS | CS7 | CSTOPB;
    tty.c_cflag = BAUDRATE | PARENB | CS7;
    tty.c_iflag = IGNPAR | ICRNL;

    tty.c_oflag = 0;
    tty.c_lflag = ICANON;
    tty.c_cc[VINTR] = 0;     /* Ctrl-c */
    tty.c_cc[VQUIT] = 0;     /* Ctrl-\ */
    tty.c_cc[VERASE] = 0;     /* del */
    tty.c_cc[VKILL] = 0;     /* @ */
    // tty.c_cc[VEOF]     = 4;     /* Ctrl-d */
    tty.c_cc[VTIME] = 0;     /* inter-character timer unused */
    tty.c_cc[VMIN] = 0;     /* blocking read until 1 character arrives */
    tty.c_cc[VSWTC] = 0;     /* '\0' */
    tty.c_cc[VSTART] = 0;     /* Ctrl-q */
    tty.c_cc[VSTOP] = 0;     /* Ctrl-s */
    tty.c_cc[VSUSP] = 0;     /* Ctrl-z */
    tty.c_cc[VEOL] = 0;     /* '\0' */
    tty.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    tty.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    tty.c_cc[VWERASE] = 0;     /* Ctrl-w */
    tty.c_cc[VLNEXT] = 0;     /* Ctrl-v */
    tty.c_cc[VEOL2] = 0;     /* '\0' */
}

void readport(void) {
    unsigned char buff;
    while (1) {
        int n = read(fd, &buff, 1);
        if (n == 0) break;

        if (buff == LF) {
            //printf("%c",buff);
        } else if (buff == CR) {
            analyse(trame);
            //cout << trame;
            //printf ("%c", buff);
            trame = '\0';
        } else if (buff == ETX) {
            //printf("\nETX = 0x%x", buff);
        } else if (buff == STX) {
            //printf("\nSTX = 0x%x", buff);
        } else {
            trame += buff;
        }
    }
}

void analyse(string a) {
    istringstream iss(a);
    string etiquette, donnee, checksum;

    getline(iss, etiquette, '\t'); // permet de sépare en 3 string les 3 elemeent transmi separe par une tabulation
    getline(iss, donnee, '\t');
    getline(iss, checksum);

    etiquette.erase(0, 1); //     utilise car le string etiquette possede un char vide a [0]

    if (etiquette == "ADSC") {
        ADSC = donnee;
    } else if (etiquette == "PRM") {
        PRM = donnee;
    } else if (etiquette == "EAST") {
        EAST = donnee;
    } else if (etiquette == "IRMS1") {
        IRMS1 = donnee;
    } else if (etiquette == "URMS1") {
        URMS1 = donnee;
    } else if (etiquette == "DATE") {
        DATE = donnee;
    } else if (etiquette == "") {
    }
}

//
//void sendport(void) {
////    if ((ADSC != "") && (IRMS1 != "")) {
//        ostringstream trame;
//        trame << STX
//              << LF << "ADC0" << ' ' << ADSC << 'A' << CR
//              << LF << "OPTARIF" << ' ' << "BASE" << 'B' << CR
//              << LF << "ISOUSC" << ' ' << "45" << 'C' << CR
//              << LF << "BASE" << ' ' << "000004768" << 'D' << CR
//              << LF << "PTEC" << ' ' << "TH.." << "E" << CR
//              << LF << "IINST" << ' ' << IRMS1 << 'F' << CR
//              << LF << "IMAX" << ' ' << "090" << 'G' << CR
//              << LF << "PAPP" << ' ' << "00000" << 'H' << CR
//              << LF << "HHPHC" << ' ' << "A" << 'I' << CR
//              << LF << "MOTDETAT" << ' ' << "000000" << 'G' << CR;
//        string buffer = trame.str();
//
////    const char *cstr = buffer.c_str();
//
//        int b = buffer.length();
//        char char_test[b + 1];
//        strcpy(char_test, buffer.c_str());
//
//        int n = write(fl, char_test, b);
//
//        std::cout << n << std::endl;
//        std::cout << std::endl <<
//                  "----------------------------" <<
//                  std::endl;
//
//
//        if (n < 0) {
//            fputs("write() of bytes failed!\n", stderr);
//        } else {
//            printf("Image sent successfully %d\n", n);
//        }
////        close(fl);
//
//    }
