
var fileIDs =new Map();
var NoteItId = chrome.contextMenus.create({title: "Note it", contexts:["selection"]});
var RefreshListid = chrome.contextMenus.create({title: "Refresh List",contexts:["selection"],parentId: NoteItId,onclick : GetAllFiles});
var SeperatorId = chrome.contextMenus.create({ "type": 'separator',contexts:["selection"],parentId: NoteItId});

function onFileClickListner(info,tab)
{	
	var filename = fileIDs.get(info.menuItemId);
	//window.alert(filename);
	if(filename === "Write a new file")
		filename = "new file";
		//window.alert(filename);
	var port = chrome.runtime.connectNative('com.noteit.native_messaging_host');
	port.onMessage.addListener(function(message,sender){
		
			//window.alert(message.addedtext)
				if(message.addedtext === "true")
				 {
					 var opt = 	{
							  type: "basic",
							  title: "Note it",
							  message: "Note added to " + filename,
							  iconUrl: "src/note48.png"
							}
					chrome.notifications.create("", opt);
				 }
				else
				{
					var opt = 	{
								  type: "basic",
								  title: "Note it",
								  message: "Failed to add note to "+filename,
								  iconUrl: "src/note48.png"
								}
								chrome.notifications.create("", opt);
				}
	});
	port.onDisconnect.addListener(function() {
		//window.alert("Disconnected");
		port = null;
	});
	port.postMessage({"request":"addText","filepath":localStorage["directory"],"filename":fileIDs.get(info.menuItemId),"text":info.selectionText});
}

function GetAllFiles(){
	
	var port = chrome.runtime.connectNative('com.noteit.native_messaging_host');
	
	port.onMessage.addListener(function(message,sender)
	{
		for (const [key, value] of fileIDs.entries()) 
		{
			chrome.contextMenus.remove(key);
		}
		fileIDs.clear();
		var jsonData = message.files;
		//window.alert(jsonData);
		for (var i = 0; i < jsonData.length; i++) 
		{
			var id = chrome.contextMenus.create({
				title: jsonData[i],
				contexts:["selection"],
				parentId: NoteItId,
				onclick : onFileClickListner
			});
			
			fileIDs.set(id,jsonData[i]);
		}
	
		var id = chrome.contextMenus.create({title: "Write a new file",contexts:["selection"],parentId: NoteItId,onclick : onFileClickListner});
		fileIDs.set(id,"Write a new file");
	});
	
	port.onDisconnect.addListener(function() {
		port = null;
	});
	
	port.postMessage({ "request":"GetFileList","directory":localStorage["directory"]});
}

chrome.extension.onMessage.addListener(function(request, sender, sendresponse) {
	
    if (request.updateContextMenu) {
           GetAllFiles();
    }
});


GetAllFiles();
