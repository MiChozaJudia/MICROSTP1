#include "database.h"

void init_database(data_base * database)
{
	char ID0[8]="45176601";
	char PIN0[5]="11111";
	char ID1[8]="00000000";
	char PIN1[5]="00000";
	for(int i=0;i<ALLOWED;i++)
	{
		for(int j=0;j<8;j++)
			if(i==0)
				database->allowed_IDS[i][j]=ID0[j];
			else if (i==1)
				database->allowed_IDS[i][j]=ID1[j];
			else
				database->allowed_IDS[i][j]='n';
		for(int j=0;j<5;j++)
			if(i==0)
				database->allowed_PINS[i][j]=PIN0[j];
			else if(i==1)
				database->allowed_PINS[i][j]=PIN1[j];
			else
				database->allowed_PINS[i][j]='n';
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
			if ((ID[j] != database->allowed_IDS[i][j]) || (ID[j]=='n'))
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

bool can_add_ID(data_base * database,char * ID, int ID_len)
{
	bool canAdd;
	for (int i = 0; i < ALLOWED; i++)
	{
		canAdd =false;
		for (int j = 0; j < ID_len; j++)
		{
			if ((ID[j] != database->allowed_IDS[i][j]))
			{
				canAdd = true;
				break;
			}

		}
		if (canAdd == true)
			break;
	}
	for(int j=0;j<ID_len;j++)
	{
		if(ID[j]=='n')
		{
			canAdd=false;
			break;
		}
	}
	return canAdd;
}

bool can_add_PIN(data_base * database,char * PIN, int PIN_len)
{
	bool canAdd=true;
	for(int i=0;i<PIN_len-1;i++)
	{
		if(PIN[i]=='n')
		{
			canAdd=false;
			break;
		}
	}
	return canAdd;
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
			if ((ID[j] != database->allowed_IDS[i][j]) || (ID[j]=='n'))
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
		for (int j = 0; j < PIN_len - 1; j++)
		{
			if (pin[j] != database->allowed_PINS[i][j] || (pin[j]=='n'))
			{
				validData = false;
				break;
			}

		}
		if(pin[PIN_len - 1] != database->allowed_PINS[i][PIN_len-1])
			validData = false;
	}
	return validData;
}

bool add_to_whitelist(data_base * database,char * ID, int ID_len, char * pin, int PIN_len)
{
	bool foundspace;
	int i;
	for (i = 0; i < ALLOWED; i++)
		{
			foundspace = false;
			for (int j = 0; j < ID_len; j++)
			{
				if (database->allowed_IDS[i][j]=='n')
				{
					foundspace = true;
					break;
				}
			}
			if (foundspace == true)
				break;
		}
	for(int j = 0;j< ID_len;j++)
	{
		database->allowed_IDS[i][j]=ID[j];
	}
	for(int j = 0;j< PIN_len;j++)
	{
		database->allowed_PINS[i][j]=pin[j];
	}
	return foundspace;
}

bool ban(data_base * database,char * ID, int ID_len, int PIN_len)
{
	bool foundID;
	int i =0;
	for (i = 0; i < ALLOWED; i++)
	{
		foundID = true;
		for (int j = 0; j < ID_len; j++)
		{
			if ((ID[j] != database->allowed_IDS[i][j]) || (ID[j]=='n'))
			{
				foundID = false;
				break;
			}

		}
		if (foundID == true)
			break;
	}
	if(foundID ==true)
	{
		for (int j = 0; j < ID_len; j++)
		{
			database->allowed_IDS[i][j]='n';
		}
		for (int j = 0; j < PIN_len; j++)
		{
			database->allowed_PINS[i][j]='n';
		}
	}
	return foundID;
}
