<?
require("include/template.php");
if( is_auth() ){
	connect_db();
	$query = "DELETE FROM `swtprices`.`steam_trade_offers`";
	$query .= " WHERE `steam_trade_offers`.`hashkey` = '" . mysql_real_escape_string($_GET["tid"]) . "' AND `steam_trade_offers`.`bidder` = '" . mysql_real_escape_string($_SESSION["steam64"]) . "' AND `steam_trade_offers`.`created` = '" . mysql_real_escape_string($_GET["dt"]) . "';";
	mysql_query($query);
	mysql_close();
}

header("Location: /myoffer.html");
?>