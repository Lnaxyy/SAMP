ShowDialog:BuyHouse(playerID, Float:price)
{
	FormatNew:title[64](LanguageString(playerID, 236), GetMySQLValue("properties", "name", "id", val2str(PVar:playerID[DIALOGPROPERTYID])));
	FormatNew:string[100](LanguageString(playerID, 235), FormatMoney(price));
	CreateDialog(playerID, "BuyHouse", DIALOG_STYLE_MSGBOX, title, string, LanguageString(playerID, 237), LanguageString(playerID, 165));
}

DialogResponse:BuyHouse(playerID, response, listItem, inputText[])
{
	if (response)
	{
		new buildingID = PVar:playerID[DIALOGPROPERTYID];
		new Float:price = floatstr(GetMySQLValue("properties", "price", "id", val2str(buildingID)));
		if (price <= PVar:playerID[BANKMONEY])
		{
			TogglePlayerControllable(playerID, false);
			PlayAudioStreamForPlayer(playerID, "http://audio.grgserver.net/mission_complete2.ogg");
			MySQLQuery("SELECT * FROM `properties` LEFT JOIN `pickups` ON `properties`.`pickupId` = `pickups`.`id` WHERE `properties`.`id` = '%d'", buildingID);
			mysql_store_result();
			SetPlayerCameraPos(playerID, floatstr(GetMySQLField("cameraPosX")), floatstr(GetMySQLField("cameraPosY")), floatstr(GetMySQLField("cameraPosZ")));
			SetPlayerCameraLookAt(playerID, floatstr(GetMySQLField("posX")), floatstr(GetMySQLField("posY")), floatstr(GetMySQLField("posZ")));
			mysql_free_result();
			GameTextForPlayer(playerID, LanguageString(playerID, 28), 8000, 1);
			MySQLQuery("UPDATE `properties` SET `ownerUserId` = '%d' WHERE `id` = '%d'", PVar:playerID[USERID], buildingID);
			GivePlayerMoneyEx(playerID, MONEY_BANK, -price);
			ReloadPickup(strval(GetMySQLValue("properties", "pickupId", "id", val2str(buildingID))));
			SetTimerEx("Timer_ScreenFader", 8000, false, "ddddddd", playerID, 0, 0, 0, 5, 0, FADE_PROPERTYBOUGHT_BLACK);
		}
		else
		{
			SendClientMessageEx(playerID, COLOR_ERROR, 29, price, PVar:playerID[BANKMONEY]);
		}
	}
}