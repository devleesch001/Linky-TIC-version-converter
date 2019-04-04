//
// Created by dev on 02/04/19.
//
#include "header.h"

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
void InsertDB(PGconn *conn, string adco, string east, string irms1, string urms1) {
    // Append the SQL statment
    std::string sSQL;
    sSQL.append("INSERT INTO proto (adsc, east, irms1, urms1) VALUES ('");
    sSQL.append(adco);
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
        cout << "Insert employee record failed\n";
        cout << PQresultStatus(res);
        //PQclear(res);
        CloseConn(conn);
    }

    cout << "Insert employee record - OK\n";

    // Clear result
    PQclear(res);
}

/* Fetch employee record and display it on screen */
void FetchDBrec(PGconn *conn){
    // Will hold the number of field in employee table
    int nFields;

    // Start a transaction block
    PGresult *res  = PQexec(conn, "BEGIN");

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << "BEGIN command failed\n";
        PQclear(res);
        CloseConn(conn);
    }

    // Clear result
    PQclear(res);

    // Fetch rows from employee table
    res = PQexec(conn, "DECLARE emprec CURSOR FOR select * from test");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        cout << "DECLARE CURSOR failed\n";
        PQclear(res);
        CloseConn(conn);
    }

    // Clear result
    PQclear(res);

    res = PQexec(conn, "FETCH ALL in emprec");

    if (PQresultStatus(res) != PGRES_TUPLES_OK){
        cout << "FETCH ALL failed\n";
        PQclear(res);
        CloseConn(conn);
    }

    // Get the field name
    nFields = PQnfields(res);

    // Prepare the header with employee table field name
    cout << "\nFetch test record:";
    cout << "\n********************************************************************\n";
    for (int i = 0; i < nFields; i++)
        printf("%-30s", PQfname(res, i));
    cout << "\n********************************************************************\n";

    // Next, print out the employee record for each row
    for (int i = 0; i < PQntuples(res); i++){
        for (int j = 0; j < nFields; j++)
            printf("%-30s", PQgetvalue(res, i, j));
        printf("\n");
    }

    PQclear(res);

    // Close the emprec
    res = PQexec(conn, "CLOSE emprec");
    PQclear(res);

    // End the transaction
    res = PQexec(conn, "END");

    // Clear result
    PQclear(res);
}

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

int postgres(string adco, string east, string irms1, string urms1){
    PGconn *conn = NULL;

    conn = ConnectDB();

    if (conn != NULL)
    {
        //CreateEmployeeTable(conn);
        if (adco != "" && east != "" && irms1 != "" && urms1 != ""){
            InsertDB(conn, adco, east, irms1, urms1);
            CloseConn(conn);
        }
        //FetchEmployeeRec(conn);


        //RemoveAllEmployeeRec(conn);

        //DropEmployeeTable(conn);


    }


    return 0;
}


