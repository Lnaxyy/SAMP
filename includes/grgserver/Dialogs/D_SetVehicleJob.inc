ShowDialog:SetVehicleJob(playerID)
{
	new jobs[2048];
	MySQLQuery("SELECT `id`, `name` FROM `jobs` ORDER BY `name` ASC");
	mysql_store_result();
	while (FetchMySQLRow())
	{
		AddLine:jobs("%s\t%s", GetMySQLField("id"), GetMySQLField("name"));
	}
	mysql_free_result();
	CreateDialog(playerID, "SetVehicleJob", DIALOG_STYLE_LIST, LanguageString(playerID, StringID:610("Select job")), jobs, "OK", LanguageString(playerID, StringID:165("Cancel")));
}

DialogResponse:SetVehicleJob(playerID, response, listItem, inputText[])
{
	if (!response)
	{
		return true;
	}
	new vehicleID = GetPlayerVehicleID(playerID);
	if (!vehicleID)
	{
		SendClientMessageEx(playerID, COLOR_ERROR, StringID:146("You are not in a vehicle!"));
		return true;
	}
	new jobID;
	new jobName[STRINGLENGTH_JOBNAME];
	sscanf(inputText, "p<\t>ds[" #STRINGLENGTH_JOBNAME "]", jobID, jobName);
	VehicleProperty:vehicleID[JOBID] = jobID;
	return true;
}