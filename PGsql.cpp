//
// Created by dev on 02/04/19.
//
#include "header.h"

string old_id, old_adsc, old_prm;
int old_east, old_irms1, old_urms1;

void CloseConn(PGconn *conn);
void InsertDB(PGconn *conn, string adsc, string east, string irms1, string urms1);
void FetchDBrec(PGconn *conn);
void RemoveRecDB(PGconn *conn);
int postgres(string adsc, string east, string irms1, string urms1);

void CloseConn(PGconn *conn){
    PQfinish(conn);
}

/* Establish connection to database */
PGconn *ConnectDB(){
    PGconn *conn = NULL;

    // Make a connection to the database
    conn = PQconnectdb("user=postgres password=toor dbname=alexis hostaddr=51.83.78.102 port=5432");

    // Check to see that the backend connection was successfully made
    if (PQstatus(conn) != CONNECTION_OK)
    {
        cout << "Connection to database failed.\n";
        CloseConn(conn);
    }

    cout << "Connection to database - OK\n";

    return conn;
}

/* Append SQL statement and insert record into employee table */
void InsertDB(PGconn *conn, string adsc, string prm, string east, string irms1, string urms1) {
    cout << "This donnes is transmit" << adsc << " - " << prm << " - " << east << " - " << irms1 << " - " << urms1 << endl << endl;

    // Append the SQL statment
    std::string sSQL;
    sSQL.append("INSERT INTO proto (adsc, prm, east, irms1, urms1) VALUES ('");
    sSQL.append(adsc);
    sSQL.append("', '");
    sSQL.append(prm);
    sSQL.append("', '");
    sSQL.append(east);
    sSQL.append("', '");
    sSQL.append(irms1);
    sSQL.append("', '");
    sSQL.append(urms1);
    sSQL.append("')");

    // Execute with sql statement
    PGresult *res = PQexec(conn, sSQL.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << "Insert data record failed\n";
        cout << PQresultStatus(res);
        //PQclear(res);
        CloseConn(conn);
    }

    cout << "Insert data record - OK\n";

    // Clear result
    PQclear(res);
}

/* Fetch employee record and display it on screen */
/*
void FetchDBrec(PGconn *conn){
    std::string dSQL;
    dSQL.append("SELECT * FROM proto ORDER BY id DESC LIMIT 1");
    PGresult *disp = PQexec(conn, dSQL.c_str());
    //printf ("%s\n", PQgetvalue(disp, 0, 1));
    oadsc = PQgetvalue(disp, 0, 1);
    oeast = PQgetvalue(disp, 0, 2);
    oirms1 = PQgetvalue(disp, 0, 3);
    ourms1 = PQgetvalue(disp, 0, 4);

    cout << oadsc << " - " << oeast << " - " << oirms1 << " - " << ourms1 << endl;
    // Close the emprec
    // Clear result
    PQclear(disp);
}
*/
/* Erase all record in employee table */
void RemoveRecDB(PGconn *conn){
    // Execute with sql statement
    PGresult *res = PQexec(conn, "DELETE FROM employee");

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << "Delete employees record failed.\n";
        PQclear(res);
        CloseConn(conn);
    }

    cout << "\nDelete employees record - OK\n";

    // Clear result
    PQclear(res);
}

int postgres(string adsc, string prm, string east, string irms1, string urms1){
    PGconn *conn = NULL;


        //CreateEmployeeTable(conn);


        if (adsc != "" && prm != "" && east != "" && irms1 != "" && urms1 != ""){
            string::size_type sz;
            int i_east = stoi (east, &sz);
            int i_irms1 = stoi (irms1, &sz);
            int i_urms1 = stoi (urms1, &sz);
            int var_east = i_east - old_east;
            int var_urms1 = i_urms1 - old_urms1;

            cout << "if ("<< adsc << " != " << old_adsc << " || " <<  prm << " != " << old_prm << " || (" << var_east << " <= 10 || " << var_east << ">= -10) || " << i_irms1 << " != " << old_irms1 << " || (" << var_urms1 << " >= " << 5 << " || " << var_urms1 << "<= -5))" << endl;

            if (adsc != old_adsc || prm != old_prm ||  (var_east >= 10 || var_east <= -10) || i_irms1 != old_irms1 || (var_urms1 >= 5 || var_urms1 <= -5)){
                cout << "une valeur est inegale" << endl << endl;
                conn = ConnectDB();

                if (conn != NULL)
                {
                    InsertDB(conn, adsc, prm, east, irms1, urms1);

                    old_adsc  = adsc;
                    old_prm   = prm;
                    old_east  = i_east;
                    old_irms1 = i_irms1;
                    old_urms1 = i_urms1;

                }

                CloseConn(conn);
            }
            else{
                cout << "Donne toute egale" << endl << endl;
            }

        //FetchDBrec(conn);
        //RemoveAllEmployeeRec(conn);
        //DropEmployeeTable(conn);

    }


    return 0;
}


