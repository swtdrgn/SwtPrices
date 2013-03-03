<?
require("include/template.php");
$new_date = date("Y-m-d",strtotime('+3 days'));
if( is_auth() ){
	connect_db();
	$query = "UPDATE  `swtprices`.`steam_trade` SET  `expiration` =  '" . $new_date . "' WHERE  `steam_trade`.`creator` =  '" . $_SESSION["steam64"]  . "' AND  `steam_trade`.`created` =  '" . $_POST["dt"] . "';";
	mysql_query($query);
	mysql_close();
}
echo $new_date;
?>