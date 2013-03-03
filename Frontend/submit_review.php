<?

require("include/template.php");

if(!isset($_POST["submit"]) || strcmp($_POST["submit"],"Rate It") != 0 || !is_auth() ){
	/*if( isset($_SESSION["steam64"]) ){
		echo '$_SESSION["steam64"] is set';
	}
	else{
		echo '$_SESSION["steam64"] is not set';
	}*/
	
	header("Location: /game.html?appid=" . $_POST["appid"] );
}
else{
	$has_game = false;
	//echo $_SESSION["steam64"] . "<br/>" . "http://steamcommunity.com/id/" . $_SESSION["steam64"] . "/games?tab=all&xml=1" . "<br/><br/>";
	//$xml = simplexml_load_file( "http://steamcommunity.com/profiles/" . $_SESSION["steam64"] . "/games?tab=all&xml=1" );
	$file = @file_get_contents( "http://steamcommunity.com/profiles/" . $_SESSION["steam64"] . "/games?tab=all&xml=1" );
	$i=0;
	for($i=0; !$file && $i<10 ;$i++){
		$file = @file_get_contents( "http://steamcommunity.com/profiles/" . $_SESSION["steam64"] . "/games?tab=all&xml=1" );
	}
	if( $i==10 ){
		header("Location: /game.html?appid=" . $_POST["appid"] );
	}
	$xml = simplexml_load_string($file);

	foreach($xml->games[0]->game as $game_iterator){
		if( $game_iterator->appID[0] == $_POST["appid"] ){
			$hours = (float) $game_iterator->hoursOnRecord[0];
			$has_game = true;
			break;
		}
	}

	if($has_game && $hours > 10){
		if( !isset($_POST["dp"]) ){
			$dp = "NULL";
		} else{
			$dp = "'" . $_POST["dp"] . "'";
		}
		
		$review = sql_filter($_POST["review"],1000000);
		$name = sql_filter($_POST["name"],50);
		
		if( is_numeric($_POST["score"]) ){
			$rating = $_POST["score"];
		}
		else{
			$rating = 0;
		}
		
		
		$query = "SELECT * FROM  `game_reviews` WHERE `appid` = " . $_POST["appid"] . " AND `steamid` LIKE " . $_SESSION["steam64"] . " LIMIT 0 , 1";
		
		connect_db();
		$result = mysql_query($query);
		
		if ($row = mysql_fetch_array($result,MYSQL_ASSOC)) {
			$previous_score = $row["score"];
			$increase_count = 0;
		}
		else{
			$previous_score = 0;
			$increase_count = 1;
		}
		
		$difference = $rating - $previous_score;

		$query = "REPLACE INTO  `swtprices`.`game_reviews` (`appid` ,`dp` ,`name` ,`steamid` ,`playtime` ,`score` ,`date` ,`review`) VALUES (" . $_POST["appid"] . "," . $dp . ",'"  . $name . "','" . $_SESSION["steam64"] . "'," . $hours . "," . $rating . ",'" . date("Y-m-d") . "',\"" . $review . "\");";
		mysql_query($query);
		//echo "<br/><br/>" . $query . "<br/><br/>";
		
		$query = "UPDATE  `swtprices`.`games` SET  `score` =  `score` + " . $difference . " , `ratingcount` =  `ratingcount` + " . $increase_count . " WHERE  `games`.`appid` = " . $_POST["appid"];
		//echo "<br/><br/>" . $query . "<br/><br/>";
		mysql_query($query);

		
		mysql_close();
		
		
	}

	header("Location: /game.html?appid=" . $_POST["appid"] );

}



?>