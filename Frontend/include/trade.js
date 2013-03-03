var xmlhttp = new XMLHttpRequest();

function request_inventory(steam64){
	
}

function create_item(element,name,icon_url,item_type){
	element.className = "selection2";
	element.onclick=0;
	
	var item=document.createElement("div");
	item.className = "game_result";
	item.id = name;
	item.icon_url = icon_url;
	item.item_type = item_type;
	item.innerHTML = '<img src="http://cdn.steamcommunity.com/economy/image/' + icon_url + '" />';
	item.onclick = function(){
		element.className = "selection1";
		element.onclick=function(){
			create_item(element,name,icon_url);
		}
		item.parentNode.removeChild(item);
	}
	document.getElementById("trade_table").appendChild(item);
}

function continue_to_want_offers(button){
	button.parentNode.style.display = "none";
	document.getElementById("want").style.display = "inherit";
}

function back_to_trade_offers(button){
	button.parentNode.parentNode.parentNode.style.display = "none";
	document.getElementById("have").style.display = "inherit";
}

function search_item( query ){
	var url = 'find_steam_games.php';
	var params = "query=" + query;
	
	xmlhttp.open("POST", url, true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	xmlhttp.setRequestHeader("Content-length", params.length);
	xmlhttp.setRequestHeader("Connection", "close");

	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("search_list").innerHTML = xmlhttp.responseText;
		}
	}
	
	xmlhttp.send(params);
}

function add_to_wants( element ){
	var item=document.createElement("div");
	item.className = "game_result";
	item.id = element.innerHTML;
	item.icon_url = "http://cdn.steampowered.com/v/gfx/apps/" + element.id + "/header_292x136.jpg";
	item.item_type = "Gift";
	item.innerHTML = '<img src="http://cdn.steampowered.com/v/gfx/apps/' + element.id + '/header_292x136.jpg" />';
	item.onclick = function(){
		item.parentNode.removeChild(item);
	}
	document.getElementById("want_table").appendChild(item);
}

function add_offer_to_wants( element ){
	var item=document.createElement("div");
	item.className = "game_result";
	item.id = element.getAttribute("gamename");
	item.icon_url = element.getAttribute("icon_url");
	item.item_type = "Gift";
	item.innerHTML = '<img src="http://cdn.steamcommunity.com/economy/image/' + item.icon_url + '" />';
	element.className = "selection2";
	element.onclick = 0;
	item.onclick = function(){
		element.className = "selection1";
		item.parentNode.removeChild(item);
		element.onclick=function(){
			add_offer_to_wants(element);
		}
	}
	document.getElementById("want_table").appendChild(item);
}

function post_trade(button){
	var url = 'post_trade.php';
	var current,global=0;
	var element = document.getElementById("trade_table");
	var games = element.getElementsByTagName("div");
	var size = games.length;
	
	
	var params = "note=" + encodeURIComponent(document.getElementById("note").value.substring(0,1000));
	params += "&tradesize=" + size;
	for( current=0;current<size;current++ ){
		params += "&trade" + current + "=" + games[current].id;
		params += "&tradeimg" + current + "=" + encodeURIComponent(games[current].icon_url);
		params += "&tradetype" + current + "=" + encodeURIComponent(games[current].item_type);
	}
	
	element = document.getElementById("want_table");
	games = element.getElementsByTagName("div");
	size = games.length;
	
	params += "&wantsize=" + size;
	
	for( current=0;current<size;current++ ){
		params += "&want" + current + "=" + encodeURIComponent(games[current].id);
		params += "&wantimg" + current + "=" + encodeURIComponent(games[current].icon_url);
		params += "&wanttype" + current + "=" + encodeURIComponent(games[current].item_type);
	}

	xmlhttp.open("POST", url, true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	xmlhttp.setRequestHeader("Content-length", params.length);
	xmlhttp.setRequestHeader("Connection", "close");

	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			//var item = document.getElementById("trade_table");
			//item.innerHTML = xmlhttp.responseText;
			//alert( xmlhttp.responseText );
			var success = JSON.parse(xmlhttp.responseText);
			if( success["success"] ){
				self.location="/mytrade.html";
			}
			else{
				alert("Failed to post trade to server. Please try again.\n\nIf problem persist, please bring this up on the forums.");
			}
		}
	}
	
	xmlhttp.send(params);
}

function post_offer(tid,cdate){
	var url = 'post_offer.php';
	var current,global=0;
	var element = document.getElementById("trade_table");
	var games = element.getElementsByTagName("div");
	var size = games.length;
	
	
	var params = "note=" + encodeURIComponent(document.getElementById("note").value.substring(0,1000));
	params += "&tradesize=" + size;
	for( current=0;current<size;current++ ){
		params += "&trade" + current + "=" + games[current].id;
		params += "&tradeimg" + current + "=" + encodeURIComponent(games[current].icon_url);
		params += "&tradetype" + current + "=" + encodeURIComponent(games[current].item_type);
	}
	
	element = document.getElementById("want_table");
	games = element.getElementsByTagName("div");
	size = games.length;
	
	params += "&wantsize=" + size;
	
	for( current=0;current<size;current++ ){
		params += "&want" + current + "=" + encodeURIComponent(games[current].id);
		params += "&wantimg" + current + "=" + encodeURIComponent(games[current].icon_url);
		params += "&wanttype" + current + "=" + encodeURIComponent(games[current].item_type);
	}
	
	params += "&tid=" + tid + "&cdate=" + cdate;

	xmlhttp.open("POST", url, true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	xmlhttp.setRequestHeader("Content-length", params.length);
	xmlhttp.setRequestHeader("Connection", "close");

	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			var success = JSON.parse(xmlhttp.responseText);
			if( success["success"] ){
				self.location="/offer.html?tid=" + tid + "&cdate=" + cdate;
			}
			else{
				alert("Server failed to submit offer. Please try again.\n\nIf problem persist, please bring this up on the forums.");
			}
		}
	}
	xmlhttp.send(params);
}