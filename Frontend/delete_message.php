<?
require("include/template.php");
if( is_auth() ){
	connect_db();
	$query = "DELETE FROM `swtprices`.`message`";
	$query .= " WHERE `message`.`sender` = '" . mysql_real_escape_string($_POST["sender"]) . "' AND `message`.`receiver` = '" . mysql_real_escape_string($_SESSION["steam64"]) . "' AND `message`.`time` = '" . mysql_real_escape_string($_POST["time"]) . "' AND `message`.`time` = '" . mysql_real_escape_string($_POST["time"]) . "';";
	mysql_query($query);
	mysql_close();
}
?>