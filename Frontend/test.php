<?php
	try{
		session_start();
		$xml = simplexml_load_file("http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=A8D997CC1A0F7A48DFAF4C7EA5281DB5&format=xml&steamids=" . $_SESSION["steam64"]);
		echo $xml->getName() . "<br />";
		var_dump($xml);
		echo "<br/>";
		//echo $xml->players[0]->player[0]->personaname . "<br/>";
		//$_SESSION["alias"] = (string) $xml->players[0]->player[0]->personaname[0]->getDocNamespaces();
		//$_SESSION["profile_url"] = (string) $xml->players[0]->player[0]->profileurl;
		
		//echo $xml->players[0]->player[0]->profileurl . "<br/>";
		//echo "This: " . $xml->players[0] . "<br/>";
		
		if( $xml->players[0] ){
			echo "YES TO 0!";
		}
		else{
			echo "NO TO 0!";
		}
		
		if( $xml->players[1] ){
			echo "YES TO 1!";
		}
		else{
			echo "NO TO 1!";
		}	
		
		echo "<br/><br/><br/><br/>";
		
		$xml2 = simplexml_load_file($_SESSION["profile_url"] . "/games?tab=all&xml=1");
		
		
		echo $xml2->games[0]->game[0]->appID[0]->getName();
		echo "<br/><br/><br/><br/>";
		
		$i = 0;
		foreach($xml2->games[0]->game as $game_iterator){
			if( $game_iterator->appID[0] == 99900 ){
				echo "GOT IT!";
				break;
			}
		}
		
		var_dump($_SESSION);
	}
	catch (Exception $e) {
		echo "Oh, look an error!";
	}
?>