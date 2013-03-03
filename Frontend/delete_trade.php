<?
require("include/template.php");
if( is_auth() ){
	connect_db();
	$query = "DELETE FROM `swtprices`.`steam_trade_offers`";
	$query .= " WHERE `steam_trade_offers`.`creator` = '" . mysql_real_escape_string($_SESSION["steam64"]) . "' AND `steam_trade_offers`.`created` = '" . mysql_real_escape_string($_POST["dt"]) . "';";
	mysql_query($query);
	$query = "DELETE FROM `swtprices`.`steam_trade`";
	$query .= " WHERE `steam_trade`.`creator` = '" . mysql_real_escape_string($_SESSION["steam64"]) . "' AND `steam_trade`.`created` = '" . mysql_real_escape_string($_POST["dt"]) . "';";
	mysql_query($query);
	mysql_close();
}
?>