<?

@mysql_connect() or die( "Unable to connect to database server");
@mysql_select_db(swtprices) or die( "Unable to select database");
		
$token = strtok($_POST["query"]," ");
$query = "SELECT * FROM  `games`";
if( $token !== false ){
	$query .= " WHERE ";
	$query .= "(`name` LIKE \"%$token%\"";
	$token = strtok(" ");
}

while ($token !== false) {
	$query .= " OR `name` LIKE \"%$token%\"";
	$token = strtok(" ");
}

$query .= ") AND `appid` IS NOT NULL LIMIT 0 , 30";

$result = mysql_query($query);

while ($row = mysql_fetch_array($result,MYSQL_ASSOC)) {
	echo "<a class='selection1' id='" . $row["appid"] . "' onclick='add_to_wants(this)'>" . $row["name"] . "</a>";
}


mysql_close();
?>