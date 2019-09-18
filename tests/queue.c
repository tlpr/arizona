#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>

int main ()
{

	printf("Detected MySQL client version: %s\n", mysql_get_client_info());
	MYSQL *connection = mysql_init(NULL);
	
	char db_user[] = "root";
	char db_pass[] = "654321";
	char db_host[] = "127.0.0.1";
	char db_name[] = "tlpr-dev";

	if (mysql_real_connect (
		connection, db_host,
		db_user, db_pass,
		db_name, 0, NULL, 0
	) == NULL) printf("Could not connect.");

	char *sql_query = "SELECT * FROM `songs`";
	mysql_query(connection, sql_query);

	MYSQL_RES *result = mysql_store_result(connection);
	if (result == NULL)
	{
		printf("%s\n", mysql_error(connection));
		mysql_close(connection);
		exit(1);
	}

	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;

	while ( (row = mysql_fetch_row(result)) )
	{
	
		for (int i = 0; i < num_fields; i++)
			printf("%s ", row[i] ? row[i] : NULL);

		printf("\n");

	}

	mysql_free_result(result);
	mysql_close(connection);

	return 0;

}

