<?
require("include/template.php");
if( is_auth() ){

	if( isset($_POST["bid"]) && isset($_POST["tid"]) && isset($_POST["tdate"]) ){
		connect_db();
		$result = mysql_query("SELECT * FROM `steam_trade_offers` WHERE `bidkey` = '" . mysql_real_escape_string($_POST["bid"]) . "' AND `hashkey` = '" . mysql_real_escape_string($_POST["tid"]) . "' AND `created` = '" . mysql_real_escape_string($_POST["tdate"]) . "' LIMIT 0,1;" );
		//echo "SELECT * FROM `steam_trade_offers` WHERE `bidkey` = '" . mysql_real_escape_string($_POST["bid"]) . "' AND `hashkey` = '" . mysql_real_escape_string($_POST["tid"]) . "' AND `created` = '" . mysql_real_escape_string($_POST["tdate"]) . "' LIMIT 0,1;" . "<br/>";
		if( $row = mysql_fetch_array($result,MYSQL_ASSOC) ){
			$receiver = $row["bidder"];
		}
		else{
			mysql_close();
			header("Location: /compose.html?bid=" . $_POST["bid"] . "&tid=" . $_POST["tid"] . "&tdate=" . $_POST["tdate"] . "&title=" . $_POST["title"] . "&body=" . $_POST["body"] );
		}
		mysql_close();
	}
	else{
		$receiver = mysql_real_escape_string($_POST["receipent"]);
	}

	//$title = mysql_real_escape_string($_POST["title"]);
	
	$title = sql_filter($_POST["title"],50);
	$body = sql_filter($_POST["body"],10000000);

	connect_db();
	$query = "INSERT INTO  `swtprices`.`message` ( `sender` , `receiver` , `subject` , `body` ) VALUES ( '" . mysql_real_escape_string($_SESSION["steam64"]) . "',  '" . $receiver . "', '" . $title . "',  '" . $body . "' );";
	mysql_query($query);
	mysql_close();
	
	if( isset($_POST["bid"]) && isset($_POST["tid"]) && isset($_POST["tdate"]) ){
		header("Location: /offer.html?tid=".$_POST["tid"]."&cdate=".$_POST["tdate"]);
	}
	else{
		header("Location: /compose.html");
	}
}
else{
	header("Location: /compose.html");
}
?>