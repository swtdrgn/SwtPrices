var xmlhttp = new XMLHttpRequest();

function renew_trade( element,dt ){
	var url = 'renew_trade.php';
	var params = "dt=" + encodeURIComponent(dt);
	
	xmlhttp.open("POST", url, true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			element.parentNode.parentNode.parentNode.getElementsByTagName("h3")[0].getElementsByTagName("time")[0].innerHTML = xmlhttp.responseText;
		}
	}
	
	xmlhttp.send(params);
}

function delete_trade( element,dt ){
	var url = 'delete_trade.php';
	var params = "dt=" + encodeURIComponent(dt);
	
	xmlhttp.open("POST", url, true);
	xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			var parent = element.parentNode.parentNode.parentNode;
			parent.parentNode.removeChild(parent);
		}
	}
	
	xmlhttp.send(params);
}