<?
require("include/template.php");
if( is_auth() && isset($_POST["cdate"]) && isset($_POST["tid"]) ){
	
	$note = sql_filter($_POST["note"],10000);
	
	$query = "";
	
	connect_db();
	
	$query = "DELETE FROM `swtprices`.`steam_trade_offers`";
	$query .= " WHERE `steam_trade_offers`.`bidder` = '" . $_SESSION["steam64"] . "' AND `steam_trade_offers`.`created` = '" . mysql_real_escape_string($_POST["cdate"]) . "' AND `steam_trade_offers`.`hashkey` = '" . mysql_real_escape_string($_POST["tid"])  . "';";
	mysql_query($query);
	//echo $query;
	
	
	$result = mysql_query("SELECT * FROM `steam_trade` WHERE `hashkey` = '" . mysql_real_escape_string($_POST["tid"]) . "' AND `created` = '" . mysql_real_escape_string($_POST["cdate"]) . "';" );
	
	$json_output = array('success' => false);
	
	if($row = mysql_fetch_array($result,MYSQL_ASSOC)){
		$poster = $row["creator"];
		$hashkey = $row["hashkey"];
		$tdate = $row["created"];
		$exdate = date("Y-m-d",strtotime('+3 days'));
		
		for( $i=0;$i<$_POST["tradesize"];$i++ ){
			$query = "";
			if( strcmp($_POST["tradetype" . $i],"Gift") == 0 ){
				$item_type=1;
			}
			else{
				$item_type=0;
			}
			
			$query .= "INSERT INTO  `swtprices`.`steam_trade_offers` (`bidder` ,`bidkey` , `created` ,`expiration` ,`type` ,`item_name` ,`item_url` ,`item_type`,`creator` ,`hashkey`) VALUES (";
			$query .= "'" . $_SESSION["steam64"] . "','" . urlencode(sha1($_SESSION["steam64"])) . "','" . $tdate . "','" .$exdate . "','0',  '" . urlencode($_POST["trade".$i]) . "',  '" . urlencode($_POST["tradeimg".$i]) . "',  '" . $item_type . "','" . $poster . "','" . $hashkey . "'";
			$query .= ");";
			//echo $query . "<br/><br/>";
			mysql_query($query);

		}
		
		for( $i=0;$i<$_POST["wantsize"];$i++ ){
			$query = "";
			if( strcmp($_POST["wanttype" . $i],"Gift") == 0 ){
				$item_type=1;
			}
			else{
				$item_type=0;
			}
			
			$query .= "INSERT INTO  `swtprices`.`steam_trade_offers` (`bidder` ,`bidkey` ,`created` ,`expiration` ,`type` ,`item_name` ,`item_url` ,`item_type` ,`creator` ,`hashkey`) VALUES (";
			$query .= "'" . $_SESSION["steam64"] . "','" . urlencode(sha1($_SESSION["steam64"])) . "','" . $tdate . "','" . $exdate . "','1',  '" . urlencode($_POST["want".$i]) . "',  '" . urlencode($_POST["wantimg".$i]) . "',  '" . $item_type . "','" . $poster . "','" . $hashkey . "'";
			$query .= ");";
			//echo $query . "<br/><br/>";
			mysql_query($query);

		}
		
		$query = "INSERT INTO  `swtprices`.`steam_trade_offers` (`bidder` ,`bidkey` ,`created` ,`expiration` ,`type` ,`note` ,`creator` ,`hashkey`) VALUES (";
		$query .= "'" . $_SESSION["steam64"] . "','" . urlencode(sha1($_SESSION["steam64"])) . "','" . $tdate . "','" . $exdate . "','2',  '" . urlencode($_POST["note"]) . "','" . $poster . "','" . $hashkey . "'";
		$query .= ");";
		//echo $query . "<br/><br/>";
		mysql_query($query);
		
		//mysql_query($query);

		$json_output["success"] = true;
	}
	mysql_close();
	
	echo json_encode($json_output);
}


?>