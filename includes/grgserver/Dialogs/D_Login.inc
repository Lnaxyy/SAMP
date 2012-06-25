ShowDialog:Login(playerID)
{
	CreateDialog(playerID, "Login", DIALOG_STYLE_PASSWORD, "Login", LanguageString(playerID, 262), "OK", LanguageString(playerID, 165));
}

DialogResponse:Login(playerID, response, listItem, inputText[])
{
	if (response)
	{
		if (strlen(inputText))
		{
			MySQLQuery("SELECT `id` FROM `users` WHERE `username` = '%s' AND `password` = '%s'", MySQLEscapeString(GetPlayerNameEx(playerID)), MD5(inputText));
			mysql_store_result();
			if (mysql_num_rows())
			{
				PVar:playerID[USERID] = strval(GetMySQLField("id"));
			}
			else
			{
				SendClientMessageEx(playerID, COLOR_ERROR, 30);
				ShowDialog:Login(playerID);
			}
			mysql_free_result();
			if (PVar:playerID[USERID])
			{
				MySQLQuery("UPDATE `users` SET `loginTime` = NOW(), `clientVersion` = '%s' WHERE `id` = '%d'", GetPlayerVersionEx(playerID), PVar:playerID[USERID]);
				printf("Player logged in: %s (UserID: %d)", GetPlayerNameEx(playerID), PVar:playerID[USERID]);
				PlayerLoop(currentPlayerID)
				{
					if (currentPlayerID != playerID)
					{
						SendClientMessageEx(currentPlayerID, COLOR_JOINLEAVE, 7, playerID);
					}
				}
				TogglePlayerSpectating(playerID, false);
				LoadPlayer(playerID);
				ReloadTextDraws(playerID);
				PVar:playerID[SPAWNAFTERLOGIN] = true;
				SendClientMessageEx(playerID, COLOR_INFO, 35);
			}
		}
		else
		{
			SendClientMessageEx(playerID, COLOR_ERROR, 36);
			ShowDialog:Login(playerID);
		}
	}
	else
	{
		SendClientMessageEx(playerID, COLOR_INFO, 37);
		Kick(playerID);
	}
	return true;
}