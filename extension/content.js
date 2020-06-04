var editBtn = document.getElementById("btnEdit");
var setBtn = document.getElementById("btnSet");
var directoryBox = document.getElementById("directoryBox");

setBtn.disabled = true;
setBtn.onclick = setDirectory;
editBtn.onclick = onEdit;
function onEdit()
{
	directoryBox.disabled = false;
	directoryBox.focus();
	setBtn.disabled = false;;	
}
function setDirectory() {

        var theValue = directoryBox.value;

        if (!theValue) {
          window.alert("Error: No value specified");
          return;
        }

		localStorage["directory"] = theValue;

		getDirectory();

		chrome.extension.sendMessage({'updateContextMenu':true});
		
		setBtn.disabled = true;
	
	window.open(window.location, '_self').close();
}

function getDirectory()
{
	directoryBox.disabled = false;
	if(!localStorage["directory"])
		directoryBox.value = "";
	else
		directoryBox.value = localStorage["directory"];
	
	directoryBox.disabled = true;
}

getDirectory();
