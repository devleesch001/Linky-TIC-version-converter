//
// Created by dev on 02/04/19.
//
#include "header.h"
extern string dbconnect;

string old_id, old_adsc, old_prm, old_east, old_irms1, old_urms1, old_date;
int old_i_east, old_i_irms1, old_i_urms1;


void CloseConn(PGconn *conn){
    PQfinish(conn);
}

/* Establish connection to database */
PGconn *ConnectDB(){
    PGconn *conn = NULL;

    //string dbConf = "user=postgres password=toor dbname=alexis hostaddr=0.0.0.0 port=000";
    const char *SQLcon_char = dbconnect.c_str();

    // Make a connection to the database
    conn = PQconnectdb(SQLcon_char);

    // Check to see that the backend connection was successfully made
    if (PQstatus(conn) != CONNECTION_OK){
        clog << "Connection to database failed.\n";
        CloseConn(conn);

    }
    else{
        clog << "Connection to database - OK\n";
        clog << "disconnect" << endl;
        //CloseConn(conn);
    }

    return conn;
}

void CreateTable(PGconn *conn, string table)
{
    // Execute with sql statement

    ostringstream oss;
    oss << "CREATE TABLE "
        << table
        << " (id serial not null constraint "
        << table << "_pk "
        << "primary key, adsc varchar(12) not null, prm varchar(14), east integer, irms1 integer, urms1 integer, date varchar(13));";

    cout << oss.str().c_str() << endl;

    PGresult *res2 = PQexec(conn, oss.str().c_str());

    if (PQresultStatus(res2) != PGRES_COMMAND_OK)
    {
        cout << "Create table failed\n";
        PQclear(res2);
        CloseConn(conn);
    } else{
        cout << "Create table - OK\n";
        // Clear result
        PQclear(res2);
    }

}

/* Append SQL statement and insert record into employee table */
void InsertDB(PGconn *conn, string table, string adsc, string prm, string east, string irms1, string urms1, string date) {

    clog << "Data transmitted : " << endl
    //<< old_adsc << " - " << old_prm << " - " << old_east << " - " << old_irms1 << " - " << old_urms1 << endl
    << adsc << " - " << prm << " - " << east << " - " << irms1 << " - " << urms1 << endl << endl;

    // Append the SQL statment
    std::string sSQL;
    sSQL.append("INSERT INTO ");
    sSQL.append(table);
    sSQL.append("(adsc, prm, east, irms1, urms1, date) VALUES ('");
    sSQL.append(adsc);
    sSQL.append("', '");
    sSQL.append(prm);
    sSQL.append("', '");
    sSQL.append(east);
    sSQL.append("', '");
    sSQL.append(irms1);
    sSQL.append("', '");
    sSQL.append(urms1);
    sSQL.append("', '");
    sSQL.append(date);
    sSQL.append("')");

    // Execute with sql statement
    PGresult *res = PQexec(conn, sSQL.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        clog << "Data record insertion failed.\n";
        clog << " error "<< PQresultStatus(res);
        PQclear(res);
        CloseConn(conn);
    }

    clog << "Insert data record - OK\n";

    // Clear result
    PQclear(res);
}
int postgres(Config& config, string adsc, string prm, string east, string irms1, string urms1, string date) {
    PGconn *conn = NULL;
    conn = ConnectDB();

        //CreateEmployeeTable(conn);

        if (adsc != "" && prm != "" && east != "" && irms1 != "" && urms1 != "" && date !=""){

            string::size_type sz;
            int i_east = stoi (east, &sz);
            int i_irms1 = stoi (irms1, &sz);
            int i_urms1 = stoi (urms1, &sz);
            int var_east = i_east - old_i_east;
            int var_urms1 = i_urms1 - old_i_urms1;

            clog << "if ("<< adsc << " != " << old_adsc << " || " <<  prm << " != " << old_prm << " || (" << var_east << " <= 10 || " << var_east << ">= -10) || " << i_irms1 << " != " << old_irms1 << " || (" << var_urms1 << " >= " << 5 << " || " << var_urms1 << "<= -5) || ("<< date << " != " << old_date << ")"<< endl;

            if (adsc != old_adsc || prm != old_prm ||  (var_east >= 10 || var_east <= -10) || i_irms1 != old_i_irms1 || (var_urms1 >= 5 || var_urms1 <= -5) || (date != date)){
                clog << "Data changed... Data will be updated. " << endl << endl;

                if (conn != NULL){

                    if (old_adsc != "" && old_prm != "" && old_east != "" && old_irms1 != "" && old_urms1 != "" && old_date != "") {
                        InsertDB(conn, config.db.table, old_adsc, old_prm, old_east, old_irms1, old_urms1, old_date);
                    }
                    InsertDB(conn, config.db.table, adsc, prm, east, irms1, urms1, date);

                    old_adsc  = adsc;
                    old_prm   = prm;
                    old_east  = east;
                    old_i_east  = i_east;
                    old_irms1 = irms1;
                    old_i_irms1 = i_irms1;
                    old_urms1 = urms1;
                    old_i_urms1 = i_urms1;

                }
            }
            else{
                clog << "No data changes... Data will not be updated." << endl << endl;
                //CloseConn(conn);
            }
        //FetchDBrec(conn);
        //RemoveAllEmployeeRec(conn);
        //DropEmployeeTable(conn);
        CloseConn(conn);
    }
    return 0;
}

void dbtestConn(){
    PGconn *conn = NULL;
    conn = ConnectDB();

}

void databaseBuild(Config& config){
    PGconn *conn = NULL;
    conn = ConnectDB();

    CreateTable(conn, config.db.table);
}