#include "database.h"

void init_database(data_base * database)
{
	char ID0[8]="45176601";
	char PIN0[5]="11111";
	for(int i=0;i<ALLOWED;i++)
	{
		for(int j=0;j<8;j++)
			if(i==0)
				database->allowed_IDS[i][j]=ID0[j];
			else
				database->allowed_IDS[i][j]='0';
		for(int j=0;j<5;j++)
			if(i==0)
				database->allowed_PINS[i][j]=PIN0[j];
			else
				database->allowed_PINS[i][j]='0';

	}
}

bool is_ID_allowed(data_base * database,char * ID, int ID_len)
{
	bool validData;
	for (int i = 0; i < ALLOWED; i++)
	{
		validData = true;
		for (int j = 0; j < ID_len; j++)
		{
			if (ID[j] != database->allowed_IDS[i][j])
			{
				validData = false;
				break;
			}

		}
		if (validData == true)
			break;
	}
	return validData;
}

bool is_PIN_correct(data_base * database,char * ID, int ID_len, char * pin, int PIN_len)
{
	bool validData;
	int i =0;
	for (i = 0; i < ALLOWED; i++)
	{
		validData = true;
		for (int j = 0; j < ID_len; j++)
		{
			if (ID[j] != database->allowed_IDS[i][j])
			{
				validData = false;
				break;
			}

		}
		if (validData == true)
			break;
	}
	if(validData ==true)
	{
		for (int j = 0; j < PIN_len; j++)
		{
			if (pin[j] != database->allowed_PINS[i][j])
			{
				validData = false;
				break;
			}

		}
	}
	return validData;
}

void add_to_whitelist(char * ID, int ID_len,char * pin)
{

}

void ban(char * ID, int ID_len)
{

}

int is_banned(char * ID, int ID_len)
{
	return 1;
}
